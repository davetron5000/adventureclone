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
#include "Holder.h"
#include "WallImage.h"
#include "Room.h"
#include "EventDispatcher.h"

#ifndef __PLAYER_H__
#define __PLAYER_H__

class Wall;

// A Player is, well, the player.
class Player : public Holder
{
protected:
	WallImage *itsBlockImage;

	BOOL isOnBridge;

	BOOL isDead;

public:

	// The movement speed of a player
	const static int theMovementSpeed;

    Player();

	void onKilled(GameObject *victim, GameObject *killer);

	virtual BOOL init(DWORD currentTime, WallImage *image, LONG x, LONG y);

    inline GameObjectType getType() { return GAMEOBJECT_TYPE_PLAYER; }

    BOOL collision(DWORD currentTime, GameObject *object);

	virtual void setRoom(Room *room);

	void setOnBridge(BOOL onBridge=TRUE)
	{
		isOnBridge = onBridge;
	}

	virtual void onResurrected(GameObject *object);

	virtual void setCarriedObject(GameObject *object)
	{
		Holder::setCarriedObject(object);
	}

	BOOL getOnBridge() { return isOnBridge; }

	BOOL getIsDead() { return isDead; }
};
#endif // __PLAYER_H__