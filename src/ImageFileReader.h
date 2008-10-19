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

#ifndef __ImageFileReader_h__
#define __ImageFileReader_h__



// ImageFileReader is an abstract base class for reading images from files on disk.
// Subclass this for the particular format of the image.
class ImageFileReader
{
public:

    // Loads the image contained within the filename.  You must pass in width, height
    // data, dataLength, palette, and palettelength pointers that will be populated with 
    // the raw data of the image, as well as the width and height.  
    // If the subclass uses an indexed color mode,
    // the palette will be non-null and palette length will be nonzero
    // You are responsible for deallocating the data and palette arrays you recieve
    // from this method.
    //
    // If false is returned, assume that no memory needs to be deallocated (the pointers
    // in question should be set to null
    virtual BOOL load(char *filename, 
                      ULONG *width, 
                      ULONG *height, 
                      BYTE **data, 
                      ULONG *dataLength,
                      RGBQUAD **palette,
                      ULONG *paletteLength) = 0;
};

#endif // __ImageFileReader_h__