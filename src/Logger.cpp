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
#include "Logger.h"

FILE *Logger::theLogFile;

void Logger::init(const char *filename)
{
#if LOGGING == 1
    theLogFile = fopen(filename,"w");
    if (theLogFile)
    {
        char buf[32];
        getTimeStamp(buf);
        fputs(buf,theLogFile);
        fputs(" - Logger::init called\n", theLogFile);
        fflush(theLogFile);
    }
#endif
}

void Logger::getTimeStamp(char *into)
{
    struct tm *newtime;
    time_t longtime;
    time( &longtime );
    newtime = localtime( &longtime );
    strcpy(into,asctime(newtime));
    char *p = strrchr(into, '\n');
    if (p != NULL)
        *p = '\0';
}

void Logger::log(const char *fmt,...)
{
#if LOGGING == 1
    char buff[512];
    va_list vl;

    va_start(vl, fmt);
    if (_vsnprintf(buff, sizeof(buff)-2, fmt, vl)==-1)
        buff[sizeof(buff)-2] = 0;
    strcat(buff, "\n");
    OutputDebugString(buff);
    if (theLogFile != NULL) 
    {
        char buff2[32];
        getTimeStamp(buff2);
        fputs(buff2, theLogFile); 
        fputs(" - ", theLogFile); 
        fputs(buff, theLogFile); 
        fflush(theLogFile); 
    }
    va_end(vl);
#endif
}

void Logger::error(const char *fmt,...)
{
    char buff[512];
    va_list vl;

    va_start(vl, fmt);
    if (_vsnprintf(buff, sizeof(buff)-2, fmt, vl)==-1)
        buff[sizeof(buff)-2] = 0;
    strcat(buff, "\n");
    OutputDebugString(buff);
    if (theLogFile != NULL) 
    {
        char buff2[64];
        getTimeStamp(buff2);
        fputs(buff2, theLogFile); 
        fputs(" - ERROR - ", theLogFile); 
        fputs(buff, theLogFile); 
        fflush(theLogFile); 
    }
    va_end(vl);
}