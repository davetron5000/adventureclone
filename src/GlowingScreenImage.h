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
#include "EventDispatcher.h"
#include <Logger.h>

#ifndef __GLOWINGSCREENIMAGE_H__
#define __GLOWINGSCREENIMAGE_H__

#define GLOWINGSCREENIMAGE_NUM_COLORS 8

// a glowing screen image has an effect like the chalice.  it periodically
// switches colors to give the effect that it's "glowing"
class GlowingScreenImage : public ScreenImage, public EventListener
{
protected:
	DWORD itsAnimationSpeed;
	int theColors[GLOWINGSCREENIMAGE_NUM_COLORS];
	int itsCurrentColorIndex;
	int itsTimerID;

public:
	virtual BOOL init(LPDIRECTDRAW directDrawObject, 
          ImageFileReader *reader, 
          char *filename, 
          DDCOLORKEY *colorKey=NULL,
		  BOOL monoColored=TRUE)
	{
		itsCurrentColorIndex = 0;
		itsAnimationSpeed = 75;
		
		itsTimerID = theEventDispatcher.setTimer(this,itsAnimationSpeed);

		// note, this assumes that the constant for the number of colors
		// is 8.  Not sure of a better way to do this
		theColors[0] = 144; // red
		theColors[1] = 191; // orange
		theColors[2] = 206; // yellow
		theColors[3] = 32;  // green
		theColors[4] = 4;   // blue
		theColors[5] = 255; // white
		theColors[6] = 127; // purple
		theColors[7] = 0;   // black


		BOOL returnVal = ScreenImage::init(directDrawObject,reader,filename,colorKey,monoColored);
		changeColor(theColors[itsCurrentColorIndex]);
		return returnVal;
	}

	void onTimer(int timerID)
	{
		if (timerID == itsTimerID)
		{
			itsTimerID = theEventDispatcher.setTimer(this,itsAnimationSpeed);
			itsCurrentColorIndex++;
			if (itsCurrentColorIndex >= GLOWINGSCREENIMAGE_NUM_COLORS)
			{
				itsCurrentColorIndex = 0;
			}
			changeColor(theColors[itsCurrentColorIndex]);
		}
	}


};

#endif //__GLOWINGSCREENIMAGE_H__