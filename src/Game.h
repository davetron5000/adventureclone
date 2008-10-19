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

#include <SoundManager.h>
#include "SoundPlayer.h"
#include <ImageFileReader.h>
#include <ScreenImage.h>
#include <ScreenshotManager.h>

#include "WallImage.h"
#include "GameObject.h"
#include "Room.h"
#include "Wall.h"
#include "Player.h"
#include "World.h"
#include "EventListener.h"
#include "InputManager.h"

#ifndef __Game_h__
#define __Game_h__





// The Game is the class that controls everything in the game (duh).  It
// encapsulates startup and shutdown, but does not encapsulate a game loop.
class Game : public EventListener
{

protected:

	InputManager itsInputManager;

	char *itsCurrentWorldName;

    World *itsWorld;

	// The new world to go to when the loop finishes
	World *itsNewWorld;

	int itsInstructionMessageTimerID;

	ScreenImage itsInstructionMessage;

	BOOL isDisplayingInstructions;

    DWORD itsLastTickTime;

	int itsWinSoundIndex;

    BOOL processInput(DWORD currentTime);

	BOOL updatePreGame(DWORD currentTime);

	SimpleArray *itsGameSelectImages;

	ScreenshotManager itsScreenshotManager;

	void pregameUpdate();

	BOOL isInGameSelectWorld;

	UINT itsCurrentlySelectedGame;

    // go through each object and check for collisions
    BOOL checkForCollisions();

    // Do any other processing needed each time through the loop
    // (including allowing other game objects to do same)
    BOOL update();

    // draw whatever is needed at this time.
    BOOL draw();

    BOOL startDirectDraw(HWND hwnd,HINSTANCE hinstance);

    BOOL startDirectSound(HWND hwnd,HINSTANCE hinstance);

    BOOL startDirectInput(HWND hwnd,HINSTANCE hinstance);

    BOOL initSurfaces(HWND hwnd,HINSTANCE hinstance);

    BOOL restoreSurfaces(DWORD currentTime);

    Room *itsCurrentRoom;

    Player *itsPlayer;
    
	enum GameState
	{
		GAMESTATE_INPROGRESS,
		GAMESTATE_WON,
		GAMESTATE_LOST,
		GAMESTATE_PAUSED,
		GAMESTATE_PREGAME,
	};


	struct StateChangeRequest
	{
		// if FALSE, the rest of this data structure should be ignored
		BOOL isActiveRequest;
		// the new state we wish to change to
		GameState itsNewState;
		// if not NULL, the new world to change to
		World *itsNewWorld;
	} itsNewStateRequest;

	void handleStateChanges(DWORD currentTime);

	GameState itsCurrentState;

	BOOL flip();
	
	BOOL pause(DWORD currentTime);
	BOOL unPause(DWORD currentTime);

	DWORD itsWinTime;
	DWORD itsUnPauseTime;
	DWORD itsPauseTime;

	// Since hitting the pause key results in 80,000,000 of that key being hit,
	// I'm going to set a time period for cooling down, where I'll ignore the 
	// pause key.  Seems kinda cheesy, but I'm not sure what else to do.
	DWORD itsPauseCooldown;

	static const DWORD theWinAnimationLength;

	// returns true if the player has won the game
	virtual BOOL gameWon();

	// returns true if the player lost the game
	virtual BOOL gameLost();

	SoundManager *itsSoundManager;
	SoundPlayer *itsSoundPlayer;

	BOOL start(DWORD currentTime);
	BOOL stop(DWORD currentTime);
public:

	static LPDIRECTDRAW theDirectDrawObject;

    static LPDIRECTDRAWSURFACE thePrimarySurface;
    static LPDIRECTDRAWSURFACE theBackbuffer;
    static LPDIRECTDRAWPALETTE thePalette;
	static LPDIRECTDRAWPALETTE theDimmedPalette;
    static LPDIRECTDRAWPALETTE theBackbufferPalette;

    static LPDIRECTDRAWCLIPPER theClipper;

    static const int theScreenWidth;
    static const int theScreenHeight;
    static const int theBitDepth;

    Game();

	void onEnterRoom(GameObject *object, Room *room);

	void onTimer(int timerID);

	// inits global objects, such as direct draw.  worldname is the name of the
	// world to create initially.
    BOOL init(DWORD currentTime, char *worldname, HWND hwnd,HINSTANCE hinstance);


	// release global objects, such as direct draw
    BOOL release(DWORD currentTime);

    // Called each time through the game loop
    // returns FALSE if there was a problem and the program should 
    // exit
    BOOL tick(DWORD currentTime);


};

#endif // __Game_h__