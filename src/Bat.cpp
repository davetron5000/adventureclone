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

#include "Bat.h"
#include <Logger.h>
#include "Room.h"
#include "Key.h"
#include <RandomGenerator.h>
#include "EventDispatcher.h"

Bat::Bat(BOOL smart) : Holder() 
{ 
	isSmarter = smart;
	isLookingForNewObject = TRUE; 
	isPlayerHoldable = TRUE;
	itsSoughtObject = NULL;
	isBatHoldable = FALSE;
	itsSpeed = 300;
	itsAttentionSpan = isSmarter ? 15000 : 5000;
	isMagnetic = FALSE;
	itsYSpeed = itsSpeed;
	itsXSpeed = 0;
	isStunned = FALSE;
}

void Bat::setCarriedObject(GameObject *object)
{
	Holder::setCarriedObject(object);
	
	// here, if we are smart, we hold onto valuable objects longer
	if (isSmarter)
	{
		if ( (object->getType() == GameObject::GAMEOBJECT_TYPE_CHALICE) ||
			 (object->getType() == GameObject::GAMEOBJECT_TYPE_KEY) )
		{
			itsAttentionSpan = 20000;
		}
		else
		{
			itsAttentionSpan = 5000;
		}
	}
	itsGrabObjectTimerID = theEventDispatcher.setTimer(this,itsAttentionSpan);
}

void Bat::onTimer(int timerID)
{
	if (timerID == itsGrabObjectTimerID)
	{
		if (!isLookingForNewObject)
		{
			isLookingForNewObject = TRUE;
			itsSoughtObject = NULL;
		}
	}
	else
	{
		Holder::onTimer(timerID);
	}
}

BOOL Bat::update(DWORD currentTime)
{
	// First, make sure we are moving
	if ( !isBeingHeld && !isStunned)
	{
		if ( (itsXSpeed == 0) && (itsYSpeed == 0) )
		{
			int xdir = RandomGenerator::getRandomNumber(3) - 1;
			int ydir = RandomGenerator::getRandomNumber(3) - 1;
	
			if ((xdir == 0) && (ydir == 0))
			{
				xdir = 1;
			}
			startMoving(currentTime,itsSpeed * xdir,itsSpeed * ydir);
		}
	}

	// now, figure out what object we want.
	if (isLookingForNewObject && !isStunned)
	{
		itsSoughtObject = determineSoughtObject(itsSoughtObject);
		setPursuitCourse(currentTime);
	}
	return Holder::update(currentTime);

}

void Bat::setPursuitCourse(DWORD currentTime)
{
	if ( (itsSoughtObject != NULL) && (!getIsBeingHeld()) )
	{
		RECT myRect;
		getRect(&myRect);
		int width = myRect.right - myRect.left;
		int height = myRect.bottom - myRect.top;
		int xdelta = itsSoughtObject->getX() - (myRect.left + (width/2));
		int ydelta = itsSoughtObject->getY() - (myRect.top + (height/2));

		int normx = xdelta == 0 ? 0 : xdelta/abs(xdelta);
		int normy = ydelta == 0 ? 0 : ydelta/abs(ydelta);
		if ((normx == 0) && (normy == 0))
			normx = 1;
		startMoving(currentTime,normx * itsSpeed,normy * itsSpeed);
	}
}

GameObject *Bat::determineSoughtObject(GameObject *soughtObject)
{
	if (!isLookingForNewObject)
		return NULL;

	if (soughtObject != NULL)
	{
		if (getRoom() == soughtObject->getRoom())
			return soughtObject;
		else
			soughtObject = NULL;
	}

	if (soughtObject == NULL)
	{
		Room *myRoom = getRoom();
		soughtObject = myRoom->searchRoom(GameObject::GAMEOBJECT_TYPE_CHALICE);
		if ( (soughtObject == NULL) || (soughtObject == getCarriedObject()) )
		{
			soughtObject = myRoom->searchRoom(GameObject::GAMEOBJECT_TYPE_SWORD);
			if ( (soughtObject == NULL) || (soughtObject == getCarriedObject()) )
			{
				// This is basically testing for:
				// object is null or the bat already has it or
				// the object is the master key and no one has it.  The idea being we don't
				// want the bat to bring the master key to the player ever.
				soughtObject = myRoom->searchRoom(GameObject::GAMEOBJECT_TYPE_KEY);
				if ( (soughtObject == NULL) || (soughtObject == getCarriedObject()) ||
					( ( ((Key*)soughtObject)->getCastle() == SUPER_SECRET_CASTLE_COLOR ) && 
					  ( soughtObject->getIsBeingHeld() == FALSE                        ) ) 
				   )
				{
					soughtObject = NULL;

					// Now, we go through the objects in the room and pick the one the player
					// is holding, unless he's not holding anything, then just take the first
					// thing we found
					for (UINT i=0;i<myRoom->getObjects()->length();i++)
					{
						GameObject *object = (GameObject *)myRoom->getObjects()->elementAt(i);
						// If we can hold it, and aren't already, and haven't already picked
						// an object
						if ( (object != getCarriedObject()) && object->getIsBatHoldable() &&
							  soughtObject == NULL)
						{
							// if it's a smart bat, don't pick up the dragon
							if ( !(isSmarter && (object->getType() == GameObject::GAMEOBJECT_TYPE_DRAGON)) )
							{
								soughtObject = object;
							}
						}
						// If someone is holding this object, grab it
						if ( (soughtObject != NULL) && object->getIsBeingHeld() )
						{
							break;
						}
					}
				}
			}
		}
	}
	return soughtObject;
}

BOOL Bat::collision(DWORD currentTime, GameObject *object)
{
	if (object == itsSoughtObject)
	{
		if (object->getIsBeingHeld())
		{
			Holder *holder = object->getHolder();
			if (holder != NULL)
				holder->setCarriedObject(NULL);
		}
		setCarriedObject(object);
		itsSoughtObject = NULL;
		isLookingForNewObject = FALSE;
	}
	return Holder::collision(currentTime,object);
}