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
#include <RectangularImage.h>
#include <Logger.h>

#ifndef __WallImage_h__

#define __WallImage_h__

// Wall image is just a RectangularImage, but for historical reasons it
// needs to be it's own type.
class WallImage : public RectangularImage
{
};
#endif // __WallImage_h__