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

#ifndef __SimpleArray_h__
#define __SimpleArray_h__


// Simple array class that uses void poUINTers.  Doesn't do any error checking,
// so be careful.
class SimpleArray
{
    void **itsArray;

    // Allocated size of the array
    UINT itsArraySize;

    // Number of objects we're storing
    UINT itsNumberOfObjects;

    void ensureCapacity(UINT size);

public:
    SimpleArray(UINT startSize=20);
	
    ~SimpleArray();

    void *elementAt(UINT index);
    void addElement(void *object);
    void setElementAt(UINT index, void *object);
	inline UINT length() { return itsNumberOfObjects; }
    void removeElementAt(UINT index);
    int findObject(void *object);

#ifdef _DEBUG
    static BOOL test();
#endif

};
#endif