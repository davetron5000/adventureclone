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
#include <string.h>

#include "SimpleArray.h"

#ifndef __PROPERTIES_H__
#define __PROPERTIES_H__

// This is non-hashtable type hashtable.  It is basically a list of
// key/value pairs that have been read from a file.  A subclass could
// implement this as a hashtable, but this is pretty brute-force, so don't
// fill it up with a bunch of stuff or use it in the game loop too much.
//
// The file that is read from is bascially a key = value type thing.  Leading
// and training whitespace are ignored, and lines starting with '#' (sans quotes)
// are ignored.  keys and values should not contain an equals sign.  No line may be
// longer than 128 characters (or whatever the constant GAMELIB_PROPERTIES_MAXLINELENGTH
//  is defined as)

#define GAMELIB_PROPERTIES_MAXLINELENGTH 128

class Properties
{
private:
	// takes one line and stores it's key/value.  This will modify 
	// the contents of the passed-in buffer!!!
	void processLine(char *line);
protected:
	SimpleArray *itsPropertyList;

	class Property
	{
	public:
		Property(const char *key, const char *value)
		{
			itsKey = strdup(key);
			itsValue = strdup(value);
		}
		char *itsKey;
		char *itsValue;

		~Property()
		{
			if (itsKey != NULL) free(itsKey);
			if (itsValue != NULL) free(itsValue);
		}
	};
public:
	// read in the file as a property file (see class comment).  If there
	// was any problem, returns FALSE
	BOOL init(char *filename);

	// release used resources. 
	BOOL release();

	// returns the associated value of the key, or NULL if the key
	// doesn't exist.  This returns a pointer to teh actual value, not a copy.
	const char * get(const char *key);

	// set a key in the list to the specified value.  key and value must
	// not be NULL.  This class will make a copy of the key and value.
	void put(const char *key, const char *value);

	// logs the whole thing to the logger
	void debugLog();
};

#endif // __PROPERTIES_H__