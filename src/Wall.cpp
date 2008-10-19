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
#include "Wall.h"
#include <Logger.h>
#include "Player.h"

const int Wall::theWallTolerance = 8;

BOOL Wall::init(DWORD currentTime, 
                ULONG x, 
                ULONG y,
                ULONG width, 
                ULONG height, 
                BYTE color, 
                LPDIRECTDRAW directDrawObject, 
                LPDIRECTDRAWPALETTE palette,
			    BOOL willChangeColor,
				UINT objectCountForColorChange,
				DDCOLORKEY *colorKey)
{
    itsWallImage = new WallImage();
	setChangesColor(willChangeColor,objectCountForColorChange);
    if (itsWallImage->init(directDrawObject,width,height,color,palette,colorKey))
    {
		itsNormalColor = itsWallImage->getColor();
        return GameObject::init(currentTime,itsWallImage,x,y);
    }
    else
    {
        Logger::error("Couldn't init wall (%d,%d) (%d,%d) [%d]",x,y,width,height,color);
    }
    return FALSE;
    
}

BOOL Wall::update(DWORD currentTime)
{
	if (willChangeToObjectColor)
	{
		changeColor(itsNormalColor);
		Room *room = getRoom();
		Player *player = (room != NULL) ? (Player *)(room->getPlayer()) : NULL;

		if ((room != NULL) && (player != NULL) && 
			(room->getObjects()->length() > (itsChangeColorObjectCount)) )
		{
			SimpleArray *objects = room->getObjects();
			int index = objects->length();
			GameObject *object = (GameObject *)(objects->elementAt(objects->length()-1));
			do
			{
				index--;
				object = (GameObject *)(objects->elementAt(index));
			} while ( (index > 0) && (object->getType() == GameObject::GAMEOBJECT_TYPE_PLAYER) );
			if (index != 0)
				changeColor(object->getPrimaryColor());
		}
	}
	return GameObject::update(currentTime);
}

BOOL Wall::isTouching(int x, int y)
{
	return GameObject::isTouching(x,y);
}

BOOL Wall::release()
{
    BOOL returnValue = TRUE;

    if (itsWallImage != NULL)
    {
        // If we used our special init and not the superclass init, 
        // we'll make sure that the ScreenImage we created is deleted
        // by not allowing the superclass to call delete on itsCurrentImageToDraw
        // and then deleting our private pointer
        itsCurrentImageToDraw = NULL;
        returnValue = GameObject::release();
        itsWallImage->release();
        delete itsWallImage;
    }
    return returnValue;
}