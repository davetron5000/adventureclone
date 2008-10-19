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
#include "RectangularImage.h"

BOOL RectangularImage::init(LPDIRECTDRAW directDrawObject, ULONG width, ULONG height, BYTE color, 
                     LPDIRECTDRAWPALETTE palette, DDCOLORKEY *colorKey)
{
	itsData = NULL;

	itsColor = color;
    itsWidth = width;
    itsHeight = height;
	itsPalette = palette;
	itsBltFx.dwSize = sizeof(DDBLTFX);
	return TRUE;
}

BOOL RectangularImage::draw(LPDIRECTDRAWSURFACE surface, int x, int y)
{
	if (itsColor == ScreenImage::theTransparencyColor)
		return TRUE;

	itsBltFx.dwFillColor = itsColor;

	itsDestinationRect.left = x;
	itsDestinationRect.top  = y;
	itsDestinationRect.right = x + itsWidth;
	itsDestinationRect.bottom = y + itsHeight;
	return (SUCCEEDED(
		surface->Blt(&itsDestinationRect,NULL,NULL,DDBLT_COLORFILL | DDBLT_WAIT,&itsBltFx)));
}
