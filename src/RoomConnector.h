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
#include "Room.h"
#include "Gate.h"

#ifndef __ROOMCONNECTOR_H__
#define __ROOMCONNECTOR_H__

// A RoomConnector encapsulates the concept of two rooms connecting to one another.
class RoomConnector
{
public:
    
    enum ConnectionLocation
    {
        // A connection on top means an object must have a negative y coordinate
        CONNECTION_LOCATION_TOP,
        // A connection on the bottom means an object must have a y coordinate greater
        // than the screen height.
        CONNECTION_LOCATION_BOTTOM,
        // A connection at left means the object must have a negative x coordinate
        CONNECTION_LOCATION_LEFT,
        // A connection at right means the object's x coordinate must be greater than
        // the screen width
        CONNECTION_LOCATION_RIGHT,
        // An arbitrary connection means that the object's rect must intersect with
        // the associated rect.
        CONNECTION_LOCATION_ARBITRARY,
    };

protected:
    Room *itsSourceRoom;
    Room *itsDestinationRoom;

	// true if this connection allows us to leave a castle.  If so,
	// we have to make sure the gate is left open
	BOOL isCastleExit;

    // The location of this connection
    ConnectionLocation itsSourceConnectionLocation;
    // The coordinates with this connection, if source connection location
    // is arbitrary
    RECT itsSourceRect;

    ConnectionLocation itsDestinationConnectionLocation;

    // If destination connection is arbitrary, or source location is arbitrary,
    // this is used.
    RECT itsDestinationRect;

	Gate *itsGate;

public:
    
	// the gate is non-NULL if this is a castle entrance/exit
    RoomConnector(Room *source, Room *destination, ConnectionLocation sourceLoc, ConnectionLocation destLoc,
                  RECT sourceRect, RECT destinationRect, BOOL castleExit=FALSE, Gate *gate=NULL);

    // Does the work of taking the object out of it's current room
    // and placing it in the destination room
    void moveObjectToDestination(GameObject *object);

    // returns true if this object can make the connection.
    BOOL canObjectConnect(GameObject *object);

	void release() {}


};

#endif // __ROOMCONNECTOR_H__