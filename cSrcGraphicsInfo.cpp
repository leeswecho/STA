#include "classes.h"
//******************************************************************************//
//******************************************************************************//
void cSrcGraphicsInfo::Init(VOID* pCA, HANDLE hLF)
{
	pCAccess = pCA;

	cClassAccess* pCA2 = (cClassAccess*)pCA;

	hLogFile = hLF;

	//initialize
	ppFrameSets = NULL;
	iNumFrameSets = 0;
	
	//the default return rect (this can be changed)
	ImgFrameDefault.Init(0, TILE_SIZE, 0, TILE_SIZE, ISURF_OCEAN, 10);

	pTempList_First = new SRCIMGFRAME_L;
	pTempList_Last = new SRCIMGFRAME_L;

	pTempList_First->pCurr = NULL;
	pTempList_First->pPrev = NULL;
	pTempList_First->pNext = (SRCIMGFRAME_L*)pTempList_Last;
	pTempList_Last->pCurr = NULL;
	pTempList_Last->pPrev = (SRCIMGFRAME_L*)pTempList_First;
	pTempList_Last->pNext = NULL;

}
//******************************************************************************//
//******************************************************************************//
void cSrcGraphicsInfo::Write_Log(LPSTR Text)
{

if (DEBUG + DEBUG_CSRCGRAPHICS >= 1)
if (hLogFile != NULL)
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
void cSrcGraphicsInfo::WriteLog_Enter()
{
if (DEBUG + DEBUG_CSRCGRAPHICS >= 1)
if (hLogFile != NULL)
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
HANDLE cSrcGraphicsInfo::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cSrcGraphicsInfo.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	return hLogFile;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSrcGraphicsInfo::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSrcGraphicsInfo::Add_Entry(int iSet, int iDirection, int iFrameIndex, int iSurfIndex, int SrcRect_left, int SrcRect_right, int SrcRect_top, int SrcRect_bottom, int iDuration)
{

	//create a new entry 
	SRCIMGFRAME* pNewFrame = new SRCIMGFRAME;
	SRCIMGFRAME_L* pNewFrame_L = new SRCIMGFRAME_L;

	//so...kinda wrapping an iterator around the frame class
	pNewFrame_L->pCurr = pNewFrame;
	pNewFrame_L->iSetI = iSet;
	pNewFrame_L->iCycleI = iDirection;
	pNewFrame_L->iFrameI = iFrameIndex;

	//set information
	pNewFrame->Init(SrcRect_left, SrcRect_right, SrcRect_top, SrcRect_bottom, iSurfIndex, iDuration);

	//put it into the temp list
	SRCIMGFRAME_L* pNext = pTempList_Last;
	SRCIMGFRAME_L* pPrev = (SRCIMGFRAME_L*)pNext->pPrev;

	pNewFrame_L->pNext = (void*)pNext;
	pNewFrame_L->pPrev = (void*)pPrev;
	pNext->pPrev = (void*)pNewFrame_L;
	pPrev->pNext = (void*)pNewFrame_L;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//flush entries from existing structure
void cSrcGraphicsInfo::Flush_Arrays()
{
	if (ppFrameSets != NULL)
	{
		for (int c990 = 0; c990 < iNumFrameSets; c990++)
		if (ppFrameSets[c990] != NULL)
		{
			for (int c991 = 0; c991 < ppFrameSets[c990]->iNumFrameCycles; c991++)
			if (ppFrameSets[c990]->ppImgFrameCycles[c991] != NULL)
			{
				//give the lists the responsibility of killing the entries as opposed to the arrays to allow easier loading and updating
				//for (int c992 = 0; c992 < ppFrameSets[c990]->ppImgFrameCycles[c991]->iNumFrames; c992++)
				//if (ppFrameSets[c990]->ppImgFrameCycles[c991]->ppImgFrames[c992] != NULL)
				//{
				//	delete(ppFrameSets[c990]->ppImgFrameCycles[c991]->ppImgFrames[c992]);
				//}
				delete(ppFrameSets[c990]->ppImgFrameCycles[c991]->ppImgFrames);
			}
			delete(ppFrameSets[c990]->ppImgFrameCycles);
		}
		delete(ppFrameSets);
	}

	//reset pointer
	ppFrameSets = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//update the graphics info
void cSrcGraphicsInfo::Update()
{
	//first clear any old entries
	Flush_Arrays();

	//now we start the fun
	//unfortunately we have to do this in three passes, the first to determine the highest Set 
	//the second to determine the highest Cycle(direction) in each of the Sets
	//the third to determine the highest Frame in each of the Cycles

	//so, first pass
	SRCIMGFRAME_L* pTrav = (SRCIMGFRAME_L*)pTempList_First->pNext;

iNumFrameSets = -1;

	while(pTrav != pTempList_Last)
	{
		if (pTrav->iSetI > iNumFrameSets)
			iNumFrameSets = pTrav->iSetI;

		pTrav = (SRCIMGFRAME_L*)pTrav->pNext;
	}
if (iNumFrameSets == -1)
	return;

	//add one to account for the fact that the number of framesets is actually the highet set index + 1
	iNumFrameSets++;

	//now, allocate
	ppFrameSets = new SRCIMGFRAMESET*[iNumFrameSets];
	ZeroMemory(ppFrameSets, sizeof(SRCIMGFRAMESET*)*iNumFrameSets);

	//now, second pass
	pTrav = (SRCIMGFRAME_L*)pTempList_First->pNext;

	while(pTrav != pTempList_Last)
	{
		//if the set hasn't been allocated, allocate it
		if (ppFrameSets[pTrav->iSetI] == NULL)
		{
			ppFrameSets[pTrav->iSetI] = new SRCIMGFRAMESET;
			ZeroMemory(ppFrameSets[pTrav->iSetI], sizeof(SRCIMGFRAMESET) );

			//this will be the first cycle in the set, so the current highest cycle number is this number (the NUMBER to allocate is that plus one)
			ppFrameSets[pTrav->iSetI]->iNumFrameCycles = pTrav->iCycleI + 1;

		} else
			{
				if (pTrav->iCycleI + 1 > ppFrameSets[pTrav->iSetI]->iNumFrameCycles)
				{
					ppFrameSets[pTrav->iSetI]->iNumFrameCycles = pTrav->iCycleI + 1;

				}
			}

		pTrav = (SRCIMGFRAME_L*)pTrav->pNext;
	}

	//now allocate arrays for the frame cycles in all the frame sets
	for (int c995 = 0; c995 < iNumFrameSets; c995++)
	{
		ppFrameSets[c995]->ppImgFrameCycles = new SRCIMGFRAMECYCLE*[ ppFrameSets[c995]->iNumFrameCycles ];
		ZeroMemory(ppFrameSets[c995]->ppImgFrameCycles, sizeof(SRCIMGFRAMECYCLE*) * ppFrameSets[c995]->iNumFrameCycles );
	}

	//now, third pass
	pTrav = (SRCIMGFRAME_L*)pTempList_First->pNext;

	while(pTrav != pTempList_Last)
	{
		//count frame numbers(allocate if necessary)
		if (ppFrameSets[ pTrav->iSetI ]->ppImgFrameCycles[ pTrav->iCycleI ] == NULL)
		{
			ppFrameSets[ pTrav->iSetI ]->ppImgFrameCycles[ pTrav->iCycleI ] = new SRCIMGFRAMECYCLE;
			ZeroMemory(ppFrameSets[ pTrav->iSetI ]->ppImgFrameCycles[ pTrav->iCycleI ], sizeof(SRCIMGFRAMECYCLE) );

			ppFrameSets[ pTrav->iSetI ]->ppImgFrameCycles[ pTrav->iCycleI ]->iNumFrames = pTrav->iFrameI + 1;
		} else
			{
				if (pTrav->iFrameI + 1 > ppFrameSets[ pTrav->iSetI ]->ppImgFrameCycles[ pTrav->iCycleI ]->iNumFrames)
					ppFrameSets[ pTrav->iSetI ]->ppImgFrameCycles[ pTrav->iCycleI ]->iNumFrames = pTrav->iFrameI + 1;
			}

		pTrav = (SRCIMGFRAME_L*)pTrav->pNext;
	}

	//now allocate frame pointers
	for (int c996 = 0; c996 < iNumFrameSets; c996++)
	{
		for (int c997 = 0; c997 < ppFrameSets[ c996 ]->iNumFrameCycles; c997++)
		{
				ppFrameSets[c996 ]->ppImgFrameCycles[c997]->ppImgFrames = new SRCIMGFRAME*[ppFrameSets[c996 ]->ppImgFrameCycles[c997]->iNumFrames];
				ZeroMemory(ppFrameSets[c996 ]->ppImgFrameCycles[c997]->ppImgFrames, sizeof(SRCIMGFRAME*) * ppFrameSets[c996 ]->ppImgFrameCycles[c997]->iNumFrames);
		}
	}

	//now we actually have to do a FOURTH pass, this time to actually put the frame data into the structures
	pTrav = (SRCIMGFRAME_L*)pTempList_First->pNext;

	while(pTrav != pTempList_Last)
	{
		//watch for overwriting
		int iOldFrameTime = 0;
		if (ppFrameSets[ pTrav->iSetI ]->ppImgFrameCycles[ pTrav->iCycleI ]->ppImgFrames[ pTrav->iFrameI ] != NULL)
		{
			iOldFrameTime = ppFrameSets[ pTrav->iSetI ]->ppImgFrameCycles[ pTrav->iCycleI ]->ppImgFrames[ pTrav->iFrameI ]->iDuration;
		}
		//set pointers (this time it is possible simply to set the pointer
		ppFrameSets[ pTrav->iSetI ]->ppImgFrameCycles[ pTrav->iCycleI ]->ppImgFrames[ pTrav->iFrameI ] = pTrav->pCurr;
		ppFrameSets[ pTrav->iSetI ]->ppImgFrameCycles[ pTrav->iCycleI ]->iTotalDuration += pTrav->pCurr->iDuration - iOldFrameTime;

		pTrav = (SRCIMGFRAME_L*)pTrav->pNext;
	}

	//now just flush the list
	//leave the entries in the list now so that we update similarly to component type lists and stuff (07.13.2004)
	//Flush_List();

	//and that's it...

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
SRCIMGFRAME* cSrcGraphicsInfo::Get_Entry(int iSet, int iDirection, int iFrame, int* piRetCode)
{
//DEBUG
Write_Log("Get_Entry called.");
WriteLog_Enter();

	int b;
	int* pR = piRetCode;

	if (pR == NULL)
	{
		pR = &b;
	}

	*pR = 0; //normal return;

	if ( ppFrameSets == NULL)
	{
		*pR = 1; //no frame set	
		return &ImgFrameDefault;
	}
	//check to see whether index exceeds range
	if (iSet >= iNumFrameSets)  //( = because if the index equals the number of sets, it's already to high)
	{
		*pR = 2; //exceeds set range
		//if this is not the default set, try the default set before returning
		if (iSet != FRAMESET_DEFAULT)
		{
			return Get_Entry(FRAMESET_DEFAULT, iDirection, iFrame, piRetCode);
		}
		return &ImgFrameDefault;
	}

	//check for whether the entry in question is actually there
	if ( ppFrameSets[iSet] == NULL)
	{
		*pR = 3; //no set
		//if this is not the default set, try the default set before returning
		if (iSet != FRAMESET_DEFAULT)
		{
			return Get_Entry(FRAMESET_DEFAULT, iDirection, iFrame, piRetCode);
		}
		return &ImgFrameDefault;
	} else
		{	
			if (iDirection >= ppFrameSets[iSet]->iNumFrameCycles)
			{
				*pR = 4; //exceeds cycle range
				return &ImgFrameDefault;
			}

			if (ppFrameSets[iSet]->ppImgFrameCycles[iDirection] == NULL)
			{
				*pR = 5; //no cycle
				return &ImgFrameDefault;
			} else
				{
					if (iFrame >= ppFrameSets[iSet]->ppImgFrameCycles[iDirection]->iNumFrames)
					{
						*pR = 6; //exceeds frame range
						if (ppFrameSets[iSet]->ppImgFrameCycles[iDirection]->ppImgFrames[0] == NULL)
						{
							return &ImgFrameDefault;
						}	else
							{
								return ppFrameSets[iSet]->ppImgFrameCycles[iDirection]->ppImgFrames[0];
							}
						//instead of returning default frame, return FIRST frame while flagging iRetCode 6
						//so GDisplay can reset cycle counter
					}


					if (ppFrameSets[iSet]->ppImgFrameCycles[iDirection]->ppImgFrames[iFrame] == NULL)
					{
						*pR = 7;
						return &ImgFrameDefault;
					}
							else
							{
								return ppFrameSets[iSet]->ppImgFrameCycles[iDirection]->ppImgFrames[iFrame];

							}
				}
		}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
SRCIMGFRAME* cSrcGraphicsInfo::Get_Entry2(int iSet, int iDirection, int iStartTick, int* piRetCode)
{
//DEBUG
Write_Log("Get_Entry2 called.");
WriteLog_Enter();
char NText[50];

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int b;
	int* pR = piRetCode;

	SRCIMGFRAMESET* pFrameSet;
	SRCIMGFRAMECYCLE* pFrameCycle;
	SRCIMGFRAME* pImgFrame;

	if (pR == NULL)
	{
		pR = &b; //in other words, do nothing with the return value
	}

	*pR = 0; //normal return;

	if ( ppFrameSets == NULL)
	{
		*pR = 1; //no frame set	
		return &ImgFrameDefault;
	}
	//check to see whether index exceeds range
	if (iSet >= iNumFrameSets)  //( = because if the index equals the number of sets, it's already to high)
	{
		*pR = 2; //exceeds set range
		//if this is not the default set, try the default set before returning
		if (iSet != FRAMESET_DEFAULT)
		{
			return Get_Entry2(FRAMESET_DEFAULT, iDirection, iStartTick, piRetCode);
		}

		return &ImgFrameDefault;
	}

	//check for whether the entry in question is actually there
	pFrameSet = ppFrameSets[iSet];
	if ( pFrameSet == NULL)
	{
		*pR = 3; //no set
		//if this is not the default set, try the default set before returning
		if (iSet != FRAMESET_DEFAULT)
		{
			return Get_Entry2(FRAMESET_DEFAULT, iDirection, iStartTick, piRetCode);
		}

		return &ImgFrameDefault;
	} else
		{	
			if (iDirection >= pFrameSet->iNumFrameCycles)
			{
				*pR = 4; //exceeds cycle range
				return &ImgFrameDefault;
			}

			pFrameCycle = pFrameSet->ppImgFrameCycles[iDirection];	
			if (pFrameCycle == NULL)
			{
				*pR = 5; //no cycle
				return &ImgFrameDefault;
			} else
				{
					//here's where things get different
					//we first the "remainder" time -- the number of ticks that have passed since the beginning
					//of the most recent cycle of the current animation
					int iRemTime = (pCA->pTimer->iCurrTick - iStartTick) % pFrameCycle->iTotalDuration;
					
					int iCurrFrame = 0;
					int bFrames = FALSE;
					int iTimeCounter = 0;
					//and then go through the array to determine which frame this lands on
					for (int c998 = 0; c998 < pFrameCycle->iNumFrames; c998++)
					{
						pImgFrame = pFrameCycle->ppImgFrames[c998];
						
						if (pImgFrame != NULL)
						{

							bFrames = TRUE; //initialize
							if (iTimeCounter <= iRemTime)
								iCurrFrame = c998;

							iTimeCounter += pImgFrame->iDuration;
							
								//this loop SHOULD terminate upon hitting the frame after the correct frame
						}

					}

					if (bFrames == FALSE)
					{
						*pR = 8;
						return &ImgFrameDefault;
					}

					//the iCurrFrame holds the correct frame
					*pR = 0; //normal return
					return pFrameCycle->ppImgFrames[iCurrFrame];
										
				}
		}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

void cSrcGraphicsInfo::Flush_List()
{

	SRCIMGFRAME_L* pTrav;
	SRCIMGFRAME_L* pTrav2;

	pTrav = (SRCIMGFRAME_L*)pTempList_First->pNext;

	while(pTrav != pTempList_Last)
	{
		pTrav2 = (SRCIMGFRAME_L*)pTrav->pNext; 

		//now delete the entries themselves here
		delete(pTrav->pCurr);

		delete(pTrav);
		pTrav = pTrav2;
	}

	//relink
	pTempList_First->pNext = (void*)pTempList_Last;
	pTempList_Last->pPrev = (void*)pTempList_First;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSrcGraphicsInfo::Exit()
{
	//just flush everything
	Flush_Arrays();
	Flush_List();

	//then delete the anchors
	delete (pTempList_First);

	delete (pTempList_Last);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSrcGraphicsInfo::Setup_Graphics_DefaultA(int iDirections, int iXFrameSize, int iYFrameSize, int iSurfSource)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//sets up graphics in the first format that was used for all grpahics prior to the cSrcGraphicsInfo system(07.13.2004)

	int iX = 0;
	int iY = 0;

	for (int c971 = 0; c971 < iDirections; c971++)
	{
		iX = int(c971 / BLIT_FPCOL) * iXFrameSize;
		iY = int(c971 % BLIT_FPCOL) * iYFrameSize;

		Add_Entry(0, c971, 0, iSurfSource, iX, iX + iXFrameSize, iY, iY + iYFrameSize, 10); //that last parameter: if there's only 1 frame in the cycle, it doesn't matter what goes there! 
	}

	Update();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
