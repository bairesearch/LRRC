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
