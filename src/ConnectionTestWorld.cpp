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
#include "ConnectionTestWorld.h"
#include "Game.h"
#include "Player.h"
#include "Key.h"
#include "TransmolecularDot.h"
#include "Magnet.h"
#include "Gate.h"

BOOL ConnectionTestWorld::init(DWORD currentTime, LPDIRECTDRAW directDrawObject)
{
    INIT_WORLD();

	SimpleArray *itsChaliceImages = NULL;
	SimpleArray *itsGateImages = NULL;
	// room 0
    NEW_ROOM("Center",bgImage,32,FALSE);

		ADD_WALL(0,0,32,32,32);
		ADD_WALL(0,Game::theScreenHeight - 32,32,32,32);
		ADD_WALL(Game::theScreenWidth - 32,0,32,32,32);
		ADD_WALL(Game::theScreenWidth - 32,Game::theScreenHeight - 32,32,32,32);
    
    END_NEW_ROOM(TRUE,FALSE);

	// room 1
    NEW_ROOM("Left",bgImage,33,FALSE);

		ADD_WALL(0,0,Game::theScreenWidth,32,33);
		ADD_WALL(0,0,32,Game::theScreenHeight,33);
		ADD_WALL(0,Game::theScreenHeight-32,Game::theScreenWidth,32,33);
    
    END_NEW_ROOM(FALSE,FALSE);

	// room 2
    NEW_ROOM("Right",bgImage,34,FALSE);

		ADD_WALL(0,0,Game::theScreenWidth,32,34);
		ADD_WALL(Game::theScreenWidth-32,0,32,Game::theScreenHeight,34);
		ADD_WALL(0,Game::theScreenHeight-32,Game::theScreenWidth,32,34);
    
    END_NEW_ROOM(FALSE,FALSE);

	// room 3
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

		ADD_GATE(1,goldGate);

    END_NEW_ROOM(FALSE,FALSE);
	
	// room 4
    NEW_ROOM("Bottom",bgImage,36,FALSE);

		ADD_WALL(0,0,32,Game::theScreenHeight,36);
		ADD_WALL(Game::theScreenWidth-32,0,32,Game::theScreenHeight,36);
		ADD_WALL(0,Game::theScreenHeight-32,Game::theScreenWidth,32,36);
    
    END_NEW_ROOM(FALSE,FALSE);

	// room 5
	NEW_ROOM("InsideYellow",bgImage,207,FALSE);

		ADD_WALL(0,Game::theScreenHeight-32,256,32,207);
		ADD_WALL(384,Game::theScreenHeight-32,256,32,207);
		ADD_WALL(0,0,Game::theScreenWidth,32,207);
		ADD_WALL(0,0,32,Game::theScreenHeight,207);
		ADD_WALL(Game::theScreenWidth-32,0,32,Game::theScreenHeight,207);

    END_NEW_ROOM(FALSE,TRUE);

	MAKE_GATE_CONNECTION(3,5,goldGate);
	MAKE_CASTLE_EXIT(5,3,goldGate);

	
    MAKE_ROOM_CONNECTION(0,1,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(0,2,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(0,3,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_ROOM_CONNECTION(0,4,RoomConnector::CONNECTION_LOCATION_BOTTOM);

    MAKE_ROOM_CONNECTION(1,0,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(2,0,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(3,0,RoomConnector::CONNECTION_LOCATION_BOTTOM);
	MAKE_ROOM_CONNECTION(4,0,RoomConnector::CONNECTION_LOCATION_TOP);

	MAKE_ROOM_CONNECTION(1,1,RoomConnector::CONNECTION_LOCATION_BOTTOM);
	MAKE_ROOM_CONNECTION(1,1,RoomConnector::CONNECTION_LOCATION_TOP);

	MAKE_ROOM_CONNECTION(2,2,RoomConnector::CONNECTION_LOCATION_BOTTOM);
	MAKE_ROOM_CONNECTION(2,2,RoomConnector::CONNECTION_LOCATION_TOP);

    MAKE_ROOM_CONNECTION(3,3,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(3,3,RoomConnector::CONNECTION_LOCATION_LEFT);

    MAKE_ROOM_CONNECTION(4,4,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(4,4,RoomConnector::CONNECTION_LOCATION_LEFT);

    MAKE_ROOM_CONNECTION(1,2,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(2,1,RoomConnector::CONNECTION_LOCATION_RIGHT);
	MAKE_ROOM_CONNECTION(3,4,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_ROOM_CONNECTION(4,3,RoomConnector::CONNECTION_LOCATION_BOTTOM);

	itsPlayerStartX = 312;
	itsPlayerStartY = Game::theScreenHeight - 64;
	CREATE_PLAYER();
	CREATE_SWORD(64,Game::theScreenHeight - 64,0);

	CREATE_KEY(1,96,Game::theScreenHeight / 2,0);
	CREATE_KEY(2,126,Game::theScreenHeight / 2,0);
	CREATE_KEY(3,156,Game::theScreenHeight / 2,0);

	CREATE_KEY(3,96,Game::theScreenHeight / 2 + 15,0);
	CREATE_KEY(2,126,Game::theScreenHeight / 2 + 15,0);
	CREATE_KEY(1,156,Game::theScreenHeight / 2 + 15,0);

	CREATE_CHALICE(400,64,0);
	CREATE_BRIDGE(300,64,0);
	CREATE_MAGNET(300,200,0);
	CREATE_DOT(16,16,0);

	END_WORLD();

	return TRUE;
}
