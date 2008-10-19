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
#include "Holder.h"
#include "Room.h"
#include "Game.h"

#include "EventDispatcher.h"

int Holder::theHeldObjectTolerance = 20;

BOOL Holder::update(DWORD currentTime)
{
    BOOL returnValue;
    returnValue = GameObject::update(currentTime);
    if (itsCarriedObject != NULL)
    {
        assert(itsCarriedObject->getRoom() == getRoom());
        returnValue = itsCarriedObject->update(currentTime) && returnValue;
    }
    return returnValue;
}

BOOL Holder::isColliding(GameObject *object)
{
	if (object != itsCarriedObject)
	{
		return GameObject::isColliding(object);
	}
	return FALSE;
}

void Holder::setX(LONG x)
{
    if (itsCarriedObject != NULL)
    {
        LONG xDiff = getX() - itsCarriedObject->getX();
		itsCarriedObject->setX(x-xDiff);
    }
    GameObject::setX(x);
}

void Holder::setRoom(Room *room)
{
    if (itsCarriedObject != NULL)
    {
        itsRoom->removeObject(itsCarriedObject);
        room->addObject(itsCarriedObject);
		itsCarriedObject->setRoom(room);
    }
    GameObject::setRoom(room);
}
void Holder::setY(LONG y)
{
    if (itsCarriedObject != NULL)
    {
        LONG yDiff = getY() - itsCarriedObject->getY();
        itsCarriedObject->setY(y-yDiff);
    }
    GameObject::setY(y);
}

void Holder::setCarriedObject(GameObject *object)
{ 
    DWORD currentTime = timeGetTime();
	GameObject *oldCarriedObject = itsCarriedObject;
    if (itsCarriedObject != NULL)
    {
		// We don't want the object getting lost of the screen, so we make sure that one pixel's
		// worth is in the room
		RECT objRect;
		itsCarriedObject->getRect(&objRect);
		LONG x = itsCarriedObject->getX();
		LONG y = itsCarriedObject->getY();

		if (objRect.right < 0) 
		{
			x = -(objRect.right - objRect.left) + 1;
		}
		if (objRect.left > Game::theScreenWidth) 
		{
			x = Game::theScreenWidth - 1;
		}
		if (objRect.bottom < 0) 
		{
			y = -(objRect.bottom - objRect.top) + 1;
		}
		if (objRect.top > Game::theScreenHeight) 
		{
			y = Game::theScreenHeight - 1;
		}
		itsCarriedObject->setX(x);
		itsCarriedObject->setY(y);
		itsCarriedObject->setIsBeingHeld(FALSE);
		itsCarriedObject->stopMoving(currentTime);
    }
	
	itsCarriedObject = object; 
	
    if (itsCarriedObject != NULL)
    {
		LONG relativeXSpeed = itsXSpeed - itsCarriedObject->getXSpeed();
		LONG relativeYSpeed = itsYSpeed - itsCarriedObject->getYSpeed();
        LONG xNorm = relativeXSpeed != 0 ? relativeXSpeed / abs(relativeXSpeed) : 0;
        LONG yNorm = relativeYSpeed != 0 ? relativeYSpeed / abs(relativeYSpeed) : 0;

		itsCarriedObject->stopMoving(currentTime);
		itsCarriedObject->setIsBeingHeld(TRUE,this);
		
        xNorm *= theHeldObjectTolerance;
        yNorm *= theHeldObjectTolerance;

        itsCarriedObject->setX(itsCarriedObject->getX() + xNorm);
        itsCarriedObject->setY(itsCarriedObject->getY() + yNorm);
		
    }
	if (object != NULL)
	{
		theEventDispatcher.fireEvent(EventDispatcher::EVENT_TYPE_GRAB,object,this);
	}
	if (oldCarriedObject != NULL)
	{
		theEventDispatcher.fireEvent(EventDispatcher::EVENT_TYPE_DROP,oldCarriedObject,this);
	}
}
