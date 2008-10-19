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
#include "Game.h"
#include <Logger.h>
#include <assert.h>
#include <Bmp8BitFileWriter.h>

#include "Game1World.h"
#include "Game2World.h"
#include "Game3World.h"
#include "ObjectTestWorld.h"
#include "GameSelectWorld.h"
#include "EventDispatcher.h"

#include "ConnectionTestWorld.h"

#include "WallImage.h"
#include <Bmp8BitFileReader.h>

#include "Bat.h"

const int Game::theScreenHeight = 382;
const int Game::theScreenWidth = 640;
const int Game::theBitDepth = 8;

const int theDisplayHeight = 480;
const int theDisplayWidth = 640;

LPDIRECTDRAW        Game::theDirectDrawObject;
LPDIRECTDRAWSURFACE Game::thePrimarySurface;
LPDIRECTDRAWSURFACE Game::theBackbuffer;
LPDIRECTDRAWPALETTE Game::thePalette;
LPDIRECTDRAWPALETTE Game::theDimmedPalette;
LPDIRECTDRAWPALETTE Game::theBackbufferPalette;
LPDIRECTDRAWCLIPPER Game::theClipper;

const DWORD Game::theWinAnimationLength = 4000;

Game::Game()
{
    theDirectDrawObject = NULL;
    thePrimarySurface = NULL;
    theBackbuffer = NULL;
    thePalette = NULL;
    theClipper = NULL;

	itsSoundManager = NULL;
	itsUnPauseTime = timeGetTime();
	itsPauseCooldown = 500;

	itsGameSelectImages = NULL;

	itsWorld = NULL;
	itsNewWorld = NULL;

	itsSoundPlayer = NULL;

	itsNewStateRequest.isActiveRequest = FALSE;

}
#define ON_BRIDGE(bridge,player,playerRect) ( (player->getCarriedObject() != bridge) && \
                                              (bridge->isColliding(playerRect)) && \
                                              (!bridge->isColliding(player)) )



BOOL Game::processInput(DWORD currentTime)
{
    static ULONG speed = Player::theMovementSpeed;

	itsInputManager.tick(currentTime);

	BOOL ignoreMovement = FALSE;
	BOOL ignorePause = FALSE;
	BOOL ignoreSoftReset = FALSE;
	BOOL ignoreGameSelection = FALSE;

	switch (itsCurrentState)
	{
		case GAMESTATE_INPROGRESS :
			ignoreGameSelection = TRUE;
			break;
		case GAMESTATE_PREGAME :
			ignoreMovement = TRUE;
			ignoreSoftReset = TRUE;
			ignorePause = TRUE;
			break;
		case GAMESTATE_WON :
			ignoreMovement = TRUE;
			ignoreSoftReset = TRUE;
			ignorePause = TRUE;
			ignoreGameSelection = TRUE;
			break;
		case GAMESTATE_LOST :
			ignoreMovement = TRUE;
			ignoreGameSelection = TRUE;
			break;
		case GAMESTATE_PAUSED :
			ignoreSoftReset = TRUE;
			ignoreMovement = TRUE;
			ignoreGameSelection = TRUE;
			break;
	}
    LONG playerXSpeed = 0;
    LONG playerYSpeed = 0;
	BOOL movePlayer = FALSE;
    if (itsInputManager.isUp)
    {
        playerYSpeed -= speed;
		movePlayer = TRUE;
    }
    if (itsInputManager.isDown)
    {
		movePlayer = TRUE;
        playerYSpeed += speed;
    }
    if (itsInputManager.isLeft)
    {
		if (!itsPlayer->getOnBridge())
		{
			movePlayer = TRUE;
			playerXSpeed -= speed;
		}
    }
    if (itsInputManager.isRight)
    {
		if (!itsPlayer->getOnBridge())
		{
			movePlayer = TRUE;
			playerXSpeed += speed;
		}
    }
    if (itsInputManager.isButton)
    {
		if (!ignoreMovement)
			itsPlayer->setCarriedObject(NULL);
	}
	if (itsInputManager.isSoftReset)
	{
		if (!ignoreSoftReset)
		{
			itsNewStateRequest.isActiveRequest = TRUE;
			itsNewStateRequest.itsNewState = GAMESTATE_INPROGRESS;
			Logger::log("Soft reset");
		}
	}
	if (itsInputManager.isHardReset)
	{
		itsNewStateRequest.isActiveRequest = TRUE;
		itsNewStateRequest.itsNewState = GAMESTATE_PREGAME;
	}
	if (itsInputManager.isGameSelect)
	{
		if (!ignoreGameSelection)
		{
			itsCurrentlySelectedGame = (itsCurrentlySelectedGame + 1) % itsGameSelectImages->length();
			isDisplayingInstructions = FALSE;
			itsInstructionMessageTimerID = theEventDispatcher.setTimer(this,3500);
		}
	}
	if (itsInputManager.isGameStart)
	{
		if (!ignoreGameSelection)
		{
			itsNewStateRequest.isActiveRequest = TRUE;
			itsNewStateRequest.itsNewState = GAMESTATE_INPROGRESS;
			isInGameSelectWorld = FALSE;
			switch (itsCurrentlySelectedGame)
			{
			case 0 :
				itsNewStateRequest.itsNewWorld = new Game1World(FALSE,FALSE);
				break;
			case 1 :
				itsNewStateRequest.itsNewWorld = new Game2World(FALSE,FALSE);
				break;
			case 2 :
				itsNewStateRequest.itsNewWorld = new Game3World(FALSE,FALSE);
				break;
			default :
				itsNewStateRequest.itsNewWorld = new GameSelectWorld();
				isInGameSelectWorld = TRUE;
				break;
			}
		}
	}
	if (itsInputManager.isPause)
	{
		if (!ignorePause)
		{
			itsNewStateRequest.isActiveRequest = TRUE;
			if (itsCurrentState == GAMESTATE_PAUSED)
				itsNewStateRequest.itsNewState = GAMESTATE_INPROGRESS;
			else
				itsNewStateRequest.itsNewState = GAMESTATE_PAUSED;
		}
	}
	if (itsInputManager.isScreenshot)
	{
		itsScreenshotManager.writeScreenShot(theBackbuffer);
	}

	if (ignoreMovement)
	{
		if (itsPlayer != NULL)
			itsPlayer->stopMoving(currentTime);
	}
	else
	{
		if (itsPlayer != NULL)
			itsPlayer->startMoving(currentTime,playerXSpeed,playerYSpeed);
	}
    return TRUE;
}

BOOL Game::unPause(DWORD currentTime)
{
	if (currentTime - itsPauseTime > itsPauseCooldown)
	{
		itsUnPauseTime = currentTime;
		thePrimarySurface->SetPalette(thePalette);
		theBackbuffer->SetPalette(theBackbufferPalette);
		itsCurrentState = GAMESTATE_INPROGRESS;
		theEventDispatcher.fireEvent(EventDispatcher::EVENT_TYPE_UNPAUSE,NULL,NULL);
	}
	return TRUE;
}
BOOL Game::pause(DWORD currentTime)
{
	if (currentTime - itsUnPauseTime > itsPauseCooldown)
	{
		itsPauseTime = currentTime;
		thePrimarySurface->SetPalette(theDimmedPalette);
		theBackbuffer->SetPalette(theDimmedPalette);
		itsCurrentState = GAMESTATE_PAUSED;
		theEventDispatcher.fireEvent(EventDispatcher::EVENT_TYPE_PAUSE,NULL,NULL);
	}
	return TRUE;
}

BOOL Game::checkForCollisions()
{
	// NOTE: Things happen here in a specific order.  Collisions between non-wall objects
	// MUST MUST MUST happen before wall collisions.  The reason is that the player could
	// collide with a dragon and thus bounce off.  If this were to occur after wall collisions were
	// calculated, the player would more easily bounce through a wall.

    UINT i;
	SimpleArray *walls = itsPlayer->getRoom()->getWalls();
	GameObject *bridge = itsPlayer->getRoom()->searchRoom(GameObject::GAMEOBJECT_TYPE_BRIDGE);

	// TODO: clean this up
    SimpleArray *objects = itsWorld->getObjects();
    for (i=0;i<objects->length();i++)
    {
		GameObject *object = (GameObject *)(objects->elementAt(i));
		
		if (object->getType() == GameObject::GAMEOBJECT_TYPE_WALL)
			continue;

		for (UINT j=1;j<objects->length();j++)
		{
			GameObject *object2 = (GameObject *)(objects->elementAt(j));
			if (object2->getType() == GameObject::GAMEOBJECT_TYPE_WALL)
				continue;

			if (object->isColliding(object2))
			{
				object->collision(itsLastTickTime,object2);
				object2->collision(itsLastTickTime,object);
			}
		}
    }

	RECT playerRect;
	itsPlayer->getRect(&playerRect);

	BOOL onBridge = FALSE;
	for (i=0;i<walls->length();i++)
	{
	    Wall *thisWall = (Wall *)(walls->elementAt(i));
		if (thisWall->isColliding(itsPlayer))
		{
			// If the player's rect is colliding with the bridge, but not their actual pixels
			// (in other words, inside the bridge's rect, but not touching the bridge itself)
			if ( (bridge != NULL) && (ON_BRIDGE(bridge,itsPlayer,playerRect)) )
			{
				onBridge = TRUE;
			}
			else
			{
				itsPlayer->collision(itsLastTickTime,thisWall);
			}
		}
	}
	itsPlayer->setOnBridge(onBridge);

    return TRUE;
}

void Game::pregameUpdate()
{
	((ScreenImage*)itsGameSelectImages->elementAt(itsCurrentlySelectedGame))->draw(theBackbuffer,0,0);
	if (isDisplayingInstructions)
	{
		itsInstructionMessage.draw(theBackbuffer,145,177);
	}
}

BOOL Game::update()
{
	BOOL updateObjects = FALSE;
	BOOL checkForWinning = FALSE;
	BOOL checkForLosing = FALSE;
	BOOL checkCollisions = FALSE;
	BOOL stopMovement = TRUE;

	switch (itsCurrentState)
	{
		case GAMESTATE_PREGAME :
			// since the pregame section is so different, we will call our own method
			pregameUpdate();
			break;
		case GAMESTATE_INPROGRESS :
			updateObjects = TRUE;
			checkForWinning = TRUE;
			checkForLosing = TRUE;
			checkCollisions = TRUE;
			stopMovement = FALSE;
			break;
		case GAMESTATE_WON :
		{
			updateObjects = TRUE;
			Room *room = itsPlayer->getRoom();
			// we need to change the walls a color for that ending flashiness
			if (itsLastTickTime - itsWinTime >= theWinAnimationLength)
			{	
				for (UINT i=0;i<room->getWalls()->length();i++)
				{
					Wall *wall = (Wall *)(room->getWalls()->elementAt(i));
					wall->setChangesColor(FALSE);
				}
			}
			else
			{
				// since the player will be holding the chalice, we can tell
				// the walls to change colors, and they will change to the color
				// of the chalice, which will make them appear to animate
				for (UINT i=0;i<room->getWalls()->length();i++)
				{
					Wall *wall = (Wall *)(room->getWalls()->elementAt(i));
					wall->setChangesColor(TRUE,1);
				}
			}
			// hack to put the player in the right place for the ending
			itsPlayer->setX(Game::theScreenHeight-64);
			itsPlayer->setY(Game::theScreenWidth/2-16);
		}
		break;
		case GAMESTATE_LOST :
			updateObjects = TRUE;
			stopMovement = FALSE;
			break;
		case GAMESTATE_PAUSED :
			stopMovement = FALSE;
			break;
	}

	theEventDispatcher.update(itsLastTickTime);


	if (updateObjects)
	{
		SimpleArray *objects = NULL;
		if (itsWorld != NULL)
		objects = itsWorld->getObjects();
		for (UINT i=0;i<objects->length();i++)
		{
			((GameObject *)(objects->elementAt(i)))->update(itsLastTickTime);
			if (stopMovement)
			{
				((GameObject *)(objects->elementAt(i)))->stopMoving(itsLastTickTime);
			}
		}
		for (i=0;i<itsWorld->getRooms()->length();i++)
		{
			((Room *)(itsWorld->getRooms()->elementAt(i)))->checkForObjectsLeaving();
		}
	}
	if (checkForWinning && gameWon())
	{
		itsNewStateRequest.itsNewState = GAMESTATE_WON;
		itsNewStateRequest.isActiveRequest = TRUE;
	}
	if (checkForLosing && gameLost())
	{
		itsNewStateRequest.itsNewState = GAMESTATE_LOST;
		itsNewStateRequest.isActiveRequest = TRUE;
	}
	if (checkCollisions)
		return checkForCollisions();
	else
		return TRUE;
}

BOOL Game::gameWon()
{
	if ( (itsPlayer->getRoom() == itsWorld->getWinningRoom()) &&
		 (itsPlayer->getCarriedObject() != NULL) )
	{
		GameObject *heldObject = itsPlayer->getCarriedObject();
		if (heldObject->getType() == GameObject::GAMEOBJECT_TYPE_BAT)
		{
			heldObject = ((Bat *)heldObject)->getCarriedObject();
		}
		if ( (heldObject != NULL) && (heldObject->getType() == GameObject::GAMEOBJECT_TYPE_CHALICE) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL Game::gameLost()
{
	if (itsPlayer->getIsDead())
		return TRUE;
	else
		return FALSE;
}

BOOL Game::draw()
{
    assert(theBackbuffer != NULL);
	BOOL result = TRUE;
	if (itsPlayer != NULL)
		result = itsPlayer->getRoom()->draw(itsLastTickTime,theBackbuffer);
	if (result)
	{
		return flip();
	}
	else
	{
		Logger::error("Error from Room::draw!");
	}
	return FALSE;
}

BOOL Game::start(DWORD currentTime)
{
	Bmp8BitFileReader reader;
	itsGameSelectImages = new SimpleArray(4);
	ScreenImage *image = new ScreenImage();
	image->init(theDirectDrawObject,&reader,"game1select.bmp",NULL);
	itsGameSelectImages->addElement(image);

	image = new ScreenImage();
	image->init(theDirectDrawObject,&reader,"game2select.bmp",NULL);
	itsGameSelectImages->addElement(image);

	image = new ScreenImage();
	image->init(theDirectDrawObject,&reader,"game3select.bmp",NULL);
	itsGameSelectImages->addElement(image);

	image = new ScreenImage();
	image->init(theDirectDrawObject,&reader,"moreselect.bmp",NULL);
	itsGameSelectImages->addElement(image);

	itsCurrentlySelectedGame = 0;
	theEventDispatcher.init();
	itsSoundPlayer = new SoundPlayer(itsSoundManager);
	itsCurrentState = GAMESTATE_PREGAME;
	itsInstructionMessageTimerID = theEventDispatcher.setTimer(this,3500);
	isDisplayingInstructions = FALSE;

	DDCOLORKEY colorKey;
    colorKey.dwColorSpaceHighValue = ScreenImage::theTransparencyColor;
    colorKey.dwColorSpaceLowValue = ScreenImage::theTransparencyColor;

	itsInstructionMessage.init(theDirectDrawObject,&reader,"instructions.bmp",&colorKey);

	itsWorld = NULL;

	return TRUE;
}

void Game::onTimer(int timerID)
{
	if (timerID == itsInstructionMessageTimerID)
	{
		isDisplayingInstructions = TRUE;
	}
}

BOOL Game::stop(DWORD currentTime)
{
	Logger::log("stopping game");
	if (itsWorld != NULL)
	{
		Logger::log("World needs to be released");
		itsWorld->release();
		Logger::log("World released");
		delete itsWorld;
		Logger::log("World deleted");
		itsWorld = NULL;
	}
	return TRUE;
}

BOOL Game::init(DWORD currentTime, char *worldname, HWND hwnd,HINSTANCE hinstance)
{
	itsCurrentWorldName = worldname;
    itsLastTickTime = currentTime;
	theEventDispatcher.init();
	CreateDirectory("screens",NULL);
	itsScreenshotManager.init("screens\\advtr");
	if (startDirectDraw(hwnd, hinstance))
    {
	    if ( startDirectSound(hwnd, hinstance) )
        {
			if ( initSurfaces(hwnd, hinstance) )
			{
                if ( startDirectInput(hwnd, hinstance) )
				{
					itsWorld = new Game1World(FALSE,FALSE);
					return start(currentTime);
				}
            }
        }
    }
    return FALSE;
}

BOOL Game::startDirectDraw(HWND hwnd,HINSTANCE hinstance)
{
    if ( SUCCEEDED(DirectDrawCreate(NULL,&theDirectDrawObject,NULL) ))
    {
        if ( SUCCEEDED(theDirectDrawObject->SetCooperativeLevel(hwnd,DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN)) )
        {
            if ( SUCCEEDED(theDirectDrawObject->SetDisplayMode(theDisplayWidth,theDisplayHeight,theBitDepth)) )
            {
                return TRUE;
            }
            else
            {
                Logger::error("Couldn't set display mode (%d,%d,%d)",theDisplayWidth,theDisplayHeight,theBitDepth);
            }
        }
        else
        {
            Logger::error("Couldn't set cooperative level");
        }
    }
    else
    {
        Logger::error("Couldn't create DirectDraw object.");
    }
    return FALSE;
}

BOOL Game::initSurfaces(HWND hwnd,HINSTANCE hinstance)
{
    DDSURFACEDESC surfaceDesc;

	// This is cheesy, but I'm not sure how to deal with it for right now
	ScreenImage bgImage;
	Bmp8BitFileReader reader;
	bgImage.init(theDirectDrawObject,&reader,"bg.bmp");

    memset(&surfaceDesc,0,sizeof(surfaceDesc));
    surfaceDesc.dwSize  = sizeof(surfaceDesc);
    surfaceDesc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    surfaceDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP|DDSCAPS_COMPLEX;
    surfaceDesc.dwBackBufferCount = 1;

    if ( SUCCEEDED(theDirectDrawObject->CreateSurface(&surfaceDesc,&thePrimarySurface,NULL)) )
    {
        assert(theBitDepth == 8);

        PALETTEENTRY palette[256];
        bgImage.getPalette()->GetEntries(0,0,256,palette);
		PALETTEENTRY dimmedPalette[256];
		for (UINT i=0;i<256;i++)
		{
			dimmedPalette[i].peBlue = (palette[i].peBlue / 3) * 2;
			dimmedPalette[i].peRed = (palette[i].peRed / 3) * 2;
			dimmedPalette[i].peGreen = (palette[i].peGreen / 3) * 2;
		}
        
        if ( (SUCCEEDED(theDirectDrawObject->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_INITIALIZE,
                                                         palette,&thePalette,NULL)) ) &&
			 (SUCCEEDED(theDirectDrawObject->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_INITIALIZE,
                                                         dimmedPalette,&theDimmedPalette,NULL)) ) )

        {
            thePrimarySurface->SetPalette(thePalette);
            DDSCAPS surfaceCaps;

            memset(&surfaceCaps,0,sizeof(surfaceCaps));
            surfaceCaps.dwCaps = DDSCAPS_BACKBUFFER;

            if ( SUCCEEDED(thePrimarySurface->GetAttachedSurface(&surfaceCaps,&theBackbuffer)) )
            {
                if (SUCCEEDED(theDirectDrawObject->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_INITIALIZE,
                                                                 palette,&theBackbufferPalette,NULL) ))
                {
                    theBackbuffer->SetPalette(theBackbufferPalette);
                    if (SUCCEEDED(theDirectDrawObject->CreateClipper(NULL,&theClipper,NULL)))
                    {
                        if (SUCCEEDED(theClipper->SetHWnd(NULL,hwnd)))
                        {
                            if (SUCCEEDED(theBackbuffer->SetClipper(theClipper)))
                            {
                                bgImage.copyDataToSurface(thePrimarySurface);
                                bgImage.copyDataToSurface(theBackbuffer);
                                return TRUE;                
                            }
                            else
                            {
                                Logger::error("Couldn't set clipper on the backbuffer");
                            }
                        }
                        else
                        {
                            Logger::error("Couldn't set HWND on the clipper");
                        }
                    }
                    else
                    {
                        Logger::error("Couldn't create the clipper");
                    }
                }
                else
                {
                    Logger::error("Couldn't create a palette!");
                }
            }
            else
            {
                Logger::error("Couldn't get attached surface from primary surface");
            }
        }
        else
        {
            Logger::error("Couldn't create the palette");
        }
    }
    else
    {
        Logger::error("Couldn't create primary surface");
    }
    return FALSE;
}


BOOL Game::startDirectSound(HWND hwnd,HINSTANCE hinstance)
{
    itsSoundManager = new SoundManager(hwnd);
    return TRUE;
}

BOOL Game::startDirectInput(HWND hwnd,HINSTANCE hinstance)
{
	return itsInputManager.init(timeGetTime(),hwnd,hinstance,Player::theMovementSpeed);
}

BOOL Game::release(DWORD currentTime)
{
	theEventDispatcher.release();
	if (itsGameSelectImages != NULL)
	{
		for (UINT i=0;i<itsGameSelectImages->length();i++)
		{
			ScreenImage *ptr = (ScreenImage *)(itsGameSelectImages->elementAt(i));
			delete ptr;
		}
		delete itsGameSelectImages;
	}

	itsInputManager.release(currentTime);

	if (itsSoundManager != NULL)
	{
		itsSoundManager->release();
		delete itsSoundManager;
	}
	if (itsSoundPlayer != NULL)
		delete itsSoundPlayer;

    // Direct Sound

    // Direct Draw
    if (theClipper != NULL)
        theClipper->Release();
    if (theBackbufferPalette != NULL)
        theBackbufferPalette->Release();
    if (theBackbuffer != NULL)
        theBackbuffer->Release();
    if (thePalette != NULL)
        thePalette->Release();
    if (thePrimarySurface != NULL)
        thePrimarySurface->Release();
    if (theDirectDrawObject != NULL)
        theDirectDrawObject->Release();

    return TRUE;
}
void Game::onEnterRoom(GameObject *object, Room *room)
{
	if (object->getType() == GameObject::GAMEOBJECT_TYPE_PLAYER)
	{
		if (isInGameSelectWorld)
		{
			World *newWorld = ((GameSelectWorld *)itsWorld)->getWorld();
			if (newWorld != NULL)
			{
				isInGameSelectWorld = FALSE;
				itsNewStateRequest.isActiveRequest = TRUE;
				itsNewStateRequest.itsNewState = GAMESTATE_INPROGRESS;
				itsNewStateRequest.itsNewWorld = newWorld;
			}
		}
	}
}

void Game::handleStateChanges(DWORD currentTime)
{
	if (itsNewStateRequest.isActiveRequest == TRUE)
	{
		switch (itsNewStateRequest.itsNewState)
		{
		case GAMESTATE_INPROGRESS :
			if (itsNewStateRequest.itsNewWorld != NULL)
			{
				// changing worlds.
				theEventDispatcher.release();
				theEventDispatcher.init();

				if (isInGameSelectWorld)
					theEventDispatcher.registerForEvent(EventDispatcher::EVENT_TYPE_ENTER,this,NULL);

				if (itsWorld != NULL)
				{
					itsWorld->release();
					delete itsWorld;
				}
				delete itsSoundPlayer;
				itsSoundPlayer = new SoundPlayer(itsSoundManager);
				itsWorld = itsNewStateRequest.itsNewWorld;
				itsWorld->init(itsLastTickTime,theDirectDrawObject);
				itsPlayer = itsWorld->getPlayer();

			}
			else
			{
				switch (itsCurrentState)
				{
				case GAMESTATE_PAUSED :
					// bring back up the lights and tell everyone to unpause
					unPause(currentTime);
					break;
				case GAMESTATE_LOST :
				case GAMESTATE_INPROGRESS :
					itsWorld->softReset();
					break;
				}
			}
			break;
		case GAMESTATE_LOST :
			theEventDispatcher.fireEvent(EventDispatcher::EVENT_TYPE_GAMELOST,NULL,NULL);
			// stop everyone from moving (but not animating) and start ignoring movement input
			break;
		case GAMESTATE_WON :
			// play the winning theme and then stop everyone from moving (but not animating)
			theEventDispatcher.fireEvent(EventDispatcher::EVENT_TYPE_GAMEWON,NULL,NULL);
			itsWinTime = currentTime;
			break;
		case GAMESTATE_PAUSED :
			// dim the lights and send a pause message to everyone
			pause(currentTime);
			break;
		case GAMESTATE_PREGAME :
			// destroy the current world and go back to game select screen
			if (itsWorld != NULL)
			{
				itsWorld->release();
				delete itsWorld;
				itsWorld = NULL;
				itsPlayer = NULL;
			}
			itsInstructionMessageTimerID = theEventDispatcher.setTimer(this,3500);
			break;
		default :
			assert("Illegal State Transition" == NULL);
			break;
		}
		itsCurrentState = itsNewStateRequest.itsNewState;
		itsNewStateRequest.isActiveRequest = FALSE;
		itsNewStateRequest.itsNewWorld = NULL;
	}
}

BOOL Game::tick(DWORD currentTime)
{
    BOOL returnValue = FALSE;
    itsLastTickTime = currentTime;
	
	handleStateChanges(currentTime);

    if ( processInput(currentTime) )
    {
		// If we are here, we are assuming that any data structure that was
		// present during processInput() is in the same state (i.e. valid, NULL, etc)
		// as it was before that call.  In other words, we haven't changed the world, object
		// list, or anything else since then.
        if ( update() )
        {
            returnValue = draw();
        }
        else
        {
            Logger::error("%s: Error from update!",__FILE__);
        }
    }
    else
    {
        Logger::error("%s: Error from processInput!",__FILE__);
    }
    return returnValue;
}

BOOL Game::flip()
{
	BOOL returnValue;
    HRESULT flipResult = thePrimarySurface->Flip(NULL,DDFLIP_WAIT);
    if (flipResult == DDERR_SURFACELOST)
    {
        Logger::log("Attempting to restore surfaces");
        returnValue = restoreSurfaces(itsLastTickTime);
    }
    else if (SUCCEEDED(flipResult))
    {
        returnValue = TRUE;
    }
    else
    {
        Logger::error("Flip failed, but it wasn't due to restore issues");
        returnValue = FALSE;
    }
	return returnValue;
}

BOOL Game::restoreSurfaces(DWORD currentTime)
{
    if (SUCCEEDED(thePrimarySurface->Restore()))
    {
        Logger::log("Primary surface restored");
        itsPlayer->getRoom()->getImage()->copyDataToSurface(thePrimarySurface);
        Logger::log("Primary surface data re-copied");
        itsPlayer->getRoom()->getImage()->copyDataToSurface(theBackbuffer);
        Logger::log("Backbuffer data re-copied");
        return itsPlayer->getRoom()->restore(currentTime);
    }
    return FALSE;
}