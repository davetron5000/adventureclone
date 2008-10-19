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
#include <ddraw.h>
#include <SimpleArray.h>
#include <ScreenImage.h>
#include "GameObject.h"
#include "Wall.h"
#include "EventListener.h"

class RoomConnector;

#ifndef __Room_h__
#define __Room_h__

// A Room is, well, a room.  It's completely static.  It can draw itself on the screen, and it
// has some operations that help deal with objects that are in it.  It can check to see if 
// objects in the room are leaving it for another room (as it stores it's connections to
// other rooms).
class Room : public EventListener
{
protected:

    // Name of this room
    char *itsName;
    
    // Array of RoomConnectors
    SimpleArray *itsConnectors;
    
    // Array of Wall objects describing where it's walls are
    SimpleArray *itsWalls;
    
    // Array of objects contained within it.
    SimpleArray *itsContainedObjects;

    // Image of the room 
    ScreenImage *itsScreenImage;

	ScreenImage *itsDarkImage;

    // Quick pointer to the player if he's in this room, so we don't have
    // to search through the array.
    GameObject *itsPlayer;

    BOOL isDark;

    // If dark, this is the visibility range in pixels.  The visibility is a
    // square area this many pixels on each side, centered around the player.
    // If the player isn't in this room, we shouldn't be drawing so it won't matter.
    ULONG itsVisibility;

	// The primary color of this room (so we can color the player that color)
	int itsPrimaryColor;

	// Space for a regionData structure so we can clip
	LPRGNDATA itsRegionData;

	LPDIRECTDRAWCLIPPER itsClipper;

	// If true, the chalice may show up in the room during random games.
	BOOL isChaliceAllowedToStart;
	// If true, any object can show up in the room during random games.
	BOOL isObjectAllowedToStart;

	// x position of an object that starts here
	LONG itsStartX;
	// y posdition of an object that starts here
	LONG itsStartY;

	// pointer to object that was randomly chosen to start here
	GameObject *itsStartingObject;

public:

    Room();
    ~Room();
	
	inline void setIsDark(BOOL dark) { isDark = dark; }
	inline void setVisibility(BOOL vis) { itsVisibility = vis; }

	inline int getPrimaryColor() { return itsPrimaryColor; }

    BOOL init(ScreenImage *image, char *name, ScreenImage *darkImage, ULONG visibility, int primaryColor,
		BOOL chaliceStart = TRUE, BOOL objectStart = TRUE, 
		int startX = -1, int startY = -1);

    BOOL draw(DWORD currentTime, LPDIRECTDRAWSURFACE surface);

	void onEnterRoom(GameObject *object, Room *room);
	void onLeaveRoom(GameObject *object, Room *room);

    inline void addWall(GameObject *wall) 
	{ 
		assert(wall->getType() == GameObject::GAMEOBJECT_TYPE_WALL);
		itsWalls->addElement(wall); 
		wall->setRoom(this); 
	}

	char *getName() { return itsName; }

	// returns the first occurence of the specified type in this room, or NULL
	// if no object of the specified type is in this room
	inline GameObject *searchRoom(GameObject::GameObjectType type)
	{
		for (UINT i=0;i<itsContainedObjects->length();i++)
		{
			GameObject *object = (GameObject *)(itsContainedObjects->elementAt(i));
			if (object->getType() == type)
				return object;
		}
		return NULL;
	}
    // This routine checks for objects that might be leaving this room
    void checkForObjectsLeaving();

    inline void addConnection(RoomConnector *connector) { itsConnectors->addElement(connector); }
    
    inline SimpleArray *getObjects() { return itsContainedObjects; }
    inline SimpleArray *getWalls() { return itsWalls; }

    // true if this is a room that is not fully lit
    inline BOOL getIsDark() { return isDark; }

	// if starting is TRUE, this object is the starting object and will be placed
	// at the starting object coordinates.  
    void addObject(GameObject *object, BOOL starting=FALSE);

	GameObject *getStartingObject() { return itsStartingObject; }
	BOOL canChaliceStart() { return isChaliceAllowedToStart; }
	BOOL canObjectStart() { return isObjectAllowedToStart; }

    void removeObject(GameObject *object);

    inline ScreenImage *getImage() { return itsScreenImage; }

    BOOL restore(DWORD currentTime);
    BOOL release();    

	// return NULL if the player isn't in this room
	GameObject *getPlayer() { return itsPlayer; }

};
#endif // __Room_h__
