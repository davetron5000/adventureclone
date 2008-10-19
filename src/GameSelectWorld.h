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
#include "NumberObject.h"
#include "Room.h"

#ifndef __GAMESELECTWORLD_H__
#define __GAMESELECTWORLD_H__

// This is a special world that allows the player to select the game they want.
// as such, there is a method called "getWorld()" that will return a freshly created
// World object based on what game they chose.  The caller is responsible for 
// freeing the memory needed for that world.
class GameSelectWorld : public World, public EventListener
{
protected:
	ScreenImage * itsNumberImages[10];

	BOOL isValidGameNumber(int number);

	static const int theNumberOfGames;

	int getGameNumber(Room *room);

	// Encapsulates the logic to "open" the side walls if the player has
	// selected a valid game.  This is broken out because there are several places
	// where we need to open/close the walls, so we can't have this is one of the
	// EventListener methods.
	void handleOpenableWalls(Room *room);

public:
	virtual BOOL init(DWORD currentTime, LPDIRECTDRAW directDrawObject);

	// this returns a pointer to a newly allocated world object that represents the players
	// choice of game.  The caller must delete this object when/if they are done with it.
	// If this returns NULL, the player hasn't chosen his game yet.
	virtual World *getWorld();

	virtual void onObjectDrop(GameObject *object, GameObject *dropper);

	virtual void onObjectGrab(GameObject *object, GameObject *grabber);

	virtual void onEnterRoom(GameObject *object, Room *room);
};

// put a number object in a room
#define CREATE_NUMBER(x,y,roomNumber,num) \
	{\
    NumberObject *number = new NumberObject();\
    number->init(currentTime,itsNumberImages[num],x,y,num);\
    itsObjects->addElement(number);\
    ((Room *)(itsRooms->elementAt(roomNumber)))->addObject(number,(x == -1 ? TRUE : FALSE));\
	}

// put a number object in a room
#define CREATE_TEXT(x,y,roomNumber,filename) \
	{\
    GameObject *object = new GameObject();\
	ScreenImage *image = new ScreenImage();\
	image->init(directDrawObject,its8BitReader,filename,&colorKey);\
	itsImages->addElement(image); \
    object->init(currentTime,image,x,y);\
    itsObjects->addElement(object);\
    ((Room *)(itsRooms->elementAt(roomNumber)))->addObject(object,(x == -1 ? TRUE : FALSE));\
	}

#define CREATE_EXTRA_ITEM_THINGY(x,y,roomNumber) \
	GameObject *obj = new GameObject(TRUE,FALSE,FALSE,GameObject::GAMEOBJECT_TYPE_EXTRAITEMS);\
	ScreenImage *exImage = new ScreenImage(); \
	exImage->init(directDrawObject,its8BitReader,"extraitems.bmp",&colorKey);\
	itsImages->addElement(exImage); \
	obj->init(currentTime,exImage,x,y); \
	itsObjects->addElement(obj); \
	((Room *)(itsRooms->elementAt(roomNumber)))->addObject(obj,FALSE); \

#endif __GAMESELECTWORLD_H__