#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cMaterialInfoTable::Init(VOID* pCA)
{
	pCAccess = pCA;

	for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
	{
		ZeroMemory(Types[lc1].Name, 40*sizeof(char));
		ZeroMemory(Types[lc1].ShortName, 10*sizeof(char));
		Types[lc1].RefChar = ' ';
		Types[lc1].ddDensity = 1;
		Types[lc1].iOverloadProjectileType = -1;
		Types[lc1].ddSpeedMultiplier = 1.0;
		Types[lc1].ddArmorMultiplier = 1.0;
		Types[lc1].ddArmorBonus = 0;
	}

	strcat(Types[MATERIAL_ELEMENT_C].Name, "Carbon");
	strcat(Types[MATERIAL_ELEMENT_C].ShortName, "Carbon");
	Types[MATERIAL_ELEMENT_C].RefChar = 'C';
	Types[MATERIAL_ELEMENT_C].ddDensity = 1;


	strcat(Types[MATERIAL_ELEMENT_I].Name, "Iron");
	strcat(Types[MATERIAL_ELEMENT_I].ShortName, "Iron");
	Types[MATERIAL_ELEMENT_I].RefChar = 'I';
	Types[MATERIAL_ELEMENT_I].ddDensity = 1;


	strcat(Types[MATERIAL_ELEMENT_S].Name, "Steel");
	strcat(Types[MATERIAL_ELEMENT_S].ShortName, "Steel");
	Types[MATERIAL_ELEMENT_S].RefChar = 'S';
	Types[MATERIAL_ELEMENT_S].ddDensity = 1;

	strcat(Types[MATERIAL_ELEMENT_P].Name, "Polypropylene");
	strcat(Types[MATERIAL_ELEMENT_P].ShortName, "Polypl");
	Types[MATERIAL_ELEMENT_P].RefChar = 'P';
	Types[MATERIAL_ELEMENT_P].ddDensity = 1;

	strcat(Types[MATERIAL_ELEMENT_A].Name, "Acetylene");
	strcat(Types[MATERIAL_ELEMENT_A].ShortName, "Acetyl");
	Types[MATERIAL_ELEMENT_A].RefChar = 'A';
	Types[MATERIAL_ELEMENT_A].ddDensity = 1;


	strcat(Types[MATERIAL_ELEMENT_T].Name, "Titanium");
	strcat(Types[MATERIAL_ELEMENT_T].ShortName, "Titani");
	Types[MATERIAL_ELEMENT_T].RefChar = 'T';
	Types[MATERIAL_ELEMENT_T].ddDensity = 1;
	Types[MATERIAL_ELEMENT_T].ddArmorMultiplier = 0;
	Types[MATERIAL_ELEMENT_T].ddArmorBonus = 0.8;

	strcat(Types[MATERIAL_ELEMENT_L].Name, "Silicon");
	strcat(Types[MATERIAL_ELEMENT_L].ShortName, "Silicon");
	Types[MATERIAL_ELEMENT_L].RefChar = 'L';
	Types[MATERIAL_ELEMENT_L].ddDensity = 1;
	Types[MATERIAL_ELEMENT_L].ddArmorMultiplier = 0;
	Types[MATERIAL_ELEMENT_L].ddArmorBonus = 0;

	strcat(Types[MATERIAL_ELEMENT_U].Name, "Uranium");
	strcat(Types[MATERIAL_ELEMENT_U].ShortName, "Urani");
	Types[MATERIAL_ELEMENT_U].RefChar = 'U';
	Types[MATERIAL_ELEMENT_U].ddDensity = 1;
	Types[MATERIAL_ELEMENT_U].iOverloadProjectileType = 11; //ouch. not good if someone removes the corresponding line out of the configuration file...
	Types[MATERIAL_ELEMENT_U].ddSpeedMultiplier = 2.0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cMaterialInfoTable::FindType_ByRefChar(char InRefChar)
{
	for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
	{
		if (Types[lc1].RefChar == InRefChar)
			return lc1;
	}
	return -1;
}