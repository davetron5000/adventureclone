#include <string.h>
#include <assert.h>

#include "SoundManager.h"
#include "Logger.h"

void SoundManager::ensureBufferSize(DWORD size)
{
	// no op , fix this later
}

int SoundManager::loadSound(char *filename, BYTE **buffer)
{
   
  HMMIO hmfr;
  MMCKINFO parent,child;
  WAVEFORMATEX wfmtx;
  int size=0;
  //reclaim memory from buffer, if already used
  delete[]*buffer;
  //init parent and child MMCKINFOs
  parent.ckid=(FOURCC)0;
  parent.cksize=0;
  parent.fccType=(FOURCC)0;
  parent.dwDataOffset=0;
  parent.dwFlags=0;
  child=parent;
  //open sound file
  hmfr=mmioOpen(filename,NULL,MMIO_READ|MMIO_ALLOCBUF);
  if(hmfr==NULL)return NULL;
   

  //descend into the RIFF
  parent.fccType=mmioFOURCC('W','A','V','E');
  if(mmioDescend(hmfr,&parent,NULL,MMIO_FINDRIFF)){
    mmioClose(hmfr,0); return NULL; //not a wave file
  }
   
  //descend to the WAVEfmt
  child.ckid=mmioFOURCC('f','m','t',' ');
  if(mmioDescend(hmfr,&child,&parent,0)){
    mmioClose(hmfr,0); return NULL; //file has no fmt chunk
  }
   
  //read the WAVEFMT from the wave file
  if(mmioRead(hmfr,(char*)&wfmtx,sizeof(wfmtx))!=sizeof(wfmtx)){
    mmioClose(hmfr,0); return NULL; //unable to read fmt chunk
  }
   
  //check wave format
  if(wfmtx.wFormatTag!=WAVE_FORMAT_PCM){
    mmioClose(hmfr,0); return NULL; //WAVE file is not PCM format
  }
   
  //ascend back to RIFF level
  if(mmioAscend(hmfr,&child,0)){
    mmioClose(hmfr,0); return NULL; //unable to ascend
  }
   
  //descend to the data chunk
  child.ckid=mmioFOURCC('d','a','t','a');
  if(mmioDescend(hmfr,&child,&parent,MMIO_FINDCHUNK)){
    mmioClose(hmfr,0); return NULL; //WAVE file has no data chunk
  }
   
  //grab memory to store sound
  size=child.cksize;
  *buffer=new BYTE[size];
  if(*buffer==NULL){
    mmioClose(hmfr,0); return NULL; //out of memory
  }
  //read the wave data
  if((int)mmioRead(hmfr,(char *)*buffer,size)!=size){
    //data read failed
    mmioClose(hmfr,0); delete[]*buffer; return NULL; 
  }
   
  //close file and return
  mmioClose(hmfr,0);
   
  return size;

}

void SoundManager::storeSound(DWORD index, BYTE *data, int dataSize, int copies,BOOL lowQuality)
{
    itsBuffers[index] = new LPDIRECTSOUNDBUFFER [copies];

    itsBufferCopies[index] = copies;

    DSBUFFERDESC bufferDesc;
    PCMWAVEFORMAT waveFormat;

    memset(&waveFormat,0,sizeof(PCMWAVEFORMAT));
    if (lowQuality)
    {
        waveFormat.wf.wFormatTag = WAVE_FORMAT_PCM;
        waveFormat.wf.nChannels = 1;
        waveFormat.wf.nSamplesPerSec = 22050;
        waveFormat.wf.nBlockAlign = 1;       
        waveFormat.wf.nAvgBytesPerSec = waveFormat.wf.nSamplesPerSec * waveFormat.wf.nBlockAlign;
        waveFormat.wBitsPerSample = 8;
    }
    else
    {
        waveFormat.wf.wFormatTag = WAVE_FORMAT_PCM;
        waveFormat.wf.nChannels = 2;
        waveFormat.wf.nSamplesPerSec = 44100;
        waveFormat.wf.nBlockAlign = 4;       
        waveFormat.wf.nAvgBytesPerSec = waveFormat.wf.nSamplesPerSec * waveFormat.wf.nBlockAlign;
        waveFormat.wBitsPerSample = 16;
    }
    memset(&bufferDesc,0,sizeof(DSBUFFERDESC));
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_STATIC;
    bufferDesc.dwBufferBytes = dataSize;
    bufferDesc.lpwfxFormat = (LPWAVEFORMATEX)&waveFormat;

    if (SUCCEEDED(itsDirectSoundObject->CreateSoundBuffer(&bufferDesc,itsBuffers[index],NULL)))
    {
        LPVOID w1,w2;
        DWORD l1,l2;
        LPDIRECTSOUNDBUFFER buffer = *itsBuffers[index];
        if (buffer->Lock(0,dataSize,&w1,&l1,&w2,&l2,0) == DSERR_BUFFERLOST)
        {
            buffer->Restore();
            if (FAILED(buffer->Lock(0,dataSize,&w1,&l1,&w2,&l2,0)))
            {
                Logger::error("Couldn't lock sound buffer!");
                return;
            }
        }
        CopyMemory(w1,data,l1);
        if (w2 != NULL) 
        {
            CopyMemory(w2,data+l1,l2);
        }
        if (FAILED(buffer->Unlock(w1,l1,w2,l2)))
        {
            Logger::error("Couldn't unlock sound buffer");
        }
        for (DWORD i=1;i<itsBufferCopies[index];i++)
        {
            if (FAILED(itsDirectSoundObject->DuplicateSoundBuffer(*itsBuffers[index],&(itsBuffers[index][i]))) )
            {
                Logger::error("Couldn't duplicate a sound buffer!");
            }
        }
    }
    else
    {
        Logger::error("Couldn't create buffer!");
    }
}

SoundManager::SoundManager(HWND hwnd, BOOL enable)
{
	isEnabled = FALSE;
	if (enable)
	{
		itsBufferSize = 10;
		itsBuffers = new LPDIRECTSOUNDBUFFER*[itsBufferSize];
		itsBufferCopies = new DWORD[itsBufferSize];
		itsFilenames = new char *[10];
		itsNumberOfBuffers = 0;
	
	    if (SUCCEEDED(DirectSoundCreate(NULL,&itsDirectSoundObject,NULL)))
	    {
			itsDirectSoundObject->SetCooperativeLevel(hwnd,DSSCL_NORMAL);
			isEnabled = TRUE;
		}
	}
}

int SoundManager::addSound(char *filename,DWORD copies,BOOL lowQuality)
{
	for (DWORD i=0;i<itsNumberOfBuffers;i++)
	{
		if (strcmp(filename,itsFilenames[i]) == 0)
		{
			return i;
		}
	}
	int returnValue = -1;
	if (isEnabled == FALSE)
		return 0;
    BYTE *data = NULL;
    int size = loadSound(filename,&data);
    if (size > 0)
    {
        ensureBufferSize(itsNumberOfBuffers+1);
        storeSound(itsNumberOfBuffers,data,size,copies,lowQuality);
        returnValue = itsNumberOfBuffers;
		itsFilenames[itsNumberOfBuffers] = filename;
		itsNumberOfBuffers++;
    }
    else
    {
        Logger::error("Couldn't load sound!");
    }
    delete[]data;

    return returnValue;
}

BOOL SoundManager::playSound(DWORD index, BOOL loop, BOOL force)
{
    if (!isEnabled) return TRUE;

	if ( (index < 0) || (index >= itsNumberOfBuffers) )
	{
		return FALSE;
	}

	LPDIRECTSOUNDBUFFER *bufferList = itsBuffers[index];

    DWORD copyIndex = 0;

    while (copyIndex < itsBufferCopies[index])
    {
        DWORD status;
        if (FAILED(bufferList[copyIndex]->GetStatus(&status)))
            status = DSBSTATUS_PLAYING;
        if ( !(status & DSBSTATUS_PLAYING) )
            break;
        copyIndex++;
    }
    if (copyIndex >= itsBufferCopies[index])
    {
        // Try to stop one
        if (force)
        {
            copyIndex = 0;
            while (copyIndex < itsBufferCopies[index])
            {
                if ( SUCCEEDED(bufferList[copyIndex]->Stop()) )
                {
                    if (SUCCEEDED(bufferList[copyIndex]->SetCurrentPosition(0)))
                    {
                        // successfully stopped, get out of this loop
                        break;
                    }
                }
                copyIndex++;
            }
        }
        else
        {
            return FALSE;
        }
    }

    if (copyIndex < itsBufferCopies[index])
    {
        return SUCCEEDED(bufferList[copyIndex]->Play(0,0,loop ? DSBPLAY_LOOPING : 0));
    }
    else
    {
        return FALSE;
    }
}

void SoundManager::stopSound(DWORD index)
{
	if (!isEnabled) return;

    if ((index < 0) || (index >= itsNumberOfBuffers))
	{
		return;
	}

    LPDIRECTSOUNDBUFFER *bufferList = itsBuffers[index];

    DWORD copyIndex = 0;

    while (copyIndex < itsBufferCopies[index])
    {
        if ( SUCCEEDED(bufferList[copyIndex]->Stop()) )
        {
            bufferList[copyIndex]->SetCurrentPosition(0);
        }
        copyIndex++;
    }
}

void SoundManager::stopAllSounds()
{
    for (DWORD i=0;i<itsNumberOfBuffers;i++)
    {
        stopSound(i);
    }
}

void SoundManager::release()
{
	if (!isEnabled) return;
    stopAllSounds();
    for (DWORD i=0;i<itsNumberOfBuffers;i++)
    {
        for (DWORD j=0;j<itsBufferCopies[i];j++)
        {
            itsBuffers[i][j]->Release();
        }
        delete[] itsBuffers[i];
		itsBuffers[i] = NULL;
    }
    delete []itsBuffers;
    delete []itsBufferCopies;
    itsDirectSoundObject->Release();
}