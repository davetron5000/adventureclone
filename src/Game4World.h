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
#include "World.h"
#include "EventListener.h"

#include <SimpleArray.h>

#ifndef __GAME4WORLD_H__
#define __GAME4WORLD_H__


#define GAME4_NUMBER_OF_ROOMS 34

class Game4World : public World, public EventListener
{
protected:

	SimpleArray *itsGateImages;
	SimpleArray *itsChaliceImages;
	SimpleArray *itsBatImages;

    int itsYellowKeyRoom;
    int itsYellowKeyX;
    int itsYellowKeyY;

    int itsWhiteKeyRoom;
    int itsWhiteKeyX;
    int itsWhiteKeyY;

    int itsBlackKeyRoom;
    int itsBlackKeyX;
    int itsBlackKeyY;

    int itsYellowDragonRoom;
    int itsYellowDragonX;
    int itsYellowDragonY;

    int itsGreenDragonRoom;
    int itsGreenDragonX;
    int itsGreenDragonY;

    int itsRedDragonRoom;
    int itsRedDragonX;
    int itsRedDragonY;

	int itsPurpleDragonRoom;
	int itsPurpleDragonX;
	int itsPurpleDragonY;

    int itsSwordRoom;
    int itsSwordX;
    int itsSwordY;

    int itsMagnetRoom;
    int itsMagnetX;
    int itsMagnetY;

    int itsBatRoom;
    int itsBatX;
    int itsBatY;

    int itsDotRoom;
    int itsDotX;
    int itsDotY;

    int itsBridgeRoom;
    int itsBridgeX;
    int itsBridgeY;

    int itsBridge2Room;
    int itsBridge2X;
    int itsBridge2Y;

    int itsChaliceRoom;
    int itsChaliceX;
    int itsChaliceY;

	int itsCandleX;
	int itsCandleY;
	int itsCandleRoom;

	int itsArmourX;
	int itsArmourY;
	int itsArmourRoom;

	BOOL isMazeDark;
	int itsDragonMouthOpenTime;
	int itsNonRedDragonSpeed;

	BOOL isExtraExtraItems;

	// a subclass can override this to change the x,y or room of objects after the rooms
	// have been created, but before the objects are.
	virtual void setObjectsPositionsAndRooms() {}

public:
	Game4World(BOOL difficult=FALSE, BOOL extraItems=FALSE);

    // sets up the rooms and objects
    virtual BOOL init(DWORD currentTime, LPDIRECTDRAW directDrawObject);

    // release resources
    virtual void release();

	virtual void onEnterRoom(GameObject *object, Room *room);
};

#endif __GAME4WORLD_H__