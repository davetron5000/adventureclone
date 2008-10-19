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
#include "ScreenImage.h"
#include "Logger.h"

#ifndef __RECTANGULARIMAGE_H__

#define __RECTANGULARIMAGE_H__

// RectangularImage is a special type of ScreenImage that is just a solid colored box.  Therefore, there
// is no need to initialize it from a file. 
class RectangularImage : public ScreenImage
{
protected:
	int itsColor;

	DDBLTFX itsBltFx;

public:
	void changeColor(int newColor) { itsColor = newColor; }

	inline int getColor() { return itsColor; }

	inline int getColor(int x, int y) { return getColor(); }

	virtual BOOL draw(LPDIRECTDRAWSURFACE surface, int x, int y);

	virtual BOOL restore() { return TRUE; }

	virtual BOOL release() { return TRUE; }

    BOOL init(LPDIRECTDRAW directDrawObject, 
		      ULONG width, 
			  ULONG height, 
			  BYTE color, 
			  LPDIRECTDRAWPALETTE palette, 
			  DDCOLORKEY *colorKey=NULL);

	// this is a no-op, because we never need to actually do this.  We don't have
	// a surface like ScreenImage does.
	virtual BOOL copyDataToSurface(LPDIRECTDRAWSURFACE surface) { return TRUE; }

};
#endif // __RECTANGULARIMAGE_H__