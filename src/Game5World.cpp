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
#include "Game5World.h"
#include <RandomGenerator.h>

inline int Game5World::getRandomRoom(BOOL chalice, int empty)
{
	int loopTimes = 0;
	int randNum = RandomGenerator::getRandomNumber(GAME4_NUMBER_OF_ROOMS);
	do
	{
		Room *room = (Room *)(itsRooms->elementAt(randNum));
		if (isRoomSuitable(room,randNum,chalice,empty))
		{
			usedRooms[randNum]++;
			return randNum;
		}
		loopTimes++;
		randNum = RandomGenerator::getRandomNumber(GAME4_NUMBER_OF_ROOMS);
	}
	while (loopTimes < 1000);
	Logger::error("Random number generation failed to find a room");
	// OK, something's wrong, so let's just iteratively find one
	for (int i=0;i<GAME4_NUMBER_OF_ROOMS;i++)
	{
		Room *room = (Room *)(itsRooms->elementAt(i));
	
		if (isRoomSuitable(room,i,chalice,empty))
		{
			usedRooms[i]++;
			return i;
		}
	}
	Logger::error("A room was still not found, so therefore there is a problem with the world, since there are more objects to be placed randomly than there are rooms");
	assert(FALSE);
	return -1;
}

Game5World::Game5World(BOOL difficult,BOOL extraItems) : Game4World(difficult,extraItems) {}

inline BOOL Game5World::isRoomSuitable(Room *room, int number, BOOL chalice, int empty)
{
	if ( (room->canObjectStart()) && ( (chalice && room->canChaliceStart()) || !chalice ) )
	{
		if (usedRooms[number] <= empty)
			return TRUE;
	}
	return FALSE;
}

void Game5World::setObjectsPositionsAndRooms()
{
	for (int i=0;i<GAME4_NUMBER_OF_ROOMS;i++) { usedRooms[i] = 0; }

	itsYellowKeyRoom = getRandomRoom(FALSE,0);
	itsYellowKeyX = -1;

	itsWhiteKeyRoom = getRandomRoom(FALSE,0);
	itsWhiteKeyX = -1;

	itsBlackKeyRoom = getRandomRoom(FALSE,0);
	itsBlackKeyX = -1;

	itsSwordRoom = getRandomRoom(FALSE,0);
	itsSwordX = -1;

	itsMagnetRoom = getRandomRoom(FALSE,0);
	itsMagnetX = -1;

	itsBridgeRoom = getRandomRoom(FALSE,0);
	itsBridgeX = -1;

	itsBridge2Room = getRandomRoom(FALSE,0);
	itsBridge2X = -1;

	itsChaliceRoom = getRandomRoom(TRUE,0);
	itsChaliceX = -1;

	itsArmourRoom = getRandomRoom(TRUE,0);
	itsArmourX = -1;

	itsCandleRoom = getRandomRoom(TRUE,0);
	itsCandleX = -1;

	itsYellowDragonRoom = getRandomRoom(FALSE,1);
	itsYellowDragonX = 320;
	itsYellowDragonY = 0;

	itsGreenDragonRoom = getRandomRoom(FALSE,1);
	itsGreenDragonX = 544;
	itsGreenDragonY = 160;

	itsRedDragonRoom = getRandomRoom(FALSE,1);
	itsRedDragonX = 72;
	itsRedDragonY = 224;

	itsPurpleDragonRoom = getRandomRoom(FALSE,1);
	itsPurpleDragonX = -1;

	itsBatRoom = getRandomRoom(FALSE,2);
	itsBatX = 64;
	itsBatY = 0;


}