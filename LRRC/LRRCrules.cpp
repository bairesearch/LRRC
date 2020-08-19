/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: LRRCrules.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: LD Rules Rounds Checker
 * Project Version: 3n7e 17-August-2020
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#include "LRRCrules.hpp"

//xml comments not yet supported by this parser

#define LRRC_RULES_XML_FILE_NAME "LRRCrules.xml"

#define RULES_XML_TAG_unitTypeDetails ((string)"unitTypeDetails")
#define RULES_XML_TAG_part ((string)"part")
#define RULES_XML_TAG_unitCombatDetails ((string)"unitCombatDetails")
#define RULES_XML_TAG_armour ((string)"armour")
#define RULES_XML_TAG_head ((string)"head")
#define RULES_XML_TAG_torso ((string)"torso")
#define RULES_XML_TAG_shield ((string)"shield")
#define RULES_XML_TAG_weapons ((string)"weapons")
#define RULES_XML_TAG_closeCombat ((string)"closeCombat")
#define RULES_XML_TAG_longDistance ((string)"longDistance")
#define RULES_XML_TAG_other ((string)"other")
#define RULES_XML_TAG_buildingDetails ((string)"buildingDetails")
#define RULES_XML_TAG_terrainDefenceDetails ((string)"terrainDefenceDetails")
#define RULES_XML_TAG_unitTypeCatagories ((string)"unitTypeCatagories")
#define RULES_XML_TAG_unitType ((string)"unitType")
#define RULES_XML_TAG_miscellaneous ((string)"miscellaneous")

#define RULES_XML_ATTRIBUTE_partID ((string)"partID")
#define RULES_XML_ATTRIBUTE_partIDLen ((string)"partIDLen")
#define RULES_XML_ATTRIBUTE_defenceMod ((string)"defenceMod")
#define RULES_XML_ATTRIBUTE_closeCombatAttackMod ((string)"closeCombatAttackMod")
#define RULES_XML_ATTRIBUTE_mountedAttackBonus ((string)"mountedAttackBonus")
#define RULES_XML_ATTRIBUTE_attackBonusAgainstMounted ((string)"attackBonusAgainstMounted")
#define RULES_XML_ATTRIBUTE_longDistanceAttackMod ((string)"longDistanceAttackMod")
#define RULES_XML_ATTRIBUTE_longDistanceRange ((string)"longDistanceRange")
#define RULES_XML_ATTRIBUTE_enum ((string)"enum")
#define RULES_XML_ATTRIBUTE_maxTravel ((string)"maxTravel")
#define RULES_XML_ATTRIBUTE_buildingMod ((string)"buildingMod")

XMLrulesClass* LRRCrulesUnitTypeDetails;
XMLrulesClass* LRRCrulesUnitCombatDetailsDefenceHead;
XMLrulesClass* LRRCrulesUnitCombatDetailsDefenceTorso;
XMLrulesClass* LRRCrulesUnitCombatDetailsDefenceShield;
XMLrulesClass* LRRCrulesUnitCombatDetailsAttackCloseCombat;
XMLrulesClass* LRRCrulesUnitCombatDetailsAttackLongDistance;
XMLrulesClass* LRRCrulesBuildingDetails;
XMLrulesClass* LRRCrulesTerrainDetails;
XMLrulesClass* LRRCrulesUnitTypeCatagories;
XMLrulesClass* LRRCrulesMiscellaneous;
XMLrulesClass* LRRCrulesSprite;

static string nullString;

bool LRRCrulesClass::parseLRRCrulesXMLfile()
{
	bool result = true;

 	XMLparserTag* firstTagInXMLFile = new XMLparserTag();	//the firstTagInXMLFile object must be initialised here (in XMLrulesClass.cpp scope). if it is initialised in XMLParserClass.cpp else it will be come corrupted,
 	if(!XMLparserClass.readXMLfile(LRRC_RULES_XML_FILE_NAME, firstTagInXMLFile))
 	{
		result = false;
	}

	/*
	#define TEMP_XML_FILE_NAME "temp.xml"
 	if(!writeXMLfile(TEMP_XML_FILE_NAME, firstTagInXMLFile))
 	{
		result = false;
	}
	*/
	
	XMLparserTag* currentTag = firstTagInXMLFile;

	LRRCrulesUnitTypeDetails = new XMLrulesClass();
	LRRCrulesUnitCombatDetailsDefenceHead = new XMLrulesClass();
	LRRCrulesUnitCombatDetailsDefenceTorso = new XMLrulesClass();
	LRRCrulesUnitCombatDetailsDefenceShield = new XMLrulesClass();
	LRRCrulesUnitCombatDetailsAttackCloseCombat = new XMLrulesClass();
	LRRCrulesUnitCombatDetailsAttackLongDistance = new XMLrulesClass();
	LRRCrulesBuildingDetails = new XMLrulesClass();
	LRRCrulesTerrainDetails = new XMLrulesClass();
	LRRCrulesUnitTypeCatagories = new XMLrulesClass();
	LRRCrulesMiscellaneous = new XMLrulesClass();
	LRRCrulesSprite = new XMLrulesClass();


	if(!parseLRRCrulesTag(currentTag))
	{
		result = false;
	}

	delete firstTagInXMLFile;


	/*
	XMLrulesClass* currentReferenceToObjectClass;

	currentReferenceToObjectClass = LRRCrulesUnitTypeDetails;
	while(currentReferenceToObjectClass->next != NULL)
	{
		cout << "name = " << currentReferenceToObjectClass->name << endl;
		cout << "stringValue = " << currentReferenceToObjectClass->stringValue << endl;
		cout << "fractionalValue = " << currentReferenceToObjectClass->fractionalValue << endl;
		cout << "attribute4 = " << currentReferenceToObjectClass->attribute4 << endl;
		cout << "attribute5 = " << currentReferenceToObjectClass->attribute5 << endl;
		cout << "attribute6 = " << currentReferenceToObjectClass->attribute6 << endl;
		cout << "attribute7 = " << currentReferenceToObjectClass->attribute7 << endl;
		cout << "attribute8 = " << currentReferenceToObjectClass->attribute8 << endl;
		currentReferenceToObjectClass = currentReferenceToObjectClass->next;
	}

	currentReferenceToObjectClass = LRRCrulesUnitCombatDetailsDefenceHead;
	while(currentReferenceToObjectClass->next != NULL)
	{
		cout << "name = " << currentReferenceToObjectClass->name << endl;
		cout << "stringValue = " << currentReferenceToObjectClass->stringValue << endl;
		cout << "fractionalValue = " << currentReferenceToObjectClass->fractionalValue << endl;
		cout << "attribute4 = " << currentReferenceToObjectClass->attribute4 << endl;
		cout << "attribute5 = " << currentReferenceToObjectClass->attribute5 << endl;
		cout << "attribute6 = " << currentReferenceToObjectClass->attribute6 << endl;
		cout << "attribute7 = " << currentReferenceToObjectClass->attribute7 << endl;
		cout << "attribute8 = " << currentReferenceToObjectClass->attribute8 << endl;
		currentReferenceToObjectClass = currentReferenceToObjectClass->next;
	}

	currentReferenceToObjectClass = LRRCrulesUnitCombatDetailsDefenceTorso;
	while(currentReferenceToObjectClass->next != NULL)
	{
		cout << "name = " << currentReferenceToObjectClass->name << endl;
		cout << "stringValue = " << currentReferenceToObjectClass->stringValue << endl;
		cout << "fractionalValue = " << currentReferenceToObjectClass->fractionalValue << endl;
		cout << "attribute4 = " << currentReferenceToObjectClass->attribute4 << endl;
		cout << "attribute5 = " << currentReferenceToObjectClass->attribute5 << endl;
		cout << "attribute6 = " << currentReferenceToObjectClass->attribute6 << endl;
		cout << "attribute7 = " << currentReferenceToObjectClass->attribute7 << endl;
		cout << "attribute8 = " << currentReferenceToObjectClass->attribute8 << endl;
		currentReferenceToObjectClass = currentReferenceToObjectClass->next;
	}

	currentReferenceToObjectClass = LRRCrulesUnitCombatDetailsDefenceShield;
	while(currentReferenceToObjectClass->next != NULL)
	{
		cout << "name = " << currentReferenceToObjectClass->name << endl;
		cout << "stringValue = " << currentReferenceToObjectClass->stringValue << endl;
		cout << "fractionalValue = " << currentReferenceToObjectClass->fractionalValue << endl;
		cout << "attribute4 = " << currentReferenceToObjectClass->attribute4 << endl;
		cout << "attribute5 = " << currentReferenceToObjectClass->attribute5 << endl;
		cout << "attribute6 = " << currentReferenceToObjectClass->attribute6 << endl;
		cout << "attribute7 = " << currentReferenceToObjectClass->attribute7 << endl;
		cout << "attribute8 = " << currentReferenceToObjectClass->attribute8 << endl;
		currentReferenceToObjectClass = currentReferenceToObjectClass->next;
	}

	currentReferenceToObjectClass = LRRCrulesUnitCombatDetailsAttackCloseCombat;
	while(currentReferenceToObjectClass->next != NULL)
	{
		cout << "name = " << currentReferenceToObjectClass->name << endl;
		cout << "stringValue = " << currentReferenceToObjectClass->stringValue << endl;
		cout << "fractionalValue = " << currentReferenceToObjectClass->fractionalValue << endl;
		cout << "attribute4 = " << currentReferenceToObjectClass->attribute4 << endl;
		cout << "attribute5 = " << currentReferenceToObjectClass->attribute5 << endl;
		cout << "attribute6 = " << currentReferenceToObjectClass->attribute6 << endl;
		cout << "attribute7 = " << currentReferenceToObjectClass->attribute7 << endl;
		cout << "attribute8 = " << currentReferenceToObjectClass->attribute8 << endl;
		currentReferenceToObjectClass = currentReferenceToObjectClass->next;
	}

	currentReferenceToObjectClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
	while(currentReferenceToObjectClass->next != NULL)
	{
		cout << "name = " << currentReferenceToObjectClass->name << endl;
		cout << "stringValue = " << currentReferenceToObjectClass->stringValue << endl;
		cout << "fractionalValue = " << currentReferenceToObjectClass->fractionalValue << endl;
		cout << "attribute4 = " << currentReferenceToObjectClass->attribute4 << endl;
		cout << "attribute5 = " << currentReferenceToObjectClass->attribute5 << endl;
		cout << "attribute6 = " << currentReferenceToObjectClass->attribute6 << endl;
		cout << "attribute7 = " << currentReferenceToObjectClass->attribute7 << endl;
		cout << "attribute8 = " << currentReferenceToObjectClass->attribute8 << endl;
		currentReferenceToObjectClass = currentReferenceToObjectClass->next;
	}

	currentReferenceToObjectClass = LRRCrulesBuildingDetails;
	while(currentReferenceToObjectClass->next != NULL)
	{
		cout << "name = " << currentReferenceToObjectClass->name << endl;
		cout << "stringValue = " << currentReferenceToObjectClass->stringValue << endl;
		cout << "fractionalValue = " << currentReferenceToObjectClass->fractionalValue << endl;
		cout << "attribute4 = " << currentReferenceToObjectClass->attribute4 << endl;
		cout << "attribute5 = " << currentReferenceToObjectClass->attribute5 << endl;
		cout << "attribute6 = " << currentReferenceToObjectClass->attribute6 << endl;
		cout << "attribute7 = " << currentReferenceToObjectClass->attribute7 << endl;
		cout << "attribute8 = " << currentReferenceToObjectClass->attribute8 << endl;
		currentReferenceToObjectClass = currentReferenceToObjectClass->next;
	}


	currentReferenceToObjectClass = LRRCrulesTerrainDetails;
	while(currentReferenceToObjectClass->next != NULL)
	{
		cout << "name = " << currentReferenceToObjectClass->name << endl;
		cout << "stringValue = " << currentReferenceToObjectClass->stringValue << endl;
		cout << "fractionalValue = " << currentReferenceToObjectClass->fractionalValue << endl;
		cout << "attribute4 = " << currentReferenceToObjectClass->attribute4 << endl;
		cout << "attribute5 = " << currentReferenceToObjectClass->attribute5 << endl;
		cout << "attribute6 = " << currentReferenceToObjectClass->attribute6 << endl;
		cout << "attribute7 = " << currentReferenceToObjectClass->attribute7 << endl;
		cout << "attribute8 = " << currentReferenceToObjectClass->attribute8 << endl;
		currentReferenceToObjectClass = currentReferenceToObjectClass->next;
	}


	currentReferenceToObjectClass = LRRCrulesUnitTypeCatagories;
	while(currentReferenceToObjectClass->next != NULL)
	{
		cout << "name = " << currentReferenceToObjectClass->name << endl;
		cout << "stringValue = " << currentReferenceToObjectClass->stringValue << endl;
		cout << "fractionalValue = " << currentReferenceToObjectClass->fractionalValue << endl;
		cout << "attribute4 = " << currentReferenceToObjectClass->attribute4 << endl;
		cout << "attribute5 = " << currentReferenceToObjectClass->attribute5 << endl;
		cout << "attribute6 = " << currentReferenceToObjectClass->attribute6 << endl;
		cout << "attribute7 = " << currentReferenceToObjectClass->attribute7 << endl;
		cout << "attribute8 = " << currentReferenceToObjectClass->attribute8 << endl;
		currentReferenceToObjectClass = currentReferenceToObjectClass->next;
	}


	currentReferenceToObjectClass = LRRCrulesMiscellaneous;
	while(currentReferenceToObjectClass->next != NULL)
	{
		cout << "name = " << currentReferenceToObjectClass->name << endl;
		cout << "stringValue = " << currentReferenceToObjectClass->stringValue << endl;
		cout << "fractionalValue = " << currentReferenceToObjectClass->fractionalValue << endl;
		cout << "attribute4 = " << currentReferenceToObjectClass->attribute4 << endl;
		cout << "attribute5 = " << currentReferenceToObjectClass->attribute5 << endl;
		cout << "attribute6 = " << currentReferenceToObjectClass->attribute6 << endl;
		cout << "attribute7 = " << currentReferenceToObjectClass->attribute7 << endl;
		cout << "attribute8 = " << currentReferenceToObjectClass->attribute8 << endl;
		currentReferenceToObjectClass = currentReferenceToObjectClass->next;
	}


	currentReferenceToObjectClass = LRRCrulesSprite;
	while(currentReferenceToObjectClass->next != NULL)
	{
		cout << "name = " << currentReferenceToObjectClass->name << endl;
		cout << "stringValue = " << currentReferenceToObjectClass->stringValue << endl;
		cout << "fractionalValue = " << currentReferenceToObjectClass->fractionalValue << endl;
		cout << "attribute4 = " << currentReferenceToObjectClass->attribute4 << endl;
		cout << "attribute5 = " << currentReferenceToObjectClass->attribute5 << endl;
		cout << "attribute6 = " << currentReferenceToObjectClass->attribute6 << endl;
		cout << "attribute7 = " << currentReferenceToObjectClass->attribute7 << endl;
		cout << "attribute8 = " << currentReferenceToObjectClass->attribute8 << endl;
		currentReferenceToObjectClass = currentReferenceToObjectClass->next;
	}
	*/

	return result;
}

bool LRRCrulesClass::parseLRRCrulesTag(XMLparserTag* currentTag)
{
	bool result = true;

	XMLparserTag* currentTagUpdated = currentTag;
	currentTagUpdated = XMLparserClass.parseTagDownALevel(currentTagUpdated, RULES_XML_TAG_rules, &result);
	if(result)
	{
		if(!XMLrulesClassObject.addRulesClassObjectsBasedOnSectionTag(currentTagUpdated, RULES_XML_TAG_unitTypeDetails, LRRCrulesUnitTypeDetails, RULES_XML_TAG_part, 3, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_partID, RULES_XML_ATTRIBUTE_partIDLen, nullString, nullString, nullString, nullString, nullString))
		{
			result = false;
		}
		currentTagUpdated=currentTagUpdated->nextTag;
		if(!parseTagUnitCombatDetails(currentTagUpdated))
		{
			result = false;
		}
		currentTagUpdated=currentTagUpdated->nextTag;
		if(!XMLrulesClassObject.addRulesClassObjectsBasedOnSectionTag(currentTagUpdated, RULES_XML_TAG_buildingDetails, LRRCrulesBuildingDetails, RULES_XML_TAG_part, 4, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_partID, RULES_XML_ATTRIBUTE_partIDLen, RULES_XML_ATTRIBUTE_buildingMod, nullString, nullString, nullString, nullString))
		{
			result = false;
		}
		currentTagUpdated=currentTagUpdated->nextTag;
		if(!XMLrulesClassObject.addRulesClassObjectsBasedOnSectionTag(currentTagUpdated, RULES_XML_TAG_terrainDefenceDetails, LRRCrulesTerrainDetails, RULES_XML_TAG_part, 3, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_partID, RULES_XML_ATTRIBUTE_partIDLen, nullString, nullString, nullString, nullString, nullString))
		{
			result = false;
		}
		currentTagUpdated=currentTagUpdated->nextTag;
		if(!XMLrulesClassObject.addRulesClassObjectsBasedOnSectionTag(currentTagUpdated, RULES_XML_TAG_unitTypeCatagories, LRRCrulesUnitTypeCatagories, RULES_XML_TAG_unitType, 5, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_stringValue, RULES_XML_ATTRIBUTE_enum, RULES_XML_ATTRIBUTE_maxTravel, RULES_XML_ATTRIBUTE_defenceMod, nullString, nullString, nullString))
		{
			result = false;
		}
		currentTagUpdated=currentTagUpdated->nextTag;
		if(!XMLrulesClassObject.addRulesClassObjectsBasedOnSectionTag(currentTagUpdated, RULES_XML_TAG_miscellaneous, LRRCrulesMiscellaneous, RULES_XML_TAG_miscellaneousItem, 3, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_stringValue, RULES_XML_ATTRIBUTE_fractionalValue, nullString, nullString, nullString, nullString, nullString))
		{
			result = false;
		}
		currentTagUpdated=currentTagUpdated->nextTag;
		if(!XMLrulesClassObject.addRulesClassObjectsBasedOnSectionTag(currentTagUpdated, RULES_XML_TAG_sprite, LRRCrulesSprite, RULES_XML_TAG_miscellaneousItem, 3, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_stringValue, RULES_XML_ATTRIBUTE_fractionalValue, nullString, nullString, nullString, nullString, nullString))
		{
			result = false;
		}
	}

	return result;
}

bool LRRCrulesClass::parseTagUnitCombatDetails(XMLparserTag* currentTag)
{
	bool result = true;

	XMLparserTag* currentTagUpdated = currentTag;
	currentTagUpdated = XMLparserClass.parseTagDownALevel(currentTagUpdated, RULES_XML_TAG_unitCombatDetails, &result);
	if(result)
	{
		if(!parseTagArmour(currentTagUpdated))
		{
			result = false;
		}
		currentTagUpdated=currentTagUpdated->nextTag;
		if(!parseTagWeapons(currentTagUpdated))
		{
			result = false;
		}
	}

	return result;
}

bool LRRCrulesClass::parseTagArmour(XMLparserTag* currentTag)
{
	bool result = true;

	XMLparserTag* currentTagUpdated = currentTag;
	currentTagUpdated = XMLparserClass.parseTagDownALevel(currentTagUpdated, RULES_XML_TAG_armour, &result);
	if(result)
	{
		if(!parseTagHead(currentTagUpdated))
		{
			result = false;
		}
		currentTagUpdated=currentTagUpdated->nextTag;
		if(!parseTagTorso(currentTagUpdated))
		{
			result = false;
		}
		currentTagUpdated=currentTagUpdated->nextTag;
		if(!parseTagShield(currentTagUpdated))
		{
			result = false;
		}
	}

	return result;
}

bool LRRCrulesClass::parseTagHead(XMLparserTag* currentTag)
{
	bool result = true;

	XMLrulesClassObject.addRulesClassObjectsBasedOnSectionTag(currentTag, RULES_XML_TAG_head, LRRCrulesUnitCombatDetailsDefenceHead, RULES_XML_TAG_part, 4, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_partID, RULES_XML_ATTRIBUTE_partIDLen, RULES_XML_ATTRIBUTE_defenceMod, nullString, nullString, nullString, nullString);

	return result;
}

bool LRRCrulesClass::parseTagTorso(XMLparserTag* currentTag)
{
	bool result = true;

	XMLrulesClassObject.addRulesClassObjectsBasedOnSectionTag(currentTag, RULES_XML_TAG_torso, LRRCrulesUnitCombatDetailsDefenceTorso, RULES_XML_TAG_part, 4, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_partID, RULES_XML_ATTRIBUTE_partIDLen, RULES_XML_ATTRIBUTE_defenceMod, nullString, nullString, nullString, nullString);

	return result;
}

bool LRRCrulesClass::parseTagShield(XMLparserTag* currentTag)
{
	bool result = true;

	XMLrulesClassObject.addRulesClassObjectsBasedOnSectionTag(currentTag, RULES_XML_TAG_shield, LRRCrulesUnitCombatDetailsDefenceShield, RULES_XML_TAG_part, 4, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_partID, RULES_XML_ATTRIBUTE_partIDLen, RULES_XML_ATTRIBUTE_defenceMod, nullString, nullString, nullString, nullString);

	return result;
}

bool LRRCrulesClass::parseTagWeapons(XMLparserTag* currentTag)
{
	bool result = true;

	XMLparserTag* currentTagUpdated = currentTag;
	currentTagUpdated = XMLparserClass.parseTagDownALevel(currentTagUpdated, RULES_XML_TAG_weapons, &result);
	if(result)
	{
		if(!parseTagCloseCombat(currentTagUpdated))
		{
			result = false;
		}
		currentTagUpdated=currentTagUpdated->nextTag;
		if(!parseTagLongDistanceCombat(currentTagUpdated))
		{
			result = false;
		}
	}

	return result;
}

bool LRRCrulesClass::parseTagCloseCombat(XMLparserTag* currentTag)
{
	bool result = true;

	XMLrulesClassObject.addRulesClassObjectsBasedOnSectionTag(currentTag, RULES_XML_TAG_closeCombat, LRRCrulesUnitCombatDetailsAttackCloseCombat, RULES_XML_TAG_part, 6, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_partID, RULES_XML_ATTRIBUTE_partIDLen, RULES_XML_ATTRIBUTE_closeCombatAttackMod, RULES_XML_ATTRIBUTE_mountedAttackBonus, RULES_XML_ATTRIBUTE_attackBonusAgainstMounted, nullString, nullString);

	return result;
}

bool LRRCrulesClass::parseTagLongDistanceCombat(XMLparserTag* currentTag)
{
	bool result = true;

	XMLrulesClassObject.addRulesClassObjectsBasedOnSectionTag(currentTag, RULES_XML_TAG_longDistance, LRRCrulesUnitCombatDetailsAttackLongDistance, RULES_XML_TAG_part, 5, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_partID, RULES_XML_ATTRIBUTE_partIDLen, RULES_XML_ATTRIBUTE_longDistanceAttackMod, RULES_XML_ATTRIBUTE_longDistanceRange, nullString, nullString, nullString);

	return result;
}

