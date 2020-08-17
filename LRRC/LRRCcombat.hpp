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
 * File Name: LRRCcombat.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3n7d 17-August-2020
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#ifndef HEADER_LRRC_COMBAT
#define HEADER_LRRC_COMBAT

#include "LDreferenceClass.hpp"
#include "LDparser.hpp"
#include "LRRCrules.hpp"
#include "LRRCparser.hpp"
#include "SHAREDvector.hpp"
#include "SHAREDvars.hpp"

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




class LRRCcombatClass
{
	private: LRRCmodelClassClass LRRCmodelClass;
	private: LDparserClass LDparser;
	private: LDreferenceClassClass LDreferenceClass;
	private: SHAREDvarsClass SHAREDvars;
	private: SHAREDvectorClass SHAREDvector;
	public: void fillInCombatExternVariables();

//combat routines supported by rules checker software (see preconditions and postconditions of functions for their requirements)
//* ******
	public: bool calculateUnitClassBasic(string unitFileName);
	public: int performCloseCombatBasic(string unit1FileName, string unit2FileName, const bool unit1performsCloseCombatAttack, const bool unit2performsCloseCombatAttack, const bool unit1TakesTheInitative, const bool unit2TakesTheInitative);
	public: int performLongDistanceCombatBasic(string unit1FileName, string unit2FileName, bool unit1performsLongDistanceAttack, bool unit2performsLongDistanceAttack);

//* ******
	public: int performLongDistanceCombatWithSceneFile(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, bool unit1intendsToPerformLongDistanceAttack, bool unit2intendsToPerformLongDistanceAttack, string thisPhaseStartSceneFileName);
		public: int performCloseCombatWithScene(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, const bool unit1intendsToPerformCloseCombatAttack, const bool unit2intendsToPerformCloseCombatAttack, LDreference* initialReferenceInThisPhaseStartScene);
	public: int performCloseCombatWithSceneFile(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, const bool unit1intendsToPerformCloseCombatAttack, const bool unit2intendsToPerformCloseCombatAttack, string thisPhaseStartSceneFileName);
		public: int performCloseCombatWithConsecutiveScenes(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, const bool unit1intendsToPerformCloseCombatAttack, const bool unit2intendsToPerformCloseCombatAttack, LDreference* initialReferenceInThisPhaseStartScene, LDreference* initialReferenceInPreMovementPhaseScene);
	public: int performCloseCombatWithConsecutiveSceneFiles(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, const bool unit1intendsToPerformCloseCombatAttack, const bool unit2intendsToPerformCloseCombatAttack, string thisPhaseStartSceneFileName, string preMovementPhaseSceneFileName);
		public: int performLongDistanceCombatWithScene(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, bool unit1intendsToPerformLongDistanceAttack, bool unit2intendsToPerformLongDistanceAttack, LDreference* initialReferenceInThisPhaseStartScene);
	public: int performLongDistanceCombatWithConsecutiveSceneFiles(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, const bool unit1intendsToPerformLongDistanceAttack, const bool unit2intendsToPerformLongDistanceAttack, string thisPhaseStartSceneFileName, string preMovementPhaseSceneFileName);
		public: int performLongDistanceCombatWithConsecutiveScenes(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, const bool unit1intendsToPerformLongDistanceAttack, const bool unit2intendsToPerformLongDistanceAttack, LDreference* initialReferenceInThisPhaseStartScene, LDreference* initialReferenceInPreMovementPhaseScene);

//* ******
	public: bool calculateUnitClassNormal(string unitFileName);
	public: int performCloseCombatNormal(string unit1FileName, string unit2FileName, const bool unit1performsCloseCombatAttack, const bool unit2performsCloseCombatAttack);
	public: int performLongDistanceCombatNormal(string unit1FileName, string unit2FileName, bool unit1performsLongDistanceAttack, bool unit2performsLongDistanceAttack);

//combat unit properties post parser (reference list searching)
	public: bool searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(LDreference* referenceBeingSearchedFor, LDreference* initialReference, const LDreference* parentReference, const bool isFillingParentUnitWithAllCombatRelevantChildModelDetails);
	private: bool compareSubmodelNamesAndIfSameCopySubmodelReference(LDreference* referenceBeingSearchedFor, LDreference* referenceCurrentlyFoundDuringSearch);

//combat routines invoked with well established combat parameters and ModelDetails details
	public: int performCloseCombatWithReferences(LDreference* unit1ReferenceInThisPhaseStartSceneFile, LDreference* unit2ReferenceInThisPhaseStartSceneFile, const bool unit1intendsToPerformCloseCombatAttack, const bool unit2intendsToPerformCloseCombatAttack, const bool unit1TakesTheInitative, const bool unit2TakesTheInitative);
		private: int performCloseCombat(ModelDetails* unit1, ModelDetails* unit2, const bool unit1performsCloseCombatAttack, const bool unit2performsCloseCombatAttack, const bool unit1TakesTheInitative, const bool unit2TakesTheInitative);
	public: int performLongDistanceCombatWithReferences(LDreference* unit1ReferenceInThisPhaseStartSceneFile, LDreference* unit2ReferenceInThisPhaseStartSceneFile, bool unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, bool unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack);
		private: int performLongDistanceCombat(const ModelDetails* unit1, const ModelDetails* unit2, bool unit1performsLongDistanceAttack, bool unit2performsLongDistanceAttack);


	public: void performFinalUnitClassCalculations(ModelDetails* u);
	public: void performFinalUnitGroupClassCalculations(ModelDetails* u);
	private: int calculateCloseCombatAttackBonus(ModelDetails* unit, const bool unit2HasHorse, const bool unitTakesInitative);
	public: void calculateLongDistanceAttackBonus(LDreference* unit1ReferenceInThisPhaseStartSceneFile, LDreference* unit2ReferenceInThisPhaseStartSceneFile, const bool unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, const bool unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, bool* unit1CanPerformLongDistanceAttack, bool* unit2CanPerformLongDistanceAttack, const double distanceBetweenUnitsForLDTest);
	private: bool performUnitOrdinatesCheck(const ModelDetails* u);

//range calculations
	private: bool calculateIfTwoUnitsAreWithinAGivenLongRangeAttackDistance(const double positionXOfUnit1, const double positionXOfUnit2, const double positionYOfUnit1, const double positionYOfUnit2, const double positionZOfUnit1, const double positionZOfUnit2, const int maximumDistance, const int minimumDistance);
	public: double calculateTheDistanceBetweenTwoUnits(vec* positionOfUnit1, vec* positionOfUnit2);
	public: double calculateTheDistanceBetweenTwoUnits(const double positionXOfUnit1, const double positionXOfUnit2, const double positionYOfUnit1, const double positionYOfUnit2, const double positionZOfUnit1, const double positionZOfUnit2);
	private: bool calculateIfUnitIsWithinAGivenLongRangeAttackDistance(const double distanceBetweenTheTwoUnits, const int maximumDistance, const int minimumDistance);
//double calculateExtraHorizontalDistanceOfProjectileWithHeightAdvantage(double verticalHeightAdvantage, double maxHorizRangeOfWeapon);
	private: int calculateLongDistanceRangeModifier(const LDreference* targetUnitReference, const LDreference* unitReference);
};



#endif

