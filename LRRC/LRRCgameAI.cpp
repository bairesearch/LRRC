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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3j2a 17-January-2017
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 * NB phase specific sprites are yet to be added to scene files based upon movement/combat outcomes
 * NB phase specific sprites are yet to be added to scene files based upon future movement/combat options
 *******************************************************************************/



#include "LRRCgameAI.hpp"

#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
#endif

#ifdef USE_ANN




	//ANNneuronContainer* firstInputNeuronInNetwork = new ANNneuronContainer();
void LRRCgameAIClass::initialiseNeuralNetwork(const int NNBeingTested, Player* currentPlayer, int currentPhase)
{
	ANNneuron* firstInputNeuronInNetwork = new ANNneuron();
	ANNneuron* firstOutputNeuronInNetwork;

	long numberOfInputNeurons;
	long numberOfOutputNeurons;
	long numberOfLayers;

	LDreference* tempUnitReference = new LDreference(true);
	LDreference* tempUnitRefenceOpponent = new LDreference(true);
	long tempUnitDecision = IRRELEVANT;
	ANNexperience* tempExperience = new ANNexperience;
	if(NNBeingTested == GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN)
	{
		numberOfOutputNeurons = PROPERTIES_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS;
		this->generateExperienceFromUnitPropertiesDecision(tempUnitReference, tempUnitRefenceOpponent, tempUnitDecision, tempExperience, currentPhase);
	}
	else if(NNBeingTested == GAME_INDEX_OF_COMBAT_EXPERIENCE_NN)
	{
		numberOfOutputNeurons = COMBAT_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS;
		this->generateExperienceFromUnitCombatDecision(tempUnitReference, tempUnitRefenceOpponent, tempUnitDecision, tempExperience, currentPhase);
	}
#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
	else if(NNBeingTested == GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN)
	{
		UnitListClass* tempUnit = new UnitListClass();
		LDreference* tempReferenceInNULLSceneFile;
		numberOfOutputNeurons = GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS;
		this->generateExperienceFromGlobalDecision(tempUnit, tempReferenceInNULLSceneFile, tempUnitReference, tempUnitRefenceOpponent, tempExperience);
	}
#endif
#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
	else if(NNBeingTested == GAME_INDEX_OF_OBJECT_EXPERIENCE_NN)
	{
		numberOfOutputNeurons = (long)this->addOrCompareAllObjectExperiences(NULL, NULL, NULL, true);
		/*
		ANNexperience* tempExperience2 = new ANNexperience;
		LDreference* tempUnitReference2 = new LDreference(true);
		long tempUnitDecision2 = IRRELEVANT;
		this->generateExperienceFromUnitPropertiesDecision(tempUnitReference2, tempUnitRefenceOpponent, tempUnitDecision2, tempExperience2, currentPhase);
		numberOfOutputNeurons = ANNexperienceClass.countNumberOfExperienceInputs(tempExperience2) - 2;	//-2 for distanceBetweenUnits and currentPhase properties experience inputs
		delete tempExperience2;
		delete tempUnitReference2;
		*/



		XMLrulesClass* currentReferenceRulesClass = LRRCrulesUnitTypeDetails;
		tempUnitReference->name = currentReferenceRulesClass->next->stringValue;	//generate a random reference name [NB not reference 1, as this is not an exclusive name]
		cout << "tempUnitReference->name = " << tempUnitReference->name << "\n\n" << endl;
		this->generateExperienceFromObjectDecision(tempUnitReference, tempUnitDecision, tempExperience, true);
	}
#endif
	else
	{
		cout << "error: initialiseNeuralNetwork{}: illegal NNBeingTested" << endl;
		exit(0);
	}
	numberOfInputNeurons = ANNexperienceClass.countNumberOfExperienceInputs(tempExperience);


	//cout << "\n\n*** numberOfInputNeurons = " << numberOfInputNeurons << endl;

	delete tempUnitReference;
	delete tempUnitRefenceOpponent;
	delete tempExperience;
		//arbitrary NN number of layers heuristic - need to optimise this


	if((NNBeingTested == GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN) || (NNBeingTested == GAME_INDEX_OF_COMBAT_EXPERIENCE_NN))
	{
		numberOfLayers = GAME_NUMBER_OF_NN_LAYERS;
		firstOutputNeuronInNetwork = ANNformation.formNeuralNetWithOptimisedProperties(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers);
	}
#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
	else if(NNBeingTested ==  GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN)
	{
		numberOfLayers = GAME_NUMBER_OF_NN_LAYERS_GLOBAL_EXPERIENCE;
		int layerDivergenceType = LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS;
		double meanLayerDivergenceFactor = 1.5;
		double probabilityANNneuronConnectionWithPreviousLayerNeuron = 1.0;
		double probabilityANNneuronConnectionWithAllPreviousLayersNeurons = 0.5;
		firstOutputNeuronInNetwork = ANNformation.formNeuralNet(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
	}
#endif
#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
	else if(NNBeingTested ==  GAME_INDEX_OF_OBJECT_EXPERIENCE_NN)
	{
		numberOfLayers = GAME_NUMBER_OF_NN_LAYERS_OBJECT_EXPERIENCE;
		int layerDivergenceType = LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING_SQUARE2D; //DEFAULT = LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING_SQUARE2D;
		double meanLayerDivergenceFactor = 1.0;
		double probabilityANNneuronConnectionWithPreviousLayerNeuron = 1.0;
		double probabilityANNneuronConnectionWithAllPreviousLayersNeurons = 0.0;		//default 0.0
		firstOutputNeuronInNetwork = ANNformation.formNeuralNet(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
	}
#endif
	else
	{

	}

	cout << "Neural Network Properties:" << endl;
	cout << "numberOfInputNeurons = " << numberOfInputNeurons << endl;
	cout << "numberOfOutputNeurons = " << numberOfOutputNeurons << endl;
	cout << "numberOfLayers = " << numberOfLayers << endl;

#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
	int nn = (NNBeingTested*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(currentPhase-1);
#else
	int nn = NNBeingTested;
#endif
	currentPlayer->firstInputNeuronInNetwork[nn] = firstInputNeuronInNetwork;
	currentPlayer->firstOutputNeuronInNetwork[nn] = firstOutputNeuronInNetwork;
	currentPlayer->numberOfInputNeurons[nn] = numberOfInputNeurons;
	currentPlayer->numberOfOutputNeurons[nn] = numberOfOutputNeurons;

}




long LRRCgameAIClass::mergePlayerUnitExperiencesIntoPlayerExperienceList(Player* currentPlayer, UnitListClass* firstUnitInUnitGroup, const int unitGroupTeam, int nnIndex)
{
	UnitListClass* currentUnitInList = firstUnitInUnitGroup;

	while(currentUnitInList->next != NULL)
	{
		#ifndef DEBUG_DO_NOT_PERFORM_KILL_RATIO_CHECKS_BEFORE_ADDING_UNIT_EXPERIENCES_TO_NN
		if(this->checkAverageKillRatioForUnitGroup(currentUnitInList) || (currentUnitInList->unitDetails->numPerson == 1))
		{
		#endif
			if(currentUnitInList->team == unitGroupTeam)
			{
				ANNexperience* currentExperienceInCurrentUnit = currentUnitInList->firstExperience[nnIndex];
				while(currentExperienceInCurrentUnit->next != NULL)
				{
					//cout << "copying experiences" << endl;
					ANNexperienceClass.copyExperiences(currentPlayer->currentExperience[nnIndex], currentExperienceInCurrentUnit);
					ANNexperience* newExperience = new ANNexperience();
					currentPlayer->currentExperience[nnIndex]->next = newExperience;
					currentPlayer->currentExperience[nnIndex] = currentPlayer->currentExperience[nnIndex]->next;

					currentExperienceInCurrentUnit = currentExperienceInCurrentUnit->next;
				}

			}

			if(currentUnitInList->isUnitGroup)
			{
				this->mergePlayerUnitExperiencesIntoPlayerExperienceList(currentPlayer, currentUnitInList->firstUnitInUnitGroup, unitGroupTeam, nnIndex);
			}
		#ifndef DEBUG_DO_NOT_PERFORM_KILL_RATIO_CHECKS_BEFORE_ADDING_UNIT_EXPERIENCES_TO_NN
		}
		#endif

		currentUnitInList = currentUnitInList->next;
	}

	return 1;
}




long LRRCgameAIClass::mergeAllUnitExperiencesIntoPlayerExperienceList(Player* currentPlayer, UnitListClass* firstUnitInUnitGroup, int nnIndex)
{
	UnitListClass* currentUnitInList = firstUnitInUnitGroup;

	while(currentUnitInList->next != NULL)
	{
		//cout << "h1" << endl;

		#ifndef DEBUG_DO_NOT_PERFORM_KILL_RATIO_CHECKS_BEFORE_ADDING_UNIT_EXPERIENCES_TO_NN
		if(this->checkAverageKillRatioForUnitGroup(currentUnitInList) || (currentUnitInList->unitDetails->numPerson == 1))
		{
		#endif
			ANNexperience* currentExperienceInCurrentUnit = currentUnitInList->firstExperience[nnIndex];
			while(currentExperienceInCurrentUnit->next != NULL)
			{
				//cout << "copying experiences" << endl;
				ANNexperienceClass.copyExperiences(currentPlayer->currentExperience[nnIndex], currentExperienceInCurrentUnit);
				ANNexperience* newExperience = new ANNexperience();
				currentPlayer->currentExperience[nnIndex]->next = newExperience;
				currentPlayer->currentExperience[nnIndex] = currentPlayer->currentExperience[nnIndex]->next;

				currentExperienceInCurrentUnit = currentExperienceInCurrentUnit->next;
			}

			if(currentUnitInList->isUnitGroup)
			{
				this->mergeAllUnitExperiencesIntoPlayerExperienceList(currentPlayer, currentUnitInList->firstUnitInUnitGroup, nnIndex);
			}
		#ifndef DEBUG_DO_NOT_PERFORM_KILL_RATIO_CHECKS_BEFORE_ADDING_UNIT_EXPERIENCES_TO_NN
		}
		#endif

		currentUnitInList = currentUnitInList->next;
	}

	return 1;
}



void LRRCgameAIClass::parseSceneFileAndFillUnitLists(string sceneFileName, UnitListClass* firstUnitInUnitList, int currentRound)
{
	bool result = true;

	LDreference* initialReferenceInSceneFile = new LDreference();
	LDreference* topLevelReferenceInSceneFile = new LDreference(sceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
	if(!LDparser.parseFile(sceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, false))
	{//file does not exist
		cout << "The file: " << sceneFileName << " does not exist in the directory" << endl;
		result = false;
	}

	this->fillUnitList(initialReferenceInSceneFile, firstUnitInUnitList, currentRound);

	delete initialReferenceInSceneFile;
	delete topLevelReferenceInSceneFile;
}





void LRRCgameAIClass::fillUnitList(LDreference* currentReferenceInSceneFile, UnitListClass* firstUnitInUnitGroup, int currentRound)
{
	LDreference* currentReference = currentReferenceInSceneFile;
	UnitListClass* currentUnitInUnitList = firstUnitInUnitGroup;
	while(currentReference->next != NULL)
	{
		if(currentReference->isSubModelReference)
		{
			//cout << "currentReference->name = " << currentReference->name << endl;

			LDreference* topLevelReferenceInUnit = new LDreference(currentReference->name, currentReference->colour, true);
			LRRCmodelClass.copyAllUnitDetails(topLevelReferenceInUnit->subModelDetails, currentReference->subModelDetails);
			LRRCcombat.searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(topLevelReferenceInUnit, currentReference->firstReferenceWithinSubModel, topLevelReferenceInUnit, true);
			/*
			//debug:
			RecordClass* currentReferenceRecordClass = topLevelReferenceInUnit->subModelDetails->recordOfUnitTypeDetails;
			while(currentReferenceRecordClass->next != NULL)
			{
				if(currentReferenceRecordClass->name == PERSON_HEAD_NAME)
				{
					cout << "topLevelReferenceInUnit->name = " << topLevelReferenceInUnit->name << endl;
					cout << "\t PERSON_HEAD_NAME topLevelReferenceInUnit->numberOfThisPartIDInTheUnit = " << currentReferenceRecordClass->numberOfThisPartIDInTheUnit << endl;
				}

				currentReferenceRecordClass = currentReferenceRecordClass->next;
			}
			*/
			LRRCmodelClass.determineUnitTypeAndMinSpeedOfUnitGroup(topLevelReferenceInUnit->subModelDetails);



			if(topLevelReferenceInUnit->subModelDetails->numPerson >= 1)
			{//add a unit to the unit list if it contains a single person

				//add a unit/group to the unit list

				currentUnitInUnitList->name = currentReference->name;
				currentUnitInUnitList->team = currentReference->colour;	//not currently used, as there is 1 UnitListClass created for every team
				currentUnitInUnitList->unitDetails = topLevelReferenceInUnit->subModelDetails;		//OR CHECK THIS; currentReference->subModelDetails or topLevelReferenceInUnit->subModelDetails??? it depends upon what information is to be recorded here
				currentUnitInUnitList->status = true;
				currentUnitInUnitList->roundSpawned = currentRound;

				UnitListClass* newUnitList = new UnitListClass();
				currentUnitInUnitList->next = newUnitList;

				if(topLevelReferenceInUnit->subModelDetails->numPerson == 1)
				{
					//do not go down further into reference tree if already reached a unit containing only a single person
				}
				else
				{
					//go down further into reference tree
					UnitListClass* newUnitList1 = new UnitListClass();
					currentUnitInUnitList->firstUnitInUnitGroup = newUnitList1;
					currentUnitInUnitList->isUnitGroup = true;
					this->fillUnitList(currentReference->firstReferenceWithinSubModel, currentUnitInUnitList->firstUnitInUnitGroup, currentRound);
				}

				currentUnitInUnitList=currentUnitInUnitList->next;
			}

			delete topLevelReferenceInUnit;
		}

		currentReference = currentReference->next;
	}
}





bool LRRCgameAIClass::determineIfUnitGroupHasAliveUnits(const UnitListClass* firstUnitInUnitGroup)
{
	bool foundAliveUnit = false;
	const UnitListClass* currentUnitInList = firstUnitInUnitGroup;

	while(currentUnitInList->next != NULL)
	{
		if(currentUnitInList->status == true)
		{
			foundAliveUnit = true;

		}

		if(currentUnitInList->isUnitGroup)
		{
			if(this->determineIfUnitGroupHasAliveUnits(currentUnitInList->firstUnitInUnitGroup))
			{
				foundAliveUnit = true;
			}
		}

		currentUnitInList = currentUnitInList->next;
	}
	return foundAliveUnit;
}

bool LRRCgameAIClass::checkAverageKillRatioForUnitGroup(UnitListClass* firstUnitInUnitGroup)
{
	bool result;

	int unitGroupTotalKillPoints = 0;
	int unitGroupTotalDeathPoints = 0;
	this->determineAverageKillRatioForUnitGroup(firstUnitInUnitGroup, &unitGroupTotalKillPoints, &unitGroupTotalDeathPoints);
	if(double(unitGroupTotalKillPoints)/double(unitGroupTotalDeathPoints) >= GAME_AI_NN_EXPERIENCE_RECORD_MIN_KILL_RATIO_REQUIREMENT)
	{
		result = true;
	}
	else
	{
		result = false;
	}

	return result;
}

void LRRCgameAIClass::determineAverageKillRatioForUnitGroup(UnitListClass* firstUnitInUnitGroup, int* killPoints, int* deathPoints)
{
	UnitListClass* currentUnitInList = firstUnitInUnitGroup;

	while(currentUnitInList->next != NULL)
	{
		if(currentUnitInList->isUnitGroup)
		{
			this->determineAverageKillRatioForUnitGroup(currentUnitInList->firstUnitInUnitGroup, killPoints, deathPoints);
		}
		else
		{
			*killPoints = *killPoints + currentUnitInList->killPoints;
			if(currentUnitInList->status == false)
			{
				*deathPoints = *deathPoints + LRRCunitClass.determineUnitWorthInPoints(currentUnitInList->unitDetails);
			}
		}

		currentUnitInList = currentUnitInList->next;
	}
}




void LRRCgameAIClass::parseSceneFileAndUpdateUnitList(string sceneFileName, UnitListClass* firstUnitInUnitList, int currentRound)
{
	bool result = true;

	LDreference* initialReferenceInSceneFile = new LDreference();
	LDreference* topLevelReferenceInSceneFile = new LDreference(sceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
	if(!LDparser.parseFile(sceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, false))
	{//file does not exist
		cout << "The file: " << sceneFileName << " does not exist in the directory" << endl;
		result = false;
	}

	this->updateUnitList(initialReferenceInSceneFile, firstUnitInUnitList, currentRound);

	delete initialReferenceInSceneFile;
	delete firstUnitInUnitList;
}

void LRRCgameAIClass::updateUnitList(LDreference* initialReferenceInSceneFile, UnitListClass* firstUnitInUnitList, int currentRound)	//required after every round
{
	this->updateUnitListWithNewUnits(initialReferenceInSceneFile, firstUnitInUnitList, currentRound);

	this->updateUnitListWithDeadUnits(initialReferenceInSceneFile, firstUnitInUnitList, currentRound);

}

void LRRCgameAIClass::updateUnitListWithNewUnits(LDreference* currentReferenceInSceneFile, UnitListClass* firstUnitInUnitGroup, int currentRound)
{
	LDreference* currentReference = currentReferenceInSceneFile;
	while(currentReference->next != NULL)
	{
		if(currentReference->isSubModelReference)
		{
			//add a unit to the unit list if it contains a single person

			bool foundUnitInList = false;
			UnitListClass* foundUnit;
			foundUnit = this->searchUnitListForUnitAndIfNotThereAddToList(firstUnitInUnitGroup, currentReference, currentRound, true, &foundUnitInList);

			//go down further into reference tree
			if(foundUnitInList)
			{
				if(foundUnit->isUnitGroup)
				{//do nothing

				}
				else
				{
					foundUnit->isUnitGroup = true;
					UnitListClass* newUnit = new UnitListClass();
					foundUnit->firstUnitInUnitGroup = newUnit;
				}
			}
			else
			{
				foundUnit->isUnitGroup = true;
				UnitListClass* newUnit = new UnitListClass();
				foundUnit->firstUnitInUnitGroup = newUnit;

			}
			this->updateUnitListWithNewUnits(currentReference->firstReferenceWithinSubModel, foundUnit->firstUnitInUnitGroup, currentRound);
		}

		currentReference = currentReference->next;
	}
}

void LRRCgameAIClass::updateUnitListWithDeadUnits(LDreference* initialReferenceInSceneFile, UnitListClass* firstUnitInUnitGroup, int currentRound)
{
	UnitListClass* currentUnitInList = firstUnitInUnitGroup;

	while(currentUnitInList->next != NULL)
	{
		bool unitIDFound = false;
		bool result = true;

		LDreference* foundReference = LRRCgameReferenceManipulation.searchReferenceListFindReference(initialReferenceInSceneFile, currentUnitInList->name, currentUnitInList->team, &unitIDFound, &result);
		if(!unitIDFound)
		{
			currentUnitInList->status = false;
			currentUnitInList->roundRemoved = currentRound;
		}

		if(currentUnitInList->isUnitGroup)
		{
			this->updateUnitListWithDeadUnits(initialReferenceInSceneFile, currentUnitInList->firstUnitInUnitGroup, currentRound);
		}

		currentUnitInList = currentUnitInList->next;
	}
}

UnitListClass* LRRCgameAIClass::searchUnitListForUnitAndIfNotThereAddToList(UnitListClass* firstUnitInUnitGroup, LDreference* unitReferenceInSceneFile, int currentRound, const bool topLevelInTree, bool* foundUnitInList)
{
	UnitListClass* currentUnitInList = firstUnitInUnitGroup;
	UnitListClass* foundUnit;

	while((currentUnitInList->next != NULL) && (!(*foundUnitInList)))
	{
		if((currentUnitInList->name == unitReferenceInSceneFile->name) && (currentUnitInList->team == unitReferenceInSceneFile->colour))
		{
			*foundUnitInList = true;
			foundUnit = currentUnitInList;
		}
		else
		{
			if(currentUnitInList->isUnitGroup)
			{
				UnitListClass* tempUnit;
				tempUnit = this->searchUnitListForUnitAndIfNotThereAddToList(currentUnitInList->firstUnitInUnitGroup, unitReferenceInSceneFile, currentRound, false, foundUnitInList);
				if(*foundUnitInList)
				{
					foundUnit = tempUnit;
				}
			}
		}


		if((currentUnitInList->next == NULL) && (!(*foundUnitInList)) && (topLevelInTree))
		{
			foundUnit = currentUnitInList;

			currentUnitInList->name = unitReferenceInSceneFile->name;
			currentUnitInList->team = unitReferenceInSceneFile->colour;	//not currently used, as there is 1 UnitListClass created for every team
			currentUnitInList->unitDetails = unitReferenceInSceneFile->subModelDetails;
			currentUnitInList->status = true;
			currentUnitInList->roundSpawned = currentRound;

			UnitListClass* newUnitList = new UnitListClass();
			currentUnitInList->next = newUnitList;


		}

		currentUnitInList = currentUnitInList->next;

	}

	return foundUnit;
}





/*
//not used
void addExperienceToPlayer(ANNexperience* experience, Player* player)
{
	player->currentExperience = experience;

	ANNexperience* newExperience = new ANNexperience();
	player->currentExperience->next = newExperience;
	player->currentExperience = player->currentExperience->next;
}

//not used
void addExperienceToUnit(ANNexperience* experience, ModelDetails* unit)
{
	unit->currentExperience = experience;

	ANNexperience* newExperience = new ANNexperience();
	unit->currentExperience->next = newExperience;
	unit->currentExperience = unit->currentExperience->next;
}

//not used
void addExperienceInputToExperience(ANNexperienceInput* experienceInput, ANNexperience* experience)
{
	experience->currentExperienceInput = experienceInput;

	ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput;
	currentExperienceInput = currentExperienceInput->next;
}
*/




//addExperienceFromUnitCombatDecision(unit, unitOpponent, unitCombatType, combatExperienceResult);





double LRRCgameAIClass::addExperiencesFromUnitDecision(UnitListClass* unit, LDreference* unitReference, LDreference* unitReferenceOpponent, long unitDecision, LDreference* initialReferenceInThisPhaseStartScene, int NNcurrentPhase, Player* currentPlayer)
{
	//bool result = true;

	double totalError = 0.0;

	totalError = totalError + this->addOrCompareExperienceFromUnitDecision(NNcurrentPhase, unit, unitReference, unitReferenceOpponent, unitDecision, currentPlayer, GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, ADD_EXPERIENCE, initialReferenceInThisPhaseStartScene);		//arbitrary player, OLD=attackerPlayer

	totalError = totalError + this->addOrCompareExperienceFromUnitDecision(NNcurrentPhase, unit, unitReference, unitReferenceOpponent, unitDecision, currentPlayer, GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, ADD_EXPERIENCE, initialReferenceInThisPhaseStartScene);		//arbitrary player, OLD=attackerPlayer

	#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
	totalError = totalError + this->addOrCompareExperienceFromUnitDecision(NNcurrentPhase, unit, unitReference, unitReferenceOpponent, unitDecision, currentPlayer, GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN, ADD_EXPERIENCE, initialReferenceInThisPhaseStartScene);		//arbitrary player, OLD=attackerPlayer
	#endif

	//return result;
	return totalError;
}

double LRRCgameAIClass::addOrCompareExperienceFromUnitDecision(int currentPhase, UnitListClass* unit, LDreference* unitReference, LDreference* unitReferenceOpponent, long unitDecision, Player* currentPlayer, const int NNBeingTested, const bool addOrCompareExperience, LDreference* initialReferenceInThisPhaseStartScene)
{
	double experienceBackPropagationPassError = 0.0;
	//bool result = true;


	#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
	int nn = (NNBeingTested*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(currentPhase-1);
	#else
	int nn = NNBeingTested;
	#endif

	/*
	cout << "A3"<< endl;
	cout << "nn = " << nn << endl;
	cout << "unit->currentExperience[nn]->classTargetValue = " << unit->currentExperience[nn]->classTargetValue << endl;
	cout << "unit->currentExperience[nn]->firstExperienceInput->inputValue = " << unit->currentExperience[nn]->firstExperienceInput->inputValue << endl;
	*/

	if(addOrCompareExperience == ADD_EXPERIENCE)
	{
	#ifndef TH_LRRC_LOW_RAM_AVAILABLE
		if(NNBeingTested == GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN)
		{
			this->generateExperienceFromUnitPropertiesDecision(unitReference, unitReferenceOpponent, unitDecision, (unit->currentExperience[nn]), currentPhase);
			ANNexperience* newExperience = new ANNexperience();
			unit->currentExperience[nn]->next = newExperience;
			unit->currentExperience[nn] = unit->currentExperience[nn]->next;
		}
		else if(NNBeingTested == GAME_INDEX_OF_COMBAT_EXPERIENCE_NN)
		{
			this->generateExperienceFromUnitCombatDecision(unitReference, unitReferenceOpponent, unitDecision, (unit->currentExperience[nn]), currentPhase);
			ANNexperience* newExperience = new ANNexperience();
			unit->currentExperience[nn]->next = newExperience;
			unit->currentExperience[nn] = unit->currentExperience[nn]->next;
		}
	#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
		else if(NNBeingTested == GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN)
		{
			this->generateExperienceFromGlobalDecision(currentPlayer->firstUnitInUnitList, initialReferenceInThisPhaseStartScene, unitReference, unitReferenceOpponent, (unit->currentExperience[nn]));
			ANNexperience* newExperience = new ANNexperience();
			unit->currentExperience[nn]->next = newExperience;
			unit->currentExperience[nn] = unit->currentExperience[nn]->next;
		}
	#endif
	#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
		else if(NNBeingTested == GAME_INDEX_OF_OBJECT_EXPERIENCE_NN)
		{
			this->generateExperienceFromObjectDecision(unitReference, unitDecision, (currentPlayer->currentExperience[nn]), true);
			ANNexperience* newExperience = new ANNexperience();
			currentPlayer->currentExperience[nn]->next = newExperience;
			currentPlayer->currentExperience[nn] = currentPlayer->currentExperience[nn]->next;
		}
	#endif
		else
		{
			cout << "error: addOrCompareExperienceFromUnitDecision{}: illegal NNBeingTested 1" << endl;
			exit(0);
		}

	#else
					ANNexperience* newExperience = new ANNexperience();
					if(NNBeingTested == GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN)
					{
						this->generateExperienceFromUnitPropertiesDecision(unitReference, unitReferenceOpponent, unitDecision, newExperience, currentPhase);
					}
					else if(NNBeingTested == GAME_INDEX_OF_COMBAT_EXPERIENCE_NN)
					{
						this->generateExperienceFromUnitCombatDecision(unitReference, unitReferenceOpponent, unitDecision, newExperience, currentPhase);
					}
				#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
					else if(NNBeingTested == GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN)
					{
						this->generateExperienceFromGlobalDecision(currentPlayer->firstUnitInUnitList, initialReferenceInThisPhaseStartScene, unitReference, unitReferenceOpponent, newExperience);
					}
				#endif
				#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
					else if(NNBeingTested == GAME_INDEX_OF_OBJECT_EXPERIENCE_NN)
					{
						this->generateExperienceFromObjectDecision(unitReference, unitDecision, newExperience, true);
					}
				#endif
					else
					{
						cout << "error: addOrCompareExperienceFromUnitDecision{}: illegal NNBeingTested 3" << endl;
						exit(0);
					}
					for(int e=0; e<TH_LRRC_LOW_RAM_NUM_CONSEC_TRAINING_EPOCHS_PER_EXPERIENCE; e++)
					{
						ANNalgorithmBackpropagationTraining.resetInputsAndClassTargets((currentPlayer->firstInputNeuronInNetwork[nn]), (currentPlayer->firstOutputNeuronInNetwork[nn]), (currentPlayer->numberOfInputNeurons[nn]), (currentPlayer->numberOfOutputNeurons[nn]), newExperience);
						double trainingErrorNotUsed = ANNBackPropogationPass(currentPlayer->firstInputNeuronInNetwork[nn], currentPlayer->firstOutputNeuronInNetwork[nn]);
					}
					delete newExperience;
	#endif
	}
	else if(addOrCompareExperience == COMPARE_EXPERIENCE)
	{
		ANNexperience* experienceWithoutKnownOutput = new ANNexperience;
		long unitHypotheticalDecisionToTest;
		unitHypotheticalDecisionToTest = unitDecision;
		//generate proposed experience
		if(NNBeingTested == GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN)
		{
			this->generateExperienceFromUnitPropertiesDecision(unitReference, unitReferenceOpponent, unitHypotheticalDecisionToTest, experienceWithoutKnownOutput, currentPhase);
		}
		else if(NNBeingTested == GAME_INDEX_OF_COMBAT_EXPERIENCE_NN)
		{
			this->generateExperienceFromUnitCombatDecision(unitReference, unitReferenceOpponent, unitHypotheticalDecisionToTest, experienceWithoutKnownOutput, currentPhase);
		}
	#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
		else if(NNBeingTested == GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN)
		{
			this->generateExperienceFromGlobalDecision(currentPlayer->firstUnitInUnitList, initialReferenceInThisPhaseStartScene, unitReference, unitReferenceOpponent, experienceWithoutKnownOutput);
		}
	#endif
	#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
		else if(NNBeingTested == GAME_INDEX_OF_OBJECT_EXPERIENCE_NN)
		{
		#ifdef ANN_OBJECT_RECOGNITION_EXPERIENCES_RANDOMISE_LIGHTING_CONDITIONS
			this->generateExperienceFromObjectDecision(unitReference, unitDecision, experienceWithoutKnownOutput, true);
		#else
			this->generateExperienceFromObjectDecision(unitReference, unitDecision, experienceWithoutKnownOutput, false);
		#endif
		}
	#endif
		else
		{
			cout << "error: addOrCompareExperienceFromUnitDecision{}: illegal NNBeingTested 4" << endl;
			exit(0);
		}
		experienceBackPropagationPassError = ANNalgorithmBackpropagationTraining.calculateExperienceErrorForHypotheticalDecisionBackpropagation(currentPlayer->firstInputNeuronInNetwork[nn], currentPlayer->firstOutputNeuronInNetwork[nn], currentPlayer->numberOfInputNeurons[nn], currentPlayer->numberOfOutputNeurons[nn], (experienceWithoutKnownOutput));
		delete experienceWithoutKnownOutput;
	}

	return experienceBackPropagationPassError;
}



void LRRCgameAIClass::generateExperienceFromUnitPropertiesDecision(LDreference* unitReference, LDreference* unitReferenceOpponent, long unitDecision, ANNexperience* currentExperience, int currentPhase)
{

	/*
	now store every property of the parties as experience inputs:

	define experience inputs for a unit properties decision experience:

	unit1 recordOfUnitTypeDetails
	unit1 recordOfUnitCombatDetailsDefenceHead
	unit1 recordOfUnitCombatDetailsDefenceTorso
	unit1 recordOfUnitCombatDetailsDefenceShield
	unit1 recordOfUnitCombatDetailsAttackCloseCombat
	unit1 recordOfUnitCombatDetailsAttackLongDistance
	unit2 recordOfUnitTypeDetails
	unit2 recordOfUnitCombatDetailsDefenceHead
	unit2 recordOfUnitCombatDetailsDefenceTorso
	unit2 recordOfUnitCombatDetailsDefenceShield
	unit2 recordOfUnitCombatDetailsAttackCloseCombat
	unit2 recordOfUnitCombatDetailsAttackLongDistance
	distance between two units

		only assume the AI Agent knows the properties of the unit.
		do not assume the AI Agent knows the rules of the game; ie do not save high level information Eg attackClass, defenceClass, etc
	*/

	//cout << "3d1" << endl;

	currentExperience->classTargetValue = unitDecision;
	ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;

	ModelDetails* unitDetails = unitReference->subModelDetails;
	ModelDetails* unitDetailsOpponent = unitReferenceOpponent->subModelDetails;

	RecordClass* currentRecord;

	//cout << "3d2" << endl;


	currentRecord = unitDetails->recordOfUnitTypeDetails;

	//cout << "unitReference->name = " << unitReference->name << endl;
	//cout << "unitReferenceOpponent->name = " << unitReferenceOpponent->name << endl;

	while(currentRecord->next != NULL)
	{
		//cout << "3d2a1" << endl;
		//cout << " currentExperienceInput->inputValue = " << currentExperienceInput->inputValue << endl;		<< ISSUE FOUND HERE!
		//cout << "currentRecord->numberOfThisPartIDInTheUnit = " << (double)(currentRecord->numberOfThisPartIDInTheUnit) << endl;
		currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)(currentRecord->numberOfThisPartIDInTheUnit), COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);

		//cout << "3d2a2" << endl;

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();

		//cout << "3d2a2b" << endl;

		currentExperienceInput->next = newExperienceInput;

		//cout << "3d2a2c" << endl;
		currentExperienceInput = currentExperienceInput->next;

		//cout << "3d2a3" << endl;

		currentRecord = currentRecord->next;
	}

	//cout << "3d2a" << endl;

	currentRecord = unitDetails->recordOfUnitCombatDetailsDefenceHead;
	while(currentRecord->next != NULL)
	{
		currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)currentRecord->numberOfThisPartIDInTheUnit, COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
		currentExperienceInput->next = newExperienceInput;
		currentExperienceInput = currentExperienceInput->next;

		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetails->recordOfUnitCombatDetailsDefenceTorso;
	while(currentRecord->next != NULL)
	{
		currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)currentRecord->numberOfThisPartIDInTheUnit, COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
		currentExperienceInput->next = newExperienceInput;
		currentExperienceInput = currentExperienceInput->next;

		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetails->recordOfUnitCombatDetailsDefenceShield;
	while(currentRecord->next != NULL)
	{
		currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)currentRecord->numberOfThisPartIDInTheUnit, COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
		currentExperienceInput->next = newExperienceInput;
		currentExperienceInput = currentExperienceInput->next;

		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetails->recordOfUnitCombatDetailsAttackCloseCombat;
	while(currentRecord->next != NULL)
	{
		currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)currentRecord->numberOfThisPartIDInTheUnit, COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
		currentExperienceInput->next = newExperienceInput;
		currentExperienceInput = currentExperienceInput->next;

		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetails->recordOfUnitCombatDetailsAttackLongDistance;
	while(currentRecord->next != NULL)
	{
		currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)currentRecord->numberOfThisPartIDInTheUnit, COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
		currentExperienceInput->next = newExperienceInput;
		currentExperienceInput = currentExperienceInput->next;

		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetailsOpponent->recordOfUnitTypeDetails;
	while(currentRecord->next != NULL)
	{
		currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)currentRecord->numberOfThisPartIDInTheUnit, COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
		currentExperienceInput->next = newExperienceInput;
		currentExperienceInput = currentExperienceInput->next;

		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetailsOpponent->recordOfUnitCombatDetailsDefenceHead;
	while(currentRecord->next != NULL)
	{
		currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)currentRecord->numberOfThisPartIDInTheUnit, COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
		currentExperienceInput->next = newExperienceInput;
		currentExperienceInput = currentExperienceInput->next;

		currentRecord = currentRecord->next;
	}
	currentRecord = unitDetailsOpponent->recordOfUnitCombatDetailsDefenceTorso;
	while(currentRecord->next != NULL)
	{
		currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)currentRecord->numberOfThisPartIDInTheUnit, COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
		currentExperienceInput->next = newExperienceInput;
		currentExperienceInput = currentExperienceInput->next;

		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetailsOpponent->recordOfUnitCombatDetailsDefenceShield;
	while(currentRecord->next != NULL)
	{
		currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)currentRecord->numberOfThisPartIDInTheUnit, COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
		currentExperienceInput->next = newExperienceInput;
		currentExperienceInput = currentExperienceInput->next;

		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetailsOpponent->recordOfUnitCombatDetailsAttackCloseCombat;
	while(currentRecord->next != NULL)
	{
		currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)currentRecord->numberOfThisPartIDInTheUnit, COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
		currentExperienceInput->next = newExperienceInput;
		currentExperienceInput = currentExperienceInput->next;

		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetailsOpponent->recordOfUnitCombatDetailsAttackLongDistance;
	while(currentRecord->next != NULL)
	{
		currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)currentRecord->numberOfThisPartIDInTheUnit, COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);

		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
		currentExperienceInput->next = newExperienceInput;
		currentExperienceInput = currentExperienceInput->next;

		currentRecord = currentRecord->next;
	}

	//cout << "3d3" << endl;

	//cout << "3d4" << endl;

	/*
	currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput((double)combatExperienceResult, COMBAT_EXPERIENCE_INPUT_RESULT_MAX);
	ANNexperienceInput* newExperienceInput2 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput2;
	currentExperienceInput = currentExperienceInput->next;
	*/

		int maximumMovementDistanceForAnyUnit = 0;
		XMLrulesClass* currentReferenceRulesClass = LRRCrulesUnitTypeCatagories;
		while(currentReferenceRulesClass->next != NULL)
		{
			maximumMovementDistanceForAnyUnit = SHAREDvars.maxInt(maximumMovementDistanceForAnyUnit, currentReferenceRulesClass->attribute4);

			currentReferenceRulesClass = currentReferenceRulesClass->next;
		}
		//cout << "DEBUG: maximumMovementDistanceForAnyUnit = " << maximumMovementDistanceForAnyUnit << endl;

	//cout << "3d5" << endl;

	double distanceBetweenUnits = LRRCcombat.calculateTheDistanceBetweenTwoUnits(&(unitReference->absolutePosition), &(unitReferenceOpponent->absolutePosition));
	currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(distanceBetweenUnits, maximumMovementDistanceForAnyUnit);
		//cout << "currentExperienceInput->inputValue = " << currentExperienceInput->inputValue << endl;
	ANNexperienceInput* newExperienceInput3 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput3;
	currentExperienceInput = currentExperienceInput->next;
	//add 2D relationship map information here - can be added to a hypbrid 1D/2D neural network...


	currentExperienceInput->inputValue = currentPhase;
	ANNexperienceInput* newExperienceInput4 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput4;
	currentExperienceInput = currentExperienceInput->next;


	//cout << "3d6" << endl;
}

void LRRCgameAIClass::generateExperienceFromUnitCombatDecision(LDreference* unitReference, LDreference* unitReferenceOpponent, long unitDecision, ANNexperience* currentExperience, int currentPhase)
{

	/*
	now store every combat level of the parties as experience inputs:

	define experience inputs for a unit combat decision experience:

	unit1 movementSpeed
	unit1 closeCombatAttackTotal
	unit1 longDistanceAttackTotal
	unit1 defenceTotal
	unit2 movementSpeed
	unit2 closeCombatAttackTotal
	unit2 longDistanceAttackTotal
	unit2 defenceTotal
	distance between two units

		only assume the AI Agent knows the properties of the unit.
		do not assume the AI Agent knows the rules of the game; ie do not save high level information Eg attackClass, defenceClass, etc
	*/




	int maximumMovementDistanceForAnyUnit = 0;
	int maximumCloseCombatAttackTotalForAnyUnit = 0;
	int maximumLongDistanceAttackTotalForAnyUnit = 0;
	int maximumDefenceHeadTotalForAnyUnit = 0;
	int maximumDefenceTorsoTotalForAnyUnit = 0;
	int maximumDefenceShieldTotalForAnyUnit = 0;
	int maximumDefenceTotalForAnyUnit = 0;
	XMLrulesClass* currentReferenceRulesClass;

	currentReferenceRulesClass = LRRCrulesUnitTypeCatagories;
	while(currentReferenceRulesClass->next != NULL)
	{
		maximumMovementDistanceForAnyUnit = SHAREDvars.maxInt(maximumMovementDistanceForAnyUnit, currentReferenceRulesClass->attribute4);

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackCloseCombat;
	while(currentReferenceRulesClass->next != NULL)
	{
		maximumCloseCombatAttackTotalForAnyUnit = SHAREDvars.maxInt(maximumCloseCombatAttackTotalForAnyUnit, (currentReferenceRulesClass->attribute4 + currentReferenceRulesClass->attribute5));

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
	while(currentReferenceRulesClass->next != NULL)
	{
		maximumLongDistanceAttackTotalForAnyUnit = SHAREDvars.maxInt(maximumLongDistanceAttackTotalForAnyUnit, currentReferenceRulesClass->attribute4);

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceHead;
	while(currentReferenceRulesClass->next != NULL)
	{
		maximumDefenceHeadTotalForAnyUnit = SHAREDvars.maxInt(maximumDefenceHeadTotalForAnyUnit, currentReferenceRulesClass->attribute4);

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceTorso;
	while(currentReferenceRulesClass->next != NULL)
	{
		maximumDefenceTorsoTotalForAnyUnit = SHAREDvars.maxInt(maximumDefenceTorsoTotalForAnyUnit, currentReferenceRulesClass->attribute4);

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceShield;
	while(currentReferenceRulesClass->next != NULL)
	{
		maximumDefenceShieldTotalForAnyUnit = SHAREDvars.maxInt(maximumDefenceShieldTotalForAnyUnit, currentReferenceRulesClass->attribute4);

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
	maximumDefenceTotalForAnyUnit = maximumDefenceHeadTotalForAnyUnit + maximumDefenceTorsoTotalForAnyUnit + maximumDefenceShieldTotalForAnyUnit;



	//cout << "3d1" << endl;

	currentExperience->classTargetValue = unitDecision;
	ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;

	ModelDetails* unitDetails = unitReference->subModelDetails;
	ModelDetails* unitDetailsOpponent = unitReferenceOpponent->subModelDetails;

	//fill experience inputs
	currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(unitDetails->movementSpeed, maximumMovementDistanceForAnyUnit);
	ANNexperienceInput* newExperienceInput1 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput1;
	currentExperienceInput = currentExperienceInput->next;

	currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(unitDetails->closeCombatAttackTotal, maximumCloseCombatAttackTotalForAnyUnit*COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);
	ANNexperienceInput* newExperienceInput2 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput2;
	currentExperienceInput = currentExperienceInput->next;

	currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(unitDetails->longDistanceAttackTotal, maximumLongDistanceAttackTotalForAnyUnit*COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);
	ANNexperienceInput* newExperienceInput3 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput3;
	currentExperienceInput = currentExperienceInput->next;

	currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(unitDetails->defenceTotal, maximumDefenceTotalForAnyUnit*COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);
	ANNexperienceInput* newExperienceInput4 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput4;
	currentExperienceInput = currentExperienceInput->next;


	currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(unitDetailsOpponent->movementSpeed, maximumMovementDistanceForAnyUnit);
	ANNexperienceInput* newExperienceInput5 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput5;
	currentExperienceInput = currentExperienceInput->next;

	currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(unitDetailsOpponent->closeCombatAttackTotal, maximumCloseCombatAttackTotalForAnyUnit*COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);
	ANNexperienceInput* newExperienceInput6 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput6;
	currentExperienceInput = currentExperienceInput->next;

	currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(unitDetailsOpponent->longDistanceAttackTotal, maximumLongDistanceAttackTotalForAnyUnit*COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);
	ANNexperienceInput* newExperienceInput7 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput7;
	currentExperienceInput = currentExperienceInput->next;

	currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(unitDetailsOpponent->defenceTotal, maximumDefenceTotalForAnyUnit*COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);
	ANNexperienceInput* newExperienceInput8 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput8;
	currentExperienceInput = currentExperienceInput->next;



	//cout << "3d5" << endl;

	double distanceBetweenUnits = LRRCcombat.calculateTheDistanceBetweenTwoUnits(&(unitReference->absolutePosition), &(unitReferenceOpponent->absolutePosition));
	currentExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(distanceBetweenUnits, maximumMovementDistanceForAnyUnit);
	ANNexperienceInput* newExperienceInput9 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput9;
	currentExperienceInput = currentExperienceInput->next;
	//add 2D relationship map information here - can be added to a hypbrid 1D/2D neural network...


	currentExperienceInput->inputValue = currentPhase;
	ANNexperienceInput* newExperienceInput10 = new ANNexperienceInput();
	currentExperienceInput->next = newExperienceInput10;
	currentExperienceInput = currentExperienceInput->next;

	//cout << "3d6" << endl;
}



#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
	//add int currentPhase parameter???
bool LRRCgameAIClass::generateExperienceFromGlobalDecision(const UnitListClass* firstUnitInUnitList, LDreference* initialReferenceInThisPhaseStartScene, const LDreference* unitReference, const LDreference* unitReferenceOpponent, ANNexperience* currentExperience)
{
	bool result = true;


	int xPosRelToUnit = (unitReferenceOpponent->absolutePosition.x - unitReference->absolutePosition.x);
	int yPosRelToUnit = (unitReferenceOpponent->absolutePosition.y - unitReference->absolutePosition.y);
	if((abs(xPosRelToUnit) > GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS) || (abs(yPosRelToUnit) > GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS))
	{
		xPosRelToUnit = GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS;
		yPosRelToUnit = GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS;
	}
	int xPosRelToUnitAsTargetValue = xPosRelToUnit + GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS;
	int yPosRelToUnitAsTargetValue = yPosRelToUnit + GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS;
	currentExperience->classTargetValue = xPosRelToUnitAsTargetValue*GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_1D + yPosRelToUnitAsTargetValue;



	//fill experience input tree with blank input values
	ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;

	for(int x=-GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS; x<=GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS; x++)
	{
		for(int y=-GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS; y<=GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS; y++)
		{
			for(int t=0; t<GLOBAL_EXPERIENCE_NUM_INPUT_TYPES; t++)
			{
				ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
				currentExperienceInput->next = newExperienceInput;
				currentExperienceInput = currentExperienceInput->next;
			}
		}
	}

	//generate2DMemoryMapExperience of LDAttack, CCattack, defence and movement totals [4 input types] for each unit group centred around unitReference - linear/Euclidian spatial map going from 0 to 100 radius in 10 unit intervals - total inputs = 10x10x4

	if(!this->generate2DMemoryMapExperience(firstUnitInUnitList, initialReferenceInThisPhaseStartScene, unitReference, currentExperience))
	{
		result = false;
	}
	return result;

	//cout << "3d6" << endl;
}



bool LRRCgameAIClass::generate2DMemoryMapExperience(const UnitListClass* firstUnitInGroup, LDreference* initialReferenceInThisPhaseStartScene, const LDreference* unitReference, ANNexperience* currentExperience)
{
	bool result = true;
	const UnitListClass* currentUnitInGroup = firstUnitInGroup;

	int maximumMovementDistanceForAnyUnit = 0;
	int maximumCloseCombatAttackTotalForAnyUnit = 0;
	int maximumLongDistanceAttackTotalForAnyUnit = 0;
	int maximumDefenceHeadTotalForAnyUnit = 0;
	int maximumDefenceTorsoTotalForAnyUnit = 0;
	int maximumDefenceShieldTotalForAnyUnit = 0;
	int maximumDefenceTotalForAnyUnit = 0;
	XMLrulesClass* currentReferenceRulesClass;

	currentReferenceRulesClass = LRRCrulesUnitTypeCatagories;
	while(currentReferenceRulesClass->next != NULL)
	{
		maximumMovementDistanceForAnyUnit = SHAREDvars.maxInt(maximumMovementDistanceForAnyUnit, currentReferenceRulesClass->attribute4);

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackCloseCombat;
	while(currentReferenceRulesClass->next != NULL)
	{
		maximumCloseCombatAttackTotalForAnyUnit = SHAREDvars.maxInt(maximumCloseCombatAttackTotalForAnyUnit, (currentReferenceRulesClass->attribute4 + currentReferenceRulesClass->attribute5));

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
	while(currentReferenceRulesClass->next != NULL)
	{
		maximumLongDistanceAttackTotalForAnyUnit = SHAREDvars.maxInt(maximumLongDistanceAttackTotalForAnyUnit, currentReferenceRulesClass->attribute4);

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceHead;
	while(currentReferenceRulesClass->next != NULL)
	{
		maximumDefenceHeadTotalForAnyUnit = SHAREDvars.maxInt(maximumDefenceHeadTotalForAnyUnit, currentReferenceRulesClass->attribute4);

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceTorso;
	while(currentReferenceRulesClass->next != NULL)
	{
		maximumDefenceTorsoTotalForAnyUnit = SHAREDvars.maxInt(maximumDefenceTorsoTotalForAnyUnit, currentReferenceRulesClass->attribute4);

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceShield;
	while(currentReferenceRulesClass->next != NULL)
	{
		maximumDefenceShieldTotalForAnyUnit = SHAREDvars.maxInt(maximumDefenceShieldTotalForAnyUnit, currentReferenceRulesClass->attribute4);

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
	maximumDefenceTotalForAnyUnit = maximumDefenceHeadTotalForAnyUnit + maximumDefenceTorsoTotalForAnyUnit + maximumDefenceShieldTotalForAnyUnit;



	while(currentUnitInGroup->next != NULL)
	{
		if(currentUnitInGroup->status == true)
		{
			//1. create a dummy reference for details calculations
			LDreference* currentReferenceInGroup = new LDreference(true);
			currentReferenceInGroup->colour = currentUnitInGroup->team;
			currentReferenceInGroup->name = currentUnitInGroup->name;
			if(!LRRCcombat.searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(currentReferenceInGroup, initialReferenceInThisPhaseStartScene, NULL, false))
			{
				cout << "generate2DMemoryMapExperience{}: Error - unit reference not found in reference list" << endl;
				cout << "\terror: cannot find player unit reference in scene file, name = " << currentReferenceInGroup->name << " id=" << currentReferenceInGroup->colour << endl;
				result = false;
			}
			else
			{
				LRRCcombat.performFinalUnitGroupClassCalculations(currentReferenceInGroup->subModelDetails);

				int xPosRelToUnit = (currentReferenceInGroup->absolutePosition.x - unitReference->absolutePosition.x);
				int yPosRelToUnit = (currentReferenceInGroup->absolutePosition.y - unitReference->absolutePosition.y);
				bool refIn2DMemoryMap = true;
				if((abs(xPosRelToUnit) > GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS) || (abs(yPosRelToUnit) > GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS))
				{
					refIn2DMemoryMap = false;
				}
				if(refIn2DMemoryMap)
				{
					ANNexperienceInput* currentReferenceExperienceInput = this->findExperienceInputIn2DMemoryMap(xPosRelToUnit, yPosRelToUnit, 0, currentExperience);

					//fill experience inputs
					currentReferenceExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(currentReferenceInGroup->subModelDetails->movementSpeed, maximumMovementDistanceForAnyUnit);
					currentReferenceExperienceInput = currentReferenceExperienceInput->next;

					currentReferenceExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(currentReferenceInGroup->subModelDetails->closeCombatAttackTotal, maximumCloseCombatAttackTotalForAnyUnit*COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);
					currentReferenceExperienceInput = currentReferenceExperienceInput->next;

					currentReferenceExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(currentReferenceInGroup->subModelDetails->longDistanceAttackTotal, maximumLongDistanceAttackTotalForAnyUnit*COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);
					currentReferenceExperienceInput = currentReferenceExperienceInput->next;

					currentReferenceExperienceInput->inputValue = ANNexperienceClass.normaliseExperienceInput(currentReferenceInGroup->subModelDetails->defenceTotal, maximumDefenceTotalForAnyUnit*COMBAT_EXPERIENCE_NUMBER_OF_A_PART_IN_A_UNIT_MAX);
					currentReferenceExperienceInput = currentReferenceExperienceInput->next;
				}

				if(currentUnitInGroup->isUnitGroup)
				{
					this->generate2DMemoryMapExperience(currentUnitInGroup->firstUnitInUnitGroup, initialReferenceInThisPhaseStartScene, unitReference, currentExperience);
				}
			}

			delete currentReferenceInGroup;

		}

		currentUnitInGroup = currentUnitInGroup->next;
	}
	return result;
}





ANNexperienceInput* LRRCgameAIClass::findExperienceInputIn2DMemoryMap(const int xPos, const int yPos, const int tPos, ANNexperience* currentExperience)
{
	ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;
	for(int x=-GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS; x<xPos; x++)
	{
		for(int y=-GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS; y<GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS; y++)
		{
			for(int t=0; t<GLOBAL_EXPERIENCE_NUM_INPUT_TYPES; t++)
			{
				currentExperienceInput = currentExperienceInput->next;
			}
		}
	}
	for(int y=-GLOBAL_EXPERIENCE_NUMBER_OF_DIFFERENT_OUTPUT_DECISIONS_RADIUS; y<yPos; y++)
	{
		for(int t=0; t<GLOBAL_EXPERIENCE_NUM_INPUT_TYPES; t++)
		{
			currentExperienceInput = currentExperienceInput->next;
		}
	}
	for(int t=0; t<tPos; t++)
	{
		currentExperienceInput = currentExperienceInput->next;
	}
	return currentExperienceInput;
}
#endif






#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES




bool LRRCgameAIClass::generateExperienceFromObjectDecision(const LDreference* objectReference, const long objectDecision, const ANNexperience* currentExperience, const bool createNewRayTracedImage)
{
	bool result = true;


	//1. generate random camera + lighting position conditions;
		//CHECK THIS - need to randomly vary lighting conditions


	char experienceNumberStringCharStar[10];
	experienceNumberStringCharStar = SHAREDvars.convertIntToString(objectDecision);
	string experienceNumberString = "";
	experienceNumberString = experienceNumberString + experienceNumberStringCharStar;

	string imageFileNameStart = "";
	imageFileNameStart = imageFileNameStart + "objectImageExperience" + experienceNumberString + "fileName" + objectReference->name;

	string raytracedImagePPMNNSceneFileName = imageFileNameStart + RGB_MAP_PPM_EXTENSION;
	char* charstarraytracedImagePPMNNSceneFileName = const_cast<char*>(raytracedImagePPMNNSceneFileName.c_str());

	//2. generate pixmap using from LDreference name [under arbitrary lighting conditions] via raytracing
	if(createNewRayTracedImage)
	{

		#ifdef ANN_OBJECT_RECOGNITION_EXPERIENCES_RANDOMISE_LIGHTING_CONDITIONS
			//NB cannot use l3p parameters to randomise lighting conditions as l3p only takes in LDRAW colours as light sources (4bit)
			cout << "WARNING: cannot use l3p parameters to randomise lighting conditions as l3p only takes in LDRAW colours as light sources (4bit)" << endl;
		#endif

	#ifdef TH_USE_RT_FOR_OBJECT_RECOGNITION_VEC_GRAPHICS
		string sceneFileNameForRayTracing = imageFileNameStart + DAT_EXTENSION;
		string vectorGraphicsTALNNSceneFileName = imageFileNameStart + TAL_EXTENSION;
		char* charstarsceneFileNameForRayTracing = const_cast<char*>(sceneFileNameForRayTracing.c_str());
		char* charstarvectorGraphicsTALNNSceneFileName = const_cast<char*>(vectorGraphicsTALNNSceneFileName.c_str());


		//reparse scenefilewithandwithout sprites - to build absolute position information
		LDreference* initialReferenceInSceneFileForRayTracing = new LDreference();
		LDreference* topLevelReferenceInSceneFileForRayTracing = new LDreference(charstarsceneFileNameForRayTracing, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(!LDparser.parseFile(charstarsceneFileNameForRayTracing, initialReferenceInSceneFileForRayTracing, topLevelReferenceInSceneFileForRayTracing, true))
		{//file does not exist
			cout << "The file: " << charstarsceneFileNameForRayTracing << " does not exist in the directory" << endl;
			result = false;
		}

		if(!RTreferenceManipulation.write2DReferenceListCollapsedTo1DToFileRayTraceFormat(charstarvectorGraphicsTALNNSceneFileName, initialReferenceInSceneFileForRayTracing, false, NULL, false, NULL, NULL))
		{
			result = false;
		}

		if(!RTscene.rayTraceScene(charstarvectorGraphicsTALNNSceneFileName, charstarraytracedImagePPMNNSceneFileName, true, false, NULL, NULL, NULL, NULL))
		{
			result = false;
		}
	#else
		#ifdef TH_USE_POVRAY_FOR_OBJECT_RECOGNITION_VEC_GRAPHICS
		//use third party raytracer, povray;

		string temp = "";
		string l3pCommand = temp + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_1 + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_2 + objectReference->name + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_4 + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_5 + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_6 + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_7;
		string povrayCommand = temp + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_1 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_2 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_3 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_4 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_5 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_6 + raytracedImagePPMNNSceneFileName + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_8 + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_9 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_10 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_11 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_12 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_13 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_14;

		//Eg system("env LDRAWDIR=/usr/share/local/LDRAW l3p /usr/share/local/LDRAW/MODELS/car.dat object -cg30,45 -b -f");
		//Eg system("povray object.pov");
		cout << l3pCommand <<endl;
		system(l3pCommand.c_str());
		cout << povrayCommand <<endl;
		system(povrayCommand.c_str());
		#else
			cout << "Error: no ray tracer enabled" << endl;
			exit(0);
		#endif
	#endif

	}
	else
	{
		//ray traced image has already been created
	}

	//cout << "3" << endl;
	//3. load pixmap into RAM objectImage from generated pixmap
	pixmap* objectImage;
	cout << "image file being loaded = " << charstarraytracedImagePPMNNSceneFileName << endl;
	objectImage = load_ppm(charstarraytracedImagePPMNNSceneFileName);

	//cout << "4" << endl;
	//4. produce contrast map from pixmap image
	unsigned char* rgbMap = new unsigned char [objectImage->wide* objectImage->high* RGB_NUM];
	double* luminosityMap = new double[objectImage->wide* objectImage->high];
	bool* luminosityBooleanMap = new bool[objectImage->wide* objectImage->high];
	double* luminosityContrastMap = new double[objectImage->wide* objectImage->high];
	bool* luminosityContrastBooleanMap = new bool[objectImage->wide* objectImage->high];

	RTpixelMaps.createRGBMapFromPixmapImage(objectImage, rgbMap);


	RTpixelMaps.createLuminosityMapFromRGBMap(objectImage->wide, objectImage->high, rgbMap, luminosityMap);
		//createLuminosityMapFromPixmapImage(objectImage, luminosityMap);
	RTpixelMaps.createLuminosityBooleanMap(objectImage->wide, objectImage->high, luminosityMap, luminosityBooleanMap);
	RTpixelMaps.createContrastMapFromMap(objectImage->wide, objectImage->high, luminosityMap, luminosityContrastMap);
	RTpixelMaps.createLuminosityContrastBooleanMap(objectImage->wide, objectImage->high, luminosityContrastMap, luminosityContrastBooleanMap);

	//cout << "5" << endl;
	//5. apply quadratic fit to image [from Java code];
		//CHECK THIS - accuracy of quadratic fit is not currently required

	//fill experience input tree

	#ifdef GAME_OBJECT_RECOGNITION_EXPERIENCE_FEED_RGB_MAP
	generateExperienceWith2DRGBMap(rgbMap, objectImage->wide, objectImage->high, currentExperience, objectDecision);
	#elif defined GAME_OBJECT_RECOGNITION_EXPERIENCE_FEED_LUMINOSITY_BOOLEAN_MAP
	generateExperienceWith2DBooleanMap(luminosityBooleanMap, objectImage->wide, objectImage->high, currentExperience, objectDecision);
	#elif defined GAME_OBJECT_RECOGNITION_EXPERIENCE_LUMINOSITY_CONTRAST_BOOLEAN_MAP
	generateExperienceWith2DBooleanMap(luminosityContrastBooleanMap, objectImage->wide, objectImage->high, currentExperience, objectDecision);
	#elif defined GAME_OBJECT_RECOGNITION_EXPERIENCE_FEED_LUMINOSITY_MAP
	generateExperienceWith2DMap(luminosityMap, objectImage->wide, objectImage->high, LUMINOSITY_MAP_MAX_LUMINOSITY_VALUE, currentExperience, objectDecision);
	#elif defined GAME_OBJECT_RECOGNITION_EXPERIENCE_LUMINOSITY_CONTRAST_MAP
	generateExperienceWith2DMap(luminosityContrastMap, objectImage->wide, objectImage->high, CONTRAST_MAP_MAX_CONTRAST_VALUE, currentExperience, objectDecision);
	#else
	cout << "Error: no feed defined" << endl;
	exit(0);
	#endif



	//cout << "4g" << endl;

	#ifdef DEBUG_ANN_OBJECT_RECOGNITION_EXPERIENCES_OUTPUT_ALL_MAPS
	string PPMFileNameLuminosity = (imageFileNameStart) + LUMINOSITY_MAP_PPM_EXTENSION;
	char* PPMFileNameLuminosityCharArray = const_cast<char*>(PPMFileNameLuminosity.c_str());
	cout << "PPMFileNameLuminosityCharArray = " << PPMFileNameLuminosityCharArray << endl;
	RTpixelMaps.generatePixmapFromLuminosityMap(PPMFileNameLuminosityCharArray, objectImage->wide, objectImage->high, luminosityMap);

	string PPMFileNameContrast = (imageFileNameStart) + LUMINOSITY_CONTRAST_MAP_PPM_EXTENSION;
	char* PPMFileNameContrastCharArray = const_cast<char*>(PPMFileNameContrast.c_str());
	cout << "PPMFileNameContrastCharArray = " << PPMFileNameContrastCharArray << endl;
	RTpixelMaps.generatePixmapFromLuminosityContrastMap(PPMFileNameContrastCharArray, objectImage->wide, objectImage->high, luminosityContrastMap);

	string PPMFileNameLuminosityBoolean = (imageFileNameStart) + LUMINOSITY_BOOLEAN_MAP_PPM_EXTENSION;
	char* PPMFileNameLuminosityBooleanCharArray = const_cast<char*>(PPMFileNameLuminosityBoolean.c_str());
	cout << "PPMFileNameLuminosityBooleanCharArray = " << PPMFileNameLuminosityBooleanCharArray << endl;
	RTpixelMaps.generatePixmapFromBooleanMap(PPMFileNameLuminosityBooleanCharArray, objectImage->wide, objectImage->high, luminosityBooleanMap);

	string PPMFileNameContrastBoolean = (imageFileNameStart) + LUMINOSITY_CONTRAST_BOOLEAN_MAP_PPM_EXTENSION;
	char* PPMFileNameContrastBooleanCharArray = const_cast<char*>(PPMFileNameContrastBoolean.c_str());
	cout << "PPMFileNameContrastBooleanCharArray = " << PPMFileNameContrastBooleanCharArray << endl;
	RTpixelMaps.generatePixmapFromBooleanMap(PPMFileNameContrastBooleanCharArray, objectImage->wide, objectImage->high, luminosityContrastBooleanMap);
	#endif

	//cout << "6" << endl;

		//memory clean up

	delete luminosityContrastMap;
	delete luminosityMap;



	/*DOESNT WORK FOR SOME REASON;
	delete objectImage;
	delete objectImageContrast;
	delete objectImageLuminosity;
	//free_pixmap(objectImage);
	//free_pixmap(objectImageContrast);
	//free_pixmap(objectImageLuminosity);
	*/
	free_pixmap(objectImage);

	return result;

	//cout << "3d6" << endl;
}

#endif





/*
void generate2DMemoryMapExperience(LDreference* firstReferenceInLayer,  LDreference* unitReference, ANNexperience* currentExperience)
{
	LDreference* currentReference = firstReferenceInLayer;

	while(currentReference->next != NULL)
	{
		if(currentReference->isSubModelReference)
		{
			int xPosRelToUnit = (currentReference->absolutePosition.x - unitReference->absolutePosition.x);
			int yPosRelToUnit = (currentReference->absolutePosition.y - unitReference->absolutePosition.y);
			bool refIn2DMemoryMap = true;
			if((xPosRelToUnit > EXPERIENCE_FROM_UNIT_TARGETTING_OUTCOME_RADIUS/EXPERIENCE_FROM_UNIT_TARGETTING_OUTCOME_INTERVAL) || (yPosRelToUnit > EXPERIENCE_FROM_UNIT_TARGETTING_OUTCOME_RADIUS/EXPERIENCE_FROM_UNIT_TARGETTING_OUTCOME_INTERVAL))
			{
				refIn2DMemoryMap = false;
			}
			if(refIn2DMemoryMap)
			{
				ANNexperienceInput* currentReferenceExperienceInput = findExperienceInputIn2DMemoryMap(xPosRelToUnit, yPosRelToUnit, 0, currentExperience);
			}

			generate2DMemoryMapExperience(currentReference->firstReferenceWithinSubModel, unitReference, currentExperience)
		}

		currentReference = currentReference->next;

	}
}
*/





























void LRRCgameAIClass::addAllUnitExperiencesToOFStream(ofstream* experienceDataSetOFStreamObject, UnitListClass* firstUnitInUnitGroup, const int nnIndex)
{
	UnitListClass* currentUnitInList = firstUnitInUnitGroup;

	while(currentUnitInList->next != NULL)
	{
		#ifndef DEBUG_DO_NOT_PERFORM_KILL_RATIO_CHECKS_BEFORE_ADDING_UNIT_EXPERIENCES_TO_NN
		if(this->checkAverageKillRatioForUnitGroup(currentUnitInList) || (currentUnitInList->unitDetails->numPerson == 1))
		{
		#endif
			ANNexperienceClass.addExperiencesToOFStream(experienceDataSetOFStreamObject, currentUnitInList->firstExperience[nnIndex]);

			if(currentUnitInList->isUnitGroup)
			{
				this->addAllUnitExperiencesToOFStream(experienceDataSetOFStreamObject, currentUnitInList->firstUnitInUnitGroup, nnIndex);
			}
		#ifndef DEBUG_DO_NOT_PERFORM_KILL_RATIO_CHECKS_BEFORE_ADDING_UNIT_EXPERIENCES_TO_NN
		}
		#endif

		currentUnitInList = currentUnitInList->next;
	}
}

void LRRCgameAIClass::addPlayerUnitExperiencesToOFStream(ofstream* experienceDataSetOFStreamObject, UnitListClass* firstUnitInUnitGroup, const int unitGroupTeam, const int nnIndex)
{
	UnitListClass* currentUnitInList = firstUnitInUnitGroup;

	while(currentUnitInList->next != NULL)
	{
		#ifndef DEBUG_DO_NOT_PERFORM_KILL_RATIO_CHECKS_BEFORE_ADDING_UNIT_EXPERIENCES_TO_NN
		if(this->checkAverageKillRatioForUnitGroup(currentUnitInList) || (currentUnitInList->unitDetails->numPerson == 1))
		{
		#endif
			if(currentUnitInList->team == unitGroupTeam)
			{
				ANNexperienceClass.addExperiencesToOFStream(experienceDataSetOFStreamObject, currentUnitInList->firstExperience[nnIndex]);
			}

			if(currentUnitInList->isUnitGroup)
			{
				this->addPlayerUnitExperiencesToOFStream(experienceDataSetOFStreamObject, currentUnitInList->firstUnitInUnitGroup, unitGroupTeam, nnIndex);
			}
		#ifndef DEBUG_DO_NOT_PERFORM_KILL_RATIO_CHECKS_BEFORE_ADDING_UNIT_EXPERIENCES_TO_NN
		}
		#endif

		currentUnitInList = currentUnitInList->next;
	}
}


#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
	//NB this routine [for ADD_EXPERIENCE] records player experiences rather than unit experiences
double LRRCgameAIClass::addOrCompareAllObjectExperiences(Player* currentPlayer, const int NNBeingTested, const bool addOrCompareExperience, const bool justCountNumberOfExperiences)
{
	double totalError = 0.0;
	int numberOfExperiences = 0;

	XMLrulesClass* currentReferenceRulesClass;
	LDreference* unitReference = new LDreference();

	currentReferenceRulesClass = LRRCrulesUnitTypeDetails;
	while(currentReferenceRulesClass->next != NULL)
	{
		if(justCountNumberOfExperiences)
		{

		}
		else
		{
			//cout << "here" << endl;
			//cout << "\n\n\ncurrentPlayer->currentExperience[NNBeingTested]->firstExperienceInput->inputValue = " << currentPlayer->currentExperience[NNBeingTested]->firstExperienceInput->inputValue << endl;

			unitReference->name = currentReferenceRulesClass->stringValue;
			//cout << "unitReference->name = " << unitReference->name << "\n\n" << endl;
			totalError = totalError + this->addOrCompareExperienceFromUnitDecision(NULL, NULL, unitReference, NULL, numberOfExperiences, currentPlayer, NNBeingTested, addOrCompareExperience, NULL);
		}
		numberOfExperiences++;
		currentReferenceRulesClass = currentReferenceRulesClass -> next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceHead;
	while(currentReferenceRulesClass->next != NULL)
	{
		if(justCountNumberOfExperiences)
		{

		}
		else
		{
			unitReference->name = currentReferenceRulesClass->stringValue;
			totalError = totalError + this->addOrCompareExperienceFromUnitDecision(NULL, NULL, unitReference, NULL, numberOfExperiences, currentPlayer, NNBeingTested, addOrCompareExperience, NULL);
		}
		numberOfExperiences++;
		currentReferenceRulesClass = currentReferenceRulesClass -> next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceTorso;
	while(currentReferenceRulesClass->next != NULL)
	{
		if(justCountNumberOfExperiences)
		{

		}
		else
		{
			unitReference->name = currentReferenceRulesClass->stringValue;
			totalError = totalError + this->addOrCompareExperienceFromUnitDecision(NULL, NULL, unitReference, NULL, numberOfExperiences, currentPlayer, NNBeingTested, addOrCompareExperience, NULL);
		}
		numberOfExperiences++;
		currentReferenceRulesClass = currentReferenceRulesClass -> next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceShield;
	while(currentReferenceRulesClass->next != NULL)
	{
		if(justCountNumberOfExperiences)
		{

		}
		else
		{
			unitReference->name = currentReferenceRulesClass->stringValue;
			totalError = totalError + this->addOrCompareExperienceFromUnitDecision(NULL, NULL, unitReference, NULL, numberOfExperiences, currentPlayer, NNBeingTested, addOrCompareExperience, NULL);
		}
		numberOfExperiences++;
		currentReferenceRulesClass = currentReferenceRulesClass -> next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackCloseCombat;
	while(currentReferenceRulesClass->next != NULL)
	{
		if(justCountNumberOfExperiences)
		{

		}
		else
		{
			unitReference->name = currentReferenceRulesClass->stringValue;
			totalError = totalError + this->addOrCompareExperienceFromUnitDecision(NULL, NULL, unitReference, NULL, numberOfExperiences, currentPlayer, NNBeingTested, addOrCompareExperience, NULL);
		}
		numberOfExperiences++;
		currentReferenceRulesClass = currentReferenceRulesClass -> next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
	while(currentReferenceRulesClass->next != NULL)
	{
		if(justCountNumberOfExperiences)
		{

		}
		else
		{
			unitReference->name = currentReferenceRulesClass->stringValue;
			totalError = totalError + this->addOrCompareExperienceFromUnitDecision(NULL, NULL, unitReference, NULL, numberOfExperiences, currentPlayer, NNBeingTested, addOrCompareExperience, NULL);
		}
		numberOfExperiences++;
		currentReferenceRulesClass = currentReferenceRulesClass -> next;
	}

	delete unitReference;

	if(justCountNumberOfExperiences)
	{
		return numberOfExperiences;
	}
	else
	{
		return totalError;
	}


}

#endif

/*OLD:


void addExperienceToByteArray()
{

}

	long ExperienceDataSetByteArrayCurrentPosition = 0;
	long ExperienceDataSetByteArraySize = XML_FILE_MAX_SIZE;
	char* ExperienceDataSetByteArray = new char[EXPERIENCE_DATASET_MAX_SIZE];

*/

#endif
