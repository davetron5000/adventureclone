/*
Copyright 2001 by David Copeland

This file is part of Dave's Clone of Adventure.

Dave's Clone of Adventure is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Dave's Clone of Adventure is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Dave's Clone of Adventure; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <assert.h>

#include "ScreenImage.h"
#include "Logger.h"

const int ScreenImage::theTransparencyColor = 253;

ScreenImage::ScreenImage()
{
    itsData = NULL;
    itsDataLength = 0;
    itsWidth = NULL;
    itsHeight = NULL;
    itsSurface = NULL;
    itsPalette = NULL;
    itsBltFlags = 0;
}

BOOL ScreenImage::release()
{
    if (itsData != NULL) 
        delete[] itsData;
    if (itsPalette != NULL)
        itsPalette->Release();
    if (itsSurface != NULL)
        itsSurface->Release();
    return TRUE;
}

BOOL ScreenImage::draw(LPDIRECTDRAWSURFACE surface, int x, int y)
{
    itsDestinationRect.left = x;
    itsDestinationRect.top  = y;
    itsDestinationRect.right = x + itsWidth;
    itsDestinationRect.bottom = y + itsHeight;

    // Using regular Blt so that clipping will occur if there's a clipper
    return SUCCEEDED(surface->Blt(&itsDestinationRect,itsSurface,NULL,itsBltFlags,NULL));
}

BOOL ScreenImage::restore()
{
    if ( SUCCEEDED(itsSurface->Restore()) )
    {
        if (copyDataToSurface(itsSurface))
        {
            return TRUE;
        }
        else
        {
            Logger::error("Couldn't copy data back to surface after restore!");
            return FALSE;
        }
    }
    else
    {
        Logger::error("Couldn't restore surface!");
        return FALSE;
    }
}

BOOL ScreenImage::init(LPDIRECTDRAW directDrawObject, 
					   ImageFileReader *reader, 
					   char *filename, 
					   DDCOLORKEY *colorKey,
					   BOOL monoColored)
{
    BOOL returnValue = FALSE;

    assert(reader != NULL);
    assert(filename != NULL);

	isMonoColored = monoColored;

    RGBQUAD *palette;
    ULONG paletteLength;

    if (reader->load(filename,&itsWidth,&itsHeight,&itsData,&itsDataLength,&palette,&paletteLength))
    {
        itsSourceRect.left = 0;
        itsSourceRect.top  = 0;
        itsSourceRect.right = itsWidth;
        itsSourceRect.bottom = itsHeight;
        
        // Only supporting 8-bit indexed color (is there any other kind?)
        if ( (palette != NULL) && (paletteLength == 256) )
        {
            PALETTEENTRY pe[256];
            for (int i=0;i<256;i++)
            {
                pe[i].peBlue  = palette[i].rgbBlue;
                pe[i].peGreen = palette[i].rgbGreen;
                pe[i].peRed   = palette[i].rgbRed;
            }

            if (SUCCEEDED(directDrawObject->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_INITIALIZE,
                                                          pe,&itsPalette,NULL)))
            {
                delete [] palette;
                returnValue = TRUE;
            }
            else
            {
                itsPalette = NULL;
            }
        }
        else
        {
            returnValue = TRUE;
        }

        if (returnValue == TRUE)
        {
            returnValue = setupSurface(directDrawObject,colorKey);
			itsCurrentColor = -1;
			if (isMonoColored && (returnValue == TRUE) && (itsData != NULL))
			{
				itsCurrentColor = theTransparencyColor;
				for (UINT i=0;i<itsDataLength;i++)
				{
					if (itsData[i] != theTransparencyColor)
					{
						itsCurrentColor = itsData[i];
						break;
					}
				}
			}
        }
    }
    else
    {
        Logger::error("Failed to read %s to init ScreenImage",filename);
    }
    return returnValue;
}

BOOL ScreenImage::setupSurface(LPDIRECTDRAW directDrawObject, DDCOLORKEY *colorKey)
{
    BOOL returnValue = FALSE;
    DDSURFACEDESC surfaceDesc;
	DWORD timer = timeGetTime();
	DWORD lastTime = timer;

    surfaceDesc.dwSize = sizeof(surfaceDesc);
    surfaceDesc.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
    surfaceDesc.ddsCaps.dwCaps = DEFAULT_DDSCAPS;
    surfaceDesc.dwHeight = itsHeight;
    surfaceDesc.dwWidth = itsWidth;

    if (SUCCEEDED(directDrawObject->CreateSurface(&surfaceDesc,&itsSurface,NULL)))
    {
        itsBltFlags = DDBLT_WAIT;
        if (colorKey != NULL)
        {
            itsSurface->SetColorKey(DDCKEY_SRCBLT,colorKey);
            itsBltFlags |= DDBLT_KEYSRC;
        }
        else
        {
            // empty, our blt flags are "all good"
        }
        if (itsPalette != NULL)
        {
            itsSurface->SetPalette(itsPalette);
        }
        returnValue = copyDataToSurface(itsSurface);
    }
    else
    {
        Logger::error("Couldn't create surface for ScreenImage!");
    }
    return returnValue;
}

void ScreenImage::changeColor(int newColor)
{ 
	if (isMonoColored)
	{
		itsCurrentColor = newColor; 

		if (itsCurrentColor != theTransparencyColor)
		{
			for (UINT i=0;i<itsDataLength;i++)
			{
				if (itsData[i] != theTransparencyColor)
					itsData[i] = itsCurrentColor;
			}
		}
		copyDataToSurface(itsSurface);
	}
}

BOOL ScreenImage::copyDataToSurface(LPDIRECTDRAWSURFACE surface)
{
    DDSURFACEDESC surfaceDesc;

    memset(&surfaceDesc,0,sizeof(DDSURFACEDESC));
    surfaceDesc.dwSize = sizeof(surfaceDesc);
    
	HRESULT result;
    if (SUCCEEDED( result = surface->Lock(NULL,&surfaceDesc,DDLOCK_WAIT,NULL)) )
    {
        int height = itsHeight;
        int width = itsWidth;
        BYTE *destination = (BYTE *)surfaceDesc.lpSurface;
        BYTE *source = itsData;

        int sourceWidth = (width > surfaceDesc.lPitch) ? 
                           surfaceDesc.lPitch : 
                           width;


        if (isMonoColored && (itsCurrentColor == theTransparencyColor))
		{
			for (int i=0;i<height;i++)
			{
				memset(destination,theTransparencyColor,width);
				destination += surfaceDesc.lPitch;
			}
		}
		else
		{
			for (int i=0;i<height;i++)
			{
				memcpy(destination,source,sourceWidth);
				destination += surfaceDesc.lPitch;
				source      += sourceWidth;
			}
		}
        return SUCCEEDED(surface->Unlock(NULL));
    }
    else
    {
        Logger::error("Couldn't lock surface in ScreenImage to copy data to surface (%d)",result);
        return FALSE;
    }
}
