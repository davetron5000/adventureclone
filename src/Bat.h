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
#include <stdlib.h>
#include <time.h>
#include "Holder.h"
#include "GameObject.h"

#ifndef __BAT_H__
#define __BAT_H__


class Bat : public Holder
{
protected:

	// TRUE if the bat is looking for a new object
	BOOL isLookingForNewObject;

	// Pointer to the object the Bat is seeking
	GameObject *itsSoughtObject;

	// How fast it flys in a particular direction
	int itsSpeed;

	// Gives the bat some extra smarts
	BOOL isSmarter;

	// How long (in ms) until the bat is bored of it's current object and goes
	// for another
	DWORD itsAttentionSpan;

	BOOL isStunned;

	GameObject *determineSoughtObject(GameObject *soughtObject);

	void setPursuitCourse(DWORD currentTime);

	int itsGrabObjectTimerID;

public:

    Bat(BOOL smart=FALSE);

    virtual void setCarriedObject(GameObject *object);

	virtual void onTimer(int timerID);

	// Contains the bat's AI
	virtual BOOL update(DWORD currentTime);

	// Allows the Bat to pick up things
	virtual BOOL collision(DWORD currentTime, GameObject *object);

	virtual GameObjectType getType() { return GAMEOBJECT_TYPE_BAT; }

	virtual void stun(BOOL stun=TRUE) 
	{ 
		isStunned = stun; 
		setCarriedObject(NULL);
		stopMoving(timeGetTime());
	}




};

#endif //__BAT_H__