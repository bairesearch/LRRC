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
 * File Name: LRRCmovement.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3a11b 09-July-2012
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#ifndef HEADER_LRRC_MOVEMENT
#define HEADER_LRRC_MOVEMENT

#include "LDreferenceClass.h"
#include "LRRCplayerClass.h"
#include "LRRCsprite.h"

bool compareSceneFilesMovementPhase(char* preMovementPhaseSceneFileName, char* thisPhaseStartSceneFileName, Player * currentPlayer, char * targetSpritesSceneFileName, bool addSprites);
	bool compareScenesMovementPhase(char * preMovementPhaseSceneFileName, Reference * initialReferenceInPreMovementPhaseScene, Reference * initialReferenceInThisPhaseStartScene, Player * currentPlayer, char * targetSpritesSceneFileName, bool addSprites);
		bool searchThisPhaseStartSceneReferenceListForReferenceComparisonInitiation(Reference * referenceInThisPhaseStartSceneFile, Reference * initialReferenceInPreMovementPhaseScene, int parentUnitSpeed, bool isAChildOfAMovingReference, Player * currentPlayer, char * targetSpritesSceneFileName, Reference * spriteListInitialReference, int * numTargetSpritesAdded);
			Reference * compareReferenceCharacteristicsToThoseInPreMovementPhaseSceneReferenceList(Reference * referenceInThisPhaseStartSceneFileBeingLocated, Reference * referenceInPreMovementPhaseSceneFile, bool resultOfComparison[], int parentUnitSpeed, bool * unitIDFound, bool * result);
				bool compareSubmodelNamesAndIfSameCheckIfValidMove(Reference * referenceInThisPhaseStartSceneSearchedFor, Reference * referenceInPreMovementPhaseSceneFoundDuringSearch, bool resultOfComparison[], int parentUnitSpeed);
			bool dealWithResultsOfComparison(Reference * referenceInThisPhaseStartSceneFile, bool resultOfComparison[], Player * currentPlayer, Reference * spriteListInitialReference, Reference * referenceInPreMovementPhaseSceneFile, int * numTargetSpritesAdded, char * targetSpritesSceneFileName, bool unitIDFound, bool isChildOfMovingReference);
		bool performFinalRoundPointsCalculations(Player * currentPlayer);

#endif
