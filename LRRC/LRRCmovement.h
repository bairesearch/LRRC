/*******************************************************************************
 *
 * File Name: LRRCmovement.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3a5g 01-Nov-2011
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
			Reference * compareReferenceCharacteristicsToThoseInPreMovementPhaseSceneReferenceList(Reference * referenceInThisPhaseStartSceneFile, Reference * referenceInPreMovementPhaseSceneFile, bool resultOfComparison[], int parentUnitSpeed, bool * unitIDFound, bool * result);
				bool compareSubmodelNamesAndIfSameCheckIfValidMove(Reference * referenceSearchedFor, Reference * referenceFoundDuringSearch, bool resultOfComparison[], int parentUnitSpeed);
			bool dealWithResultsOfComparison(Reference * referenceInThisPhaseStartSceneFile, bool resultOfComparison[], Player * currentPlayer, Reference * spriteListInitialReference, Reference * referenceInPreMovementPhaseSceneFile, int * numTargetSpritesAdded, char * targetSpritesSceneFileName, bool unitIDFound, bool isChildOfMovingReference);
		bool performFinalRoundPointsCalculations(Player * currentPlayer);

#endif
