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
 * File Name: LRRCgameReferenceManipulation.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3j2a 17-January-2017
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#ifndef HEADER_LRRC_GAME_REFERENCE_MANIPULATION
#define HEADER_LRRC_GAME_REFERENCE_MANIPULATION

#include "LDreferenceClass.hpp"
#include "LDreferenceManipulation.hpp"
#include "SHAREDvector.hpp"
#include "LDparser.hpp"

/*secondary game routines*/
class LRRCgameReferenceManipulationClass
{
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: SHAREDvarsClass SHAREDvars;
	private: LDparserClass LDparser;
	public: LDreference* searchReferenceListRemoveReference(LDreference* initialReferenceInSceneFile, const string unitFileName, const int unitColour, bool* unitIDFound, bool* result);
	public: LDreference* searchReferenceListFindReference(LDreference* initialReferenceInSceneFile, const string unitFileName, const int unitColour, bool* unitIDFound, bool* result);
/*
void searchReferenceListAssignHasPerformedCombat(int currentPhase, LDreference* initialReferenceInSceneFile, char* unitFileName, int unitColour, bool* unitIDFound, bool* result);
*/
	public: bool obtainUserInputInt(int* userInputInt);

	public: bool obtainUnitDetailsFromUserWOSceneRef(const int currentPhase, string* unit1FileName, string* unit2FileName, int* unit1ID, int* unit2ID, string sceneFileName);
		public: bool obtainUnitDetailsFromUserForCombat(string* unit1FileName, string* unit2FileName, int* unit1ID, int* unit2ID, const LDreference* initialReferenceInSceneFile);

			private: bool determineIfUnitsExists(const int currentPhase, const string unitAttackerFileName, const string unitDefenderFileName, const int unitAttackerPlayerID, const int unitDefenderPlayerID, const LDreference* initialReferenceInSceneFile);
				private: void parseRefListCheckUnitExists(const int currentPhase, const LDreference* initialReferenceInSceneFile, const string unitFileName, const int unitColour, bool* unitIDFound, bool* result);
			private: bool determineUnitNamesWithColours(const int currentPhase, string* unit1FileName, string* unit2FileName, const int unit1ID, const int unit2ID, LDreference* initialReferenceInSceneFile);
				private: void parseRefListDetRefNames(const int currentPhase, LDreference* reference, string* referenceName, const int referenceColour, bool* unitIDFound, bool* result);

	public: void searchReferenceListPrintReferenceDetails(const LDreference* initialReferenceInSceneFile);
};

#endif




