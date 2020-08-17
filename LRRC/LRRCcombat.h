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
 * File Name: LRRCcombat.h
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


#ifndef HEADER_LRRC_COMBAT
#define HEADER_LRRC_COMBAT

#include "LDreferenceClass.h"

#define CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY_NAME "CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY"
#define RANGE_MODIFIER_BASED_ON_INCREASED_HEIGHT_NAME "RANGE_MODIFIER_BASED_ON_INCREASED_HEIGHT"
//#define CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY (4)		//distance given in cm
//#define RANGE_MODIFIER_BASED_ON_INCREASED_HEIGHT //0.2	//distance given in cm

#define ABS_OF_SHORT 32767.0
	//this is the same as RAND_MAX C++ constant

#define ATTACK_ERROR_FILE_IO 0
#define ATTACK_NEITHER_UNIT_CAN_STRIKE 1
#define ATTACK_BOTH_UNITS_STRIKE_BOTH_CAN_STRIKE 2
#define ATTACK_NEITHER_UNITS_STRIKE_BOTH_CAN_STRIKE 3
#define ATTACK_UNIT1_STRIKES_BOTH_CAN_STRIKE 4
#define ATTACK_UNIT1_STRIKES_UNIT1_CAN_STRIKE 5
#define ATTACK_NEITHER_STRIKES_UNIT1_CAN_STRIKE 6
#define ATTACK_UNIT2_STRIKES_BOTH_CAN_STRIKE 7
#define ATTACK_UNIT2_STRIKES_UNIT2_CAN_STRIKE 8
#define ATTACK_NEITHER_STRIKES_UNIT2_CAN_STRIKE 9
#define TOTAL_NUMBER_OF_COMBAT_OUTCOMES 10

#define DEFAULT_COLOUR_OF_SUB_MODEL_CURRENTLY_BEING_SEARCHED_FOR 0





extern double CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY;
extern double RANGE_MODIFIER_BASED_ON_INCREASED_HEIGHT;




void fillInCombatExternVariables();

//combat routines supported by rules checker software (see preconditions and postconditions of functions for their requirements)
//* ******
bool calculateUnitClassBasic(char* unitFileName);
int performCloseCombatBasic(char* unit1FileName, char* unit2FileName, bool unit1performsCloseCombatAttack, bool unit2performsCloseCombatAttack, bool unit1TakesTheInitative, bool unit2TakesTheInitative);
int performLongDistanceCombatBasic(char* unit1FileName, char* unit2FileName, bool unit1performsLongDistanceAttack, bool unit2performsLongDistanceAttack);

//* ******
int performLongDistanceCombatWithSceneFile(char* unit1FileName, char* unit2FileName, int unit1ID, int unit2ID, bool unit1intendsToPerformLongDistanceAttack, bool unit2intendsToPerformLongDistanceAttack, char* thisPhaseStartSceneFileName);
	int performCloseCombatWithScene(char* unit1FileName, char* unit2FileName, int unit1ID, int unit2ID, bool unit1intendsToPerformCloseCombatAttack, bool unit2intendsToPerformCloseCombatAttack, LDreference* initialReferenceInThisPhaseStartScene);
int performCloseCombatWithSceneFile(char* unit1FileName, char* unit2FileName, int unit1ID, int unit2ID, bool unit1intendsToPerformCloseCombatAttack, bool unit2intendsToPerformCloseCombatAttack, char* thisPhaseStartSceneFileName);
	int performCloseCombatWithConsecutiveScenes(char* unit1FileName, char* unit2FileName, int unit1ID, int unit2ID, bool unit1intendsToPerformCloseCombatAttack, bool unit2intendsToPerformCloseCombatAttack, LDreference* initialReferenceInThisPhaseStartScene, LDreference* initialReferenceInPreMovementPhaseScene);
int performCloseCombatWithConsecutiveSceneFiles(char* unit1FileName, char* unit2FileName, int unit1ID, int unit2ID, bool unit1intendsToPerformCloseCombatAttack, bool unit2intendsToPerformCloseCombatAttack, char* thisPhaseStartSceneFileName, char* preMovementPhaseSceneFileName);
	int performLongDistanceCombatWithScene(char* unit1FileName, char* unit2FileName, int unit1ID, int unit2ID, bool unit1intendsToPerformLongDistanceAttack, bool unit2intendsToPerformLongDistanceAttack, LDreference* initialReferenceInThisPhaseStartScene);
int performLongDistanceCombatWithConsecutiveSceneFiles(char* unit1FileName, char* unit2FileName, int unit1ID, int unit2ID, bool unit1intendsToPerformLongDistanceAttack, bool unit2intendsToPerformLongDistanceAttack, char* thisPhaseStartSceneFileName, char* preMovementPhaseSceneFileName);
	int performLongDistanceCombatWithConsecutiveScenes(char* unit1FileName, char* unit2FileName, int unit1ID, int unit2ID, bool unit1intendsToPerformLongDistanceAttack, bool unit2intendsToPerformLongDistanceAttack, LDreference* initialReferenceInThisPhaseStartScene, LDreference* initialReferenceInPreMovementPhaseScene);

//* ******
bool calculateUnitClassNormal(char* unitFileName);
int performCloseCombatNormal(char* unit1FileName, char* unit2FileName, bool unit1performsCloseCombatAttack, bool unit2performsCloseCombatAttack);
int performLongDistanceCombatNormal(char* unit1FileName, char* unit2FileName, bool unit1performsLongDistanceAttack, bool unit2performsLongDistanceAttack);

//combat unit properties post parser (reference list searching)
bool searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(LDreference* referenceBeingSearchedFor, LDreference* initialReference, LDreference* parentReference, bool isFillingParentUnitWithAllCombatRelevantChildModelDetails);
bool compareSubmodelNamesAndIfSameCopySubmodelReference(LDreference* referenceBeingSearchedFor, LDreference* referenceCurrentlyFoundDuringSearch);

//combat routines invoked with well established combat parameters and ModelDetails details
int performCloseCombatWithReferences(LDreference* unit1ReferenceInThisPhaseStartSceneFile, LDreference* unit2ReferenceInThisPhaseStartSceneFile, bool unit1intendsToPerformCloseCombatAttack, bool unit2intendsToPerformCloseCombatAttack, bool unit1TakesTheInitative, bool unit2TakesTheInitative);
	int performCloseCombat(ModelDetails* unit1, ModelDetails* unit2, bool unit1performsCloseCombatAttack, bool unit2performsCloseCombatAttack, bool unit1TakesTheInitative, bool unit2TakesTheInitative);
int performLongDistanceCombatWithReferences(LDreference* unit1ReferenceInThisPhaseStartSceneFile, LDreference* unit2ReferenceInThisPhaseStartSceneFile, bool unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, bool unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack);
	int performLongDistanceCombat(ModelDetails* unit1, ModelDetails* unit2, bool unit1performsLongDistanceAttack, bool unit2performsLongDistanceAttack);


void performFinalUnitClassCalculations(ModelDetails* u);
void performFinalUnitGroupClassCalculations(ModelDetails* u);
int calculateCloseCombatAttackBonus(ModelDetails* unit, bool unit2HasHorse, bool unitTakesInitative);
void calculateLongDistanceAttackBonus(LDreference* unit1ReferenceInThisPhaseStartSceneFile, LDreference* unit2ReferenceInThisPhaseStartSceneFile, bool unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, bool unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, bool* unit1CanPerformLongDistanceAttack, bool* unit2CanPerformLongDistanceAttack, double distanceBetweenUnitsForLDTest);
bool performUnitOrdinatesCheck(ModelDetails* u);

//range calculations
bool calculateIfTwoUnitsAreWithinAGivenLongRangeAttackDistance(double positionXOfUnit1, double positionXOfUnit2, double positionYOfUnit1, double positionYOfUnit2, double positionZOfUnit1, double positionZOfUnit2, int maximumDistance, int minimumDistance);
double calculateTheDistanceBetweenTwoUnits(vec* positionOfUnit1, vec* positionOfUnit2);
double calculateTheDistanceBetweenTwoUnits(double positionXOfUnit1, double positionXOfUnit2, double positionYOfUnit1, double positionYOfUnit2, double positionZOfUnit1, double positionZOfUnit2);
bool calculateIfUnitIsWithinAGivenLongRangeAttackDistance(double distanceBetweenTheTwoUnits, int maximumDistance, int minimumDistance);
//double calculateExtraHorizontalDistanceOfProjectileWithHeightAdvantage(double verticalHeightAdvantage, double maxHorizRangeOfWeapon);
int calculateLongDistanceRangeModifier(LDreference* targetUnitReference, LDreference* unitReference);



#endif

