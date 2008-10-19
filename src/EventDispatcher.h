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
#include "EventListener.h"
#include <SimpleArray.h>
#include "GameObject.h"

#ifndef __EVENTDISPATCHER_H__
#define __EVENTDISPATCHER_H__

class EventDispatcher : public EventListener
{
protected:

	unsigned int itsNextTimerID;


	// quasi union to hold all info about a registered event
	// the type of event is known by the location of this object in the array
	// if itsInterestedObject is NULL, itsObjectType determines the type of object
	// that the listener is interested in.  If _that_ is GAMEOBJECT_TYPE_GAMEOBJECT, then
	// the listener is interested in everything.
	struct EventRecord
	{
		EventListener *itsListener;
		GameObject *itsInterestedObject;
		GameObject::GameObjectType itsObjectType;
		DWORD itsRegisterTime;
		DWORD itsFireTime;
		unsigned long itsTimeLength;
		unsigned int itsTimerID;
	};


	// static array of simple arrays, allocated at construction to the size of the
	// event types.  No timer events are stored here
	SimpleArray *itsRegisteredListeners;

	// Only the timer events.
	SimpleArray *itsTimerEvents;

public:

	EventDispatcher();

	BOOL init();

	enum EventType
	{
		EVENT_TYPE_GRAB=0,
		EVENT_TYPE_DROP,
		EVENT_TYPE_ENTER,
		EVENT_TYPE_LEAVE,
		EVENT_TYPE_ATTACKED,
		EVENT_TYPE_KILLED,
		EVENT_TYPE_RESURRECTED,
		EVENT_TYPE_RELOCATED,
		EVENT_TYPE_PAUSE,
		EVENT_TYPE_UNPAUSE,
		EVENT_TYPE_GAMEWON,
		EVENT_TYPE_GAMELOST,
		// no timer event, as that is special case
		MAX_EVENT_TYPES
	};

	// called by the system to update the time
	BOOL update(DWORD currentTime);

	// Allows an object to register for an event.
	// type is the type of event the listener wants to hear about
	// listener is usually the caller, but is the object that wants to hear about the event
	// recipient is the object that the listener wishes to hear events about.  If NULL, the listener
	// will hear all events of this type.
	void registerForEvent(EventType type, EventListener *listener, GameObject *recipient=NULL);
	
	// similar to the other version, but this allows the listener to hear about events happening to 
	// all objects of a certain type, instead of just one particular object.
	void registerForEvent(EventType type, EventListener *listener, GameObject::GameObjectType objectType);

	// set a timer event for milliseconds in the future.  This returns a unique
	// timerID that will be passed into the onTimer() callback method
	unsigned int setTimer(EventListener *listener, unsigned int milliseconds);

	// fire an event of type type
	// recipient is the gameobject recieving the event (it will be passed in as the first GameObject to
	// the EventListener method).  data is a pointer to the optional data, based on the EventListener
	// interface.  For example, if this event is EVENT_TYPE_LEAVE, then data will point to 
	// the Room object that recipient is leaving.
	// For EVENT_TYPE_PAUSE and EVENT_TYPE_UNPAUSE, there is no recipient nor data, so to fire these events,
	// you do not need to specifiy them
	void fireEvent(EventType type, GameObject *recipient, void *data);

	BOOL release();

	virtual void onPause();
	virtual void onUnpause();

};

extern EventDispatcher theEventDispatcher;

#endif // __EVENTDISPATCHER_H__