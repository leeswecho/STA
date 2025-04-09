#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//08.09.2003
void cNationManager::MainInit(VOID* pCA, int iMaxNations)
{
	int lc1; //local counter
	pCAccess = pCA;

	ppNations = new cNation*[iMaxNations];

	iNumNations = iMaxNations;

	for (lc1 = 0; lc1 < iMaxNations; lc1++)
	{
		ppNations[lc1] = NULL;

	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cNationManager::Init_Nation(int iID, LPSTR pName, int iNameLength, BOOL bAIFlag, BOOL bHumanPlayable)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	ppNations[iID] = new cNation;
	
	ppNations[iID]->ddResources = 10000;  //HACK (TEMP)
	ppNations[iID]->ddPossession = 0;
	ppNations[iID]->iCapitalID = -1;
	ppNations[iID]->iID = iID;
	ppNations[iID]->bAIFlag = bAIFlag;
	ppNations[iID]->bHumanPlayable = bHumanPlayable;
	ppNations[iID]->iNumStructures = 0;
	ppNations[iID]->iNumUnits = 0;

	ZeroMemory(ppNations[iID]->Name, sizeof(char) * 80);
	memcpy(ppNations[iID]->Name, pName, sizeof(char) * iNameLength);

	

//DEBUG
pCA->pInterface->Write_Log("[Nation Manager]Init Check: ");
pCA->pInterface->Write_Log(ppNations[iID]->Name);
pCA->pInterface->WriteLog_Enter();

//set the log file name
char LogFileName[80];
ZeroMemory(LogFileName, sizeof(char)* 80);

strcat(LogFileName, pName);//, sizeof(char) * iNameLength);
strcat(LogFileName, ".txt");


	ppNations[iID]->NationalInventory.Init_B(pCAccess, LogFileName, pCA->pCmpInfoTable->iHighestTypeID + 1);
	ppNations[iID]->AINew.Init(pCA, ppNations[iID]->NationalInventory.hLogFile, iID);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cNationManager::Exit()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

int lc1; //local counter
	
	for (lc1 = 0; lc1 < iNumNations; lc1++)
	{
		//DEBUG
		char NText[50];
		pCA->pDD_Display->Write_Log("Starting Nation Exit ");
		itoa(lc1, NText, 10);
		pCA->pDD_Display->Write_Log(NText);
		pCA->pDD_Display->WriteLog_Enter();

		if (ppNations[lc1] != NULL)
		{
			ppNations[lc1]->NationalInventory.Exit_B();

		}

		//DEBUG
		pCA->pDD_Display->Write_Log("- Finished Nation Exit");
		pCA->pDD_Display->WriteLog_Enter();

	}
	//we have to exit the Nations in two loops because the AI exits will be firing off events that all of the AINews must be online to process
	for (int lc2 =0 ; lc2 < iNumNations; lc2++)
	{

		if (ppNations[lc2] != NULL)
		{

				//note that for now AINew Exit must come after AI Exit because AI Exit also exits the Seed Groups (which will fire off AINew Events that AINew must still be up to process)
			ppNations[lc2]->AINew.Exit();

			delete(ppNations[lc2]);
		}
	}
		//DEBUG
		pCA->pDD_Display->Write_Log("Deleting Nation Array Pointer");
		pCA->pDD_Display->WriteLog_Enter();


	delete(ppNations);

		//DEBUG
		pCA->pDD_Display->Write_Log("Function Done");
		pCA->pDD_Display->WriteLog_Enter();


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cNationManager::Deduct_SupportCosts()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cUnitA* pUL_Trav = (cUnitA*)pCA->pUnitList->pUL_First->pNext_UL;

	while (pUL_Trav != pCA->pUnitList->pUL_Last)
	{
		if (ppNations[pUL_Trav->iControl] != NULL)
		{
			ppNations[pUL_Trav->iControl]->ddResources -= pUL_Trav->ddSupportCost;
		}

		pUL_Trav = (cUnitA*)pUL_Trav->pNext_UL;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
