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
#include "Game1World.h"
#include "Game.h"

#include "Gate.h"
#include "Dragon.h"
#include "Bat.h"
#include "Key.h"
#include "Magnet.h"

Game1World::Game1World(BOOL difficult, BOOL extraItems) : World(difficult,extraItems) 
{ 
	isMazeDark = difficult; 
	itsDragonMouthOpenTime = difficult ? 1000 : 2000;
}

BOOL Game1World::init(DWORD currentTime, LPDIRECTDRAW directDrawObject)
{
	itsGateImages = NULL;
	itsChaliceImages = NULL;
	itsBatImages = NULL;

	INIT_WORLD();

	// room 0
	// bottom goes to 10
    NEW_ROOM("BelowYellowCastle",bgImage,32,FALSE);

		ADD_WALL(0,0,256,32,32);
		ADD_WALL(384,0,256,32,32);
		ADD_WALL(0,Game::theScreenHeight-32,Game::theScreenWidth,32,32);
    
    END_NEW_ROOM(FALSE,FALSE);

	// room 1
    NEW_ROOM("YellowVestibule",bgImage,207,FALSE);

		int color=207;
		ADD_WALL(0,Game::theScreenHeight-32,256,32,color);
		ADD_WALL(384,Game::theScreenHeight-32,256,32,color);
		ADD_WALL(0,0,160,32,color);
		ADD_WALL(Game::theScreenWidth-160,0,160,32,color);
		ADD_WALL(0,0,32,Game::theScreenHeight,color);
		ADD_WALL(Game::theScreenWidth-32,0,32,Game::theScreenHeight,color);
		ADD_WALL(160,0,16,32,color);
		ADD_WALL(192,0,16,32,color);
		ADD_WALL(224,0,16,32,color);
		ADD_WALL(256,0,16,32,color);
		ADD_WALL(368,0,16,32,color);
		ADD_WALL(400,0,16,32,color);
		ADD_WALL(432,0,16,32,color);
		ADD_WALL(464,0,16,32,color);
		ADD_WALL(160,32,112,128,color);
		ADD_WALL(368,32,112,128,color);
		ADD_WALL(272,96,96,64,color);
		ADD_WALL(192,160,256,64,color);
		ADD_WALL(192,224,96,64,color);
		ADD_WALL(352,224,96,64,color);

		ADD_GATE(CASTLE_COLOR_GOLD,goldGate);

    END_NEW_ROOM(TRUE,FALSE);

	// room 2
	NEW_ROOM("InsideYellow",bgImage,207,FALSE);

		ADD_WALL(0,Game::theScreenHeight-32,256,32,207);
		ADD_WALL(384,Game::theScreenHeight-32,256,32,207);
		ADD_WALL(0,0,Game::theScreenWidth,32,207);
		ADD_WALL(0,0,32,Game::theScreenHeight,207);
		ADD_WALL(Game::theScreenWidth-32,0,32,Game::theScreenHeight,207);


    END_NEW_ROOM(FALSE,TRUE);

	// room 3
	// bottom goes to 11
	NEW_ROOM("LeftOfGreenRoom",bgImage,88,FALSE);
		ADD_WALL(0,0,256,32,88);
		ADD_WALL(384,0,256,32,88);

		ADD_WALL(0,Game::theScreenHeight-32,Game::theScreenWidth,32,88);
		
		ADD_CHANGING_WALL(40,0,8,Game::theScreenHeight,0);
		
    END_NEW_ROOM(FALSE,FALSE);

	// room 4
	NEW_ROOM("RightOfGreenRoom",bgImage,210,FALSE);

		ADD_WALL(0,Game::theScreenHeight-32,256,32,210);
		ADD_WALL(384,Game::theScreenHeight-32,256,32,210);
		ADD_WALL(0,0,Game::theScreenWidth,32,210);
		
		ADD_CHANGING_WALL(Game::theScreenWidth-48,0,8,Game::theScreenHeight,0);

    END_NEW_ROOM(FALSE,FALSE);

	// room 5
	// bottom goes to 11
	NEW_ROOM("GreenDragonRoom",bgImage,187,FALSE);
		ADD_WALL(0,0,256,32,187);
		ADD_WALL(384,0,256,32,187);

		ADD_WALL(0,0,32,Game::theScreenHeight,187);
		ADD_WALL(Game::theScreenWidth - 32,0,32,Game::theScreenHeight,187);
		ADD_WALL(0,Game::theScreenHeight-32,Game::theScreenWidth,32,187);
	END_NEW_ROOM(FALSE,FALSE);

	// room 6
	NEW_ROOM("Labrynth Entrance",bgImage,239,isMazeDark);

	    // left walls
		ADD_WALL(0,0,96,32,239);
		ADD_WALL(0,96,64,64,239);
		ADD_WALL(0,224,96,64,239);
		ADD_WALL(96,0,32,288,239);

		// right walls
		ADD_WALL(544,0,96,32,239);
		ADD_WALL(576,96,64,64,239);
		ADD_WALL(544,224,96,64,239);
		ADD_WALL(512,0,32,288,239);

		// middle walls
		ADD_WALL(160,224,320,64,239);
		ADD_WALL(160,0,32,224,239);
		ADD_WALL(224,0,32,96,239);
		ADD_WALL(224,96,192,64,239);
		
		ADD_WALL(288,0,64,32,239);

		ADD_WALL(384,0,32,96,239);
		ADD_WALL(448,0,32,224,239);

		// Bottom walls
		ADD_WALL(0,352,256,32,239);
		ADD_WALL(384,352,256,32,239);
	END_NEW_ROOM(FALSE,FALSE);

	// room 7
	NEW_ROOM("Labrynth 2nd Room",bgImage,239,isMazeDark);

		// left walls
		ADD_WALL(0,0,64,32,239);
		ADD_WALL(0,96,192,64,239);
		ADD_WALL(0,224,64,64,239);
		ADD_WALL(0,352,128,32,239);

		ADD_WALL(96,0,32,96,239);
		ADD_WALL(96,224,32,160,239);

		// right walls
		ADD_WALL(576,0,64,32,239);
		ADD_WALL(448,96,192,64,239);
		ADD_WALL(576,224,64,64,239);
		ADD_WALL(512,352,128,32,239);

		ADD_WALL(512,0,32,96,239);
		ADD_WALL(512,224,32,160,239);

		// middle walls
		ADD_WALL(160,96,32,288,239);
		ADD_WALL(448,96,32,288,239);

		ADD_WALL(160,0,128,32,239);
		ADD_WALL(352,0,128,32,239);

		ADD_WALL(224,0,64,288,239);
		ADD_WALL(352,0,64,288,239);

		ADD_WALL(224,288,32,96,239);
		ADD_WALL(384,288,32,96,239);

	END_NEW_ROOM(FALSE,FALSE);

	// room 8
	NEW_ROOM("Labrynth Exit",bgImage,239,isMazeDark);

		ADD_WALL(0,0,256,32,239);
		ADD_WALL(384,0,256,32,239);

		ADD_WALL(0,96,64,192,239);
		ADD_WALL(576,96,64,192,239);

		ADD_WALL(0,352,64,32,239);
		ADD_WALL(576,352,64,32,239);

		ADD_WALL(128,32,32,192,239);
		ADD_WALL(480,32,32,192,239);

		ADD_WALL(224,32,32,64,239);
		ADD_WALL(384,32,32,64,239);

		ADD_WALL(224,96,64,64,239);
		ADD_WALL(352,96,64,64,239);

		ADD_WALL(64,224,224,64,239);
		ADD_WALL(352,224,224,64,239);

		ADD_WALL(96,288,32,96,239);
		ADD_WALL(512,288,32,96,239);

		ADD_WALL(160,352,128,32,239);
		ADD_WALL(352,352,128,32,239);

		ADD_WALL(256,288,32,64,239);
		ADD_WALL(352,288,32,64,239);

	END_NEW_ROOM(FALSE,FALSE);

	// room 9
	// bottom goes to 4
	NEW_ROOM("Labrynth Room 3",bgImage,239,isMazeDark);

		ADD_WALL(0,0,128,32,239);
		ADD_WALL(512,0,128,32,239);

		ADD_WALL(160,0,32,32,239);
		ADD_WALL(448,0,32,32,239);

		ADD_WALL(224,0,32,160,239);
		ADD_WALL(384,0,32,160,239);

		ADD_WALL(96,32,32,128,239);
		ADD_WALL(512,32,32,128,239);

		ADD_WALL(128,96,96,64,239);
		ADD_WALL(416,96,96,64,239);

		ADD_WALL(0,96,64,192,239);
		ADD_WALL(576,96,64,192,239);

		ADD_WALL(64,224,64,64,239);
		ADD_WALL(512,224,64,64,239);

		ADD_WALL(96,288,32,64,239);
		ADD_WALL(512,288,32,64,239);

		ADD_WALL(0,352,640,32,239);

	END_NEW_ROOM(FALSE,FALSE);

	// room 10
	NEW_ROOM("Labrynth Room 4",bgImage,239,isMazeDark);

		ADD_WALL(0,0,640,32,239);

		ADD_WALL(0,96,160,64,239);
		ADD_WALL(480,96,160,64,239);

		ADD_WALL(0,160,64,128,239);
		ADD_WALL(576,160,64,128,239);

		ADD_WALL(0,352,128,32,239);
		ADD_WALL(512,352,128,32,239);

		ADD_WALL(192,96,256,64,239);

		ADD_WALL(288,160,64,224,239);

		ADD_WALL(96,224,160,64,239);
		ADD_WALL(384,224,160,64,239);

		ADD_WALL(96,288,32,64,239);
		ADD_WALL(512,288,32,64,239);

		ADD_WALL(160,352,32,32,239);
		ADD_WALL(448,352,32,32,239);

		ADD_WALL(224,288,32,96,239);
		ADD_WALL(384,288,32,96,239);

	END_NEW_ROOM(FALSE,FALSE);

	// room 11
    NEW_ROOM("BlackVestibule",bgImage,0,FALSE);

		int color=0;
		ADD_WALL(0,Game::theScreenHeight-32,256,32,color);
		ADD_WALL(384,Game::theScreenHeight-32,256,32,color);
		ADD_WALL(0,0,160,32,color);
		ADD_WALL(Game::theScreenWidth-160,0,160,32,color);
		ADD_WALL(0,0,32,Game::theScreenHeight,color);
		ADD_WALL(Game::theScreenWidth-32,0,32,Game::theScreenHeight,color);
		ADD_WALL(160,0,16,32,color);
		ADD_WALL(192,0,16,32,color);
		ADD_WALL(224,0,16,32,color);
		ADD_WALL(256,0,16,32,color);
		ADD_WALL(368,0,16,32,color);
		ADD_WALL(400,0,16,32,color);
		ADD_WALL(432,0,16,32,color);
		ADD_WALL(464,0,16,32,color);
		ADD_WALL(160,32,112,128,color);
		ADD_WALL(368,32,112,128,color);
		ADD_WALL(272,96,96,64,color);
		ADD_WALL(192,160,256,64,color);
		ADD_WALL(192,224,96,64,color);
		ADD_WALL(352,224,96,64,color);

		ADD_GATE(CASTLE_COLOR_BLACK,blackGate);

    END_NEW_ROOM(FALSE,FALSE);

	// room 12
	NEW_ROOM("InsideBlack",bgImage,180,FALSE);

		ADD_WALL(0,0,256,32,180);
		ADD_WALL(384,0,256,32,180);

		ADD_WALL(0,352,256,32,180);
		ADD_WALL(384,352,256,32,180);

		ADD_WALL(0,0,32,Game::theScreenHeight,180);
		ADD_WALL(608,0,32,Game::theScreenHeight,180);
		
	END_NEW_ROOM(FALSE,FALSE);

	// room 13
	NEW_ROOM("InsideBlack",bgImage,111,FALSE);

		ADD_WALL(0,0,640,32,111);

		ADD_WALL(0,352,256,32,111);
		ADD_WALL(384,352,256,32,111);

		ADD_WALL(0,0,32,Game::theScreenHeight,111);
		ADD_WALL(608,0,32,Game::theScreenHeight,111);
		
	END_NEW_ROOM(FALSE,FALSE);


	MAKE_ROOM_CONNECTION(0,10,RoomConnector::CONNECTION_LOCATION_BOTTOM);
	MAKE_ROOM_CONNECTION(0,1,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_ROOM_CONNECTION(0,3,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(0,4,RoomConnector::CONNECTION_LOCATION_RIGHT);
	
    MAKE_ROOM_CONNECTION(1,0,RoomConnector::CONNECTION_LOCATION_BOTTOM);
	MAKE_ROOM_CONNECTION(1,1,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(1,1,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(1,0,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_GATE_CONNECTION(1,2,goldGate);
	
	MAKE_ROOM_CONNECTION(2,2,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(2,2,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(2,2,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_CASTLE_EXIT(2,1,goldGate);

	MAKE_ROOM_CONNECTION(3,0,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(3,4,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(3,11,RoomConnector::CONNECTION_LOCATION_BOTTOM);
	MAKE_ROOM_CONNECTION(3,6,RoomConnector::CONNECTION_LOCATION_TOP);

	MAKE_ROOM_CONNECTION(4,0,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(4,3,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(4,5,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_ROOM_CONNECTION(4,5,RoomConnector::CONNECTION_LOCATION_BOTTOM);

	MAKE_ROOM_CONNECTION(5,11,RoomConnector::CONNECTION_LOCATION_BOTTOM);
	MAKE_ROOM_CONNECTION(5,4,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_ROOM_CONNECTION(5,5,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_ROOM_CONNECTION(5,5,RoomConnector::CONNECTION_LOCATION_BOTTOM);

	MAKE_ROOM_CONNECTION(6,3,RoomConnector::CONNECTION_LOCATION_BOTTOM);
	MAKE_ROOM_CONNECTION(6,7,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(6,7,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(6,10,RoomConnector::CONNECTION_LOCATION_TOP);

	MAKE_ROOM_CONNECTION(7,6,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(7,6,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(7,8,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_ROOM_CONNECTION(7,9,RoomConnector::CONNECTION_LOCATION_BOTTOM);

	MAKE_ROOM_CONNECTION(8,7,RoomConnector::CONNECTION_LOCATION_BOTTOM);
	MAKE_ROOM_CONNECTION(8,9,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(8,10,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(8,11,RoomConnector::CONNECTION_LOCATION_TOP);

	MAKE_ROOM_CONNECTION(9,7,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_ROOM_CONNECTION(9,10,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(9,8,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(9,4,RoomConnector::CONNECTION_LOCATION_BOTTOM);

	MAKE_ROOM_CONNECTION(10,9,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(10,6,RoomConnector::CONNECTION_LOCATION_BOTTOM);
	MAKE_ROOM_CONNECTION(10,8,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(10,0,RoomConnector::CONNECTION_LOCATION_TOP);

	MAKE_ROOM_CONNECTION(11,5,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_ROOM_CONNECTION(11,8,RoomConnector::CONNECTION_LOCATION_BOTTOM);
	MAKE_ROOM_CONNECTION(11,11,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(11,11,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_GATE_CONNECTION(11,12,blackGate);

	MAKE_ROOM_CONNECTION(12,12,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(12,12,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(12,13,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_CASTLE_EXIT(12,11,blackGate);

	MAKE_ROOM_CONNECTION(13,12,RoomConnector::CONNECTION_LOCATION_BOTTOM);
	MAKE_ROOM_CONNECTION(13,13,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(13,13,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(13,12,RoomConnector::CONNECTION_LOCATION_TOP);

	itsPlayerStartX = 312;
	itsPlayerStartY = Game::theScreenHeight - 64;
	CREATE_PLAYER();


	// other objects

	CREATE_KEY(CASTLE_COLOR_GOLD,144,Game::theScreenHeight / 2 - 16,1);
	CREATE_DRAGON(128,128,DRAGON_COLOR_YELLOW,3,thisKey,NULL,0,0,itsDragonMouthOpenTime);
	CREATE_KEY(CASTLE_COLOR_BLACK,96,Game::theScreenHeight / 2,5);
	CREATE_DRAGON(128,128,DRAGON_COLOR_GREEN,5,thisKey,NULL,0,0,itsDragonMouthOpenTime);

	CREATE_SWORD(64,Game::theScreenHeight - 64,2);
	CREATE_BRIDGE(152,200,8);
	CREATE_MAGNET(Game::theScreenWidth - 128,Game::theScreenHeight - 96,12);
	CREATE_CHALICE(96,Game::theScreenHeight - 128,13);

	assert(chalice != NULL);
	assert(sword != NULL);

	END_WORLD();

    return TRUE;
}

void Game1World::release()
{
	if (itsChaliceImages != NULL)
		delete itsChaliceImages;
	if (itsGateImages != NULL)
		delete itsGateImages;
	if (itsBatImages != NULL)
		delete itsBatImages;
}
