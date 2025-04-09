#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDA_Audio::Init(VOID* pCA, BOOL bSecByPass)
{
	pCAccess = pCA;
if (DEBUG + DEBUG_DAUDIO >= 1)
	Init_LogFile();


//FMOD specific
FSOUND_Init(44100, 32, 0);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cDA_Audio::bIsPlaying(int iSoundSegIndex)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//if index is less than 0, return
	if (iSoundSegIndex < 0)
		return FALSE;

BOOL bRet = FALSE;

	//use FMOD instead
		//FMOD specific
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_MIDI)
			bRet = FMUSIC_IsPlaying(pCA->pSounds[iSoundSegIndex]->pFMOD_handle);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_SAMPLE)
			bRet = FSOUND_IsPlaying(0);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_STREAM)
			bRet = FSOUND_IsPlaying(1);

return bRet;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDA_Audio::Stop_Media(int iSoundSegIndex)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//if index is less than 0, return
	if (iSoundSegIndex < 0)
		return;

	//use FMOD instead
		//FMOD specific
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_MIDI)
			FMUSIC_StopSong(pCA->pSounds[iSoundSegIndex]->pFMOD_handle);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_SAMPLE)
			FSOUND_StopSound((int)pCA->pSounds[iSoundSegIndex]->pFMOD_handle_sample);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_STREAM)
			FSOUND_Stream_Stop(pCA->pSounds[iSoundSegIndex]->pFMOD_handle_stream);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDA_Audio::Play_Primary(int iSoundSegIndex)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int ichan = 0;

	//find first free channel
	if (FSOUND_IsPlaying(0) == TRUE)
	for (int lc1 = 0; lc1 < 16; lc1++)
	{
		if (FSOUND_IsPlaying(lc1) == FALSE)
		{
			ichan = lc1;
			break;
		}
	}

	//use FMOD instead
		//FMOD specific
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_MIDI)
			FMUSIC_PlaySong(pCA->pSounds[iSoundSegIndex]->pFMOD_handle);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_SAMPLE)
			FSOUND_PlaySound(ichan, pCA->pSounds[iSoundSegIndex]->pFMOD_handle_sample);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_STREAM)
			FSOUND_Stream_Play(ichan, pCA->pSounds[iSoundSegIndex]->pFMOD_handle_stream);
}

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cDA_Audio::Determine_Standard_Volume(int iX, int iY)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iXView = pCA->pInterface->map_xview + (pCA->pGDisplay->MDRAW_NUMCOLS / 2);
	int iYView = pCA->pInterface->map_yview + (pCA->pGDisplay->MDRAW_NUMROWS / 2);

	//then calc distance
	int dx = iX - iXView;
	int dy = iY - iYView;
	//this is, intuitively, how many x screens away
	double distance_ratio = sqrt(double(dx*dx + dy*dy)) / pCA->pGDisplay->MDRAW_NUMCOLS;

	if (distance_ratio > 3)
		return 0;
	if (distance_ratio > 2)
		return 0;
	if (distance_ratio > 1)
		return 0;
	return 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDA_Audio::Play_Primary(int iSoundSegIndex, long fVolume)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//if index is less than 0, return
	if (iSoundSegIndex < 0)
		return;

	int ichan = 0;

	//find first free channel
	if (FSOUND_IsPlaying(0) == TRUE)
	for (int lc1 = 0; lc1 < 16; lc1++)
	{
		if (FSOUND_IsPlaying(lc1) == FALSE)
		{
			ichan = lc1;
			break;
		}
	}

	//use FMOD instead
		//FMOD specific
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_MIDI)
			FMUSIC_PlaySong(pCA->pSounds[iSoundSegIndex]->pFMOD_handle);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_SAMPLE)
			FSOUND_PlaySound(ichan, pCA->pSounds[iSoundSegIndex]->pFMOD_handle_sample);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_STREAM)
			FSOUND_Stream_Play(ichan, pCA->pSounds[iSoundSegIndex]->pFMOD_handle_stream);
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDA_Audio::Play_Secondary(int iSoundSegIndex)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int ichan = 16;

	//find first free channel
	if (FSOUND_IsPlaying(0) == TRUE)
	for (int lc1 = 16; lc1 < 32; lc1++)
	{
		if (FSOUND_IsPlaying(lc1) == FALSE)
		{
			ichan = lc1;
			break;
		}
	}

	//use FMOD instead
		//FMOD specific
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_MIDI)
			FMUSIC_PlaySong(pCA->pSounds[iSoundSegIndex]->pFMOD_handle);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_SAMPLE)
			FSOUND_PlaySound(ichan, pCA->pSounds[iSoundSegIndex]->pFMOD_handle_sample);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_STREAM)
			FSOUND_Stream_Play(ichan, pCA->pSounds[iSoundSegIndex]->pFMOD_handle_stream);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDA_Audio::Play_Secondary(int iSoundSegIndex, long fVolume)
{
	//if index is less than 0, return
	if (iSoundSegIndex < 0)
		return;

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//just play.....
	//pPerformance->PlaySegmentEx(pCA->pSounds[iSoundSegIndex]->pSegment, NULL, NULL, 0, 0, NULL, NULL, NULL);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_MIDI)
			FMUSIC_SetMasterVolume(pCA->pSounds[iSoundSegIndex]->pFMOD_handle, (int) fVolume * 255); 
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_SAMPLE)
			FSOUND_SetVolume((int)pCA->pSounds[iSoundSegIndex]->pFMOD_handle_sample, (int) fVolume * 255); 
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_STREAM)
			FSOUND_SetVolume((int)pCA->pSounds[iSoundSegIndex]->pFMOD_handle_stream, (int) fVolume * 255); 

	int ichan = 16;

	//find first free channel
	if (FSOUND_IsPlaying(0) == TRUE)
	for (int lc1 = 16; lc1 < 32; lc1++)
	{
		if (FSOUND_IsPlaying(lc1) == FALSE)
		{
			ichan = lc1;
			break;
		}
	}

	//use FMOD instead
		//FMOD specific
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_MIDI)
			FMUSIC_PlaySong(pCA->pSounds[iSoundSegIndex]->pFMOD_handle);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_SAMPLE)
			FSOUND_PlaySound(ichan, pCA->pSounds[iSoundSegIndex]->pFMOD_handle_sample);
		if (pCA->pSounds[iSoundSegIndex]->iFModMediaType == FMOD_MEDIA_TYPE_STREAM)
			FSOUND_Stream_Play(ichan, pCA->pSounds[iSoundSegIndex]->pFMOD_handle_stream);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDA_Audio::Exit()
{
	//FMOD specific
	FSOUND_Close();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDA_Audio::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cDA_Audio.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDA_Audio::WriteLog_Enter()
{
if (DEBUG + DEBUG_DAUDIO >= 1)
{
DWORD numbyteswritten = 0;

//write in an automatic ENTER line in the file
char etr[3];

etr[0] = 13;
etr[1] = 10;

WriteFile( hLogFile,
		   etr,
		   2,
		   &numbyteswritten,
		   NULL);
}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDA_Audio::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_DAUDIO >= 1)
{
DWORD numbyteswritten = 0;

WriteFile( hLogFile,
		   Text,
		   strlen(Text),
		   &numbyteswritten,
		   NULL);

}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
