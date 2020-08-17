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
 * File Name: LRRCgameAI.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3n7b 17-August-2020
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

#include "LRRCglobalDefs.hpp"

#ifdef USE_ANN

#include "ANNexperienceClass.hpp"
#include "LRRCplayerClass.hpp"
#include "LRRCunitClass.hpp"
#include "LDreferenceClass.hpp"
	//#include "ORoperations.hpp"
#include "LDparser.hpp"
#include "LRRCgameReferenceManipulation.hpp"
#include "LRRCrules.hpp"
#include "LRRCcombat.hpp"
#include "ANNneuronClass.hpp"
#include "ANNformation.hpp"
#include "ANNalgorithmBackpropagationTraining.hpp"
#include "ANNxmlConversion.hpp"
#include "ANNalgorithmBackpropagationUpdate.hpp"
#include "ANNdisplay.hpp"
#include "RTppm.hpp"


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

class LRRCgameAIClass
{
	private: ANNexperienceClassClass ANNexperienceClass;
	private: ANNformationClass ANNformation;
	private: LDparserClass LDparser;
	private: LRRCmodelClassClass LRRCmodelClass;
	private: LRRCcombatClass LRRCcombat;
	private: LRRCgameReferenceManipulationClass LRRCgameReferenceManipulation;
	private: LRRCunitClassClass LRRCunitClass;
	private: ANNalgorithmBackpropagationTrainingClass ANNalgorithmBackpropagationTraining;
	private: ANNdisplayClass ANNdisplay;
	private: SHAREDvarsClass SHAREDvars;
	private: RTppmClass RTppm;
	private: RTsceneClass RTscene;
	private: RTpixelMapsClass RTpixelMaps;
	private: RTreferenceManipulationClass RTreferenceManipulation;
	public: void initialiseNeuralNetwork(const int NNBeingTested, Player* currentPlayer, int currentPhase);

	private: long mergePlayerUnitExperiencesIntoPlayerExperienceList(Player* currentPlayer, UnitListClass* firstUnitInUnitGroup, const int unitGroupTeam, int nnIndex);
	private: long mergeAllUnitExperiencesIntoPlayerExperienceList(Player* currentPlayer, UnitListClass* firstUnitInUnitGroup, int nnIndex);


	private: void parseSceneFileAndFillUnitLists(string sceneFileName, UnitListClass* firstUnitInUnitList, int currentRound);
		public: void fillUnitList(LDreference* currentReferenceInSceneFile, UnitListClass* firstUnitInUnitGroup, int currentRound);

	public: void parseSceneFileAndUpdateUnitList(string sceneFileName, UnitListClass* firstUnitInUnitList, int currentRound);
		private: void updateUnitList(LDreference* initialReferenceInSceneFile, UnitListClass* firstUnitInUnitList, int currentRound);	//required after every round
			private: void updateUnitListWithNewUnits(LDreference* currentReferenceInSceneFile, UnitListClass* firstUnitInUnitGroup, int currentRound);
				private: UnitListClass* searchUnitListForUnitAndIfNotThereAddToList(UnitListClass* firstUnitInUnitGroup, LDreference* unitReferenceInSceneFile, int currentRound, const bool topLevelInTree, bool* foundUnitInList);
			private: void updateUnitListWithDeadUnits(LDreference* initialReferenceInSceneFile, UnitListClass* firstUnitInUnitGroup, int currentRound);



	public: bool determineIfUnitGroupHasAliveUnits(const UnitListClass* firstUnitInUnitGroup);
	public: bool checkAverageKillRatioForUnitGroup(UnitListClass* firstUnitInUnitGroup);
		private: void determineAverageKillRatioForUnitGroup(UnitListClass* firstUnitInUnitGroup, int* killPoints, int* deathPoints);


#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
	private: double addOrCompareAllObjectExperiences(Player* currentPlayer, const int NNBeingTested, const bool addOrCompareExperience, const bool justCountNumberOfExperiences);
#endif

	public: double addExperiencesFromUnitDecision(UnitListClass* unit, LDreference* unitReference, LDreference* unitReferenceOpponent, long unitDecision, LDreference* initialReferenceInThisPhaseStartScene, int NNcurrentPhase, Player* currentPlayer);
		public: double addOrCompareExperienceFromUnitDecision(int currentPhase, UnitListClass* unit, LDreference* unitReference, LDreference* unitReferenceOpponent, long unitDecision, Player* currentPlayer, const int NNBeingTested, const bool addOrCompareExperience, LDreference* initialReferenceInThisPhaseStartScene);
			private: void generateExperienceFromUnitPropertiesDecision(LDreference* unitReference, LDreference* unitReferenceOpponent, long unitDecision, ANNexperience* currentExperience, int currentPhase);
			private: void generateExperienceFromUnitCombatDecision(LDreference* unitReference, LDreference* unitReferenceOpponent, long unitDecision, ANNexperience* currentExperience, int currentPhase);
		#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
			public: bool generateExperienceFromGlobalDecision(const UnitListClass* firstUnitInUnitList, LDreference* initialReferenceInThisPhaseStartScene, const LDreference* unitReference, const LDreference* unitReferenceOpponent, ANNexperience* currentExperience);
				private: bool generate2DMemoryMapExperience(const UnitListClass* firstUnitInGroup, LDreference* initialReferenceInThisPhaseStartScene, const LDreference* unitReference, ANNexperience* currentExperience);
					private: ANNexperienceInput* findExperienceInputIn2DMemoryMap(const int xPos, const int yPos, const int tPos, ANNexperience* currentExperience);
		#endif
		#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
			private: bool generateExperienceFromObjectDecision(const LDreference* objectReference, long objectDecision, ANNexperience* currentExperience, const bool createNewRayTracedImage);
		#endif


	private: void addAllUnitExperiencesToOFStream(ofstream* experienceDataSetOFStreamObject, UnitListClass* firstUnitInUnitGroup, const int nnIndex);
	private: void addPlayerUnitExperiencesToOFStream(ofstream* experienceDataSetOFStreamObject, UnitListClass* firstUnitInUnitGroup, const int unitGroupTeam, const int nnIndex);
};

#endif

#endif



