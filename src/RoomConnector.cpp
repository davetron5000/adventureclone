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
#include "RoomConnector.h"
#include "Game.h"
#include <Logger.h>
#include "Gate.h"
#include "Dragon.h"
#include "EventDispatcher.h"

void RoomConnector::moveObjectToDestination(GameObject *object)
{
    assert(object->getRoom() == itsSourceRoom);
    itsSourceRoom->removeObject(object);
	itsDestinationRoom->addObject(object);
    RECT objectRect;
	object->getRect(&objectRect);
	int width = objectRect.right - objectRect.left;
	int height = objectRect.bottom - objectRect.top;

    switch (itsSourceConnectionLocation)
    {
        case CONNECTION_LOCATION_TOP :
        {
            object->setY(Game::theScreenHeight - height);
        }
        break;
        case CONNECTION_LOCATION_BOTTOM :
        {
            object->setY(0);
        }
        break;
        case CONNECTION_LOCATION_LEFT :
        {
            object->setX(Game::theScreenWidth - width);
        }
        break;
        case CONNECTION_LOCATION_RIGHT :
        {
            object->setX(0);
        }
        break;
        case CONNECTION_LOCATION_ARBITRARY :
        {
            switch (itsDestinationConnectionLocation)
            {
                case CONNECTION_LOCATION_TOP :
                {
                    object->setY(0);
                }
                break;
                case CONNECTION_LOCATION_BOTTOM :
                {
                    object->setY(Game::theScreenHeight - height);
                }
                break;
                case CONNECTION_LOCATION_LEFT :
                {
                    object->setX(0);
                }
                break;
                case CONNECTION_LOCATION_RIGHT :
                {
                    object->setX(Game::theScreenWidth - width);
                }
                break;
                case CONNECTION_LOCATION_ARBITRARY :
                {
                    object->setX(itsDestinationRect.left);
                    object->setY(itsDestinationRect.top);
                }
            }
        }
        break;
    }
	if ( isCastleExit && (object->getType() == GameObject::GAMEOBJECT_TYPE_PLAYER) )
	{
		// the player is moving out of a castle.
		SimpleArray *objects = itsDestinationRoom->getObjects();
		for (UINT i=0;i<objects->length();i++)
		{
			GameObject *maybeGate = (GameObject *)(objects->elementAt(i));
			if (maybeGate->getType() == GameObject::GAMEOBJECT_TYPE_GATE)
			{
				((Gate *)maybeGate)->setIsUp();
				break;
			}
		}
	}
	theEventDispatcher.fireEvent(EventDispatcher::EVENT_TYPE_LEAVE,object,itsSourceRoom);
	theEventDispatcher.fireEvent(EventDispatcher::EVENT_TYPE_ENTER,object,itsDestinationRoom);
}

BOOL RoomConnector::canObjectConnect(GameObject *object)
{
    assert(object != NULL);
    // only objects with "intelligence" can connect
	if ( (object->getType() != GameObject::GAMEOBJECT_TYPE_PLAYER) &&
         (object->getType() != GameObject::GAMEOBJECT_TYPE_BAT) &&
         (object->getType() != GameObject::GAMEOBJECT_TYPE_DRAGON) )
         return FALSE;

	// if there's a gate and it's not up, don't let anyone go through
	if ( (itsGate != NULL) && ( ! itsGate->getIsUp() ) && !isCastleExit )
		return FALSE;

	if (object->getIsBeingHeld())
		return FALSE;
    assert(object->getRoom() == itsSourceRoom);
    BOOL returnValue = FALSE;
    RECT objectRect;

	// For the dragon, he's got to be able to eat players at the bottom of the screen
	// so, we'll use his head as the rect for checking for leaving.
	if (object->getType() == GameObject::GAMEOBJECT_TYPE_DRAGON)
		((Dragon *)object)->getHeadRect(&objectRect);
	else
		object->getRect(&objectRect);

    switch (itsSourceConnectionLocation)
    {
        case CONNECTION_LOCATION_TOP :
        {
            if (objectRect.top < 0)
                returnValue = TRUE;
        }
        break;
        case CONNECTION_LOCATION_BOTTOM :
        {
            if (objectRect.bottom > Game::theScreenHeight)
                returnValue = TRUE;
        }
        break;
        case CONNECTION_LOCATION_LEFT :
        {
            if (objectRect.left < 0)
                returnValue = TRUE;
        }
        break;
        case CONNECTION_LOCATION_RIGHT :
        {
            if (objectRect.right > Game::theScreenWidth)
                returnValue = TRUE;
        }
        break;
        case CONNECTION_LOCATION_ARBITRARY :
        {
            if (object->isColliding(itsSourceRect))
                returnValue = TRUE;
        }
        break;
    }
    return returnValue;
}

RoomConnector::RoomConnector(Room *source, Room *destination, ConnectionLocation sourceLoc, ConnectionLocation destLoc,
                             RECT sourceRect, RECT destinationRect, BOOL castleExit, Gate *gate)
{
    assert(source != NULL);
    assert(destination != NULL);

    itsSourceRoom = source;
    itsDestinationRoom = destination;
    itsSourceConnectionLocation = sourceLoc;
    itsDestinationConnectionLocation = destLoc;
    itsSourceRect = sourceRect;
    itsDestinationRect = destinationRect;
	isCastleExit = castleExit;
	itsGate = gate;
}
