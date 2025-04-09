#include "classes.h"
//*****************************************************************************//
//*****************************************************************************//
void cStringTable::Init(VOID* pCA)
{

	pCAccess = pCA;

	for (int c91 = 0; c91 < 10; c91++)
	{
		iRandomStringCounter[c91] = 0;
		iOverStringCounter[c91] = 0;
	}

	//initialize pointers
	for (int c92 = 0; c92 < STRING_NUMIDS; c92++)
	{
		pTable[c92] = NULL;
	}

	//register a number of strings the system will use
	Register_String( "Build Cmp", STRI_BUILD_COMPONENT, 15);
	Register_String( "Build Unit", STRI_BUILD_UNIT, 10);
	Register_String( "Blank", STRI_BLANK, 5);

	Register_String( "Lexington", 4000, 5);	
	Register_String( "Saratoga", 4001, 5);
	Register_String( "Yorktown", 4002, 5);
	Register_String( "Enterprise", 4003, 5);
	Register_String( "Missouri", 4004, 5);	
	Register_String( "Iowa", 4005, 5);
	Register_String( "New Jersey", 4006, 5);
	Register_String( "Wisconsin", 4007, 5);
	Register_String( "Tennessee", 4008, 5);	
	Register_String( "Washington", 4009, 5);
	Register_String( "North Carolina", 4010, 5);
	Register_String( "Olympia", 4011, 5);
	Register_String( "Essex", 4012, 5);	
	Register_String( "Midway", 4013, 5);
	Register_String( "Gambier Bay", 4014, 5);
	Register_String( "St. Lo", 4015, 5);
	Register_String( "Baltimore", 4016, 5);	
	Register_String( "Boston", 4017, 5);
	Register_String( "Quincy", 4018, 5);
	Register_String( "Pittsburgh", 4019, 5);
	Register_String( "St. Paul", 4020, 5);	
	Register_String( "Columbus", 4021, 5);
	Register_String( "Helena", 4022, 5);
	Register_String( "Bremerton", 4023, 5);
	Register_String( "Fall River", 4024, 5);	
	Register_String( "Macon", 4025, 5);
	Register_String( "Toledo", 4026, 5);
	Register_String( "Los Angeles", 4027, 5);
	Register_String( "Chicago", 4028, 5);	
	Register_String( "Oregon City", 4029, 5);
	Register_String( "Albany", 4030, 5);
	Register_String( "Rochester", 4031, 5);

	Register_String( "Yamato", 4100, 5);
	Register_String( "Musashi", 4101, 5);
	Register_String( "Akagi", 4102, 5);
	Register_String( "Kaga", 4103, 5);
	Register_String( "Ise", 4104, 5);
	Register_String( "Hyuga", 4105, 5);
	Register_String( "Yamashiro", 4106, 5);
	Register_String( "Nagato", 4107, 5);
	Register_String( "Mutsu", 4108, 5);
	Register_String( "Junyo", 4109, 5);
	Register_String( "Ryujo", 4110, 5);
	Register_String( "Taiho", 4111, 5);
	Register_String( "Furutaka", 4112, 5);
	Register_String( "Kako", 4113, 5);
	Register_String( "Kinugasa", 4114, 5);
	Register_String( "Aoba", 4115, 5);
	Register_String( "Jintsu", 4116, 5);
	Register_String( "Naka", 4117, 5);
	Register_String( "Sendai", 4118, 5);
	Register_String( "Abukama", 4119, 5);
	Register_String( "Isuzu", 4120, 5);
	Register_String( "Kinu", 4121, 5);
	Register_String( "Nagara", 4122, 5);
	Register_String( "Natori", 4123, 5);
	Register_String( "Yura", 4124, 5);
	Register_String( "Kiso", 4125, 5);
	Register_String( "Kitakami", 4126, 5);
	Register_String( "Kuma", 4127, 5);
	Register_String( "Oi", 4128, 5);
	Register_String( "Tama", 4129, 5);
	Register_String( "Yubari", 4130, 5);
	Register_String( "Tatsuta", 4131, 5);

	Register_String( "Iron Duke", 4200, 5);	
	Register_String( "Warspite", 4201, 5);	
	Register_String( "Royal Oak", 4202, 5);	
	Register_String( "Royal Sovereign", 4203, 5);
	Register_String( "Revenge", 4204, 5);	
	Register_String( "Ramilles", 4205, 5);	
	Register_String( "Repulse", 4206, 5);	
	Register_String( "Nelson", 4207, 5);
	Register_String( "King George V", 4208, 5);	
	Register_String( "Invincible", 4209, 5);	
	Register_String( "Inflexible", 4210, 5);	
	Register_String( "Furious", 4211, 5);
	Register_String( "Duke of Marlborough", 4212, 5);	
	Register_String( "Anson", 4213, 5);	
	Register_String( "Renown", 4214, 5);	
	Register_String( "Indefatigable", 4215, 5);

	Register_String( "Bismarck", 4300, 5);
	Register_String( "Tirpitz", 4301, 5);
	Register_String( "Scharnhorst", 4302, 5);
	Register_String( "Gneisenau", 4303, 5);
	Register_String( "Koenigsberg", 4304, 5);
	Register_String( "Prinz Eugen", 4305, 5);
	Register_String( "Karlsruhe", 4306, 5);
	Register_String( "Koeln", 4307, 5);
	Register_String( "Nymphe", 4308, 5);
	Register_String( "Thetis", 4309, 5);
	Register_String( "Amazone", 4310, 5);
	Register_String( "Medusa", 4311, 5);
	Register_String( "Nymphe", 4312, 5);
	Register_String( "Thetis", 4313, 5);
	Register_String( "Amazone", 4314, 5);
	Register_String( "Medusa", 4315, 5);
	Register_String( "Seydlitz", 4312, 5);
	Register_String( "Moltke", 4313, 5);
	Register_String( "Nuernberg", 4314, 5);
	Register_String( "Admiral Hipper", 4315, 5);


}
//*****************************************************************************//
//*****************************************************************************//
void cStringTable::Exit()
{
	//go through and delete
	for (int c92 = 0; c92 < STRING_NUMIDS; c92++)
	{
		if (pTable[c92] != NULL)
			delete [] pTable[c92];
	}

}
//*****************************************************************************//
//*****************************************************************************//
char* cStringTable::Register_String(LPSTR pString, int iIndex, int iStrLen)
{

	//make a copy of the string
	//char* pStr = new char[iStrLen + 1 + GEN_PADBYTES];
	char* pStr = new char[strlen(pString) + 1 + GEN_PADBYTES];
	ZeroMemory(pStr, sizeof(char)* (strlen(pString) + 1 + GEN_PADBYTES));

	//currently a little wary of memcpy, so I'm doing this manually....
	for (int c92 = 0; c92 < strlen(pString); c92++)
	{
		pStr[c92] = pString[c92];
	}

	//Null-terminate, just in case there wasn't one
	//pStr[iStrLen] = 0;



	//assign
	if (pTable[iIndex] != NULL)
		delete [] pTable[iIndex];

	pTable[iIndex] = pStr;

	return pTable[iIndex];

}
//*****************************************************************************//
//*****************************************************************************//
void cStringTable::Unregister_String(int iIndex)
{
	
	//assign
	if (pTable[iIndex] != NULL)
		delete [] pTable[iIndex];

}
//*****************************************************************************//
//*****************************************************************************//
char* cStringTable::Supply_Random_String(int iNameType)
{

	//check if the NEXT entry is NULL...if so, simply issue "blank unit"
	if (pTable[iRandomStringCounter[iNameType] + (iNameType * 100) + 4000] == NULL)
	{
		//iRandomStringCounter[iNameType] = 0;
		return pTable[STRI_BLANK];
	} 
		else
	{


	char* retstr = pTable[iRandomStringCounter[iNameType] + (iNameType * 100) + 4000];

	iRandomStringCounter[iNameType]++;

	return retstr;
	}
}
//*****************************************************************************//
//*****************************************************************************//


