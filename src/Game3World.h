#include "Game2World.h"

#ifndef __GAME3WORLD_H__
#define __GAME3WORLD_H__

// We store locations for each room as to where objects should be when "randomly"
// placed there.
class Game3World : public Game2World
{
protected:

	int usedRooms[GAME2_NUMBER_OF_ROOMS];

	// if chalice is TRUE, the room must be able to take a chalice.
	// empty is the max number of objects that can be at the room to assing
	int getRandomRoom(BOOL chalice, int empty);

	BOOL isRoomSuitable(Room *room, int number, BOOL chalice, int empty);

public:
	Game3World(BOOL difficult=FALSE,BOOL extraItems=FALSE);
	// using the same map as game2world, this sets objects at psuedo-random positions
	virtual void setObjectsPositionsAndRooms();
};

#endif // __GAME3WORLD_H__