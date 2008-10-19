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
#include "EventDispatcher.h"
#include <Logger.h>

EventDispatcher::EventDispatcher()
{
	itsTimerEvents = NULL;
	itsRegisteredListeners = NULL;
}

BOOL EventDispatcher::init()
{
	itsTimerEvents = new SimpleArray(10);
	itsRegisteredListeners = new SimpleArray[MAX_EVENT_TYPES + 1];

	// registering for pause so we can pause the timers
	registerForEvent(EVENT_TYPE_PAUSE,this,NULL);
	registerForEvent(EVENT_TYPE_UNPAUSE,this,NULL);
	return TRUE;
}

BOOL EventDispatcher::update(DWORD currentTime)
{
	EventRecord *record = NULL;
	UINT currentIndex = 0;
	while (currentIndex < itsTimerEvents->length())
	{
		record = (EventRecord *)(itsTimerEvents->elementAt(currentIndex));
		if ( record->itsFireTime <= currentTime )
		{
			record->itsListener->onTimer(record->itsTimerID);
			itsTimerEvents->removeElementAt(currentIndex);
			currentIndex--;
		}
		currentIndex++;
	}
	return TRUE;
}

void EventDispatcher::registerForEvent(EventType type, EventListener *listener, GameObject *recipient)
{
	EventRecord *newRecord = new EventRecord();
	newRecord->itsInterestedObject = recipient;
	newRecord->itsListener = listener;
	newRecord->itsObjectType = GameObject::GAMEOBJECT_TYPE_GAMEOBJECT;
	newRecord->itsRegisterTime = timeGetTime();
	itsRegisteredListeners[type].addElement(newRecord);
}

void EventDispatcher::registerForEvent(EventType type, EventListener *listener, GameObject::GameObjectType objectType)
{
	EventRecord *newRecord = new EventRecord();
	newRecord->itsInterestedObject = NULL;
	newRecord->itsListener = listener;
	newRecord->itsObjectType = objectType;
	newRecord->itsRegisterTime = timeGetTime();
	itsRegisteredListeners[type].addElement(newRecord);
}

unsigned int EventDispatcher::setTimer(EventListener *listener, unsigned int milliseconds)
{
	EventRecord *newRecord = new EventRecord();
	newRecord->itsListener = listener;
	newRecord->itsRegisterTime = timeGetTime();
	newRecord->itsTimeLength = milliseconds;
	newRecord->itsTimerID = itsNextTimerID;
	newRecord->itsFireTime = newRecord->itsRegisterTime + milliseconds;
	itsTimerEvents->addElement(newRecord);
	itsNextTimerID++;
	return newRecord->itsTimerID;
}

#define LISTENER_SEARCH_BEGIN() \
	for (UINT i=0;i<itsRegisteredListeners[type].length();i++) \
	{

#define LISTENER_SEARCH_END() }

// EventType agnostic way of checking to see if we should fire the event on the object in question.
#define IF_FIRE_EVENTS() \
	BOOL fire = FALSE; \
	recordPointer = (EventRecord *)itsRegisteredListeners[type].elementAt(i); \
	if (recordPointer->itsInterestedObject != NULL) \
	{ \
		if (recordPointer->itsInterestedObject == recipient) \
		{ \
			fire = TRUE; \
		} \
	} \
	else if (recordPointer->itsObjectType != GameObject::GAMEOBJECT_TYPE_GAMEOBJECT) \
	{ \
		if (recordPointer->itsObjectType == recipient->getType()) \
		{ \
			fire = TRUE; \
		} \
	} \
	else \
	{ \
		fire = TRUE; \
	} \
	if (fire == TRUE) \

void EventDispatcher::fireEvent(EventType type, GameObject *recipient, void *data)
{
	EventRecord *recordPointer = NULL;
	switch (type)
	{		
	case EVENT_TYPE_PAUSE :
		{
			for (UINT i=0;i<itsRegisteredListeners[type].length();i++)
			{
				((EventRecord *)itsRegisteredListeners[type].elementAt(i))->itsListener->onPause();
			}
		}
		break;
	case EVENT_TYPE_UNPAUSE :
		{
			for (UINT i=0;i<itsRegisteredListeners[type].length();i++)
			{
				((EventRecord *)itsRegisteredListeners[type].elementAt(i))->itsListener->onUnpause();
			}
		}
		break;
	case EVENT_TYPE_GAMEWON :
		{
			for (UINT i=0;i<itsRegisteredListeners[type].length();i++)
			{
				((EventRecord *)itsRegisteredListeners[type].elementAt(i))->itsListener->onWinGame();
			}
		}
		break;
	case EVENT_TYPE_GAMELOST :
		{
			for (UINT i=0;i<itsRegisteredListeners[type].length();i++)
			{
				((EventRecord *)itsRegisteredListeners[type].elementAt(i))->itsListener->onLoseGame();
			}
		}
		break;
	case EVENT_TYPE_GRAB :
		{
			GameObject *grabber = (GameObject *)data;
			LISTENER_SEARCH_BEGIN()
				IF_FIRE_EVENTS()
				{
					recordPointer->itsListener->onObjectGrab(recipient,grabber);
				}
			LISTENER_SEARCH_END()
		}
		break;
	case EVENT_TYPE_DROP :
		{
			GameObject *dropper = (GameObject *)data;
			LISTENER_SEARCH_BEGIN()
				IF_FIRE_EVENTS()
				{
					recordPointer->itsListener->onObjectDrop(recipient,dropper);
				}
			LISTENER_SEARCH_END()
		}
		break;
	case EVENT_TYPE_ENTER :
		{
			Room *newRoom = (Room *)data;
			LISTENER_SEARCH_BEGIN()
			IF_FIRE_EVENTS()
			{
				recordPointer->itsListener->onEnterRoom(recipient,newRoom);
			}
			LISTENER_SEARCH_END()
		}
		break;
	case EVENT_TYPE_LEAVE :
		{
			Room *oldRoom = (Room *)data;
			LISTENER_SEARCH_BEGIN()
			IF_FIRE_EVENTS()
			{
				recordPointer->itsListener->onLeaveRoom(recipient,oldRoom);
			}
			LISTENER_SEARCH_END()
		}
		break;
	case EVENT_TYPE_ATTACKED :
		{
			GameObject *attacker = (GameObject *)data;
			LISTENER_SEARCH_BEGIN()
			IF_FIRE_EVENTS()
			{
				recordPointer->itsListener->onAttacked(recipient,attacker);
			}
			LISTENER_SEARCH_END()
		}
		break;
	case EVENT_TYPE_KILLED :
		{
			GameObject *killer = (GameObject *)data;
			LISTENER_SEARCH_BEGIN()
			IF_FIRE_EVENTS()
			{
				recordPointer->itsListener->onKilled(recipient,killer);
			}
			LISTENER_SEARCH_END()
		}
		break;
	case EVENT_TYPE_RESURRECTED :
		{
			LISTENER_SEARCH_BEGIN()
			IF_FIRE_EVENTS()
			{
				recordPointer->itsListener->onResurrected(recipient);
			}
			LISTENER_SEARCH_END()
		}
		break;
	case EVENT_TYPE_RELOCATED :
		{
			LISTENER_SEARCH_BEGIN()
			IF_FIRE_EVENTS()
			{
				recordPointer->itsListener->onRelocated(recipient);
			}
			LISTENER_SEARCH_END()
		}
		break;
    default:
		{
			LISTENER_SEARCH_BEGIN()
			IF_FIRE_EVENTS()
			{
				recordPointer->itsListener->onUnknown(type,recipient,data);
			}
			LISTENER_SEARCH_END()
		}
		break;
	}
}

BOOL EventDispatcher::release()
{
	if (itsRegisteredListeners != NULL)
	{
		for (int i=0;i<MAX_EVENT_TYPES;i++)
		{
			SimpleArray *array = itsRegisteredListeners + i;
			for (UINT j=0;j<array->length();j++)
			{
				EventRecord *record = (EventRecord *)(array->elementAt(j));
				delete record;
			}
		}
		delete[] itsRegisteredListeners;
		itsRegisteredListeners = NULL;
	}

	if (itsTimerEvents != NULL)
	{
		delete itsTimerEvents;
		itsTimerEvents = NULL;
	}

	return TRUE;
}

void EventDispatcher::onPause()
{
	DWORD currentTime = timeGetTime();
	for (UINT i=0;i<itsTimerEvents->length();i++)
	{
		// For pausing, we see how much time has passed and set this timer's
		// length to the amout of remaining time.
		EventRecord *record = (EventRecord *)(itsTimerEvents->elementAt(i));
		record->itsTimeLength = (record->itsTimeLength - 
										(currentTime - record->itsRegisterTime) );
		if (record->itsTimeLength < 0)
			record->itsTimeLength = 0;
		// NOT setting firetime or register time.  We don't know that until
		// unpause.
	}
}

void EventDispatcher::onUnpause()
{
	DWORD currentTime = timeGetTime();
	for (UINT i=0;i<itsTimerEvents->length();i++)
	{
		// Now that we are unpausing, we set the timer's register time as now and 
		// it's fire time to now + length (which is the remaining time)
		EventRecord *record = (EventRecord *)(itsTimerEvents->elementAt(i));
		record->itsRegisterTime = currentTime;
		record->itsFireTime = currentTime + record->itsTimeLength;
	}
}