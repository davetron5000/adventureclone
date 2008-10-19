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
#include "Room.h"
#include <Logger.h>
#include "Game.h"

#include <assert.h>
#include "RoomConnector.h"
#include "EventDispatcher.h"

Room::Room()
{
    itsConnectors = new SimpleArray(10);
    itsWalls = new SimpleArray(10);
    itsContainedObjects = new SimpleArray(10);
	itsStartingObject = NULL;
}

Room::~Room()
{
    delete itsConnectors;
    delete itsWalls;
    delete itsContainedObjects;
}

BOOL Room::release()
{
	free(itsRegionData);
    return TRUE;
}

BOOL Room::init(ScreenImage *image, char *name, ScreenImage *darkImage, ULONG visibility, int primaryColor,
				BOOL chaliceStart, BOOL objectStart, int startX, int startY)
{
    isDark = darkImage != NULL;
	itsDarkImage = darkImage;
    itsVisibility = visibility;
    itsScreenImage = image;
    itsName = name;
	itsPlayer = NULL;
	itsPrimaryColor = primaryColor;
	itsRegionData = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + sizeof(RECT));
	itsRegionData->rdh.dwSize = sizeof(RGNDATAHEADER);
	itsRegionData->rdh.iType = RDH_RECTANGLES;
	itsRegionData->rdh.nCount = 1;
	itsRegionData->rdh.nRgnSize = sizeof(RECT);

	isChaliceAllowedToStart = chaliceStart;
	isObjectAllowedToStart = objectStart;
	// if not specified, starting loc is bottom right
	itsStartX = (startX == -1 ? 544 : startX);
	itsStartY = (startY == -1 ? 288 : startY);

	return (SUCCEEDED(Game::theDirectDrawObject->CreateClipper(0,&itsClipper,NULL)));
}

BOOL Room::draw(DWORD currentTime, LPDIRECTDRAWSURFACE surface)
{
    assert(surface != NULL);
    BOOL returnValue = TRUE;
    if (itsScreenImage->draw(surface,0,0))
    {
		RECT rectToClip;
		if (isDark && itsPlayer != NULL)
		{
			// If the room is dark, we draw only the square around the player at any given
			// time.
			RECT playerRect;
			itsPlayer->getRect(&playerRect);
			LONG centerX, centerY;
			centerX = playerRect.left + ((playerRect.right - playerRect.left) / 2);
			centerY = playerRect.top + ((playerRect.bottom - playerRect.top) / 2);
			
			ULONG visDiv2 = itsVisibility / 2;
			
			rectToClip.left = centerX - visDiv2;
			rectToClip.right = centerX + visDiv2;
			rectToClip.top = centerY - visDiv2;
			rectToClip.bottom = centerY + visDiv2;

			if (rectToClip.left < 0) rectToClip.left = 0;
			if (rectToClip.top < 0) rectToClip.top = 0;
			if (rectToClip.right > Game::theScreenWidth) rectToClip.right = Game::theScreenWidth;
			if (rectToClip.bottom > Game::theScreenHeight) rectToClip.bottom = Game::theScreenHeight;

		}
		else
		{
			rectToClip.left = 0;
			rectToClip.right = Game::theScreenWidth;
			rectToClip.top = 0;
			rectToClip.bottom = Game::theScreenHeight;
		}

		
		memcpy(itsRegionData->Buffer,&rectToClip,sizeof(RECT));
		
		itsRegionData->rdh.rcBound.left   = rectToClip.left;
		itsRegionData->rdh.rcBound.right  = rectToClip.right;
		itsRegionData->rdh.rcBound.top    = rectToClip.top;
		itsRegionData->rdh.rcBound.bottom = rectToClip.bottom;

		if (SUCCEEDED(itsClipper->SetClipList(itsRegionData,0)))
		{
			if (SUCCEEDED(surface->SetClipper(itsClipper)))
			{
				if (isDark && itsDarkImage != NULL)
				{
					if (itsDarkImage->draw(surface,0,0))
					{
					}
					else
					{
						Logger::error("Error in drawing dark background!");
					}
				}
				for (ULONG i=0;i<itsWalls->length();i++)
				{
				
					GameObject *obj = (GameObject *)(itsWalls->elementAt(i));
					if (!obj->draw(currentTime,surface))
					{
						returnValue = FALSE;
						Logger::error("error in drawing wall in room");
						break;
					}
				}
			}
			else
			{
				Logger::error("Error setting clipper!");
			}
		}
		else
		{
			Logger::error("Error setting clipper's clip list!");
		}
		if (isDark)
		{
			rectToClip.left = 0;
			rectToClip.right = Game::theScreenWidth;
			rectToClip.top = 0;
			rectToClip.bottom = Game::theScreenHeight;
			
			memcpy(itsRegionData->Buffer,&rectToClip,sizeof(RECT));
			
			itsRegionData->rdh.rcBound.left   = rectToClip.left;
			itsRegionData->rdh.rcBound.right  = rectToClip.right;
			itsRegionData->rdh.rcBound.top    = rectToClip.top;
			itsRegionData->rdh.rcBound.bottom = rectToClip.bottom;

			if (SUCCEEDED(itsClipper->SetClipList(itsRegionData,0)))
			{
				if (SUCCEEDED(surface->SetClipper(itsClipper)))
				{
				}
				else
				{
					Logger::error("Error setting clipper back to normal to draw objects");
				}
			}
			else
			{
				Logger::error("Error setting clipper's clip list back to normal to draw objects");
			}
		}
			
		if (returnValue)
		{
			for (ULONG i=0;i<itsContainedObjects->length();i++)
			{
				GameObject *obj = (GameObject *)(itsContainedObjects->elementAt(i));
				if (!obj->draw(currentTime,surface))
				{
					returnValue = FALSE;
					Logger::error("error in drawing contained object in room");
					break;
				}
			}
		}
    }
    else
    {
        Logger::error("Error in drawing background for room");
        returnValue = FALSE;
    }
    return returnValue;
}

void Room::onEnterRoom(GameObject *object, Room *room)
{
	if (room == this)
	{
	}
}

void Room::onLeaveRoom(GameObject *object, Room *room)
{
	if (room == this)
	{
	}
}

void Room::addObject(GameObject *object, BOOL starting)
{
    if (-1 == itsContainedObjects->findObject(object))
    {
        itsContainedObjects->addElement(object);
        object->setRoom(this);
		if (object->getType() == GameObject::GAMEOBJECT_TYPE_PLAYER)
			itsPlayer = (Player *)object;
		if (starting == TRUE)
		{
			object->setX(itsStartX);
			object->setY(itsStartY);
		}
		if (object->getType() == GameObject::GAMEOBJECT_TYPE_CANDLE)
			itsVisibility *= 2;
    }
}

void Room::removeObject(GameObject *object)
{
    int index = itsContainedObjects->findObject(object);
    if (index != -1)
    {
		if (object->getType() == GameObject::GAMEOBJECT_TYPE_CANDLE)
			itsVisibility /= 2;
        itsContainedObjects->removeElementAt(index);
		if (object->getType() == GameObject::GAMEOBJECT_TYPE_PLAYER)
			itsPlayer = NULL;
    }
}

void Room::checkForObjectsLeaving()
{
    // TODO: go through each connector and see if an object is "connecting" to another
    // room.
    for (UINT i=0;i<itsConnectors->length();i++)
    {
        RoomConnector *connector = (RoomConnector *)(itsConnectors->elementAt(i));
        BOOL done = FALSE;
        UINT j = 0;
        
        // Problem here is that when we remove an element, that changes the length
        // and indeces of the object in that array.  So, when an element is removed, we don't
        // up the index, since the next object in the array will get the old object's index
        // If we aren't removing an object, increment it as normal.
        // Also note that we cannot cache the length of the array because the length may change
        while (j < itsContainedObjects->length())
        {
            UINT currentLength = itsContainedObjects->length();
            GameObject *object = (GameObject *)(itsContainedObjects->elementAt(j));
            if (connector->canObjectConnect(object))
            {
                connector->moveObjectToDestination(object);
                UINT newLength = itsContainedObjects->length();
                // Here, we need to modify our index to make sure we dont' miss anything,
                // because we don't know how many objects might have been removed from this
                // room as a result of connections
                j -= (newLength - 1);
            }
            else
            {
                j++;
            }
        }
    }
}

BOOL Room::restore(DWORD currentTime)
{
    BOOL returnValue = TRUE;
    ULONG i;

    returnValue = itsScreenImage->restore();
    for (i=0;i<itsWalls->length();i++)
    {
        returnValue = returnValue && 
                      ((GameObject*)(itsWalls->elementAt(i)))->restore(currentTime);
    }
    for (i=0;i<itsContainedObjects->length();i++)
    {
        returnValue = returnValue && 
                      ((GameObject*)(itsContainedObjects->elementAt(i)))->restore(currentTime);
    }
    return returnValue;
}