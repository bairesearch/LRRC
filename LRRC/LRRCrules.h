/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: LRRCrules.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3i19c 15-December-2016
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#ifndef HEADER_LRRC_RULES
#define HEADER_LRRC_RULES

#include "SHAREDglobalDefs.h"
#include "XMLparserClass.h"
#include "XMLrulesClass.h"

//there really should be child classes of XMLrulesClass called LRRCrulesUnitTypeDetails, unitCombatDetailsAttack, and unitCombatDetailsDefence that inherit rulesObject and include unique parameters


bool parseLRRCrulesXMLfile();
	bool parseLRRCrulesTag(XMLparserTag* currentTag);
		bool parseTagUnitCombatDetails(XMLparserTag* currentTag);
			bool parseTagArmour(XMLparserTag* currentTag);
				bool parseTagHead(XMLparserTag* currentTag);
				bool parseTagTorso(XMLparserTag* currentTag);
				bool parseTagShield(XMLparserTag* currentTag);
			bool parseTagWeapons(XMLparserTag* currentTag);
				bool parseTagCloseCombat(XMLparserTag* currentTag);
				bool parseTagLongDistanceCombat(XMLparserTag* currentTag);


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


#endif
