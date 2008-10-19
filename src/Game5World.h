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
#include "Game4World.h"

#ifndef __GAME5WORLD_H__
#define __GAME5WORLD_H__

class Game5World : public Game4World
{
protected:

	int usedRooms[GAME4_NUMBER_OF_ROOMS];

	// if chalice is TRUE, the room must be able to take a chalice.
	// empty is the max number of objects that can be at the room to assing
	int getRandomRoom(BOOL chalice, int empty);

	BOOL isRoomSuitable(Room *room, int number, BOOL chalice, int empty);

public:
	Game5World(BOOL difficult=FALSE,BOOL extraItems=FALSE);
	// using the same map as game2world, this sets objects at psuedo-random positions
	virtual void setObjectsPositionsAndRooms();
};

#endif // __GAME5WORLD_H__