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

#include <dinput.h>
#include <windows.h>

#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

class InputManager
{
protected:

	void setupKeyMappings(char *filename);

	LPDIRECTINPUT itsDirectInputObject;
	LPDIRECTINPUTDEVICE itsKeyboard;
#if (DIRECTINPUT_VERSION>=0x0500)
	LPDIRECTINPUTDEVICE2 itsJoystick;
#endif

    UCHAR itsUpKey;
    UCHAR itsDownKey;
    UCHAR itsLeftKey;
    UCHAR itsRightKey;
    UCHAR itsButtonKey;

	UCHAR itsScreenshotKey;

	UCHAR itsResetKey;
	UCHAR itsPauseKey;
	UCHAR itsSoftResetKey;

	UCHAR itsGameSelectKey;
	UCHAR itsGameStartKey;

	DWORD itsLastPauseTime;
	DWORD itsLastGameSelectTime;

	DWORD itsLastScreenshotTime;

	static const DWORD theInputCooldownTime;

	GUID itsJoystickGUID;

	BOOL isJoystick;

public:

	static char theJoystickName[256];

	BOOL isLeft;
	BOOL isRight;
	BOOL isUp;
	BOOL isDown;
	BOOL isButton;

	BOOL isScreenshot;

	BOOL isSoftReset;
	BOOL isHardReset;

	BOOL isGameSelect;
	BOOL isGameStart;

	BOOL isPause;

	// if using an analog joystick, these are the speeds
	int itsXSpeed;
	int itsYSpeed;

	BOOL init(DWORD currentTime, HWND hwnd, HINSTANCE hinstance, UINT joystickRange,
		      char *configurationFileName=NULL);


	BOOL release(DWORD currentTime);

	BOOL tick(DWORD currentTime);
};

#endif // __INPUTMANAGER_H__