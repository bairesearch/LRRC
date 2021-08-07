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
 * File Name: LRRCrules.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: LD Rules Rounds Checker
 * Project Version: 3p4a 07-August-2021
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#ifndef HEADER_LRRC_RULES
#define HEADER_LRRC_RULES

#include "SHAREDglobalDefs.hpp"
#include "XMLparserClass.hpp"
#include "XMLrulesClass.hpp"

extern XMLrulesClass* LRRCrulesUnitTypeDetails;
extern XMLrulesClass* LRRCrulesUnitCombatDetailsDefenceHead;
extern XMLrulesClass* LRRCrulesUnitCombatDetailsDefenceTorso;
extern XMLrulesClass* LRRCrulesUnitCombatDetailsDefenceShield;
extern XMLrulesClass* LRRCrulesUnitCombatDetailsAttackCloseCombat;
extern XMLrulesClass* LRRCrulesUnitCombatDetailsAttackLongDistance;
extern XMLrulesClass* LRRCrulesBuildingDetails;
extern XMLrulesClass* LRRCrulesTerrainDetails;
extern XMLrulesClass* LRRCrulesUnitTypeCatagories;
extern XMLrulesClass* LRRCrulesMiscellaneous;
extern XMLrulesClass* LRRCrulesSprite;

//there really should be child classes of XMLrulesClass called LRRCrulesUnitTypeDetails, unitCombatDetailsAttack, and unitCombatDetailsDefence that inherit rulesObject and include unique parameters

class LRRCrulesClass
{
	private: XMLparserClassClass XMLparserClass;
	private: XMLrulesClassClass XMLrulesClassObject;
	public: bool parseLRRCrulesXMLfile();
		private: bool parseLRRCrulesTag(XMLparserTag* currentTag);
			private: bool parseTagUnitCombatDetails(XMLparserTag* currentTag);
				private: bool parseTagArmour(XMLparserTag* currentTag);
					private: bool parseTagHead(XMLparserTag* currentTag);
					private: bool parseTagTorso(XMLparserTag* currentTag);
					private: bool parseTagShield(XMLparserTag* currentTag);
				private: bool parseTagWeapons(XMLparserTag* currentTag);
					private: bool parseTagCloseCombat(XMLparserTag* currentTag);
					private: bool parseTagLongDistanceCombat(XMLparserTag* currentTag);
};




#endif
