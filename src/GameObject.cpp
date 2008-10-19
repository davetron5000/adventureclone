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
#include <Logger.h>
#include "EventDispatcher.h"

GameObject::GameObject(BOOL playerHoldable, BOOL batHoldable, BOOL magnetic, GameObjectType type)
{
	itsAnimationImages = NULL;
	itsType = type;
    itsXLastMoveTime = 0;
    itsYLastMoveTime = 0;
    isMagnetic = magnetic;
    isPlayerHoldable = playerHoldable;
    isBatHoldable = batHoldable;
    itsXSpeed = 0;
    itsYSpeed = 0;
	isBeingHeld = FALSE;
	itsRoom = NULL;
	isAnimating = FALSE;
	isBeingHeld = FALSE;

	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_UNPAUSE,this,NULL);
	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_PAUSE,this,NULL);
}

int GameObject::getPrimaryColor()
{
	for (UINT x=0;x<itsCurrentImageToDraw->getWidth();x++)
	{
		for (UINT y=0;y<itsCurrentImageToDraw->getHeight();y++)
		{
			int color = itsCurrentImageToDraw->getColor(x,y);
			if (color != ScreenImage::theTransparencyColor)
				return color;
		}
	}
	return ScreenImage::theTransparencyColor;
}

BOOL GameObject::isColliding(RECT objectRect)
{
	RECT myRect;
    getRect(&myRect);
	// Basically checking each corner of both rectangles to see if that corner is
	// contained within the other rectange.
    return ( IS_POINT_IN_RECT(myRect.left,myRect.top,objectRect) ||
			 IS_POINT_IN_RECT(myRect.right,myRect.top,objectRect) ||
			 IS_POINT_IN_RECT(myRect.right,myRect.bottom,objectRect) ||
			 IS_POINT_IN_RECT(myRect.left,myRect.bottom,objectRect) ||
			 IS_POINT_IN_RECT(objectRect.left,objectRect.top,myRect) ||
			 IS_POINT_IN_RECT(objectRect.right,objectRect.top,myRect) ||
			 IS_POINT_IN_RECT(objectRect.right,objectRect.bottom,myRect) ||
			 IS_POINT_IN_RECT(objectRect.left,objectRect.bottom,myRect)
			 );
}

BOOL GameObject::isColliding(GameObject *object)
{
    assert(object != NULL);
	if (object != this)
	{
		if (object->getRoom() == getRoom())
		{
			RECT myRect,objectRect;
			getRect(&myRect);
			object->getRect(&objectRect);

			RECT newRect;

			// First get a rectangle that contains both rectangles here
			newRect.left   = (objectRect.left   < myRect.left   ? objectRect.left   : myRect.left);
			newRect.right  = (objectRect.right  > myRect.right  ? objectRect.right  : myRect.right);
			newRect.top    = (objectRect.top    < myRect.top    ? objectRect.top    : myRect.top);
			newRect.bottom = (objectRect.bottom > myRect.bottom ? objectRect.bottom : myRect.bottom);

			// now, traverse each point in that rectangle
			for (int row=newRect.top;row<newRect.bottom;row++)
			{
				// If this row is between the top/bottom of each object's rect
				if (BETWEEN(row,objectRect.top,objectRect.bottom) && 
					BETWEEN(row,myRect.top,myRect.bottom) )
				{
					for (int col=newRect.left;col<newRect.right;col++)
					{
						// If this column is between the top/bottom of each object's rect
						// (which means that the x,y point col,row is within each object's rects)
						if (BETWEEN(col,objectRect.left,objectRect.right) &&
							BETWEEN(col,myRect.left,myRect.right) )
						{
							// See if the point is touching the object
							if (object->isTouching(col,row) && isTouching(col,row))
								return TRUE;
						}
					}
				}
			}
		}
	}

	return FALSE;
}

BOOL GameObject::isTouching(int x, int y)
{
	x -= itsX;
	y -= itsY;
	if (itsCurrentImageToDraw->getColor(x,y) != ScreenImage::theTransparencyColor)
		return TRUE;
	else
		return FALSE;
}

BOOL GameObject::startMoving(DWORD currentTime, LONG x, LONG y)
{
    itsXSpeed = x;
    itsYSpeed = y;
    return TRUE;
}

BOOL GameObject::stopMoving(DWORD currentTime)
{
    itsXSpeed = 0;
    itsYSpeed = 0;
    itsXLastMoveTime = 0;
    itsYLastMoveTime = 0;
    return TRUE;
}

BOOL GameObject::move(DWORD currentTime)
{
    // If this is the first time we are moving, don't do anything.  Just
    // normalize the last move time and exit.
    if ((itsXLastMoveTime == 0) && (itsYLastMoveTime == 0))
    {
        itsXLastMoveTime = currentTime;
        itsYLastMoveTime = currentTime;
        return TRUE;
    }
    LONG deltaX = itsXSpeed * (currentTime - itsXLastMoveTime);
    LONG deltaY = itsYSpeed * (currentTime - itsYLastMoveTime);

    deltaX /= 1000;
    deltaY /= 1000;

    if ((deltaX != 0) || (itsXSpeed == 0))
        itsXLastMoveTime = currentTime;
    if ((deltaY != 0) || (itsYSpeed == 0))
        itsYLastMoveTime = currentTime;

    itsLastX = itsX;
    itsLastY = itsY;
    setX(itsX + deltaX);
    setY(itsY + deltaY);

    return TRUE;
}

BOOL GameObject::collision(DWORD currentTime, GameObject *object)
{
    return TRUE;
}

BOOL GameObject::init(DWORD currentTime, ScreenImage *image, LONG x, LONG y)
{
    itsCurrentImageToDraw = image;
    itsX = x;
    itsY = y;
	
    return TRUE;
}

BOOL GameObject::init(DWORD currentTime, SimpleArray *images, LONG x, LONG y, DWORD animationSpeed)
{
	BOOL returnValue = init(currentTime,(ScreenImage*)(images->elementAt(0)),x,y);
	itsAnimationImages = images;
	itsLastAnimateTime = currentTime;
	itsCurrentAnimationFrame = 0;
	itsAnimationSpeed = animationSpeed;
	
	return returnValue;
}

BOOL GameObject::draw(DWORD currentTime, LPDIRECTDRAWSURFACE surface)
{
    return itsCurrentImageToDraw->draw(surface,itsX,itsY);
}

BOOL GameObject::update(DWORD currentTime)
{
	// if this is an animated object and we're animating, and it's time to switch frames....
	if ( (itsAnimationImages != NULL) && (isAnimating) && (currentTime - itsLastAnimateTime) >= itsAnimationSpeed )
	{
		itsCurrentAnimationFrame = getNextAnimationIndex();
		itsCurrentImageToDraw = ( (ScreenImage *)itsAnimationImages->elementAt(itsCurrentAnimationFrame) );
		itsLastAnimateTime = currentTime;
	}
    return move(currentTime);
}

BOOL GameObject::restore(DWORD currentTime)
{
    itsCurrentImageToDraw->restore();
    return TRUE;
}

BOOL GameObject::release()
{
    return TRUE;
}