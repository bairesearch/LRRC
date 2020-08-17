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
 * File Name: LRRCmovement.hpp
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


#ifndef HEADER_LRRC_MOVEMENT
#define HEADER_LRRC_MOVEMENT

#include "LDreferenceClass.hpp"
#include "LRRCplayerClass.hpp"
#include "LRRCsprite.hpp"
#include "LRRCcombat.hpp"
#include "LRRCparser.hpp"
#include "LDparser.hpp"
#include "LDreferenceManipulation.hpp"
#include "LRRCrules.hpp"

class LRRCmovementClass
{
	private: LDparserClass LDparser;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: LRRCmodelClassClass LRRCmodelClass;
	private: LRRCcombatClass LRRCcombat;
	private: LDreferenceClassClass LDreferenceClass;
	private: LRRCspriteClass LRRCsprite;
	private: LRRCparserClass LRRCparser;
	public: bool compareSceneFilesMovementPhase(string preMovementPhaseSceneFileName, string thisPhaseStartSceneFileName, Player* currentPlayer, string targetSpritesSceneFileName, const bool addSprites);
		public: bool compareScenesMovementPhase(const string preMovementPhaseSceneFileName, LDreference* initialReferenceInPreMovementPhaseScene, LDreference* initialReferenceInThisPhaseStartScene, Player* currentPlayer, string targetSpritesSceneFileName, const bool addSprites);
			private: bool searchThisPhaseStartSceneReferenceListForReferenceComparisonInitiation(LDreference* referenceInThisPhaseStartSceneFile, LDreference* initialReferenceInPreMovementPhaseScene, const int parentUnitSpeed, const bool isAChildOfAMovingReference, const Player* currentPlayer, string targetSpritesSceneFileName, LDreference* spriteListInitialReference, int* numTargetSpritesAdded);
				private: LDreference* compareReferenceCharacteristicsToThoseInPreMovementPhaseSceneReferenceList(const LDreference* referenceInThisPhaseStartSceneFileBeingLocated, LDreference* referenceInPreMovementPhaseSceneFile, bool resultOfComparison[], const int parentUnitSpeed, bool* unitIDFound, bool* result);
					private: bool compareSubmodelNamesAndIfSameCheckIfValidMove(const LDreference* referenceInThisPhaseStartSceneSearchedFor, const LDreference* referenceInPreMovementPhaseSceneFoundDuringSearch, bool resultOfComparison[], const int parentUnitSpeed);
				private: bool dealWithResultsOfComparison(LDreference* referenceInThisPhaseStartSceneFile, const bool resultOfComparison[], const Player* currentPlayer, LDreference* spriteListInitialReference, LDreference* referenceInPreMovementPhaseSceneFile, int* numTargetSpritesAdded, string targetSpritesSceneFileName, const bool unitIDFound, const bool isChildOfMovingReference);
			private: bool performFinalRoundPointsCalculations(Player* currentPlayer);
};

#endif
