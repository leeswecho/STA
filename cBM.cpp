#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBM::Init(LPSTR IFN, LPSTR IFN2, LPSTR OFN)
{
	Init_LogFile();
	
	

	InFileName = IFN;
	InFileName2 = IFN2; //new file to receive sector information (06.12.2004)
	OutFileName = OFN;

	//the new file to receive sector information. to minimize the chances of error(i'm in a hurry to implement this)
	//every single line to load each file has been doubled almost verbatim(06.12.2004)

//open the file for reading
	hBitmap = CreateFile(InFileName, 
						GENERIC_READ, 
						0, 
						NULL, 
						OPEN_EXISTING, 
						FILE_ATTRIBUTE_NORMAL,
						NULL);
	hBitmap2 = CreateFile(InFileName2, 
						GENERIC_READ, 
						0, 
						NULL, 
						OPEN_EXISTING, 
						FILE_ATTRIBUTE_NORMAL,
						NULL);	


	bMain = TRUE;
	if (hBitmap == INVALID_HANDLE_VALUE)
	{
		bMain = FALSE;
		return;
	}

	//do loading for main file
	ZeroMemory(&ImageHeader, sizeof(BITMAPFILEHEADER));
	rb = ReadFile(hBitmap, &ImageHeader, sizeof(BITMAPFILEHEADER), &dwNumBytesRead, NULL);
	ImageInfo = (LPBITMAPINFO)malloc(sizeof(BITMAPINFO));
	rb = ReadFile(hBitmap, ImageInfo, sizeof(BITMAPINFO), &dwNumBytesRead, NULL);
	SetFilePointer(hBitmap, ImageHeader.bfOffBits, NULL, FILE_BEGIN);
	ImageBuffer = (BYTE*)malloc(ImageInfo->bmiHeader.biSizeImage);
	rb = ReadFile(hBitmap, ImageBuffer, ImageInfo->bmiHeader.biSizeImage, &dwNumBytesRead, NULL);

	bSector = TRUE;
	if (hBitmap2 == INVALID_HANDLE_VALUE)
	{
		bSector = FALSE;
		Write_Log("No Sector Source File Found! Continuing anyway....");
		WriteLog_Enter();
	}

	if (bSector == TRUE)
	{
		ZeroMemory(&ImageHeader2, sizeof(BITMAPFILEHEADER));
		rb = ReadFile(hBitmap2, &ImageHeader2, sizeof(BITMAPFILEHEADER), &dwNumBytesRead, NULL);
		ImageInfo2 = (LPBITMAPINFO)malloc(sizeof(BITMAPINFO));
		rb = ReadFile(hBitmap2, ImageInfo2, sizeof(BITMAPINFO), &dwNumBytesRead, NULL);
		SetFilePointer(hBitmap2, ImageHeader2.bfOffBits, NULL, FILE_BEGIN);
		ImageBuffer2 = (BYTE*)malloc(ImageInfo2->bmiHeader.biSizeImage);
		rb = ReadFile(hBitmap2, ImageBuffer2, ImageInfo2->bmiHeader.biSizeImage, &dwNumBytesRead, NULL);
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBM::Write_MapFile_101()
{
	//create a new file with the filename specified
	hMapFile = CreateFile(OutFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	//create a buffer that will hold mapdata(the 1000 because for some odd reason there were allocation problems here...
	//doesn't matter since the extra bytes won't be written
	MapBuffer = (BYTE*)malloc(ImageInfo->bmiHeader.biSizeImage + 1000);

//kind of messy here because it was much improvised (it works though!)
int ImageSize = ImageInfo->bmiHeader.biSizeImage;
int Height = ImageInfo->bmiHeader.biHeight;
int Width = ImageSize / Height;     //this is the total width of every line(there are extra black dots at the end
int mappos, bitpos;
//for DEBUG
char NText[50];

	//right now, there is no reason for any number conversion(there could be in the future), so just copy the
	//image buffer straight into the mapbuffer(except flipping the lines
	for (c1 = 0; c1 <= Height - 1; c1++)
	for (c2 = 0; c2 <= ImageInfo->bmiHeader.biWidth - 1; c2++)      //write only to biWidth on the map (leave out the extra lines that were on the bitmap)
	{
		mappos = (ImageInfo->bmiHeader.biWidth * (Height - 1 - c1)) + c2;
		bitpos = (Width * c1) + c2;
		
		//move the mapdata forward by four bytes so that the mapsize can be placed at the beginning
		*(MapBuffer + mappos + 4) = *(ImageBuffer + bitpos);

//DEBUG
Write_Log("Tile ");
itoa(mappos, NText, 10);
Write_Log(NText);
Write_Log(": ");
itoa(*(MapBuffer + mappos + 4), NText, 10);
Write_Log(NText);
WriteLog_Enter();

	}
	
	//create a WORD that will be used to plug the mapsize into the mapfile
	WORD* wp = (WORD *)MapBuffer;
	//width(x) first
	*(wp) = ImageInfo->bmiHeader.biWidth;
	wp++;
	//then the height(y)
	*(wp) = Height;
	wp = NULL;

	//now, write the mapbuffer straight into the Mapfile (size is 4 bytes larger because of the size data)
	rb = WriteFile(hMapFile, MapBuffer, ImageInfo->bmiHeader.biWidth * Height + 4, &dwNumBytesWritten, NULL);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBM::Write_MapFile_200()
{
	if (bMain == FALSE)
	{
		Write_Log("Failed to Run. Reason: No source file.");
		WriteLog_Enter();
		return;
	}

	//create a new file with the filename specified
	hMapFile = CreateFile(OutFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	//create a buffer that will hold mapdata(the 1000 because for some odd reason there were allocation problems here...
	//doesn't matter since the extra bytes won't be written
	MapBuffer = (BYTE*)malloc(ImageInfo->bmiHeader.biSizeImage * 1.1);

//kind of messy here because it was much improvised (it works though!)
int ImageSize = ImageInfo->bmiHeader.biSizeImage;
int Height = ImageInfo->bmiHeader.biHeight;
int Width = (ImageSize * 8) / (Height * ImageInfo->bmiHeader.biBitCount);     //this is the total width of every line(there are extra black dots at the end
int mappos, bitpos;


//Text holding buffer
char NText[50];


	//version 2.01 version number
	LPSTR OutText = "[BASIC] {2.01,";



	//write to it the [BASIC] block....and stuff
	WriteFile( hMapFile, OutText, sizeof(char) * strlen(OutText), &dwNumBytesWritten, NULL);


//seed the map with a random ID number(can be changed, of course, but currently, just a way to "ensure" identical maps in multiplayer (05.11.2004)	
int iID = rand() % 1000000;
	itoa(iID, NText, 10);
	WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);
	//comma
	NText[0] = ',';
	NText[1] = 0;
	WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);
	//get the width as a string
	ZeroMemory(NText, sizeof(char)*50);
	itoa(ImageInfo->bmiHeader.biWidth, NText, 10);
	WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);
	//comma
	NText[0] = ',';
	NText[1] = 0;
	WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);
	//and then the height
	ZeroMemory(NText, sizeof(char)*50);
	itoa(Height, NText, 10);
	WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);	
	//comma + enter
	NText[0] = ',';
	NText[1] = 13;
	NText[2] = 10;
	NText[3] = 0;
	WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);

	int iRawVal; //for conversion (05.11.2004)
	int iOutVal;

	//right now, there is no reason for any number conversion(there could be in the future), so just copy the
	//image buffer straight into the mapbuffer(except flipping the lines
	for (c1 = 0; c1 <= Height - 1; c1++)
	{
	//add two extra bits on the MAP buffer's side so that we can put an carriage return into the mapfile for alignment
	for (c2 = 0; c2 <= ImageInfo->bmiHeader.biWidth + 1; c2++)      //write only to biWidth on the map (leave out the extra lines that were on the bitmap)
	{
		mappos = ((ImageInfo->bmiHeader.biWidth + 2) * (Height - 1 - c1)) + c2;
		bitpos = (Width * c1) + c2;
		
		iRawVal = MAPTILE_OCEAN; //default
		iRawVal = *(ImageBuffer + bitpos);

		//now perform conversion
		//do for each type
			switch(iRawVal)
			{
				case MAPTILE_OCEAN:
					iOutVal = MAPFILE_SYM_OCEAN;
				break;
				case MAPTILE_GRASSLAND:
					iOutVal = MAPFILE_SYM_GRASSLAND;
				break;
				case MAPTILE_MOUNTAIN:
					iOutVal = MAPFILE_SYM_MOUNTAIN;
				break;
				case MAPTILE_HILL:
					iOutVal = MAPFILE_SYM_HILL;
				break;
				case MAPTILE_FOREST:
					iOutVal = MAPFILE_SYM_FOREST;
				break;
				case MAPTILE_RIVER:
					iOutVal = MAPFILE_SYM_RIVER;
				break;
				default:
					iOutVal = MAPFILE_SYM_OCEAN;
				break;
			}

		//
		*(MapBuffer + mappos) = iOutVal;

	}

		mappos = ((ImageInfo->bmiHeader.biWidth + 2) * (Height - 1 - c1)) + ImageInfo->bmiHeader.biWidth;		
		*(MapBuffer + mappos) = 13;
		mappos = ((ImageInfo->bmiHeader.biWidth + 2) * (Height - 1 - c1)) + ImageInfo->bmiHeader.biWidth + 1;		
		*(MapBuffer + mappos) = 10;
	}
	
	//the extra Height*2 is for the extra two bits on each line for carriage return
	rb = WriteFile(hMapFile, MapBuffer, ImageInfo->bmiHeader.biWidth * Height + (Height*2), &dwNumBytesWritten, NULL);

	//write the close bracket
	LPSTR OutText2 = "}";
	WriteFile( hMapFile, OutText2, sizeof(char) * strlen(OutText2), &dwNumBytesWritten, NULL);

//write the enter 
	NText[0] = 13;
	NText[1] = 10;	
	NText[2] = 0;
	NText[3] = 0;
	WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);

//the process of writing out sector information is a little harder. We basically have to map out each color in the image
//to a unique character pair, and write out each of these to the map. To do this, we need to go through the entire image
//to find all the used colors, and then assign character pairs(ex. AA, AB, AC, etc.) to these.
//we then go back and start actually writing the SECTOR block using these associations. 
//once done, we need to go back AGAIN and write out the SECTREG block (06.12.2004) ...actual impelementation saved for a later date!

//don't do this if we don't have a sector source file
if (bSector == TRUE)
{
//do the same for the sector image
int ImageSize2 = ImageInfo2->bmiHeader.biSizeImage;
int Height2 = ImageInfo2->bmiHeader.biHeight;
int Width2 = (ImageSize2 * 8)/ (Height2 * ImageInfo2->bmiHeader.biBitCount);     //this is the total width of every line(there are extra black dots at the end
int mappos2, bitpos2; 

		//before we do anything we need to write out the beginning formatting of the SECTOR block
		LPSTR OutText3 = "[SECTOR] { ";

		//write to it the [SECTOR] block....and stuff
		WriteFile( hMapFile, OutText3, sizeof(char) * strlen(OutText3), &dwNumBytesWritten, NULL);
		//write in an enter
		NText[0] = 13;
		NText[1] = 10;	
		NText[2] = 0;
		NText[3] = 0;
		WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);


	//the big sector identifier table
	cSectorAssignment* SectorAssigns[66000]; //this table stores possible color values for bitmaps of up to high(16-bit) color
	ZeroMemory(SectorAssigns, sizeof(cSectorAssignment*) * 66000);

	BYTE bCharCounters[2];
	bCharCounters[0] = 65;
	bCharCounters[1] = 65; //start both counters off at 65 (ASCII 'AA')

	//use different reads for different bit depth bitmaps
	//24-bit read
	if (ImageInfo2->bmiHeader.biBitCount == 24)
	{

//DEBUG
Write_Log("Beginning 24 bit map conv SECTOR");
WriteLog_Enter();

		BYTE* pbScan1 = (BYTE*)ImageBuffer2;
		BYTE* pbScan2 = pbScan1 + 1;
		BYTE* pbScan3 = pbScan2 + 1;

		//the first pass through, we do not need to be especially concerned with the 
		//formatting, only the colors present in the image, so we simply scan through it
		for (int c996 = 0; c996 < ImageInfo2->bmiHeader.biSizeImage / 3; c996++)
		{
//DEBUG 
itoa(*pbScan1, NText, 10);
Write_Log(NText);
Write_Log(" ");
itoa(*pbScan2, NText, 10);
Write_Log(NText);
Write_Log(" ");
itoa(*pbScan3, NText, 10);
Write_Log(NText);
WriteLog_Enter();

			WORD wRet = Conv_24bit_to_16bit(*pbScan1, *pbScan2, *pbScan3); //convert to 16bits

			if (SectorAssigns[wRet] == NULL)
			{
				//set this sector assignment
				SectorAssigns[wRet] = new cSectorAssignment;
				SectorAssigns[wRet]->IDPair[0] = bCharCounters[0];
				SectorAssigns[wRet]->IDPair[1] = bCharCounters[1];
				SectorAssigns[wRet]->IDPair[2] = 0;

//DEBUG
Write_Log(" Character Assignment: ");
Write_Log( SectorAssigns[wRet]->IDPair);
WriteLog_Enter();

				//now increment the char counters
				bCharCounters[0]++;

				if (bCharCounters[0] > 90)  //90 is ASCII 'Z'
				{
					bCharCounters[0] = 65;
					bCharCounters[1]++;

					if (bCharCounters[1] == 91) //91 is ASCII '[' (which would spell trouble)
						bCharCounters[1] = 97;  //97 is ASCII 'a'
				}

			}

				//now move forward the pointer
				pbScan1 += 3; //move forward 3 bytes
				pbScan2 = pbScan1 + 1;
				pbScan3 = pbScan2 + 1;

		}

//DEBUG
Write_Log("Beginning second (map-writing) pass");
WriteLog_Enter();
		//the second pass, we do need to be concerned about the formatting,
		//so this time we must scan line-by-line, and from bottom up
		BYTE* pbStart = (BYTE*)ImageBuffer2;

//DEBUG
itoa(Height2, NText, 10);
Write_Log("Height2: ");
Write_Log(NText);
Write_Log(" Width2: ");
itoa(Width2, NText, 10);
Write_Log(NText);
WriteLog_Enter();

		for (c1 = 0; c1 <= Height2 - 1; c1++)
		{
			for (c2 = 0; c2 < Width2; c2++)      //write only to biWidth on the map (leave out the extra lines that were on the bitmap)
			{
				mappos = ((ImageInfo2->bmiHeader.biWidth) * (Height2 - 1 - c1)) + c2;
				pbScan1 = pbStart + (mappos * 3); //3 bytes per pixel
				pbScan2 = pbScan1 + 1;
				pbScan3 = pbScan2 + 1;
				WORD wRet = Conv_24bit_to_16bit(*pbScan1, *pbScan2, *pbScan3);

//DEBUG
/*
Write_Log("Writing Entry...");
itoa(c2, NText, 10);
Write_Log(NText);
Write_Log(" ");
itoa(c1, NText, 10);
Write_Log(NText);
WriteLog_Enter();
*/				//write out the characters associated with this color
				WriteFile( hMapFile, "<",sizeof(char) *  strlen("<"), &dwNumBytesWritten, NULL);
				WriteFile( hMapFile, SectorAssigns[wRet]->IDPair, sizeof(char) * strlen(SectorAssigns[wRet]->IDPair), &dwNumBytesWritten, NULL);
				WriteFile( hMapFile, ">", sizeof(char) * strlen(">"), &dwNumBytesWritten, NULL);
			}
			//and then write out the carriage return

			NText[0] = 13;
			NText[1] = 10;	
			NText[2] = 0;
			NText[3] = 0;
			WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);

		}

	}

	//16-bit read
	if (ImageInfo2->bmiHeader.biBitCount == 16)
	{
		WORD* pwScan = (WORD*)ImageBuffer2;

		//the first pass through, we do not need to be especially concerned with the 
		//formatting, only the colors present in the image, so we simply scan through it
		for (int c996 = 0; c996 < ImageInfo2->bmiHeader.biSizeImage / 2; c996++)
		{
			if (SectorAssigns[*pwScan] == NULL)
			{
				//set this sector assignment
				SectorAssigns[*pwScan] = new cSectorAssignment;
				SectorAssigns[*pwScan]->IDPair[0] = bCharCounters[0];
				SectorAssigns[*pwScan]->IDPair[1] = bCharCounters[1];
				SectorAssigns[*pwScan]->IDPair[2] = 0;

				//now increment the char counters
				bCharCounters[0]++;

				if (bCharCounters[0] > 90)  //90 is ASCII 'Z'
				{
					bCharCounters[0] = 65;
					bCharCounters[1]++;

					if (bCharCounters[1] == 91) //91 is ASCII '[' (which would spell trouble)
						bCharCounters[1] = 97;  //97 is ASCII 'a'
				}

			}

				//now move forward the pointer
				pwScan++;

		}

		//the second pass, we do need to be concerned about the formatting,
		//so this time we must scan line-by-line, and from bottom up
		WORD* pwStart = (WORD*)ImageBuffer2;

		for (c1 = 0; c1 <= Height2 - 1; c1++)
		{
			for (c2 = 0; c2 < Width2; c2++)      //write only to biWidth on the map (leave out the extra lines that were on the bitmap)
			{
				mappos = (ImageInfo2->bmiHeader.biWidth * (Height2 - 1 - c1)) + c2;
				pwScan = pwStart + mappos;

				//write out the characters associated with this color
				WriteFile( hMapFile, "<",sizeof(char) *  strlen("<"), &dwNumBytesWritten, NULL);
				WriteFile( hMapFile, SectorAssigns[*pwScan]->IDPair, sizeof(char) * strlen(SectorAssigns[*pwScan]->IDPair), &dwNumBytesWritten, NULL);
				WriteFile( hMapFile, ">", sizeof(char) * strlen(">"), &dwNumBytesWritten, NULL);
			}
			//and then write out the carriage return

			NText[0] = 13;
			NText[1] = 10;	
			NText[2] = 0;
			NText[3] = 0;
			WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);

		}

	}
	
	//8 Bit Count
	if (ImageInfo2->bmiHeader.biBitCount == 8)
	{
//DEBUG
Write_Log("Beginning 8 bit map conv SECTOR");
WriteLog_Enter();

		BYTE* pbScan = (BYTE*)ImageBuffer2;

		//the first pass through, we do not need to be especially concerned with the 
		//formatting, only the colors present in the image, so we simply scan through it
		for (int c996 = 0; c996 < ImageInfo2->bmiHeader.biSizeImage; c996++)
		{
//DEBUG
itoa(*pbScan, NText, 10);
Write_Log(NText);
WriteLog_Enter();
			if (SectorAssigns[*pbScan] == NULL)
			{


				//set this sector assignment
				SectorAssigns[*pbScan] = new cSectorAssignment;
				SectorAssigns[*pbScan]->IDPair[0] = bCharCounters[0];
				SectorAssigns[*pbScan]->IDPair[1] = bCharCounters[1];
				SectorAssigns[*pbScan]->IDPair[2] = 0;

//DEBUG
Write_Log("Color:");
itoa(*pbScan, NText, 10);
Write_Log(NText);
Write_Log(" Character Assignment: ");
Write_Log( SectorAssigns[*pbScan]->IDPair);
WriteLog_Enter();

				//now increment the char counters
				bCharCounters[0]++;

				if (bCharCounters[0] > 90)  //90 is ASCII 'Z'
				{
					bCharCounters[0] = 65;
					bCharCounters[1]++;

					if (bCharCounters[1] == 91) //91 is ASCII '[' (which would spell trouble)
						bCharCounters[1] = 97;  //97 is ASCII 'a'
				}

			}

				//now move forward the pointer
				pbScan++;

		}

		//the second pass, we do need to be concerned about the formatting,
		//so this time we must scan line-by-line, and from bottom up
		BYTE* pbStart = (BYTE*)ImageBuffer2;

		for (c1 = 0; c1 <= Height2 - 1; c1++)
		{
			for (c2 = 0; c2 < Width2; c2++)      //write only to biWidth on the map (leave out the extra lines that were on the bitmap)
			{
				mappos = (ImageInfo2->bmiHeader.biWidth * (Height2 - 1 - c1)) + c2;
				pbScan = pbStart + mappos;

				//write out the characters associated with this color
				WriteFile( hMapFile, "<",sizeof(char) *  strlen("<"), &dwNumBytesWritten, NULL);
				WriteFile( hMapFile, SectorAssigns[*pbScan]->IDPair, sizeof(char) * strlen(SectorAssigns[*pbScan]->IDPair), &dwNumBytesWritten, NULL);
				WriteFile( hMapFile, ">", sizeof(char) * strlen(">"), &dwNumBytesWritten, NULL);

//DEBUG
Write_Log(SectorAssigns[*pbScan]->IDPair);

			}
			//and then write out the carriage return

			NText[0] = 13;
			NText[1] = 10;	
			NText[2] = 0;
			NText[3] = 0;
			WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);

		}

	}

	//now close the sector block with a close bracket and an enter
	WriteFile( hMapFile, "}", sizeof(char) * strlen("}"), &dwNumBytesWritten, NULL);
			NText[0] = 13;
			NText[1] = 10;	
			NText[2] = 0;
			NText[3] = 0;
			WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);

	//NOW, we have to write out SECTREG blocks for each sector that was registered. We do this by going through the sector assignments table
	for (int c990 = 0; c990 < 66000; c990++)
	{
		if (SectorAssigns[c990] != NULL)
		{

			//write SECTREG
			LPSTR OutText4 = "[SECTREG] {Sector ";
			LPSTR OutText20 = SectorAssigns[c990]->IDPair;
			LPSTR OutText21 = ", ";
			WriteFile( hMapFile, OutText4, sizeof(char) * strlen(OutText4), &dwNumBytesWritten, NULL);
			WriteFile( hMapFile, OutText20, sizeof(char) * strlen(OutText20), &dwNumBytesWritten, NULL);
			WriteFile( hMapFile, OutText21, sizeof(char) * strlen(OutText21), &dwNumBytesWritten, NULL);

			//write the ID pair string
			WriteFile( hMapFile, SectorAssigns[c990]->IDPair, sizeof(char) * strlen(SectorAssigns[c990]->IDPair), &dwNumBytesWritten, NULL);

			//write out the rest
			LPSTR OutText5 = ",0}";
			WriteFile( hMapFile, OutText5, sizeof(char) * strlen(OutText5), &dwNumBytesWritten, NULL);
			//write out an enter
			NText[0] = 13;
			NText[1] = 10;	
			NText[2] = 0;
			NText[3] = 0;
			WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);
		}
	}
}

	//write the enter and the tilde
	NText[0] = 13;
	NText[1] = 10;
	NText[2] = '~';
	NText[3] = 0;
	WriteFile( hMapFile, NText, sizeof(char) * strlen(NText), &dwNumBytesWritten, NULL);



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

cBM::~cBM()
{

//free everything!
	free(ImageBuffer);
Write_Log("A");
	free(MapBuffer);
Write_Log("B");
	free(ImageInfo);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBM::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("Log_MapConv.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBM::Write_Log(LPSTR Text)
{

DWORD numbyteswritten = 0;

WriteFile( hLogFile,
		   Text,
		   strlen(Text),
		   &numbyteswritten,
		   NULL);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBM::WriteLog_Enter()
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
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
WORD cBM::Conv_24bit_to_16bit(BYTE bR, BYTE bG, BYTE bB)
{
	WORD wRet = 0;
	WORD wR = bR;
	WORD wG = bG;
	WORD wB = bB;

	//conversion is to 5:6:5 convention, so shave 3 lsb off of R and B channels
	//shave 2 lsb off of G channel
	wR = wR >> 3;
	wG = wG >> 2;
	wB = wB >> 3;

	//and then move them to their positions

	wR = wR << 11;
	wG = wG << 5;

	//and then return the result

	return wR + wG + wB;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
