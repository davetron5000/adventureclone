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
#include "InputManager.h"
#include <Logger.h>
#include "Player.h"

#define IS_KEY_DOWN(data,n) (data[n] & 0x080)

const DWORD InputManager::theInputCooldownTime = 200;

char InputManager::theJoystickName[256] = "";

BOOL CALLBACK joystickEnumFunction(LPCDIDEVICEINSTANCE lpddi, LPVOID guid_ptr)
{
	*(GUID*)guid_ptr = lpddi->guidInstance;
	strcpy(InputManager::theJoystickName,(char *)lpddi->tszProductName);
	return DIENUM_STOP;
}

void InputManager::setupKeyMappings(char *filename)
{
	itsUpKey = DIK_UP;
	itsDownKey = DIK_DOWN;
	itsLeftKey = DIK_LEFT;
	itsRightKey = DIK_RIGHT;
	itsButtonKey = DIK_SPACE;

	itsResetKey = DIK_S;
	itsPauseKey = DIK_P;
	itsSoftResetKey = DIK_R;

	itsGameSelectKey = DIK_S;
	itsGameStartKey = DIK_R;

	itsScreenshotKey = DIK_F12;
}

BOOL InputManager::init(DWORD currentTime, HWND hwnd, HINSTANCE hinstance, UINT joystickRange,
						char *configurationFileName)
{
    itsDirectInputObject = NULL;
    itsKeyboard = NULL;
#if (DIRECTINPUT_VERSION >= 0x0500)
	itsJoystick = NULL;
#endif

	itsLastPauseTime = currentTime;
	itsLastGameSelectTime = currentTime;
	itsLastScreenshotTime = currentTime;

	setupKeyMappings(configurationFileName);


    BOOL returnValue = FALSE;
    if (SUCCEEDED(DirectInputCreate(hinstance,DIRECTINPUT_VERSION,&itsDirectInputObject,NULL)))
    {
        if (SUCCEEDED(itsDirectInputObject->CreateDevice(GUID_SysKeyboard,&itsKeyboard,NULL)))
        {
            if (SUCCEEDED(itsKeyboard->SetCooperativeLevel(hwnd,DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
            {
                if (SUCCEEDED(itsKeyboard->SetDataFormat(&c_dfDIKeyboard)))
                {
                    if (SUCCEEDED(itsKeyboard->Acquire()))
                    {
                        returnValue = TRUE;
                    }
                    else
                    {
                        Logger::error("Couldn't acquire keyboard!");
                    }
                }
                else
                {
                    Logger::error("Couldn't set data format on keyboard");
                }
            }
            else
            {
                Logger::error("Couldn't set cooperation level on keyboard");
            }
        }
        else
        {
            Logger::error("couldn't create keyboard device");
        }
#if (DIRECTINPUT_VERSION>=0x0500)
		// now create a joystick (maybe)
		if (SUCCEEDED(itsDirectInputObject->EnumDevices(
												DIDEVTYPE_JOYSTICK,
												joystickEnumFunction,
												&itsJoystickGUID,
												DIEDFL_ATTACHEDONLY)))
		{
			LPDIRECTINPUTDEVICE tmpJoystick;
			if (returnValue && (strcmp(theJoystickName,"") != 0) )
			{
				if (SUCCEEDED(itsDirectInputObject->CreateDevice(itsJoystickGUID,&tmpJoystick,NULL)) )
				{
					// need to get the DirectInputDevice2 interface version
					if (SUCCEEDED(tmpJoystick->QueryInterface(IID_IDirectInputDevice2,(void **)&itsJoystick)) )
					{
						if (tmpJoystick)
							tmpJoystick->Release();
						if (SUCCEEDED(itsJoystick->SetCooperativeLevel(hwnd,DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
						{
							if (SUCCEEDED(itsJoystick->SetDataFormat(&c_dfDIJoystick)))
							{
								DIPROPRANGE joystickAxisRange;

								joystickAxisRange.lMin = -1 * joystickRange;
								joystickAxisRange.lMax = joystickRange;
								joystickAxisRange.diph.dwSize = sizeof (DIPROPRANGE);
								joystickAxisRange.diph.dwHeaderSize = sizeof (DIPROPHEADER);
								joystickAxisRange.diph.dwObj = DIJOFS_X;
								joystickAxisRange.diph.dwHow = DIPH_BYOFFSET;
								itsJoystick->SetProperty(DIPROP_RANGE,&joystickAxisRange.diph);

								joystickAxisRange.lMin = -1 * joystickRange;
								joystickAxisRange.lMax = joystickRange;
								joystickAxisRange.diph.dwSize = sizeof (DIPROPRANGE);
								joystickAxisRange.diph.dwHeaderSize = sizeof (DIPROPHEADER);
								joystickAxisRange.diph.dwObj = DIJOFS_Y;
								joystickAxisRange.diph.dwHow = DIPH_BYOFFSET;
								itsJoystick->SetProperty(DIPROP_RANGE,&joystickAxisRange.diph);

								if (SUCCEEDED(itsJoystick->Acquire()) )
								{
								}
								else
								{
									Logger::error("Error acquiring joystick");
								}
							}
							else
							{
								Logger::error("Error setting joystick data format");
							}
						}
						else
						{
							Logger::error("Error setting joystick coop level");
						}
					}
					else
					{
						Logger::error("Error getting better joystick interface");
					}
				}
				else
				{
					Logger::error("Error creating joystick device");
				}
			}
			else
			{
				Logger::log("No joysticks attached to system");
			}
		}
		else
		{
			Logger::error("Error enumerating joystick devices");
		}
#endif
    }
    else
    {
        Logger::error("Couldn't create directinput object");
    }

    if (returnValue != TRUE)
    {
        if (itsKeyboard != NULL)
		{
			itsKeyboard->Unacquire();
            itsKeyboard->Release();
		}
#if (DIRECTINPUT_VERSION >= 0x0500)
		if (itsJoystick != NULL)
		{
			itsJoystick->Unacquire();
			itsJoystick->Release();
		}
#endif
        if (itsDirectInputObject != NULL)
            itsDirectInputObject->Release();
        itsKeyboard = NULL;
#if (DIRECTINPUT_VERSION >= 0x0500)
		itsJoystick = NULL;
#endif
        itsDirectInputObject = NULL;
    }
    return returnValue;
}

BOOL InputManager::release(DWORD currentTime)
{
	if (itsKeyboard != NULL)
	{
		itsKeyboard->Unacquire();
        itsKeyboard->Release();
	}
#if (DIRECTINPUT_VERSION >= 0x0500)
	if (itsJoystick != NULL)
	{
		itsJoystick->Unacquire();
		itsJoystick->Release();
	}
#endif
    if (itsDirectInputObject != NULL)
        itsDirectInputObject->Release();
    itsKeyboard = NULL;
    itsDirectInputObject = NULL;
	return TRUE;
}

BOOL InputManager::tick(DWORD currentTime)
{
	static UCHAR keystate[256];

	isLeft = FALSE;
	isRight = FALSE;
	isUp = FALSE;
	isDown = FALSE;
	isButton = FALSE;
	isSoftReset = FALSE;
	isHardReset = FALSE;
	isGameSelect = FALSE;
	isGameStart = FALSE;
	isPause = FALSE;
	isScreenshot = FALSE;
    
	// default to player movement speeds.
	itsXSpeed = Player::theMovementSpeed;
	itsYSpeed = Player::theMovementSpeed;
    if (SUCCEEDED(itsKeyboard->GetDeviceState(256,(LPVOID)keystate)))
    {
        if (IS_KEY_DOWN(keystate,itsUpKey))
        {
			isUp = TRUE;
			itsYSpeed *= -1;
        }
        if (IS_KEY_DOWN(keystate,itsDownKey))
        {
			isDown = TRUE;
        }
        if (IS_KEY_DOWN(keystate,itsLeftKey))
        {
			isLeft = TRUE;
			itsXSpeed *= -1;
        }
        if (IS_KEY_DOWN(keystate,itsRightKey))
        {
			isRight = TRUE;
        }
        if (IS_KEY_DOWN(keystate,itsButtonKey))
        {
			isButton = TRUE;
		}
		if (IS_KEY_DOWN(keystate,itsSoftResetKey))
		{
			if (currentTime - itsLastGameSelectTime > theInputCooldownTime)
			{
				isSoftReset = TRUE;
				itsLastGameSelectTime = currentTime;
			}
		}
		if (IS_KEY_DOWN(keystate,itsResetKey))
		{
				isHardReset = TRUE;
		}
		if (IS_KEY_DOWN(keystate,itsPauseKey))
		{
			if (currentTime - itsLastPauseTime > theInputCooldownTime)
			{
				isPause = TRUE;
				itsLastPauseTime = currentTime;
			}
		}
		if (IS_KEY_DOWN(keystate,itsGameSelectKey))
		{
			if (currentTime - itsLastGameSelectTime > theInputCooldownTime)
			{
				itsLastGameSelectTime = currentTime;
				isGameSelect = TRUE;
			}
		}
		if (IS_KEY_DOWN(keystate,itsGameStartKey))
		{
			isGameStart = TRUE;
		}
		if (IS_KEY_DOWN(keystate,itsScreenshotKey))
		{
			if (currentTime - itsLastScreenshotTime > theInputCooldownTime)
			{
				isScreenshot = TRUE;
				itsLastScreenshotTime = currentTime;
			}
		}
    }
#if (DIRECTINPUT_VERSION >= 0x0500)
	if ((itsJoystick != NULL) && (SUCCEEDED(itsJoystick->Poll())))
	{
		DIJOYSTATE joystate;
		if (SUCCEEDED(itsJoystick->GetDeviceState(sizeof(DIJOYSTATE),&joystate)))
		{
			itsXSpeed = abs(joystate.lX) < 30 ? 0 : joystate.lX;
			itsYSpeed = abs(joystate.lY) < 30 ? 0 : joystate.lY;
			if (!isButton)
				isButton = joystate.rgbButtons[0] & 0x80;

			if (itsXSpeed < 0)
				isLeft = TRUE;
			else if (itsXSpeed > 0)
				isRight = TRUE;

			if (itsYSpeed < 0)
				isUp = TRUE;
			else if (itsYSpeed > 0)
				isDown = TRUE;
		}
	}
#endif
    return TRUE;
}

