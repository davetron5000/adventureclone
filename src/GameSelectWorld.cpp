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
#include "GameSelectWorld.h"
#include "Key.h"
#include "Wall.h"
#include "Game1World.h"
#include "Game2World.h"
#include "Game3World.h"
#include "Game4World.h"
#include "Game5World.h"
#include "ConnectionTestWorld.h"
#include "ObjectTestWorld.h"
#include "EventDispatcher.h"

const int GameSelectWorld::theNumberOfGames = 5;

BOOL GameSelectWorld::init(DWORD currentTime, LPDIRECTDRAW directDrawObject)
{
	INIT_WORLD();

	for (int i=0;i<10;i++)
	{
		char filename[20];
		sprintf(filename,"%d.bmp",i);

		itsNumberImages[i] = new ScreenImage();
		itsNumberImages[i]->init(directDrawObject,its8BitReader,filename,&colorKey);
		itsImages->addElement(itsNumberImages[i]);
	}

	// room 0
	NEW_ROOM("CustomGame1",bgImage,56,FALSE);

		ADD_WALL(0,0,640,32,56);

		ADD_WALL(0,0,32,384,56);
		ADD_WALL(608,0,32,384,56);

		ADD_WALL(0,352,256,32,56);
		ADD_WALL(384,352,256,32,56);

	END_NEW_ROOM(TRUE,FALSE);

	// room 1
	NEW_ROOM("CustomGame2",bgImage,145,FALSE);

		ADD_WALL(0,0,256,32,145);
		ADD_WALL(384,0,256,32,145);

		ADD_WALL(0,352,640,32,145);

		// IMPORTANT!!! The order of the walls must stay the same!
		ADD_WALL(24,0,4,384,0);
		ADD_WALL(616,0,4,384,0);

		ADD_WALL(544,0,32,320,145);
		ADD_WALL(576,0,64,256,145);
		ADD_WALL(608,288,32,96,145);


	END_NEW_ROOM(FALSE,FALSE);

	// room 2
	NEW_ROOM("SelectGameCustom",bgImage,3,FALSE);
	END_NEW_ROOM(FALSE,FALSE);

	// room 3
	NEW_ROOM("SelectGameCustomDifficult",bgImage,3,FALSE);
	END_NEW_ROOM(FALSE,FALSE);

	MAKE_ROOM_CONNECTION(0,1,RoomConnector::CONNECTION_LOCATION_BOTTOM);

	MAKE_ROOM_CONNECTION(1,0,RoomConnector::CONNECTION_LOCATION_TOP);
	MAKE_ROOM_CONNECTION(1,2,RoomConnector::CONNECTION_LOCATION_LEFT);
	MAKE_ROOM_CONNECTION(1,3,RoomConnector::CONNECTION_LOCATION_RIGHT);


	for (int j=0;j<10;j++)
	{
		int y = 34 + (j*32);
		if ( ((theNumberOfGames) <= 9) && j == 0)
			continue;
		CREATE_NUMBER(64,y,0,j);
		if ((theNumberOfGames) > 9)
		{
			CREATE_NUMBER(560,y,0,j);
		}
		else if ((theNumberOfGames) <= j)
		{
			break;
		}
	}

	CREATE_EXTRA_ITEM_THINGY(145,290,1);
	CREATE_TEXT(8,162,1,"easy.bmp");
	CREATE_TEXT(486,330,1,"difficult.bmp");

	itsPlayerStartX = 312;
	itsPlayerStartY = 64;
	CREATE_PLAYER();

	END_WORLD();

	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_DROP,this,GameObject::GAMEOBJECT_TYPE_NUMBER);
	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_GRAB,this,GameObject::GAMEOBJECT_TYPE_NUMBER);
	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_ENTER,this,GameObject::GAMEOBJECT_TYPE_PLAYER);
	return TRUE;

}

void GameSelectWorld::handleOpenableWalls(Room *room)
{
	char *roomName = room->getName();
	if (strcmp(roomName,"CustomGame2") == 0)
	{
		// yes, this is a bit cheesy, but it's a hell of a lot easier
		((Wall*)(room->getWalls()->elementAt(3)))->changeColor(0);
		((Wall*)(room->getWalls()->elementAt(4)))->changeColor(0);
		int gameNumber = getGameNumber(room);
		if (isValidGameNumber(gameNumber))
		{
			Logger::log("Valid game");
			// again with the cheese
			((Wall*)(room->getWalls()->elementAt(3)))->changeColor(ScreenImage::theTransparencyColor);
			((Wall*)(room->getWalls()->elementAt(4)))->changeColor(ScreenImage::theTransparencyColor);
		}
	}
}

void GameSelectWorld::onObjectDrop(GameObject *object, GameObject *dropper)
{
	Room *room = dropper->getRoom();
	handleOpenableWalls(room);
}

void GameSelectWorld::onObjectGrab(GameObject *object, GameObject *grabber)
{
	Room *room = grabber->getRoom();
	handleOpenableWalls(room);
}

void GameSelectWorld::onEnterRoom(GameObject *object, Room *room)
{
	handleOpenableWalls(room);
}

int GameSelectWorld::getGameNumber(Room *room)
{
	NumberObject *first = NULL, *second = NULL;
	int gameNumber = 0;

	for (UINT i=0;i<room->getObjects()->length();i++)
	{
		if ( (first != NULL) && (second != NULL) )
			break;
		GameObject *object = (GameObject *)(room->getObjects()->elementAt(i));
		if ( (object->getType() == GameObject::GAMEOBJECT_TYPE_NUMBER) &&
			 (object->getIsBeingHeld() == FALSE) )
		{
			if (first != NULL)
				second = (NumberObject *)object;
			else
				first = (NumberObject *)object;
		}
	}
	if (first != NULL)
	{
		

		if (second == NULL)
		{
			gameNumber = first->getNumber();
		}
		else
		{
			// the game number is the number as readable on screen (ignoring vertical
			// position)
			if (first->getX() < second->getX())
				gameNumber = ( first->getNumber() * 10 ) + second->getNumber();
			else
				gameNumber = ( second->getNumber() * 10 ) + first->getNumber();
		}
	}
	return gameNumber;

}

BOOL GameSelectWorld::isValidGameNumber(int number)
{
	// hard coded for now.
	return ( (number <= (theNumberOfGames+2)) && (number > 0) );
}

World *GameSelectWorld::getWorld()
{
	char *name = itsPlayer->getRoom()->getName();
	if ( (strcmp(name,"SelectGameCustom") == 0) || (strcmp(name,"SelectGameCustomDifficult") == 0) )
	{
		// OK, figure it out
		int gameNumber = 1; // just in case
		for (UINT i=0;i<itsRooms->length();i++)
		{
			if (strcmp("CustomGame2",((Room*)(itsRooms->elementAt(i)))->getName()) == 0)
			{
				gameNumber = getGameNumber( ((Room*)itsRooms->elementAt(i)) );
				break;
			}
		}

		BOOL hard = strcmp(name,"SelectGameCustomDifficult") == 0;
		BOOL extras = FALSE;

		if ( (itsPlayer->getCarriedObject() != NULL) &&
			(itsPlayer->getCarriedObject()->getType() == GameObject::GAMEOBJECT_TYPE_EXTRAITEMS) )
		{
			extras = TRUE;
		}
		switch (gameNumber)
		{
		case 1 :
			return new Game1World(hard,extras);
			break;
		case 2 :
			return new Game2World(hard,extras);
			break;
		case 3 :
			return new Game3World(hard,extras);
			break;
		case 4 :
			return new Game4World(hard,extras);
			break;
		case 5 :
			return new Game5World(hard,extras);
			break;
		case (theNumberOfGames+1) :
			return new ConnectionTestWorld();
			break;
		case (theNumberOfGames+2) :
			return new ObjectTestWorld();
			break;
		default :
			return new Game1World();
			
		}
	}
	return NULL;
}
