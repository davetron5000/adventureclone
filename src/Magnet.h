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
#include "Room.h"

#ifndef __MAGNET_H__
#define __MAGNET_H__

class Magnet : public GameObject
{
protected:
	
	// speed, in pixels/second that the magnet pulls objects towards it
	UINT itsMagnetismStrengh;

	// The number of object it can attract at once
	UINT itsNumberOfObjectsToAttract;

public:

	Magnet() : GameObject(TRUE,TRUE,FALSE,GAMEOBJECT_TYPE_MAGNET)
	{}

	virtual BOOL init(DWORD currentTime, ScreenImage *image, LONG x, LONG y, UINT strength=75, UINT numAttract=1)
	{
		BOOL returnValue = GameObject::init(currentTime,image,x,y);
		itsMagnetismStrengh = strength;
		itsNumberOfObjectsToAttract = numAttract;
		return returnValue;
	}

	virtual BOOL update(DWORD currentTime);

	virtual void setRoom(Room *room);
};

#endif __MAGNET_H__