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
#ifndef __SoundManager_h__
#define __SoundManager_h__

#include <dsound.h>

// This handles loading and playing sounds.
class SoundManager
{
protected:
    LPDIRECTSOUND itsDirectSoundObject;

    LPDIRECTSOUNDBUFFER **itsBuffers;
	char **itsFilenames;

    DWORD *itsBufferCopies;
    DWORD itsBufferSize;
    DWORD itsNumberOfBuffers;

    void ensureBufferSize(DWORD size);

    int loadSound(char *filename, BYTE **data);

    // Stores the sound data in itsBuffers, making sure to create the proper
    // number of copies
    void storeSound(DWORD index, BYTE *data, int dataSize, int copies,BOOL lowQuality=FALSE);

	BOOL isEnabled;

public:
    SoundManager(HWND hwnd, BOOL enable=TRUE);

    // returns the index of this sound.  Creates copies copies of it.  If lowQuality is true,
	// it will assume the sound is 8-bit mono, 22Khz.  If lowQuality is false, it
	// will assume 16-bit stereo, 44.1kHz.  Returns -1 if the sound couldn't be loaded
    int addSound(char *filename,DWORD copies,BOOL lowQuality=FALSE);

    // plays sound at index, looping if loop.  If force is true, this
    // will stop a playing copy of this sound if it has to to play the sound
    // now.  Which copy is stopped is undefined.  returs TRUE if it played the sound
    BOOL playSound(DWORD index, BOOL loop, BOOL force);

    // stops all copies of this sound
    void stopSound(DWORD index);

    void stopAllSounds();

    void release();



};
#endif //__SoundManager_h__ 