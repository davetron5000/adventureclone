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
#include "SoundPlayer.h"
#include "EventDispatcher.h"
#include "Player.h"

SoundPlayer::SoundPlayer(SoundManager *soundManager)
{
	itsSoundManager = soundManager;

	itsPickupIndex = soundManager->addSound("pickup.wav",1,TRUE);
	itsDropIndex = soundManager->addSound("drop.wav",1,TRUE);
	itsAttackIndex = soundManager->addSound("attack.wav",1,TRUE);
	itsDeathIndex = soundManager->addSound("death.wav",1,TRUE);
	itsGameWonIndex = soundManager->addSound("ending.wav",1,TRUE);
	itsSwallowedIndex = soundManager->addSound("swallow.wav",1,TRUE);
	itsSuperSecretIndex = soundManager->addSound("alldeath.wav",1,TRUE);

	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_GRAB,this,NULL);
	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_DROP,this,NULL);
	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_KILLED,this,NULL);
	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_ATTACKED,this,NULL);
	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_GAMEWON,this,NULL);
	theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_GAMELOST,this,NULL);

	theEventDispatcher.registerForEvent(EventDispatcher::MAX_EVENT_TYPES,this,NULL);
}

void SoundPlayer::onObjectGrab(GameObject *object, GameObject *grabber)
{
	if (grabber->getType() == GameObject::GAMEOBJECT_TYPE_PLAYER)
	{
		itsSoundManager->playSound(itsPickupIndex,FALSE,TRUE);
	}
}

void SoundPlayer::onObjectDrop(GameObject *object, GameObject *dropper)
{
	if (dropper->getType() == GameObject::GAMEOBJECT_TYPE_PLAYER)
	{
		if (((Player *)dropper)->getIsDead() == FALSE)
		{
			itsSoundManager->playSound(itsDropIndex,FALSE,TRUE);
		}
	}
}

void SoundPlayer::onKilled(GameObject *victim, GameObject *killer)
{
	switch (victim->getType())
	{
	case GameObject::GAMEOBJECT_TYPE_PLAYER :
		itsSoundManager->playSound(itsSwallowedIndex,FALSE,TRUE);
		break;
	case GameObject::GAMEOBJECT_TYPE_DRAGON :
		itsSoundManager->playSound(itsDeathIndex,FALSE,TRUE);
		break;
	}
}

void SoundPlayer::onAttacked(GameObject *victim, GameObject *attacker)
{
	if (attacker->getType() == GameObject::GAMEOBJECT_TYPE_DRAGON)
	{
		itsSoundManager->playSound(itsAttackIndex,FALSE,TRUE);
	}
}

void SoundPlayer::onWinGame()
{
	itsSoundManager->stopAllSounds();
	itsSoundManager->playSound(itsGameWonIndex,FALSE,FALSE);
}

void SoundPlayer::onPause()
{
}

void SoundPlayer::onUnknown(int eventType, GameObject *object, void *data)
{
	if (eventType = EventDispatcher::MAX_EVENT_TYPES)
	{
		itsSoundManager->playSound(itsSuperSecretIndex,TRUE,TRUE);
	}
}