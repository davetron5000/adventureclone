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
#include <windows.h>
#include <ddraw.h>
#include <assert.h>

#include <ScreenImage.h>
#include <SimpleArray.h>
#include "EventListener.h"

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#define IS_POINT_IN_RECT(x,y,rect) \
        ( ( (x >= rect.left) && (x < rect.right ) ) && \
          ( (y >= rect.top)  && (y < rect.bottom) ) )

#define IS_POINT_IN_BOTH_RECTS(x,y,one,two) \
        ( IS_POINT_IN_RECT(x,y,one) && IS_POINT_IN_RECT(x,y,two) )


#define BETWEEN(val,lo,hi) (((val) >= (lo)) && ((val) < (hi)))

// avoid circular reference
class Room;
class Holder;

// This is an object in the game.  It exists in a Room (see Room.h) and it's coordinates
// are contextual within the room.  
class GameObject : public EventListener
{
protected:

    // TRUE if this object is attracted to the magnet
    BOOL isMagnetic;

    // TRUE if the player may hold this object
    BOOL isPlayerHoldable;

    // TRUE if the bat may hold this object
    BOOL isBatHoldable;

    // Current X speed
    LONG itsXSpeed;
    // Current Y speed
    LONG itsYSpeed;

    // last time at which we moved at least one pixel in the X direction
    DWORD itsXLastMoveTime;
    // last time at which we moved at least one pixel in the Y direction
    DWORD itsYLastMoveTime;

    // Current X location in room
    LONG itsX;

    // previous X coordinate
    LONG itsLastX;

    // Current Y location in room
    LONG itsY;

    // Previous Y coordinate
    LONG itsLastY;

    // The image that should be drawn on the surface the next time
    // draw is called.  Subclasses that are animated can point this
    // to the appropriate member of their animation array.
    ScreenImage *itsCurrentImageToDraw;

    // Current room
    Room *itsRoom;

	BOOL isBeingHeld;

	// TRUE if a magnet is attracting this object
	BOOL isBeingAttracted;

	// Array of images over which to animate
	SimpleArray *itsAnimationImages;
	// Speed of the animation
	DWORD itsAnimationSpeed;
	
	UINT itsCurrentAnimationFrame;

	DWORD itsLastAnimateTime;

	BOOL isAnimating;

    // Helper method to move the object's position, based on current speed
    virtual BOOL move(DWORD currentTime);

	virtual UINT getNextAnimationIndex() { return (itsCurrentAnimationFrame + 1) % itsAnimationImages->length(); }

	Holder *itsHolder;

public:
    enum GameObjectType
    {
        GAMEOBJECT_TYPE_GAMEOBJECT=0,
        GAMEOBJECT_TYPE_PLAYER,
        GAMEOBJECT_TYPE_WALL,
        GAMEOBJECT_TYPE_BAT,
        GAMEOBJECT_TYPE_SWORD,
        GAMEOBJECT_TYPE_DRAGON,
		GAMEOBJECT_TYPE_GATE,
		GAMEOBJECT_TYPE_KEY,
		GAMEOBJECT_TYPE_BRIDGE,
		GAMEOBJECT_TYPE_DOT,
		GAMEOBJECT_TYPE_MAGNET,
		GAMEOBJECT_TYPE_CHALICE,
		GAMEOBJECT_TYPE_CANDLE,
		GAMEOBJECT_TYPE_ARMOUR,
		GAMEOBJECT_TYPE_NUMBER,
		GAMEOBJECT_TYPE_EXTRAITEMS,
        MAX_GAME_OBJECT_TYPES
    };

protected:

	GameObjectType itsType;

public:

	GameObject(BOOL playerHoldable=FALSE, 
		       BOOL batHoldable=FALSE,
			   BOOL magnetic=FALSE,
			   GameObjectType=GAMEOBJECT_TYPE_GAMEOBJECT);

    // returns the rect taken up by this object
    inline void getRect(RECT *rect)
    {
        assert(rect != NULL);
        rect->left = itsX;
        rect->top = itsY;
        rect->right = itsCurrentImageToDraw->getWidth() + itsX;
        rect->bottom = itsCurrentImageToDraw->getHeight() + itsY;
    }

	virtual void onUnpause()
	{
		DWORD currentTime = timeGetTime();
		itsLastAnimateTime = currentTime;
		itsXLastMoveTime = currentTime;
		itsYLastMoveTime = currentTime;
	}

    inline BOOL getIsPlayerHoldable() { return isPlayerHoldable; }
    inline BOOL getIsBatHoldable() { return isBatHoldable; }
    inline BOOL getIsMagnetic() { return isMagnetic; }

	inline BOOL getIsBeingHeld() { return isBeingHeld; }
	inline void setIsBeingHeld(BOOL val,Holder *holder=NULL) { isBeingHeld = val; itsHolder= val ? holder : NULL; }
	inline Holder *getHolder() { return itsHolder; }

	inline BOOL getIsBeingAttracted() { return isBeingAttracted; }
	inline void setIsBeingAttracted(BOOL val) { isBeingAttracted = val; }

    virtual inline GameObjectType getType() { return itsType; }

	// init a non-animating GameObject
    virtual BOOL init(DWORD currentTime, ScreenImage *image, LONG x, LONG y);

	// init an animating GameObject
	virtual BOOL init(DWORD currentTime, SimpleArray *images, LONG x, LONG y, DWORD animationSpeed);

    // Tells the object to start moving.  x and y are the speeds, in pixels/second
    // at which the object should move.
    virtual BOOL startMoving(DWORD currentTime, LONG x, LONG y);

    // Tells the object to stop moving.
    virtual BOOL stopMoving(DWORD currentTime);

    // Tells the object it collided with the passed-in object
    virtual BOOL collision(DWORD currentTime, GameObject *object);

    // returns TRUE if this object is colliding with the passed-in object. 
    virtual BOOL isColliding(GameObject *object);
    virtual BOOL isColliding(RECT objectRect);

	// returns true if this point is "touching" the object, based on trasparent pixels
	virtual BOOL isTouching(int x, int y);

    // Tells the object to draw itself at it's x/y location on the passed-in surface
    // There is an assumption that the surface passed in to 
    // the draw routine is such that if the object draws itself at (itsX,itsY) on that surface,
    // it will be correct (i.e., the upper-left corner of the Room in which the object exists
    // is at the upper left corner of the passed-in surface)
    virtual BOOL draw(DWORD currentTime, LPDIRECTDRAWSURFACE surface);

    // called once per loop iteration
    virtual BOOL update(DWORD currentTime);

    // restore the object
    virtual BOOL restore(DWORD currentTime);

    // Release resources
    virtual BOOL release();

	virtual void startAnimating(DWORD currentTime) { isAnimating = TRUE; }
	virtual void stopAnimating(DWORD currentTime)  { isAnimating = FALSE; }

    inline LONG getX() { return itsX; }
    inline LONG getY() { return itsY; }
    inline virtual void setX(LONG x) { itsX = x; }
    inline virtual void setY(LONG y) { itsY = y; }

	inline LONG getXSpeed() { return itsXSpeed; }
	inline LONG getYSpeed() { return itsYSpeed; }

    virtual inline void setRoom(Room *room) { itsRoom = room; }
    virtual inline Room *getRoom() { return itsRoom; }

	virtual int getPrimaryColor();
};

#endif // __GAMEOBJECT_H__