/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
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
 * File Name: LRRCgameReferenceManipulation.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3i19d 15-December-2016
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#ifndef HEADER_LRRC_GAME_REFERENCE_MANIPULATION
#define HEADER_LRRC_GAME_REFERENCE_MANIPULATION

#include "LDreferenceClass.h"

/*secondary game routines*/
LDreference* searchReferenceListRemoveReference(LDreference* initialReferenceInSceneFile, string unitFileName, int unitColour, bool* unitIDFound, bool* result);
LDreference* searchReferenceListFindReference(LDreference* initialReferenceInSceneFile, string unitFileName, int unitColour, bool* unitIDFound, bool* result);
/*
void searchReferenceListAssignHasPerformedCombat(int currentPhase, LDreference* initialReferenceInSceneFile, char* unitFileName, int unitColour, bool* unitIDFound, bool* result);
*/
bool obtainUserInputInt(int* userInputInt);

bool obtainUnitDetailsFromUserWOSceneRef(int currentPhase, string* unit1FileName, string* unit2FileName, int* unit1ID, int* unit2ID, string sceneFileName);
	bool obtainUnitDetailsFromUserForCombat(string* unit1FileName, string* unit2FileName, int* unit1ID, int* unit2ID, LDreference* initialReferenceInSceneFile);

		bool determineIfUnitsExists(int currentPhase, string unitAttackerFileName, string unitDefenderFileName, int unitAttackerPlayerID, int unitDefenderPlayerID, LDreference* initialReferenceInSceneFile);
			void parseRefListCheckUnitExists(int currentPhase, LDreference* initialReferenceInSceneFile, string unitFileName, int unitColour, bool* unitIDFound, bool* result);
		bool determineUnitNamesWithColours(int currentPhase, string* unit1FileName, string* unit2FileName, int unit1ID, int unit2ID, LDreference* initialReferenceInSceneFile);
			void parseRefListDetRefNames(int currentPhase, LDreference* reference, string* referenceName, int referenceColour, bool* unitIDFound, bool* result);

void searchReferenceListPrintReferenceDetails(LDreference* initialReferenceInSceneFile);

#endif




