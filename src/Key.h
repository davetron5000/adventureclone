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
#include "EventDispatcher.h"
#include "EventListener.h"
#include <Logger.h>

#ifndef __KEY_H__
#define __KEY_H__

#define SUPER_SECRET_CASTLE_COLOR -1

class Key : public GameObject
{
protected:
	int itsCastleColor;

public:

	Key() : GameObject(TRUE,TRUE,TRUE,GAMEOBJECT_TYPE_KEY)
	{
		isPlayerHoldable = TRUE;
		isBatHoldable = TRUE;
		isMagnetic = TRUE;
	}

	int getCastle() { return itsCastleColor; }

	BOOL init(DWORD currentTime, ScreenImage *image, LONG x, LONG y, INT castleColor)
	{
		BOOL returnValue = GameObject::init(currentTime,image,x,y);
		itsCastleColor = castleColor;
		return returnValue;
		
	}
};
#endif // __KEY_H__