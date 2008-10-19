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
#include <SoundManager.h>
#include "EventListener.h"

#ifndef __SOUNDPLAYER_H__
#define __SOUNDPLAYER_H__

// This class handles playing sounds for various events so that the
// gameobjects don't have to worry about it.
class SoundPlayer : public EventListener
{
protected:
	SoundManager *itsSoundManager;
	int itsPickupIndex;
	int itsDropIndex;
	int itsAttackIndex;
	int itsDeathIndex;
	int itsGameWonIndex;
	int itsSwallowedIndex;

	int itsSuperSecretIndex;

public:
	SoundPlayer(SoundManager *soundManager);

	virtual void onObjectGrab(GameObject *object, GameObject *grabber);

	virtual void onObjectDrop(GameObject *object, GameObject *dropper);

	virtual void onKilled(GameObject *victim, GameObject *killer);

	virtual void onAttacked(GameObject *victim, GameObject *attacker);

	virtual void onWinGame();

	virtual void onPause();

	virtual void onUnknown(int eventType, GameObject *object, void *data);
};

#endif // __SOUNDPLAYER_H__