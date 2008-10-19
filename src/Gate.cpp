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
#include "Gate.h"
#include "Key.h"
#include <Logger.h>
#include "Player.h"
#include "Wall.h"

#include <assert.h>

BOOL Gate::init(DWORD currentTime, SimpleArray *animationImages, 
		      LONG x, LONG y, DWORD animationSpeed, INT castleColor)
{
	isUp = FALSE;
	itsCastleColor = castleColor;
	itsCoolDown = 500;

	return GameObject::init(currentTime,animationImages,x,y,animationSpeed);
}

BOOL Gate::collision(DWORD currentTime, GameObject *object)
{
	if (object->getType() == GAMEOBJECT_TYPE_KEY)
	{
		if ((isAnimating == FALSE) && ((currentTime - itsLastAnimateTime) >= itsCoolDown))
		{
			Key *key = (Key *)object;

			if ((key->getCastle() == itsCastleColor) || (key->getCastle() == SUPER_SECRET_CASTLE_COLOR))
			{
				startAnimating(currentTime);
				isUp = !isUp;
				itsCurrentAnimationFrame = isUp ? 0 : itsAnimationImages->length() - 1;
			}
		}
	}
	else if (object->getType() == GAMEOBJECT_TYPE_PLAYER)
	{
		if ((!isUp) && (!isAnimating))
		{
			object->setY(itsY + itsCurrentImageToDraw->getHeight() + Wall::theWallTolerance);
		}
	}
	return TRUE;
}

BOOL Gate::update(DWORD currentTime)
{
	BOOL returnValue = GameObject::update(currentTime);
	if ( (itsCurrentAnimationFrame == 0) || 
		 ((itsCurrentAnimationFrame+1) == itsAnimationImages->length()) )
	{
		stopAnimating(currentTime);
	}
	
	return returnValue;
}