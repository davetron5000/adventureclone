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
#include "Bmp8BitFileReader.h"
#include "Logger.h"


BOOL Bmp8BitFileReader::load(char *filename, ULONG *width, ULONG *height, 
                             BYTE **data, ULONG *dataLength,
                             RGBQUAD **palette, ULONG *paletteLength)
{
    BOOL isOK=FALSE;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    
    *data = NULL;
    *palette = NULL;

    //open input file for reading
    HANDLE hfile = CreateFile(filename,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            (LPSECURITY_ATTRIBUTES)NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            (HANDLE)NULL);

    //bail if file could not be opened
    if ( hfile == INVALID_HANDLE_VALUE )
        return FALSE;
    //read the header and info structures
    DWORD actualBytesRead;
    
    if ( ReadFile(hfile,&fileHeader,sizeof(fileHeader),&actualBytesRead,NULL) )
    {
        if ( ReadFile(hfile,&infoHeader,sizeof(infoHeader),&actualBytesRead,NULL) )
        {
            if ( infoHeader.biBitCount == 8 )
            {
                (*palette) = new RGBQUAD[256];
                (*paletteLength) = 256;
                
                if ( ReadFile(hfile,(*palette),256 * sizeof(RGBQUAD),&actualBytesRead,NULL) )
                {
                    //allocate memory for image data
                    int imageNumPixels=infoHeader.biWidth * infoHeader.biHeight;

                    (*data) = new BYTE[imageNumPixels]; //allocate new space
    
                    if ( data != NULL )
                    {
                        *dataLength = imageNumPixels;

                        (*width)  = infoHeader.biWidth;
                        (*height) = infoHeader.biHeight;

                        //round up line width to next multiple of 4
                        UINT paddedWidth=(*width+3)&~3;
                        //read bmp image
                        UINT i=0; //counter
                        BYTE trash[4]; //to hold the trash at the end of each line
                        int remainder=paddedWidth-*width; //width of trash
                        
                        isOK = TRUE;
                        
                        while ( isOK && (i < *height) )
                        {
                            // BMP data is stored backwards for some reason.  We want our array to
                            // be not backwards, so read it in and fill in *data from the end.

                            isOK = isOK && 
                                 ReadFile(hfile,
                                      (BYTE*)((*data) + ((*height - i - 1) * (*width) ) ),
                                      (*width),
                                      &actualBytesRead,
                                      NULL);
                            if (!isOK)
                            {
                                Logger::error("Couldn't read %d bytes from %s (data)!",*width,filename);
                            }
                            //read trash at end of line
                            isOK = isOK && ReadFile(hfile,trash,remainder,&actualBytesRead,NULL);
                            if (!isOK)
                            {
                                Logger::error("Couldn't read %d bytes from %s (remainder)!",remainder,filename);
                            }
                            i++; //next line
                        }
                    }
                    else
                    {
                        Logger::error("Couldn't allocate %d BYTEs for image data for %s",imageNumPixels,filename);
                    }
                }
                else
                {
                    Logger::error("Couldn't read palette from %s",filename);
                }
            }
            else
            {
                Logger::error("%s is not an 8-bit bitmap!",filename);
            }
        }
        else
        {
            Logger::error("Couldn't read the info header from %s",filename);
        }
    }
    else
    {
        Logger::error("Couldn't read the file header from %s",filename);
    }

    if ( !isOK )
    {
        Logger::error("There was some error reading the file %s",filename);
        Logger::error("If you got no other error message than this one, it probably means premature end of file");
        if (*data != NULL) 
            delete[] (*data);
        if (*palette != NULL) 
            delete[] (*palette);
        (*data) = NULL;
        (*palette) = NULL;
        *dataLength = 0;
        *paletteLength = 0;
        *width = 0;
        *height = 0;
    }
    //close up and exit
    CloseHandle(hfile);
    return isOK;
}
