#include "classes.h"
//*********************************************************************************************//
//*********************************************************************************************//
void cConfig::Init(void* pCA)
{

	Init_LogFile();

	pCAccess = pCA;

	iTempStore_DDRenderMode = DDRM_ALPHA; //this uses more memory and is slower but it actually has better compatibility
										  //than DDRM_NORMAL
	iTempStore_MMUpdateTime = 5; //fairly good response time without slowing, at least the test system down
	//
	bTempStore_TimerThread = FALSE; //don't use timer thread if possible
	bTempStore_ForceCore0 = FALSE; //don't force Core 0 if possible


	bTextZero = FALSE;

	//when cDD_Display initializes, it will get the render mode from here and from there on store it in cDD_Display

}
//*********************************************************************************************//
//*********************************************************************************************//
void cConfig::Load_Data(char* LoadFileName)
{


//DEBUG
Write_Log(LoadFileName);
Write_Log(" Beginning");
WriteLog_Enter();

	hInfile = CreateFile(LoadFileName, 		
		GENERIC_READ, 
		FILE_SHARE_READ, 
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hInfile == INVALID_HANDLE_VALUE)
	{
//DEBUG
Write_Log(LoadFileName);
Write_Log(" INVALID FILE HANDLE");
WriteLog_Enter();

		return;
	}

//DEBUG
Write_Log(LoadFileName);
Write_Log(" Passed createfile");
WriteLog_Enter();

	int iIDBlock = CONFIG_IDBLOCK_NOTHING;
	char InBuffer[TEMP_STD_STRLEN];
		ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
	char Inchar = ' ';

	DWORD dwNumBytesRead;

	char NText[50];

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//
	while ( InBuffer[0] != '~')
	{

		InBuffer[0] = 0;
		InBuffer[1] = 0;
		ReadFile( hInfile, InBuffer, 1, &dwNumBytesRead, NULL);

		iIDBlock = CONFIG_IDBLOCK_NOTHING;

		//look for the IDBlock
		if (InBuffer[0] == '[')
		{
			//read to find out what the block identifier is
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ']');
			
			//figure out what it was

			//the LGRAPHIC (load graphic) block
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'G' &&
				InBuffer[2] == 'R' &&
				InBuffer[3] == 'A' &&
				InBuffer[4] == 'P' &&
				InBuffer[5] == 'H' &&
				InBuffer[6] == 'I' &&
				InBuffer[7] == 'C' &&
				InBuffer[8] == 0)
				{
					iIDBlock = CONFIG_IDBLOCK_LGRAPHIC;				
				}
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'S' &&
				InBuffer[2] == 'O' &&
				InBuffer[3] == 'U' &&
				InBuffer[4] == 'N' &&
				InBuffer[5] == 'D' &&
				InBuffer[6] == 0)
				{
					iIDBlock = CONFIG_IDBLOCK_LSOUND;				
				}
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'S' &&
				InBuffer[2] == 'O' &&
				InBuffer[3] == 'U' &&
				InBuffer[4] == 'N' &&
				InBuffer[5] == 'D' &&
				InBuffer[6] == 0)
				{
					iIDBlock = CONFIG_IDBLOCK_LSOUND;				
				}
			if (InBuffer[0] == 'A' &&
				InBuffer[1] == 'C' &&
				InBuffer[2] == 'K' &&
				InBuffer[3] == '_' &&
				InBuffer[4] == 'S' &&
				InBuffer[5] == 0)
				{
					iIDBlock = CONFIG_IDBLOCK_ACK_STRUCT;				
				}
			if (InBuffer[0] == 'T' &&
				InBuffer[1] == 'E' &&
				InBuffer[2] == 'X' &&
				InBuffer[3] == 'T' &&
				InBuffer[4] == 'Z' &&
				InBuffer[5] == 'E' &&
				InBuffer[6] == 'R' &&
				InBuffer[7] == 'O' &&
				InBuffer[8] == 0)
				{
					iIDBlock = CONFIG_IDBLOCK_TEXTZERO;
				}
			if (InBuffer[0] == 'A' &&
				InBuffer[1] == 'C' &&
				InBuffer[2] == 'K' &&
				InBuffer[3] == '_' &&
				InBuffer[4] == 'U' &&
				InBuffer[5] == 0)
				{
					iIDBlock = CONFIG_IDBLOCK_ACK_UNIT;				
				}
			if (InBuffer[0] == 'A' &&
				InBuffer[1] == 'C' &&
				InBuffer[2] == 'K' &&
				InBuffer[3] == '_' &&
				InBuffer[4] == 'M' &&
				InBuffer[5] == 'E' &&
				InBuffer[6] == 'N' &&
				InBuffer[7] == 'T' &&
				InBuffer[8] == 'A' &&
				InBuffer[9] == 'T' &&
				InBuffer[10] == 0)
				{
					iIDBlock = CONFIG_IDBLOCK_ACK_MENTAT;				
				}
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'T' &&
				InBuffer[2] == 'C' &&
				InBuffer[3] == 'M' &&
				InBuffer[4] == 'P' &&
				InBuffer[5] == '_' &&
				InBuffer[6] == 'F' &&
				InBuffer[7] == 'R' &&
				InBuffer[8] == 0)
				{
					iIDBlock = CONFIG_IDBLOCK_LTCMP_FR;
				}
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'T' &&
				InBuffer[2] == 'C' &&
				InBuffer[3] == 'M' &&
				InBuffer[4] == 'P' &&
				InBuffer[5] == '_' &&
				InBuffer[6] == 'E' &&
				InBuffer[7] == 'N' &&
				InBuffer[8] == 0)
				{
					iIDBlock = CONFIG_IDBLOCK_LTCMP_EN;
				}
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'T' &&
				InBuffer[2] == 'C' &&
				InBuffer[3] == 'M' &&
				InBuffer[4] == 'P' &&
				InBuffer[5] == '_' &&
				InBuffer[6] == 'W' &&
				InBuffer[7] == 'P' &&
				InBuffer[8] == 0)
				{
					iIDBlock = CONFIG_IDBLOCK_LTCMP_WP;
				}
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'T' &&
				InBuffer[2] == 'C' &&
				InBuffer[3] == 'M' &&
				InBuffer[4] == 'P' &&
				InBuffer[5] == '_' &&
				InBuffer[6] == 'T' &&
				InBuffer[7] == 'L' &&
				InBuffer[8] == 0)
				{
					iIDBlock = CONFIG_IDBLOCK_LTCMP_TL;
				}
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'T' &&
				InBuffer[2] == 'C' &&
				InBuffer[3] == 'M' &&
				InBuffer[4] == 'P' &&
				InBuffer[5] == '_' &&
				InBuffer[6] == 'D' &&
				InBuffer[7] == 'F' &&
				InBuffer[8] == 0)
				{
					iIDBlock = CONFIG_IDBLOCK_LTCMP_DF;
				}

			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'T' &&
				InBuffer[2] == 'C' &&
				InBuffer[3] == 'M' &&
				InBuffer[4] == 'P' &&
				InBuffer[5] == '_' &&
				InBuffer[6] == 'S' &&
				InBuffer[7] == 'T' &&
				InBuffer[8] == 0)
				{
				    iIDBlock = CONFIG_IDBLOCK_LTCMP_ST;
				}
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'T' &&
				InBuffer[2] == 'C' &&
				InBuffer[3] == 'M' &&
				InBuffer[4] == 'P' &&
				InBuffer[5] == '_' &&
				InBuffer[6] == 'G' &&
				InBuffer[7] == 'R' &&
				InBuffer[8] == 0)          //this block loads the new source graphics block information for a component type (07.12.2004)
			{
					iIDBlock = CONFIG_IDBLOCK_LTCMP_GR;
			}
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'T' &&
				InBuffer[2] == 'C' &&
				InBuffer[3] == 'M' &&
				InBuffer[4] == 'P' &&
				InBuffer[5] == '_' &&
				InBuffer[6] == 'M' &&
				InBuffer[7] == 'A' &&
				InBuffer[8] == 'T' &&
				InBuffer[9] == 0)          //this block loads the new source graphics block information for a component type (07.12.2004)
			{
					iIDBlock = CONFIG_IDBLOCK_LTCMP_MAT;
			}	
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'T' &&
				InBuffer[2] == 'P' &&
				InBuffer[3] == 'R' &&
				InBuffer[4] == 'J' &&
				InBuffer[5] == '_' &&
				InBuffer[6] == 'G' &&
				InBuffer[7] == 'R' &&
				InBuffer[8] == 0)          //LTCMP_GR's projectile counterpart (07.20.2004)
			{
					iIDBlock = CONFIG_IDBLOCK_LTPRJ_GR;
			}
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'T' &&
				InBuffer[2] == 'P' &&
				InBuffer[3] == 'R' &&
				InBuffer[4] == 'J' &&
				InBuffer[5] == 0)          //this block loads the new source graphics block information for a component type (07.12.2004)
			{
					iIDBlock = CONFIG_IDBLOCK_LTPRJ;
			}
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'T' &&
				InBuffer[2] == 'U' &&
				InBuffer[3] == 'N' &&
				InBuffer[4] == 'I' &&
				InBuffer[5] == 'T' &&
				InBuffer[6] == 0)          //this block loads the new source graphics block information for a component type (07.12.2004)
			{
					iIDBlock = CONFIG_IDBLOCK_LTUNIT;
			}
			if (InBuffer[0] == 'D' &&
				InBuffer[1] == 'D' &&
				InBuffer[2] == 'R' &&
				InBuffer[3] == 'M' &&
				InBuffer[4] == 0)
			{
					iIDBlock = CONFIG_IDBLOCK_DDRM;
			}
			if (InBuffer[0] == 'L' &&
				InBuffer[1] == 'T' &&
				InBuffer[2] == 'C' &&
				InBuffer[3] == 'M' &&
				InBuffer[4] == 'P' &&
				InBuffer[5] == '_' &&
				InBuffer[6] == 'G' &&
				InBuffer[7] == 'R' &&
				InBuffer[8] == '_' &&
				InBuffer[9] == 'S' &&
				InBuffer[10] == 'T' &&
				InBuffer[11] == 'A' &&
				InBuffer[12] == 0)
			{
					iIDBlock = CONFIG_IDBLOCK_LTCMP_GR_STA;
			}
			if (InBuffer[0] == 'M' &&
				InBuffer[1] == 'M' &&
				InBuffer[2] == 'U' &&
				InBuffer[3] == 'P' &&
				InBuffer[4] == 'D' &&
				InBuffer[5] == 'A' &&
				InBuffer[6] == 'T' &&
				InBuffer[7] == 'E' &&
				InBuffer[8] == 'T' &&
				InBuffer[9] == 0)
			{
					iIDBlock = CONFIG_IDBLOCK_MMUPDATET;
			}
			if (InBuffer[0] == 'S' &&
				InBuffer[1] == 'E' &&
				InBuffer[2] == 'T' &&
				InBuffer[3] == 'R' &&
				InBuffer[4] == 'E' &&
				InBuffer[5] == 'S' &&
				InBuffer[6] == 0)
			{
					iIDBlock = CONFIG_IDBLOCK_SETRES;
			}
			if (InBuffer[0] == 'T' &&
				InBuffer[1] == 'I' &&
				InBuffer[2] == 'M' &&
				InBuffer[3] == 'E' &&
				InBuffer[4] == 'R' &&
				InBuffer[5] == 'T' &&
				InBuffer[6] == 'H' &&
				InBuffer[7] == 'R' &&
				InBuffer[8] == 'E' &&
				InBuffer[9] == 'A' &&
				InBuffer[10] == 'D' &&
				InBuffer[11] == 0)
			{
					iIDBlock = CONFIG_IDBLOCK_TIMERTHREAD;
			}
			if (InBuffer[0] == 'C' &&
				InBuffer[1] == 'O' &&
				InBuffer[2] == 'R' &&
				InBuffer[3] == 'E' &&
				InBuffer[4] == '0' &&
				InBuffer[5] == 0)
			{
					iIDBlock = CONFIG_IDBLOCK_CORE0;
			}

		}



		//do something depending on what those blocks were
		if (iIDBlock == CONFIG_IDBLOCK_DDRM)
		if (pCA->pDD_Display->iDDRenderMode == VAL_INVALID) //the mode can only be changed on startup
		{
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');

			//just read in the text, NORMAL or ALPHA
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}'); //first and only parameter

			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
			{
				iTempStore_DDRenderMode = DDRM_NORMAL;
			}
			if (InBuffer[0] == 'A' || InBuffer[0] == 'a')
			{
				iTempStore_DDRenderMode = DDRM_ALPHA;
			}
			if (InBuffer[0] == 'S' || InBuffer[0] == 's')
			{
				iTempStore_DDRenderMode = DDRM_SYSMEM;
			}

		}
		if (iIDBlock == CONFIG_IDBLOCK_TEXTZERO)
		{
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			//read in the target index
			bTextZero = FALSE;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');

			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bTextZero = TRUE;
			
		}
		if (iIDBlock == CONFIG_IDBLOCK_CORE0)
		{
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			//read in the target index
			BOOL bForceCore0 = FALSE;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');

			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bForceCore0 = TRUE;
			//load
			bTempStore_ForceCore0 = bForceCore0;
		}
		if (iIDBlock == CONFIG_IDBLOCK_TIMERTHREAD)
		{
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			//read in the target index
			BOOL bTimerThread = FALSE;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');

			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bTimerThread = TRUE;
			//load
			bTempStore_TimerThread = bTimerThread;
		}
		if (iIDBlock == CONFIG_IDBLOCK_LSOUND)
		{
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			//read in the target index
			int iIndex = 0;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iIndex = atoi(InBuffer);	//treat the index as starting from the highest predefined image

			//read in the sound file name
			char namebuffer[TEMP_STD_STRLEN];
			ZeroMemory(namebuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');
			//strcopy
			for (int c1 = 0; c1 < strlen(InBuffer); c1++)
				namebuffer[c1] = InBuffer[c1];

			BOOL bMIDIflag = FALSE;

			//check the last three letters for a MIDI extension 
			if (InBuffer[strlen(InBuffer)-1] == 'D' || InBuffer[strlen(InBuffer)-1] == 'd')
			if (InBuffer[strlen(InBuffer)-2] == 'I' || InBuffer[strlen(InBuffer)-2] == 'i')
			if (InBuffer[strlen(InBuffer)-3] == 'M' || InBuffer[strlen(InBuffer)-3] == 'm')
				bMIDIflag = TRUE;

			//load

			pCA->pSounds[iIndex]->Init(pCA, NULL, NULL, namebuffer, bMIDIflag);
		}
		if (iIDBlock == CONFIG_IDBLOCK_ACK_UNIT || iIDBlock == CONFIG_IDBLOCK_ACK_STRUCT || iIDBlock == CONFIG_IDBLOCK_ACK_MENTAT)
		{
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			//read in unit type
			int iType = 0;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iType = atoi(InBuffer);

			//read in the acknowledgement type
			int iAckType = 0;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iAckType = atoi(InBuffer);

			//read in the personality index
			int iPersonality = 0;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iPersonality = atoi(InBuffer);

			//read in the cycle index
			int iCycle = 0;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iCycle = atoi(InBuffer);

			//read in the soundseg to get
			int iSoundSeg = 0;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iSoundSeg = atoi(InBuffer);

			//read in the acknowledgement text (will accept commas in this case)
			char textbuffer[TEMP_STD_STRLEN];
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');

			//load directly
			 if  (iIDBlock == CONFIG_IDBLOCK_ACK_UNIT)
				pCA->pAckTable->Set_Ack(FALSE, iType, iAckType, iPersonality, iCycle, iSoundSeg, InBuffer, strlen(InBuffer));
			 if  (iIDBlock == CONFIG_IDBLOCK_ACK_STRUCT)
				pCA->pAckTable->Set_Ack(TRUE, iType, iAckType, iPersonality, iCycle, iSoundSeg, InBuffer, strlen(InBuffer));
			 if (iIDBlock == CONFIG_IDBLOCK_ACK_MENTAT)
				pCA->pAckTable->Set_Ack(FALSE, 0, iAckType, iPersonality, iCycle, iSoundSeg, InBuffer, strlen(InBuffer));

			 
		}
		if (iIDBlock == CONFIG_IDBLOCK_LGRAPHIC)
		{
//DEBUG
Write_Log("LGRAPHIC block encountered");
WriteLog_Enter();
			int iTargetImageSurf  = -1;
			char ImageNameBuffer[TEMP_STD_STRLEN];
			ZeroMemory(ImageNameBuffer, sizeof(char) * TEMP_STD_STRLEN);
			BOOL bSysMemFlag = FALSE;
			BOOL bExBufferKey = FALSE;  //"alpha key" channel bit
			BOOL bExBufferMark = FALSE; //"mark key" channel bit
			DWORD dwColorKey;
			BOOL bForceDimFlag = FALSE;
			int iForceX;
			int iForceY;
			int iSysMemScheme = pCA->pDD_Display->sysmem_flag_normal;

			//read to the open bracket
			Read_To_Stop(InBuffer, '{');

			//read in target image surf index
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iTargetImageSurf = atoi(InBuffer);	//treat the index as starting from the highest predefined image

			//read in image file name
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			//strcopy
			strcpy(ImageNameBuffer, InBuffer);
			
			//read in "use system memory" flag
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			//if the buffer reads first letter 'T', enable the flag
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bSysMemFlag = TRUE;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				bSysMemFlag = FALSE;
			if (InBuffer[0] == 'A' || InBuffer[0] == 'a')
				bSysMemFlag = pCA->pDD_Display->sysmem_flag_alpharender;
			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
				bSysMemFlag = pCA->pDD_Display->sysmem_flag_normal;
			if (InBuffer[0] == 'M' || InBuffer[0] == 'm')
				bSysMemFlag = pCA->pDD_Display->sysmem_flag_menu;
			//

			//read in "create exbuffer key channel
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			//if the buffer reads first letter 'T', enable the flag
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bExBufferKey = TRUE;

			//read in "create exbuffer mark channel
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			//if the buffer reads first letter 'T', enable the flag
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bExBufferMark = TRUE;

			//read in color key
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			//get the colorkey (read the text "WHITE", "BLACK" ... "RED" etc.)
			if (InBuffer[0] == 'W' || InBuffer[0] == 'w')
				dwColorKey = pCA->pColorTable->White;
			if (InBuffer[0] == 'B' || InBuffer[0] == 'b')
				dwColorKey = pCA->pColorTable->Black;
			if (InBuffer[0] == 'R' || InBuffer[0] == 'r')
				dwColorKey = pCA->pColorTable->Red;

			//read in force-dimension information
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			//if the buffer reads first letter 'T', enable the flag
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bForceDimFlag = TRUE;

			//read in the iForce X and Y
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iForceX = atoi(InBuffer);

			//
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');
			iForceY = atoi(InBuffer);

			//determine what to put in for exbufferflags
			BYTE bExBufferFlags = bExBufferKey | bExBufferMark;


BOOL bSysMem = FALSE;
if (bSysMemFlag + pCA->pDD_Display->iDDRenderMode > 0)
{
	bSysMem = TRUE;
	pCA->pDD_Display->Write_Log(" SysMem Invoked.");
	pCA->pDD_Display->WriteLog_Enter();
}

			//now do it!
			pCA->pImages[iTargetImageSurf]->Init( pCAccess, iTargetImageSurf, pCA->pDD_Display->pDD7, ImageNameBuffer, bSysMem, bExBufferFlags, dwColorKey, bForceDimFlag, iForceX, iForceY);

		}
		if (iIDBlock == CONFIG_IDBLOCK_LTCMP_MAT)
		{
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');		
			//read in the component type
			int iType = -1;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iType = atoi(InBuffer);

			//read in the material type
			int iMaterialType = -1;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iMaterialType = atoi(InBuffer);
			char CheckChar = toupper(InBuffer[0]);
			int iMaterialTypeCheck = pCA->pMaterialInfoTable->FindType_ByRefChar(CheckChar);

			if (iMaterialType >= 0 && iMaterialType < NUM_MATERIAL_TYPES)
				iMaterialType = iMaterialTypeCheck;
			//read in the material amount
			double ddAmount = -1;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');
			ddAmount = atof(InBuffer);

			//now add
			if (iMaterialType >= 0 && iMaterialType < NUM_MATERIAL_TYPES)
			pCA->pCmpInfoTable->pCmpTable[iType]->MaterialCost.Materials[iMaterialType] = ddAmount;

		}
		if (iIDBlock == CONFIG_IDBLOCK_LTCMP_GR)
		{
//DEBUG
Write_Log("LTCMP_GR block encountered");
WriteLog_Enter();
			
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			//load a single frame of animation to source graphics

			//read in the target component type
			int iType = -1;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iType = atoi(InBuffer);

			//read in the target ZLevel
			int iZLevel = 1;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iZLevel = atoi(InBuffer);
		
			cCmpInfo* pInfo = pCA->pCmpInfoTable->pCmpTable[iType];

			//adjust this based on whether it's a frame or anything else
			if (pInfo->iClass != CMPCLASS_FRAME)
				iZLevel += ZLEVEL_MIDBASE;

//from here we begin reading graphic blocks
			InBuffer[0] = ' ';
			while (InBuffer[0] != '}')
			{
				ReadFile( hInfile, InBuffer, 1, &dwNumBytesRead, NULL);
			
				//check for open bracket
				if ( InBuffer[0] == '<')
				{
					//read in the target Set index
					int iSetI = 0;
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iSetI = atoi(InBuffer);

					//read in the target Cycle index
					int iCycleI = 0;
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iCycleI = atoi(InBuffer);


					//read in the target Frame index
					int iFrameI = 0;
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iFrameI = atoi(InBuffer);

					//finally read in the source isurf number
					int iSurfSource = 1;
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iSurfSource = atoi(InBuffer);

					//read in the four rect numbers
					int iLeft = 0;
					int iRight = TILE_SIZE;
					int iTop = 0;
					int iBottom = TILE_SIZE;

					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iLeft = atoi(InBuffer);
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iRight = atoi(InBuffer);
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iTop = atoi(InBuffer);
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ','); //last entry
					iBottom = atoi(InBuffer);

					//read in the duration
					int iDuration = 10; //default
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, '>'); //last entry
					iDuration = atoi(InBuffer);
					
					if (iDuration < 0) //protect against negative duration
						iDuration = 10;

					//finally input the information
					//make sure the target type is not null

					//specifying the zlevel is tricky, for a frame component it's the absolute zlevel,
					//for  non-frame components is the relative zlevel vs. the MIDBASE
					if (pInfo != NULL)
					{
							if (pInfo->DrawSchemes[iZLevel].bValid == TRUE)
							{
								pInfo->DrawSchemes[iZLevel].SourceGraphics.Add_Entry(iSetI, iCycleI, iFrameI, iSurfSource, iLeft, iRight, iTop, iBottom, iDuration);
							}
					}
				}  //end read <> block

			}  //end read {} 

			//order update of cSrcGraphicsInfo 
			pInfo->DrawSchemes[iZLevel].SourceGraphics.Update();

		} // end LTCMP_GR block
		if (iIDBlock == CONFIG_IDBLOCK_LTCMP_GR_STA)
		{
//DEBUG
Write_Log("Encountered LTCMP_GR_STA block");
WriteLog_Enter();
			//loads "default"A graphics scheme
			//read to open bracket
			Read_To_Stop(InBuffer, '{');

			//read in component type
			int iType = VAL_INVALID; //Default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(InBuffer, ',');
			iType = atoi(InBuffer);	

	//DEBUG
Write_Log("Type: ");
itoa(iType, NText, 10);
Write_Log(NText);
WriteLog_Enter();

			//read in ZLevel
			int iZLevel = VAL_INVALID; //Default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(InBuffer, ',');
			iZLevel = atoi(InBuffer);	

			cCmpInfo* pInfo = pCA->pCmpInfoTable->pCmpTable[iType];	
			//adjust this based on whether it's a frame or anything else
			if (pInfo->iClass != CMPCLASS_FRAME)
				iZLevel += ZLEVEL_MIDBASE;

			int iDirections = pInfo->DrawSchemes[iZLevel].iRotateFrames;


			//read in Directions
			/* //Disabled! (can already get it elsewhere)
			int iDirections = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(InBuffer, ',');
			iDirections = atoi(InBuffer);
			*/

			//read in iXFramesize
			int iXFrameSize = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(InBuffer, ',');
			iXFrameSize = atoi(InBuffer);
			
			//read in iYFramesize
			int iYFrameSize = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(InBuffer, ',');
			iYFrameSize = atoi(InBuffer);

			//read in source source graphic index
			int iSurfIndex = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(InBuffer, '}');
			iSurfIndex = atoi(InBuffer);

//DEBUG
Write_Log("ZLevel: ");
itoa(iZLevel, NText, 10);
Write_Log(NText);
WriteLog_Enter();

				if (pInfo != NULL)
				{

						if (pInfo->DrawSchemes[iZLevel].bValid == TRUE)
						{
//DEBUG
Write_Log("Adding Graphics: ");
itoa(iZLevel, NText, 10);
Write_Log(NText);
Write_Log("\t");
Write_Log("Index: ");
itoa(iSurfIndex, NText, 10);
Write_Log(NText);
Write_Log("\t");
Write_Log("Type: ");
itoa(iType, NText, 10);

Write_Log(NText);

WriteLog_Enter();
							pInfo->DrawSchemes[iZLevel].SourceGraphics.Setup_Graphics_DefaultA(iDirections, iXFrameSize, iYFrameSize, iSurfIndex);
						}
				}

		}
		//here goes (the incredible complex load frame type routine)
		if (iIDBlock == CONFIG_IDBLOCK_LTCMP_FR)
		{
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			int iCmpType = -1;
			int iSize = -1;
			int iMass = -1;
			double ddResourceCost = -1;
			double ddSupportCost = -1;
			int iIntegrity = -1;
			float fAccelBonus = -1;
			float fDecelBonus = -1;

			//read in the component type 
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iCmpType = atoi(InBuffer);

			//read in the name
			char TypeName[TEMP_STD_STRLEN];
			ZeroMemory(TypeName, sizeof(char)*TEMP_STD_STRLEN);
			//we might as well read in the name directly to the NameBuffer
			Read_To_Stop(TypeName, ',');

			//read in the size
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iSize = atoi(InBuffer);

			//read in the mass
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iMass = atoi(InBuffer);

			//read in the resource cost
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			ddResourceCost = atof(InBuffer);

			//read in the support cost
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			ddSupportCost = atof(InBuffer);

			//read in the bMarkFlag
			BOOL bMarkFlag = FALSE; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bMarkFlag = TRUE;

			//read in the integrity
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iIntegrity = atoi(InBuffer);

			int iNumOffsets = 0;
			//read in the number of offsets
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iNumOffsets = atoi(InBuffer);

			//read in the accel bonus
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fAccelBonus = atof(InBuffer);

			//read in the decel bonus
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fDecelBonus = atof(InBuffer);

			//read in the capacity
			int iCapacity = 0; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iCapacity = atoi(InBuffer);

			//information for each of the number of specified offsets is in terms of sets of five parameters
			//1. the starting index
			//2. the zlevel
			//3. the "base" z level
			//4. specification of whether we are using the standard offset array group or a custom offset array group
			//5. specification of which of the arrays in the group we are using

			//allocate an array for the data (5 is the number of offset spots that the addtype_simple function takes)
			//so the number we allocate is whichever is larger
			int iNumSlots = 5;
			if (iNumOffsets > 5)
				iNumSlots = iNumOffsets;

			int* pStartingIndexes = new int[iNumSlots]; ZeroMemory(pStartingIndexes, sizeof(int)*iNumSlots);
			int* pZLevels = new int[iNumSlots]; ZeroMemory(pZLevels, sizeof(int)*iNumSlots);
			int* pZBaseLevels = new int[iNumSlots]; ZeroMemory(pZBaseLevels, sizeof(int)*iNumSlots);
			char* pArrayGroups = new char[iNumSlots]; ZeroMemory(pArrayGroups, sizeof(char)*iNumSlots);
			int* pArrayGroupIndexes = new int[iNumSlots]; ZeroMemory(pArrayGroupIndexes, sizeof(int)*iNumSlots);
			CMPDRAWOFFSETARRAY** ppOffsetArrays = new CMPDRAWOFFSETARRAY*[iNumSlots]; ZeroMemory( ppOffsetArrays, sizeof(CMPDRAWOFFSETARRAY*) * iNumSlots);

			for (int c995 = 0; c995 < iNumOffsets; c995++)
			{	
				//read in the Offset Array Group
				ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
				Read_To_Stop(InBuffer, ',');
				pArrayGroups[c995] = 'S';
				if (InBuffer[0] == 'S') //standard
				{
					pArrayGroups[c995] = 'S';
					if (InBuffer[8] == '8')  //switch to standard 8 if necessary
						pArrayGroups[c995] = '8';
				}
				if (InBuffer[0] == 'C') //custom
					pArrayGroups[c995] = 'C';
				
				//read in the index in the offset array group
				ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
				Read_To_Stop(InBuffer, ',');
				pArrayGroupIndexes[c995] = atoi(InBuffer);

				//right now, for expediency, both offset groups map to the 'standard' group
				if (pArrayGroups[c995] == 'S' || pArrayGroups[c995] == 'C')
				{
					if (pArrayGroupIndexes[c995] < 40)
						ppOffsetArrays[c995] = &pCA->pCmpOffsetTable->StdDist_From_Center[pArrayGroupIndexes[c995]];
							else
						ppOffsetArrays[c995] = &pCA->pCmpOffsetTable->StdDist_From_Center[39];
				}
				//standard8
				if (pArrayGroups[c995] == '8')
				{
					if (pArrayGroupIndexes[c995] < 40)
						ppOffsetArrays[c995] = &pCA->pCmpOffsetTable->StdDist_From_Center8[pArrayGroupIndexes[c995]];
							else
						ppOffsetArrays[c995] = &pCA->pCmpOffsetTable->StdDist_From_Center8[39];
				}
				//read in the starting index
				ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
				Read_To_Stop(InBuffer, ',');
				pStartingIndexes[c995] = atoi(InBuffer);
				
				//read in the zlevel
				ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
				Read_To_Stop(InBuffer, ',');
				pZLevels[c995] = atoi(InBuffer);

				//read in the zlevel base
				ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
				Read_To_Stop(InBuffer, ',');
				pZBaseLevels[c995] = atoi(InBuffer);

			}


			float fTrav_Grassland = 0;
			float fTrav_Ocean = 0;
			float fTrav_Mountain = 0;
			float fTrav_Hill = 0;
			float fTrav_River = 0;
			float fTrav_Forest = 0;
			float fTrav_Air = 0;

			//get terrain data
			//read in the index in the offset array group
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fTrav_Grassland = atof(InBuffer);
//DEBUG
Write_Log(TypeName);
Write_Log(" ");
Write_Log(InBuffer);
WriteLog_Enter();

			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fTrav_Ocean = atof(InBuffer);

			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fTrav_Mountain = atof(InBuffer);

			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fTrav_Hill = atof(InBuffer);

			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fTrav_River = atof(InBuffer);

			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fTrav_Forest = atof(InBuffer);

			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fTrav_Air = atof(InBuffer);

			//get extmove characteristics
			int iMoveExtCharacteristics = -1;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iMoveExtCharacteristics = atoi(InBuffer);

			//get z0 and z1 characteristics
			//for each zlevel, we're getting seven ints and a boolean
			//get bDrawCharacteristics
			int iDrawCharacteristics_z0 = CMPDRAW_DRAWNORMAL;  //default
			//bdraw is just DRAWNORMAL, SINGLEFRAME, NODRAW, so check the first letter of the entry
			//
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'D' || InBuffer[0] == 'd')
				iDrawCharacteristics_z0 = CMPDRAW_DRAWNORMAL;
			if (InBuffer[0] == 'S' || InBuffer[0] == 's')
				iDrawCharacteristics_z0 = CMPDRAW_SINGLEFRAME; //singleframe no longer means anything, as long as it's not NODRAW
			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
				iDrawCharacteristics_z0 = CMPDRAW_NODRAW;

			//get bDirCharacteristics_z0 (has two settings, lock and free)
			int iDirCharacteristics_z0 = CMPDIR_LOCK; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');

			if (InBuffer[0] == 'L' || InBuffer[0] == 'l')
				iDirCharacteristics_z0 = CMPDIR_LOCK;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				iDirCharacteristics_z0 = CMPDIR_FREE;

			//get iRotateFrames
			int iRotateFrames_z0 = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iRotateFrames_z0 = atoi(InBuffer);

			//get value parameter
			BOOL bValid_z0 = TRUE; //default (we run into problems, I think, if it isn't true)
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bValid_z0 = TRUE;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				bValid_z0 = FALSE;

			//get z1 stuff
			//get bDrawCharacteristics
			int iDrawCharacteristics_z1 = CMPDRAW_NODRAW;  //default
			//bdraw is just DRAWNORMAL, SINGLEFRAME, NODRAW, so check the first letter of the entry
			//
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'D' || InBuffer[0] == 'd')
				iDrawCharacteristics_z1 = CMPDRAW_DRAWNORMAL;
			if (InBuffer[0] == 'S' || InBuffer[0] == 's')
				iDrawCharacteristics_z1 = CMPDRAW_SINGLEFRAME;
			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
				iDrawCharacteristics_z1 = CMPDRAW_NODRAW;

			//get bDirCharacteristics_z1 (has two settings, lock and free)
			int iDirCharacteristics_z1 = CMPDIR_LOCK; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');

			if (InBuffer[0] == 'L' || InBuffer[0] == 'l')
				iDirCharacteristics_z1 = CMPDIR_LOCK;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				iDirCharacteristics_z1 = CMPDIR_FREE;

			//get iRotateFrames
			int iRotateFrames_z1 = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iRotateFrames_z1 = atoi(InBuffer);

			//get value parameter
			BOOL bValid_z1 = TRUE; //default (we run into problems, I think, if it isn't true)
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');   //this is the last parameter, read to the close bracket '}'
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bValid_z1 = TRUE;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				bValid_z1 = FALSE;
		
			//we could get one more flag to indicate whether we want to reconstruct the buffer now, but 
			//instead it might be less confusing to hide the details of the implementation and simply force-call reconstruct 
			//so, here we would add it
			pCA->pCmpInfoTable->Add_FrameType_Simple(iCmpType,
													 TypeName,
													 iSize,
													 iMass,
													 ddResourceCost,
													 ddSupportCost,
													 bMarkFlag,
													 iIntegrity,
													 iNumOffsets,
													 fAccelBonus,
													 fDecelBonus,
													 iCapacity,
													 ppOffsetArrays[0],
													 pStartingIndexes[0],
													 pZLevels[0],
													 pZBaseLevels[0],
													 ppOffsetArrays[1],
													 pStartingIndexes[1],
													 pZLevels[1],
													 pZBaseLevels[1],
													 ppOffsetArrays[2],
													 pStartingIndexes[2],
													 pZLevels[2],
													 pZBaseLevels[2],
													 ppOffsetArrays[3],
													 pStartingIndexes[3],
													 pZLevels[3],
													 pZBaseLevels[3],
													 ppOffsetArrays[4],
													 pStartingIndexes[4],
													 pZLevels[4],
													 pZBaseLevels[4],
													 fTrav_Grassland,
													 fTrav_Ocean,
													 fTrav_Mountain,
													 fTrav_Hill,
													 fTrav_River,
													 fTrav_Forest,
													 fTrav_Air,
													 iMoveExtCharacteristics,
													 iDrawCharacteristics_z0,
													 iDirCharacteristics_z0,
													 iRotateFrames_z0,
													 bValid_z0,
													 iDrawCharacteristics_z1,
													 iDirCharacteristics_z1,
													 iRotateFrames_z1,
													 bValid_z1,
													 TRUE);

			//now delete the offset information allocated
			delete [] pStartingIndexes;
			delete [] pZLevels;
			delete [] pZBaseLevels;
			delete [] pArrayGroups;
			delete [] pArrayGroupIndexes;
			delete [] ppOffsetArrays;

		}
		if (iIDBlock == CONFIG_IDBLOCK_LTCMP_EN) //engine type loading block
		{
			
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			int iCmpType = -1;
			int iSize = -1;
			int iMass = -1;
			double ddResourceCost = -1;
			double ddSupportCost = -1;
			int iIntegrity = -1;
			float fAccelBonus = -1;
			float fDecelBonus = -1;
			int iPower = -1;
				
			//read in the component type 
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iCmpType = atoi(InBuffer);

			//read name directly to name buffer
			char Name[TEMP_STD_STRLEN];
			ZeroMemory(Name, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(Name, ',');
			
			//read in the size
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iSize = atoi(InBuffer);			
	
			//read in the mass
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iMass = atoi(InBuffer);		

			//read in the resource cost
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			ddResourceCost = atof(InBuffer);	

			//read in the support cost
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			ddSupportCost = atof(InBuffer);

			//read in the bMarkFlag
			BOOL bMarkFlag = FALSE; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bMarkFlag = TRUE;

			//read in the power
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iPower = atoi(InBuffer);	

			//get bDrawCharacteristics
			int iDrawCharacteristics = CMPDRAW_DRAWNORMAL;  //default
			//bdraw is just DRAWNORMAL, SINGLEFRAME, NODRAW, so check the first letter of the entry
			//
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'D' || InBuffer[0] == 'd')
				iDrawCharacteristics = CMPDRAW_DRAWNORMAL;
			if (InBuffer[0] == 'S' || InBuffer[0] == 's')
				iDrawCharacteristics = CMPDRAW_SINGLEFRAME; //singleframe no longer means anything, as long as it's not NODRAW
			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
				iDrawCharacteristics = CMPDRAW_NODRAW;

			//get bDirCharacteristics_z1 (has two settings, lock and free)
			int iDirCharacteristics = CMPDIR_LOCK; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');

			if (InBuffer[0] == 'L' || InBuffer[0] == 'l')
				iDirCharacteristics = CMPDIR_LOCK;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				iDirCharacteristics = CMPDIR_FREE;

			//get iRotateFrames
			int iRotateFrames = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iRotateFrames = atoi(InBuffer);

			//get value parameter
			BOOL bValid = TRUE; //default (we run into problems, I think, if it isn't true)
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');   //this is the last parameter, read to the close bracket '}'
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bValid = TRUE;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				bValid = FALSE;

			pCA->pCmpInfoTable->Add_EngineType_Simple(iCmpType, 
													  Name, 
													  iSize, 
													  iMass, 
													  ddResourceCost,
													  ddSupportCost,
													  bMarkFlag,
													  iPower, 
													  iDrawCharacteristics, 
													  iDirCharacteristics, 
													  iRotateFrames, 
													  bValid, 
													  TRUE);
		}
		if (iIDBlock == CONFIG_IDBLOCK_LTCMP_TL)
		{
			
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			int iCmpType = -1;
			int iSize = -1;
			int iMass = -1;
			double ddResourceCost = -1;
			double ddSupportCost = -1;
			int iIntegrity = -1;
			float fAccelBonus = -1;
			float fDecelBonus = -1;
			int iPower = -1;
				
			//read in the component type 
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iCmpType = atoi(InBuffer);

			//read name directly to name buffer
			char Name[TEMP_STD_STRLEN];
			ZeroMemory(Name, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(Name, ',');
			
			//read in the size
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iSize = atoi(InBuffer);			
	
			//read in the mass
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iMass = atoi(InBuffer);		

			//read in the resource cost
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			ddResourceCost = atof(InBuffer);

			//read in the support cost
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			ddSupportCost = atof(InBuffer);

			//read in the bMarkFlag
			BOOL bMarkFlag = FALSE; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bMarkFlag = TRUE;

			//read in userate
			int iUseRate = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iUseRate = atoi(InBuffer);	

			//read in preuseprocedure
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			
			//right now there's only one preuseprocedure that's available...NOTHING
			int iPreUseProcedure = PRETOOL_NOTHING; //default
			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
				iPreUseProcedure = PRETOOL_NOTHING;

			//right now there are two use procedures available, NOTHING( :-)) and BUILD
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			int iUseProcedure = TOOL_NOTHING; //default
			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
				iUseProcedure = TOOL_NOTHING;
			if (InBuffer[0] == 'B' || InBuffer[0] == 'b')
				iUseProcedure = TOOL_BUILD;

			//get fRange
			double fRange = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fRange = atof(InBuffer);

			//get bDrawCharacteristics
			int iDrawCharacteristics = CMPDRAW_NODRAW;  //default
			//bdraw is just DRAWNORMAL, SINGLEFRAME, NODRAW, so check the first letter of the entry
			//
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'D' || InBuffer[0] == 'd')
				iDrawCharacteristics = CMPDRAW_DRAWNORMAL;
			if (InBuffer[0] == 'S' || InBuffer[0] == 's')
				iDrawCharacteristics = CMPDRAW_SINGLEFRAME;
			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
				iDrawCharacteristics = CMPDRAW_NODRAW;

			//get bDirCharacteristics_z1 (has two settings, lock and free)
			int iDirCharacteristics = CMPDIR_LOCK; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');

			if (InBuffer[0] == 'L' || InBuffer[0] == 'l')
				iDirCharacteristics = CMPDIR_LOCK;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				iDirCharacteristics = CMPDIR_FREE;

			//get iRotateFrames
			int iRotateFrames = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iRotateFrames = atoi(InBuffer);

			//get value parameter
			BOOL bValid = TRUE; //default (we run into problems, I think, if it isn't true)
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');   //this is the last parameter, read to the close bracket '}'
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bValid = TRUE;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				bValid = FALSE;


			pCA->pCmpInfoTable->Add_ToolType_Simple( iCmpType,
													 Name,
													 iSize,
													 iMass,
													 ddResourceCost,
													 ddSupportCost,
													 bMarkFlag,
													 iUseRate,
													 iPreUseProcedure,
													 iUseProcedure,
													 fRange,
													 iDrawCharacteristics,
													 iDirCharacteristics,
													 iRotateFrames,
													 bValid,
													 TRUE);
		}
		if (iIDBlock == CONFIG_IDBLOCK_LTCMP_ST)
		{
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			int iCmpType = -1;
			int iSize = -1;
			int iMass = -1;
			double ddResourceCost = -1;
			double ddSupportCost = -1;
			int iIntegrity = -1;
			float fAccelBonus = -1;
			float fDecelBonus = -1;
				
			//read in the component type 
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iCmpType = atoi(InBuffer);

			//read name directly to name buffer
			char Name[TEMP_STD_STRLEN];
			ZeroMemory(Name, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(Name, ',');
			
			//read in the size
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iSize = atoi(InBuffer);			
	
			//read in the mass
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iMass = atoi(InBuffer);		

			//read in the resource cost
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			ddResourceCost = atof(InBuffer);

			//read in the support cost
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			ddSupportCost = atof(InBuffer);


			//read in the bMarkFlag
			BOOL bMarkFlag = FALSE; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bMarkFlag = TRUE;

			//read iStorageBonus
			int iStorageBonus = 0; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iStorageBonus = atoi(InBuffer);

			//read in the material storage bonus
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			double ddMaterialStorageBonus = atof(InBuffer);

			//get bDrawCharacteristics
			int iDrawCharacteristics = CMPDRAW_NODRAW;  //default
			//bdraw is just DRAWNORMAL, SINGLEFRAME, NODRAW, so check the first letter of the entry
			//
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'D' || InBuffer[0] == 'd')
				iDrawCharacteristics = CMPDRAW_DRAWNORMAL;
			if (InBuffer[0] == 'S' || InBuffer[0] == 's')
				iDrawCharacteristics = CMPDRAW_SINGLEFRAME;
			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
				iDrawCharacteristics = CMPDRAW_NODRAW;

			//get bDirCharacteristics (has two settings, lock and free)
			int iDirCharacteristics = CMPDIR_LOCK; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');

			if (InBuffer[0] == 'L' || InBuffer[0] == 'l')
				iDirCharacteristics = CMPDIR_LOCK;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				iDirCharacteristics = CMPDIR_FREE;

			//get iRotateFrames
			int iRotateFrames = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iRotateFrames = atoi(InBuffer);

			//get value parameter
			BOOL bValid = TRUE; //default (we run into problems, I think, if it isn't true)
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');   //this is the last parameter, read to the close bracket '}'
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bValid = TRUE;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				bValid = FALSE;

			pCA->pCmpInfoTable->Add_StorageType_Simple(iCmpType,
													   Name,
													   iSize,
													   iMass,
													   ddResourceCost,
													   ddSupportCost,
													   bMarkFlag,
													   iStorageBonus,
													   ddMaterialStorageBonus,
													   iDrawCharacteristics,
													   iDirCharacteristics,
													   iRotateFrames,
													   bValid,
													   TRUE);
		}
		if (iIDBlock == CONFIG_IDBLOCK_LTCMP_DF)
		{

			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			int iCmpType = -1;
			int iSize = -1;
			int iMass = -1;
			double ddResourceCost = -1;
			double ddSupportCost = -1;
			int iIntegrity = -1;
			float fAccelBonus = -1;
			float fDecelBonus = -1;
			int iPower = -1;
				
			//read in the component type 
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iCmpType = atoi(InBuffer);

			//read name directly to name buffer
			char Name[TEMP_STD_STRLEN];
			ZeroMemory(Name, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(Name, ',');
			
			//read in the size
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iSize = atoi(InBuffer);			
	
			//read in the mass
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iMass = atoi(InBuffer);		

			//read in the resource cost
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			ddResourceCost = atof(InBuffer);

			//read in the support cost
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			ddSupportCost = atof(InBuffer);


			//read in the bMarkFlag
			BOOL bMarkFlag = FALSE; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bMarkFlag = TRUE;

			//read iDefenseBonus
			int iDefenseBonus = 0; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iDefenseBonus = atoi(InBuffer);

			//get bDrawCharacteristics
			int iDrawCharacteristics = CMPDRAW_NODRAW;  //default
			//bdraw is just DRAWNORMAL, SINGLEFRAME, NODRAW, so check the first letter of the entry
			//
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'D' || InBuffer[0] == 'd')
				iDrawCharacteristics = CMPDRAW_DRAWNORMAL;
			if (InBuffer[0] == 'S' || InBuffer[0] == 's')
				iDrawCharacteristics = CMPDRAW_SINGLEFRAME;
			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
				iDrawCharacteristics = CMPDRAW_NODRAW;

			//get bDirCharacteristics (has two settings, lock and free)
			int iDirCharacteristics = CMPDIR_LOCK; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');

			if (InBuffer[0] == 'L' || InBuffer[0] == 'l')
				iDirCharacteristics = CMPDIR_LOCK;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				iDirCharacteristics = CMPDIR_FREE;

			//get iRotateFrames
			int iRotateFrames = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iRotateFrames = atoi(InBuffer);

			//get value parameter
			BOOL bValid = TRUE; //default (we run into problems, I think, if it isn't true)
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');   //this is the last parameter, read to the close bracket '}'
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bValid = TRUE;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				bValid = FALSE;

			pCA->pCmpInfoTable->Add_DefenseType_Simple(iCmpType,
													   Name,
													   iSize,
													   iMass,
													   ddResourceCost,
													   ddSupportCost,
													   bMarkFlag,
													   iDefenseBonus,
													   iDrawCharacteristics,
													   iDirCharacteristics,
													   iRotateFrames,
													   bValid,
													   TRUE);

		}
		if (iIDBlock == CONFIG_IDBLOCK_LTCMP_WP)
		{

			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			int iCmpType = -1;
			int iSize = -1;
			int iMass = -1;
			double ddResourceCost = -1;
			double ddSupportCost = -1;
			int iIntegrity = -1;
			float fAccelBonus = -1;
			float fDecelBonus = -1;
			int iPower = -1;
				
			//read in the component type 
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iCmpType = atoi(InBuffer);

			//read name directly to name buffer
			char Name[TEMP_STD_STRLEN];
			ZeroMemory(Name, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(Name, ',');
			
			//read in the size
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iSize = atoi(InBuffer);			
	
			//read in the mass
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iMass = atoi(InBuffer);		

			//read in the resource cost
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			ddResourceCost = atof(InBuffer);

			//read in the support cost
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			ddSupportCost = atof(InBuffer);


			//read in the bMarkFlag
			BOOL bMarkFlag = FALSE; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bMarkFlag = TRUE;

			//read in proj type
			int iProjectileType = PTYPE_TEST01; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iProjectileType = atoi(InBuffer);	

			//read in fire rate
			int iFireRate = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iFireRate = atoi(InBuffer);

			//read in preattackprocedure
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			
			//there are two pre attack procedures available....AIM and NOTHING
			int iPreAttackProcedure = PREATTACK_NOTHING; //default
			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
				iPreAttackProcedure = PREATTACK_NOTHING;
			if (InBuffer[0] == 'A' || InBuffer[0] == 'a')
				iPreAttackProcedure = PREATTACK_AIM;

			//right now there are two attack procedures available, PROJECTILE and NOTHING
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			int iAttackProcedure = ATTACK_NOTHING; //default
			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
				iAttackProcedure = ATTACK_NOTHING;
			if (InBuffer[0] == 'P' || InBuffer[0] == 'p')
				iAttackProcedure = ATTACK_PROJECTILE;

			//get fRange
			double fRange = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fRange = atof(InBuffer);

			//get bDrawCharacteristics
			int iDrawCharacteristics = CMPDRAW_NODRAW;  //default
			//bdraw is just DRAWNORMAL, SINGLEFRAME, NODRAW, so check the first letter of the entry
			//
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'D' || InBuffer[0] == 'd')
				iDrawCharacteristics = CMPDRAW_DRAWNORMAL;
			if (InBuffer[0] == 'S' || InBuffer[0] == 's')
				iDrawCharacteristics = CMPDRAW_SINGLEFRAME;
			if (InBuffer[0] == 'N' || InBuffer[0] == 'n')
				iDrawCharacteristics = CMPDRAW_NODRAW;

			//get bDirCharacteristics (has two settings, lock and free)
			int iDirCharacteristics = CMPDIR_LOCK; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');

			if (InBuffer[0] == 'L' || InBuffer[0] == 'l')
				iDirCharacteristics = CMPDIR_LOCK;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				iDirCharacteristics = CMPDIR_FREE;

			//get iRotateFrames
			int iRotateFrames = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iRotateFrames = atoi(InBuffer);

			//get value parameter
			BOOL bValid = TRUE; //default (we run into problems, I think, if it isn't true)
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');   //this is the last parameter, read to the close bracket '}'
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bValid = TRUE;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				bValid = FALSE;

if (bValid == TRUE)
{
//DEBUG
Write_Log(Name);
Write_Log(" TRUE information: ");
Write_Log(NText);
WriteLog_Enter();

}
	
			pCA->pCmpInfoTable->Add_WeaponType_Simple( iCmpType,
													   Name,
													   iSize,
													   iMass,
													   ddResourceCost,
													   ddSupportCost,
													   bMarkFlag,
													   iProjectileType,
													   iFireRate,
													   iPreAttackProcedure,
													   iAttackProcedure,
													   fRange,
													   iDrawCharacteristics,
													   iDirCharacteristics,
													   iRotateFrames,
													   bValid,
													   TRUE);
		}
        if (iIDBlock == CONFIG_IDBLOCK_LTPRJ)
		{
//DEBUG
Write_Log("LTPRJ block encountered.");
WriteLog_Enter();

			//read to open bracket
			Read_To_Stop(InBuffer, '{');

			//read in the type
			int iType = -1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iType = atoi(InBuffer);

			//read in range
			float fRange = -1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fRange = atof(InBuffer);

			//read in speed
			float fSpeed = 0; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fSpeed = atof(InBuffer);

			//read in acceleration
			float fAccel = 0; // default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fAccel = atof(InBuffer);

			//read in max speed
			float fMaxSpeed = -1; // default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			fMaxSpeed = atof(InBuffer);

			//read in iDamage
			int iDamage = 0; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iDamage = atoi(InBuffer);

			//read in splash damage mode
			int iSplashDamageMode = 0; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iSplashDamageMode = atoi(InBuffer);
			if (strlen(InBuffer) >= 7)
			if (InBuffer[0] == 'S')
			if (InBuffer[1] == 'P')
			if (InBuffer[6] == '1')
				iSplashDamageMode = 1;

			//read on Line of Fire mode
			BOOL bLineOfFire = TRUE; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			if (InBuffer[0] == 'T' || InBuffer[0] == 't')
				bLineOfFire = TRUE;
			if (InBuffer[0] == 'F' || InBuffer[0] == 'f')
				bLineOfFire = FALSE;

			//read in iRotateFrames
			int iRotateFrames = 1; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iRotateFrames = atoi(InBuffer);

			//read in the type of Trail it makes
			int iAnimType_Trail = VAL_INVALID; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			//map the indexes to the numbers
			iAnimType_Trail = atoi(InBuffer);			

			//read in the type of detonation (there's currently two, EXPL01 (blue) and EXPL02 (red) )
			int iAnimType_Detonate = VAL_INVALID; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iAnimType_Detonate = atoi(InBuffer);

			//sigh, read in the soundsegment for detonation
			int iSoundSeg_Detonate = VAL_INVALID; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iSoundSeg_Detonate = atoi(InBuffer);

			//read in the soundseg for firing
			int iSoundSeg_Fire = VAL_INVALID; //default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, '}');
			iSoundSeg_Fire = atoi(InBuffer);

			//set the information
			pCA->pPTypeTable->ProjTypes[iType].fRemrange = fRange;
			pCA->pPTypeTable->ProjTypes[iType].fSpeed = fSpeed;
			pCA->pPTypeTable->ProjTypes[iType].fMaxSpeed = fMaxSpeed;
			pCA->pPTypeTable->ProjTypes[iType].fAccel = fAccel;
			pCA->pPTypeTable->ProjTypes[iType].iDamage = iDamage;
			pCA->pPTypeTable->ProjTypes[iType].iRotateFrames = iRotateFrames;
			pCA->pPTypeTable->ProjTypes[iType].iAnimType_Trail = iAnimType_Trail;
			pCA->pPTypeTable->ProjTypes[iType].iAnimType_Detonate = iAnimType_Detonate;
			pCA->pPTypeTable->ProjTypes[iType].iSoundSeg_Detonate = iSoundSeg_Detonate;
			pCA->pPTypeTable->ProjTypes[iType].iSoundSeg_Fire = iSoundSeg_Fire;
			pCA->pPTypeTable->ProjTypes[iType].SourceGraphics.Init(pCA, NULL);			
			pCA->pPTypeTable->ProjTypes[iType].bLineOfFire = bLineOfFire;
			if (iSplashDamageMode == 1)
				pCA->pPTypeTable->ProjTypes[iType].Set_1TileSplash(0.5,0.2);
		}
		if (iIDBlock == CONFIG_IDBLOCK_LTPRJ_GR) //this is pretty much the same as LTCMP_GR
		{
//DEBUG
Write_Log("LTPRJ_GR block encountered.");
WriteLog_Enter();		
			//read to the open bracket
			Read_To_Stop(InBuffer, '{');
			//load a single frame of animation to source graphics

			//read in the target projectile type
			int iType = -1;
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
			Read_To_Stop(InBuffer, ',');
			iType = atoi(InBuffer);

			PROJTYPEINFO* pInfo = &pCA->pPTypeTable->ProjTypes[iType];

//from here we begin reading graphic blocks
			InBuffer[0] = ' ';
			while (InBuffer[0] != '}')
			{
				ReadFile( hInfile, InBuffer, 1, &dwNumBytesRead, NULL);
			
				//check for open bracket
				if ( InBuffer[0] == '<')
				{
					//read in the target Set index
					int iSetI = 0;
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iSetI = atoi(InBuffer);

					//read in the target Cycle index
					int iCycleI = 0;
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iCycleI = atoi(InBuffer);

					//read in the target Frame index
					int iFrameI = 0;
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iFrameI = atoi(InBuffer);

					//finally read in the source isurf number
					int iSurfSource = 1;
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iSurfSource = atoi(InBuffer);

					//read in the four rect numbers
					int iLeft = 0;
					int iRight = TILE_SIZE;
					int iTop = 0;
					int iBottom = TILE_SIZE;

					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iLeft = atoi(InBuffer);
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iRight = atoi(InBuffer);
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ',');
					iTop = atoi(InBuffer);
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, ','); //last entry
					iBottom = atoi(InBuffer);

					//read in duration
					int iDuration = 10; //default
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);
					Read_To_Stop(InBuffer, '>'); //last entry
					iDuration = atoi(InBuffer);

					//finally input the information
					//make sure the target type is not null
					if (pInfo != NULL)
					{
						pInfo->SourceGraphics.Add_Entry(iSetI, iCycleI, iFrameI, iSurfSource, iLeft, iRight, iTop, iBottom, iDuration);


					}


				}  //end read <> block

			}  //end read {} 

			//order update of cSrcGraphicsInfo 
			pInfo->SourceGraphics.Update();

		}
		if (iIDBlock == CONFIG_IDBLOCK_MMUPDATET)
		{
			//read to open bracket
			Read_To_Stop(InBuffer, '{');
			//read in the first parameter, the type
			int iTime = 5; //Default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(InBuffer, '}');
			iTime = atoi(InBuffer);
			iTempStore_MMUpdateTime = iTime;
		}
		if (iIDBlock == CONFIG_IDBLOCK_SETRES)
		{
			//read to open bracket
			Read_To_Stop(InBuffer, '{');
			int iX = 640; //default
			int iY = 480;
			int iB = 16;

			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(InBuffer, ',');
			iX = atoi(InBuffer);
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(InBuffer, ',');
			iY = atoi(InBuffer);
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(InBuffer, '}');
			iB = atoi(InBuffer);

			pCA->pDD_Display->iXR_TempStore = iX;
			pCA->pDD_Display->iYR_TempStore = iY;
			pCA->pDD_Display->iBPP_TempStore = iB;

		}
		if (iIDBlock == CONFIG_IDBLOCK_LTUNIT)
		{
			//read to open bracket
			Read_To_Stop(InBuffer, '{');
			//read in the first parameter, the type
			int iType = VAL_INVALID; //Default
			ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(InBuffer, ',');
			iType = atoi(InBuffer);

//DEBUG
itoa(iType, NText, 10);
Write_Log("Type: ");
Write_Log(NText);
WriteLog_Enter();
		
			//read in the second parameter, the name
			char Name[TEMP_STD_STRLEN];
			ZeroMemory(Name, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(Name, ',');

			//read in the third parameter, the short name base
			char ShortName[TEMP_STD_STRLEN];
			ZeroMemory(ShortName, sizeof(char)*TEMP_STD_STRLEN);					
			Read_To_Stop(ShortName, ',');

			InBuffer[0] = ' ';

			while (InBuffer[0] != ',' && InBuffer[0] != '~')
			{

				ReadFile( hInfile, InBuffer, 1, &dwNumBytesRead, NULL);

				//find open bracket <
				if (InBuffer[0] == '<')
				{
					//read in component type
					int iCmpType = VAL_INVALID; //Default
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
					Read_To_Stop(InBuffer, '>');
					iCmpType = atoi(InBuffer);

					//add type
					pCA->pUTypeTable->UnitTypes[iType].CmpAttached.Add_Component(iCmpType);
//DEBUG
Write_Log("Added Component to unit type.");
WriteLog_Enter();

				}
			}
			InBuffer[0] = ' ';

			while (InBuffer[0] != '}' && InBuffer[0] != '~')
			{

				ReadFile( hInfile, InBuffer, 1, &dwNumBytesRead, NULL);

				//find open bracket <
				if (InBuffer[0] == '<')
				{
					//read in component type
					int iPermissionType = VAL_INVALID; //Default
					ZeroMemory(InBuffer, sizeof(char)*TEMP_STD_STRLEN);					
					Read_To_Stop(InBuffer, '>');
					iPermissionType = atoi(InBuffer);

					//add type
					pCA->pUTypeTable->UnitTypes[iType].Permissions_Required_Build.Add_Permission(iPermissionType);
//DEBUG
Write_Log("Added permission to unit type.");
WriteLog_Enter();

				}

			}

			//now setup the unit
			pCA->pUTypeTable->UnitTypes[iType].Update_Cost(pCA);
			pCA->pUTypeTable->UnitTypes[iType].Update_Permissions(pCA);
			pCA->pUTypeTable->UnitTypes[iType].Set_Name(Name, TEMP_STD_STRLEN);
			pCA->pUTypeTable->UnitTypes[iType].Set_ShortName(ShortName, TEMP_STD_STRLEN);
			if (iType > pCA->pUTypeTable->iHighestTypeID)
					pCA->pUTypeTable->iHighestTypeID = iType;
//DEBUG
itoa(pCA->pUTypeTable->UnitTypes[iType].CmpAttached.iNumComponents, NText, 10);
Write_Log("Number of components: ");
Write_Log(NText);
WriteLog_Enter();

		}  
	}


	//close file
	CloseHandle(hInfile);

//DEBUG
Write_Log(LoadFileName);
Write_Log(" Completed loading.");
WriteLog_Enter();


}
//*********************************************************************************************//
//*********************************************************************************************//
void cConfig::Read_To_Stop(char* LoadBuffer, char stopchar)
{
	int chcount = 0;
	char inchar = 0;
	DWORD dwNumBytesRead;
	
	while (inchar != stopchar)
	{
		ReadFile(hInfile, &inchar, 1, &dwNumBytesRead, NULL);
		if (inchar != stopchar)  
		if (inchar != ' ' || chcount != 0) //discard pre-text whitespace also
		if (inchar != 10 && inchar != 13) //discard carriage returns also
		{
			LoadBuffer[chcount] = inchar;
			//safeguard
			if (inchar == '~')
			{
				LoadBuffer[0] = '~';
				return;
			}

			chcount++;
			//safeguard
			if (chcount > TEMP_STD_STRLEN)
				chcount = 0;
		}

	}
	LoadBuffer[chcount] = 0;

}
//*********************************************************************************************//
//*********************************************************************************************//
void cConfig::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
if (DEBUG + DEBUG_CONFIG)
{
	hLogFile = CreateFile("./debuglogs/Log_cConfig.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cConfig::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_CONFIG >= 1)
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
void cConfig::WriteLog_Enter()
{
if (DEBUG + DEBUG_CONFIG >= 1)
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
