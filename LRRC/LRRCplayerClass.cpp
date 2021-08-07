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
 * File Name: LRRCplayerClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: LD Rules Rounds Checker
 * Project Version: 3p4a 07-August-2021
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


/*LDreferenceClass.cpp: Defines a class for references to submodels/parts/units within a scene file*/

#include "LRRCplayerClass.hpp"
//#include "LRRCmodelClass.hpp"

int64_t PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT;
int64_t PLAYER_ROUND_RITUAL_COMBAT_POINTS_ALLOCATED;
int64_t PLAYER_ROUND_RITUAL_BUILDING_POINTS_ALLOCATED;
double PLAYER_MAXIMUM_BUILD_DISTANCE_FROM_STARTING_POST;
int BUILDING_DEFAULT_MOD;


Player::Player(void)
{
	//Define default values for Player

	name = "";
	id = 0;			//player ID - Eg colour
	credits = PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT;
	startPosition.x = 0.0;
	startPosition.y = 0.0;
	startPosition.z = 0.0;
	currentPhase = GAME_PHASE_GENERIC;
	playerIsAI = false;
	status = true;

	firstUnitInUnitList = NULL;
	currentObjectInUnitList = NULL;

	#ifdef USE_ANN
	creativity = DEFAULT_AI_PLAYER_CREATIVITY;		//a large degree of creativity is assigned to player for now...

	/*
	firstExperience = new ANNexperience();
	currentExperience = firstExperience;
	*/
	for(int nn = 0; nn < GAME_NUMBER_OF_EXPERIENCE_NN; nn++)
	{
		firstExperience[nn] = new ANNexperience();
		currentExperience[nn] = firstExperience[nn];
	}

	for(int i=0; i<GAME_NUMBER_OF_EXPERIENCE_NN; i++)
	{
		firstInputNeuronInNetwork[i] = NULL;
		firstOutputNeuronInNetwork[i] = NULL;
		numberOfInputNeurons[i] = 0;
		numberOfOutputNeurons[i] = 0;
	}
	#endif

	next = NULL;
}

Player::~Player()
{

	#ifdef USE_ANN
	for(int i = 0; i< GAME_NUMBER_OF_EXPERIENCE_NN; i++)
	{
		if(firstExperience[i] != NULL)
		{
			delete firstExperience[i];
		}
	}

	/*
	if(firstExperience != NULL)
	{
		delete firstExperience;
	}
	*/
	#endif

	#ifdef USE_SEPARATE_UNIT_LISTS_PER_PLAYER
	if(firstUnitInUnitList != NULL)
	{
		delete firstUnitInUnitList;
	}
	#endif

	#ifdef USE_SEPARATE_AI_PER_PLAYER
	for(int i = 0; i< GAME_NUMBER_OF_EXPERIENCE_NN; i++)
	{
		if(firstInputNeuronInNetwork[i] != NULL)
		{
			delete firstInputNeuronInNetwork[i];
		}
	}
	#endif

	if(next != NULL)
	{
		delete next;
	}


}


void LRRCplayerClassClass::fillInPlayerClassExternVariables()
{
	XMLrulesClass* currentReferenceRulesClass = LRRCrulesMiscellaneous;

	while(currentReferenceRulesClass->next != NULL)
	{
		if(currentReferenceRulesClass->name == PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT_NAME)
		{
			PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT = int64_t(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == PLAYER_ROUND_RITUAL_COMBAT_POINTS_ALLOCATED_NAME)
		{
			PLAYER_ROUND_RITUAL_COMBAT_POINTS_ALLOCATED = int64_t(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == PLAYER_ROUND_RITUAL_BUILDING_POINTS_ALLOCATED_NAME)
		{
			PLAYER_ROUND_RITUAL_BUILDING_POINTS_ALLOCATED = int64_t(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == PLAYER_MAXIMUM_BUILD_DISTANCE_FROM_STARTING_POST_NAME)
		{
			PLAYER_MAXIMUM_BUILD_DISTANCE_FROM_STARTING_POST = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == BUILDING_DEFAULT_MOD_NAME)
		{
			BUILDING_DEFAULT_MOD = int(currentReferenceRulesClass->fractionalValue);
		}
		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
}


void LRRCplayerClassClass::fillPlayerDetails(Player* p, string playerName, int playerID, int playerInitialCredits)
{
	p->name = playerName;
	p->id = playerID;			//player ID - Eg colour
	p->credits = playerInitialCredits;
}

void LRRCplayerClassClass::fillPlayerDetails(Player* p, string playerName, int playerID, int playerInitialCredits, vec* playerStartPosition)
{
	p->name = playerName;
	p->id = playerID;			//player ID - Eg colour
	p->credits = playerInitialCredits;
	SHAREDvector.copyVectors(&(p->startPosition), playerStartPosition);
}

Player* LRRCplayerClassClass::findPlayer(Player* initialPlayerInList, const int playerID)
{
	Player* playerThatWasLocated = NULL;

	Player* currentPlayerInList = initialPlayerInList;
	while(currentPlayerInList->next != NULL)
	{
		if(currentPlayerInList->id == playerID)
		{
			playerThatWasLocated = currentPlayerInList;
		}
		currentPlayerInList = currentPlayerInList->next;
	}

	return playerThatWasLocated;
}


