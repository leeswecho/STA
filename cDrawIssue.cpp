#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDrawIssueBuffer::Init()
{

	//allocate and link
	pDIB_First = new cDrawIssue;
	pDIB_Last = new cDrawIssue;

	pDIB_First->pPrev = NULL;
	pDIB_First->pNext = (void*)pDIB_Last;
	pDIB_Last->pPrev = (void*)pDIB_First;
	pDIB_Last->pNext = NULL;



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDrawIssueBuffer::Exit()
{

	//clear and free
	Clear_List();

	//free anchors
	delete(pDIB_Last);
	delete(pDIB_First);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cDrawIssue* cDrawIssueBuffer::Create_Add_Node(RECT DestRect,
								RECT SourceRect,
								int iSourceIndex,
								int iSourceType,
								DWORD dwColorKey)
{
	//create node
	cDrawIssue* pNodeNew = new cDrawIssue;

	//set data
	pNodeNew->DestRect = DestRect;
	pNodeNew->dwColorKey = dwColorKey;
	pNodeNew->iSourceIndex = iSourceIndex;
	pNodeNew->iSourceType = iSourceType;
	pNodeNew->SourceRect = SourceRect;


	Add_Node(pNodeNew, FALSE);

	return pNodeNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDrawIssueBuffer::Add_Node(cDrawIssue* pNodeNew, BOOL bCopyFlag)
{

	cDrawIssue* pNew = pNodeNew;


	if (bCopyFlag == TRUE)
	{
		//deep copy
		pNew = new cDrawIssue;

		pNew->DestRect = pNodeNew->DestRect;
		pNew->dwColorKey = pNodeNew->dwColorKey;
		pNew->iSourceIndex = pNodeNew->iSourceIndex;
		pNew->iSourceType = pNodeNew->iSourceType;
		pNew->SourceRect = pNodeNew->SourceRect;

	}


	//set links
	cDrawIssue* pNode_Next = pDIB_Last;
	cDrawIssue* pNode_Prev = (cDrawIssue*)pDIB_Last->pPrev;

	pNew->pNext = (void*)pNode_Next;
	pNew->pPrev = (void*)pNode_Prev;
	pNode_Next->pPrev = (void*)pNew;
	pNode_Prev->pNext = (void*)pNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDrawIssueBuffer::Delete_Node(cDrawIssue* pNode)
{
	//set for relink
	cDrawIssue* pNext = (cDrawIssue*)pNode->pNext;
	cDrawIssue* pPrev = (cDrawIssue*)pNode->pPrev;

	//link without old
	pNext->pPrev = (void*)pPrev;
	pPrev->pNext = (void*)pNext;

	//kill
	delete(pNode);




}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDrawIssueBuffer::Clear_List()
{
	//WAY over due for a templated list class(09.26.2003)
	cDrawIssue* pTrav = (cDrawIssue*)pDIB_First->pNext;
	cDrawIssue* pTrav2;

	while (pTrav != pDIB_Last)
	{
		pTrav2 = (cDrawIssue*)pTrav->pNext;

		delete(pTrav);

		pTrav = pTrav2;
	}

	//relink the anchors
	pDIB_First->pNext = (void*)pDIB_Last;
	pDIB_Last->pPrev = (void*)pDIB_First;



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
