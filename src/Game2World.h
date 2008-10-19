#include "World.h"

#ifndef __GAME2WORLD_H__
#define __GAME2WORLD_H__

#define GAME2_NUMBER_OF_ROOMS 32

class Game2World : public World
{
protected:

	
	SimpleArray *itsGateImages;
	SimpleArray *itsChaliceImages;
	SimpleArray *itsBatImages;
	SimpleArray *itsRobinettImages;
	SimpleArray *itsThanksImages;
	SimpleArray *itsDavetronImages;
	// OK, the main difference between game 2 and 3 is that in game 3 the objects
	// start off in pseudo random locations, but in game 2, the locations are
	// the same.  So, we have the following list of rooms and x/y coords for each 
	// of the objects.
	// If the X coordinate is set at -1, then the default starting location for the requested room
	// will be used to place the object in the room.  This is handy for randomly assigning objects to
	// rooms because you don't need to figure out the layout; just use the defaults

    int itsYellowKeyRoom;
    int itsYellowKeyX;
    int itsYellowKeyY;

    int itsWhiteKeyRoom;
    int itsWhiteKeyX;
    int itsWhiteKeyY;

    int itsBlackKeyRoom;
    int itsBlackKeyX;
    int itsBlackKeyY;

    int itsYellowDragonRoom;
    int itsYellowDragonX;
    int itsYellowDragonY;

    int itsGreenDragonRoom;
    int itsGreenDragonX;
    int itsGreenDragonY;

    int itsRedDragonRoom;
    int itsRedDragonX;
    int itsRedDragonY;

    int itsSwordRoom;
    int itsSwordX;
    int itsSwordY;

    int itsMagnetRoom;
    int itsMagnetX;
    int itsMagnetY;

    int itsBatRoom;
    int itsBatX;
    int itsBatY;

    int itsDotRoom;
    int itsDotX;
    int itsDotY;

    int itsBridgeRoom;
    int itsBridgeX;
    int itsBridgeY;

    int itsChaliceRoom;
    int itsChaliceX;
    int itsChaliceY;

	int itsCandleX;
	int itsCandleY;
	int itsCandleRoom;

	int itsArmourX;
	int itsArmourY;
	int itsArmourRoom;

	// if TRUE, the normally visible mazes (the "Labrynths") are dark
	BOOL isMazeDark;
	int itsDragonMouthOpenTime;
	int itsNonRedDragonSpeed;

	// a subclass can overrid this to change the x,y or room of objects after the rooms
	// have been created, but before the objects are.
	virtual void setObjectsPositionsAndRooms() {}

public:
	Game2World(BOOL difficult=FALSE, BOOL extraItems=FALSE);

    // sets up the rooms and objects
    virtual BOOL init(DWORD currentTime, LPDIRECTDRAW directDrawObject);

    // release resources
    virtual void release();
};

#endif // __GAME2WORLD_H__