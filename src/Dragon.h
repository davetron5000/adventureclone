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
#ifndef __DRAGON_H__
#define __DRAGON_H__

#include "GameObject.h"
#include "Player.h"
#include "Game.h"

class Dragon : public GameObject
{
protected:
	// The color of this dragon
	int itsColor;

	// This is the rect that is considered the "head" of the dragon, where the player can get eaten
	RECT itsHeadRect;

	// True if this dragon is dead
	BOOL isDead;

	
	BOOL isAtePlayer;

	// utility  method for the dragon to "attack"; ensures that the dragon is alive and not in the middle of an attack
	virtual BOOL attack(DWORD currentTime, GameObject *victim);

	ScreenImage *itsNormalImage;
	ScreenImage *itsAttackingImage;
	ScreenImage *itsDeadImage;

	// coordinates of the stomach, so we can move the player in there when he gets eaten
	LONG itsStomachX;
	LONG itsStomachY;

	LONG itsPlayerLastX;
	LONG itsPlayerLastY;

	// This is basically it's chance of getting killed by a successful
	// sword hit.  We get a random number between 1 and toughness, and if the
	// result is 0, the dragon dies.
	int itsToughness;

	// AI Methods

	// This will cause the dragon to move towards an object
	void moveTowardsObject(DWORD currentTime, GameObject *object, BOOL runAway=FALSE);
	void runAwayFromObject(DWORD currentTime, GameObject *object);
	void moveTowardsPoint(DWORD currentTime, int x, int y, BOOL runAway=FALSE);

	// Stats for this dragon that control it's behavior

	// Speed, in pixels per second, that this dragon moves
	int itsSpeed;

	// time, in milliseconds, the dragon will spend chasing the player if the player
	// isn't in the current room
	DWORD itsPersistance;

	// An object the dragon will run away from
	GameObject *itsFearedObject;

	// An object the dragon will stay in a room to protect (or chase the player to protect)
	GameObject *itsProtectedObject;

	// How long the dragon will leave its mouth open before closing it and (hopefully) eating the player
	DWORD itsTimeToLeaveMouthOpen;


	// Dynamic states of this dragon, essentially, "what is it doing now?"

	// True if the dragon's mouth is open
	BOOL isMouthOpen;

	// TRUE if this dragon is chasing the player
	BOOL isPursuingPlayer;

	// when it started purusing the player
	DWORD itsPursuitStartTime;

	int itsMouthOpenTimerID;

	BOOL isBitingDown;

public:

	// distance someone colliding with a dragon will be pushed back
	static int theDragonTolerance;

	Dragon();

	void setPersistence(DWORD pers);
	void setSpeed(int speed);
	void setMouthOpenTime(DWORD time);
	void setToughness(int toughness);

	void onTimer(int timerID);

	// A dragon needs 3 images, one for normal, one for attacking (i.e. mouth open) and one for being dead.
	// Also, we need to know the rectangle (relative to the whole dragon) representing the head as well as
	// the coordinates of the upper-left of the stomach
    virtual BOOL init(DWORD currentTime, ScreenImage *normalImage, ScreenImage *attackingImage, ScreenImage *deadImage, 
		              RECT headRect, LONG stomachX, LONG stomachY, LONG x, LONG y);

	void setProtectedObject(GameObject *object);
	void setFearedObject(GameObject *object);

	virtual void onResurrected(GameObject *object);

	virtual BOOL update(DWORD currentTime);

	virtual BOOL collision(DWORD currentTime, GameObject *object);

	inline BOOL Dragon::stopMoving(DWORD currentTime)
	{
		if (isBeingHeld) return TRUE;
		else return GameObject::stopMoving(currentTime);
	}

	inline void Dragon::getHeadRect(RECT *rect)
	{
		rect->left   = getX() + itsHeadRect.left;
		rect->top    = getY() + itsHeadRect.top;
		rect->right  = getX() + itsHeadRect.right;
		rect->bottom = getY() + itsHeadRect.bottom;
	}

	// public only for easter egg purposes.  Don't use unless you know what you
	// are doing! Player must be in the same room as the dragon, or you
	// will be in trouble
	void kill(DWORD currentTime);
};

#endif // __DRAGON_H__
