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
#include <assert.h>

#include "Logger.h"
#include "Properties.h"


BOOL Properties::init(char *filename)
{
	itsPropertyList = new SimpleArray();
	HANDLE hfile = CreateFile(filename,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            (LPSECURITY_ATTRIBUTES)NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            (HANDLE)NULL);

	DWORD bytesRead;
	char oneLine[GAMELIB_PROPERTIES_MAXLINELENGTH];
	char buffer[GAMELIB_PROPERTIES_MAXLINELENGTH];

	oneLine[0] = '\0';

	BOOL isOK = TRUE;

	// OK, yuck.  There are two loops.  The outer loop is the _read loop_ and the inner
	// loop is the _line processing loop_.  The read loop fills up buffer with as much as
	// it can get from the file.  The line processing loop copies from that buffer into
	// oneLine, such that oneLine always contains either  a complete line or all characters
	// on one line that have been read from buffer so far.  Basically it copies chars into
	// oneLine until hitting a \n.  At that point, it calls processLine, which tokenizes
	// the line and puts it in the data structure.
	// After taht, it resets oneLine and linePtr, and picks up reading from buffer where
	// it left off.  If it gets to the end of buffer before reading an entire line,
	// the line processing loop exits, and we have another iteration of the read loop.
	// At the end of the read loop, if there's anything left in oneLine, send it to 
	// processLine
	while ( isOK )
	{
		isOK = ReadFile(hfile,buffer,GAMELIB_PROPERTIES_MAXLINELENGTH-1,&bytesRead,NULL);
		if (isOK)
		{
			buffer[bytesRead] = '\0';
			char *linePtr, *bufPtr;

			bufPtr = buffer;
			linePtr = strchr(oneLine,'\0');
			BOOL done = FALSE;
			DWORD count = 0;
			while ( !done )
			{
				if (*bufPtr == '\n')
				{
					// make sure oneLine doesn't end in cr or lf
					if (*(linePtr-1) == '\r')
						*(linePtr-1) = '\0';

					processLine(oneLine);

					// now reset it
					oneLine[0] = '\0';
					linePtr = oneLine;
				}
				else
				{
					*linePtr = *bufPtr;
					// only increment linePtr if we wrote to it
					linePtr++;
					// not that necessary, but helps for debugging to have oneLine
					// always be null terminated
					*linePtr ='\0';
				}
				// always increment bufPtr
				bufPtr++;
				count++;
				if (count >= bytesRead)
				{
					done = TRUE;
				}

			}
			*linePtr = '\0';
		}
		if (bytesRead == 0)
			isOK = FALSE;
	}
	if (oneLine[0] != '\0')
	{
		processLine(oneLine);
	}


	return isOK;
}

void Properties::processLine(char *line)
{
	char *key = line;
	char *value = strchr(line,'=');
	char *zeroMe;

	if ((*key == '#') || (value == NULL) || (value == key))
		return;

	value++;

	// start one behind the equals sign
	zeroMe = value-2;
	while (isspace(*zeroMe))
	{
		zeroMe--;
	}
	zeroMe++; // we are pointing to a non-space, so go one beyond
	*zeroMe = '\0';

	while (isspace(*key)) { key++; }

	while (isspace(*value)) { value++; }
	zeroMe = value + strlen(value);
	zeroMe--;

	while (isspace(*zeroMe))
	{
		zeroMe--;
	}
	zeroMe++; // we are pointing to a non-space, so go one beyond
	*zeroMe = '\0';

	put(key,value);
	
};

BOOL Properties::release()
{
	for (UINT i=0;i<itsPropertyList->length();i++)
	{
		Property *p = (Property *)(itsPropertyList->elementAt(i));
		delete p;
	}
	delete itsPropertyList;
	itsPropertyList = NULL;
	return TRUE;
}

const char * Properties::get(const char *key)
{
	for (UINT i=0;i<itsPropertyList->length();i++)
	{
		Property *p = (Property *)(itsPropertyList->elementAt(i));
		if (strcmp(p->itsKey,key) == 0)
			return p->itsValue;
	}
	return NULL;
}

void Properties::put(const char *key, const char *value)
{
	assert(key != NULL);
	assert(value != NULL);

	Property *p = new Property(key,value);

	itsPropertyList->addElement(p);
}

void Properties::debugLog()
{
	Logger::log("Properties {");
	for (UINT i=0;i<itsPropertyList->length();i++)
	{
		Property *p = (Property *)(itsPropertyList->elementAt(i));
		Logger::log("'%s'=>'%s',",p->itsKey,p->itsValue);
	}
	Logger::log("};");
}