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
#include "Dragon.h"
#include "Game.h"
#include <Logger.h>
#include <RandomGenerator.h>
#include "EventDispatcher.h"

int Dragon::theDragonTolerance = 4;

Dragon::Dragon() : GameObject(FALSE,TRUE,FALSE,GAMEOBJECT_TYPE_DRAGON) 
{ 
	itsPersistance = 10000;
	itsFearedObject = NULL;
	itsProtectedObject = NULL;
	isPursuingPlayer = FALSE;
	itsSpeed = Player::theMovementSpeed - 75;
	itsTimeToLeaveMouthOpen = 1000; // 3000 to be like game #1 of the atari cart
	isDead = FALSE;
	isMouthOpen = FALSE;
	isBitingDown = FALSE;
	isAtePlayer = FALSE;
	itsPlayerLastX = Game::theScreenWidth / 2;
	itsPlayerLastY = Game::theScreenHeight / 2;

	itsToughness = 10; 

	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_RESURRECTED,this,this);
}

void Dragon::setPersistence(DWORD pers) { itsPersistance = pers; }
void Dragon::setSpeed(int speed) { itsSpeed = speed; }
void Dragon::setMouthOpenTime(DWORD time) { itsTimeToLeaveMouthOpen = time; }
void Dragon::setToughness(int toughness) { itsToughness = toughness; }

// A dragon needs 3 images, one for normal, one for attacking (i.e. mouth open) and one for being dead.
// Also, we need to know the rectangle (relative to the whole dragon) representing the head as well as
// the coordinates of the upper-left of the stomach
BOOL Dragon::init(DWORD currentTime, ScreenImage *normalImage, ScreenImage *attackingImage, ScreenImage *deadImage, 
		          RECT headRect, LONG stomachX, LONG stomachY, LONG x, LONG y)
{
	itsNormalImage = normalImage;
	itsAttackingImage = attackingImage;
	itsDeadImage = deadImage;
	itsHeadRect = headRect;
	itsStomachX = stomachX;
	itsStomachY = stomachY;
	return GameObject::init(currentTime,normalImage,x,y);
}


void Dragon::setProtectedObject(GameObject *object) { itsProtectedObject = object; }
void Dragon::setFearedObject(GameObject *object) { itsFearedObject = object; }

void Dragon::onResurrected(GameObject *object)
{
	isAtePlayer = FALSE;
	isPursuingPlayer = FALSE;
	isDead = FALSE;
	isMouthOpen = FALSE;
	itsCurrentImageToDraw = itsNormalImage;		
	GameObject::onResurrected(object);
}

void Dragon::onTimer(int timerID)
{
	if (timerID == itsMouthOpenTimerID)
	{
		isBitingDown = TRUE;
	}
}

BOOL Dragon::update(DWORD currentTime)
{
	if (!isDead)
	{
		// if the player is around, remember where we last saw him
		GameObject *player = getRoom()->getPlayer();
		if (player != NULL)
		{
			itsPlayerLastX = player->getX();
			itsPlayerLastY = player->getY();
		}
		if (isBitingDown)
		{
			isBitingDown = FALSE;
			// close mouth and hopefully eat the player!
			isMouthOpen = FALSE;
			itsCurrentImageToDraw = itsNormalImage;
			stopMoving(currentTime);
			GameObject *player = getRoom()->getPlayer();
			if (player != NULL)
			{
				GameObject *heldObject = ((Player *)player)->getCarriedObject();
				if ( (heldObject != NULL) && 
					 (heldObject->getType() == GameObject::GAMEOBJECT_TYPE_ARMOUR))
				{
					// player has armour - he's protected
				}
				else
				{
					RECT headRect;
					getHeadRect(&headRect);
					if (player->isColliding(headRect))
					{
						// player got eaten!
						player->setX(getX() + itsStomachX);
						player->setY(getY() + itsStomachY);
						theEventDispatcher.fireEvent(EventDispatcher::EVENT_TYPE_KILLED,player,this);
						isAtePlayer = TRUE;
					}
				}
			}
		}
		else if (isMouthOpen)
		{
			stopMoving(currentTime);
		}
		else
		{
			if (!isBeingHeld && !isAtePlayer)
			{
				if (player != NULL)
				{
					if ( (itsFearedObject != NULL) && (((Player *)player)->getCarriedObject() == itsFearedObject) )
					{
						isPursuingPlayer = FALSE;
						runAwayFromObject(currentTime,player);
					}
					else
					{
						// player's in the room, so reset our pursuit time
						itsPursuitStartTime = currentTime;
						isPursuingPlayer = TRUE;
						moveTowardsObject(currentTime,player);
					}
				}
				else
				{
					// player's not in the room.  Now we have to decide what to do.
					if (-1 == (getRoom()->getObjects()->findObject(itsProtectedObject)))
					{
						if (isPursuingPlayer == TRUE)
						{
							if (currentTime - itsPursuitStartTime >= itsPersistance)
							{
								isPursuingPlayer = FALSE;
								stopMoving(currentTime);
							}
							else
							{
								// its possible that we are going after the player
								// but had to stop to try to take a bite.  So, head
								// towards where we last saw him.
								if ((itsXSpeed == 0) && (itsYSpeed == 0))
								{
									moveTowardsPoint(currentTime,itsPlayerLastX,itsPlayerLastY);
								}
								// keep running after the player
								// since we are here in the code, that means we're in
								// motion and haven't changed direction since the player
								// was in the room, so we should be going towards him
								// still.  So, we do nothing.
							}
						}
						else
						{
							// not purusing the player, so stay put
							stopMoving(currentTime);
						}
					}
					else
					{
						// we are protecting an object in the room, so stay here
						stopMoving(currentTime);
					}
				}
			}
		}
	}
	return GameObject::update(currentTime);
}

void Dragon::runAwayFromObject(DWORD currentTime, GameObject *object)
{
	moveTowardsObject(currentTime,object,TRUE);
}

void Dragon::moveTowardsObject(DWORD currentTime, GameObject *object, BOOL runAway)
{
	moveTowardsPoint(currentTime,object->getX(),object->getY(),runAway);
}

void Dragon::moveTowardsPoint(DWORD currentTime, int x, int y, BOOL runAway)
{
	//make him follow the player
	RECT myRect;
	getHeadRect(&myRect);
	int width = myRect.right - myRect.left;
	int height = myRect.bottom - myRect.top;
	int xdelta = x - (myRect.left + (width/2));
	int ydelta = y - (myRect.top + (height/2));

	int normx = xdelta == 0 ? 0 : xdelta/abs(xdelta);
	int normy = ydelta == 0 ? 0 : ydelta/abs(ydelta);

	if (runAway)
	{
		normx *= -1;
		normy *= -1;
	}
	
	startMoving(currentTime,normx * itsSpeed,normy * itsSpeed);
}

BOOL Dragon::collision(DWORD currentTime, GameObject *object)
{
	if (object->getType() == GameObject::GAMEOBJECT_TYPE_PLAYER)
	{
		if (!isAtePlayer)
		{
			object->collision(currentTime,this);
			return attack(currentTime,object);
		}
	}
	if (object->getType() == GameObject::GAMEOBJECT_TYPE_SWORD)
	{
		// If the dragon isn't in the same room as the player, it's invincible
		if (getRoom()->getPlayer() != NULL)
		{
			RECT headRect;
			getHeadRect(&headRect);

			// the sword must hit the body and the dragon must not be attacking
			if (!isDead && !object->isColliding(headRect) && !isMouthOpen)
			{
				// finally, give the dragon a random chance to live
				if ( (RandomGenerator::getRandomNumber(itsToughness) == 0) )
				{
					kill(currentTime);
				}
			}
		}

	}
	return TRUE;
}

void Dragon::kill(DWORD currentTime)
{
	isDead = TRUE;
	itsCurrentImageToDraw = itsDeadImage;
	stopMoving(currentTime);
	isMouthOpen = FALSE;
	theEventDispatcher.fireEvent(EventDispatcher::EVENT_TYPE_KILLED,this,getRoom()->getPlayer());
}

BOOL Dragon::attack(DWORD currentTime, GameObject *victim)
{
	if (!isDead && !isMouthOpen && !isAtePlayer)
	{
		isMouthOpen = TRUE;
		itsCurrentImageToDraw = itsAttackingImage;
		stopMoving(currentTime);
		itsMouthOpenTimerID = theEventDispatcher.setTimer(this,itsTimeToLeaveMouthOpen);

		RECT headRect;
		getHeadRect(&headRect);
		int headHeight = headRect.bottom - headRect.top;

		RECT victimRect;
		victim->getRect(&victimRect);

		if (!isBeingHeld)
		{
			setX(victimRect.right - 1);
			setY(victimRect.top - (headHeight/2));
		}

		theEventDispatcher.fireEvent(EventDispatcher::EVENT_TYPE_ATTACKED,victim,this);
	}
	return TRUE;
}
