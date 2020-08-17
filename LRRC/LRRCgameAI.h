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
 * File Name: LRRCgameAI.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3i19d 15-December-2016
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 * NB phase specific sprites are yet to be added to scene files based upon movement/combat outcomes
 * NB phase specific sprites are yet to be added to scene files based upon future movement/combat options
 *******************************************************************************/


#ifndef HEADER_LRRC_GAME_AI
#define HEADER_LRRC_GAME_AI

#include "LRRCglobalDefs.h"

#ifdef USE_ANN

#include "ANNexperienceClass.h"
#include "LRRCplayerClass.h"
#include "LRRCunitClass.h"
#include "LDreferenceClass.h"



//ANN Defs;

#ifdef TEST_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_THEN_CONVERGING
	#define GAME_NUMBER_OF_NN_LAYERS (3)		//default = 5
#else
	#define GAME_NUMBER_OF_NN_LAYERS (4)
#endif
#define GAME_NUMBER_OF_NN_LAYERS_GLOBAL_EXPERIENCE (7)	//this used to be 10
#define GAME_NUMBER_OF_NN_LAYERS_OBJECT_EXPERIENCE (7)	//this used to be 10
//#define TH_ANN_WITH_LRRC_OBJECT_RECOGNITION_EXPERIENCES_MAX_NUMBER_OF_EPOCHS 1000		//not used - NB 2D NNs do not use folds during training as each (image) experience has a different class target
#define	LRRC_OBJECT_RECOGNITION_EXPERIENCES_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS 1000
#define	LRRC_MAX_NUMBER_OF_EPOCHS 50
#define	LRRC_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS 50





#define CONTRAST_MAP_GENERATION_KERNEL_WIDTH (3)
#define CONTRAST_MAP_GENERATION_KERNEL_HEIGHT (3)

#define PIXMAP_NUMBER_OF_RGB_COLOURS (3)
#define LUMINOSITY_MAP_MAX_RGB_VALUE (256)
#define LUMINOSITY_MAP_MAX_LUMINOSITY_VALUE (LUMINOSITY_MAP_MAX_RGB_VALUE*PIXMAP_NUMBER_OF_RGB_COLOURS)
#define CONTRAST_MAP_MAX_CONTRAST_VALUE (LUMINOSITY_MAP_MAX_LUMINOSITY_VALUE)
#define CONTRAST_MAP_THRESHOLD_VALUE (100)	//33/256 or 100/768







//Combat/Unit ANNexperience Defs;

#define COMBAT_TYPE_CLOSE_COMBAT (1)
#define COMBAT_TYPE_LONG_DISTANCE (2)


#define GAME_ANN_MAX_ERROR (0.01)

#define COMBAT_EXPERIENCE_OUTPUT_DECISION_IGNORE 0				//ignore target and continue moving towards goal (/ not worth attacking and not worth evading opponent)
#define COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_CLOSECOMBAT 1		//decided to attack {or move to attack} opponent as it is considered easy (/ profitable for the mission)
#define COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_LONGDISTANCE 2				//decided to move closer to the unit [possibly for close combat/long range attack]
#define COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_CLOSECOMBAT 3				//decide not to attack opponent in CC - being attacked will result a serious danger (/ danger to the mission) and therefore must be evaded at the cost of a time delay
#define COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_LONGDISTANCE 4				//decide not to attack opponent in LD - being attacked will result a serious danger (/ danger to the mission) and therefore must be evaded at the cost of a time delay

#define COMBAT_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS 5
#define PROPERTIES_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS (COMBAT_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS)

	//COMBAT_EXPERIENCE_OUTPUT_DECISION_IGNORE and COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE may be merged in the future...

//OLD: #define COMBAT_EXPERIENCE_OUTPUT_DECISION_MOVE_TO_ATTACK 3
//OLD: #define COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_LONGDISTANCE 4

#ifdef DEBUG_HAVNT_YET_STARTED_TESTING_UNITGROUPS
#define MAXIMUM_UNIT_GROUP_SIZE (1)
#else
#define MAXIMUM_UNIT_GROUP_SIZE (10)
#endif
#define COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX (MAXIMUM_UNIT_GROUP_SIZE)
#define COMBAT_EXPERIENCE_MAXIMUM_LEVEL (6)

#define GLOBAL_EXPERIENCE_RADIUS (50)
#define GLOBAL_EXPERIENCE_INTERVAL (25)
#define GLOBAL_EXPERIENCE_NUM_INPUT_TYPES (4)
#define GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS (GLOBAL_EXPERIENCE_RADIUS/GLOBAL_EXPERIENCE_INTERVAL)
#define GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_1D (((GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS)*2)+1)
#define GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS (GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_1D*GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_1D)
#define GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_INPUT_DECISIONS (GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_1D*GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_1D*GLOBAL_EXPERIENCE_NUM_INPUT_TYPES)



#define GAME_AI_NN_EXPERIENCE_RECORD_MIN_KILL_RATIO_REQUIREMENT (1.0)


#define ADD_EXPERIENCE (true)
#define COMPARE_EXPERIENCE (false)


/*
//not used
void addExperienceToPlayer(ANNexperience* experience, Player* player);
void addExperienceToUnit(ANNexperience* experience, ModelDetails* unit);
void addExperienceInputToExperience(ANNexperienceInput* experienceInput, ANNexperience* experience);
*/

void initialiseNeuralNetwork(int NNBeingTested, Player* currentPlayer, int currentPhase);

long mergePlayerUnitExperiencesIntoPlayerExperienceList(Player* currentPlayer, UnitListClass* firstUnitInUnitGroup, int unitGroupTeam, int nnIndex);
long mergeAllUnitExperiencesIntoPlayerExperienceList(Player* currentPlayer, UnitListClass* firstUnitInUnitGroup, int nnIndex);


void parseSceneFileAndFillUnitLists(string sceneFileName, UnitListClass* firstUnitInUnitList, int currentRound);
	void fillUnitList(LDreference* currentReferenceInSceneFile, UnitListClass* firstUnitInUnitGroup, int currentRound);

void parseSceneFileAndUpdateUnitList(string sceneFileName, UnitListClass* firstUnitInUnitList, int currentRound);
	void updateUnitList(LDreference* initialReferenceInSceneFile, UnitListClass* firstUnitInUnitList, int currentRound);	//required after every round
		void updateUnitListWithNewUnits(LDreference* currentReferenceInSceneFile, UnitListClass* firstUnitInUnitGroup, int currentRound);
			UnitListClass* searchUnitListForUnitAndIfNotThereAddToList(UnitListClass* firstUnitInUnitGroup, LDreference* unitReferenceInSceneFile, int currentRound, bool topLevelInTree, bool* foundUnitInList);
		void updateUnitListWithDeadUnits(LDreference* initialReferenceInSceneFile, UnitListClass* firstUnitInUnitGroup, int currentRound);



bool determineIfUnitGroupHasAliveUnits(UnitListClass* firstUnitInUnitGroup);
bool checkAverageKillRatioForUnitGroup(UnitListClass* firstUnitInUnitGroup);
	void determineAverageKillRatioForUnitGroup(UnitListClass* firstUnitInUnitGroup, int* killPoints, int* deathPoints);


#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
double addOrCompareAllObjectExperiences(Player* currentPlayer, int NNBeingTested, bool addOrCompareExperience, bool justCountNumberOfExperiences);
#endif

double addExperiencesFromUnitDecision(UnitListClass* unit, LDreference* unitReference, LDreference* unitReferenceOpponent, long unitDecision, LDreference* initialReferenceInThisPhaseStartScene, int NNcurrentPhase, Player* currentPlayer);
	double addOrCompareExperienceFromUnitDecision(int currentPhase, UnitListClass* unit, LDreference* unitReference, LDreference* unitReferenceOpponent, long unitDecision, Player* currentPlayer, int NNBeingTested, bool addOrCompareExperience, LDreference* initialReferenceInThisPhaseStartScene);
		void generateExperienceFromUnitPropertiesDecision(LDreference* unitReference, LDreference* unitReferenceOpponent, long unitDecision, ANNexperience* currentExperience, int currentPhase);
		void generateExperienceFromUnitCombatDecision(LDreference* unitReference, LDreference* unitReferenceOpponent, long unitDecision, ANNexperience* currentExperience, int currentPhase);
		#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
		bool generateExperienceFromGlobalDecision(UnitListClass* firstUnitInUnitList, LDreference* initialReferenceInThisPhaseStartScene, LDreference* unitReference, LDreference* unitReferenceOpponent, ANNexperience* currentExperience);
			bool generate2DMemoryMapExperience(UnitListClass* firstUnitInGroup, LDreference* initialReferenceInThisPhaseStartScene, LDreference* unitReference, ANNexperience* currentExperience);
				ANNexperienceInput* findExperienceInputIn2DMemoryMap(int xPos, int yPos, int tPos, ANNexperience* currentExperience);
		#endif
		#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
		bool generateExperienceFromObjectDecision(LDreference* objectReference, long objectDecision, ANNexperience* currentExperience, bool createNewRayTracedImage);
		#endif


void addAllUnitExperiencesToOFStream(ofstream* experienceDataSetOFStreamObject, UnitListClass* firstUnitInUnitGroup, int nnIndex);
void addPlayerUnitExperiencesToOFStream(ofstream* experienceDataSetOFStreamObject, UnitListClass* firstUnitInUnitGroup, int unitGroupTeam, int nnIndex);

#endif

#endif



