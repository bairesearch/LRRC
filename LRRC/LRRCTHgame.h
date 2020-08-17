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
 * File Name: LRRCTHgame.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3j1b 14-January-2017
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#ifndef HEADER_TH_LRRC_GAME
#define HEADER_TH_LRRC_GAME

//#include "LDreferenceClass.h"
#include "LRRCplayerClass.h"
#include "LRRCglobalDefs.h"

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

#endif


