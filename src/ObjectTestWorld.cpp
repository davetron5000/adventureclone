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
#include "ObjectTestWorld.h"
#include "Game.h"
#include "Player.h"
#include "Key.h"
#include "TransmolecularDot.h"
#include "Magnet.h"

BOOL ObjectTestWorld::init(DWORD currentTime, LPDIRECTDRAW directDrawObject)
{
    INIT_WORLD();

	SimpleArray *itsChaliceImages = NULL;
	// room 0
    NEW_ROOM("Room",bgImage,32,FALSE);

		ADD_WALL(0,0,Game::theScreenWidth,32,32);
		ADD_WALL(0,0,32,Game::theScreenHeight,32);
		ADD_WALL(Game::theScreenWidth-32,0,32,Game::theScreenHeight,32);
		ADD_WALL(0,Game::theScreenHeight-32,Game::theScreenWidth,32,32);
    
    END_NEW_ROOM(TRUE,FALSE);

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