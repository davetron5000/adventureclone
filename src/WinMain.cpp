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
#include <windowsx.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include <objbase.h>

#define INITGUID

#include <Logger.h>
#include <SimpleArray.h>
#include "Game.h"
#include <Bmp8BitFileReader.h>
#include <RandomGenerator.h>
#include "EventDispatcher.h"
#include "resource.h"
#include <Properties.h>

BOOL isAppActive=FALSE;
HWND theHwnd;
Game theGame;
DWORD theMillisecondsPerFrame;
EventDispatcher theEventDispatcher;

long CALLBACK WindowProc(HWND hwnd,UINT message,
                         WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
        case WM_ACTIVATEAPP: 
            isAppActive=wParam; 
            Logger::log("Application activated");
            break;
        case WM_CREATE: 
            break; 
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
                DestroyWindow(hwnd);
            break;
        case WM_DESTROY: //end of game
            Logger::log("Application exited");
            theGame.release(timeGetTime());
            ShowCursor(TRUE); //show the mouse cursor
            PostQuitMessage(0); //and exit
            break;
        default: //default window procedure
            return DefWindowProc(hwnd,message,wParam,lParam);
    } //switch(message)
    return 0L;
} //WindowProc

//windows system functions
HWND createDefaultWindow(char* name,HINSTANCE hInstance)
{
    WNDCLASS wc; //window registration info
    //register display window
    wc.style=CS_HREDRAW|CS_VREDRAW; //style
    wc.lpfnWndProc=WindowProc; //window message handler
    wc.cbClsExtra=wc.cbWndExtra=0;
    wc.hInstance=hInstance;
    wc.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor=LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground=NULL;
    wc.lpszMenuName=NULL;
    wc.lpszClassName=name;
    RegisterClass(&wc);
    //create and set up fullscreen window
    return CreateWindowEx(WS_EX_TOPMOST,name,name,
                          WS_POPUP,0,0,GetSystemMetrics(SM_CXSCREEN),
                          GetSystemMetrics(SM_CYSCREEN),NULL,NULL,hInstance,NULL);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,int nCmdShow)
{
    Logger::init("log.txt");

	char *worldname = lpCmdLine;
    
    theHwnd = createDefaultWindow("Adventure",hInstance);
    if( !theHwnd )
    {
        return FALSE;
    }

    //set up window
    ShowWindow(theHwnd,nCmdShow); 
    UpdateWindow(theHwnd);
    SetFocus(theHwnd); //allow input from keyboard
    ShowCursor(FALSE); //hide the cursor

	theMillisecondsPerFrame = 34;

	if (theGame.init(timeGetTime(),worldname,theHwnd,hInstance))
    {
        MSG msg; //current message
        DWORD currentTime,lastTime = timeGetTime();
        while(TRUE)
        {
            if (PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
            {
                if(!GetMessage(&msg,NULL,0,0))
                    return msg.wParam;
                TranslateMessage(&msg); 
                DispatchMessage(&msg);
            }
            else 
            {
                currentTime = timeGetTime();
				if ( (currentTime - lastTime) >= theMillisecondsPerFrame )
				{
					lastTime = currentTime;
					if (isAppActive)
					{
						if (FALSE == theGame.tick(currentTime))
						{
							Logger::error("Got error from tick!");
							DestroyWindow(theHwnd);
						}
					}
					else
					{
						WaitMessage();
					}
				}
				
            }
        }
    }
    else
    {
        return FALSE;
    }
} //WinMain
