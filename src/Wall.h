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
#include "GameObject.h"
#include "WallImage.h"

#ifndef __WALL_H__
#define __WALL_H__


// A Wall is just a basic GameObject that can be initialized with a width/height and color, since
// most of the time in Adventure, a wall is a solid-colored block.  
class Wall : public GameObject
{
protected :
	int itsNormalColor;

    // this will hold our WallImage, so that we can not rely on the super
    // classes' usage of itsCurrentImageToDraw so much
    WallImage *itsWallImage;

	// if TRUE, this wall will change the color of an object brought into the
	// room.
	BOOL willChangeToObjectColor;

	UINT itsChangeColorObjectCount;

public:

	static const int theWallTolerance;

    // We set this to NULL so that we can tell if the user of this class used our init
    // or the superclass's
    inline Wall() : GameObject(FALSE,FALSE,FALSE,GAMEOBJECT_TYPE_WALL) { itsWallImage = NULL; }

    // A Wall is just a colored box, so all we need is it's color and size (and position), but
    // since we arne't taking a ScreenImage, we'll need the ddraw object and a palette
    BOOL init(DWORD currentTime, 
              ULONG x, 
              ULONG y,
              ULONG width, 
              ULONG height, 
              BYTE color, 
              LPDIRECTDRAW directDrawObject, 
              LPDIRECTDRAWPALETTE palette,
			  BOOL willChangeColor=FALSE,
			  UINT objectCountForColorChange=2,
			  DDCOLORKEY *colorKey=NULL);

    // Since a Wall creates its own ScreenImage, instead of taking one in init,
    // we need to make sure delete the one we created (even if the super class normally
    // does...we don't want to couple our implementation with it's)
    BOOL release();

	void setChangesColor(BOOL val=TRUE,UINT objectCountForColorChange=2) 
	{ 
		willChangeToObjectColor = val; 
		itsChangeColorObjectCount = objectCountForColorChange;
	}

	int getNormalColor() { return itsNormalColor; }

	inline void changeColor(int colorIndex) { itsWallImage->changeColor(colorIndex); }

	virtual BOOL update(DWORD currentTime);

	virtual BOOL isTouching(int x, int y);
};

#endif // __WALL_H__