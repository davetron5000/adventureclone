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

#include "ImageFileWriter.h"

#ifndef __BMP8BITFILEWRITER_H__
#define __BMP8BITFILEWRITER_H__

class Bmp8BitFileWriter
{
public:
	// write out image data.  if palette is non-null, it assumes this is a 256 color palettized image.
	// for 16 or higher bit, set palette to null.
	BOOL write(char *filename, BYTE *data, int width, int height, RGBQUAD *palette);
};

#endif //__BMP8BITFILEWRITER_H__