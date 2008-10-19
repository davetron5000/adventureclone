#include <Bmp8BitFileWriter.h>
#include <ddraw.h>

#ifndef __SCREENSHOTMANAGER_H__
#define __SCREENSHOTMANAGER_H__

// Manages taking and storing screenshots.  When initialized, this will
// set up a counter.  Each time a screenshot is taken, a Windows BMP file
// is written using a filebase and the counter (which is incremented 
// each time).  Things to note:
// - This will pick up where the last screenshot left off.  It does this 
//   by attempting to create the output file until successful (although
//   there is a failsafe mechanism that will stop after 100 attempts).
// - This writes out an 8-bit windows BMP, so it probably won't work with
//   a true color or hi-color surface.
class ScreenshotManager
{
protected:
	Bmp8BitFileWriter itsImageWriter;

	int itsNextIndex;

	char *itsFileBase;

public:
	ScreenshotManager() { itsFileBase = NULL; }

	// init the screenshot manager.  fileBase will have the ordinal number of
	// the screenshot appended to it each time a screenshot is taken.
	// makes a copy of the filebase
	BOOL init(char *fileBase);

	// Writes this surface out to a file.  This will get the height,
	// wdith, palette, etc. from the surface.
	BOOL writeScreenShot(LPDIRECTDRAWSURFACE surface);

	// release resources
	BOOL release();
};

#endif //__SCREENSHOTMANAGER_H__