#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Init(LPSTR FileName, VOID* pCA)
{
	pCAccess = pCA;

if (DEBUG + DEBUG_GAMEWORLD_A >= 1)
	Init_LogFile();
	
	//iMap_xview = 0; vars now part of cInterface 12.28.2001
	//iMap_yview = 0;

	Load_Map_200(FileName);

	//initialize resource payouts
	TileInfos[MAPTILE_GRASSLAND].ResourcePayout[MATERIAL_ELEMENT_C] = 0.3;
	TileInfos[MAPTILE_GRASSLAND].ResourcePayout[MATERIAL_ELEMENT_I] = 1;
	TileInfos[MAPTILE_HILL].ResourcePayout[MATERIAL_ELEMENT_I] = 3;
	TileInfos[MAPTILE_FOREST].ResourcePayout[MATERIAL_ELEMENT_C] = 1;
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cGameWorldA::IsValidLinkLocation(SQCOORDSET sqLocation)
{
	if (sqLocation.x  < 0)
		return FALSE;
	if (sqLocation.y < 0)
		return FALSE;
	if (sqLocation.x >= iWidth)
		return FALSE;
	if (sqLocation.y >= iHeight)
		return FALSE;

	if ( (pMap[sqLocation.index].barrOverlay & MAPOVERLAY_LINK_MASK) != 0) 
		return FALSE;

	return TRUE;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cGameWorldA::IsValidLinkLocation(int iX_Square, int iY_Square)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	SQCOORDSET sqLocation;
	
	sqLocation.x = iX_Square;
	sqLocation.y = iY_Square;
	sqLocation.Calc_Index(iWidth);

	return IsValidLinkLocation(sqLocation);
}

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cGameWorldA::BigIsValidBuildLocation(int iX_Square, int iY_Square, int iStructType)
{
	int iNumOcean = 0;

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//prepare for location checks
	cStructInfo* pInfo = pCA->pStructInfoTable->pTable[iStructType];

	//if
	if (iX_Square >= iWidth)
		return FALSE;
	if (iX_Square < 0)
		return FALSE;
	if (iY_Square >= iHeight)
		return FALSE;
	if (iY_Square < 0)
		return FALSE;

	//-1?
	for (int lc95 = 0; lc95 < (pInfo->iOccMaptiles); lc95++)
	{

         int iTile = (iX_Square + pInfo->Pos_offset_array[lc95].x) + (pInfo->Pos_offset_array[lc95].y + iY_Square) * pCA->pGameWorld->iWidth;
		SQCOORDSET sqTile;
		sqTile.index = iTile;
		sqTile.Calc_xy(pCA->pGameWorld->iWidth);
	
		if (pCA->pGameWorld->pMap[iTile].iType == MAPTILE_OCEAN)
			iNumOcean++;

		if ( pCA->pGameWorld->IsValidBuildLocation(sqTile, iStructType) == FALSE)
		{
  			return FALSE;
		}
	}

	//for shipyards, there must be at least one ocean tile
	if (iStructType == TEMP_STYPE_SHIPYARD)
	if ((iNumOcean == 0) || (iNumOcean > 3))
		return FALSE;

	return TRUE;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cGameWorldA::IsValidBuildLocation(SQCOORDSET sqLocation, int iStructType)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	if (sqLocation.x  < 0)
		return FALSE;
	if (sqLocation.y < 0)
		return FALSE;
	if (sqLocation.x >= (iWidth-1))
		return FALSE;
	if (sqLocation.y >= (iHeight-1))
		return FALSE;

	if (pMap[sqLocation.index].pOccupant_Structure != NULL)
		return FALSE;

	if (pMap[sqLocation.index].pOccupant_Unit != NULL)
		return FALSE;

	//for now, can't buidlin forest
	if (pMap[sqLocation.index].iType == MAPTILE_FOREST)
		return FALSE;

	if (iStructType != TEMP_STYPE_SHIPYARD)
	if (pMap[sqLocation.index].iType == MAPTILE_OCEAN)
		return FALSE;

	return TRUE;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cGameWorldA::Link_Point_to_Point(int iMapIndex1, int iMapIndex2)
{
	//spawn links from point to point, return the number of NEW links created
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//create a path from point to point
	cPATHENTRY* pPath = pCA->pPathFinder->Find_Path_Link(iMapIndex1, iMapIndex2, 2000);

	int iNumNewLinks = 0;
	while (pPath != NULL)
	{
		cPATHENTRY* pPathNext = (cPATHENTRY*)pPath->pNext;

		if ( (pMap[pPath->Position.index].barrOverlay & MAPOVERLAY_LINK_MASK) == 0)
		{	
			iNumNewLinks++;
			Add_Link(pPath->Position.index);
		}
		//delete and set
		delete pPath;
		pPath = pPathNext;
	}

	return iNumNewLinks;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cGameWorldA::IsValidBuildLocation(int iX_Square, int iY_Square, int iStructType)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	SQCOORDSET sqLocation;
	
	sqLocation.x = iX_Square;
	sqLocation.y = iY_Square;
	sqLocation.Calc_Index(iWidth);

	return IsValidBuildLocation(sqLocation, iStructType);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Add_Link(int iMapIndex)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;


	//error check
		if ( (iMapIndex < 0) || (iMapIndex > (iHeight * iWidth) ) )
		return;

	//set link value
	MTile* pTile = &pMap[iMapIndex];

	pTile->barrOverlay = pTile->barrOverlay | MAPOVERLAY_LINK_MASK;

	int iX = iMapIndex % iWidth;

	int iUp = iMapIndex - iWidth;
	int iDown = iMapIndex + iWidth;
	int iLeft = iMapIndex - 1;
	int iRight = iMapIndex + 1;

	//up
	if (iUp >= 0)
	{
		MTile* pTileUp = &pMap[iUp];
		//if this Tile contains an occupant structure, link it too
		if (pTileUp->pOccupant_Structure != NULL)
			pTileUp->barrOverlay = pTileUp->barrOverlay | MAPOVERLAY_LINK_MASK;
		
		//if we have a link here too, redetermine drawframe for neighboring tile
		if ((pTileUp->barrOverlay & MAPOVERLAY_LINK_MASK) > 0)
			pTileUp->iDrawFrame_LinkGraphic = Determine_Link_DrawFrame(iUp);
	}
	//down
	if (iDown >= 0)
	{
		MTile* pTileDown = &pMap[iDown];
		//if this Tile contains an occupant structure, link it too
		if (pTileDown->pOccupant_Structure != NULL)
			pTileDown->barrOverlay = pTileDown->barrOverlay | MAPOVERLAY_LINK_MASK;
		
		//if we have a link here too, redetermine drawframe for neighboring tile
		if ((pTileDown->barrOverlay & MAPOVERLAY_LINK_MASK) > 0)
			pTileDown->iDrawFrame_LinkGraphic = Determine_Link_DrawFrame(iDown);
	}
	//Right
	if (iX < (iWidth - 1) )
	{
		MTile* pTileRight = &pMap[iRight];
		//if this Tile contains an occupant structure, link it too
		if (pTileRight->pOccupant_Structure != NULL)
			pTileRight->barrOverlay = pTileRight->barrOverlay | MAPOVERLAY_LINK_MASK;
		
		//if we have a link here too, redetermine drawframe for neighboring tile
		if ((pTileRight->barrOverlay & MAPOVERLAY_LINK_MASK) > 0)
			pTileRight->iDrawFrame_LinkGraphic = Determine_Link_DrawFrame(iRight);
	}
	//Left
	if (iX > 0)
	{
		MTile* pTileLeft = &pMap[iLeft];
		//if this Tile contains an occupant structure, link it too
		if (pTileLeft->pOccupant_Structure != NULL)
			pTileLeft->barrOverlay = pTileLeft->barrOverlay | MAPOVERLAY_LINK_MASK;
		
		//if we have a link here too, redetermine drawframe for neighboring tile
		if ((pTileLeft->barrOverlay & MAPOVERLAY_LINK_MASK) > 0)
			pTileLeft->iDrawFrame_LinkGraphic = Determine_Link_DrawFrame(iLeft);
	}
	//finally determine it's own drawframe
	pTile->iDrawFrame_LinkGraphic = Determine_Link_DrawFrame(iMapIndex);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cGameWorldA::Determine_Link_DrawFrame(int iMapIndex)
{	
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	if ( (iMapIndex < 0) || (iMapIndex > (iHeight * iWidth) ) )
		return -1;

	MTile* pTile = &pMap[iMapIndex];

	//if selected tile doesn't contain a link, return -1
	if ((pTile->barrOverlay & MAPOVERLAY_LINK_MASK) == 0)
		return -2;

	int iX = iMapIndex % iWidth;

	int iDrawFrame = 0;

	int iUp = iMapIndex - iWidth;
	int iDown = iMapIndex + iWidth;
	int iLeft = iMapIndex - 1;
	int iRight = iMapIndex + 1;

	//up direction adds 1
	if (iUp >= 0)
	{
		MTile* pTileUp = &pMap[iUp];
		if ( (pTileUp->barrOverlay & MAPOVERLAY_LINK_MASK) > 0)
			iDrawFrame += 1;
	}
	//down adds 4
	if (iDown < (iWidth * iHeight) )
	{
		MTile* pTileDown = &pMap[iDown];
		if ( (pTileDown->barrOverlay & MAPOVERLAY_LINK_MASK) > 0)
			iDrawFrame += 4;
	}
	//right adds 2
	//condition is : "if not the right-most tile" (since there's nothing to check in that case
	if (iX < (iWidth - 1) )
	{
		MTile* pTileRight = &pMap[iRight];
		if ( (pTileRight->barrOverlay & MAPOVERLAY_LINK_MASK) > 0)
			iDrawFrame += 2;
	}
	//left adds 8
	//condition is : "if not the left-most tile" (since there's nothing to check in that case)
	if (iX > 0)
	{
		MTile* pTileLeft = &pMap[iLeft];
		if ( (pTileLeft->barrOverlay & MAPOVERLAY_LINK_MASK) > 0)
			iDrawFrame += 8;
	}

	return iDrawFrame;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Load_Map_101(LPSTR FileName)
{
	
	//read in the file with the supplied filename
	hMapFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

if (hMapFile == INVALID_HANDLE_VALUE)
	Write_Log("CreateFile Failed!");

	//read in the first 4 bytes as two words: these have the height and width
	//re-use the loadbuffer here so we don't have to declare another buffer
	pLoadBuffer = new BYTE[4];
	ReadFile(hMapFile, pLoadBuffer, 4, &dwNumBytesRead, NULL);

	//use a word pointer to access the data
	WORD* wp = (WORD *)pLoadBuffer;
	iWidth = *(wp);
	wp++;
	iHeight = *(wp);
	//delete the load buffer so that we can read in the actual map
	delete(pLoadBuffer);


	//now read in the rest of the file to the loadbuffer...the information gets stored as bytes....
	//first allocate memory for the loadbuffer
	pLoadBuffer = new BYTE[iHeight*iWidth];
	ReadFile(hMapFile, pLoadBuffer, iHeight*iWidth, &dwNumBytesRead, NULL);

	pMap = new MTile[iHeight*iWidth];

	//copy the tile information from the loadbuffer into the map array
	for (c1 = 0; c1 < (iHeight * iWidth) ; c1++)
	{
	pByteRead = (BYTE*)pLoadBuffer; //-1 to adjust for the +1 above
	pByteRead = pByteRead + c1;
	pMap[c1].iType = (BYTE) *pByteRead;
					//initialize its own position
					pMap[c1].Pos.index = c1;
					pMap[c1].Pos.Calc_xy(iWidth);
	}

	//now deallocate the loadbuffer
	delete(pLoadBuffer);

	//initialize draw frames for all tiles at 0(their default)
	//and initialize stacking pointers
	for (c1 = 0; c1 <= iHeight * iWidth - 1; c1++)
	{
		pMap[c1].iDrawFrame = 0;
		pMap[c1].iDrawFrame_Layer2 = 0;
		pMap[c1].pOccupant_Unit = NULL;
		pMap[c1].pOccupant_Structure = NULL;
		pMap[c1].bIMTargeted_Nation = -1;
		pMap[c1].wIMTargeted_Count = 0;
		pMap[c1].barrOverlay = 0;

	}

//DEBUG
char NText[50];
for (int lc9 = iHeight * iWidth - 10; lc9 < iHeight * iWidth; lc9++)
{
Write_Log("Maptile ");
itoa(lc9, NText, 10);
Write_Log(NText);
Write_Log(": ");
itoa(pMap[lc9].iType, NText, 10);
Write_Log(NText);
WriteLog_Enter();

}

	//modify the map iDrawFrames to appropriate values
	Modify_Map();



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cGameWorldA::UniqueMapIDOf(char* FileName)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//read in the file with the supplied filename
	hMapFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	//if we couldn't find the file, return -2
	if (hMapFile == INVALID_HANDLE_VALUE)
	{
		return -2;
	}

	int iIDBlock = MAP_V2_IDBLOCK_NOTHING;

	char LoadBuffer[80]; //buffer _max 
	char inchar = ' ';
	char NText[50];

	int chcount;
	int iSize = 0;

	ZeroMemory(LoadBuffer, sizeof(char)*80);
	while (LoadBuffer[0] != '~')
	{
		//check for a tilde
		ReadFile(hMapFile, LoadBuffer, 1, &dwNumBytesRead, NULL);

		//if the character is an open bracket, read until closed bracket
		if (LoadBuffer[0] == '[')
		{
			Read_To_Stop(LoadBuffer, ']');

			iIDBlock = MAP_V2_IDBLOCK_NOTHING;
			//figure out what was in the identifier block
			//'BASIC' (first, map data block)
			if (LoadBuffer[0] == 'B' && 
				LoadBuffer[1] == 'A' &&
				LoadBuffer[2] == 'S' &&
				LoadBuffer[3] == 'I' &&
				LoadBuffer[4] == 'C' &&
				LoadBuffer[5] == 0)
			{
				iIDBlock = MAP_V2_IDBLOCK_BASIC;
			}

			//depending on what was read by the identifier block, figure out how to read this data
			
			//BASIC block
			if (iIDBlock == MAP_V2_IDBLOCK_BASIC)
			{
				//search for the next open bracket
				Read_To_Stop(LoadBuffer, '{');

				
				Read_To_Stop(LoadBuffer, ',');
				int iVersion = atof(LoadBuffer);

				Read_To_Stop(LoadBuffer, ',');
				int iID = atoi(LoadBuffer);
				
				//if we have an ID, return it
				CloseHandle(hMapFile);
				return iID;

			}
		}
	}
	//if we didn't find anything, return -1
	return -1;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//05.11.2004
void cGameWorldA::Load_Map_200(LPSTR FileName)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;


	int iIDBlock = MAP_V2_IDBLOCK_NOTHING;

	//read in the file with the supplied filename
	hMapFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	char LoadBuffer[80]; //buffer _max 
	char inchar = ' ';
	char NText[50];

	int chcount;
	int iSize = 0;

	LoadBuffer[0] = 0;

	if (pMap != NULL)
	{
		delete(pMap);
		pMap = NULL;
	}


	//use tilde as indicator of EOF
	while (LoadBuffer[0] != '~')
	{
		
		//check for a tilde
		ReadFile(hMapFile, LoadBuffer, 1, &dwNumBytesRead, NULL);

		//if the character is an open bracket, read until closed bracket
		if (LoadBuffer[0] == '[')
		{

			Read_To_Stop(LoadBuffer, ']');

			iIDBlock = MAP_V2_IDBLOCK_NOTHING;
			//figure out what was in the identifier block
			//'BASIC' (first, map data block)
			if (LoadBuffer[0] == 'B' && 
				LoadBuffer[1] == 'A' &&
				LoadBuffer[2] == 'S' &&
				LoadBuffer[3] == 'I' &&
				LoadBuffer[4] == 'C' &&
				LoadBuffer[5] == 0)
			{
				iIDBlock = MAP_V2_IDBLOCK_BASIC;
			}
			//'UCR' (unit create block)
			if (LoadBuffer[0] == 'U' && 
				LoadBuffer[1] == 'C' &&
				LoadBuffer[2] == 'R' &&
				LoadBuffer[3] == 0)
			{
				iIDBlock = MAP_V2_IDBLOCK_UCR;
			}
			//'SCR' (structure create block)
			if (LoadBuffer[0] == 'S' && 
				LoadBuffer[1] == 'C' &&
				LoadBuffer[2] == 'R' &&
				LoadBuffer[3] == 0)
			{
				iIDBlock = MAP_V2_IDBLOCK_SCR;
			}
			//'SCR' (structure create block)
			if (LoadBuffer[0] == 'F' && 
				LoadBuffer[1] == 'I' &&
				LoadBuffer[2] == 'L' &&
				LoadBuffer[3] == 'L' && 
				LoadBuffer[4] == 'U' &&
				LoadBuffer[5] == 'N' &&
				LoadBuffer[6] == 'I' && 
				LoadBuffer[7] == 'T' &&
				LoadBuffer[8] == 0)
			{
				iIDBlock = MAP_V2_IDBLOCK_FILLUNIT;
			}
			//'BRIEFTEXT
			if (LoadBuffer[0] == 'B' && 
				LoadBuffer[1] == 'R' &&
				LoadBuffer[2] == 'I' &&
				LoadBuffer[3] == 'E' && 
				LoadBuffer[4] == 'F' &&
				LoadBuffer[5] == 'T' &&
				LoadBuffer[6] == 'E' && 
				LoadBuffer[7] == 'X' &&
				LoadBuffer[8] == 'T' &&
				LoadBuffer[9] == 0)
			{
				iIDBlock = MAP_V2_IDBLOCK_BRIEFTEXT;
			}
			//'NATION' (nation define block)
			if (LoadBuffer[0] == 'N' && 
				LoadBuffer[1] == 'A' &&
				LoadBuffer[2] == 'T' &&
				LoadBuffer[3] == 'I' && 
				LoadBuffer[4] == 'O' &&
				LoadBuffer[5] == 'N' &&
				LoadBuffer[6] == 0)
			{
				iIDBlock = MAP_V2_IDBLOCK_NATION;
			}
			//'NATION' (nation define block)
			if (LoadBuffer[0] == 'S' && 
				LoadBuffer[1] == 'E' &&
				LoadBuffer[2] == 'T' &&
				LoadBuffer[3] == 'L' && 
				LoadBuffer[4] == 'A' &&
				LoadBuffer[5] == 'Y' &&
				LoadBuffer[6] == 'E' && 
				LoadBuffer[7] == 'R' &&
				LoadBuffer[8] == '2' &&
				LoadBuffer[9] == 0)
			{
				iIDBlock = MAP_V2_IDBLOCK_SETLAYER2;
			}

			//'SECTOR' (sector define block)
			if (LoadBuffer[0] == 'S' && 
				LoadBuffer[1] == 'E' &&
				LoadBuffer[2] == 'C' &&
				LoadBuffer[3] == 'T' && 
				LoadBuffer[4] == 'O' &&
				LoadBuffer[5] == 'R' &&
				LoadBuffer[6] == 0)
			{
				iIDBlock = MAP_V2_IDBLOCK_SECTOR;
			}
			//'TUTORIAL' (enable/disable tutorial)
			if (LoadBuffer[0] == 'T' && 
				LoadBuffer[1] == 'U' &&
				LoadBuffer[2] == 'T' &&
				LoadBuffer[3] == 'O' && 
				LoadBuffer[4] == 'R' &&
				LoadBuffer[5] == 'I' &&
				LoadBuffer[6] == 'A' &&
				LoadBuffer[7] == 'L' &&
				LoadBuffer[8] == 0)
			{
				iIDBlock = MAP_V2_IDBLOCK_TUTORIAL;
			}
			//'SECTREG' (sector registration block)
			if (LoadBuffer[0] == 'S' && 
				LoadBuffer[1] == 'E' &&
				LoadBuffer[2] == 'C' &&
				LoadBuffer[3] == 'T' && 
				LoadBuffer[4] == 'R' &&
				LoadBuffer[5] == 'E' &&
				LoadBuffer[6] == 'G' &&
				LoadBuffer[7] == 0)
			{
				iIDBlock = MAP_V2_IDBLOCK_SECTREG;
			}

			//depending on what was read by the identifier block, figure out how to read this data
			
			//BASIC block
			if (iIDBlock == MAP_V2_IDBLOCK_BASIC)
			{
				//search for the next open bracket
				Read_To_Stop(LoadBuffer, '{');

				
				Read_To_Stop(LoadBuffer, ',');
				iVersion = atof(LoadBuffer);

				Read_To_Stop(LoadBuffer, ',');
				iID = atoi(LoadBuffer);

				Read_To_Stop(LoadBuffer, ',');
				iWidth = atoi(LoadBuffer);
				
				Read_To_Stop(LoadBuffer, ',');
				iHeight = atoi(LoadBuffer);

				iSize = iHeight*iWidth;

				//allocate
				pMap = new MTile[iSize]; 

				//the rest of the characters are the basic map data...the terrain.
				//read these in until the size limit is reached, or there is no more
				//data in the block(in that case, fill in with default(grassland)
				chcount = 0;
				while(chcount < iSize && inchar != '}')
				{
					ReadFile(hMapFile, &inchar, 1, &dwNumBytesRead, NULL);					

					//identify it's own position
					pMap[chcount].Pos.index = chcount;
					pMap[chcount].Pos.Calc_xy(iWidth);
					//now check if any of these are valid characters
					//
					if (inchar == MAPFILE_SYM_GRASSLAND)
					{
						pMap[chcount].iType = MAPTILE_GRASSLAND;
						chcount++;
						//DEBUG
						Write_Log("Grassland");
						WriteLog_Enter();
					}
					if (inchar == MAPFILE_SYM_OCEAN)
					{
						pMap[chcount].iType = MAPTILE_OCEAN;
						chcount++;
						//DEBUG
						Write_Log("Ocean");
						WriteLog_Enter();
					}
					if (inchar == MAPFILE_SYM_RIVER)
					{
						pMap[chcount].iType = MAPTILE_RIVER;
						chcount++;
						//DEBUG
						Write_Log("Mountain");
						WriteLog_Enter();
					}
					if (inchar == MAPFILE_SYM_MOUNTAIN)
					{
						pMap[chcount].iType = MAPTILE_MOUNTAIN;
						chcount++;
						//DEBUG
						Write_Log("Mountain");
						WriteLog_Enter();
					}
					if (inchar == MAPFILE_SYM_HILL)
					{
						pMap[chcount].iType = MAPTILE_HILL;
						chcount++;
						//DEBUG
						Write_Log("Hill");
						WriteLog_Enter();
					}
					if (inchar == MAPFILE_SYM_FOREST)
					{
						pMap[chcount].iType = MAPTILE_FOREST;
						chcount++;
						//DEBUG
						Write_Log("Forest");
						WriteLog_Enter();
					}
				}

				//if there was not enough data in the block, fill in the rest with ocean
				if (chcount < iSize - 1)
				for (int c48 = chcount; c48 < iSize; c48++)
				{
					pMap[c48].iType = MAPTILE_OCEAN;
						//DEBUG
						Write_Log("Ocean DEFAULT");
						WriteLog_Enter();
					
				}
				//if there was too much data in the block, advance to the end of the bracket
				if (inchar != '}')
				{
					Read_To_Stop(LoadBuffer, '}');
				}
				
				//now, fill in everything else
				//initialize draw frames for all tiles at 0(their default)
				//and initialize stacking pointers
				for (c1 = 0; c1 <= iHeight * iWidth - 1; c1++)
				{
					pMap[c1].barrRevealed = 0;
					pMap[c1].iDrawFrame = 0;
					pMap[c1].iDrawFrame_Layer2 = 0;
					pMap[c1].pOccupant_Unit = NULL;
					pMap[c1].pOccupant_Structure = NULL;
					pMap[c1].bIMTargeted_Nation = -1;
					pMap[c1].wIMTargeted_Count = 0;
					pMap[c1].iSector = MAPFILE_SCT_NOSECTOR;  //default sector, "no sector"
					pMap[c1].barrOverlay = 0;

				}

				//register the "no sector" ID
				pCA->pSectorTable->Register_Sector(MAPFILE_SCT_NOSECTOR, "Undefined Sector", 0);
			}
			if (iIDBlock == MAP_V2_IDBLOCK_SETLAYER2)
			{
				//DEBUG
				Write_Log("SETLAYER2 Block");
				WriteLog_Enter();

				//search for the next open bracket
				Read_To_Stop(LoadBuffer, '{');

				//read in X position
				Read_To_Stop(LoadBuffer, ',');
				int iX = atoi(LoadBuffer);
				//read in Y position
				Read_To_Stop(LoadBuffer, ',');
				int iY = atoi(LoadBuffer);
				//read in Layer2 type
				Read_To_Stop(LoadBuffer, '}');
				int iType = atoi(LoadBuffer);
					//allow substitutions
					if (LoadBuffer[0] == 'U' || LoadBuffer[0] == 'u')
						iType = 31; //this is the uranium frame

				SQCOORDSET Coords;
				Coords.x = iX;
				Coords.y = iY;
				Coords.Calc_Index(pCA->pGameWorld->iWidth);

				pCA->pGameWorld->pMap[Coords.index].iDrawFrame_Layer2 = iType;

			}
			if (iIDBlock == MAP_V2_IDBLOCK_FILLUNIT)
			{
				//DEBUG
				Write_Log("FILLUNIT Block");
				WriteLog_Enter();

				//search for the next open bracket
				Read_To_Stop(LoadBuffer, '{');

				//read in unit ID
				Read_To_Stop(LoadBuffer, ',');
				int iID = atoi(LoadBuffer);
				//to make this easier on ourselves, if we have a negative value here, attempt to fill the LAST unit creatd
				if (iID < 0)
					iID = pCA->pUnitList->ID_Last_Assigned;

				//then read in material type
				Read_To_Stop(LoadBuffer, ',');
				int iType = atoi(LoadBuffer);
				char CheckChar = toupper(LoadBuffer[0]);
				int iTypeCheck = pCA->pMaterialInfoTable->FindType_ByRefChar(CheckChar);

				if (iTypeCheck >= 0 && iTypeCheck < NUM_MATERIAL_TYPES)
					iType = iTypeCheck;

				//then read in amount
				Read_To_Stop(LoadBuffer, '}');
				double fAmount = atof(LoadBuffer);

				//okay now check for ID
				cUnitA* pUnit = NULL;

				if (iID <= pCA->pUnitList->ID_Last_Assigned )
				{	
					pUnit = pCA->pUnitTable->pUnitTable[iID];
					if (pUnit != NULL)
					{
						pUnit->Storage.iMaterialType = iType;
						pUnit->Storage.ddMaterialStored = fAmount;
						if (fAmount > pUnit->Storage.ddMaterialCapacity)
							pUnit->Storage.ddMaterialStored = pUnit->Storage.ddMaterialCapacity;
						//recalc the unit's attribus
						pUnit->Recalc_Attributes();
						pUnit->Update_SupportedCommands();
					}
				}
			}
			if (iIDBlock == MAP_V2_IDBLOCK_BRIEFTEXT)
			{
				//DEBUG
				Write_Log("BRIEFTEXT Block");
				WriteLog_Enter();

				//search for the next open bracket
				Read_To_Stop(LoadBuffer, '{');
				char BriefText[5000];
				ZeroMemory(BriefText, sizeof(char)*5000);
				Read_To_Stop(BriefText, '}', 5000);

				//now load this data to the main buffer
				ZeroMemory(pCA->pGameWorld->Briefing.BriefText, sizeof(char)*5000);
				memcpy(pCA->pGameWorld->Briefing.BriefText, BriefText, sizeof(char)*strlen(BriefText));
			}
			if (iIDBlock == MAP_V2_IDBLOCK_TUTORIAL)
			{
				//DEBUG
				Write_Log("Tutorial Block");
				WriteLog_Enter();

				//search for the next open bracket
				Read_To_Stop(LoadBuffer, '{');
				Read_To_Stop(LoadBuffer, '}');

				if (LoadBuffer[0] == 'T' || LoadBuffer[0] == 't')
					pCA->pGameOptions->bTutorial = TRUE;
				if (LoadBuffer[0] == 'F' || LoadBuffer[0] == 'f')
					pCA->pGameOptions->bTutorial = FALSE;
			}
			if (iIDBlock == MAP_V2_IDBLOCK_SECTOR)
			{
				//DEBUG
				Write_Log("SECTOR Block");
				WriteLog_Enter();

				//search for the next open bracket
				Read_To_Stop(LoadBuffer, '{');

				//now, what follows here is a chain of little packets of two characters ( ex. <AF> ) 
				//that correspond to each maptile on the map. The number of packets SHOULD be equal
				//to the number of maptiles on the map(measures should be taken to ensure failure proofing otherwise)
				int iIndex = 0;

				inchar = ' ';
				//read characters until end-of-file(error occurred) or the close curly bracket is found(})
				while(iIndex < iSize && inchar != '}' && dwNumBytesRead != 0)
				{
					//read in characters
					ReadFile(hMapFile, &inchar, 1, &dwNumBytesRead, NULL);
			
					//if an open-angle bracket is found(begin packet)
					if (inchar == '<')
					{
						//read to the close bracket
						Read_To_Stop(LoadBuffer, '>');
						//now, create a short pointer that reads the first two bytes(characters) as the sector index value
						unsigned short* pSector = (unsigned short*)LoadBuffer;
						int iSector = *pSector;

						//DEBUG
						itoa(iSector, NText, 10);
						Write_Log("Sector: ");
						Write_Log(NText);
						WriteLog_Enter();

						//set the maptile sector value at this index
						pMap[iIndex].iSector = iSector;
						iIndex++;
					}


				}
				//if there were not enough packets, fill the rest with default value(0)
				for (int c993 = iIndex; c993 < iSize; c993++)
				{
					pMap[c993].iSector = MAPFILE_SCT_NOSECTOR;
				}
				//if we didn't stop on the close bracket '}', read until it
				if (inchar != '}')
					Read_To_Stop(LoadBuffer, '}');
			}
			if (iIDBlock == MAP_V2_IDBLOCK_SECTREG)
			{
				//Read to open bracket
				Read_To_Stop(LoadBuffer, '{');

				//DEBUG
				Write_Log("SECTREG Block");
				WriteLog_Enter();
				
				//this one is easy....there are three fields, the name, ID, and resource bonus
				//by convention, the name is in the first field
				char NameText[TEMP_STD_STRLEN];
				//name...
				ZeroMemory(LoadBuffer, sizeof(char)*TEMP_STD_STRLEN);
				ZeroMemory(NameText, sizeof(char)*TEMP_STD_STRLEN);
				
				//read in and copy
				Read_To_Stop(LoadBuffer, ',');
				memcpy(NameText, LoadBuffer, strlen(LoadBuffer));
					//DEBUG
					Write_Log("Name: ");
					Write_Log(NameText);
					WriteLog_Enter();
		
				//read in the ID
				int iID = 0;
				ZeroMemory(LoadBuffer, sizeof(char)*TEMP_STD_STRLEN);
				Read_To_Stop(LoadBuffer, ',');
				//now, this will also be a string(of two characters). we need to convert this in the same way as we converted
				//it in the SECTOR block before
				unsigned short* pID = (unsigned short*)LoadBuffer;
				iID = *pID;
					//DEBUG
					Write_Log("ID: ");
					itoa(iID, NText, 10);
					Write_Log(NText);
					WriteLog_Enter();

				
				//read in the resource bonus
				int iResourceBonus = 0;
				ZeroMemory(LoadBuffer, sizeof(char)*TEMP_STD_STRLEN);
				Read_To_Stop(LoadBuffer, '}');
				iResourceBonus = atoi(LoadBuffer);
					//DEBUG
					Write_Log("Resource Bonus: ");
					itoa(iResourceBonus, NText, 10);
					Write_Log(NText);
					WriteLog_Enter();					

				//now register the sector
				pCA->pSectorTable->Register_Sector(iID, NameText, iResourceBonus);

			}
			if (iIDBlock == MAP_V2_IDBLOCK_NATION)
			{
				//DEBUG
				Write_Log("NATION Block");
				WriteLog_Enter();

				//search for the next open bracket
				Read_To_Stop(LoadBuffer, '{');

				//read in nation ID
				Read_To_Stop(LoadBuffer, ',');
				int iNationID = atoi(LoadBuffer);

				//name...
				char NameText[TEMP_STD_STRLEN];
				ZeroMemory(LoadBuffer, sizeof(char)*80);
				ZeroMemory(NameText, sizeof(char)*80);
				Read_To_Stop(LoadBuffer, ',');
				memcpy(NameText, LoadBuffer, strlen(LoadBuffer));

				//bHumanPlayable Flag
				BOOL bHumanPlayable = TRUE; //default
				Read_To_Stop(LoadBuffer, '}');

				if (LoadBuffer[0] == 'F' || LoadBuffer[0] == 'f')
				{
					bHumanPlayable = FALSE;
				}
				if (LoadBuffer[0] == 'T' || LoadBuffer[0] == 't')
				{
					bHumanPlayable = TRUE;
				}

				//now set
				if ( (iNationID >= 0) && (iNationID < TEMP_DP_MAXPLAYERS))
				{
					pCA->pNationManager->Init_Nation(iNationID, NameText, strlen(NameText), FALSE, bHumanPlayable);
				}
			}
			if (iIDBlock == MAP_V2_IDBLOCK_UCR)
			{

				//DEBUG
				Write_Log("UCR Block");
				WriteLog_Enter();

				//search for the next open bracket
				Read_To_Stop(LoadBuffer, '{');

				//this one is fairly simple....

				char NameText[TEMP_STD_STRLEN];
				//name...
				ZeroMemory(LoadBuffer, sizeof(char)*80);
				ZeroMemory(NameText, sizeof(char)*80);

				Read_To_Stop(LoadBuffer, ',');
				memcpy(NameText, LoadBuffer, strlen(LoadBuffer));


				Read_To_Stop(LoadBuffer, ',');
				int iX = atoi(LoadBuffer);
				
				Read_To_Stop(LoadBuffer, ',');
				int iY = atoi(LoadBuffer);
				
				Read_To_Stop(LoadBuffer, ',');
				int iControl = atoi(LoadBuffer);
				
				Read_To_Stop(LoadBuffer, '}');
				int iType = atoi(LoadBuffer);
				
				//create (do not create if nation controlling the structure does not exist
				if (pCA->pNationManager->ppNations[iControl] != NULL)
				{
					pCA->pUnitList->Create_Unit(NameText, iType, iControl, iX, iY);
				}

				
			}
			if (iIDBlock == MAP_V2_IDBLOCK_SCR)
			{

				//DEBUG
				Write_Log("SCR Block");
				WriteLog_Enter();

				//search for the next open bracket
				Read_To_Stop(LoadBuffer, '{');

				//this one is fairly simple....get the three parameters
				char NameText[80];
				//name...
				ZeroMemory(LoadBuffer, sizeof(char)*80);
				ZeroMemory(NameText, sizeof(char)*80);

				Read_To_Stop(LoadBuffer, ',');
				memcpy(NameText, LoadBuffer, strlen(LoadBuffer));

				Read_To_Stop(LoadBuffer, ',');
				int iX = atoi(LoadBuffer);
				
				Read_To_Stop(LoadBuffer, ',');
				int iY = atoi(LoadBuffer);
				
				Read_To_Stop(LoadBuffer, ',');
				int iControl = atoi(LoadBuffer);
				
				Read_To_Stop(LoadBuffer, '}');
				int iType = atoi(LoadBuffer);
				
				//create
				//(do not create if the nation that will control the structure does not exist)
				if (pCA->pNationManager->ppNations[iControl] != NULL)
				{
					cStructureA* pStruct = pCA->pStructList->Create_Structure(NameText, 0, iType, iControl, iX, iY);
					//the current Create_Structure routine is designed for "unit use" (units use it to build structures)
					//which leaves it's fCompletion at 0. so we must manually set the fCompletion to 1 here
					pStruct->fCompletion = 1.0;
					pStruct->iHP = pStruct->iHPMax;
				}
		
			}			
		}


	}


	//modify the map iDrawFrames to appropriate values
	Modify_Map();


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cGameWorldA::~cGameWorldA()
{

	//the allocation for pMap gets reported for a memory leak...i have no idea why, since it is being
	//deleted at this destructor!
	//delete(pMap);
	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Exit()
{
	if (pMap != NULL)
		delete(pMap);
	pMap = NULL;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cGameWorldA.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_GAMEWORLD_A >= 1)
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
void cGameWorldA::WriteLog_Enter()
{
if (DEBUG + DEBUG_GAMEWORLD_A >= 1)
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
void cGameWorldA::Modify_MTile_Ocean(int MapIndex)
{

ABOVE = MapIndex-iWidth;
ABOVE_LEFT = MapIndex-iWidth-1;
ABOVE_RIGHT = MapIndex-iWidth+1;
LEFT = MapIndex - 1;
RIGHT = MapIndex + 1;
BELOW = MapIndex + (iWidth);
BELOW_LEFT = MapIndex + (iWidth) - 1;
BELOW_RIGHT = MapIndex + (iWidth) + 1;

int iDrawFrame = 0; //initially assume no neighbors
int iDrawFrame_Layer2 = 0;

//Main Layer:
//
//				128 1  2
//				 64	T  4
//			     32 16 8 			
//
//
//Layer 2:
//					1
//				  8	T 2
//			        4

//adjust iDrawFrame depending on what neighbors exist
//adjust layer2 iDrawFrame to determine drawing of rivermouths
//check against invalid "neighboring" values(off the map, on the other side of the map)
if (pMap[MapIndex].iType == ISURF_OCEAN)
	{
		if (ABOVE >= 0)
			{
			if (pMap[ABOVE].iType != ISURF_OCEAN)
				iDrawFrame = iDrawFrame + 1;
			if (pMap[ABOVE].iType == ISURF_RIVER)
				iDrawFrame_Layer2 = iDrawFrame_Layer2 + 1;
			}

		
		if (ABOVE_RIGHT >= 0 && MapIndex % iWidth != iWidth - 1)
		if (pMap[ABOVE_RIGHT].iType != ISURF_OCEAN) 
			iDrawFrame = iDrawFrame + 2;

		
		if (MapIndex % iWidth != iWidth - 1)
			{
			if (pMap[RIGHT].iType != ISURF_OCEAN) 
				iDrawFrame = iDrawFrame + 4;
			if (pMap[RIGHT].iType == ISURF_RIVER)
				iDrawFrame_Layer2 = iDrawFrame_Layer2 + 2;
			}


		if (BELOW_RIGHT < (iHeight * iWidth) && MapIndex % iWidth != iWidth - 1)
		if (pMap[BELOW_RIGHT].iType != ISURF_OCEAN) 
				iDrawFrame = iDrawFrame + 8;
			

		if (BELOW < iHeight * iWidth)
			{
			if (pMap[BELOW].iType != ISURF_OCEAN) 
				iDrawFrame = iDrawFrame + 16;
			if (pMap[BELOW].iType == ISURF_RIVER)
				iDrawFrame_Layer2 = iDrawFrame_Layer2 + 4;
			}


		if (BELOW_LEFT < (iHeight * iWidth) && MapIndex % iWidth != 0)
		if (pMap[BELOW_LEFT].iType != ISURF_OCEAN)
			iDrawFrame = iDrawFrame + 32;
		
		if (MapIndex % iWidth != 0)
			{
			if (pMap[LEFT].iType != ISURF_OCEAN)
				iDrawFrame = iDrawFrame + 64;
			if (pMap[LEFT].iType == ISURF_RIVER)
				iDrawFrame_Layer2 = iDrawFrame_Layer2 + 8;
			}

		if (ABOVE_LEFT >= 0 && MapIndex % iWidth != 0)
		if (pMap[ABOVE_LEFT].iType != ISURF_OCEAN)
			iDrawFrame = iDrawFrame + 128;

		pMap[MapIndex].iDrawFrame = iDrawFrame;
		pMap[MapIndex].iDrawFrame_Layer2 = iDrawFrame_Layer2;
	
	}

	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cGameWorldA::isNextToOcean(int iIndex)
{
	SQCOORDSET sqCheck;

	sqCheck.index  = iIndex;
	sqCheck.Calc_xy(iWidth);

	//UP
	sqCheck.y--;
	sqCheck.Calc_Index(iWidth);
	if (sqCheck.y >= 0)
	if (pMap[sqCheck.index].iType == MAPTILE_OCEAN)
		return TRUE;
	//UPRIGHT
	sqCheck.x++;
	sqCheck.Calc_Index(iWidth);
	if (sqCheck.x < iWidth)
	if (sqCheck.y >= 0)
	if (pMap[sqCheck.index].iType == MAPTILE_OCEAN)
		return TRUE;
	//RIGHT
	sqCheck.y++;
	sqCheck.Calc_Index(iWidth);
	if (sqCheck.x < iWidth)
	if (pMap[sqCheck.index].iType == MAPTILE_OCEAN)
		return TRUE;
	//DOWNRIGHT
	sqCheck.y++;
	sqCheck.Calc_Index(iWidth);
	if (sqCheck.x < iWidth)
	if (sqCheck.y < iHeight)
	if (pMap[sqCheck.index].iType == MAPTILE_OCEAN)
		return TRUE;
	//DOWN
	sqCheck.x--;
	sqCheck.Calc_Index(iWidth);
	if (sqCheck.y < iHeight)
	if (pMap[sqCheck.index].iType == MAPTILE_OCEAN)
		return TRUE;
	//DOWNLEFT
	sqCheck.x--;
	sqCheck.Calc_Index(iWidth);
	if (sqCheck.y < iHeight)
	if (sqCheck.x >= 0)
	if (pMap[sqCheck.index].iType == MAPTILE_OCEAN)
		return TRUE;
	//LEFT
	sqCheck.y--;
	sqCheck.Calc_Index(iWidth);
	if (sqCheck.x >= 0)
	if (pMap[sqCheck.index].iType == MAPTILE_OCEAN)
		return TRUE;
	//UPLEFT
	sqCheck.y--;
	sqCheck.Calc_Index(iWidth);
	if (sqCheck.x >= 0)
	if (sqCheck.y >= 0)
	if (pMap[sqCheck.index].iType == MAPTILE_OCEAN)
		return TRUE;

	//if we failed all these return false
	return FALSE;
	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Modify_MTile_4Way(int MapIndex)
{
ABOVE = MapIndex-iWidth;
LEFT = MapIndex - 1;
RIGHT = MapIndex + 1;
BELOW = MapIndex + (iWidth);

//initially assume no neighbors
int iDrawFrame = 0;

//					1
//				  8	T 2
//			        4

		if (ABOVE >= 0)
		if (pMap[ABOVE].iType == pMap[MapIndex].iType)
			iDrawFrame = iDrawFrame + 1;

		if (BELOW < iHeight * iWidth)
		if (pMap[BELOW].iType == pMap[MapIndex].iType)
			iDrawFrame = iDrawFrame + 4;

		if (MapIndex % iWidth != 0)
		if (pMap[LEFT].iType == pMap[MapIndex].iType)
			iDrawFrame = iDrawFrame + 8;

		if (MapIndex % iWidth != iWidth - 1)
		if (pMap[RIGHT].iType == pMap[MapIndex].iType)
			iDrawFrame = iDrawFrame + 2;

		pMap[MapIndex].iDrawFrame = iDrawFrame;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Modify_MTile_River(int MapIndex)
{

ABOVE = MapIndex-iWidth;
LEFT = MapIndex - 1;
RIGHT = MapIndex + 1;
BELOW = MapIndex + (iWidth);

//initially assume no neighbors
int iDrawFrame = 0;

//					1
//				  8	T 2
//			        4

//the RIVER tile has is neighbor-dependent on both ocean and river tiles

		if (ABOVE >= 0)
			if (pMap[ABOVE].iType == ISURF_RIVER || pMap[ABOVE].iType == ISURF_OCEAN)
			iDrawFrame = iDrawFrame + 1;

		if (BELOW < iHeight * iWidth)
		if (pMap[BELOW].iType == ISURF_RIVER || pMap[BELOW].iType == ISURF_OCEAN)
			iDrawFrame = iDrawFrame + 4;

		if (MapIndex % iWidth != 0)
		if (pMap[LEFT].iType == ISURF_RIVER || pMap[LEFT].iType == ISURF_OCEAN)
			iDrawFrame = iDrawFrame + 8;

		if (MapIndex % iWidth != iWidth - 1)
		if (pMap[RIGHT].iType == ISURF_RIVER || pMap[RIGHT].iType == ISURF_OCEAN)
			iDrawFrame = iDrawFrame + 2;

		pMap[MapIndex].iDrawFrame = iDrawFrame;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Modify_Map()
{

	for (c1 = 0; c1 < iHeight*iWidth; c1++)
	{
		pMap[c1].iDrawFrame = 0;
//		pMap[c1].iDrawFrame_Layer2 = 0;
//		itoa(c1, Text, 10);
//		Write_Log("-");
//		Write_Log(Text);
//		Write_Log("-");
//		if (pMap[c1].iType == ISURF_FOREST)
//			Modify_MTile_4Way(c1);
		if (pMap[c1].iType == ISURF_RIVER)
			Modify_MTile_River(c1);
		if (pMap[c1].iType == ISURF_OCEAN)
			Modify_MTile_Ocean(c1);
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Register_Unit(SQCOORDSET Pos_square, VOID* pUnitA)    //01.18.2002 register and unregister will probably call these functions to update their positions
																		 //or, they might not use these functions at all and may simply do it themselves, although it would be harder
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	if (pUnitA == NULL)
	{
		pCA->pChatBuffer->Add_CBNode("Register_Unit: Gave me a NULL Pointer!", 60);
		return;
	}

	//TEMP recalc the index just to make sure the right values were passed
	Pos_square.Calc_Index(iWidth);

	cUnitA* pUnitNext = (cUnitA*)pMap[Pos_square.index].pOccupant_Unit;
	cUnitA* pUnit = (cUnitA*)pUnitA;

	//will link the occupant pointer on the specified maptile to that unit
	pUnit->pNext_Stack = (VOID*)pUnitNext;
	pUnit->pPrev_Stack = NULL;   //because there isn't any really viable way to link back to the MTILE

	//if there actually is a unit already stacked here at this tile, modify it's backlink
	if (pUnitNext != NULL)
		pUnitNext->pPrev_Stack = (VOID*)pUnit;
	//set the occupant pointer itself to point to this new unit at the front
	pMap[Pos_square.index].pOccupant_Unit = (VOID*)pUnit;

	//(06.03.2002) Register the unit's IMTarget on the Gameworld
	pMap[pUnit->Pos_sqimtarget.index].wIMTargeted_Count++;

	//if it was the first unit of that nation to set there, set the nation flag too
	if (pMap[pUnit->Pos_sqimtarget.index].wIMTargeted_Count == 1)
		pMap[pUnit->Pos_sqimtarget.index].bIMTargeted_Nation = pUnit->iControl;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Register_Structure(SQCOORDSET Pos_square, VOID* pStructureA)
{
	//for the moment I do not deem it necessary to code support
	//for multiple structures stacking fully or partially on top of each other
	//(and have no idea how I would use that or how that would work out)
	//so the code currently going here will assume 1 structure per square only
	//(03.28.2003)
	cStructureA* pStructure = (cStructureA*)pStructureA;
	cClassAccess* pCA = (cClassAccess*)pCAccess;


	//register the Pos_primary position
	pMap[pStructure->Pos_primary.index].pOccupant_Structure = pStructureA;

	for (int c35 = 0; c35 < pStructure->iNumArrayEntries; c35++)
	{
		pMap[pStructure->Pos_array[c35].index].pOccupant_Structure = pStructureA;
	}

	//here for now, basically if this structure is a sector capital, set the pCapital of the sector it is on to this structure
/* disabled 09.20.2006
	if (pStructure->iType == 33 || pStructure->iType == 34)
	{
		//DEBUG
		Write_Log("Capital registered in sector");
		WriteLog_Enter();

		pCA->pSectorTable->Sectors[pMap[Pos_square.index].iSector]->pCapital = pStructure;
		pCA->pSectorTable->Sectors[pMap[Pos_square.index].iSector]->iControl = pStructure->iControl;
	}
*/

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Unregister_Unit(SQCOORDSET Pos_square, VOID* pUnitA)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cUnitA* pUnit = (cUnitA*)pUnitA;
	cUnitA* pUnitNext = (cUnitA*)pUnit->pNext_Stack;
	cUnitA* pUnitPrev = (cUnitA*)pUnit->pPrev_Stack;

	//set the selected unit's stack links to NULL
	pUnit->pNext_Stack = NULL;
	pUnit->pPrev_Stack = NULL;

	//link pUnitNext and the MTILE to each other(sort of)
	//if there actually is a unit that was linked, set it's pointer back
		if (pUnitNext != NULL)
			pUnitNext->pPrev_Stack = NULL; //can't link back to the MTILE itself, it's not a cUnit!

	//if the prev of the unit was NULL, then it was at the front of the stack, and join the MTILE to the next tile
	if (pUnitPrev == NULL)
	{
	//if (pMap[Pos_square.index].pOccupant_Unit == pUnit)  //why is this wrong???
		pMap[Pos_square.index].pOccupant_Unit = (VOID*)pUnitNext;
	}

	//if it wasn't NULL, then join whatever was there to it
	if (pUnitPrev != NULL)
		pUnitPrev->pNext_Stack = (VOID*)pUnitNext;

	//remove the unit's IMTarget from the map
	pUnit->Pos_sqimtarget.Calc_Index(iWidth);
	if (pMap[pUnit->Pos_sqimtarget.index].wIMTargeted_Count > 0)
		pMap[pUnit->Pos_sqimtarget.index].wIMTargeted_Count-= 1;



	//if it was the only unit of that nation to set there, set the nation flag back to default 0
	if (pMap[pUnit->Pos_sqimtarget.index].wIMTargeted_Count == 0)
		pMap[pUnit->Pos_sqimtarget.index].bIMTargeted_Nation = -1;	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Unregister_Structure(SQCOORDSET Pos_square, VOID* pStructureA)
{
	//identical/reverse code to the register code(03.28.2003)
	cStructureA* pStructure = (cStructureA*)pStructureA;
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//unregister the Pos_primary position
	pMap[pStructure->Pos_primary.index].pOccupant_Structure = NULL;

	for (int c35 = 0; c35 < pStructure->iNumArrayEntries; c35++)
	{
			
		pMap[pStructure->Pos_array[c35].index].pOccupant_Structure = NULL;
	}

	//if the sector capital pointer matches this structure, clear it
	//disabled 09.20.2006
/*	if ( pCA->pSectorTable->Sectors[ pMap[Pos_square.index].iSector ]->pCapital == pStructure)
	{
		//DEBUG
		Write_Log("Capital Unregistered from sector");
		WriteLog_Enter();

		pCA->pSectorTable->Sectors[ pMap[Pos_square.index].iSector ]->pCapital = NULL;
		pCA->pSectorTable->Sectors[ pMap[Pos_square.index].iSector ]->iControl = -1;

	} */

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Read_To_Stop(char* LoadBuffer, char stopchar)
{
	Read_To_Stop(LoadBuffer, stopchar, TEMP_STD_STRLEN);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Read_To_Stop(char* LoadBuffer, char stopchar, int iBufferLimit)
{
	int chcount = 0;
	char inchar = 0;
	
	while (inchar != stopchar)
	{
		ReadFile(hMapFile, &inchar, 1, &dwNumBytesRead, NULL);
		if (inchar != stopchar)  
		if (inchar != ' ' || chcount != 0) //discard pre-text whitespace also
		{
			LoadBuffer[chcount] = inchar;
			//safeguard
			if (inchar == '~')
				return;

			chcount++;
			//safeguard
			if (chcount > iBufferLimit)
				chcount = 0;
		}

	}
	LoadBuffer[chcount] = 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//09.20.2006
void cGameWorldA::Assess()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int lcx;
	int lcy;

	for (lcx = 0; lcx < iWidth; lcx++)
	for (lcy = 0; lcy < iHeight; lcy++)
	{
		MTile* pTile = &pMap[ (lcy * iWidth) + lcx];
		//now clear
		pTile->iControl = -1;
		pTile->iControlLevel = -1;
		pTile->iPossessingStructID = -1;
	}

		MTile* pTile;

	//now go through each tile and assess controls
	for (lcy = 0; lcy < iHeight; lcy++)
	for (lcx = 0; lcx < iWidth; lcx++)
	{

		
		pTile = &pCA->pGameWorld->pMap[ lcy * iWidth + lcx ];
		
		//sweep away all IMtargets (to prevent errors from piling up... :-/
		pTile->wIMTargeted_Count = 0;
		pTile->bIMTargeted_Nation = -1;

		//monitor topology of storage tank networks
		//if this tile is a structure...
		//if this tile doesn't yet belong to a region, give it one


/*   (no longer grant territorial bonus to units (resource allocation (to where?) problem occurs) 10.19.2006
		//if a unit is here
		cUnitA* pUnit = (cUnitA*)pTile->pOccupant_Unit;
		if (pUnit != NULL)
		{
			//exert control over neighboring tiles (radius 2) with level 100 occupancy
			Possess(lcx, lcy, TEMP_CLAIMRADIUS_UNITS, pUnit->iControl, 100);
			//control its own tile with level 200 occupancy
			pTile->iControl = pUnit->iControl;
			pTile->iControlLevel = 200; //level 200 occupancy
		}
*/

		//if a structure is here
		cStructureA* pStruct = (cStructureA*)pTile->pOccupant_Structure;

		if (pStruct != NULL) //&& pStruct->bOn == TRUE)  //this doesn't work out that well
		if (pStruct->iType == TEMP_STYPE_RSC || pStruct->iType == TEMP_STYPE_OUTPOST)		
		{
		
			//make sure position equals Pos_Primary
			if (pStruct->Pos_primary.x == lcx && pStruct->Pos_primary.y == lcy)
			{
				//exert control over neighboring tiles (radius Y) with level 300 occupancy
				if (pStruct->iType == TEMP_STYPE_RSC)
				{
					//clear its resource collection array first
					ZeroMemory(pStruct->pResourceCollection, sizeof(double)*NUM_MATERIAL_TYPES);
					Possess(pStruct->iID, lcx, lcy, TEMP_CLAIMRADIUS_CAPITAL, pStruct->iControl, 300);
				}
				if (pStruct->iType == TEMP_STYPE_OUTPOST)
				{
					//clear its resource collection array first
										//clear its resource collection array first
					ZeroMemory(pStruct->pResourceCollection, sizeof(double)*NUM_MATERIAL_TYPES);
					Possess(pStruct->iID, lcx, lcy, TEMP_CLAIMRADIUS_OUTPOST, pStruct->iControl, 300);
				}

				//control its own tile with level 400 occupancy
				pTile->iControl = pStruct->iControl;
				pTile->iControlLevel = 400; //level 200 occupancy
				pTile->iPossessingStructID = pStruct->iID;
			}
			//if it is a structure, but isn'tprimary tile, exert with level 300 occupancy
				pTile->iControl = pStruct->iControl;
				pTile->iControlLevel = 300; //level 200 occupancy	
				pTile->iPossessingStructID = pStruct->iID;
		}


	}





	int Tiles[TEMP_DP_MAXPLAYERS]; 
	int Bonuses[TEMP_DP_MAXPLAYERS];
	ZeroMemory(Tiles, sizeof(int)*TEMP_DP_MAXPLAYERS);
	ZeroMemory(Bonuses, sizeof(int)*TEMP_DP_MAXPLAYERS);

	int iNumTiles = iWidth * iHeight;
	int iNumPossessTiles = 0; //the number of "non-ocean" tiles that we can possess (to be counted)

	cStructureA* pStruct;
	//now access and clear
	for (lcx = 0; lcx < iWidth; lcx++)
	for (lcy = 0; lcy < iHeight; lcy++)
	{
		//only check if we're not ocean
		MTile* pTile = &pCA->pGameWorld->pMap[ (lcy * iWidth) + lcx];

		if (pTile->iType != MAPTILE_OCEAN)
		{
			//if we're here, this tile is "possessable"
			iNumPossessTiles++;

			if (pTile->iControl >= 0)
			{	
				pStruct = pCA->pStructIDTable->pTable[pTile->iPossessingStructID]; 
				if (pStruct->iCollectionModifiedLast != pCA->pTimer->iCurrTick)
				{
					for (int lc3 = 0; lc3 < NUM_MATERIAL_TYPES; lc3++)
						pStruct->pResourceCollection[lc3] = 0;
					
					pStruct->iCollectionModifiedLast = pCA->pTimer->iCurrTick;
				}

				Tiles[pTile->iControl]++;

				for (int lc6 = 0; lc6 < NUM_MATERIAL_TYPES; lc6++)
					pStruct->pResourceCollection[lc6] += TileInfos[pTile->iType].ResourcePayout[lc6];
			}
		}

	}

	//now grant bonuses to nations
	for (int lc1 = 0; lc1 < TEMP_DP_MAXPLAYERS; lc1++)
	if (pCA->pNationManager->ppNations[lc1] != NULL)
	{
		//pCA->pNationManager->ppNations[lc1]->ddResources += Bonuses[lc1];
		pCA->pNationManager->ppNations[lc1]->iPossession = Tiles[lc1];
		pCA->pNationManager->ppNations[lc1]->ddPossession  = double(Tiles[lc1])/double(iNumPossessTiles);
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Possess(int iStructID, int iX_tile, int iY_tile, int iRad, int iControl, int iControlLevel)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iWidth = pCA->pGameWorld->iWidth;
	int iHeight = pCA->pGameWorld->iHeight;

	int iBound_XLow = iX_tile - iRad;
	if (iBound_XLow < 0)
		iBound_XLow = 0;
	
	int iBound_XHigh = iX_tile + iRad;
	if (iBound_XHigh >= (iWidth - 1))
		iBound_XHigh = iWidth - 2;

	int iBound_YLow = iY_tile - iRad;
	if (iBound_YLow < 0)
		iBound_YLow = 0;
	
	int iBound_YHigh = iY_tile + iRad;
	if (iBound_YHigh >= (iHeight - 1))
		iBound_YHigh = iHeight - 2;


	int iHigherManhattan = iRad * 1.41;  //sqrt(2)

	for (int cy = iBound_YLow; cy <= iBound_YHigh; cy++)
	for (int cx = iBound_XLow; cx <= iBound_XHigh; cx++)
	{
			int index = (cy * iWidth) + cx;
			MTile* pTile = &pCA->pGameWorld->pMap[ index];
		if ( abs(cy - iY_tile) + abs(cx-iX_tile) <= iHigherManhattan)
		{
			if (iControlLevel > pCA->pGameWorld->pMap[ (cy * iWidth) + cx].iControlLevel)
			{
				pTile->iControl = iControl;	
				pTile->iControlLevel  = iControlLevel;
				pTile->iPossessingStructID = iStructID;
			} else if ((iControlLevel == pCA->pGameWorld->pMap[ (cy * iWidth) + cx].iControlLevel))
			{
				 //if we're talking about different nations, then nobody gets it
				 if (pTile->iControl != iControl)
				 {
						pTile->iControlLevel = 1000;
						pTile->iControl = -1;
				 }
				 //if its the same nation, the structure with the lower ID gets it
				 if (pTile->iControl == iControl)
				 {	
						if (iStructID < pTile->iPossessingStructID)
						{
							pTile->iPossessingStructID = iStructID;
						}
				 }
			}
		}
	} 




}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::Reveal(int iX_tile, int iY_tile, int iRad, int iControl)
{

	if (iY_tile < 0)
		return;

	BYTE icheck = 1;
	icheck = icheck << iControl;

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iWidth = pCA->pGameWorld->iWidth;
	int iHeight = pCA->pGameWorld->iHeight;

	int iBound_XLow = iX_tile - iRad;
	if (iBound_XLow < 0)
		iBound_XLow = 0;
	
	int iBound_XHigh = iX_tile + iRad;
	if (iBound_XHigh >= (iWidth - 1))
		iBound_XHigh = iWidth - 2;

	int iBound_YLow = iY_tile - iRad;
	if (iBound_YLow < 0)
		iBound_YLow = 0;
	
	int iBound_YHigh = iY_tile + iRad;
	if (iBound_YHigh >= (iHeight - 1))
		iBound_YHigh = iHeight - 2;

	int iHigherManhattan = iRad * 1.41;  //sqrt(2)

	for (int cy = iBound_YLow; cy <= iBound_YHigh; cy++)
	for (int cx = iBound_XLow; cx <= iBound_XHigh; cx++)
	if ( abs(cy - iY_tile) + abs(cx-iX_tile) <= iHigherManhattan)
		{
			MTILE* pTile = &pCA->pGameWorld->pMap[ (cy * iWidth) + cx];

			pTile->barrRevealed = pTile->barrRevealed | icheck;
		}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
SQCOORDSET cGameWorldA::Select_Scouting_Location(int iControl)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	SQCOORDSET SqRet;
	BYTE icheck = 1;
	icheck = icheck << iControl;

	SqRet.x = -1;
	SqRet.y = -1;
	SqRet.Calc_Index(iWidth);

	for (int lc1 = 0; lc1 < 2000; lc1++)
	{
		//we do two rands because RAND_MAX is insufficient to cover the entire map
		SqRet.x = pCA->pRandGen->rand2() % iWidth;
		SqRet.y = pCA->pRandGen->rand2() % iHeight;
		SqRet.Calc_Index(iWidth);
		if ( (pMap[SqRet.index].barrRevealed & icheck) == 0) 
			return SqRet;
	}

	//failed to find
	return SqRet;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameWorldA::RebuildMTLinkLists()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//for all maptiles, clear all thel inks
	for (int lc1 = 0; lc1 < (iWidth * iHeight); lc1++)
	{
		pMap[lc1].pSectorList_Next = NULL;
		pMap[lc1].pSectorList_Prev = NULL;
	}
	//for all sectors, exit and init MTLists
	for (int lc2 = 0; lc2 < MAX_SECTORS; lc2++)
	{	
		if (pCA->pSectorTable->Sectors[lc2] != NULL)
		{
			pCA->pSectorTable->Sectors[lc2]->Exit_MTList();
			pCA->pSectorTable->Sectors[lc2]->Init_MTList();
		}
	}
	//now go through whole map and add tiles to the appropriate sectors
	for (int lc3 = 0; lc3 < (iWidth * iHeight); lc3++)
	{
		MTILE* pTile = &pMap[lc3];
		pCA->pSectorTable->Sectors[pTile->iSector]->Add_MTile(pTile);
	}

	for (int lc4 =0 ; lc4 < MAX_SECTORS; lc4++)
	{
		cSector* pSector = pCA->pSectorTable->Sectors[lc4];

		if (pSector != NULL)
		{
			//DEBUG
			char NText[50];
			itoa(pSector->iID, NText, 10);
			pCA->pDD_Display->Write_Log("Sector: ");
			pCA->pDD_Display->Write_Log(NText);
			itoa(pSector->iNumMapTiles, NText, 10);
			pCA->pDD_Display->Write_Log(" Num Tiles: ");
			pCA->pDD_Display->Write_Log(NText);
			pCA->pDD_Display->WriteLog_Enter();
		}
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
