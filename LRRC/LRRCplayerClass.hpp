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
 * File Name: LRRCplayerClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: LD Rules Rounds Checker
 * Project Version: 3n7e 17-August-2020
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


/*LRRCplayerClass.cpp: Defines a class for references to submodels/parts/units within a scene file*/

#ifndef HEADER_LRRC_PLAYER_CLASS
#define HEADER_LRRC_PLAYER_CLASS

#include "SHAREDvector.hpp"
#include "LRRCglobalDefs.hpp"
#include "LRRCunitClass.hpp"

#ifdef USE_ANN
#include "ANNexperienceClass.hpp"
#include "ANNneuronClass.hpp"
#include "LRRCrules.hpp"
#define DEFAULT_AI_PLAYER_CREATIVITY (0.3)
#define PERFORM_MOVEMENT_FOR_CC_BIAS (2.0)	//PERFORM_MOVEMENT_FOR_CC_BIAS*DEFAULT_AI_PLAYER_CREATIVITY must be less than 1
#endif

#define PLAYER_NAME_MAX_LENGTH (100)
#define PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT_NAME "PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT"
//#define PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT (1000)


/*???
#define PLAYER_STARTING_POST_X_POSITION 0
#define PLAYER_STARTING_POST_Y_POSITION 0
#define PLAYER_STARTING_POST_Z_POSITION 0
*/


//Player Credits Calculation Definitions

#define PLAYER_ROUND_RITUAL_COMBAT_POINTS_ALLOCATED_NAME "PLAYER_ROUND_RITUAL_COMBAT_POINTS_ALLOCATED"
#define PLAYER_ROUND_RITUAL_BUILDING_POINTS_ALLOCATED_NAME "PLAYER_ROUND_RITUAL_BUILDING_POINTS_ALLOCATED"
#define PLAYER_MAXIMUM_BUILD_DISTANCE_FROM_STARTING_POST_NAME "PLAYER_MAXIMUM_BUILD_DISTANCE_FROM_STARTING_POST"		//the max distance any ModelDetails / building part can be initially placed away from the starting post
/*
#define PLAYER_ROUND_RITUAL_COMBAT_POINTS_ALLOCATED 1000
#define PLAYER_ROUND_RITUAL_BUILDING_POINTS_ALLOCATED 1000
#define PLAYER_MAXIMUM_BUILD_DISTANCE_FROM_STARTING_POST 100.0		//the max distance any ModelDetails / building part can be initially placed away from the starting post
*/

#define BUILDING_DEFAULT_MOD_NAME "BUILDING_DEFAULT_MOD"
//#define BUILDING_DEFAULT_MOD 10



extern int64_t PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT;
extern int64_t PLAYER_ROUND_RITUAL_COMBAT_POINTS_ALLOCATED;
extern int64_t PLAYER_ROUND_RITUAL_BUILDING_POINTS_ALLOCATED;
extern double PLAYER_MAXIMUM_BUILD_DISTANCE_FROM_STARTING_POST;		//the max distance any ModelDetails / building part can be initially placed away from the starting post
extern int BUILDING_DEFAULT_MOD;

class Player
{
private:
	/*There are currently no private attributes of this class*/
public:

	Player(void); // constructor declaration
	//Player(string playerName, int playerID, int playerInitialCredits, vec* playerStartPosition);
	//Player(string playerName, int playerID, int playerInitialCredits);
	~Player();	//  and destructor.

	vec startPosition;
	string name;
	int id;			//player ID - Eg colour/team
	int64_t credits;	//bank account balance
	int currentPhase;

	bool playerIsAI;

	bool status; //dead or alive

	UnitListClass* firstUnitInUnitList;
	UnitListClass* currentObjectInUnitList;	//temp variable for UnitListClass* initialisation	//is this used anymore?

	#ifdef USE_ANN
	double creativity;

	/*
	ANNexperience* firstExperience;		//not currently used, using unit experiences instead
	ANNexperience* currentExperience;		//not currently used,using unit experiences instead		//temp variable for experience linked list creation
	*/
	ANNexperience* firstExperience[GAME_NUMBER_OF_EXPERIENCE_NN];
	ANNexperience* currentExperience[GAME_NUMBER_OF_EXPERIENCE_NN];		//temp variable for experience linked list creation

	ANNneuron* firstInputNeuronInNetwork[GAME_NUMBER_OF_EXPERIENCE_NN];
	ANNneuron* firstOutputNeuronInNetwork[GAME_NUMBER_OF_EXPERIENCE_NN];
	int64_t numberOfInputNeurons[GAME_NUMBER_OF_EXPERIENCE_NN];
	int64_t numberOfOutputNeurons[GAME_NUMBER_OF_EXPERIENCE_NN];
	#endif

	Player* next;
};

class LRRCplayerClassClass
{
	private: SHAREDvectorClass SHAREDvector;
	public: void fillInPlayerClassExternVariables();

	//#define NUMBER_OF_FREE_BUILDING_BRICKS_BASED_ON_BUILDING_WALLS_RATIO 14

	public: void fillPlayerDetails(Player* p, string playerName, int playerID, int playerInitialCredits);
	public: void fillPlayerDetails(Player* p, string playerName, int playerID, int playerInitialCredits, vec* playerStartPosition);

	public: Player* findPlayer(Player* initialPlayerInList, const int playerID);
};

#endif

