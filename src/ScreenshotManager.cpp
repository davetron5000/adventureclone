#include "ScreenshotManager.h"
#include <assert.h>
#include <Logger.h>

BOOL ScreenshotManager::init(char *fileBase)
{
	itsFileBase = strdup(fileBase);
	itsNextIndex = 0;
	return TRUE;
}

// Writes this surface out to a file.  This will get the height,
// wdith, palette, etc. from the surface.
BOOL ScreenshotManager::writeScreenShot(LPDIRECTDRAWSURFACE surface)
{
	assert(itsFileBase != NULL);

	char filename[1024];
	RGBQUAD rgbQuadPalette[256];


	LPDIRECTDRAWPALETTE palette;

	// now, create our palette
	if (SUCCEEDED(surface->GetPalette(&palette)))
	{
		PALETTEENTRY entries[256];
		palette->GetEntries(0,0,256,entries);
		for (int i=0;i<256;i++)
		{
			rgbQuadPalette[i].rgbBlue = entries[i].peBlue;
			rgbQuadPalette[i].rgbGreen = entries[i].peGreen;
			rgbQuadPalette[i].rgbRed = entries[i].peRed;
			rgbQuadPalette[i].rgbReserved = 0;
		}
		DDSURFACEDESC surfaceDesc;
		memset(&surfaceDesc,0,sizeof(DDSURFACEDESC));
		surfaceDesc.dwSize = sizeof(surfaceDesc);

		if (SUCCEEDED( surface->Lock(NULL,&surfaceDesc,DDLOCK_WAIT,NULL)) )
		{
			// ugh, have to copy it, because the surface might have extra junk in it.
			static BYTE data[640 * 480];

			int width = surfaceDesc.dwWidth;
			int height = surfaceDesc.dwHeight;

			BYTE *source = (BYTE *)surfaceDesc.lpSurface;
			BYTE *destination = data;

			for (int i=0;i<height;i++)
			{
				memcpy(destination,source,width);
				source      += surfaceDesc.lPitch;
				destination += width;
			}
			BOOL wroteFile;
			int loopExecutionCount = 0;
			do
			{
				sprintf(filename,"%s%03d.bmp",itsFileBase,itsNextIndex);
				wroteFile = itsImageWriter.write(filename,data,640,480,rgbQuadPalette);
				loopExecutionCount++;
				itsNextIndex++;
			}
			while (!wroteFile && (loopExecutionCount < 100));
			surface->Unlock(NULL);
			if (loopExecutionCount >= 100)
			{
				Logger::error("While trying to write a screenshot, we attempted 100 or more times and were not successful.  There could be a larger problem");
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		else
		{
			Logger::error("Couldn't lock surface to take a screenshot");
		}
	}
	else
	{
		Logger::error("Couldn't get the palette from the surface to write a screenshot!");
	}
	return FALSE;
}

// release resources
BOOL ScreenshotManager::release()
{
	free(itsFileBase);
	itsFileBase = NULL;
	return TRUE;
}
