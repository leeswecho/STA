#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSoundSegment::Init(VOID* pCA, IDirectMusicPerformance8* pDM8Perf, IDirectMusicLoader8* pDM8Loader, LPSTR FName, BOOL bMIDIflag)
{
	pCAccess = pCA;
	HRESULT hr20;

//DEBUG
	cClassAccess* pCA2 = (cClassAccess*)pCA;
	const char* KText;

	FileName = FName;

	if (bMIDIflag)
	{
		pFMOD_handle = FMUSIC_LoadSong(FileName);
		iFModMediaType = FMOD_MEDIA_TYPE_MIDI;
	}
	else
	{
		HANDLE hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		int iFileSize = GetFileSize(hFile, NULL);
		CloseHandle(hFile);
		if (iFileSize < 300000)
		{
			pFMOD_handle_sample = FSOUND_Sample_Load(FSOUND_FREE, FileName, 0, 0, 0);
			iFModMediaType = FMOD_MEDIA_TYPE_SAMPLE;
		} else
		{

			pFMOD_handle_stream = FSOUND_Stream_Open(FileName, 0, 0, 0);
			iFModMediaType = FMOD_MEDIA_TYPE_STREAM;
		}
		
	}

	bInit_Run = TRUE;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSoundSegment::Exit()
{
	//release
	if (bInit_Run == TRUE)
	{
		//FMOD specific
		if (iFModMediaType == FMOD_MEDIA_TYPE_MIDI)
			FMUSIC_FreeSong(pFMOD_handle);
		if (iFModMediaType == FMOD_MEDIA_TYPE_SAMPLE)
			FSOUND_Sample_Free(pFMOD_handle_sample);
		if (iFModMediaType == FMOD_MEDIA_TYPE_STREAM)
			FSOUND_Stream_Close(pFMOD_handle_stream);
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
