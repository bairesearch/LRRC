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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: LD Rules Rounds Checker
 * Project Version: 3n7e 17-August-2020
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 * NB phase specific sprites are yet to be added to scene files based upon movement/combat outcomes
 * NB phase specific sprites are yet to be added to scene files based upon future movement/combat options
 *******************************************************************************/


#include "LRRCunitClass.hpp"


UnitListClass::UnitListClass(void)
{
	number = 0;

	status = false;
	roundSpawned = 0;
	roundRemoved = 0;

	name = "";
	team = 0;	//team
	//id = 0;
	unitDetails = NULL;
	numKills = 0;
	killPoints = 0;

	/*temporary per player turn variables*/
		//this assumes that reference list is re-parsed after every player's turn - otherwise the hasPerformedAction will have to be reinitialised explicitly before every player's turn
	for(int phase = 0; phase < GAME_PHASES_NUM_PER_TURN; phase++)
	{
		hasPerformedAction[phase] = false;
	}

	isUnitGroup = false;
	firstUnitInUnitGroup = NULL;

	#ifdef USE_ANN
	for(int nn = 0; nn < GAME_NUMBER_OF_EXPERIENCE_NN; nn++)
	{
		firstExperience[nn] = new ANNexperience();
		currentExperience[nn] = firstExperience[nn];
	}

	for(int nn = 0; nn < GAME_NUMBER_OF_EXPERIENCE_NN; nn++)
	{
		selfLearningTempVarBackPropagationPassError[nn] = MAX_ANN_BACK_PROPAGATION_ERROR;
	}
	selfLearningTempVarUnitHasBeenTried = false;
	selfLearningTempVarUnitPrefersLongDistanceCombatToMovement = false;
	#endif

	next = NULL;
}

UnitListClass::~UnitListClass()
{

	if(unitDetails != NULL);
	{
		delete unitDetails;
	}

	#ifdef USE_ANN
	for(int i = 0; i< GAME_NUMBER_OF_EXPERIENCE_NN; i++)
	{
		if(firstExperience[i] != NULL)
		{
			delete firstExperience[i];
		}
	}
	#endif

	if(firstUnitInUnitGroup != NULL)
	{
		delete firstUnitInUnitGroup;
	}

	if(next != NULL)
	{
		delete next;
	}
}



UnitListClass* LRRCunitClassClass::searchUnitListFindUnit(UnitListClass* firstUnitInUnitGroup, const string unitName, const int unitColour, bool* unitIDFound)
{
	UnitListClass* currentUnitInList = firstUnitInUnitGroup;
	UnitListClass* unitFound = NULL;

	while((currentUnitInList->next != NULL) && (*unitIDFound == false))
	{
		if((currentUnitInList->name == unitName) && (currentUnitInList->team == unitColour))
		{
			*unitIDFound = true;
			unitFound = currentUnitInList;
		}
		else
		{
			if(currentUnitInList->isUnitGroup)
			{
				UnitListClass* tempUnit;
				tempUnit = searchUnitListFindUnit(currentUnitInList->firstUnitInUnitGroup, unitName, unitColour, unitIDFound);
				if(*unitIDFound)
				{
					unitFound = tempUnit;
				}
			}
		}

		currentUnitInList = currentUnitInList->next;
	}

	return unitFound;

}

void LRRCunitClassClass::addUnitToList(UnitListClass* firstUnitInUnitList, string unitName, int unitColour, ModelDetails* unitDetails, int currentRound)
{
	UnitListClass* currentUnitInList = firstUnitInUnitList;

	while(currentUnitInList->next != NULL)
	{
		if(currentUnitInList->next == NULL)
		{
			currentUnitInList->name = unitName;
			currentUnitInList->team = unitColour;	//not currently used, as there is 1 UnitListClass created for every team
			currentUnitInList->unitDetails = unitDetails;


			currentUnitInList->status = true;
			currentUnitInList->roundSpawned = currentRound;

			UnitListClass* newUnitList = new UnitListClass();
			currentUnitInList->next = newUnitList;
		}

		currentUnitInList = currentUnitInList->next;
	}
}

#ifdef USE_ANN
UnitListClass* LRRCunitClassClass::searchUnitListFindOpponentWithLowestError(const int currentPlayerTurn, const int nn, UnitListClass* firstUnitInUnitGroup, double* currentLowestError, bool* foundOpponent, const int NNcurrentPhase)
{
	UnitListClass* currentUnitInList = firstUnitInUnitGroup;
	UnitListClass* unitFound = NULL;

	while(currentUnitInList->next != NULL)
	{
		if(currentUnitInList->selfLearningTempVarUnitHasBeenTried == false)
		{
			if(currentUnitInList->status == true)
			{
				if(currentUnitInList->team != currentPlayerTurn)
				{
					bool passedErrorRequirements = false;
					if(nn == GAME_INDEX_OF_ALL_EXPERIENCES_NN)
					{//use average error across all neural net types

						int totalError = 0;
						for(int i=0; i < GAME_NUMBER_OF_EXPERIENCE_NN_UTILISED; i++)	//or should this only be properties and combat experiences (not global experiences?)
						{

						#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
							int nnForParticularNNPhase;
							if(NNcurrentPhase == GAME_PHASE_MOVEMENT)
							{
								nnForParticularNNPhase = (i*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(GAME_PHASE_CLOSECOMBAT-1);
								double ccError =  currentUnitInList->selfLearningTempVarBackPropagationPassError[nnForParticularNNPhase];
								nnForParticularNNPhase = (i*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(GAME_PHASE_LONGDISTANCECOMBAT-1);
								double ldError =  currentUnitInList->selfLearningTempVarBackPropagationPassError[nnForParticularNNPhase];
								double minError;
								if(ccError < ldError)
								{
									minError = ccError;
								}
								else
								{
									minError = ldError;
								}
								totalError = totalError + minError;
							}
							else
							{
								nnForParticularNNPhase = (i*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(NNcurrentPhase-1);
								totalError = totalError + currentUnitInList->selfLearningTempVarBackPropagationPassError[nnForParticularNNPhase];
							}
						#else
							int nnForParticularNNPhase = i;
							totalError = totalError + currentUnitInList->selfLearningTempVarBackPropagationPassError[nnForParticularNNPhase];
						#endif

						}

						if(totalError <* currentLowestError)
						{
							passedErrorRequirements = true;
							*currentLowestError = totalError;
						}
					}
					else
					{

					#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
						int nnForParticularNNPhase = (nn*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(NNcurrentPhase-1);
					#else
						int nnForParticularNNPhase = nn;
					#endif

						if((currentUnitInList->selfLearningTempVarBackPropagationPassError[nnForParticularNNPhase] <* currentLowestError))
						{
							passedErrorRequirements = true;
							*currentLowestError = currentUnitInList->selfLearningTempVarBackPropagationPassError[nnForParticularNNPhase];
						}
					}

					if(passedErrorRequirements)
					{
						if(!(currentUnitInList->selfLearningTempVarUnitPrefersLongDistanceCombatToMovement))
						{
							*foundOpponent = true;
							unitFound = currentUnitInList;
						}
						else
						{
							//cout << "selfLearningTempVarUnitPrefersLongDistanceCombatToMovement" << endl;
							*foundOpponent = false;
						}

					}
				}
			}
		}

		if(currentUnitInList->isUnitGroup)
		{
			UnitListClass* tempUnit;
			bool tempFoundUnit = false;
			tempUnit = searchUnitListFindOpponentWithLowestError(currentPlayerTurn, nn, currentUnitInList->firstUnitInUnitGroup, currentLowestError, &tempFoundUnit, NNcurrentPhase);
			if(tempFoundUnit)
			{
				*foundOpponent = true;
				unitFound = tempUnit;
			}
		}

		currentUnitInList = currentUnitInList->next;
	}

	return unitFound;
}
#endif

bool LRRCunitClassClass::splitUnitGroup(UnitListClass* firstUnitInUnitList, const string unitGroupName, const int unitGroupColour, int currentRound)
{
	bool result = true;

	UnitListClass* currentUnitInList = firstUnitInUnitList;
	UnitListClass* unitGroupInList;

	bool foundUnitInList = false;

	while(currentUnitInList->next != NULL)
	{
		if((currentUnitInList->name == unitGroupName) && (currentUnitInList->team == unitGroupColour))
		{
			if(foundUnitInList)
			{
				cout << "Error: splitUnitGroup{}; unit group found twice in unit list" << endl;
			}

			if(currentUnitInList->isUnitGroup == true)
			{
				foundUnitInList = true;

				currentUnitInList->roundRemoved = currentRound;
				currentUnitInList->status = false;
				unitGroupInList = currentUnitInList;
			}
			else
			{
				cout << "Error: splitUnitGroup{}; unit in list is not a group" << endl;
				result = false;
			}
		}

		if(currentUnitInList->isUnitGroup == true)
		{
			if(!splitUnitGroup(currentUnitInList->firstUnitInUnitGroup, unitGroupName, unitGroupColour, currentRound))
			{
				result = false;
			}
		}

		if((foundUnitInList) && (currentUnitInList->next == NULL))
		{
			currentUnitInList->next = unitGroupInList->firstUnitInUnitGroup;
		}

		currentUnitInList = currentUnitInList->next;
	}

	return result;
}

void LRRCunitClassClass::searchUnitListAssignHasNotPerformedAction(UnitListClass* firstUnitInUnitGroup)
{
	UnitListClass* currentUnitInList = firstUnitInUnitGroup;

	while(currentUnitInList->next != NULL)
	{

		for(int phase = 0; phase < GAME_PHASES_NUM_PER_TURN; phase++)
		{
			currentUnitInList->hasPerformedAction[phase] = false;
		}

		if(currentUnitInList->isUnitGroup)
		{
			searchUnitListAssignHasNotPerformedAction(currentUnitInList->firstUnitInUnitGroup);
		}

		currentUnitInList = currentUnitInList->next;
	}
}

int LRRCunitClassClass::determineUnitWorthInPoints(const ModelDetails* unitDetails)
{
	int totalPoints;

	int movementPoints;
	int longDistancePoints;
	int closeCombatPoints;

	movementPoints = unitDetails->movementSpeed/GAME_UNIT_POINTS_MOVEMENT_RANGE_MODIFIER;
	longDistancePoints = unitDetails->longDistanceAttackTotal + unitDetails->longDistanceAttackBaseRange/GAME_UNIT_POINTS_LONG_DISTANCE_RANGE_MODIFIER;
	closeCombatPoints = unitDetails->closeCombatAttackTotal;

	totalPoints = (movementPoints + SHAREDvars.maxInt(longDistancePoints, closeCombatPoints));

	return totalPoints;
}


