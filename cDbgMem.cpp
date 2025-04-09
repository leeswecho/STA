#include "classes.h"
//********************************************************************************************//
//********************************************************************************************//
void cDbgMem::Write_Log_H(LPSTR Text, HANDLE hLog)
{
	
DWORD numbyteswritten = 0;

WriteFile( hLog,
		   Text,
		   strlen(Text),
		   &numbyteswritten,
		   NULL);

}
//********************************************************************************************//
//********************************************************************************************//
void cDbgMem::WriteLog_Enter_H(HANDLE hLog)
{

DWORD numbyteswritten = 0;

//write in an automatic ENTER line in the file
char etr[3];

etr[0] = 13;
etr[1] = 10;

WriteFile( hLog,
		   etr,
		   2,
		   &numbyteswritten,
		   NULL);
}
//********************************************************************************************//
//********************************************************************************************//
//Memory Checking Functions
void cDbgMem::MemCheck(void* ptr, HANDLE hLog)
{
	char NText[20];

/*

_CrtMemBlockHeader* pHead = pHdr(ptr);

//DEBUG
Write_Log_H(" ****Beginning Memory Status Dump****", hLog);
WriteLog_Enter_H(hLog);
Write_Log_H(" Pointer: ", hLog);
ultoa((int)ptr, NText, 10);
Write_Log_H(NText, hLog);
WriteLog_Enter_H(hLog);


	Write_Log_H(" *Mem Block Data: ", hLog);
	WriteLog_Enter_H(hLog);

	Write_Log_H(" pBlockHeaderNext: ", hLog);
	ultoa((unsigned int)pHead->pBlockHeaderNext, NText, 10);
	Write_Log_H(NText, hLog);
	WriteLog_Enter_H(hLog);
	Write_Log_H(" pBlockHeaderPrev: ", hLog);
	ultoa((unsigned int)pHead->pBlockHeaderPrev, NText, 10);
	Write_Log_H(NText, hLog);
	WriteLog_Enter_H(hLog);	
	
	Write_Log_H(" nLine: ", hLog);
	ultoa(pHead->nLine, NText, 10);
	Write_Log_H(NText, hLog);
	WriteLog_Enter_H(hLog);
	
	Write_Log_H(" Datasize: ", hLog);
	ultoa(pHead->nDataSize, NText, 10);
	Write_Log_H(NText, hLog);
	WriteLog_Enter_H(hLog);

	Write_Log_H(" nBlockUse: ", hLog);
	ultoa(pHead->nBlockUse, NText, 10);
	Write_Log_H(NText, hLog);
	WriteLog_Enter_H(hLog);

	Write_Log_H(" lRequest: ", hLog);
	ultoa(pHead->lRequest, NText, 10);
	Write_Log_H(NText, hLog);
	WriteLog_Enter_H(hLog);

	Write_Log_H(" Gap: ", hLog);

for (int c89 = 0; c89 < nNoMansLandSize; c89++)
{
	ultoa(pHead->gap[c89], NText, 10);
	Write_Log_H(NText, hLog);
	Write_Log_H(" ", hLog);
}
	WriteLog_Enter_H(hLog);

//HACK: force data size to 1, type to 1, see what happens
//if (pHead->nDataSize == 1202590843)
//{
//	pHead->nDataSize = 20;
//	pHead->nBlockUse = 1;
//}

Write_Log_H(" *Data: ", hLog);
unsigned int* piTrav = (unsigned int*)ptr;

if (pHead->nBlockUse == 1)
for (int c88 = 0; c88 < pHead->nDataSize / sizeof(unsigned int); c88++)
{
	ultoa(*piTrav, NText, 10);
	Write_Log_H(NText, hLog);
	Write_Log_H(" ", hLog);
	piTrav++;
}	

BYTE* pbTrav = (BYTE*)piTrav;
Write_Log_H(" EndGap: ", hLog);
for (int c90 = 0; c90 < nNoMansLandSize; c90++)
{
	ultoa(*pbTrav, NText, 10);
	Write_Log_H(NText, hLog);
	Write_Log_H(" ", hLog);
	pbTrav++;
}
	
	WriteLog_Enter_H(hLog);
*/

}
//********************************************************************************************//
//********************************************************************************************//
