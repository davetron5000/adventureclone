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
#include "Dragon.h"
#include "EventDispatcher.h"

World::World(BOOL difficult, BOOL extraItems)
{
	itsImages = new SimpleArray(10);
	itsConnectors = new SimpleArray(10);
	itsRooms = new SimpleArray(10);
	itsObjects = new SimpleArray(10);
	its8BitReader = new Bmp8BitFileReader();

	itsStartingRoom = NULL;
	itsWinningRoom = NULL;
	itsPlayer = NULL;

	itsPlayerStartX = -1;
	itsPlayerStartY = -1;

	isDifficult = difficult;
	isExtraItems = extraItems;
}

void World::softReset()
{
	for (UINT i=0;i<itsObjects->length();i++)
	{
		GameObject *object = (GameObject *)itsObjects->elementAt(i);
		theEventDispatcher.fireEvent(EventDispatcher::EVENT_TYPE_RESURRECTED,object,NULL);
		if (object->getType() == GameObject::GAMEOBJECT_TYPE_PLAYER)
		{
			// if it's the player, we have to put him back at the start
			object->getRoom()->removeObject(object);
			getStartingRoom()->addObject(object);
			object->setX(itsPlayerStartX);
			object->setY(itsPlayerStartY);
		}
	}
}

void World::release()
{
    UINT i;
    for (i=0;i<itsObjects->length();i++)
    {
        GameObject *obj = (GameObject *)(itsObjects->elementAt(i));
        obj->release();
        delete obj;
        
    }
    for (i=0;i<itsConnectors->length();i++)
    {
        RoomConnector *con = (RoomConnector *)(itsConnectors->elementAt(i));
		con->release();
        delete con;
    }
    for (i=0;i<itsImages->length();i++)
    {
        ScreenImage *image = (ScreenImage *)(itsImages->elementAt(i));
        image->release();
        delete image;
    }

	for (i=0;i<itsRooms->length();i++)
	{
		Room *room = (Room *)(itsRooms->elementAt(i));
		room->release();
		delete room;
	}

    itsStartingRoom = NULL;
    itsPlayer = NULL;
	itsWinningRoom = NULL;
	
    delete itsRooms;
    delete itsObjects;
    delete itsImages;
    delete its8BitReader;
	delete itsConnectors;
}
