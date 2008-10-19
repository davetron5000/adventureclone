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
#include <windows.h>
#include <ddraw.h>

#include "ImageFileReader.h"

#ifndef __ScreenImage_h__
#define __ScreenImage_h__


#define DEFAULT_DDSCAPS DDSCAPS_OFFSCREENPLAIN

// A ScreenImage is essentially a sprite, but is also a background, too.  this is something
// that should be drawn onto the screen during the game.
class ScreenImage
{
protected:
    BYTE *itsData;
    ULONG itsDataLength;
    ULONG itsWidth;
    ULONG itsHeight;

	// If TRUE, this image is made up of only two colors and as such can
	// be changed during it's life.
	BOOL isMonoColored;
	// If isMonoColored is TRUE, this is the current color of the object
	int itsCurrentColor;

	RECT itsDestinationRect;


    LPDIRECTDRAWSURFACE itsSurface;
    LPDIRECTDRAWPALETTE itsPalette;
    DWORD itsBltFlags;


    RECT itsSourceRect;

    // Helper method to setup the surface after we've read our file
    BOOL setupSurface(LPDIRECTDRAW directDrawObject, DDCOLORKEY *colorKey);

    // Helper method to copy the raw image data to the surface
    
public:

	const static int theTransparencyColor;

    ScreenImage();

    // Draw this image onto the passed-in surface starting at (x,y) on 
    // the surface
    virtual BOOL draw(LPDIRECTDRAWSURFACE surface, int x, int y);

    // restore after everything gets all screwed up from alt-tab or whatever
    virtual BOOL restore();

    // Init this screen image from the passed-in file using the passed-in reader.  If colorKey
    // is non-null, use that for transparency blitting.
    virtual BOOL init(LPDIRECTDRAW directDrawObject, 
              ImageFileReader *reader, 
              char *filename, 
              DDCOLORKEY *colorKey=NULL,
			  BOOL monoColored=TRUE);

    // Copies the data of this image to the specified surface.  Used internally to
    // restore/init the encapsulated surface for blitting later.  Used externally
    // to copy this image's data to another surface, if need be.
    virtual BOOL copyDataToSurface(LPDIRECTDRAWSURFACE surface);
    
    // Release resources
    virtual BOOL release();

	// If this image is mono-colored, this will change it's color.
	virtual void changeColor(int newColor);

    inline ULONG getWidth()  { return itsWidth; }
    inline ULONG getHeight() { return itsHeight; }
    inline LPDIRECTDRAWPALETTE getPalette() { return itsPalette; }

	virtual inline int getColor(int x, int y)
	{
		int index = x + (y * itsWidth);
		if ( (index >= 0) && ((UINT)index < itsDataLength) )
			return itsData[index];
		else
			return theTransparencyColor;
	}



};

#endif // __ScreenImage_h__