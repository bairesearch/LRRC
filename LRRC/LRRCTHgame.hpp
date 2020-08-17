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
 * File Name: LRRCTHgame.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3n7c 17-August-2020
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#ifndef HEADER_TH_LRRC_GAME
#define HEADER_TH_LRRC_GAME

#include "LRRCglobalDefs.hpp"
#include "LRRCgame.hpp"
#include "LRRCmovement.hpp"
#include "LRRCcombat.hpp"
#include "LRRCsprite.hpp"
#include "LRRCgameReferenceManipulation.hpp"
#include "LRRCrules.hpp"
#include "LRRCgameAI.hpp"
#include "LRRCunitClass.hpp"
#include "LRRCplayerClass.hpp"
//#include "LDreferenceClass.hpp"
#include "LDparser.hpp"
#include "LDreferenceManipulation.hpp"
//#include "LDsprite.hpp"
#include "RTreferenceManipulation.hpp"
#ifdef USE_RT
#include "RTscene.hpp"
#endif
#include "ANNneuronClass.hpp"
#include "ANNformation.hpp"
#include "ANNxmlConversion.hpp"
#include "ANNalgorithmBackpropagationTraining.hpp"
#include "ANNalgorithmBackpropagationUpdate.hpp"
#include "ANNdisplay.hpp"


#ifdef DEMONSTRATION_SCENARIO1
	#define TH_LRRC_GAME_NUMBER_OF_UNITS_TO_FEED (10)	//default = 50
#else
	#define TH_LRRC_GAME_NUMBER_OF_UNITS_TO_FEED (50)	//default = 50
#endif

//#define TH_LRRC_GAME_DO_NOT_CHECK_EVASION_IN_NN_ERROR

#ifndef DEBUG_DO_NOT_USE_RANDOM_DICE_DURING_COMBAT
	#define TH_LRRC_GAME_USE_AVERAGED_COMBAT_OUTCOMES
	#define TH_LRRC_GAME_NUM_AVERAGED_COMBAT_OUTCOMES (30)
	#define TH_LRRC_GAME_AVERAGED_COMBAT_OUTCOMES_GAIN_REQUIRED_PERCENTAGE (10) 	//percent
#endif

#ifdef TH_LRRC_GAME_EMULATE_GAME_AI_REQUIREMENTS
	#define TH_LRRC_GAME_USE_RANDOM_DISTANCES_TO_TRAIN_NETWORKS
	#define TH_LRRC_GAME_NUM_RANDOM_DISTANCES_TO_TRAIN_NETWORKS (10)
	#define TH_LRRC_GAME_RANGE_OF_RANDOM_LD_DISTANCES_TO_TRAIN_NETWORK (40)		//40 Ldraw units in distance (also = CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY* TH_LRRC_GAME_NUM_RANDOM_DISTANCES_TO_TRAIN_NETWORKS)
		//this should be the same as the maximum LD attack distance as calculated in LRRCgameAI's generateCombat/PropertiesExperience() function
#else
	//#define DEBUG_TH_DO_NOT_USE_ADVANCED_LEARNING_BASED_UPON_DISTANCES
#endif




class LRRCTHgameClass
{
	private: LRRCplayerClassClass LRRCplayerClass;
	private: LRRCmodelClassClass LRRCmodelClass;
	private: LRRCrulesClass LRRCrules;
	private: LRRCspriteClass LRRCsprite;
	private: LRRCcombatClass LRRCcombat;
	private: LRRCparserClass LRRCparser;
	private: LRRCunitClassClass LRRCunitClass;
	private: LRRCmovementClass LRRCmovement;
	private: LRRCgameClass LRRCgame;
	private: LRRCgameAIClass LRRCgameAI;

	private: SHAREDvectorClass SHAREDvector;

	int THtestANNusingCombatExperiences();
		long THgenerateUnitListwithAllUnitProperties(UnitListClass* firstUnitInUnitList);
			bool generateUnitDetails(ModelDetails* unitDetails, int selectionUnitTypeCatagories, int selectionUnitCombatDetailsAttackCloseCombat, int selectionUnitCombatDetailsAttackLongDistance, int selectionUnitCombatDetailsDefenceHead, int selectionUnitCombatDetailsDefenceTorso, int selectionUnitCombatDetailsDefenceShield);
		bool randomiseUnitList(UnitListClass* firstUnitInNonRandomisedUnitList, UnitListClass* firstUnitInUnitList, long numberOfUnits);
		bool cullUnitList(UnitListClass* firstUnitInUnitList, int numberOfUnitsRequired);
		double THperformGenericCombatWithAllPermutationsOfUnitsInListAndAddOrCompareExperiences(int currentPhase, UnitListClass* firstUnitInUnitList, Player* currentPlayer, int NNBeingTested, bool addOrCompareExperience, int numberOfDistancesToTrainNetwork);
			double THperformGenericCombatWithTwoCombatReadyUnitsAndAddOrCompareExperience(int currentPhase, UnitListClass* unitAttacker, UnitListClass* unitDefender, Player* currentPlayer, int NNBeingTested, bool addOrCompareExperience, int numberOfDistancesToTrainNetwork);
				#ifdef TH_LRRC_GAME_USE_AVERAGED_COMBAT_OUTCOMES
				int repeatCombatAndFindMostSuccessfulContendor(int currentPhase, LDreference* unitAttackerReference, LDreference* unitDefenderReference, int numberOfCombatSequences, bool addOrCompareExperience);
				#endif
};


#endif


