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
#include "Magnet.h"
#include "Room.h"
#include <math.h>

#define SHOULD_ATTRACT(object) ( (object != this) && (object->getIsMagnetic()) )

BOOL Magnet::update(DWORD currentTime)
{
	SimpleArray *objects = getRoom()->getObjects();
	UINT attracted = 0;
	for (UINT i=0;i<objects->length();i++)
	{
		GameObject *object = (GameObject *)(objects->elementAt(i));
		if ( SHOULD_ATTRACT(object) )
		{
			// we want to attract objects, so if the object we want is being held,
			// count that as an attracted object, but don't actually move it
			if (!object->getIsBeingHeld())
			{
				RECT myRect;
				getRect(&myRect);
				int xdelta = myRect.left - object->getX();
				int ydelta = myRect.bottom - object->getY();
				int normx = xdelta == 0 ? 0 : xdelta/abs(xdelta);
				int normy = ydelta == 0 ? 0 : ydelta/abs(ydelta);
				object->startMoving(currentTime,normx * itsMagnetismStrengh,normy * itsMagnetismStrengh);
				object->setIsBeingAttracted(TRUE);
			}
			attracted++;
			if (attracted == itsNumberOfObjectsToAttract)
				break;
		}
	}
	return GameObject::update(currentTime);
}


void Magnet::setRoom(Room *room)
{
	if (getRoom() != NULL)
	{
		SimpleArray *objects = getRoom()->getObjects();
		UINT attracted = 0;
		for (UINT i=0;i<objects->length();i++)
		{
			GameObject *object = (GameObject *)(objects->elementAt(i));
			if ( SHOULD_ATTRACT(object) )
			{
				object->setIsBeingAttracted(FALSE);
				object->stopMoving(itsXLastMoveTime);
			}
		}
	}
	GameObject::setRoom(room);
}
	
