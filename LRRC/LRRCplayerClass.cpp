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
 * File Name: LRRCplayerClass.cpp
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


/*LDreferenceClass.cpp: Defines a class for references to submodels/parts/units within a scene file*/

#include "LRRCplayerClass.h"
//#include "LRRCmodelClass.h"

long PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT;
long PLAYER_ROUND_RITUAL_COMBAT_POINTS_ALLOCATED;
long PLAYER_ROUND_RITUAL_BUILDING_POINTS_ALLOCATED;
double PLAYER_MAXIMUM_BUILD_DISTANCE_FROM_STARTING_POST;
int BUILDING_DEFAULT_MOD;


void LRRCplayerClassClass::fillInPlayerClassExternVariables()
{
	XMLrulesClass* currentReferenceRulesClass = LRRCrulesMiscellaneous;

	while(currentReferenceRulesClass->next != NULL)
	{
		if(currentReferenceRulesClass->name == PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT_NAME)
		{
			PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT = long(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == PLAYER_ROUND_RITUAL_COMBAT_POINTS_ALLOCATED_NAME)
		{
			PLAYER_ROUND_RITUAL_COMBAT_POINTS_ALLOCATED = long(currentReferenceRulesClass->fractionalValue);
		}
		else if(currentReferenceRulesClass->name == PLAYER_ROUND_RITUAL_BUILDING_POINTS_ALLOCATED_NAME)
		{
			PLAYER_ROUND_RITUAL_BUILDING_POINTS_ALLOCATED = long(currentReferenceRulesClass->fractionalValue);
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


