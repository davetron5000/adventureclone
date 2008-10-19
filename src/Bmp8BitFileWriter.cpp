#include "Bmp8BitFileWriter.h"
#include <Logger.h>

#define LOG_ERROR_MESSAGE(message,filename,lastError) \
	{ \
	LPVOID errorString;\
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, \
			  NULL,\
			  lastError,\
			  MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),\
			  (LPTSTR) &errorString,\
			  0,\
			  NULL);\
	Logger::error(message "(filename: %s error: %s)",filename,(char *)errorString);\
	LocalFree(errorString);\
	}

BOOL Bmp8BitFileWriter::write(char *filename, BYTE *data, int width, int height, RGBQUAD *palette)
{
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER infoHeader;

	header.bfType = 0x4D42;
	header.bfSize = sizeof(header) + sizeof(infoHeader) + (256 * sizeof(RGBQUAD)) + (width*height);
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits = sizeof(header) + sizeof(infoHeader) + (256 * sizeof(RGBQUAD));

	infoHeader.biSize = sizeof(infoHeader);
	infoHeader.biWidth = width;
	infoHeader.biHeight = height;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 8;
	infoHeader.biCompression = BI_RGB;
	infoHeader.biSizeImage = 0; // Allegedly, this is OK
	infoHeader.biXPelsPerMeter = 10; // whatever
	infoHeader.biYPelsPerMeter = 10; // whatever
	infoHeader.biClrUsed = 0; // should mean to use all colors
	infoHeader.biClrImportant = 0; // all colors are important

	HANDLE hFile = CreateFile(filename,GENERIC_WRITE,
		                      FILE_SHARE_WRITE,
							  NULL,
							  CREATE_NEW,
							  FILE_ATTRIBUTE_NORMAL,
							  NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD bytesWritten;

		if (0 != WriteFile(hFile,&header,sizeof(header),&bytesWritten,NULL))
		{
			if (0 != WriteFile(hFile,&infoHeader,sizeof(infoHeader),&bytesWritten,NULL))
			{
				if (0 != WriteFile(hFile,palette,256 * sizeof(RGBQUAD),&bytesWritten,NULL))
				{
					// have to write backwards
					BYTE *dataPtr = (data + (width * (height - 1)));
					BOOL isOK = TRUE;
					for (int i=0;i<height;i++)
					{
						if (0 != WriteFile(hFile,dataPtr,width,&bytesWritten,NULL))
						{
						}
						else
						{
							isOK = FALSE;
							LOG_ERROR_MESSAGE("Couldn't write data",filename,GetLastError());
							break;
						}
						dataPtr -= width;
					}
					CloseHandle(hFile);
					return TRUE;
				}
				else
				{
					LOG_ERROR_MESSAGE("Couldn't write palette",filename,GetLastError());
				}
			}
			else
			{
				LOG_ERROR_MESSAGE("Couldn't write info header",filename,GetLastError());
			}
		}
		else
		{
			LOG_ERROR_MESSAGE("Couldn't write header",filename,GetLastError());
		}
	}
	else
	{
		LOG_ERROR_MESSAGE("Couldn't create file",filename,GetLastError());

	}

	return FALSE;
}
