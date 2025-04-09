#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAnimFrame::Init(RECT Rect, int iIS, int iBT, int iOVC)
{
	
	SourceRect = Rect;
	iImageSurf = iIS;
	iBlittype = iBT;
	iOverlayColor = iOVC;

	pPrev = NULL;
	pNext = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAnimType::Init()
{
	
	//allocate and link
	pAF_First = new(cAnimFrame);
	pAF_Last = new(cAnimFrame);

	pAF_First->pPrev = NULL;
	pAF_First->pNext = (void*)pAF_Last;
	pAF_Last->pNext = NULL;
	pAF_Last->pPrev = (void*)pAF_First;

	//init
	pTable = NULL;

	iType = VAL_INVALID; //not yet in a buffer yet
	iNumFrames = 0;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAnimType::Add_Frame(RECT Rect, int iIS, int iBlittype, int iOverlayColor)
{

	cAnimFrame* pNext = pAF_Last;
	cAnimFrame* pPrev = (cAnimFrame*)pAF_Last->pPrev;

	//allocate and link
	cAnimFrame* pAF_New = new(cAnimFrame);

	//set
	pAF_New->Init(Rect, iIS, iBlittype, iOverlayColor);
		
	pAF_New->pNext = (void*)pNext;
	pAF_New->pPrev = (void*)pPrev;
	pNext->pPrev = (void*)pAF_New;
	pPrev->pNext = (void*)pAF_New;

	//increment the number of frames
	iNumFrames++;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAnimType::Flush_Buffer()
{
	cAnimFrame* pTrav = (cAnimFrame*)pAF_First->pNext;
	cAnimFrame* pTrav2;

	while (pTrav != pAF_Last)
	{
		pTrav2 = pTrav;
		
		pTrav = (cAnimFrame*)pTrav->pNext;

		delete(pTrav2);
	}

	//re-link anchors
	pAF_First->pNext = (void*)pAF_Last;
	pAF_Last->pPrev = (void*)pAF_First;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAnimType::Exit()
{
	//flush the buffer
	Flush_Buffer();

	//delete anchors
	delete(pAF_First);
	delete(pAF_Last);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAnimInfoTable::Init(void* pCA)
{
	pCAccess = pCA;

if (DEBUG + DEBUG_ANIMINFOTABLE >= 1)
	Init_LogFile();


//DEBUG
Write_Log("Setting up Anchors...");
WriteLog_Enter();

	//allocate and link
	pAT_First = new (cAnimType);
	pAT_Last = new(cAnimType);

	//set
	pAT_First->iType = VAL_INVALID;
	pAT_Last->iType = VAL_INVALID;

	//link
	pAT_First->pPrev = NULL;
	pAT_First->pNext = (void*)pAT_Last;
	pAT_Last->pNext = NULL;
	pAT_Last->pPrev = (void*)pAT_First;

	//initialize table
	pTable = NULL;

//DEBUG
Write_Log("Addin empty types...");
WriteLog_Enter();


//add a test animation (01.18.2004)
	Add_EmptyType(ANIMTYPE_SPARKLE01);
	Add_EmptyType(ANIMTYPE_TRAIL01);
	Add_EmptyType(ANIMTYPE_EXPL01);
	Add_EmptyType(ANIMTYPE_EXPL02);
	Add_EmptyType(ANIMTYPE_UEXPL01);
	Add_EmptyType(ANIMTYPE_MSLTRAIL1);
	Add_EmptyType(ANIMTYPE_BLAST);


//DEBUG
Write_Log("Reconstructing Table...");
WriteLog_Enter();


//reconstruct the table
	Reconstruct();


//DEBUG
Write_Log("Start Adding Frames...");
WriteLog_Enter();


//now begin adding frames
/////////////////

//DEBUG
Write_Log(" ANIMTYPE_SPARKLE01");
WriteLog_Enter();
	cAnimType* pType = pTable[ANIMTYPE_SPARKLE01];

RECT AddRect;
for (int lc1 = 0; lc1 < 5; lc1++)
{
	AddRect.top = lc1 * 8;
	AddRect.bottom = AddRect.top + 8;
	AddRect.left = 0; 
	AddRect.right = 8;

//DEBUG
Write_Log("     Frame");
WriteLog_Enter();

	pType->Add_Frame(AddRect, ISURF_ANIM_TEST01, BQU_BLITTYPE_NORMAL, 0);

}
//DEBUG
Write_Log("     Reconstructing....");
WriteLog_Enter();

	pType->Reconstruct();
///////////////



////////////////

//DEBUG
Write_Log(" ANIMTYPE_TRAIL01");
WriteLog_Enter();
	pType = pTable[ANIMTYPE_TRAIL01];


int iFrameBaseSize = 24;
int lc1 = 0;
for (lc1 = 0; lc1 < 2; lc1++)
{
	AddRect.top = lc1 * iFrameBaseSize;
	AddRect.bottom = AddRect.top + iFrameBaseSize;
	AddRect.left = 0; 
	AddRect.right = iFrameBaseSize;

	pType->Add_Frame(AddRect, ISURF_ANIM_TEST02, BQU_BLITTYPE_ALPHA, 0);
}
	pType->Reconstruct();
///////////////////

////////////////

//DEBUG
Write_Log(" ANIMTYPE_EXPL01");
WriteLog_Enter();
	pType = pTable[ANIMTYPE_EXPL01];

iFrameBaseSize = 24;
for (lc1 = 0; lc1 < 2; lc1++)
{
	AddRect.top = lc1 * iFrameBaseSize;
	AddRect.bottom = AddRect.top + iFrameBaseSize;
	AddRect.left = 0; 
	AddRect.right = iFrameBaseSize;

	pType->Add_Frame(AddRect, ISURF_ANIM_TEST03, BQU_BLITTYPE_ALPHA, 0);
}
	pType->Reconstruct();
///////////////////

////////////////

//DEBUG
Write_Log(" ANIMTYPE_EXPL02");
WriteLog_Enter();
	pType = pTable[ANIMTYPE_EXPL02];

iFrameBaseSize = 24;
for (lc1 = 0; lc1 < 2; lc1++)
{
	AddRect.top = lc1 * iFrameBaseSize;
	AddRect.bottom = AddRect.top + iFrameBaseSize;
	AddRect.left = 0; 
	AddRect.right = iFrameBaseSize;

	pType->Add_Frame(AddRect, ISURF_ANIM_TEST04, BQU_BLITTYPE_ALPHA, 0);
}
	pType->Reconstruct();
///////////////////


////////////////

//DEBUG
Write_Log(" ANIMTYPE_UEXPL01");
WriteLog_Enter();
	pType = pTable[ANIMTYPE_UEXPL01];

iFrameBaseSize = 36;
for (lc1 = 0; lc1 < 10; lc1++)
{
	AddRect.top = lc1 * iFrameBaseSize;
	AddRect.bottom = AddRect.top + iFrameBaseSize;
	AddRect.left = 0; 
	AddRect.right = iFrameBaseSize;

	pType->Add_Frame(AddRect, ISURF_ANIM_UEXPL01, BQU_BLITTYPE_ALPHA, 0);
}
	pType->Reconstruct();
///////////////////

////////////////

//DEBUG
Write_Log(" ANIMTYPE_MSLTRAIL1");
WriteLog_Enter();
	pType = pTable[ANIMTYPE_MSLTRAIL1];

iFrameBaseSize = 12;
for (lc1 = 0; lc1 < 3; lc1++)
{
	AddRect.top = lc1 * iFrameBaseSize;
	AddRect.bottom = AddRect.top + iFrameBaseSize;
	AddRect.left = 0; 
	AddRect.right = iFrameBaseSize;

	pType->Add_Frame(AddRect, ISURF_ANIM_MSLTRAIL1, BQU_BLITTYPE_NORMAL, 0);
}
	pType->Reconstruct();
///////////////////
//DEBUG
Write_Log(" ANIMTYPE_BLAST");
WriteLog_Enter();
	pType = pTable[ANIMTYPE_BLAST];

	//frame1
	AddRect.left = 0;
	AddRect.right = 124;
	AddRect.top = 0;
	AddRect.bottom = 124;
	pType->Add_Frame(AddRect, ISURF_ANIM_BLAST, BQU_BLITTYPE_ALPHA, 0);
	//frame2
	AddRect.left = 0;
	AddRect.right = 80;
	AddRect.top = 125;
	AddRect.bottom = 210;
	pType->Add_Frame(AddRect, ISURF_ANIM_BLAST, BQU_BLITTYPE_ALPHA, 0);
	//frame3
	AddRect.left = 0;
	AddRect.right = 67;
	AddRect.top = 211;
	AddRect.bottom = 280;
	pType->Add_Frame(AddRect, ISURF_ANIM_BLAST, BQU_BLITTYPE_ALPHA, 0);
	//frame4
	AddRect.left = 0;
	AddRect.right = 67;
	AddRect.top = 281;
	AddRect.bottom = 350;
	pType->Add_Frame(AddRect, ISURF_ANIM_BLAST, BQU_BLITTYPE_ALPHA, 0);
	//frame5
	AddRect.left = 0;
	AddRect.right = 67;
	AddRect.top = 351;
	AddRect.bottom = 420;
	pType->Add_Frame(AddRect, ISURF_ANIM_BLAST, BQU_BLITTYPE_ALPHA, 0);
	//frame6
	AddRect.left = 0;
	AddRect.right = 67;
	AddRect.top = 421;
	AddRect.bottom = 490;
	pType->Add_Frame(AddRect, ISURF_ANIM_BLAST, BQU_BLITTYPE_ALPHA, 0);
	pType->Reconstruct();
//DEBUG
Write_Log("Initialization Complete");
WriteLog_Enter();
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAnimInfoTable::Add_EmptyType(int iType)
{

	//add to the list

	//allocate
	cAnimType* pAT_New = new (cAnimType);

	cAnimType* pNext = pAT_Last;
	cAnimType* pPrev = (cAnimType*)pAT_Last->pPrev;

	//set
	pAT_New->Init();
	pAT_New->iType = iType;

	//link
	pAT_New->pNext = (void*)pNext;
	pAT_New->pPrev = (void*)pPrev;

	pNext->pPrev = (void*)pAT_New;
	pPrev->pNext = (void*)pAT_New;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAnimType::Reconstruct()
{
	//set up the table using information from the list

	//delete any previous table
	if (pTable != NULL)
		delete(pTable);

	pTable = new cAnimFrame*[iNumFrames];

	//traverse 
	cAnimFrame* pAF_Trav = (cAnimFrame*)pAF_First->pNext;

int lc1 = 0;

	while (pAF_Trav != pAF_Last)
	{
		pTable[lc1] = pAF_Trav;
		lc1++;

		pAF_Trav = (cAnimFrame*)pAF_Trav->pNext;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAnimInfoTable::Reconstruct()
{
//DEBUG
Write_Log("[Rcnst] Deallocating");
WriteLog_Enter();

	if (pTable != NULL)
		delete(pTable);

int iHighestIndex = 1;

//DEBUG
Write_Log("[Rcnst] Traversing to get highest index");
WriteLog_Enter();

	//traverse 
	cAnimType* pAT_Trav = (cAnimType*)pAT_First->pNext;

	while (pAT_Trav != pAT_Last)
	{
		if (pAT_Trav->iType > iHighestIndex)
			iHighestIndex = pAT_Trav->iType;

		pAT_Trav = (cAnimType*)pAT_Trav->pNext;
	}
//DEBUG
Write_Log("[Rcnst] Allocating");
WriteLog_Enter();

//allocate table
pTable = new cAnimType*[iHighestIndex + 2]; //should be 1, +2 to be safe

//DEBUG
Write_Log("[Rcnst] Zeroing memory");
WriteLog_Enter();

	ZeroMemory(pTable, sizeof(cAnimType*) * (iHighestIndex + 2) );


//DEBUG
Write_Log("[Rcnst] Resetting values....");
WriteLog_Enter();

	//no go back and reset
	pAT_Trav = (cAnimType*)pAT_First->pNext;
	
	while (pAT_Trav != pAT_Last)
	{
		pTable[pAT_Trav->iType] = pAT_Trav;

		pAT_Trav = (cAnimType*)pAT_Trav->pNext;
	}

//DEBUG
Write_Log("[Rcnst] Complete");
WriteLog_Enter();
	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAnimInfoTable::Exit()
{
	//clear the list 
	cAnimType* pAT_Trav = (cAnimType*)pAT_First->pNext;
	cAnimType* pAT_Trav2;

	while (pAT_Trav != pAT_Last)
	{
		pAT_Trav2= pAT_Trav;

		pAT_Trav = (cAnimType*)pAT_Trav->pNext;

		pAT_Trav2->Exit();
		delete(pAT_Trav2);
	}

	//delete the anchors
	delete(pAT_First);
	delete(pAT_Last);

	//delete the table
	if (pTable != NULL)
		delete(pTable);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAnimInfoTable::Write_Log(LPSTR Text)
{

if (DEBUG + DEBUG_ANIMINFOTABLE >= 1)
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
void cAnimInfoTable::WriteLog_Enter()
{
if (DEBUG + DEBUG_ANIMINFOTABLE >= 1)
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
void cAnimInfoTable::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cAnimInfoTable.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAnimInfoTable::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cEvent::Init_Simple(void* pCA, int iAnimType, LONG lAnimSTick, int _iSoundSegIndex, int _iVolume, int iZLev, int iAnimX, int iAnimY)
{
		pCAccess = pCA;

		cClassAccess* pCA2 = (cClassAccess*)pCA;

		//set up the animation structure
		pAnimation = new (cAnimation);

		pAnimation->Init();
		pAnimation->iType = iAnimType;
		pAnimation->iZLevel = iZLev;
		pAnimation->iX = iAnimX;
		pAnimation->iY = iAnimY;

		iSoundSegIndex = _iSoundSegIndex;
		iVolume = _iVolume;
		bSoundPlayed = FALSE;

		lAnimStartTick = lAnimSTick;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cEvent::Exit()
{

	//delete the animation
	delete(pAnimation);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cEvent::Calc_Completion()
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	BOOL bRet = FALSE;

	if (pAnimation->iType >= 0)
	if (pAnimation->iCurrFrame > pCA->pAnimInfoTable->pTable[pAnimation->iType]->iNumFrames - 1 )
			bRet = TRUE;

	if (pAnimation->iType < 0)
	if (bSoundPlayed == TRUE)
			bRet = TRUE;

	return bRet;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cEvent::Calc_Execute()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	BOOL bRet = FALSE;

		if (pCA->pTimer->iCurrTick >= lAnimStartTick)
			bRet = TRUE;

	return bRet;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cEventBuffer::Init(VOID* pCA)
{
	pCAccess = pCA;

if (DEBUG + DEBUG_EVENTBUFFER >= 1)
	Init_LogFile();

	//allocate
	pEV_First = new(cEvent);
	pEV_Last = new(cEvent);

	//link
	pEV_First->pPrev = NULL;
	pEV_First->pNext = (void*)pEV_Last;
	pEV_Last->pPrev = (void*)pEV_First;
	pEV_Last->pNext = NULL;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cEventBuffer::Add_Event(int iAnimType, LONG lAnimTick, int iSoundSegIndex, int iVolume, int iZLevel, int iAnimX, int iAnimY)
{
	//create a new event
	cEvent* pEV_New = new (cEvent);

	//initialize
	pEV_New->Init_Simple(pCAccess, iAnimType, lAnimTick, iSoundSegIndex, iVolume, iZLevel, iAnimX, iAnimY);

	//link
	cEvent* pNext = pEV_Last;
	cEvent* pPrev = (cEvent*)pEV_Last->pPrev;

	pEV_New->pNext = (void*)pNext;
	pEV_New->pPrev = (void*)pPrev;
	pNext->pPrev = (void*)pEV_New;
	pPrev->pNext = (void*)pEV_New;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cEventBuffer::Flush_Buffer()
{
	cEvent* pEV_Trav = (cEvent*)pEV_First->pNext;
	cEvent* pEV_Trav2;

	while (pEV_Trav != pEV_Last)
	{
		pEV_Trav2 = pEV_Trav;

		pEV_Trav = (cEvent*)pEV_Trav->pNext;

		pEV_Trav2->Exit();
		delete(pEV_Trav2);
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cEventBuffer::Exit()
{

	Flush_Buffer();

	//delete anchors
	delete(pEV_First);
	delete(pEV_Last);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cEventBuffer::Execute_Event(cEvent* pEvent)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
Write_Log("[Execute Event] Calculating Execute...");
WriteLog_Enter();
	//return 0 to flag "do not destroy me yet"
	if (pEvent->Calc_Execute() != TRUE)
		return 0;

//DEBUG
Write_Log("[Execute Event] Calculating Destroy...");
WriteLog_Enter();
	//flag destroy if it's time is up
	if (pEvent->Calc_Completion() == TRUE)
		return 1;

//DEBUG
Write_Log("[Execute Event] Doing it's thing...");
WriteLog_Enter();
	//now do it's thing
	//draw it's animation

//DEBUG
Write_Log("[Process Buffer] Getting Info...");
WriteLog_Enter();

		//play sound, if we 
		if (pEvent->iSoundSegIndex >= 0)
		if (pEvent->bSoundPlayed == FALSE)
		{
			//technically "play" it even, if we don't actually play it due to the number of sounds going on
			pEvent->bSoundPlayed = TRUE;

			if (iSoundsPlayed <= TEMP_SIMULSOUNDS_MAX)
			{
				pCA->pDA_Audio->Play_Primary(pEvent->iSoundSegIndex, pEvent->iVolume);
				iSoundsPlayed++;
			}
		}

		if (pEvent->pAnimation->iType >= 0)
		{
			//draw the current frame
			cAnimType* pAnimInfo = pCA->pAnimInfoTable->pTable[pEvent->pAnimation->iType];

//DEBUG
		char NText[50];
Write_Log("[Process Buffer] Accessing Frame: ");
//DEBUG
itoa(pEvent->pAnimation->iCurrFrame, NText, 10);
Write_Log(NText);
WriteLog_Enter();

			//use the blitqueue to blit
			cAnimFrame* pAnimFrame = pAnimInfo->pTable[pEvent->pAnimation->iCurrFrame];


			//get width and height
			int width = pAnimFrame->SourceRect.right - pAnimFrame->SourceRect.left;
			int height = pAnimFrame->SourceRect.bottom - pAnimFrame->SourceRect.top;

			int iX = pEvent->pAnimation->iX - width/2;
			int iY = pEvent->pAnimation->iY - height/2;

			//compensate for offset system
			iX += TILE_SIZE / 2;
			iY += TILE_SIZE / 2;
	
			//subtract screen pos offsets
			iX -= pCA->pInterface->map_xview * TILE_SIZE;
			iY -= pCA->pInterface->map_yview * TILE_SIZE;
		
//DEBUG
Write_Log("[Process Buffer] Submitting Blit request ISurf: ");
itoa(pAnimFrame->iImageSurf, NText, 10);
Write_Log(NText);
WriteLog_Enter();

			//only draw if it is within the map view window
			if (iX >= 0 && iY >= 0 && iX < pCA->pDD_Display->i_XResD * MDRAW_XSCR && iY < pCA->pDD_Display->i_YResD * MDRAW_YSCR)
				pCA->pGDisplay->UBlitQueue.Submit_BlitRequest(pEvent->pAnimation->iZLevel, iX, iY, pCA->pImages[pAnimFrame->iImageSurf]->pDDSImage,
															  &pAnimFrame->SourceRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT, pAnimFrame->iBlittype, pAnimFrame->iOverlayColor, pCA->pImages[pAnimFrame->iImageSurf]->pExBuffer);

	//DEBUG
	Write_Log("[Process Buffer] Finishing up...");
	WriteLog_Enter();
			//now increment the frame
			pEvent->pAnimation->iCurrFrame = (pCA->pTimer->iCurrTick - pEvent->lAnimStartTick) / 5;
		}

	return 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cEventBuffer::Process_Buffer()
{
	cEvent* pEV_Trav = (cEvent*)pEV_First->pNext;
	cEvent* pEV_Trav2;

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//for deletion
	cEvent* pNext;
	cEvent* pPrev;

		iSoundsPlayed--;
		if (iSoundsPlayed < 0)
			iSoundsPlayed = 0;

	while (pEV_Trav != pEV_Last)
	{
		pEV_Trav2 = pEV_Trav;
//DEBUG
char NText[20];
Write_Log("[Process Buffer] Executing Events");
WriteLog_Enter();
		int iRet = Execute_Event(pEV_Trav);
//DEBUG
Write_Log("[Process Buffer] Executing Events Complete.");
WriteLog_Enter();
		//increment
		pEV_Trav = (cEvent*)pEV_Trav->pNext;

		//now delete if necessary
		if (iRet == 1)
		{
//DEBUG
Write_Log("[Process Buffer] Deleting Event...");
WriteLog_Enter();
			pNext = (cEvent*)pEV_Trav2->pNext;
			pPrev = (cEvent*)pEV_Trav2->pPrev;
//DEBUG
Write_Log("[Process Buffer] Exiting...");
WriteLog_Enter();
			pEV_Trav2->Exit();
		
			delete(pEV_Trav2);

			pNext->pPrev = (void*)pPrev;
			pPrev->pNext = (void*)pNext;
		}
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cEventBuffer::Write_Log(LPSTR Text)
{

if (DEBUG + DEBUG_EVENTBUFFER >= 1)
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
void cEventBuffer::WriteLog_Enter()
{
if (DEBUG + DEBUG_EVENTBUFFER >= 1)
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
void cEventBuffer::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cEventBuffer.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cEventBuffer::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////


