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
#include <windowsx.h>
#include <ddraw.h>

#include <ScreenImage.h>
#include "GlowingScreenImage.h"
#include "Player.h"
#include "Room.h"
#include <Bmp8BitFileReader.h>
#include "RoomConnector.h"

#ifndef __WORLD_H__
#define __WORLD_H__

#define CASTLE_COLOR_GOLD 207
#define CASTLE_COLOR_WHITE 246
#define CASTLE_COLOR_BLACK 0

#define DRAGON_COLOR_YELLOW 207
#define DRAGON_COLOR_GREEN 100
#define DRAGON_COLOR_RED 182

// A "World" is a description of the setting of the game.  It basically contains
// all the rooms and all the objects and is responsible for the creation and release
// of said objects.  While this implementation has much hard-coded, a future implementation
// could initialize the "world" from a descriptor file or something.  Subclasses should implement init()
// and release() to provide their customized world
// The protected arrays are for memory management.  Basically, when you new something, put it into it's
// appropriate array.  Then, during release(), the objects will be deleted for you.
class World
{
protected:

	UINT itsPlayerStartX, itsPlayerStartY;
    // All the rooms in the world.  When you create a room in init(), add it to this array
    SimpleArray *itsRooms;
    // All the objects in the world.  When you create _any_ object other than a room, add it to this array
    SimpleArray *itsObjects;

    // List of all images we created.  When you create _any_ image, add it to this array
    SimpleArray *itsImages;

    // All the RoomConnectors we created.  When you create _any_ connector, add it to this array
    SimpleArray *itsConnectors;

    Room *itsStartingRoom;

	// This is the room where the player must bring the chalice
	Room *itsWinningRoom;

    ImageFileReader *its8BitReader;

    Player *itsPlayer;

	// if TRUE, this is a "difficult" version of this world
	BOOL isDifficult;

	// if TRUE, extra items not in the original adventure game are used
	BOOL isExtraItems;

public:

    World(BOOL difficult=FALSE, BOOL extraItems=FALSE);
    
    // sets up the rooms and objects.  Subclasses must implement this as this is where all the work
	// is done
    virtual BOOL init(DWORD currentTime, LPDIRECTDRAW directDrawObject) = 0;

    // release resources.  This method releases resources stored in the arrays mentioned above.  Subclasses
	// will probably need to override this for their own purposes, but be sure to call it.
    virtual void release();

	// a soft reset basically ressurects all the dragons and puts the player back at
	// the starting location alive and well
	virtual void softReset();

    // called at the start of the game, this returns the starting room
    inline Room *getStartingRoom()   { return itsStartingRoom; }
    // returns a pointer to the player objects
    inline Player *getPlayer()       { return itsPlayer; }

    inline SimpleArray *getRooms()   { return itsRooms; }
    inline SimpleArray *getObjects() { return itsObjects; }

	inline Room *getWinningRoom()    { return itsWinningRoom; }
};


// These are macros that are useful in creating your world.  Basically, you create rooms first using NEW_ROOM,
// ADD_WALL, and END_NEW_ROOM.  Then, create your objects and connections.  Objects and connections are created
// by referring to the room number's index.  This index is the 0-based index according to the order in which rooms
// are created.  Look at Game1World for an example usage.

// Begin the definition of a room
#define NEW_ROOM(name,bgImage,primaryColor,isDark) \
{ \
    Room *room = new Room(); \
	int primColor = isDark ? bgImage->getColor(0,0) : primaryColor;\
    room->init(bgImage,name,isDark ? darkBgImage : NULL,128,primColor); \
    Wall *tmpWall = NULL; \
    LPDIRECTDRAWPALETTE palette = room->getImage()->getPalette();

// OR, use this one if you are going to be randomly assigning objects
#define NEW_ROOM_EXTRA(name,bgImage,primaryColor,isDark,isChalice,isOthers,x,y) \
{ \
    Room *room = new Room(); \
	int primColor = isDark ? bgImage->getColor(0,0) : primaryColor;\
    room->init(bgImage,name,isDark ? darkBgImage : NULL,128,primColor,isChalice,isOthers,x,y); \
    Wall *tmpWall = NULL; \
    LPDIRECTDRAWPALETTE palette = room->getImage()->getPalette();
		
#define CREATE_SWORD(x,y,roomNumber) \
    newImage = new ScreenImage();\
    newImage->init(directDrawObject,its8BitReader,"sword.bmp",&colorKey);\
    itsImages->addElement(newImage);\
    GameObject *sword = new GameObject(TRUE,TRUE,TRUE,GameObject::GAMEOBJECT_TYPE_SWORD);\
    sword->init(currentTime,newImage,x,y);\
    itsObjects->addElement(sword);\
    ((Room *)(itsRooms->elementAt(roomNumber)))->addObject(sword,(x == -1 ? TRUE : FALSE))\

#define CREATE_BAT(x,y,roomNumber) \
	if (itsBatImages == NULL)\
	{\
		itsBatImages = new SimpleArray(2);\
		newImage = new ScreenImage();\
		newImage->init(directDrawObject,its8BitReader,"bat00.bmp",&colorKey);\
		itsImages->addElement(newImage);\
		itsBatImages->addElement(newImage);\
		newImage = new ScreenImage();\
		newImage->init(directDrawObject,its8BitReader,"bat01.bmp",&colorKey);\
		itsImages->addElement(newImage);\
		itsBatImages->addElement(newImage);\
	}\
    Bat *bat = new Bat(isDifficult);\
    bat->init(currentTime,itsBatImages,x,y,250);\
	bat->startAnimating(currentTime);\
	bat->startMoving(currentTime,0,350);\
    itsObjects->addElement(bat);\
    ((Room *)(itsRooms->elementAt(roomNumber)))->addObject(bat,(x == -1 ? TRUE : FALSE))

// prot is the GameObject the dragon will protect
// fear is the GameObject the dragon fears
// pers is the dragon's "persistence" when pursuing the player
// speed is the dragon's movement speed
// mouthOpen is how long he will leave his mouth open when attacking.
// these can be set to 0 to use the defaults
#define CREATE_DRAGON(x,y,color,roomNumber,prot,fear,pers,speed,mouthOpen) \
	{ \
		newImage = new ScreenImage(); \
		ScreenImage *deathImage,*attackImage; \
		deathImage = new ScreenImage(); \
		attackImage = new ScreenImage(); \
		newImage->init(directDrawObject,its8BitReader,"dragon-normal.bmp",&colorKey); \
		attackImage->init(directDrawObject,its8BitReader,"dragon-attack.bmp",&colorKey); \
		deathImage->init(directDrawObject,its8BitReader,"dragon-dead.bmp",&colorKey); \
 \
		newImage->changeColor(color);\
		attackImage->changeColor(color); \
		deathImage->changeColor(color); \
		itsImages->addElement(newImage); \
		itsImages->addElement(attackImage); \
		itsImages->addElement(deathImage); \
 \
		Dragon *dragon = new Dragon(); \
		RECT headRect; \
		headRect.left = 0; \
		headRect.right = newImage->getWidth(); \
		headRect.top = 0; \
		headRect.bottom = 40; \
		dragon->init(currentTime,newImage,attackImage,deathImage,headRect,11,47,x,y); \
		if (pers > 0) dragon->setPersistence(pers); \
		if (speed > 0) dragon->setSpeed(speed); \
		if (mouthOpen > 0) dragon->setMouthOpenTime(mouthOpen); \
		dragon->setProtectedObject(prot); \
		dragon->setFearedObject(fear); \
		itsObjects->addElement(dragon); \
		((Room*)(itsRooms->elementAt(roomNumber)))->addObject(dragon,(x == -1 ? TRUE : FALSE)); \
	}

#define CREATE_BRIDGE(x,y,roomNumber) \
    newImage = new ScreenImage();\
    newImage->init(directDrawObject,its8BitReader,"bridge.bmp",&colorKey);\
    itsImages->addElement(newImage);\
    bridge = new GameObject(TRUE,TRUE,TRUE,GameObject::GAMEOBJECT_TYPE_BRIDGE);\
    bridge->init(currentTime,newImage,x,y);\
    itsObjects->addElement(bridge);\
    ((Room *)(itsRooms->elementAt(roomNumber)))->addObject(bridge,(x == -1 ? TRUE : FALSE))

#define CREATE_CANDLE(x,y,roomNumber) \
	GameObject *candle = NULL;\
	if (isExtraItems) \
	{\
		newImage = new ScreenImage();\
		newImage->init(directDrawObject,its8BitReader,"candle.bmp",&colorKey);\
		itsImages->addElement(newImage);\
		candle = new GameObject(TRUE,TRUE,FALSE,GameObject::GAMEOBJECT_TYPE_CANDLE);\
		candle->init(currentTime,newImage,x,y);\
		itsObjects->addElement(candle);\
		((Room *)(itsRooms->elementAt(roomNumber)))->addObject(candle,(x == -1 ? TRUE : FALSE));\
	}\

#define CREATE_ARMOUR(x,y,roomNumber) \
	GameObject *armour = NULL;\
	if (isExtraItems) \
	{\
		newImage = new ScreenImage();\
		newImage->init(directDrawObject,its8BitReader,"armour.bmp",&colorKey);\
		itsImages->addElement(newImage);\
		armour = new GameObject(TRUE,TRUE,TRUE,GameObject::GAMEOBJECT_TYPE_ARMOUR);\
		armour->init(currentTime,newImage,x,y);\
		itsObjects->addElement(armour);\
		((Room *)(itsRooms->elementAt(roomNumber)))->addObject(armour,(x == -1 ? TRUE : FALSE));\
	}

#define CREATE_MAGNET(x,y,roomNumber) \
    newImage = new ScreenImage();\
    newImage->init(directDrawObject,its8BitReader,"magnet.bmp",&colorKey);\
    itsImages->addElement(newImage);\
    Magnet *magnet = new Magnet();\
    magnet->init(currentTime,newImage,x,y);\
    itsObjects->addElement(magnet);\
    ((Room *)(itsRooms->elementAt(roomNumber)))->addObject(magnet,(x == -1 ? TRUE : FALSE))

#define CREATE_DOT(x,y,roomNumber) \
    newImage = new ScreenImage();\
    newImage->init(directDrawObject,its8BitReader,"dot.bmp",&colorKey);\
    itsImages->addElement(newImage);\
    TransmolecularDot *dot = new TransmolecularDot();\
    dot->init(currentTime,newImage,x,y);\
    itsObjects->addElement(dot);\
    ((Room *)(itsRooms->elementAt(roomNumber)))->addObject(dot)

#define ADD_GATE(castleId,gatePtr) \
	{\
		if (itsGateImages == NULL)\
		{\
			itsGateImages = new SimpleArray(3); \
			char filename[128];\
			for (int i=0;i<5;i++)\
			{\
				sprintf(filename,"gate%02d.bmp",i);\
				newImage = new ScreenImage();\
				newImage->init(directDrawObject,its8BitReader,filename,&colorKey);\
				itsGateImages->addElement(newImage);\
				itsImages->addElement(newImage);\
			}\
		}\
		gate = new Gate(); \
		gate->init(currentTime,itsGateImages,306,224,200,castleId); \
		gatePtr = gate;\
		itsObjects->addElement(gate); \
		room->addObject(gate); \
	}

#define MAKE_GATE_CONNECTION(x,y,gateObject) \
	{\
		Room *room1,*room2;\
		room1 = (Room *)(itsRooms->elementAt(x));\
        room2 = (Room *)(itsRooms->elementAt(y));\
		RECT rect;\
		rect.left = 314;\
		rect.top = 240;\
		rect.right = 326;\
		rect.bottom = 248;\
		RoomConnector *con = new RoomConnector(room1,room2,RoomConnector::CONNECTION_LOCATION_ARBITRARY,\
			RoomConnector::CONNECTION_LOCATION_BOTTOM,rect,rect,FALSE,gateObject);\
		itsConnectors->addElement(con);\
		room1->addConnection(con);\
		\
	}\

#define CREATE_CHALICE(x,y,roomNumber) \
    newImage = new GlowingScreenImage();\
    newImage->init(directDrawObject,its8BitReader,"chalice.bmp",&colorKey);\
    itsImages->addElement(newImage);\
    GameObject *chalice = new GameObject(TRUE,TRUE,TRUE,GameObject::GAMEOBJECT_TYPE_CHALICE);\
    chalice->init(currentTime,newImage,x,y);\
    itsObjects->addElement(chalice);\
    ((Room *)(itsRooms->elementAt(roomNumber)))->addObject(chalice,(x == -1 ? TRUE : FALSE))

#define CREATE_EASTEREGG(x,y,roomNumber,filebase,imageList) \
	{\
    newImage = new GlowingScreenImage();\
	char filename[128];\
	sprintf(filename,"%s.bmp",filebase);\
    newImage->init(directDrawObject,its8BitReader,filename,&colorKey);\
    itsImages->addElement(newImage);\
    GameObject *easterEgg = new GameObject(FALSE,FALSE,FALSE,GameObject::GAMEOBJECT_TYPE_WALL);\
    easterEgg->init(currentTime,newImage,x,y);\
    itsObjects->addElement(easterEgg);\
    ((Room *)(itsRooms->elementAt(roomNumber)))->addWall(easterEgg);\
    ((Room *)(itsRooms->elementAt(roomNumber)))->addObject(easterEgg,(x == -1 ? TRUE : FALSE)); \
	}

// Add a wall to the current room.  Must be between NEW_ROOM and END_NEW_ROOM
#define ADD_WALL(x,y,w,h,color) \
    tmpWall = new Wall(); \
    tmpWall->init(currentTime,x,y,w,h,room->getIsDark() ? primColor : color,directDrawObject,palette,FALSE,1,&colorKey); \
	room->addWall(tmpWall); \
    itsObjects->addElement(tmpWall)

#define ADD_CHANGING_WALL(x,y,w,h,color) \
    tmpWall = new Wall(); \
    tmpWall->init(currentTime,x,y,w,h,room->getIsDark() ? primColor : color,directDrawObject,palette,TRUE,2,&colorKey); \
	room->addWall(tmpWall); \
    itsObjects->addElement(tmpWall)

// End the definition of a room.  If is_starting is true, this room will be the
// starting room for this world
#define END_NEW_ROOM(is_starting,is_winning) \
    if (is_starting) \
        itsStartingRoom = room; \
	if (is_winning) \
		itsWinningRoom = room; \
    itsRooms->addElement(room); \
}


// Creates a connection between the two rooms for non-arbitrary connections
#define MAKE_ROOM_CONNECTION(srcIndex,destIndex,srcType) \
    { \
        Room *room1,*room2; \
        RECT rect; \
        rect.bottom = rect.top = rect.left = rect.right = 0; \
        room1 = (Room *)(itsRooms->elementAt(srcIndex)); \
        room2 = (Room *)(itsRooms->elementAt(destIndex)); \
        RoomConnector *con = new RoomConnector(room1,room2, \
                                  srcType, \
                                  srcType,rect,rect); \
        itsConnectors->addElement(con); \
        room1->addConnection(con); \
    }

#define CREATE_KEY(castleId,x,y,roomNumber) \
	{\
		assert((castleId >= 0) && (castleId < 256));\
		\
		newImage = new ScreenImage();\
		newImage->init(directDrawObject,its8BitReader,"key.bmp",&colorKey);\
		newImage->changeColor(castleId);\
		Key *key = new Key();\
		thisKey = key;\
		key->init(currentTime,newImage,x,y,castleId);\
		itsImages->addElement(newImage);\
		itsObjects->addElement(key);\
		((Room*)(itsRooms->elementAt(roomNumber)))->addObject(key,(x == -1 ? TRUE : FALSE));\
	}

#define CREATE_PLAYER() \
	assert(itsPlayerStartX != -1);\
	assert(itsPlayerStartY != -1); \
    newImage = new WallImage();\
    ((WallImage*)newImage)->init(directDrawObject,16,16,32,itsStartingRoom->getImage()->getPalette());\
    itsImages->addElement(newImage);\
\
    itsPlayer = new Player();\
    itsPlayer->init(currentTime,(WallImage *)newImage,itsPlayerStartX,itsPlayerStartY);\
    itsObjects->addElement(itsPlayer);\
    itsStartingRoom->addObject(itsPlayer)

#define MAKE_CASTLE_EXIT(x,y,gateObject) \
	{ \
		Room *room1,*room2; \
		room1 = (Room *)(itsRooms->elementAt(x)); \
        room2 = room1; \
		RECT rect; \
		rect.left = 0; \
		rect.top = Game::theScreenHeight + 3; \
		rect.right = 256; \
		rect.bottom = Game::theScreenHeight + 10; \
		RoomConnector *con = new RoomConnector(room1,room2,RoomConnector::CONNECTION_LOCATION_ARBITRARY, \
			RoomConnector::CONNECTION_LOCATION_TOP,rect,rect); \
		itsConnectors->addElement(con); \
		room1->addConnection(con); \
		 \
	} \
	{ \
		Room *room1,*room2; \
		room1 = (Room *)(itsRooms->elementAt(x)); \
        room2 = room1; \
		RECT rect; \
		rect.left = 384; \
		rect.top = Game::theScreenHeight + 3; \
		rect.right = 640; \
		rect.bottom = Game::theScreenHeight + 10; \
		RoomConnector *con = new RoomConnector(room1,room2,RoomConnector::CONNECTION_LOCATION_ARBITRARY, \
			RoomConnector::CONNECTION_LOCATION_TOP,rect,rect); \
		itsConnectors->addElement(con); \
		room1->addConnection(con); \
		 \
	} \
	{ \
		Room *room1,*room2; \
		room1 = (Room *)(itsRooms->elementAt(x)); \
        room2 = (Room *)(itsRooms->elementAt(y)); \
		RECT srcrect; \
		srcrect.left = 256; \
		srcrect.top = Game::theScreenHeight + 3; \
		srcrect.right = 384; \
		srcrect.bottom = srcrect.top + 10; \
 \
		RECT destrect; \
		destrect.left = 314; \
		destrect.top = 248; \
		destrect.right = 326; \
		destrect.bottom = 342; \
		RoomConnector *con = new RoomConnector(room1,room2,RoomConnector::CONNECTION_LOCATION_ARBITRARY, \
			RoomConnector::CONNECTION_LOCATION_ARBITRARY,srcrect,destrect,TRUE,gateObject); \
		itsConnectors->addElement(con); \
		room1->addConnection(con); \
		 \
	} \

#define INIT_WORLD() \
    ScreenImage *newImage = new ScreenImage(); \
	ScreenImage *bgImage = new ScreenImage(); \
	ScreenImage *darkBgImage = new ScreenImage(); \
	Key *thisKey = NULL; \
	GameObject *bridge = NULL;\
	Gate *gate = NULL; \
	Gate *goldGate = NULL; \
	Gate *whiteGate = NULL; \
	Gate *blackGate = NULL; \
	Gate *blueGate = NULL; \
 \
	DDCOLORKEY colorKey; \
    colorKey.dwColorSpaceHighValue = ScreenImage::theTransparencyColor; \
    colorKey.dwColorSpaceLowValue = ScreenImage::theTransparencyColor; \
 \
	bgImage->init(directDrawObject,its8BitReader,"bg.bmp"); \
	darkBgImage->init(directDrawObject,its8BitReader,"bgdark.bmp"); \
    itsImages->addElement(bgImage); \
	itsImages->addElement(darkBgImage)

#define END_WORLD()

#endif // __WORLD_H__