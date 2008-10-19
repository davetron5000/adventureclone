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
#include <SimpleArray.h>

#ifndef __GATE_H__
#define __GATE_H__

class Gate : public GameObject
{
protected:

	BOOL isUp;

	INT itsCastleColor;

	// Amount of time after animation is complete that we ignore keys
	DWORD itsCoolDown;

	virtual UINT getNextAnimationIndex() 
	{ 
		if (isUp)
		{
			return (itsCurrentAnimationFrame + 1) % itsAnimationImages->length();
		}
		else
		{
			if (itsCurrentAnimationFrame == 0)
				return 0;
			else 
				return (itsCurrentAnimationFrame - 1);
		}
	}

public:

	Gate() : GameObject() {}

	BOOL init(DWORD currentTime, SimpleArray *animationImages, 
		      LONG x, LONG y, DWORD animationSpeed, INT castleColor);

	GameObjectType getType() { return GAMEOBJECT_TYPE_GATE; }

	virtual BOOL collision(DWORD currentTime, GameObject *object);

	// called once per loop iteration
    virtual BOOL update(DWORD currentTime);

	void setIsUp(BOOL up=TRUE) 
	{ 
		isUp = up; 
		itsCurrentAnimationFrame = isUp ? itsAnimationImages->length() -1 : 0;
		itsCurrentImageToDraw = (ScreenImage *)(itsAnimationImages->elementAt(itsCurrentAnimationFrame));
	}

	BOOL getIsUp() { return isUp; }

};

#endif // __GATE_H__