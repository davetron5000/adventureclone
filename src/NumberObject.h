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
#include <assert.h>


#ifndef __NUMBEROBJECT_H__
#define __NUMBEROBJECT_H__

class NumberObject : public GameObject
{
protected:
	int itsNumber;

public:

	NumberObject() : GameObject(TRUE,FALSE,FALSE,GAMEOBJECT_TYPE_NUMBER)
	{}

	// Init this object.  number must be a one-digit number
	virtual BOOL init(DWORD currentTime, ScreenImage *image, LONG x, LONG y, int number)
	{
		assert(number < 10);

		itsNumber = number;

		return GameObject::init(currentTime,image,x,y);
	}

	virtual int getNumber() { return itsNumber; }
};

#endif __NUMBEROBJECT_H__