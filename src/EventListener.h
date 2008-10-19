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


#ifndef __EVENTLISTENER_H__
#define __EVENTLISTENER_H__

class GameObject;
class Room;

// this is an interface for an event listener.  Rather than have tons
// of listeners for every type of event, we'll just have one, and provide
// no-op implementations for each of the callback methods.
// it is expected that an object that wants to be a listener will multiply 
// inherit from this and implement any methd it is interested in
class EventListener
{
public:

	// called when an object is picked up
	virtual void onObjectGrab(GameObject *object, GameObject *grabber) {}

	// called when an object is put down
	virtual void onObjectDrop(GameObject *object, GameObject *dropper) {}

	// called when an object enters a room
	virtual void onEnterRoom(GameObject *object, Room *room) {}

	// callled when an object leaves a room
	virtual void onLeaveRoom(GameObject *object, Room *room) {}

	// called when an object gets attacked
	virtual void onAttacked(GameObject *victim, GameObject *attacker) {}

	// called when an object is killed
	virtual void onKilled(GameObject *victim, GameObject *killer) {}

	// called when an object is resurrected from death
	virtual void onResurrected(GameObject *object) {}
	
	// called when an object's position is changed
	virtual void onRelocated(GameObject *object) {}

	// called when the game is paused
	virtual void onPause() {}

	// called when the game is unpaused
	virtual void onUnpause() {}

	// called when the timer specified by the ID elapses (see EventDispatcher)
	virtual void onTimer(int timerID) {}

	// called when the game is won
	virtual void onWinGame() {}

	// called when the game is lost
	virtual void onLoseGame() {}

	virtual void onUnknown(int eventType, GameObject *object, void *data) {}

};
#endif // __EVENTLISTENER_H__
