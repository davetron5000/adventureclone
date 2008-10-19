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

#ifndef __HOLDER_H__
#define __HOLDER_H__


// A holder is an object that can carry other objects.  In adventure, this is
// probably the player and the bat.  The magnet wouldn't be a holder because it can't
// carry objects around; objects just follow the magnet.
class Holder : public GameObject
{
protected:
    GameObject *itsCarriedObject;

    // How far away the picked-up object should be placed when picked up
    static int theHeldObjectTolerance;

public:
	
    inline Holder() { itsCarriedObject = NULL; }

    virtual inline GameObject *getCarriedObject() { return itsCarriedObject; }

    virtual void setCarriedObject(GameObject *object);
    
    virtual inline BOOL isCarryingObject() { return itsCarriedObject != NULL; }

    virtual BOOL update(DWORD currentTime);

    virtual void setRoom(Room *room);

	virtual BOOL isColliding(GameObject *object);

    virtual void setX(LONG x);
    virtual void setY(LONG y);

};

#endif // __HOLDER_H__