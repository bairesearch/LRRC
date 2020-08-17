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
 * File Name: LRRCunitClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3n7d 17-August-2020
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 * NB phase specific sprites are yet to be added to scene files based upon movement/combat outcomes
 * NB phase specific sprites are yet to be added to scene files based upon future movement/combat options
 *******************************************************************************/


#ifndef HEADER_LRRC_UNIT_CLASS
#define HEADER_LRRC_UNIT_CLASS

#include "LRRCglobalDefs.hpp"
//#assert USE_ANN

#ifdef USE_ANN
#include "ANNexperienceClass.hpp"
#endif

#include "LRRCmodelClass.hpp"
#include "SHAREDvars.hpp"


#define GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN (0)
#define GAME_INDEX_OF_COMBAT_EXPERIENCE_NN (1)
#define GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN (2)
#define GAME_INDEX_OF_OBJECT_EXPERIENCE_NN (3)
#define GAME_NUMBER_OF_EXPERIENCE_NN_WITHOUT_SEPARATE_COMBAT_SCENARIOS (4)

#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
	#define GAME_NUMBER_OF_EXPERIENCE_NN_UTILISED_WITHOUT_SEPARATE_COMBAT_SCENARIOS (GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN+1)
#else
	#define GAME_NUMBER_OF_EXPERIENCE_NN_UTILISED_WITHOUT_SEPARATE_COMBAT_SCENARIOS (GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN)
#endif

#define ANN_SEPARATE_CC_FROM_LD_NETWORK_COMBAT_SCENARIO_USED_FOR_MOVEMENT_PHASE (GAME_PHASE_CLOSECOMBAT)

#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK		//the following will be tested with the test harness first
	#define ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS (2) //CC Attack and LD Attack
	#define GAME_NUMBER_OF_EXPERIENCE_NN (GAME_NUMBER_OF_EXPERIENCE_NN_WITHOUT_SEPARATE_COMBAT_SCENARIOS* 2)
	#define GAME_NUMBER_OF_EXPERIENCE_NN_UTILISED (GAME_NUMBER_OF_EXPERIENCE_NN_UTILISED_WITHOUT_SEPARATE_COMBAT_SCENARIOS* 2)

#else
	#define ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS (1)
	#define GAME_NUMBER_OF_EXPERIENCE_NN (GAME_NUMBER_OF_EXPERIENCE_NN_WITHOUT_SEPARATE_COMBAT_SCENARIOS)
	#define GAME_NUMBER_OF_EXPERIENCE_NN_UTILISED (GAME_NUMBER_OF_EXPERIENCE_NN_UTILISED_WITHOUT_SEPARATE_COMBAT_SCENARIOS)
#endif
#define TH_LRRC_GAME_MAX_TRAINING_FOLDS (10)
#define GAME_INDEX_OF_ALL_EXPERIENCES_NN (GAME_NUMBER_OF_EXPERIENCE_NN_UTILISED)


#define GAME_UNIT_POINTS_LONG_DISTANCE_RANGE_MODIFIER (10)
	//eg a long bow with range 20, and long distance attack value d4 [level 2], recieves (20/10=2 + 2) = 4 points.
	//eg a crossbow with range 15, and long distance attack value d6 [level 3], recieves (15/10=1 + 3) = 4 points
	//eg a spearman with range 10, and long distance attack value d8 [level 3], recieves (10/10=1 + 3) = 4 points
#define GAME_UNIT_POINTS_MOVEMENT_RANGE_MODIFIER (5)
	//eg a knight receives (20) 20/5 = 4 points
	//eg a footsoldier (10) receives 10/5 = 2 points

#define ALIVE (true)
#define DEAD (false)

class UnitListClass
{
private:
	/*There are currently no private attributes of this class*/
public:

	UnitListClass(void); // constructor
	~UnitListClass();	// and destructor.

	int number;
	bool status;	//alive/dead
	int roundSpawned;
	int roundRemoved;
	int numKills;
	int killPoints;

	/*temporary per player turn variables*/
	bool hasPerformedAction[GAME_PHASES_NUM_PER_TURN];

	string name;	//currently used identifier of the unit in game scene files
	int team;		//not currently used, as there is 1 UnitListClass created for every team
	//int64_t id;		//the unique id/number for every unit - not currently used
	ModelDetails* unitDetails;

	bool isUnitGroup;
	UnitListClass* firstUnitInUnitGroup;

#ifdef USE_ANN

	ANNexperience* firstExperience[GAME_NUMBER_OF_EXPERIENCE_NN];
	ANNexperience* currentExperience[GAME_NUMBER_OF_EXPERIENCE_NN];		//temp variable for experience linked list creation

	double selfLearningTempVarBackPropagationPassError[GAME_NUMBER_OF_EXPERIENCE_NN];	//temp variable used for determination of whether to use this unit for a certain game action (Eg Combat)
	bool selfLearningTempVarUnitHasBeenTried;
	bool selfLearningTempVarUnitPrefersLongDistanceCombatToMovement;

#endif

	UnitListClass* next;

};

class LRRCunitClassClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: UnitListClass* searchUnitListFindOpponentWithLowestError(const int currentPlayerTurn, const int nn, UnitListClass* firstUnitInUnitGroup, double* currentLowestError, bool* foundOpponent, const int NNcurrentPhase);

	public: UnitListClass* searchUnitListFindUnit(UnitListClass* firstUnitInUnitGroup, const string unitName, const int unitColour, bool* unitIDFound);

	private: void addUnitToList(UnitListClass* firstUnitInUnitList, string unitName, int unitColour, ModelDetails* unitDetails, int currentRound);
	private: bool splitUnitGroup(UnitListClass* firstUnitInUnitList, const string unitGroupName, const int unitGroupColour, int currentRound);

	public: void searchUnitListAssignHasNotPerformedAction(UnitListClass* firstUnitInUnitGroup);

	public: int determineUnitWorthInPoints(const ModelDetails* unitDetails);
};

#endif




