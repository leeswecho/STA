#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cNotifBuffer::Init(VOID* pCA)
{

	//Init_LogFile();

	//allocate and link
	pNB_First = new cNotifNode;
	pNB_Last = new cNotifNode;

	pNB_First->pPrev = NULL;
	pNB_First->pNext = (void*)pNB_Last;
	pNB_Last->pPrev = (void*)pNB_First;
	pNB_Last->pNext = NULL;

	pNB_First->Text = NULL;
	pNB_Last->Text = NULL;
	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cNotifBuffer::Exit()
{
//
	cNotifNode* pNBT = (cNotifNode*)pNB_First->pNext;
	cNotifNode* pNBT2;

	while (pNBT != pNB_Last)
	{
		pNBT2 = (cNotifNode*)pNBT->pNext;
//this causes problems when one uses text literals that are the same for multiple nodes.
//the compiler links them all to the same text and so it crashes here when it tries to delete them "individually

		if (pNBT->Text != NULL && pNBT->bFreeTextFlag == TRUE)
			delete(pNBT->Text);

		delete(pNBT);
		pNBT = pNBT2;
	}

	delete(pNB_First);
	delete(pNB_Last);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cNotifBuffer::Create_Add_Node(LPSTR NewText, DWORD dwActiveColor, DWORD dwPassiveColor, BOOL bFreeTextFlag)
{
	//bFreeTextFlag indicates whether the notification buffer should be responsible for freeing the passed pointer when it is done
	//TRUE = yes, Notifbuffer needs to free this string

	cNotifNode* pNB_New = new cNotifNode;
	cNotifNode* pNB_Next = (cNotifNode*)pNB_First->pNext;

	//set and link
	pNB_New->Text = NewText;
	pNB_New->dwActiveColor = dwActiveColor;
	pNB_New->dwPassiveColor = dwPassiveColor;
	pNB_New->bFreeTextFlag = bFreeTextFlag;
	pNB_New->pPrev = (void*)pNB_First;
	pNB_New->pNext = (void*)pNB_Next;

	pNB_First->pNext = (void*)pNB_New;
	pNB_Next->pPrev = (void*)pNB_New;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cNotifBuffer::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cNotifBuffer.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cNotifBuffer::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_NOTIFBUFFER >= 1)
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

void cNotifBuffer::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

void cNotifBuffer::WriteLog_Enter()
{
if (DEBUG + DEBUG_NOTIFBUFFER >= 1)
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
