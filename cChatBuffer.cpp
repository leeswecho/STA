#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//by function, this class could be added to cInterface....but this would make cInterface even more complex 07.22.2002
void cChatBuffer::Init(VOID* pCA, int iPersistTicks)
{
	pCAccess = pCA;

	iPersistTickCount = iPersistTicks;

	//allocate for anchors
	pCBL_First = new CBNODE;
	pCBL_Last = new CBNODE;

	//initialize the anchors
	pCBL_First->Text = NULL;
	pCBL_First->pNext = (void*)pCBL_Last;
	pCBL_First->pPrev = NULL;
	
	pCBL_Last->Text = NULL;
	pCBL_Last->pPrev = (void*)pCBL_First;
	pCBL_Last->pNext = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cChatBuffer::Exit()
{
	//clear everything in the buffer
	pCBL_Trav = pCBL_First;

	while (pCBL_Trav != NULL)
	{	
		pCBL_Trav2 = (CBNODE*)pCBL_Trav->pNext;
		//free the text AND the node itself
		if (pCBL_Trav->Text != NULL)
			delete(pCBL_Trav->Text);
		delete(pCBL_Trav);
		pCBL_Trav = pCBL_Trav2;

	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cChatBuffer::Clear_List()
{
	//clear everything in the buffer
	pCBL_Trav = (CBNODE*)pCBL_First->pNext;

	while (pCBL_Trav != pCBL_Last)
	{	
		pCBL_Trav2 = (CBNODE*)pCBL_Trav->pNext;
		//free the text AND the node itself
		if (pCBL_Trav->Text != NULL)
			delete(pCBL_Trav->Text);
		delete(pCBL_Trav);
		pCBL_Trav = pCBL_Trav2;

	}

	//then relink the two
	pCBL_First->pNext = (CBNODE*)pCBL_Last;
	pCBL_Last->pPrev = (CBNODE*)pCBL_First;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cChatBuffer::Add_CBNode(LPSTR Text, int iStayTime)
{
	if (strlen(Text) == 0)
		return;

	//allocate for the node itself
	CBNODE* pNodeNew = new CBNODE;
	//allocate for the text
	pNodeNew->Text = new char[strlen(Text)+1]; //add +1 for the NULL-terminator

	//deep copy the text
	memcpy(pNodeNew->Text, Text, sizeof(char)*(strlen(Text)+1));		//
	
	//set ticks remaining
	pNodeNew->iTicksRemaining = iStayTime; 

	//link into the list, just in front of first
	pCBL_Trav = (CBNODE*)pCBL_First->pNext;

	pNodeNew->pPrev = (void*)pCBL_First;
	pNodeNew->pNext = (void*)pCBL_Trav;
	pCBL_Trav->pPrev = (void*)pNodeNew;
	pCBL_First->pNext = (void*)pNodeNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cChatBuffer::Remove_CBNode(CBNODE* pNode)
{

//de-link itself from buffer 
//uses different traversers because it is called from within Process_ChatBuffer, which also usses traversers
	pCBL_Trav3 = (CBNODE*)pNode->pPrev;
	pCBL_Trav4 = (CBNODE*)pNode->pNext;

	if (pCBL_Trav3 != NULL)
		pCBL_Trav3->pNext = (void*)pCBL_Trav4;
	if (pCBL_Trav4  != NULL)
		pCBL_Trav4->pPrev = (void*)pCBL_Trav3;

	//delete the buffer text and the buffer itself
	delete(pNode->Text);
	delete(pNode);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cChatBuffer::Process_ChatBuffer() //process the chatbuffer
{
	//go through buffer, decrement counts, remove expired nodes
	pCBL_Trav = (CBNODE*)pCBL_First->pNext;

	while(pCBL_Trav != pCBL_Last)
	{
	//decrement tick count
		pCBL_Trav->iTicksRemaining--;
		pCBL_Trav2 = pCBL_Trav;
	//increment pointer
		pCBL_Trav = (CBNODE*)pCBL_Trav->pNext;
	//check for expired message
		if (pCBL_Trav2->iTicksRemaining <= 0)
			Remove_CBNode(pCBL_Trav2);
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAckTable::Init(void* pCA)
{
	pCAccess = pCA;
	cClassAccess* pCA2 = (cClassAccess*)pCA;
	
	iNumAcks =  (pCA2->pUTypeTable->iHighestTypeID + 101)*3*3*NUM_ACK_TYPES;
	Table = new cAcknowledgement[iNumAcks];


	for (int lc1 = 0; lc1 < iNumAcks; lc1++)
	{
		Table[lc1].iSndSeg = -1;
		Table[lc1].AckText = NULL;
	}

	/*
		Table[0].iSndSeg = SOUNDSEG_ACK0;
		strcat(Table[0].AckText, "Understood.");

		Table[1].iSndSeg = SOUNDSEG_ACK1;
		strcat(Table[1].AckText, "Roger!");
		
		Table[2].iSndSeg = SOUNDSEG_ACK2;
		strcat(Table[2].AckText, "Aye!");


		Table[10].iSndSeg = SOUNDSEG_ACK10;
		strcat(Table[10].AckText, "Yes?");

		Table[11].iSndSeg = SOUNDSEG_ACK11;
		strcat(Table[11].AckText, "What is it?");
		
		Table[12].iSndSeg = SOUNDSEG_ACK12;
		strcat(Table[12].AckText, "Standing By.");
	*/

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAckTable::Exit()
{
	for (int lc1 = 0; lc1 < iNumAcks; lc1++)
	{
		if (Table[lc1].AckText != NULL)
			delete [] Table[lc1].AckText;
		Table[lc1].AckText = NULL;
	}

	delete [] Table;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cAckTable::Get_Ack(BOOL bStruct, int iObjectType, int iAckType, int iPersonality, int iCycleIndex)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;



	//returns the TABLE index. Not the sound itself )in the case you ever desire acknowledgement text
	int iTableIndex = 0;
	
	if (bStruct == TRUE)
	{
		// objtypeindex * NUM_ACK_TYPES * 3 personalities * 3 sounds per cycle + iCycleIndex
		iTableIndex = (iObjectType+1)*3*3* NUM_ACK_TYPES + iAckType *3*3 + iPersonality * 3  + iCycleIndex;
	} else
	{
		// objtypeindex(start at index above 100) * NUM_ACK_TYPES * 3 personalities * 3 sounds per cycle + iCycleIndex
		iTableIndex = (iObjectType+100)*3*3*NUM_ACK_TYPES + iAckType *3*3 + iPersonality * 3  + iCycleIndex;
	}

	if (iAckType == ACK_TYPE_MENTAT_CONSTRUCTION_COMPLETE)
	{
		iTableIndex = iAckType *3*3 + iPersonality * 3  + iCycleIndex;
	}
	if (iAckType == ACK_TYPE_MENTAT_GAME_START)
	{
		iTableIndex = iAckType *3*3 + iPersonality * 3  + iCycleIndex;
	}


	return iTableIndex;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAckTable::Set_Ack(BOOL bStruct, int iObjectType, int iAckType, int iPersonality, int iCycleIndex, int iSoundSegToSetTo, char* AckText, int iAckTextLength)
{
	int iTableIndex = 0;
	if (bStruct == TRUE)
	{
		// objtypeindex * NUM_ACK_TYPES * 3 personalities * 3 sounds per cycle + iCycleIndex
		iTableIndex = (iObjectType+1)*3*3* NUM_ACK_TYPES + iAckType *3*3 + iPersonality * 3 +  iCycleIndex;
	} else
	{
		// objtypeindex(start at index above 100) * NUM_ACK_TYPES * 3 personalities * 3 sounds per cycle + iCycleIndex
		iTableIndex = (iObjectType+100)*3*3*NUM_ACK_TYPES + iAckType *3*3 + iPersonality * 3  + iCycleIndex;
	}

	if (iAckType == ACK_TYPE_MENTAT_CONSTRUCTION_COMPLETE)
	{
		iTableIndex = iAckType *3*3 + iPersonality * 3  + iCycleIndex;
	}
	if (iAckType == ACK_TYPE_MENTAT_GAME_START)
	{
		iTableIndex = iAckType *3*3 + iPersonality * 3  + iCycleIndex;
	}

	Table[iTableIndex].iSndSeg = iSoundSegToSetTo;
	Table[iTableIndex].AckText = new char[iAckTextLength+2];
	ZeroMemory(Table[iTableIndex].AckText, sizeof(char)*(iAckTextLength+2));
	strcat(Table[iTableIndex].AckText, AckText);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

int cAckTable::Supply_Random_Ack()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//NOTE: the reason why these use rand() instead of rand2() is because the only one that heres and sees the acknowledgements
	//is the controlling player(acknowledgements are not sent over the commqueue). Consequently they aren't synched with any other players, and will
	//mess up the synchronized generator if we use rand2 here ...   06.04.2005

	int iret = rand() % 10; //pCA->pRandGen->rand2() % 10;

	while (Table[iret].iSndSeg < 0)
	{
		iret = rand() % 10; //pCA->pRandGen->rand2() % 10;
	}

	return iret;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cAckTable::Supply_Random_Ack2()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	//NOTE: the reason why these use rand() instead of rand2() is because the only one that heres and sees the acknowledgements
	//is the controlling player(acknowledgements are not sent over the commqueue). Consequently they aren't synched with any other players, and will
	//mess up the synchronized generator if we use rand2 here ...   06.04.2005

	int iret = 10 + rand() % 10; //pCA->pRandGen->rand2() % 10;

	while (Table[iret].iSndSeg < 0)
	{
		iret = 10 + rand() % 10; //pCA->pRandGen->rand2() % 10;
	}

	return iret;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
