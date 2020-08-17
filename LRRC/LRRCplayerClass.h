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
 * File Name: LRRCplayerClass.h
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



/*LRRCplayerClass.cpp: Defines a class for references to submodels/parts/units within a scene file*/

#ifndef HEADER_LRRC_PLAYER_CLASS
#define HEADER_LRRC_PLAYER_CLASS

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;

#include "SHAREDvector.h"
#include "LRRCglobalDefs.h"
#include "LRRCunitClass.h"

#ifdef USE_ANN
#include "ANNexperienceClass.h"
#include "ANNneuronClass.h"
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



extern long PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT;
extern long PLAYER_ROUND_RITUAL_COMBAT_POINTS_ALLOCATED;
extern long PLAYER_ROUND_RITUAL_BUILDING_POINTS_ALLOCATED;
extern double PLAYER_MAXIMUM_BUILD_DISTANCE_FROM_STARTING_POST;		//the max distance any ModelDetails / building part can be initially placed away from the starting post
extern int BUILDING_DEFAULT_MOD;


void fillInPlayerClassExternVariables();

//#define NUMBER_OF_FREE_BUILDING_BRICKS_BASED_ON_BUILDING_WALLS_RATIO 14




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
	long credits;	//bank account balance
	int currentPhase;

	bool playerIsAI;

	bool status; //dead or alive

	UnitListClass * firstUnitInUnitList;
	UnitListClass * currentObjectInUnitList;	//temp variable for UnitListClass * initialisation	//is this used anymore?


#ifdef USE_ANN
	double creativity;

	/*
	Experience * firstExperience;		//not currently used, using unit experiences instead
	Experience * currentExperience;		//not currently used,using unit experiences instead		//temp variable for experience linked list creation
	*/
	Experience * firstExperience[GAME_NUMBER_OF_EXPERIENCE_NN];
	Experience * currentExperience[GAME_NUMBER_OF_EXPERIENCE_NN];		//temp variable for experience linked list creation

	NeuronContainer * firstInputNeuronInNetwork[GAME_NUMBER_OF_EXPERIENCE_NN];
	NeuronContainer * firstOutputNeuronInNetwork[GAME_NUMBER_OF_EXPERIENCE_NN];
	long numberOfInputNeurons[GAME_NUMBER_OF_EXPERIENCE_NN];
	long numberOfOutputNeurons[GAME_NUMBER_OF_EXPERIENCE_NN];
#endif

	Player * next;
};

void fillPlayerDetails(Player * p, string playerName, int playerID, int playerInitialCredits);
void fillPlayerDetails(Player * p, string playerName, int playerID, int playerInitialCredits, vec* playerStartPosition);

Player * findPlayer(Player * initialPlayerInList, int playerID);

#endif

