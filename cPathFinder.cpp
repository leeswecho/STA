#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Init(VOID* pCA, float ASHeuristic)
{

	//start the logfile
if (DEBUG + DEBUG_PATHFINDER >= 1)
	Init_LogFile();

	//store the ASHeuristic
	A_Star_Heuristic = ASHeuristic;
	//set the maximum number of paths to execute per tick
	MaxIterations_Tick = 1200; //temp test

	//store the class access pointer
	pCAccess = pCA;
	cClassAccess* pCA2 = (cClassAccess*)pCAccess;

	//initialize the list anchors
	pOL_First = new cPLNODE;
	pOL_Last = new cPLNODE;
	pCL_First = new cPLNODE;
	pCL_Last = new cPLNODE;
	pTL_First = new cPLNODE;
	pTL_Last = new cPLNODE;

	pMOL_First = new MTILE;
	pMOL_Last = new MTILE;
	pMCL_First = new MTILE;
	pMCL_Last = new MTILE;
	pMTL_First = new MTILE;
	pMTL_Last = new MTILE;

	pSecSchemeOL_First = new cSector;
		pSecSchemeOL_First->pPrev = NULL;
		pSecSchemeOL_First->pNext = pSecSchemeOL_Last;
	pSecSchemeOL_Last = new cSector;
		pSecSchemeOL_Last->pPrev = pSecSchemeOL_First;
		pSecSchemeOL_Last->pNext = NULL;
	pSecSchemeCL_First = new cSector;
		pSecSchemeCL_First->pPrev = NULL;
		pSecSchemeCL_First->pNext = pSecSchemeCL_Last;
	pSecSchemeCL_Last = new cSector;
		pSecSchemeCL_Last->pPrev = pSecSchemeCL_First;
		pSecSchemeCL_Last->pNext = NULL;


	//link the first and last to each other
	pOL_First->pNext = (VOID*)pOL_Last;
	pOL_First->pPrev = NULL;
	pOL_Last->pPrev = (VOID*)pOL_First;
	pOL_Last->pNext = NULL;

	pCL_First->pNext = (VOID*)pCL_Last;
	pCL_First->pPrev = NULL;
	pCL_Last->pPrev = (VOID*)pCL_First;
	pCL_Last->pNext = NULL;

	pTL_First->pNext = (VOID*)pTL_Last;
	pTL_First->pPrev = NULL;
	pTL_Last->pPrev = (VOID*)pTL_First;
	pTL_Last->pNext = NULL;

	pMOL_First->pNext = pMOL_Last;
	pMOL_Last->pPrev = pMOL_First;
	pMCL_First->pNext = pMCL_Last;
	pMCL_Last->pPrev = pMCL_First;
	pMTL_First->pNext = pMTL_Last;
	pMTL_Last->pPrev = pMTL_First;

//TEST to ensure that we know these nodes are anchor nodes
	pOL_First->fEvalSum = -6996;
	pOL_Last->fEvalSum = -6996;

	//initialize simple pather functions
	pS_First = new cSimpleNode;
	pS_Last = new cSimpleNode;
	iJobNumber = 1;  //initialize the simple job number

	pS_First->pPrev = NULL;
	pS_First->pNext = pS_Last;
	pS_Last->pPrev = pS_First;
	pS_Last->pNext = NULL;
	


}
//////////**************************************************************************B************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::PostInit()
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;


	//no longer used 2007.02.16
	//allocate an array of bytes the size of the map to speed up pathfinding
	int iSize = (pCA->pGameWorld->iWidth * pCA->pGameWorld->iHeight);
	pPathState = new cPLNODE*[iSize + 100]; //double pointer, because it is pointing to pointers

	//make sure all the pointers are NULL	//memset(pPathState, iSize, 0);
	Clear_PathArray();


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cPathFinder::Adjust_Target2(int iIndex_Start, int iIDUnit)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iRad = 10;
	int iMapWidth = pCA->pGameWorld->iWidth;
	SQCOORDSET sqStart;
	sqStart.index = iIndex_Start;
	sqStart.Calc_xy(iMapWidth);

	cUnitA* pUnit = pCA->pUnitTable->pUnitTable[iIDUnit];

	if (pUnit == NULL)
		return -1;	 

	cCmpInfo* pFrameInf = pCA->pCmpInfoTable->pCmpTable[pUnit->pFrame->iType];

	for (int lc1 = 0; lc1 < iRad; lc1++)
	{
		int iXHigh = sqStart.x + lc1;
		int iXLow = sqStart.x - lc1;
		int iYHigh = sqStart.y + lc1;
		int iYLow = sqStart.y - lc1;
		if (iXLow < 0)
			iXLow = 0;
		if (iYLow < 0)
			iYLow = 0;
		if (iYHigh >= pCA->pGameWorld->iHeight)
			iYHigh = pCA->pGameWorld->iHeight - 1;
		if (iXHigh >= pCA->pGameWorld->iWidth)
			iXHigh = pCA->pGameWorld->iWidth - 1;
		
		for (int lcx = iXLow; lcx <= iXHigh; lcx++)
		for (int lcy = iYLow; lcy <= iYHigh; lcy++)
		{		
			MTILE* pTile = &pCA->pGameWorld->pMap[lcy*iMapWidth + lcx];
			if (pFrameInf->pFrameInfo->MoveCharacteristics[pTile->iType] > 0)
				if (pTile->pOccupant_Unit == NULL)
				if (pTile->pOccupant_Structure == NULL)
					return (lcy*iMapWidth + lcx);
		}	
	}
	//if we got here we're in trouble 
	pCA->pChatBuffer->Add_CBNode("Houston, we've got a problem", 1000);
	return iIndex_Start;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Process_OpenNode_Link(MTILE* pOpenNode)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//do this so I don't have to run references a billion times
	int iWidth = pCA->pGameWorld->iWidth;
	int iNode = pOpenNode->Pos.index;
	int iGoal = Pos_goal.index;

	UP = iNode - iWidth;
	DOWN = iNode + iWidth;
	LEFT = iNode -1;
	RIGHT = iNode + 1;
	UPRIGHT = iNode - iWidth + 1;
	UPLEFT = iNode - iWidth - 1;
	DOWNLEFT = iNode + iWidth - 1;
	DOWNRIGHT = iNode + iWidth + 1;

	//spawn templist nodes adjacent to it if possible
	//by convention, do the orthagonal moves first(actually I think this is to make 
	
	//eval them all first(03.28.2003)
	BOOL Results[4];
	//			  	0 
	//			  3 U 1
	//			    2 

	Results[0] = Eval_NewNodeSpawn_Link(UP, pOpenNode);
	Results[1] = Eval_NewNodeSpawn_Link(RIGHT, pOpenNode);
	Results[2] = Eval_NewNodeSpawn_Link(DOWN, pOpenNode);
	Results[3] = Eval_NewNodeSpawn_Link(LEFT, pOpenNode);

	//UP  (if it passes the test, spawn a temp node
	if (Results[0])
		Insert_TempNode2(pOpenNode, UP);
	//DOWN
	if (Results[2])
		Insert_TempNode2(pOpenNode, DOWN);
	//LEFT
	if (Results[3])
		Insert_TempNode2(pOpenNode, LEFT);
	//RIGHT
	if (Results[1])
		Insert_TempNode2(pOpenNode, RIGHT);

	//now that thats done, move the open node from the open list to the closed list
	//set traversers
	MTILE* pMOL_Trav = pOpenNode->pPrev;
	MTILE* pMOL_Trav2 = pOpenNode->pNext;

	//unlink from the open list
	pMOL_Trav->pNext = pMOL_Trav2;
	pMOL_Trav2->pPrev = pMOL_Trav;

	MTILE* pMCL_Trav;
	MTILE* pMCL_Trav2;
	//check the first address at the top of the closed list...
	pMCL_Trav = pMCL_First->pNext;
	
	//if it's the LAST node or the evalsum on this node isn't better than the one currently there, move it to the back
	if ((pMCL_Trav == pMCL_Last) || (pOpenNode->fEval >= pMCL_Trav->fEval))
		pMCL_Trav = pMCL_Last;

	//perform insertion depending on what pCL_Trav is(it will always be the node AFTER where the former open node will go
	pMCL_Trav2 = pMCL_Trav->pPrev;

	//perform the linkup
	pOpenNode->pNext = pMCL_Trav;
	pOpenNode->pPrev = pMCL_Trav2;
	pMCL_Trav2->pNext = pOpenNode;
	pMCL_Trav->pPrev = pOpenNode;

	//okay, that's done

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Process_OpenNode2(MTILE* pOpenNode)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//do this so I don't have to run references a billion times
	int iWidth = pCA->pGameWorld->iWidth;
	int iNode = pOpenNode->Pos.index;
	int iGoal = Pos_goal.index;

	UP = iNode - iWidth;
	DOWN = iNode + iWidth;
	LEFT = iNode -1;
	RIGHT = iNode + 1;
	UPRIGHT = iNode - iWidth + 1;
	UPLEFT = iNode - iWidth - 1;
	DOWNLEFT = iNode + iWidth - 1;
	DOWNRIGHT = iNode + iWidth + 1;

	//spawn templist nodes adjacent to it if possible
	//by convention, do the orthagonal moves first(actually I think this is to make 
	
	//eval them all first(03.28.2003)
	BOOL Results[8];
	//			  7	0 1
	//			  6 U 2
	//			  5 4 3

	Results[0] = Eval_NewNodeSpawn2(UP, pOpenNode);
	Results[1] = Eval_NewNodeSpawn2(UPRIGHT, pOpenNode);
	Results[2] = Eval_NewNodeSpawn2(RIGHT, pOpenNode);
	Results[3] = Eval_NewNodeSpawn2(DOWNRIGHT, pOpenNode);
	Results[4] = Eval_NewNodeSpawn2(DOWN, pOpenNode);
	Results[5] = Eval_NewNodeSpawn2(DOWNLEFT, pOpenNode);
	Results[6] = Eval_NewNodeSpawn2(LEFT, pOpenNode);
	Results[7] = Eval_NewNodeSpawn2(UPLEFT, pOpenNode);

	//UP  (if it passes the test, spawn a temp node
	if (Results[0])
		Insert_TempNode2(pOpenNode, UP);
	//DOWN
	if (Results[4])
		Insert_TempNode2(pOpenNode, DOWN);
	//LEFT
	if (Results[6])
		Insert_TempNode2(pOpenNode, LEFT);
	//RIGHT
	if (Results[2])
		Insert_TempNode2(pOpenNode, RIGHT);

	//diagonal directions	(note that the pathfinder currently blocks against "squeezing" corners)
	//UPLEFT
	if (Results[6] && Results[7] && Results[0])
		Insert_TempNode2(pOpenNode, UPLEFT);
	//UPRIGHT
	if (Results[0] && Results[1] && Results[2])
		Insert_TempNode2(pOpenNode, UPRIGHT);
	//DOWNLEFT
	if (Results[4] && Results[5] && Results[6])
		Insert_TempNode2(pOpenNode, DOWNLEFT);
	//DOWNRIGHT
	if (Results[2] && Results[3] && Results[4])
		Insert_TempNode2(pOpenNode, DOWNRIGHT);

	//now that thats done, move the open node from the open list to the closed list
	//set traversers
	MTILE* pMOL_Trav = pOpenNode->pPrev;
	MTILE* pMOL_Trav2 = pOpenNode->pNext;

	//unlink from the open list
	pMOL_Trav->pNext = pMOL_Trav2;
	pMOL_Trav2->pPrev = pMOL_Trav;

	MTILE* pMCL_Trav;
	MTILE* pMCL_Trav2;
	//check the first address at the top of the closed list...
	pMCL_Trav = pMCL_First->pNext;
	
	//if it's the LAST node or the evalsum on this node isn't better than the one currently there, move it to the back
	if ((pMCL_Trav == pMCL_Last) || (pOpenNode->fEval >= pMCL_Trav->fEval))
		pMCL_Trav = pMCL_Last;

	//perform insertion depending on what pCL_Trav is(it will always be the node AFTER where the former open node will go
	pMCL_Trav2 = pMCL_Trav->pPrev;

	//perform the linkup
	pOpenNode->pNext = pMCL_Trav;
	pOpenNode->pPrev = pMCL_Trav2;
	pMCL_Trav2->pNext = pOpenNode;
	pMCL_Trav->pPrev = pOpenNode;

	//okay, that's done

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Clear_Lists_All2()
{
	pMOL_First->pNext = pMOL_Last;
	pMOL_Last->pPrev = pMOL_First;
	pMCL_First->pNext = pMCL_Last;
	pMCL_Last->pPrev = pMCL_First;
	pMTL_First->pNext = pMTL_Last;
	pMTL_Last->pPrev = pMTL_First;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cPathFinder::Insert_TempNode_Link(MTILE* pParent, int iIndex_node)
{
	//inserts a node into the templist, already sorted(so that the pathfinder only needs to pass through the templist
	//once to correctly sort-list the nodes into the open list
	//this is the LINK version

	//procedure here....
	//calculate the desirability of the current space from the target
	//calculate the dist from home(just an increment)
	//create the node
	//fill with appropriate values
	//sort it into the list

	//get classaccess
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iWidth = pCA->pGameWorld->iWidth;

	//return value
	BOOL bReturn;

	//find maptile
	MTILE* pNew = &pMap[iIndex_node];
	//set parent and that this tile has been "touched"(considered) by its job number
	pNew->pParent = pParent;
	pNew->iLastParticipatedJobNumber = iJobNumber;

		double diffx = (Pos_goal.x - pNew->Pos.x);
		double diffy = (Pos_goal.y - pNew->Pos.y);
		double ddsum2 = sqrt( diffx*diffx + diffy*diffy);
		pNew->fDistFromTarget = ddsum2;
	
	//if this happens to be the first node, the parent will be NULL, so set up a special case here:
	if (pParent  != NULL)	
	{
			//diagonals not allowed for links, further, traversability not an issue either, so not considered here
			pNew->fDistFromHome = pParent->fDistFromHome + 1.0;  //don't try to increment the pParent value...it changes the pParent value!
	}
			else
		pNew->fDistFromHome = 0;   //since it would be the first node

	//get the evalsum
	pNew->fEval = Calc_Evalsum(pNew->fDistFromHome, pNew->Pos, Pos_goal);

		//now traverse the temp list and sort...
		//set the traverser to the first node in the temp list(after the anchor)
	MTILE* pMTrav = pMTL_First->pNext;
	MTILE* pMTrav2;

		//forward traverse until the traverser reaches the one the new node is immediately before
		while (pNew->fEval >= pMTrav->fEval && pMTrav != pMTL_Last)
			pMTrav = pMTrav->pNext;

		//loop should have stopped on a node whose evalsum is higher than that of the new node or the LAST node
		//this is the NEXT pointer
		//set the second traverser to the PREVIOUS pointer
		pMTrav2 = pMTrav->pPrev;

		//now link all the nodes together
		pMTrav2->pNext = pNew;
		pMTrav->pPrev = pNew;
		pNew->pNext = pMTrav;
		pNew->pPrev = pMTrav2;


	
/*  //code to perform diagnostic on pathfinding 
WriteLog_Enter();
char text01[50];
itoa(pPL_New->Pos_node.x, text01, 10);
Write_Log("Temp Node Inserted at x: ");
Write_Log(text01);
Write_Log(" y: ");
itoa(pPL_New->Pos_node.y, text01, 10);
Write_Log(text01);
_gcvt(pPL_New->fEvalSum, 7, text01);
Write_Log(" with evalsum ");
Write_Log(text01);
WriteLog_Enter();
 */

		//signify that the operation was a success
		bReturn = TRUE;

	return bReturn;
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cPathFinder::Insert_TempNode2(MTILE* pParent, int iIndex_node)
{
	//inserts a node into the templist, already sorted(so that the pathfinder only needs to pass through the templist
	//once to correctly sort-list the nodes into the open list

	//procedure here....
	//calculate the desirability of the current space from the target
	//calculate the dist from home(just an increment)
	//create the node
	//fill with appropriate values
	//sort it into the list

	//get classaccess
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iWidth = pCA->pGameWorld->iWidth;

	//return value
	BOOL bReturn;

	//find maptile
	MTILE* pNew = &pMap[iIndex_node];
	//set parent and that this tile has been "touched"(considered) by its job number
	pNew->pParent = pParent;
	pNew->iLastParticipatedJobNumber = iJobNumber;

		double diffx = (Pos_goal.x - pNew->Pos.x);
		double diffy = (Pos_goal.y - pNew->Pos.y);
		double ddsum2 = sqrt( diffx*diffx + diffy*diffy);
		pNew->fDistFromTarget = ddsum2;
	
	//extended 03.21.2003 to take into account terrain traversability differences in the pathing
	//if this happens to be the first node, the parent will be NULL, so set up a special case here:
	if (pParent  != NULL)	
	{
		int sum = pParent->Pos.x - pNew->Pos.x + pParent->Pos.y - pNew->Pos.y;
		//if the parent differs diagonally, at 1.4 instead of 1
		if (sum == 2 || sum == -2 || sum == 0)
			pNew->fDistFromHome = pParent->fDistFromHome + (1.4 / PassCharacteristics[pNew->iType]);  //don't try to increment the pParent value...it changes the pParent value!
				else
			pNew->fDistFromHome = pParent->fDistFromHome + (1.0 / PassCharacteristics[pNew->iType]);  //don't try to increment the pParent value...it changes the pParent value!
	}
			else
		pNew->fDistFromHome = 0;   //since it would be the first node

	//get the evalsum
	pNew->fEval = Calc_Evalsum(pNew->fDistFromHome, pNew->Pos, Pos_goal);

		//now traverse the temp list and sort...
		//set the traverser to the first node in the temp list(after the anchor)
	MTILE* pMTrav = pMTL_First->pNext;
	MTILE* pMTrav2;

		//forward traverse until the traverser reaches the one the new node is immediately before
		while (pNew->fEval >= pMTrav->fEval && pMTrav != pMTL_Last)
			pMTrav = pMTrav->pNext;

		//loop should have stopped on a node whose evalsum is higher than that of the new node or the LAST node
		//this is the NEXT pointer
		//set the second traverser to the PREVIOUS pointer
		pMTrav2 = pMTrav->pPrev;

		//now link all the nodes together
		pMTrav2->pNext = pNew;
		pMTrav->pPrev = pNew;
		pNew->pNext = pMTrav;
		pNew->pPrev = pMTrav2;


	
/*  //code to perform diagnostic on pathfinding 
WriteLog_Enter();
char text01[50];
itoa(pPL_New->Pos_node.x, text01, 10);
Write_Log("Temp Node Inserted at x: ");
Write_Log(text01);
Write_Log(" y: ");
itoa(pPL_New->Pos_node.y, text01, 10);
Write_Log(text01);
_gcvt(pPL_New->fEvalSum, 7, text01);
Write_Log(" with evalsum ");
Write_Log(text01);
WriteLog_Enter();
 */

		//signify that the operation was a success
		bReturn = TRUE;

	return bReturn;
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cPathFinder::Eval_NewNodeSpawn_Link(int iIndex_new, MTILE* pParent)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	SQCOORDSET Pos_new;
	BOOL bReturn = TRUE;

	Pos_new.index = iIndex_new;
	Pos_new.Calc_xy(pCA->pGameWorld->iWidth);
	//see how much we can optimize this function because it is called extensively and critically during pathfinding
	MTILE* pTile = &pMap[Pos_new.index];

	//with the coordinates of the new node, check to see if it is a viable new node
	//check to see if the index is actually on the map
	if (Pos_new.index < 0 || Pos_new.index >= pCA->pGameWorld->iHeight * pCA->pGameWorld->iWidth)
		return FALSE;//bReturn = FALSE;

	//check to see if the move is actually truly adjacent(like if it has wrapped back to the other side of the map
	int xdiff = Pos_new.x - pParent->Pos.x;
	int ydiff = Pos_new.y - pParent->Pos.y;

	if (abs(xdiff) > 1 || abs(ydiff) > 1)
		return FALSE;//bReturn = FALSE;

	//check to see if there is already a node on the pathing list here
	if (pTile->iLastParticipatedJobNumber == iJobNumber)
		return FALSE;

	return bReturn;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cPathFinder::Eval_NewNodeSpawn2(int iIndex_new, MTILE* pParent)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	SQCOORDSET Pos_new;
	BOOL bReturn = TRUE;

	Pos_new.index = iIndex_new;
	Pos_new.Calc_xy(pCA->pGameWorld->iWidth);
	//see how much we can optimize this function because it is called extensively and critically during pathfinding
	MTILE* pTile = &pMap[Pos_new.index];

	//with the coordinates of the new node, check to see if it is a viable new node
	//check to see if the index is actually on the map
	if (Pos_new.index < 0 || Pos_new.index >= pCA->pGameWorld->iHeight * pCA->pGameWorld->iWidth)
		return FALSE;//bReturn = FALSE;

	//check to see if it is occupied or unreachable
	//actually, currently it doesn't matter....I guess
	//I can't implement the unreach based on terrain yet(since I don't really have any unit types yet), and the current map arrangement can 
	//support unit stacking
	//(01.31.2002)
	//TEST(01.31.2002) just to check if the pather can successfully avoid certain types of terrain
//	if (pMTile->iType == ISURF_OCEAN)
//		return FALSE;//bReturn = FALSE;

	//03.21.2003(extended to unit-specific terrain mapping)
	if (PassCharacteristics[pTile->iType] < 0)
		return FALSE;

	//temp...disqualifies nodes if ANY type of unit is found, not just enemy ones, the completely correct procedure
	//would involve searching the point of pOccupant_Unit(06.05.2002)
	if (bConsiderUnits)
	if (pTile->pOccupant_Unit != NULL)
		return FALSE; //bReturn = FALSE;
	if (pTile->pOccupant_Structure != NULL && pParent->pOccupant_Structure == NULL)
		return FALSE;

	//check to see if the move is actually truly adjacent(like if it has wrapped back to the other side of the map
	int xdiff = Pos_new.x - pParent->Pos.x;
	int ydiff = Pos_new.y - pParent->Pos.y;

	if (abs(xdiff) > 1 || abs(ydiff) > 1)
		return FALSE;//bReturn = FALSE;

	//check to see if there is already a node on the pathing list here
	if (pTile->iLastParticipatedJobNumber == iJobNumber)
		return FALSE;

//not sure if this should be here 06.05.2002
//	if (bConsiderUnits)
//	if (pMTile->wIMTargeted_Count != 0)
//		return FALSE; //bReturn = FALSE;

 //bReturn = FALSE; 
//	if (pPathState[Pos_new.index]->iEvalSum < Calc_Evalsum(
//		bReturn = FALSE;     //this check will be performed at tempnode creation so that evalsum calculation only has to happen once --> 02.01.2002
							 //grr...re-enabled because I can't figure out how to make the path improvement scheme work. (02.01.2002)
//checking moved into insert tempnode (02.02.2002)	

	return bReturn;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Process_TempList_Link()
{

//	cPLNODE* pOL_New;

	//sort the temp list into the open list
	//set the two traverser. #1 to the templist #2 to the openlist
	MTILE* pMTL_Trav = pMTL_First->pNext;
	MTILE* pMOL_Trav = pMOL_First->pNext;
	MTILE* pMOL_Prev;
	MTILE* pMOL_Next;
	MTILE* pMTL_Prev;
	MTILE* pMTL_Next;

	//add all of the nodes in the temp list, one by one...
	while (pMTL_Trav != pMTL_Last)
	{	
		//traverse the open list until it finds the first open node with an evalsum equal or greater than the temp list node(or the LAST node on the open list)
		while(pMOL_Trav->fEval <= pMTL_Trav->fEval && pMOL_Trav != pMOL_Last)
			pMOL_Trav = pMOL_Trav->pNext;
			//should stop when it is on the AFTER node
		
		//I'm not exactly sure if this will turn out to be faster than simply creating a new node and inserting it like in Sam32 --> heck I"m not sure this'll work...(01.31.2002)
		//now set the apppropriate pointers so that the current node can be removed and re-linked into the new list
		pMTL_Prev = pMTL_Trav->pPrev;
		pMTL_Next = pMTL_Trav->pNext;  //careful
		pMOL_Prev = pMOL_Trav->pPrev;
		pMOL_Next = pMOL_Trav;

		//disconnect and re-seal the temp list....
		pMTL_Prev->pNext = pMTL_Next;
		pMTL_Next->pPrev = pMTL_Prev;

		//insert the temp node into the open list
		pMTL_Trav->pNext = pMOL_Next;
		pMTL_Trav->pPrev = pMOL_Prev;
		pMOL_Prev->pNext = pMTL_Trav;
		pMOL_Next->pPrev = pMTL_Trav;

		//reset pTL_Trav to the next node in the list
		pMTL_Trav = pMTL_Next;
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Process_TempList2()
{

//	cPLNODE* pOL_New;

	//sort the temp list into the open list
	//set the two traverser. #1 to the templist #2 to the openlist
	MTILE* pMTL_Trav = pMTL_First->pNext;
	MTILE* pMOL_Trav = pMOL_First->pNext;
	MTILE* pMOL_Prev;
	MTILE* pMOL_Next;
	MTILE* pMTL_Prev;
	MTILE* pMTL_Next;

	//add all of the nodes in the temp list, one by one...
	while (pMTL_Trav != pMTL_Last)
	{	
		//traverse the open list until it finds the first open node with an evalsum equal or greater than the temp list node(or the LAST node on the open list)
		while(pMOL_Trav->fEval <= pMTL_Trav->fEval && pMOL_Trav != pMOL_Last)
			pMOL_Trav = pMOL_Trav->pNext;
			//should stop when it is on the AFTER node
		
		//I'm not exactly sure if this will turn out to be faster than simply creating a new node and inserting it like in Sam32 --> heck I"m not sure this'll work...(01.31.2002)
		//now set the apppropriate pointers so that the current node can be removed and re-linked into the new list
		pMTL_Prev = pMTL_Trav->pPrev;
		pMTL_Next = pMTL_Trav->pNext;  //careful
		pMOL_Prev = pMOL_Trav->pPrev;
		pMOL_Next = pMOL_Trav;

		//disconnect and re-seal the temp list....
		pMTL_Prev->pNext = pMTL_Next;
		pMTL_Next->pPrev = pMTL_Prev;

		//insert the temp node into the open list
		pMTL_Trav->pNext = pMOL_Next;
		pMTL_Trav->pPrev = pMOL_Prev;
		pMOL_Prev->pNext = pMTL_Trav;
		pMOL_Next->pPrev = pMTL_Trav;

		//reset pTL_Trav to the next node in the list
		pMTL_Trav = pMTL_Next;
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cPATHENTRY* cPathFinder::Generate_Path2(MTILE* pNode_End)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cPATHENTRY*  pPE1; 
	cPATHENTRY*  pPE2;
	MTILE*	 pTrav;

	//set the traverser to the node thingie
	pTrav = pNode_End;

	//allocate memory for the first pathentry node
	pPE1 = new cPATHENTRY;

	//fill it with the appropriate information
	pPE1->Position.index = pTrav->Pos.index; 
	pPE1->Position.x = pTrav->Pos.x; 
	pPE1->Position.y = pTrav->Pos.y; 
	pPE1->pNext = NULL;		//this will be the LAST node in the final created list
	pPE1->pPrev = NULL;     //the node "previous" to this has not yet been created


	//now do this (START node will be the last node to meet this condition)
	while (pTrav->pParent != NULL)
	{

		//go back one "generation"
	   pTrav = pTrav->pParent;

	   //allocate pPE2
	   pPE2 = new cPATHENTRY;

		//fill it with the appropriate information
	   pPE2->Position.index = pTrav->Pos.index;
	   pPE2->Position.x = pTrav->Pos.x;
	   pPE2->Position.y = pTrav->Pos.y;
	   pPE2->pNext = (VOID*)pPE1;  
	   pPE2->pPrev = NULL;    //hasn't been allocated yet.

	   pPE1->pPrev = (VOID*)pPE2;   //set pPE1's prev entry to pPE2...

	   //"advance" pPE1 to pPE2
	   pPE1 = pPE2;

	}     //loop should break with pPE1 and pPE2 holding the start node coordinates

	//make sure the start node has prev index set to NULL
	pPE1->pPrev = NULL;

	//return either pPE1 or pPE2
	return pPE1;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cPathFinder::Calc_Distance(float xstart, float ystart, float xend, float yend)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
/*
char NText[20];
pCA->pTimer->StartCount(TIMER_COUNTER1);
*/

	float xdiff = abs(xend - xstart);
	float ydiff = abs(yend - ystart);
	double dist = sqrt( (xdiff * xdiff) + (ydiff * ydiff));


/*
pCA->pTimer->EndCount(TIMER_COUNTER1);
//DEBUG (testing the speed of function
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER1], NText, 10);
pCA->pTimer->Write_Log("   Calc_Distance C: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();
*/

	return dist;
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cPATHENTRY* cPathFinder::Find_Path2(int IDUnit, int iIndex_Start, int iIndex_Goal, int iSearchDepth, BOOL* pbDeferFlag_Ret, BOOL bUnitConsiderFlag, BOOL bExhaustFlag, double ddAllowableBackOffDistance)
{
	return Find_Path3(IDUnit, iIndex_Start, iIndex_Goal, iSearchDepth, pbDeferFlag_Ret, bUnitConsiderFlag, bExhaustFlag, ddAllowableBackOffDistance, TRUE);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cPATHENTRY*	cPathFinder::Find_Path_Link(int iIndex_Start, int iIndex_Goal, int iSearchDepth)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	pMap = pCA->pGameWorld->pMap;
	pCA->pTimer->StartCount(TIMER_PATHFIND);

	//set Pos_goal
	Pos_goal.index = iIndex_Goal;
	Pos_goal.Calc_xy(pCA->pGameWorld->iWidth);

	//calculate euclidean distance
	SQCOORDSET Pos_start;
	Pos_start.index = iIndex_Start;
	Pos_start.Calc_xy(pCA->pGameWorld->iWidth);
	double ddDist = Calc_Distance(Pos_start.x, Pos_start.y, Pos_goal.x, Pos_goal.y);

	//increment job number
	iJobNumber++;

//DEBUG
char NText[20];
//pCA->pTimer->StartCount(TIMER_COUNTER1);

	int iSize = (pCA->pGameWorld->iWidth * pCA->pGameWorld->iHeight);
	int iCurrIterationCount = 0;
	int iRunPF_Flag = 1;     //flag on whether to run the pathfinding loop
	cPLNODE* pGoal = NULL;   //contains the "goal" node, whatever that might be
	cPATHENTRY* pPE_Start = NULL;  //path entry node of the start node and continuing to the finish
	cUnitA* pUnit;
 
	//clear out all of the lists
	Clear_Lists_All2();
	//simply relink to each other

	//insert the start node into the templist
	Insert_TempNode_Link(NULL, iIndex_Start);

	//process it immediately into the openlist
	Process_TempList_Link();

	//begin the main pathfinding cycle....
	//while(no goal node found)
	//-sort the temp nodes into the open list
	//-process the node at the top of the open list --> -spawn temp nodes, -move it to closed list(do a half-sort)


	//run this loop until a condition comes to stop it 
	while (iRunPF_Flag == 1)
	{
		//check whether break conditions exist
		if (iCurrIterationCount >= iSearchDepth)
			iRunPF_Flag = 2;
		if (pMOL_First->pNext == pMOL_Last)
			iRunPF_Flag = 3;
		if (pMOL_First->pNext->Pos.index == iIndex_Goal)
			iRunPF_Flag = 4;
		if (iRunPF_Flag == 1)
		{
			//process the first node on the open list
			Process_OpenNode_Link( pMOL_First->pNext);
			//process the templist
			Process_TempList_Link();
			//increse the iterationcount
			iCurrIterationCount++;
		}
	}

	
	iFP2_Ret = iRunPF_Flag;

	//select what the goal node should be depending on what condition the loop exited with
	//nominally select it to be the top node in the open list
	MTILE* pMGoal = pMOL_First->pNext;

	//if the loop broke on running out of open nodes or ran into the iteration count limit
	if (iRunPF_Flag == 3 || iRunPF_Flag == 2 || iRunPF_Flag == 6)
		pMGoal = pMCL_First->pNext;

	//error-catch..
	if (pMGoal == pMOL_Last)
	{
		pCA->pChatBuffer->Add_CBNode("WARNING 113 ... List ran out of open nodes. Set to first node on closed list", 1000);
		pMGoal = pMCL_First->pNext;
	}

	//now, generate the path back to the goal
	pPE_Start = Generate_Path2(pMGoal);


	//return pPE_Start
	return pPE_Start;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cPATHENTRY* cPathFinder::Find_Path3(int IDUnit, int iIndex_Start, int iIndex_Goal, int iSearchDepth, BOOL* pbDeferFlag_Ret, BOOL bUnitConsiderFlag, BOOL bExhaustFlag, double ddAllowableBackOffDistance, BOOL bReturn_Path)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	pMap = pCA->pGameWorld->pMap;
	pCA->pTimer->StartCount(TIMER_PATHFIND);

	//set Pos_goal
	Pos_goal.index = iIndex_Goal;
	Pos_goal.Calc_xy(pCA->pGameWorld->iWidth);

	//calculate euclidean distance
	SQCOORDSET Pos_start;
	Pos_start.index = iIndex_Start;
	Pos_start.Calc_xy(pCA->pGameWorld->iWidth);
	double ddDist = Calc_Distance(Pos_start.x, Pos_start.y, Pos_goal.x, Pos_goal.y);

	//TEST
	//perform a movability check
/*
	if (IDUnit >= 0)
	{
		char NText[50];
		Write_Log("Start Index: ");
		itoa(iIndex_Start, NText, 10);
		Write_Log(NText);
		Write_Log(" End: ");
		itoa(iIndex_Goal, NText, 10);
		Write_Log(NText);
		BOOL bCheck = CanUnitTraverse(IDUnit, iIndex_Start, iIndex_Goal);
		if (bCheck == TRUE)
			Write_Log(" YES");
		else
			Write_Log(" NO");
		WriteLog_Enter();
	}
*/
	//increment job number
	iJobNumber++;

	//set
	bConsiderUnits = bUnitConsiderFlag;

//DEBUG
char NText[20];
//pCA->pTimer->StartCount(TIMER_COUNTER1);

	int iSize = (pCA->pGameWorld->iWidth * pCA->pGameWorld->iHeight);
	int iCurrIterationCount = 0;
	int iRunPF_Flag = 1;     //flag on whether to run the pathfinding loop
	cPLNODE* pGoal = NULL;   //contains the "goal" node, whatever that might be
	cPATHENTRY* pPE_Start = NULL;  //path entry node of the start node and continuing to the finish
	cUnitA* pUnit;

	//set deferflag initially to FALSE
	*pbDeferFlag_Ret = FALSE;

	//get the current tickcount
	//possibly permanent measure to ensure that the pathfinder is not flooded with extensive path orders in one tick(02.02.2003)
	if (pCA->pTimer->iCurrTick > pCA->pPathFinder->llCurrRunningTick)
	{
		IterationsExecuted_Tick = 0;
		llCurrRunningTick = pCA->pTimer->iCurrTick;
	}

	//store the ID of the unit which requested the path
	iID_Unit = IDUnit;

	//a unit with no frame can go nowhere
	for (int c8 = 0; c8 < NUM_MAPTILE_TYPES; c8++)
		PassCharacteristics[c8] = -1;

	//do not do all this on invalid unit(negative index)
	if (iID_Unit >= 0)
	{
		pUnit = pCA->pUnitTable->pUnitTable[iID_Unit];
		cCmpInfo* pInfo = pCA->pCmpInfoTable->pCmpTable[pUnit->pFrame->iType];
		for (int c7 = 0; c7 < NUM_MAPTILE_TYPES; c7++)
			PassCharacteristics[c7] = pInfo->pFrameInfo->MoveCharacteristics[c7];
	}



	if (IDUnit >= 0) //get unit pointer if valid unit
	{
		pUnit = pCA->pUnitTable->pUnitTable[IDUnit];
		//if airborne, unit has no obstructions (for now, just plot direct path)
		if (pUnit->Pos_ABS.z > 0)
		{
			cPATHENTRY*	 pPL_Start = new cPATHENTRY;
			cPATHENTRY*	 pPL_End = new cPATHENTRY;

			//re-jiggered to get the unit to simply go to the target
			pPL_Start->Position.index = Pos_goal.index; //iIndex Start
			pPL_Start->Position.Calc_xy(pCA->pGameWorld->iWidth);
			pPL_Start->pPrev = NULL;
			pPL_Start->pNext = NULL;//(void*)pPL_End;

//			pPL_End->Position.index = iIndex_Goal;
//			pPL_End->Position.Calc_xy(pCA->pGameWorld->iWidth);
//			pPL_End->pNext = NULL;
//			pPL_End->pPrev = (void*)pPL_Start;

			return pPL_Start;
		
		}
	}
 
	//clear out all of the lists
	Clear_Lists_All2();
	//simply relink to each other

	//insert the start node into the templist
	Insert_TempNode2(NULL, iIndex_Start);

	//process it immediately into the openlist
	Process_TempList2();

	//begin the main pathfinding cycle....
	//while(no goal node found)
	//-sort the temp nodes into the open list
	//-process the node at the top of the open list --> -spawn temp nodes, -move it to closed list(do a half-sort)


pCA->pTimer->StartCount(TIMER_PF_A);
	//run this loop until a condition comes to stop it 
	while (iRunPF_Flag == 1)
	{
		//check whether break conditions exist
		if (iCurrIterationCount >= iSearchDepth)
			iRunPF_Flag = 2;
		if (pMOL_First->pNext == pMOL_Last)
			iRunPF_Flag = 3;
		if (pMOL_First->pNext->Pos.index == iIndex_Goal)
			iRunPF_Flag = 4;
		//if the current best open node is worse than some mutiple of the current best closed node
		//the reasoning is that the current best path under consideration is already at least #x worse than simply going straight ahead,
		//and can only get worse. To prevent from exhausting the entire map looking for increasingly crappy paths, let's just quit
			if (pMOL_First->pNext != pMOL_Last)
			{

			//allowable backoff distance reached
			if(pMOL_First->pNext->fDistFromTarget <= ddAllowableBackOffDistance)
			{/*
					//DEBUG
					char NText[50];
				   gcvt(pMCL_First->pNext->fDistFromTarget, 7, NText);
				   Write_Log("Loop broken due to Dist From Target = ");
				   Write_Log(NText);
				   WriteLog_Enter();
				   itoa((int)pMCL_First, NText, 10);
				   Write_Log(NText); 
				   Write_Log("\t");
				   itoa((int)pMCL_First->pNext, NText, 10);
				   Write_Log(NText); 
				   Write_Log("\t");
				   itoa((int)pMCL_First->pNext->pNext, NText, 10);
				   Write_Log(NText); 
				   Write_Log("\t");
			*/
				iRunPF_Flag = 7;
			}

			}
		if (pMCL_First->pNext != pMCL_Last)
		{

		if (bExhaustFlag == FALSE)
		if (pMOL_First->pNext->fEval > (pMCL_First->pNext->fEval * TEMP_PATHFINDER_CRAPPY_PATH_THRESHOLD))
			iRunPF_Flag = 6;
		}
		//if (IterationsExecuted_Tick + iCurrIterationCount >= MaxIterations_Tick)  //do not allow the pathfinder to defer...for now
		//	iRunPF_Flag = 5;



		if (iRunPF_Flag == 1)
		{
			//process the first node on the open list
			Process_OpenNode2( pMOL_First->pNext);
			//process the templist
			Process_TempList2();
			//increse the iterationcount
			iCurrIterationCount++;
		}
	}

pCA->pTimer->EndCount(TIMER_PF_A);
pCA->pTimer->StartCount(TIMER_PF_B);

//DEBUG
/*
char NText2[50];
itoa(iCurrIterationCount, NText2, 10);
if (iRunPF_Flag == 4)
    strcat(NText2, " OK");
if (iRunPF_Flag == 6)
    strcat(NText2, " Crappy Path");
if (iRunPF_Flag == 2)
    strcat(NText2, " Iteration Limit");
if (iRunPF_Flag == 3)
    strcat(NText2, " No more open nodes");
pCA->pChatBuffer->Add_CBNode(NText2, 60);
*/

char text01[50];
	itoa(iRunPF_Flag, text01, 10);
	WriteLog_Enter();
	Write_Log("The Pathfinding loop was broken with flag number ");
	Write_Log(text01);
	Write_Log("\t");
	Write_Log("Allowable Backoff: ");
	gcvt(ddAllowableBackOffDistance, 7, text01);
	Write_Log(text01);
	WriteLog_Enter();  
	
	iFP2_Ret = iRunPF_Flag;

	//if the loop broke on reaching the iterations per tick limit, set the deferflag
	if (iRunPF_Flag == 5)
		*pbDeferFlag_Ret = TRUE;

	//select what the goal node should be depending on what condition the loop exited with
	//nominally select it to be the top node in the open list
	MTILE* pMGoal = pMOL_First->pNext;

	//if we had a "bum" open list, ...flag...
	//DEBUG

	//if the loop broke on running out of open nodes or ran into the iteration count limit
	if (iRunPF_Flag == 3 || iRunPF_Flag == 2 || iRunPF_Flag == 6)
		pMGoal = pMCL_First->pNext;

	if (iRunPF_Flag == 7)
		pMGoal = pMOL_First->pNext;
	//error-catch..
	if (pMGoal == pMOL_Last)
	{
		pCA->pChatBuffer->Add_CBNode("WARNING 113 ... List ran out of open nodes. Set to first node on closed list", 1000);
		pMGoal = pMCL_First->pNext;
	}


	//now, generate the path back to the goal
	if (bReturn_Path == TRUE)
		pPE_Start = Generate_Path2(pMGoal);
	//increase the number of iterations executed
	IterationsExecuted_Tick += iCurrIterationCount;
//DEBUG (write down the number of ticks executed)
	Write_Log("Current Iterations Executed: ");
	itoa(IterationsExecuted_Tick, NText, 10);
	Write_Log(NText);
	Write_Log(" Current Tick: ");
	itoa(pCA->pTimer->iCurrTick, NText, 10);
	Write_Log(NText);
	WriteLog_Enter();
	


pCA->pTimer->EndCount(TIMER_PF_B);
pCA->pTimer->EndCount(TIMER_PATHFIND);

/*
pCA->pTimer->EndCount(TIMER_COUNTER1);
//DEBUG (testing the speed of function
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER1], NText, 10);
pCA->pTimer->Write_Log("   Find Path: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();
*/

	//return pPE_Start
	return pPE_Start;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cPATHENTRY* cPathFinder::Find_Path(int IDUnit, int iIndex_Start, int iIndex_Goal, int iSearchDepth, BOOL* pbDeferFlag_Ret, BOOL bUnitConsiderFlag, double ddAllowableBackOffDistance)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//try re-directing to Find_Path2!!!
	cPATHENTRY* pRet = Find_Path2(IDUnit, iIndex_Start, iIndex_Goal, iSearchDepth, pbDeferFlag_Ret, bUnitConsiderFlag, FALSE, ddAllowableBackOffDistance);

	SQCOORDSET sqStart;
	sqStart.index = iIndex_Start;
	sqStart.Calc_xy(pCA->pGameWorld->iWidth);
	Path_Diag(&sqStart, pRet);

	return pRet;

pCA->pTimer->StartCount(TIMER_PATHFIND);


	//set
	bConsiderUnits = bUnitConsiderFlag;

//DEBUG
char NText[20];
//pCA->pTimer->StartCount(TIMER_COUNTER1);

	int iSize = (pCA->pGameWorld->iWidth * pCA->pGameWorld->iHeight);
	int iCurrIterationCount = 0;
	int iRunPF_Flag = 1;     //flag on whether to run the pathfinding loop
	cPLNODE* pGoal;   //contains the "goal" node, whatever that might be
	cPATHENTRY* pPE_Start;  //path entry node of the start node and continuing to the finish
	cUnitA* pUnit;

	//set deferflag initially to FALSE
	*pbDeferFlag_Ret = FALSE;

	//get the current tickcount
	//possibly permanent measure to ensure that the pathfinder is not flooded with extensive path orders in one tick(02.02.2003)
	if (pCA->pTimer->iCurrTick > pCA->pPathFinder->llCurrRunningTick)
	{
		IterationsExecuted_Tick = 0;
		llCurrRunningTick = pCA->pTimer->iCurrTick;
	}

	//store the ID of the unit which requested the path
	iID_Unit = IDUnit;

	//a unit with no frame can go nowhere
	for (int c8 = 0; c8 < NUM_MAPTILE_TYPES; c8++)
		PassCharacteristics[c8] = -1;

	//do not do all this on invalid unit(negative index)
	if (iID_Unit >= 0)
	{
		pUnit = pCA->pUnitTable->pUnitTable[iID_Unit];
		cCmpInfo* pInfo = pCA->pCmpInfoTable->pCmpTable[pUnit->pFrame->iType];
		for (int c7 = 0; c7 < NUM_MAPTILE_TYPES; c7++)
			PassCharacteristics[c7] = pInfo->pFrameInfo->MoveCharacteristics[c7];
	}



	if (IDUnit >= 0) //get unit pointer if valid unit
	{
		pUnit = pCA->pUnitTable->pUnitTable[IDUnit];
		//if airborne, unit has no obstructions (for now, just plot direct path)
		if (pUnit->Pos_ABS.z > 0)
		{
			cPATHENTRY*	 pPL_Start = new cPATHENTRY;
			cPATHENTRY*	 pPL_End = new cPATHENTRY;

			//re-jiggered to get the unit to simply go to the target
			pPL_Start->Position.index = iIndex_Goal; //iIndex Start
			pPL_Start->Position.Calc_xy(pCA->pGameWorld->iWidth);
			pPL_Start->pPrev = NULL;
			pPL_Start->pNext = NULL;//(void*)pPL_End;

//			pPL_End->Position.index = iIndex_Goal;
//			pPL_End->Position.Calc_xy(pCA->pGameWorld->iWidth);
//			pPL_End->pNext = NULL;
//			pPL_End->pPrev = (void*)pPL_Start;

			return pPL_Start;
		
		}
	}
 
	//clear out all of the lists
	Clear_Lists_All();

	//clear out the path state array
	//Clear_PathArray(); no longer necessary because clear lists() unregisters all the registered nodes from the array (02.02.2002)

	//insert the start node into the templist
	Insert_TempNode(NULL, iIndex_Start, iIndex_Goal);

	//process it immediately into the openlist
	Process_TempList();

	//begin the main pathfinding cycle....
	//while(no goal node found)
	//-sort the temp nodes into the open list
	//-process the node at the top of the open list --> -spawn temp nodes, -move it to closed list(do a half-sort)


pCA->pTimer->StartCount(TIMER_PF_A);
	//run this loop until a condition comes to stop it 
	while (iRunPF_Flag == 1)
	{
		//check whether break conditions exist
		if (iCurrIterationCount >= iSearchDepth)
			iRunPF_Flag = 2;
		if (pOL_First->pNext == (cPLNODE*)pOL_Last)
			iRunPF_Flag = 3;
		if (((cPLNODE*)pOL_First->pNext)->Pos_node.index == iIndex_Goal)
			iRunPF_Flag = 4;
		if (IterationsExecuted_Tick + iCurrIterationCount >= MaxIterations_Tick)
			iRunPF_Flag = 5;

		if (iRunPF_Flag == 1)
		{
			//process the first node on the open list
			Process_OpenNode( (cPLNODE*)pOL_First->pNext);
			//process the templist
			Process_TempList();
			//increse the iterationcount
			iCurrIterationCount++;
		}
	}

pCA->pTimer->EndCount(TIMER_PF_A);
pCA->pTimer->StartCount(TIMER_PF_B);


char text01[50];
	itoa(iRunPF_Flag, text01, 10);
	WriteLog_Enter();
	Write_Log("The Pathfinding loop was broken with flag number ");
	Write_Log(text01);
	WriteLog_Enter();  

	//if the loop broke on reaching the iterations per tick limit, set the deferflag
	if (iRunPF_Flag == 5)
		*pbDeferFlag_Ret = TRUE;

	//select what the goal node should be depending on what condition the loop exited with
	//nominally select it to be the top node in the open list
	pGoal = (cPLNODE*)pOL_First->pNext;

	//if we had a "bum" open list, ...flag...
	//DEBUG

	//if the loop broke on running out of open nodes or ran into the iteration count limit, set it to be the top node on the closed list(01.31.2002)
	if (iRunPF_Flag == 3 || iRunPF_Flag == 2)
		pGoal = (cPLNODE*)pCL_First->pNext;

	//error-catch..
	if (pGoal == pOL_Last)
	{
		pCA->pChatBuffer->Add_CBNode("WARNING 113 ... List ran out of open nodes. Set to first node on closed list", 1000);
		pGoal = (cPLNODE*)pCL_First->pNext;
	}


	//now, generate the path back to the goal
	pPE_Start = Generate_Path(pGoal);
	//increase the number of iterations executed
	IterationsExecuted_Tick += iCurrIterationCount;
//DEBUG (write down the number of ticks executed)
	Write_Log("Current Iterations Executed: ");
	itoa(IterationsExecuted_Tick, NText, 10);
	Write_Log(NText);
	Write_Log(" Current Tick: ");
	itoa(pCA->pTimer->iCurrTick, NText, 10);
	Write_Log(NText);
	WriteLog_Enter();
	


pCA->pTimer->EndCount(TIMER_PF_B);
pCA->pTimer->EndCount(TIMER_PATHFIND);

/*
pCA->pTimer->EndCount(TIMER_COUNTER1);
//DEBUG (testing the speed of function
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER1], NText, 10);
pCA->pTimer->Write_Log("   Find Path: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();
*/

	//return pPE_Start
	return pPE_Start;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cPATHENTRY* cPathFinder::Generate_Path(cPLNODE* pNode_End)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cPATHENTRY*  pPE1; 
	cPATHENTRY*  pPE2;
	cPLNODE*	 pPL_Trav;

	//set the traverser to the node thingie
	pPL_Trav = pNode_End;

	//allocate memory for the first pathentry node
	pPE1 = new cPATHENTRY;

	//fill it with the appropriate information
	pPE1->Position.index = pPL_Trav->Pos_node.index; 
	pPE1->Position.x = pPL_Trav->Pos_node.x; 
	pPE1->Position.y = pPL_Trav->Pos_node.y; 
	pPE1->pNext = NULL;		//this will be the LAST node in the final created list
	pPE1->pPrev = NULL;     //the node "previous" to this has not yet been created


	//now do this (START node will be the last node to meet this condition)
	while ((cPLNODE*)pPL_Trav->pParent != NULL)
	{

		//go back one "generation"
	   pPL_Trav = (cPLNODE*)pPL_Trav->pParent;

	   //allocate pPE2
	   pPE2 = new cPATHENTRY;

		//fill it with the appropriate information
	   pPE2->Position.index = pPL_Trav->Pos_node.index;
	   pPE2->Position.x = pPL_Trav->Pos_node.x;
	   pPE2->Position.y = pPL_Trav->Pos_node.y;
	   pPE2->pNext = (VOID*)pPE1;  
	   pPE2->pPrev = NULL;    //hasn't been allocated yet.

	   pPE1->pPrev = (VOID*)pPE2;   //set pPE1's prev entry to pPE2...

	   //"advance" pPE1 to pPE2
	   pPE1 = pPE2;

	}     //loop should break with pPE1 and pPE2 holding the start node coordinates

	//make sure the start node has prev index set to NULL
	pPE1->pPrev = NULL;


	//return either pPE1 or pPE2
	return pPE1;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cPathFinder::Insert_TempNode(cPLNODE *pParent, int iIndex_node, int iIndex_goal)
{

	//inserts a node into the templist, already sorted(so that the pathfinder only needs to pass through the templist
	//once to correctly sort-list the nodes into the open list

	//procedure here....
	//calculate the desirability of the current space from the target
	//calculate the dist from home(just an increment)
	//create the node
	//fill with appropriate values
	//sort it into the list

	//get classaccess
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cPLNODE* pPL_New;

	//return value
	BOOL bReturn;
	int iProcFlag = 1;    //flag on whether or not to run the insertion

	//allocate memory for the new node
	pPL_New = new cPLNODE;
	//add the appropriate information to the node
	pPL_New->Pos_node.index = iIndex_node;
	pPL_New->Pos_node.Calc_xy(pCA->pGameWorld->iWidth);
	pPL_New->Pos_goal.index = iIndex_goal;
	pPL_New->Pos_goal.Calc_xy(pCA->pGameWorld->iWidth);
	pPL_New->pParent = (VOID*)pParent;

	//extended 03.21.2003 to take into account terrain traversability differences in the pathing
	//if this happens to be the first node, the parent will be NULL, so set up a special case here:
	if (pParent  != NULL)	
	{
		int sum = pParent->Pos_node.x - pPL_New->Pos_node.x + pParent->Pos_node.y - pPL_New->Pos_node.y;
		//if the parent differs diagonally, at 1.4 instead of 1
		if (sum == 2 || sum == -2 || sum == 0)
			pPL_New->fDist_from_home = pParent->fDist_from_home + (1.4 / PassCharacteristics[pCA->pGameWorld->pMap[pPL_New->Pos_node.index].iType]);  //don't try to increment the pParent value...it changes the pParent value!
				else
			pPL_New->fDist_from_home = pParent->fDist_from_home + (1 / PassCharacteristics[pCA->pGameWorld->pMap[pPL_New->Pos_node.index].iType]);  //don't try to increment the pParent value...it changes the pParent value!
	}
			else
		pPL_New->fDist_from_home = 0;   //since it would be the first node

	//get the evalsum
	pPL_New->fEvalSum = Calc_Evalsum(pPL_New->fDist_from_home, pPL_New->Pos_node, pPL_New->Pos_goal);

	//if the evalsum of this node is worse or equal to that of the node that is already in this space, kill
//	if (pPathState[pPL_New->Pos_node.index] != NULL)
//	if (pPathState[pPL_New->Pos_node.index]->fEvalSum <= pPL_New->fEvalSum)
//			iProcFlag = 0;
	
	if (iProcFlag == 0)
	{
		//de-allocate node don't do any processing; it's dead
		delete (pPL_New);
		bReturn = FALSE;
	}
		
	if (iProcFlag == 1)
	{
		//now traverse the temp list and sort...
		//set the traverser to the first node in the temp list(after the anchor)
		pPLTrav = (cPLNODE*)pTL_First->pNext;

		//forward traverse until the traverser reaches the one the new node is immediately before
		while (pPL_New->fEvalSum >= pPLTrav->fEvalSum && pPLTrav != pTL_Last)
			pPLTrav = (cPLNODE*)pPLTrav->pNext;

		//loop should have stopped on a node whose evalsum is higher than that of the new node or the LAST node
		//this is the NEXT pointer
		//set the second traverser to the PREVIOUS pointer
		pPLTrav2 = (cPLNODE*)pPLTrav->pPrev;

		//now link all the nodes together
		pPLTrav2->pNext = (VOID*)pPL_New;
		pPLTrav->pPrev = (VOID*)pPL_New;
		pPL_New->pNext = (VOID*)pPLTrav;
		pPL_New->pPrev = (VOID*)pPLTrav2;


		//now that that node is in the list, the corresponding index in the nodestatus array must be set
		pPathState[pPL_New->Pos_node.index] = pPL_New;

		//set the traversers to NULL
		pPLTrav = NULL;
		pPLTrav2 = NULL;
	
/*  //code to perform diagnostic on pathfinding 
WriteLog_Enter();
char text01[50];
itoa(pPL_New->Pos_node.x, text01, 10);
Write_Log("Temp Node Inserted at x: ");
Write_Log(text01);
Write_Log(" y: ");
itoa(pPL_New->Pos_node.y, text01, 10);
Write_Log(text01);
_gcvt(pPL_New->fEvalSum, 7, text01);
Write_Log(" with evalsum ");
Write_Log(text01);
WriteLog_Enter();
 */

		//signify that the operation was a success
		bReturn = TRUE;
	}

	return bReturn;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Process_TempList()
{

//	cPLNODE* pOL_New;

	//sort the temp list into the open list
	//set the two traverser. #1 to the templist #2 to the openlist
	pTL_Trav = (cPLNODE*)pTL_First->pNext;
	pOL_Trav = (cPLNODE*)pOL_First->pNext;


	//add all of the nodes in the temp list, one by one...
	while (pTL_Trav != pTL_Last)
	{	
		//traverse the open list until it finds the first open node with an evalsum equal or greater than the temp list node(or the LAST node on the open list)
		while(pOL_Trav->fEvalSum <= pTL_Trav->fEvalSum && pOL_Trav != pOL_Last)
			pOL_Trav = (cPLNODE*)pOL_Trav->pNext;
			//should stop when it is on the AFTER node
/*
		//try allocating and see if that is faster (06.28.2002)
		pOL_New = new cPLNODE;
		
		pOL_New->fEvalSum = pTL_Trav->fEvalSum;
		pOL_New->iDist_from_home = pTL_Trav->iDist_from_home;
		pOL_New->Pos_goal = pTL_Trav->Pos_goal;
		pOL_New->Pos_node = pTL_Trav->Pos_node;
		pOL_New->pParent = pTL_Trav->pParent;
		pOL_New->iObjectType = pTL_Trav->iObjectType;

		//insert into list
		pOL_Trav2 = (cPLNODE*)pOL_Trav->pPrev;

		//reset
		pOL_New->pNext = (VOID*)pOL_Trav;
		pOL_New->pPrev = (VOID*)pOL_Trav2;
		pOL_Trav->pPrev = (VOID*)pOL_New;
		pOL_Trav2->pNext = (VOID*)pOL_New;

		//move forward pTL_Trav
		pTL_Trav = (cPLNODE*)pTL_Trav->pNext;
*/
		
		//I'm not exactly sure if this will turn out to be faster than simply creating a new node and inserting it like in Sam32 --> heck I"m not sure this'll work...(01.31.2002)
		//now set the apppropriate pointers so that the current node can be removed and re-linked into the new list
		pTL_Trav2 = (cPLNODE*)pTL_Trav->pPrev;
		pTL_Trav3 = (cPLNODE*)pTL_Trav->pNext;
		pOL_Trav2 = (cPLNODE*)pOL_Trav->pPrev;

		
		//disconnect and re-seal the temp list....
		pTL_Trav2->pNext = (VOID*)pTL_Trav3;
		pTL_Trav3->pPrev = (VOID*)pTL_Trav2;

		//insert the temp node into the open list
		pTL_Trav->pNext = (VOID*)pOL_Trav;
		pTL_Trav->pPrev = (VOID*)pOL_Trav2;
		pOL_Trav->pPrev = (VOID*)pTL_Trav;
		pOL_Trav2->pNext = (VOID*)pTL_Trav;


		//reset pTL_Trav to the next node in the list
		pTL_Trav = pTL_Trav3;
	}

	//after its done, delete the entire list
//	Clear_List_Specify(pTL_First, pTL_Last);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
float cPathFinder::Calc_Evalsum(float fDist_Home, SQCOORDSET Pos_Node, SQCOORDSET Pos_Goal)
{
	
	float evalsum = 0;

	//right now just calculates distances(01.30.2002)
	float xdiff = Pos_Goal.x - Pos_Node.x;
	float ydiff = Pos_Goal.y - Pos_Node.y;

	//this calculation of linear dist to target is strange, but it is true if diagonal moves are allowed
	if (fabs(xdiff) >= fabs(ydiff))
		evalsum = fabs(xdiff);
			else
		evalsum = fabs(ydiff);

	//weight the sum by the astarheuristic
	evalsum = evalsum + fDist_Home / A_Star_Heuristic;

	return evalsum;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Agg_Partition_Sectors()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	MTILE* pMap = pCA->pGameWorld->pMap;
	//reset sector assignments
	for (int lc1 = 0; lc1 < (pCA->pGameWorld->iHeight * pCA->pGameWorld->iWidth); lc1++)
	{
		pMap[lc1].iSector = MAPFILE_SCT_NOSECTOR;
	}
	//clear the sector table and reinitialize
	pCA->pSectorTable->Exit();
	pCA->pSectorTable->Init(pCA);
    	
	//now start at 1 in assigning sectors
	int iNextSectorID = 1;
	for (int lc2 = 0; lc2 < (pCA->pGameWorld->iHeight * pCA->pGameWorld->iWidth); lc2++)
	{
		//blot if the current tile is an unassigned sector
		if (pMap[lc2].iSector == MAPFILE_SCT_NOSECTOR)
		{
			//re-register sector
			pCA->pSectorTable->Register_Sector(iNextSectorID, "Sector", 0);		
			pCA->pSectorTable->Sectors[iNextSectorID]->pRepresentativeTile = &pMap[lc2];
			//blot the map
			Agg_Sector_Blot(lc2, iNextSectorID);

			iNextSectorID++;
		}
	}

	int iWidth = pCA->pGameWorld->iWidth;
	//now establish contacts between sectors
	for (int lc3 = 0; lc3 < (pCA->pGameWorld->iHeight * pCA->pGameWorld->iWidth); lc3++)
	{
		//check left
		if (pMap[lc3].Pos.x > 0)
		if (pMap[lc3].iSector != pMap[lc3 - 1].iSector)
		{
			pCA->pSectorTable->Sectors[ pMap[lc3].iSector]->Add_Link(pMap[lc3-1].iSector);
			pCA->pSectorTable->Sectors[ pMap[lc3-1].iSector]->Add_Link(pMap[lc3].iSector);
		}	
		//check up
		if (pMap[lc3].Pos.y > 0)
		if (pMap[lc3].iSector != pMap[lc3 - iWidth].iSector)
		{
			pCA->pSectorTable->Sectors[ pMap[lc3].iSector]->Add_Link(pMap[lc3-iWidth].iSector);
			pCA->pSectorTable->Sectors[ pMap[lc3-iWidth].iSector]->Add_Link(pMap[lc3].iSector);
		}
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Agg_Sector_Blot(int iIndex_node, int iSectorID)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	//increment the Job number
	iJobNumber++;
	//clear all lists
	Clear_Lists_All2();

	//put the first node into the open list
	Agg_Insert_OpenNode(iIndex_node);

	//now loop until we exhaust the entire open list
	while (pMOL_First->pNext != pMOL_Last)
	{
		MTILE* pTile = pMOL_First->pNext;
		//DEBUG
		//char NText[20];
		//itoa(pTile->Pos.index, NText, 10);
		//Write_Log(NText);
		//Write_Log(".Spawning");

		//"blot"
		pTile->iSector = iSectorID;

		//and propagate
		Agg_Spawn_Nodes_SameTerrain(pTile->Pos.index);

		//DEBUG
		//Write_Log(".Retiring");

		//retire this node
		Agg_Detach_Reattach_To_Closed_List(pTile->Pos.index);

		//DEBUG
		//Write_Log(".Done");
		//WriteLog_Enter();

	}
		//DEBUG
		//Write_Log("Function_Done");
		//WriteLog_Enter();
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Agg_Detach_Reattach_To_Closed_List(int iIndex_node)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	MTILE* pTile = &pCA->pGameWorld->pMap[iIndex_node];	
	
	//re-link
	MTILE* pPrev = pTile->pPrev;
	MTILE* pNext = pTile->pNext;
	pPrev->pNext = pNext;
	pNext->pPrev = pPrev;

	//now go to the closed list
	pPrev = pMCL_First;
	pNext = pMCL_First->pNext;
	//and link
	pPrev->pNext = pTile;
	pNext->pPrev = pTile;
	pTile->pNext = pNext;
	pTile->pPrev = pPrev;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Agg_Spawn_Nodes_SameTerrain(int iNode)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	MTILE* pMap = pCA->pGameWorld->pMap;

	UP = iNode - pCA->pGameWorld->iWidth;
	DOWN = iNode + pCA->pGameWorld->iWidth;
	LEFT = iNode -1;
	RIGHT = iNode + 1;
	UPRIGHT = iNode - pCA->pGameWorld->iWidth + 1;
	UPLEFT = iNode - pCA->pGameWorld->iWidth - 1;
	DOWNLEFT = iNode + pCA->pGameWorld->iWidth - 1;
	DOWNRIGHT = iNode + pCA->pGameWorld->iWidth + 1;

	if (pMap[iNode].Pos.y > 0)
	if (pMap[UP].iType == pMap[iNode].iType)
	if (pMap[UP].iLastParticipatedJobNumber != iJobNumber)
	{
		Agg_Insert_OpenNode(UP);
	}
	if (pMap[iNode].Pos.y < (pCA->pGameWorld->iHeight - 1))
	if (pMap[DOWN].iType == pMap[iNode].iType)
	if (pMap[DOWN].iLastParticipatedJobNumber != iJobNumber)
	{
		Agg_Insert_OpenNode(DOWN);
	}
	if (pMap[iNode].Pos.x > 0)
	if (pMap[LEFT].iType == pMap[iNode].iType)
	if (pMap[LEFT].iLastParticipatedJobNumber != iJobNumber)
	{
		Agg_Insert_OpenNode(LEFT);
	}
	if (pMap[iNode].Pos.x < (pCA->pGameWorld->iWidth - 1))
	if (pMap[RIGHT].iType == pMap[iNode].iType)
	if (pMap[RIGHT].iLastParticipatedJobNumber != iJobNumber)
	{
		Agg_Insert_OpenNode(RIGHT);
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Agg_Insert_OpenNode(int iIndex_node)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	MTILE* pMap = pCA->pGameWorld->pMap;

	if (pMap[iIndex_node].iLastParticipatedJobNumber != iJobNumber)
	{
		pMap[iIndex_node].iLastParticipatedJobNumber = iJobNumber;

		//stick on the end
		MTILE* pNext = pMOL_Last;
		MTILE* pPrev = pMOL_Last->pPrev;

		pMap[iIndex_node].pNext = pNext;
		pMap[iIndex_node].pPrev = pPrev;
		pPrev->pNext = &pMap[iIndex_node];
		pNext->pPrev = &pMap[iIndex_node];
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Clear_Lists_All()
{
	
	//clear out the three lists (be VERY CAREFUL that you don't pass pointers which are not linked, otherwise something bad might happen :-))
	Clear_List_Specify(pOL_First, pOL_Last);
	Clear_List_Specify(pCL_First, pCL_Last);
	Clear_List_Specify(pTL_First, pTL_Last);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Clear_List_Specify(cPLNODE* pPL_Start, cPLNODE* pPL_End)
{
	//clears out all of the nodes between the start and the end nodes
	pPLTrav = (cPLNODE*)pPL_Start->pNext;
	pPLTrav2 = (cPLNODE*)pPLTrav->pNext;

	while (pPLTrav != pPL_End)
	{
		//unregister it from pPathState if it was registered
		//if (pPathState[pPLTrav->Pos_node.index] == pPLTrav)  //do I need this? (06.28.2002)
			pPathState[pPLTrav->Pos_node.index] = NULL;
		//deallocate the current node
		delete(pPLTrav);
		//set it to the next node
		pPLTrav = pPLTrav2;
		//advance to the next node
		pPLTrav2 = (cPLNODE*)pPLTrav2->pNext;
	}

	//re-link the start and end nodes back to each other (otherwise the pather will work once then crash :-))
	pPL_Start->pNext = (cPLNODE*)pPL_End;
	pPL_End->pPrev = (cPLNODE*)pPL_Start;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Process_OpenNode(cPLNODE* pOpenNode)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//do this so I don't have to run references a billion times
	int iWidth = pCA->pGameWorld->iWidth;
	int iNode = pOpenNode->Pos_node.index;
	int iGoal = pOpenNode->Pos_goal.index;

	UP = iNode - iWidth;
	DOWN = iNode + iWidth;
	LEFT = iNode -1;
	RIGHT = iNode + 1;
	UPRIGHT = iNode - iWidth + 1;
	UPLEFT = iNode - iWidth - 1;
	DOWNLEFT = iNode + iWidth - 1;
	DOWNRIGHT = iNode + iWidth + 1;

	//spawn templist nodes adjacent to it if possible
	//by convention, do the orthagonal moves first(actually I think this is to make 
	
	//eval them all first(03.28.2003)
	BOOL Results[8];
	//			  7	0 1
	//			  6 U 2
	//			  5 4 3

	Results[0] = Eval_NewNodeSpawn(UP, pOpenNode);
	Results[1] = Eval_NewNodeSpawn(UPRIGHT, pOpenNode);
	Results[2] = Eval_NewNodeSpawn(RIGHT, pOpenNode);
	Results[3] = Eval_NewNodeSpawn(DOWNRIGHT, pOpenNode);
	Results[4] = Eval_NewNodeSpawn(DOWN, pOpenNode);
	Results[5] = Eval_NewNodeSpawn(DOWNLEFT, pOpenNode);
	Results[6] = Eval_NewNodeSpawn(LEFT, pOpenNode);
	Results[7] = Eval_NewNodeSpawn(UPLEFT, pOpenNode);

	//UP  (if it passes the test, spawn a temp node
	if (Results[0])
		Insert_TempNode(pOpenNode, UP, iGoal);

	//DOWN
	if (Results[4])
		Insert_TempNode(pOpenNode, DOWN, iGoal);
	//LEFT
	if (Results[6])
		Insert_TempNode(pOpenNode, LEFT, iGoal);
	//RIGHT
	if (Results[2])
		Insert_TempNode(pOpenNode, RIGHT, iGoal);

	//diagonal directions	(note that the pathfinder currently blocks against "squeezing" corners)
	//UPLEFT
	if (Results[6] && Results[7] && Results[0])
		Insert_TempNode(pOpenNode, UPLEFT, iGoal);
	//UPRIGHT
	if (Results[0] && Results[1] && Results[2])
		Insert_TempNode(pOpenNode, UPRIGHT, iGoal);
	//DOWNLEFT
	if (Results[4] && Results[5] && Results[6])
		Insert_TempNode(pOpenNode, DOWNLEFT, iGoal);
	//DOWNRIGHT
	if (Results[2] && Results[3] && Results[4])
		Insert_TempNode(pOpenNode, DOWNRIGHT, iGoal);

	//now that thats done, move the open node from the open list to the closed list
	//set traversers
	pOL_Trav = (cPLNODE*)pOpenNode->pPrev;
	pOL_Trav2 = (cPLNODE*)pOpenNode->pNext;

	//unlink from the open list
	pOL_Trav->pNext = (VOID*)pOL_Trav2;
	pOL_Trav2->pPrev = (VOID*)pOL_Trav;

	//check the first address at the top of the closed list...
	pCL_Trav = (cPLNODE*)pCL_First->pNext;
	
	//if it's the LAST node or the evalsum on this node isn't better than the one currently there, move it to the back
	if (pCL_Trav == pCL_Last || pOpenNode->fEvalSum >= pCL_Trav->fEvalSum)
		pCL_Trav = pCL_Last;

	//perform insertion depending on what pCL_Trav is(it will always be the node AFTER where the former open node will go
	pCL_Trav2 = (cPLNODE*)pCL_Trav->pPrev;

	//perform the linkup
	pOpenNode->pNext = (VOID*)pCL_Trav;
	pOpenNode->pPrev = (VOID*)pCL_Trav2;
	pCL_Trav2->pNext = (VOID*)pOpenNode;
	pCL_Trav->pPrev = (VOID*)pOpenNode;

	//okay, that's done

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cPathFinder::Eval_NewNodeSpawn(int iIndex_new, cPLNODE* pParent)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	SQCOORDSET Pos_new;
	BOOL bReturn = TRUE;

	Pos_new.index = iIndex_new;
	Pos_new.Calc_xy(pCA->pGameWorld->iWidth);
	//see how much we can optimize this function because it is called extensively and critically during pathfinding
	MTILE* pMTile = pCA->pGameWorld->pMap + Pos_new.index;
	MTILE* pMTileOld = pCA->pGameWorld->pMap + pParent->Pos_node.index;


	//with the coordinates of the new node, check to see if it is a viable new node
	//check to see if the index is actually on the map
	if (Pos_new.index < 0 || Pos_new.index >= pCA->pGameWorld->iHeight * pCA->pGameWorld->iWidth)
		return FALSE;//bReturn = FALSE;

	//check to see if it is occupied or unreachable
	//actually, currently it doesn't matter....I guess
	//I can't implement the unreach based on terrain yet(since I don't really have any unit types yet), and the current map arrangement can 
	//support unit stacking
	//(01.31.2002)
	//TEST(01.31.2002) just to check if the pather can successfully avoid certain types of terrain
//	if (pMTile->iType == ISURF_OCEAN)
//		return FALSE;//bReturn = FALSE;

	//03.21.2003(extended to unit-specific terrain mapping)
	if (PassCharacteristics[pMTile->iType] < 0)
		return FALSE;


	//temp...disqualifies nodes if ANY type of unit is found, not just enemy ones, the completely correct procedure
	//would involve searching the point of pOccupant_Unit(06.05.2002)
	if (bConsiderUnits)
	if (pMTile->pOccupant_Unit != NULL)
		return FALSE; //bReturn = FALSE;
	if (pMTile->pOccupant_Structure != NULL && pMTileOld->pOccupant_Structure == NULL)
		return FALSE;

	//check to see if the move is actually truly adjacent(like if it has wrapped back to the other side of the map
	int xdiff = Pos_new.x - pParent->Pos_node.x;
	int ydiff = Pos_new.y - pParent->Pos_node.y;

	if (abs(xdiff) > 1 || abs(ydiff) > 1)
		return FALSE;//bReturn = FALSE;

	//check to see if there is already a node on the pathing list here, and if it is, which node would be better
	if (pPathState[Pos_new.index] != NULL)
		return FALSE;

//not sure if this should be here 06.05.2002
//	if (bConsiderUnits)
//	if (pMTile->wIMTargeted_Count != 0)
//		return FALSE; //bReturn = FALSE;

 //bReturn = FALSE; 
//	if (pPathState[Pos_new.index]->iEvalSum < Calc_Evalsum(
//		bReturn = FALSE;     //this check will be performed at tempnode creation so that evalsum calculation only has to happen once --> 02.01.2002
							 //grr...re-enabled because I can't figure out how to make the path improvement scheme work. (02.01.2002)
//checking moved into insert tempnode (02.02.2002)	

	return bReturn;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cPathFinder.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_PATHFINDER >= 1)
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
void cPathFinder::WriteLog_Enter()
{

if (DEBUG + DEBUG_PATHFINDER >= 1)
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
void cPathFinder::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cPathFinder::~cPathFinder()
{

/*	//safety precaution, clear out all of the lists     //moved to Exit() 07.03.2002
	Clear_Lists_All();
	//delete pathstate memory
	delete(pPathState);

	//close the logfile
	Exit_LogFile();

	//delete all the list anchors
	delete(pOL_First);
	delete(pOL_Last);	
	delete(pCL_First);
	delete(pCL_Last);
	delete(pTL_First);
	delete(pTL_Last);
*/
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Exit()
{
	Clear_Lists_All();
	//delete pathstate memory
	delete(pPathState);

	//close the logfile
	Exit_LogFile();

	//delete all the list anchors
	delete(pOL_First);
	delete(pOL_Last);	
	delete(pCL_First);
	delete(pCL_Last);
	delete(pTL_First);
	delete(pTL_Last);

	delete(pSecSchemeOL_First);
	delete(pSecSchemeOL_Last);
	delete(pSecSchemeCL_First);
	delete(pSecSchemeCL_Last);

	delete(pS_First);
	delete(pS_Last);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::SCS_Clear_Lists_All()
{
	pSecSchemeOL_First->pNext = pSecSchemeOL_Last;
	pSecSchemeOL_Last->pPrev = pSecSchemeOL_First;

	pSecSchemeCL_First->pNext = pSecSchemeCL_Last;
	pSecSchemeCL_Last->pPrev = pSecSchemeCL_First;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cPathFinder::CanUnitTraverse(int iUnitID, int iStartIndex, int iEndIndex)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//clear all lists
	SCS_Clear_Lists_All();

	//increment job number
	iJobNumber++;

	//
	MTILE* pEndTile = &pCA->pGameWorld->pMap[iEndIndex];
	cSector* pEndSector = pCA->pSectorTable->Sectors[ pEndTile->iSector ];

	//get the sector
	MTILE* pStartTile = &pCA->pGameWorld->pMap[iStartIndex];
	cSector* pStartSector = pCA->pSectorTable->Sectors[pStartTile->iSector];

	//insert the start sector
	SCS_Insert_OpenNode(pStartSector->iID);

	//then iterate
	while ( pSecSchemeOL_First->pNext != pSecSchemeOL_Last)
	{
		int iCurrSector = pSecSchemeOL_First->pNext->iID;

		//if we have a match, stop and return
		if ( iCurrSector == pEndSector->iID)
			return TRUE;
		//if not, spawn more
		SCS_Spawn_New_OpenNodes(iCurrSector, iUnitID);
		//then retire itself
		SCS_Detach_Reattach_to_Closed_List(iCurrSector);
		
	}

	//if we run out of open nodes, then return false
	return FALSE;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::SCS_Spawn_New_OpenNodes(int iSector, int iUnitID)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cSector* pSector = pCA->pSectorTable->Sectors[iSector];
	
	cUnitA* pUnit = pCA->pUnitTable->pUnitTable[iUnitID];
	float* MoveCharacteristics = pUnit->pFrameType->pFrameInfo->MoveCharacteristics;

	cSectorLinkEntry* pTrav = pSector->pSectorLink_First->pNext;

	while (pTrav != pSector->pSectorLink_Last)
	{
		cSector* pMoveToSector = pCA->pSectorTable->Sectors[ pTrav->iSectorLinkedTo ];

		if (pMoveToSector->iLastParticipatedJobNumber != iJobNumber)
		if (MoveCharacteristics[ pMoveToSector->pRepresentativeTile->iType ] > 0)
		{
			SCS_Insert_OpenNode( pTrav->iSectorLinkedTo );
		}

		pTrav = pTrav->pNext;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::SCS_Detach_Reattach_to_Closed_List(int iSector)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cSector* pSector = pCA->pSectorTable->Sectors[iSector];	
	
	//re-link
	cSector* pPrev = pSector->pPrev;
	cSector* pNext = pSector->pNext;
	pPrev->pNext = pNext;
	pNext->pPrev = pPrev;

	//now go to the closed list
	pPrev = pSecSchemeCL_First;
	pNext = pSecSchemeCL_First->pNext;
	//and link
	pPrev->pNext = pSector;
	pNext->pPrev = pSector;
	pSector->pNext = pNext;
	pSector->pPrev = pPrev;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::SCS_Insert_OpenNode(int iSector)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cSector** Sectors = pCA->pSectorTable->Sectors;

	if (Sectors[iSector]->iLastParticipatedJobNumber != iJobNumber)
	{
		Sectors[iSector]->iLastParticipatedJobNumber = iJobNumber;

		//stick on the end
		cSector* pNext = pSecSchemeOL_Last;
		cSector* pPrev = pSecSchemeOL_Last->pPrev;

		Sectors[iSector]->pNext = pNext;
		Sectors[iSector]->pPrev = pPrev;
		pPrev->pNext = Sectors[iSector];
		pNext->pPrev = Sectors[iSector];
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Remove_SimpleNode(cSimpleNode* pNode)
{
	//removes, but doesn't delete
	cSimpleNode* pNext = pNode->pNext;
	cSimpleNode* pPrev = pNode->pPrev;

	pNext->pPrev = pPrev;
	pPrev->pNext = pNext;

	pNode->pNext = NULL;
	pNode->pPrev = NULL;

	//why does it not delete? deleting it here (18 Oct 2008)
	//otherwise, this becomes the source of a huge memory leak
	delete pNode;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Clear_SimpleList()
{
	cSimpleNode* pTrav = pS_First->pNext;
	cSimpleNode* pTrav2;

	while (pTrav != pS_Last)
	{
		pTrav2 = pTrav->pNext;
		Remove_SimpleNode(pTrav);
		pTrav = pTrav2;
	}
	//relink
	pS_First->pNext = pS_Last;
	pS_Last->pPrev = pS_First;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cSimpleNode* cPathFinder::Add_SimpleNode(int iMapIndex)
{
	cSimpleNode* pNewNode = new cSimpleNode;
	pNewNode->iMapIndex = iMapIndex;

	Add_SimpleNode(pNewNode);

	return pNewNode;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Add_SimpleNode(cSimpleNode* pNode)
{
	//attach at back
	cSimpleNode* pPrev = pS_Last->pPrev;
	cSimpleNode* pNext = pS_Last;

	pPrev->pNext = pNode;
	pNext->pPrev = pNode;

	pNode->pNext = pNext;
	pNode->pPrev = pPrev;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cPathFinder::EvalTankSquare_TakeFrom(int iMapIndex, int iMaterialType, int iCurrJobNumber, double ddMinValue)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iret = 0;
	MTile* pTile = &pCA->pGameWorld->pMap[iMapIndex];

	//kick immediately if we've already processed this tile
	if (pTile->iLastParticipatedJobNumber == iCurrJobNumber)
	{
		return iret;
	}

	//check for tank-carrying unit
	if (pTile->pOccupant_Unit != NULL)
	{
		cUnitA* pUnit = (cUnitA*)pTile->pOccupant_Unit;

		if (pUnit->Storage.ddMaterialCapacity > 0.0)
		{
			if ( pUnit->Storage.iMaterialType == iMaterialType)
			{
				//if the tank doesn't have enough stuff
				if ((ddMinValue > 0.0) &&
				   (pUnit->Storage.ddMaterialStored < ddMinValue))
				{
				//	pCA->pChatBuffer->Add_CBNode("Insufficient supply rejection.", 60);
					iret |= 1;
				} else 
				{
					//if (pUnit->bExternalOrderCompleted == FALSE)
					if (pUnit->Pos_sqcurr.index != pUnit->Pos_sqtarget.index)
					{
						//HACK: if the unit is blocked (assume that that means L2 and L1[0] modes are idle, then
						//pass as OK
						if ((pUnit->bMode_L2 == UMODE_L2_IDLE) && (pUnit->bModes_L1[0] == UMODE_L1_IDLE))
						{
							iret |= 2;
						}
						else	
							iret |= 1;
					//	pCA->pChatBuffer->Add_CBNode("External order rejection.", 60);
						
					}
					else			
						iret |= 2; //end the search! we've found something
				}
			}
			else
			{
			//	pCA->pChatBuffer->Add_CBNode("Material Type rejection.", 60);
				iret |= 1; //we can't put stuff in the tanker but we can use it to reach another tank
			}
		} 
	}
	//check for tank structure
	if (pTile->pOccupant_Structure != NULL)
	{
		cStructureA* pStruct = (cStructureA*)pTile->pOccupant_Structure;
		if (pStruct->Storage.ddMaterialCapacity > 0.0)
		{
			if ( pStruct->Storage.iMaterialType == iMaterialType)
			{
				//if the tank doesn't have enough stuff
				if ((ddMinValue > 0.0) && (pStruct->Storage.ddMaterialStored < ddMinValue))
				{
					iret |= 4;
				} else
					iret |= 8; //found it
			}
			else
				iret |= 4; //we can't put stuff in the tank but we can use it to reach another tank
		} 
		else 
		{
			iret |= 4; //structures that don't have storage capacity can also be used to transfer
		}
	}
	//check for link
	if ( (pTile->barrOverlay & MAPOVERLAY_LINK_MASK ) > 0)
		iret |= 4;

	return iret;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cPathFinder::EvalTankSquare_PutInto(int iMapIndex, int iMaterialType, int iCurrJobNumber, double ddMinValue)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iret = 0;
	MTile* pTile = &pCA->pGameWorld->pMap[iMapIndex];

	//kick immediately if we've already processed this tile
	if (pTile->iLastParticipatedJobNumber == iCurrJobNumber)
	{
		return iret;
	}

	//check for tank-carrying unit
	if (pTile->pOccupant_Unit != NULL)
	{
		cUnitA* pUnit = (cUnitA*)pTile->pOccupant_Unit;

		if (pUnit->Storage.ddMaterialCapacity > 0)
		{
			//only allow units as pipes when putting into
			iret |= 1;
//			if ( (pUnit->Storage.ddMaterialStored <= 0) || (pUnit->Storage.iMaterialType == iMaterialType))
//			{
/*
				//make sure it isn't full
				if (pUnit->Storage.ddMaterialCapacity <= pUnit->Storage.ddMaterialStored)
				{
					//pCA->pChatBuffer->Add_CBNode("Full storage rejection.", 60);
					iret |= 1;
				}

				//if (pUnit->bExternalOrderCompleted == FALSE)
				if (pUnit->Pos_sqcurr.index != pUnit->Pos_sqtarget.index)
				{
					//pCA->pChatBuffer->Add_CBNode("External order rejection.", 60);
					iret |= 1;
				}
				if (pUnit->wExternalOrderIssued == CQ2TYPE_TRANSFER_MATERIAL)
				{
					//pCA->pChatBuffer->Add_CBNode("Transfer material rejection.", 60);
					iret |= 1;
				}

				
				//iret |= 2; //end the search! we've found something

			}
			else
			{
			//	if (pCA->pGameOptions->iControl == pUnit->iControl)
			//		pCA->pChatBuffer->Add_CBNode("Material Type rejection.", 60);
				iret |= 1; //we can't put stuff in the tanker but we can use it to reach another tank
			} */
		} 
	}
	//check for tank structure
	if (pTile->pOccupant_Structure != NULL)
	{
		cStructureA* pStruct = (cStructureA*)pTile->pOccupant_Structure;
		if (pStruct->Storage.ddMaterialCapacity > 0)
		{
			if ( /*(pStruct->Storage.ddMaterialStored <= 0) ||*/ (pStruct->Storage.iMaterialType == iMaterialType))
			{

				//make sure it isn't full...
				if (pStruct->Storage.ddMaterialCapacity > pStruct->Storage.ddMaterialStored)
				{
					iret |= 8; //end the search! we found it
				}
				else
				{
					//pCA->pChatBuffer->Add_CBNode("Full storage rejection.", 60);
					iret |= 4;
				}
			}
			else
			{
				//pCA->pChatBuffer->Add_CBNode("Material Type rejection.", 60);
				iret |= 4; //we can't put stuff in the tank but we can use it to reach another tank
			}
		} 
		else 
		{
				//pCA->pChatBuffer->Add_CBNode(pStruct->Name, 60);
				//pCA->pChatBuffer->Add_CBNode("Capacity rejection.", 60);
			iret |= 4; //structures that don't have storage capacity can also be used to transfer
		}
	}
	//check for link
	if ( (pTile->barrOverlay & MAPOVERLAY_LINK_MASK ) > 0)
		iret |= 4;


	return iret;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cStorageModule* cPathFinder::Find_ConnectedTank(int iMapIndex, int iMaterialType, double ddMinValue, int iMode, int iDummy)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iMapIndexResult = Find_ConnectedTank(iMapIndex, iMaterialType, ddMinValue, iMode);

	if (iMapIndexResult < 0)
		return NULL;

	//if we're here, try to find out what we've got
	MTile* pTile = &pCA->pGameWorld->pMap[iMapIndexResult];

	if (pTile->pOccupant_Structure != NULL)
	{
		cStructureA* pStruct = (cStructureA*)pTile->pOccupant_Structure;
		if (pStruct->Storage.ddMaterialCapacity > 0)
		if ( (pStruct->Storage.ddMaterialStored <= 0) || (pStruct->Storage.iMaterialType == iMaterialType))
			return &pStruct->Storage;
	}
	if (pTile->pOccupant_Unit != NULL)
	{
		cUnitA* pUnit = (cUnitA*)pTile->pOccupant_Unit;
		if (pUnit->Storage.ddMaterialCapacity > 0)
		if ( (pUnit->Storage.ddMaterialStored <= 0) || (pUnit->Storage.iMaterialType == iMaterialType))
			return &pUnit->Storage;
	}
	//if we get here and didn't get anything, return NULL also (we shouldn't, but...)
	pCA->pChatBuffer->Add_CBNode("Error in Find_ConnectedTank", 60);
	return NULL;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cPathFinder::Find_ConnectedTank(int iMapIndex, int iMaterialType, double ddMinValue, int iMode)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iWidth = pCA->pGameWorld->iWidth;
	int iHeight = pCA->pGameWorld->iHeight;
	SQCOORDSET TestIndex;
	MTile* pMap = pCA->pGameWorld->pMap;
	//
	Clear_SimpleList();

	iJobNumber++; //increment job number

	//evaluate the start node (so we can toss out nonsense calls...)
	int iret = 0;
	//#define FINDTANKMODE_PUTINTO	0
	//#define FINDTANKMODE_TAKEFROM	1
	if (iMode == FINDTANKMODE_PUTINTO)
		iret = EvalTankSquare_PutInto(iMapIndex, iMaterialType, iJobNumber, ddMinValue);
	if (iMode == FINDTANKMODE_TAKEFROM)
		iret = EvalTankSquare_TakeFrom(iMapIndex, iMaterialType, iJobNumber, ddMinValue);

	if (iret == 0)
	{
		return -1;
	}
	if ((iret & 0xA) != 0) 
	{
		return iMapIndex;
	}



	//add the current index to the list
	Add_SimpleNode(iMapIndex);
	pMap[iMapIndex].iLastParticipatedJobNumber = iJobNumber;

	
	int iNumToProcessNow = 1;
	int iNumToProcessNextRound = 0;
	
	while (TRUE)
	//for (int lc2 = 0; lc2 < 10; lc2++)
	{
		//
		for (int lc1 = 0; lc1 < iNumToProcessNow; lc1++)
		{
			cSimpleNode* pNode = pS_First->pNext;
			TestIndex.index = pNode->iMapIndex;
			TestIndex.Calc_xy(iWidth);

			//spawn (left)
			if (TestIndex.x != 0)
			{
				int iVal = 0;
				if (iMode == FINDTANKMODE_PUTINTO)
					iVal = EvalTankSquare_PutInto(TestIndex.index - 1, iMaterialType, iJobNumber, ddMinValue);
				if (iMode == FINDTANKMODE_TAKEFROM)
					iVal = EvalTankSquare_TakeFrom(TestIndex.index - 1, iMaterialType, iJobNumber, ddMinValue);

				if ((iVal & 0xA) != 0) 
				{
					return (TestIndex.index - 1);
				}
				if (iVal != 0)
				{
					Add_SimpleNode(TestIndex.index - 1);
					pMap[TestIndex.index - 1].iLastParticipatedJobNumber = iJobNumber;
					iNumToProcessNextRound++;
				}
			}
			//spawn right
			if (TestIndex.x != (iHeight - 1))
			{
				int iVal = 0;
				if (iMode == FINDTANKMODE_PUTINTO)
					iVal = EvalTankSquare_PutInto(TestIndex.index + 1, iMaterialType, iJobNumber, ddMinValue);
				if (iMode == FINDTANKMODE_TAKEFROM)
					iVal = EvalTankSquare_TakeFrom(TestIndex.index + 1, iMaterialType, iJobNumber, ddMinValue);
				if ((iVal & 0xA) != 0) 
				{

					return (TestIndex.index + 1);
				}
				if (iVal != 0)
				{
					Add_SimpleNode(TestIndex.index + 1);
					pMap[TestIndex.index + 1].iLastParticipatedJobNumber = iJobNumber;
					iNumToProcessNextRound++;
				}
			}
			//spawn up
			if (TestIndex.y != 0)
			{
				int iVal = 0;
				if (iMode == FINDTANKMODE_PUTINTO)
					iVal = EvalTankSquare_PutInto(TestIndex.index - iWidth, iMaterialType, iJobNumber, ddMinValue);
				if (iMode == FINDTANKMODE_TAKEFROM)
					iVal = EvalTankSquare_TakeFrom(TestIndex.index - iWidth, iMaterialType, iJobNumber, ddMinValue);
				if ((iVal & 0xA) != 0) 
				{

					return (TestIndex.index - iWidth);
				}
				if (iVal != 0)
				{
					Add_SimpleNode(TestIndex.index - iWidth);
					pMap[TestIndex.index - iWidth].iLastParticipatedJobNumber = iJobNumber;
					iNumToProcessNextRound++;
				}
			}
			//spawn down
			if (TestIndex.y != (iHeight - 1))
			{
				int iVal = 0;
				if (iMode == FINDTANKMODE_PUTINTO)
					iVal = EvalTankSquare_PutInto(TestIndex.index + iWidth, iMaterialType, iJobNumber, ddMinValue);
				if (iMode == FINDTANKMODE_TAKEFROM)
					iVal = EvalTankSquare_TakeFrom(TestIndex.index + iWidth, iMaterialType, iJobNumber, ddMinValue);
				if ((iVal & 0xA) != 0) 
				{
			
					return (TestIndex.index  + iWidth);
				}
				if (iVal != 0)
				{
					Add_SimpleNode(TestIndex.index + iWidth);
					pMap[TestIndex.index + iWidth].iLastParticipatedJobNumber = iJobNumber;
					iNumToProcessNextRound++;
				}
			}
			//remove the current node
			Remove_SimpleNode(pNode);
		}

		//reset and do again
		//
		iNumToProcessNow = iNumToProcessNextRound;
		iNumToProcessNextRound = 0;
		//if none, we're done
		if (iNumToProcessNow == 0)
		{
			return -1;
		}
	}

	pCA->pDD_Display->Write_Log("END. not supposed to get here \r\n");


	//we can't ever reach here, but just to silence a compiler error...
	return -1;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Alter_Parent(cPLNODE* pParent_Old, cPLNODE* pParent_New)  //02.01.2002 02.02.2002(this function is NOT used because the parent altering scheme doesn't work
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iIndex = pParent_Old->Pos_node.index;
	int iWidth = pCA->pGameWorld->iHeight * pCA->pGameWorld->iWidth;

	//performs a change the parent address of all of the nodes spawned by pParent_Old to that of pParent_New
	
	int UP = iIndex - iWidth;
	int DOWN = iIndex + iWidth;
	int LEFT = iIndex -1;
	int RIGHT = iIndex + 1;
	int UPRIGHT = iIndex - iWidth + 1;
	int UPLEFT = iIndex - iWidth - 1;
	int DOWNLEFT = iIndex + iWidth - 1;
	int DOWNRIGHT = iIndex + iWidth + 1;

	//change all of the addresses
	//UP
	if (pPathState[UP] != NULL)
	if (pPathState[UP]->pParent == pParent_Old)
		pPathState[UP]->pParent = pParent_New;
	//DOWN
	if (pPathState[DOWN] != NULL)
	if (pPathState[DOWN]->pParent == pParent_Old)
		pPathState[DOWN]->pParent = pParent_New;

	//LEFT
	if (pPathState[LEFT] != NULL)
	if (pPathState[LEFT]->pParent == pParent_Old)
		pPathState[LEFT]->pParent = pParent_New;

	//RIGHT
	if (pPathState[RIGHT] != NULL)
	if (pPathState[RIGHT]->pParent == pParent_Old)
		pPathState[RIGHT]->pParent = pParent_New;

	//UPRIGHT
	if (pPathState[UPRIGHT] != NULL)
	if (pPathState[UPRIGHT]->pParent == pParent_Old)
		pPathState[UPRIGHT]->pParent = pParent_New;

	//UPLEFT
	if (pPathState[UPLEFT] != NULL)
	if (pPathState[UPLEFT]->pParent == pParent_Old)
		pPathState[UPLEFT]->pParent = pParent_New;

	//DOWNRIGHT
	if (pPathState[DOWNRIGHT] != NULL)
	if (pPathState[DOWNRIGHT]->pParent == pParent_Old)
		pPathState[DOWNRIGHT]->pParent = pParent_New;

	//DOWNLEFT
	if (pPathState[DOWNLEFT] != NULL)
	if (pPathState[DOWNLEFT]->pParent == pParent_Old)
		pPathState[DOWNLEFT]->pParent = pParent_New;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Destroy_Path(cPATHENTRY* pPathEntry) //used to destroy unit paths if they need to be destroyed
{
	cPATHENTRY* pPE_Trav;
	cPATHENTRY* pPE_Trav2;
	cClassAccess* pCA = (cClassAccess*)pCAccess;


//DEBUG
/*
char NText[20];
pCA->pTimer->StartCount(TIMER_COUNTER1);
*/

//don't destroy anything if there isn't any path to destroy(pointer passed is NULL)
if (pPathEntry != NULL)
{
	//first destroy all nodes in front of pPathEntry	
	pPE_Trav = (cPATHENTRY*)pPathEntry->pNext;
	pPE_Trav2 = pPE_Trav;

	while (pPE_Trav2 != NULL)
	{
		pPE_Trav2 = (cPATHENTRY*)pPE_Trav->pNext;

			delete(pPE_Trav);
		pPE_Trav = pPE_Trav2;
	}

	//then destroy all nodes before pPathEntry
	pPE_Trav = (cPATHENTRY*)pPathEntry->pPrev;
	pPE_Trav2 = pPE_Trav;

	while (pPE_Trav2 != NULL)
	{
		pPE_Trav2 = (cPATHENTRY*)pPE_Trav->pPrev;
		delete(pPE_Trav);
		pPE_Trav = pPE_Trav2;
	}

	
	//finally delete pPathEntry itself
	delete(pPathEntry);
	//set it to NULL so unit knows it needs repathing
	pPathEntry = NULL;
}	

/*
pCA->pTimer->EndCount(TIMER_COUNTER1);
//DEBUG (testing the speed of function
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER1], NText, 10);
pCA->pTimer->Write_Log("   Destroy Path: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();
*/



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cPathFinder::Adjust_Target(int iMapIndex, int iIDUnit)
{

		cClassAccess* pCA = (cClassAccess*)pCAccess;
		cUnitA* pUnit;
		cCmpInfo* pInfo;

//DEBUG
//Write_Log("ADJUST TARGET");
//WriteLog_Enter();
//Write_Log(" Time: ");
char NText[50];
//itoa(pCA->pTimer->iCurrTick, NText, 10);
//Write_Log(NText);
//WriteLog_Enter();


//initialize passcharacteristics
for (int c99 = 0; c99 < NUM_MAPTILE_TYPES; c99++)
{
	PassCharacteristics[c99] = -1;
}

//DEBUG
//pCA->pTimer->StartCount(TIMER_COUNTER1);

//returns index of nearest viable node
	cPLNODE*	pAdjust = NULL;
	int failflag = 0;	//denoting the target adjuster failed (i'm too tired to think of a more elegant way to do it)
	


	//store ID of unit
	iID_Unit = iIDUnit;

	//do not do all this on invalid unit(negative index)
	if (iID_Unit >= 0)
	{
		pUnit = pCA->pUnitTable->pUnitTable[iID_Unit];
		pInfo = pCA->pCmpInfoTable->pCmpTable[pUnit->pFrame->iType];
		for (int c7 = 0; c7 < NUM_MAPTILE_TYPES; c7++)
			PassCharacteristics[c7] = pInfo->pFrameInfo->MoveCharacteristics[c7];
	}


//DEBUG
//Write_Log(" Clearing Lists. ");
//WriteLog_Enter();

	//clear the lists
	Clear_Lists_All();


//DEBUG
//Write_Log(" Insert TempNode. ");
//WriteLog_Enter();
	//
	//insert a tempnode on the templist
	AT_Insert_TempNode(NULL, iMapIndex);

	while (pAdjust == NULL)
	{

//DEBUG
//Write_Log(" Processing TempList. ");
//WriteLog_Enter();
		//move the tempnodes onto the open list
		Process_TempList();


//DEBUG
//Write_Log(" Processing Open Nodes. ");
//WriteLog_Enter();
		//if it runs out of open nodes, then flag a failure
		if (pOL_First->pNext != (VOID*)pOL_Last)
		{
			
			pAdjust = AT_Process_OpenNode((cPLNODE*)pOL_First->pNext);

		}
				else
				{ 
//DEBUG
//Write_Log(" Ran out of open nodes. Flagging failure. ");
//WriteLog_Enter();

					failflag = 1;
				}

		if (failflag == 1)
			break;

	
	}

//DEBUG
//Write_Log(" Done. Returning coords: ");
//WriteLog_Enter();

/*
pCA->pTimer->EndCount(TIMER_COUNTER1);
//DEBUG (testing the speed of function
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER1], NText, 10);
pCA->pTimer->Write_Log("   Adjust Target: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();
*/

if (failflag == 1)	
	return -1;
		else
	return pAdjust->Pos_node.index;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::AT_Insert_TempNode(cPLNODE* pParent, int iIndex_node)
{

////DEBUG
Write_Log("     AT_Insert_TempNode: Setting up class access");
WriteLog_Enter();

	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
Write_Log("     AT_Insert_TempNode: Test Node Declaration");
WriteLog_Enter();

cPLNODE* pPL_QI;

//DEBUG
Write_Log("     AT_Insert_TempNode: Allocating Node");
WriteLog_Enter();


//allocate
cPLNODE* pPL_New = new cPLNODE;


//DEBUG
Write_Log("     AT_Insert_TempNode: Setting Information");
WriteLog_Enter();

	//fill in information
	pPL_New->Pos_node.index = iIndex_node;
	pPL_New->Pos_node.Calc_xy(pCA->pGameWorld->iWidth);
	pPL_New->iObjectType = pCA->pUnitTable->pUnitTable[iID_Unit]->iType;
	pPL_New->pParent = (VOID*)pParent;
	
//DEBUG
Write_Log("     AT_Insert_TempNode: Setting Distance from Home");
WriteLog_Enter();

	//set distance from home
	if (pParent == NULL)
		pPL_New->fDist_from_home = 0;
			else
		pPL_New->fDist_from_home = pParent->fDist_from_home + 1;

	//for target adjustment, evalsum is simply distance from home
	pPL_New->fEvalSum = (float)pPL_New->fDist_from_home;

//DEBUG
Write_Log("     AT_Insert_TempNode: Inserting into templist");
WriteLog_Enter();

	//insert it into the list
	pTL_Trav = (cPLNODE*)pTL_First->pNext;

	//traverse the list until it reaches the first node with an equal-or-higher evalsum, or the last node
	while (pPL_New->fEvalSum > pTL_Trav->fEvalSum && pTL_Trav != pTL_Last)
		pTL_Trav = (cPLNODE*)pTL_Trav->pNext;

	//pTL_Trav2 is the previous node
	pTL_Trav2 = (cPLNODE*)pTL_Trav->pPrev;

	//link new to traversers
	pPL_New->pNext = (VOID*)pTL_Trav;
	pPL_New->pPrev = (VOID*)pTL_Trav2;
	pTL_Trav->pPrev = (VOID*)pPL_New;
	pTL_Trav2->pNext = (VOID*)pPL_New;

//DEBUG
Write_Log("     AT_Insert_TempNode: Register into pathstate array");
WriteLog_Enter();

	//register into the pathstate array
	pPathState[iIndex_node] = pPL_New;

//DEBUG
Write_Log("     AT_Insert_TempNode: Done.");
WriteLog_Enter();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cPLNODE* cPathFinder::AT_Process_OpenNode(cPLNODE* pOpenNode)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iWidth = pCA->pGameWorld->iWidth;
	cPLNODE* pReturn = NULL;
	int offsets[8] = {-iWidth, -iWidth + 1, 1, iWidth + 1, iWidth, iWidth - 1, -1, -1 - iWidth};
int randnum = pCA->pRandGen->rand2() % 8;
	//int randnum = rand() % 8;
	char NText[50];



//DEBUG
	itoa(randnum, NText, 10);
	Write_Log("    Process_OpenNode: Random number generated is: ");
	Write_Log(NText);
	WriteLog_Enter();

	//spawn the nodes
//spawn the rest(except for the random one) in sequence
	for (int lc3 = 0; lc3 <= 7; lc3++)
	{

		if (lc3 != randnum)
		{
//DEBUG
	itoa(lc3, NText, 10);
	Write_Log("    Process_OpenNode: Evaluating New Node for Spawning:  ");
	Write_Log(NText);
	WriteLog_Enter();
		if (AT_EvalNewNode_Spawn(pOpenNode->Pos_node.index + offsets[lc3], pOpenNode) == TRUE)
			{
//DEBUG
	itoa(lc3, NText, 10);
	Write_Log("    Process_OpenNode: Spawning Node ");
	Write_Log(NText);
	WriteLog_Enter();
	Write_Log("     Index: ");
	itoa(pOpenNode->Pos_node.index + offsets[lc3], NText, 10);
	Write_Log(NText);
	WriteLog_Enter();
				AT_Insert_TempNode(pOpenNode, pOpenNode->Pos_node.index + offsets[lc3]);
			}

		}
	}

//DEBUG
	itoa(randnum, NText, 10);
	Write_Log("    Process_OpenNode: Testing Node ");
	Write_Log(NText);
	WriteLog_Enter();
//spawn the random one last, so that it ends up first in the temp list
	if (AT_EvalNewNode_Spawn(pOpenNode->Pos_node.index + offsets[randnum], pOpenNode) == TRUE)
	{
//DEBUG
	Write_Log("    Process_OpenNode: Spawning Node ");
	Write_Log(NText);
	WriteLog_Enter();
	Write_Log("     Index: ");
	itoa(pOpenNode->Pos_node.index + offsets[randnum], NText, 10);
	Write_Log(NText);
	WriteLog_Enter();
		AT_Insert_TempNode(pOpenNode, pOpenNode->Pos_node.index + offsets[randnum]);

	}

//DEBUG
	Write_Log("    Process_OpenNode: move open node from open to closed list ");
	WriteLog_Enter();	
	//unlink this node from the open list and move it to the closed list
	pOL_Trav = (cPLNODE*)pOpenNode->pPrev;
	pOL_Trav2 = (cPLNODE*)pOpenNode->pNext;
	
	pOL_Trav->pNext = (VOID*)pOL_Trav2;
	pOL_Trav2->pPrev = (VOID*)pOL_Trav;

	pCL_Trav = (cPLNODE*)pCL_First->pNext;

	//re-link
	pCL_First->pNext = (VOID*)pOpenNode;
	pCL_Trav->pPrev = (VOID*)pOpenNode;
	pOpenNode->pNext = (VOID*)pCL_Trav;
	pOpenNode->pPrev = (VOID*)pCL_First;

		

//DEBUG
	Write_Log("    AT_Process_OpenNode: checking open node for validity ");
	WriteLog_Enter();	
//	itoa(pOpenNode->Pos_node.index, NText, 10);
//	Write_Log(NText);
//	WriteLog_Enter();
	//check the open node for anything and return its address if its good (TEMP until terrain buffers can go up) 06.27.2002
	//also check for friendly units sometimes later on
	if (pCA->pGameWorld->pMap[pOpenNode->Pos_node.index].pOccupant_Unit == NULL && 
//		pCA->pGameWorld->pMap[pOpenNode->Pos_node.index].iType != ISURF_OCEAN) //
		PassCharacteristics[pCA->pGameWorld->pMap[pOpenNode->Pos_node.index].iType] != -1)
	{
//DEBUG
	Write_Log("    AT_Process_OpenNode: open node valid. returning");
	WriteLog_Enter();
		
		pReturn = pOpenNode;
	}


return pReturn;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cPathFinder::AT_EvalNewNode_Spawn(int iMapIndex, cPLNODE* pParent)
{
	BOOL bReturn = TRUE;
	SQCOORDSET Pos_new;
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int xdiff, ydiff;

	Pos_new.index = iMapIndex;
	Pos_new.Calc_xy(pCA->pGameWorld->iWidth);

	//the only qualifications under target adjustment is that the location is on the map(and that the node has not already been listed)
	xdiff = Pos_new.x - pParent->Pos_node.x;
	ydiff = Pos_new.y - pParent->Pos_node.y;

	if (abs(xdiff) > 1 || abs(ydiff) > 1)
		return FALSE;

	if (Pos_new.index < 0 || Pos_new.index >= (pCA->pGameWorld->iHeight * pCA->pGameWorld->iWidth))
		return FALSE;

	if (pPathState[iMapIndex] != NULL)
		return FALSE;

	return TRUE;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Path_Diag(SQCOORDSET* pSqCurr, cPATHENTRY* pPath)
{

	char NText[50];
	int nodenum = 0;
	int dx = 0;
	int dy = 0;
	cPATHENTRY* pPrev = pPath;

	Write_Log("Start Pos: ");
	itoa(pSqCurr->x, NText, 10);
	Write_Log(NText);
	Write_Log(", ");
	itoa(pSqCurr->y, NText, 10);
	Write_Log(NText);
	WriteLog_Enter();

	if (pPath == NULL)
	{
		Write_Log("Hey! this path is null!");
		WriteLog_Enter();
	}

	while(pPath != NULL)
	{
		//calculate delta
		dx = pPath->Position.x - pPrev->Position.x;
		dy = pPath->Position.y - pPrev->Position.y;
		//issue an alert if either is greater than 1
		if (abs(dx) > 1 || abs(dy) > 1)
		{Write_Log("ALERT!"); WriteLog_Enter(); }

		//dump the position of the following nodes:
		Write_Log("Node ");
		itoa(nodenum, NText, 10);
		Write_Log(NText);
		Write_Log(": ");
		//itoa(pPath->Position.x, NText, 10);
		itoa(dx, NText, 10);
		Write_Log(NText);
		Write_Log(", ");
		//itoa(pPath->Position.y, NText, 10);
		itoa(dy, NText, 10);
		Write_Log(NText);
		WriteLog_Enter();
		nodenum++;
		//increment
		pPrev = pPath;
		pPath = (cPATHENTRY*)pPath->pNext;

	}
		Write_Log("Completed path trace.");
		WriteLog_Enter();


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cPathFinder::arePointsConnected(int iMapIndex1, int iMapIndex2)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iWidth = pCA->pGameWorld->iWidth;
	int iHeight = pCA->pGameWorld->iHeight;

	SQCOORDSET TestIndex;
	MTile* pMap = pCA->pGameWorld->pMap;
	//
	Clear_SimpleList();

	iJobNumber++; //increment job number

	//evaluate the start node (so we can toss out nonsense calls...)
	int iret = 0;
		iret = EvalTankSquare_TakeFrom(iMapIndex1, -1, iJobNumber, 0);

	if (iret == 0)
	{
			//DEBUG
			pCA->pChatBuffer->Add_CBNode("Initial reject", 60);

		return -1;
	}
	//add the current index to the list
	Add_SimpleNode(iMapIndex1);
	pMap[iMapIndex1].iLastParticipatedJobNumber = iJobNumber;

	
	int iNumToProcessNow = 1;
	int iNumToProcessNextRound = 0;

	while (TRUE)
	//for (int lc2 = 0; lc2 < 10; lc2++)
	{

		//
		for (int lc1 = 0; lc1 < iNumToProcessNow; lc1++)
		{
			cSimpleNode* pNode = pS_First->pNext;
			TestIndex.index = pNode->iMapIndex;
			TestIndex.Calc_xy(iWidth);

			//return TRUE if we got here
			if (TestIndex.index == iMapIndex2)
			{


				SQCOORDSET sqGoal;
				sqGoal.index = iMapIndex2;
				sqGoal.Calc_xy(pCA->pGameWorld->iWidth); 
				SQCOORDSET sqStart;
				sqStart.index = iMapIndex1;
				sqStart.Calc_xy(pCA->pGameWorld->iWidth);
				return Calc_Distance(sqGoal.x, sqGoal.y, sqStart.x, sqStart.y);
			}

			//spawn (left)
			if (TestIndex.x != 0)
			{
				int iVal = 0;
					iVal = EvalTankSquare_TakeFrom(TestIndex.index - 1, -1, iJobNumber, 0);


				if ((iVal & 0xC) != 0) //tank STRUCTURES only
				{

					Add_SimpleNode(TestIndex.index - 1);
					pMap[TestIndex.index - 1].iLastParticipatedJobNumber = iJobNumber;
					iNumToProcessNextRound++;
				}
			}
			//spawn right
			if (TestIndex.x != (iHeight - 1))
			{
				int iVal = 0;
					iVal = EvalTankSquare_TakeFrom(TestIndex.index + 1, -1, iJobNumber, 0);

				if ((iVal & 0xC) != 0) //tank STRUCTURES only
				{

					Add_SimpleNode(TestIndex.index + 1);
					pMap[TestIndex.index + 1].iLastParticipatedJobNumber = iJobNumber;
					iNumToProcessNextRound++;
				}
			}
			//spawn up
			if (TestIndex.y != 0)
			{
				int iVal = 0;
					iVal = EvalTankSquare_TakeFrom(TestIndex.index - iWidth, -1, iJobNumber, 0);

				if ((iVal & 0xC) != 0) //tank STRUCTURES only
				{

					Add_SimpleNode(TestIndex.index - iWidth);
					pMap[TestIndex.index - iWidth].iLastParticipatedJobNumber = iJobNumber;
					iNumToProcessNextRound++;
				}
			}
			//spawn down
			if (TestIndex.y != (iHeight - 1))
			{
				int iVal = 0;
					iVal = EvalTankSquare_TakeFrom(TestIndex.index + iWidth, -1, iJobNumber, 0);

				if ((iVal & 0xC) != 0) //tank STRUCTURES only
				{

					Add_SimpleNode(TestIndex.index + iWidth);
					pMap[TestIndex.index + iWidth].iLastParticipatedJobNumber = iJobNumber;
					iNumToProcessNextRound++;
				}
			}
			//remove the current node
			Remove_SimpleNode(pNode);
		}

		//reset and do again
		//
		iNumToProcessNow = iNumToProcessNextRound;
		iNumToProcessNextRound = 0;
		//if none, we're done
		if (iNumToProcessNow == 0)
		{
			//DEBUG
			pCA->pChatBuffer->Add_CBNode("Ran out of nodes", 60);
			return -1;
		}
	}

	//we can't ever reach here, but just to silence a compiler error...
	pCA->pChatBuffer->Add_CBNode("Error in arePointsConnected", 60);
	return -1;
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPathFinder::Clear_PathArray()   //no longer used (too slow !) 02.02.2002
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iSize = pCA->pGameWorld->iHeight * pCA->pGameWorld->iWidth;
	cPLNODE** pi;

	for (c1 = 0; c1 <= iSize; c1++)
	{
		pi = pPathState + c1;
		*pi = NULL;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
