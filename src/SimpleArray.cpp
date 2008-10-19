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
#ifdef _DEBUG
#include "Logger.h"
#endif

#include "SimpleArray.h"
#include "assert.h"

SimpleArray::SimpleArray(UINT startSize)
{
    itsArray = new void*[startSize];
    itsArraySize = startSize;
    itsNumberOfObjects = 0;
}

int SimpleArray::findObject(void *object)
{
    for (UINT i=0;i<itsNumberOfObjects;i++)
    {
        if (itsArray[i] == object)
            return i;
    }
    return -1;
}
void SimpleArray::removeElementAt(UINT index)
{
    assert(index < itsNumberOfObjects);
    assert(index >= 0);
    for (UINT i=index;i<(itsNumberOfObjects-1);i++)
    {
        itsArray[i] = itsArray[i+1];
    }
    itsNumberOfObjects--;
}
void *SimpleArray::elementAt(UINT index)
{
    assert(index < itsNumberOfObjects);
    return itsArray[index];
}

void SimpleArray::addElement(void *object)
{
    ensureCapacity(itsNumberOfObjects+1);
    itsArray[itsNumberOfObjects] = object;
    itsNumberOfObjects++;
}



void SimpleArray::setElementAt(UINT index, void *object)
{
    assert(index < itsNumberOfObjects);
    itsArray[index] = object;
}

void SimpleArray::ensureCapacity(UINT size)
{
    if (size <= itsArraySize) return;
    while (size > itsArraySize)
    {
        itsArraySize *= 2;
    }
    void **oldArray = itsArray;
    itsArray = new void*[itsArraySize];
    for (UINT i=0;i<itsNumberOfObjects;i++)
    {
        itsArray[i] = oldArray[i];
    }

    delete[] oldArray;
}

SimpleArray::~SimpleArray()
{
    delete[] itsArray;
}

#ifdef _DEBUG

class DumbObject
{
protected:
    UINT itsValue;
public:
    DumbObject(UINT val) { itsValue = val; }
    UINT getValue() { return itsValue; }
    void setValue(UINT val) { itsValue = val; }
};

BOOL SimpleArray::test()
{
    {
        SimpleArray array(4);

        char *one   = "dave";
        char *two   = "copeland";
        char *three = "mitra";
        char *four  = "moshari";
        char *five  = "foo";
        char *six   = "bar";

        array.addElement((void *)one);
        array.addElement((void *)two);
        array.addElement((void *)three);
        array.addElement((void *)four);
        array.addElement((void *)five);
        array.addElement((void *)six);

        assert(array.length() == 6);
        assert( 0 == (strcmp((char *)array.elementAt(0),one)) );
        assert( 0 == (strcmp((char *)array.elementAt(1),two)) );
        assert( 0 == (strcmp((char *)array.elementAt(2),three)) );
        assert( 0 == (strcmp((char *)array.elementAt(3),four)) );
        assert( 0 == (strcmp((char *)array.elementAt(4),five)) );
        assert( 0 == (strcmp((char *)array.elementAt(5),six)) );

        Logger::log("SimpleArray Passed test regarding strings");
    }
    {
        SimpleArray array(4);

        DumbObject *one   = new DumbObject(1);
        DumbObject *two   = new DumbObject(2);
        DumbObject *three = new DumbObject(3);
        DumbObject *four  = new DumbObject(4);
        DumbObject *five  = new DumbObject(5);
        DumbObject *six   = new DumbObject(6);

        array.addElement((void *)one);
        array.addElement((void *)two);
        array.addElement((void *)three);
        array.addElement((void *)four);
        array.addElement((void *)five);
        array.addElement((void *)six);

        assert(array.length() == 6);
        assert( 1 == ( ( (DumbObject *) (array.elementAt(0)) )->getValue() ) );
        assert( 2 == ( ((DumbObject *)(array.elementAt(1)))->getValue() ) );
        assert( 3 == ( ((DumbObject *)(array.elementAt(2)))->getValue() ) );
        assert( 4 == ( ((DumbObject *)(array.elementAt(3)))->getValue() ) );
        assert( 5 == ( ((DumbObject *)(array.elementAt(4)))->getValue() ) );
        assert( 6 == ( ((DumbObject *)(array.elementAt(5)))->getValue() ) );

        array.setElementAt(2,(void *)one);

        assert( 1 == ( ((DumbObject *)array.elementAt(2))->getValue() ) );

        Logger::log("SimpleArray Passed test regarding objects");

    }
    return TRUE;
}
#endif