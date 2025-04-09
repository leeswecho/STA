#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cClassAccess::Init(cDD_Display* pDD,
			cSectorTable* pSCT, 
			cDI_Input* pDI,
			cDP_Multiplay* pDP,
			cDA_Audio* pDA,
			cImageSurface* pIM, 
			cSoundSegment* pSS,
			cGDisplay* pGD, 
			cInputState* pIS, 
			cInterface* pIF, 
			cGameWorldA* pGW, 
			cCommQueue* pCQ, 
			cAPPDATA* pAD, 
			cTimer* pTM, 
			cUnitListA* pUL,
			cUnitTableA* pUT, 
			cProjListA* pPL,
			cPathFinder* pPF,
			cUnitTT* pUTT,
			cProjTT* pPTT,
			cGameOptions* pGO,
			cRandGen* pRG,
			cChatBuffer* pCB,
			cMinimap* pMM,
			cColorTable* pCT,
			cCmpInfoTable* pCIT,
			cFontInfoTable* pFIT,
			cCmpOffsetTable* pCOT,
			cStructListA* pSLA,
			cStructIDTable* pSIDT,
			cStructInfoTable* pSIT,
			cCmpIDTable* pCIDT,
			cCmdDataReqTable* pCDRT,
			cInputPacketBuffer* pIPB,
			cCmdInfoTable* pCMIT,
			cNationManager* pNM,
			cNotifBuffer* pNB,
			cAnimInfoTable* pAIT,
			cEventBuffer* pEB,
			cDbgMem* pDM,
			cStringTable* pSRT,
			cConfig* pCFG,
			cAckTable* pAckT,
			cMaterialInfoTable* pMIT)
{

	pDD_Display = pDD;
	//pD3D_Display = pD3D;
	pDI_Input = pDI;
	pDP_Multiplay = pDP;
	pDA_Audio = pDA;
	pGDisplay = pGD;
	pInterface = pIF;
	pGameWorld = pGW;
	pCommQueue = pCQ;
	pAppData = pAD;
	pInputState = pIS;
	pTimer = pTM;
	pUnitList = pUL;
	pUnitTable = pUT;
	pProjList = pPL;
	pPathFinder = pPF;
	pGameOptions = pGO;
	pRandGen = pRG;
	pChatBuffer = pCB;
	pMinimap = pMM;
	pColorTable = pCT;
	pCmpInfoTable = pCIT; 
	pFontInfoTable = pFIT;
	pPTypeTable = pPTT;
	pUTypeTable = pUTT;
	pCmpOffsetTable = pCOT;
	pStructList = pSLA;
	pStructIDTable = pSIDT;
	pStructInfoTable = pSIT;
	pCmpIDTable = pCIDT;
	pCmdReqTable = pCDRT;
	pInputPacketBuffer = pIPB;
	pCmdInfoTable = pCMIT;
	pNationManager = pNM;
	pNotifBuffer = pNB;
	pAnimInfoTable = pAIT;
	pEventBuffer = pEB;
	pDbgMem = pDM;
	pStringTable = pSRT;
	pSectorTable = pSCT;
	pConfig = pCFG;
	pAckTable = pAckT;
	pMaterialInfoTable = pMIT;

	for (c1 = 0; c1 <= NUM_MAX_ISURFACES - 1; c1++)
	{
		pImages[c1] = (cImageSurface*)pIM + c1;
	}
	for (c1 = 0; c1 <= NUM_SOUNDSEGS - 1; c1++)
		pSounds[c1] = (cSoundSegment*)pSS + c1;



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
