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
#include "Player.h"
#include <Logger.h>
#include "Dragon.h"
#include "Gate.h"
#include "Wall.h"

const int Player::theMovementSpeed = 275;

Player::Player() : Holder()
{
	itsBlockImage = NULL;
	isDead = FALSE;
	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_KILLED,this,this);
	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_RESURRECTED,this,this);
}

void Player::onResurrected(GameObject *object)
{
	setCarriedObject(NULL);
	isDead = FALSE;
	GameObject::onResurrected(object);
}

void Player::onKilled(GameObject *victim, GameObject *killer)
{
	isDead = TRUE;
	stopMoving(timeGetTime());
	GameObject::onKilled(victim,killer);
}

BOOL Player::collision(DWORD currentTime, GameObject *object)
{
	// the number of times we let ourselves loop before giving up
	static const int MAX_LOOP_TIMES = 64;

	GameObjectType type = object->getType();
	if ((type == GAMEOBJECT_TYPE_WALL) || (type == GAMEOBJECT_TYPE_DRAGON))
    {
		if (type == GAMEOBJECT_TYPE_DRAGON)
		{
			// if the player has armour, he can walk through the dragons
			if ((itsCarriedObject != NULL) && (itsCarriedObject->getType() == GameObject::GAMEOBJECT_TYPE_ARMOUR) )
			{
				return TRUE;
			}
		}
		else
		{
			if ((itsCarriedObject != NULL) && (itsCarriedObject->getType() == GameObject::GAMEOBJECT_TYPE_DOT) )
			{
				Wall *wall = (Wall *)object;
				if (wall->getPrimaryColor() == ScreenImage::theTransparencyColor)
					return TRUE;
			}
		}
		int loopTimes = 0;
        do
		{
			loopTimes++;
			BOOL left,right,top,bottom;

			RECT me,him;
			getRect(&me);
			object->getRect(&him);

			left = (me.left < him.left);
			right = (me.right >= him.right);
			top = (me.top < him.top);
			bottom = (me.bottom >= him.bottom);

			BOOL below = !top && bottom;
			BOOL above = !bottom && top;
			BOOL toTheLeft = left && !right;
			BOOL toTheRight = !left && right;

			LONG normX, normY;
			normX = toTheLeft ? -1 : (toTheRight ? 1 : 0);
			normY = above ? -1 : (below ? 1 : 0);

			// now, in the case where we are overlapping a wall entirely, all we can
			// do is to go in the opposite direction we were just going
			if (left && right)
			{
				normX = (itsXSpeed != 0) ? (itsXSpeed / abs(itsXSpeed)) : 0 ;
				normX *= -1;
			}
			if (top && bottom)
			{
				normY = (itsYSpeed != 0) ? (itsYSpeed / abs(itsYSpeed)) : 0 ;
				normY *= -1;
			}

			int tolerance = (type == GAMEOBJECT_TYPE_DRAGON)   ? 
									Dragon::theDragonTolerance : 
									Wall::theWallTolerance;

			if ((normX == 0) && (normY == 0))
			{
				// probably we are inside a wall
				normX = (itsXSpeed != 0) ? (itsXSpeed / abs(itsXSpeed)) : 0 ;
				normX *= -1;
				normY = (itsYSpeed != 0) ? (itsYSpeed / abs(itsYSpeed)) : 0 ;
				normY *= -1;
			}
			setX(itsX + (normX * tolerance));
			setY(itsY + (normY * tolerance));
		}
		// slight cheese.  Only loop through once when hitting a dragon, because sometimes,
		// the dragon can push us through walls.
		while((loopTimes < MAX_LOOP_TIMES) && isColliding(object));

		if (loopTimes >= MAX_LOOP_TIMES)
		{
			Logger::error("When dealing with Player collision, we looped more than 1000 times and would've gone infinite");
			Logger::error("We were colliding with a %s",(type == GAMEOBJECT_TYPE_WALL ? "wall" : "non-wall"));
		}
		
    }
    else if (object->getIsPlayerHoldable() && (!object->getIsBeingHeld()))
    {
        setCarriedObject(object);
    }
    else
    {
    }
    return TRUE;
}

BOOL Player::init(DWORD currentTime, WallImage *image, LONG x, LONG y)
{
	itsBlockImage = image;
	return Holder::init(currentTime,image,x,y);
}

void Player::setRoom(Room *room)
{
	Holder::setRoom(room);
	if (itsBlockImage != NULL)
		itsBlockImage->changeColor(room->getPrimaryColor());
}
