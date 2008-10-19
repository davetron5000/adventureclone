#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "World.h"

#ifndef __GAME1WORLD_H__
#define __GAME1WORLD_H__

class Game1World : public World
{
protected:

	SimpleArray *itsGateImages;
	SimpleArray *itsChaliceImages;
	SimpleArray *itsBatImages;

	BOOL isMazeDark;
	int itsDragonMouthOpenTime;

public:

	Game1World(BOOL difficult=FALSE, BOOL extraItems=FALSE);

    // sets up the rooms and objects
    virtual BOOL init(DWORD currentTime, LPDIRECTDRAW directDrawObject);

    // release resources
    virtual void release();
};

#endif // __GAME1WORLD_H__