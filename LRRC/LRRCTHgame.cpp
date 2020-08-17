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
 * File Name: LRRCTHgame.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3i19d 15-December-2016
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#include "LRRCglobalDefs.h"
#include "LRRCTHgame.h"
#include "LRRCgame.h"
#include "LRRCmovement.h"
#include "LRRCcombat.h"
#include "LRRCsprite.h"
#include "LDparser.h"
#include "LDreferenceManipulation.h"
#include "RTreferenceManipulation.h"
#include "LRRCgameReferenceManipulation.h"
#include "LRRCrules.h"

#ifdef USE_RT
#include "RTscene.h"
#endif


#include "ANNneuronClass.h"
#include "ANNFormation.h"
#include "LRRCgameAI.h"
#include "LRRCunitClass.h"
#include "ANNTraining.h"
#include "ANNXMLconversion.h"
#include "ANNsprite.h"
#include "ANNUpdateAlgorithm.h"
#include "ANNdisplay.h"


//Test Harness Code:


bool cullUnitList(UnitListClass* firstUnitInUnitList, int numberOfUnitsRequired)
{
	UnitListClass* currentUnitInUnitList = firstUnitInUnitList;

	for(int i=0; i<numberOfUnitsRequired; i++)
	{
		currentUnitInUnitList = currentUnitInUnitList->next;
	}
	currentUnitInUnitList->next = NULL;

	return true;
}

bool randomiseUnitList(UnitListClass* firstUnitInNonRandomisedUnitList, UnitListClass* firstUnitInUnitList, long numberOfUnits)
{
	UnitListClass* currentUnitInNonRandomisedUnitList = firstUnitInNonRandomisedUnitList->next;
	UnitListClass* previousUnitInNonRandomisedUnitList = firstUnitInNonRandomisedUnitList;
	UnitListClass* currentUnitInUnitList = firstUnitInUnitList;

	long indexOfCurrentUnitInNonRandomisedUnitList = 0;
	long currentNumberOfUnitsInRandomisedUnitList = numberOfUnits;
	double randomNumberBetween0And1 = ((double(abs((short)rand())))/(ABS_OF_SHORT));
	long randomUnitSelected = (long)(randomNumberBetween0And1*(double)currentNumberOfUnitsInRandomisedUnitList);
	//cout << "randomUnitSelected = " << randomUnitSelected << endl;

	while(currentNumberOfUnitsInRandomisedUnitList != 0)		//(currentNumberOfUnitsInRandomisedUnitList != 0) or currentUnitInNonRandomisedUnitList->next != NULL)	//or (currentNumberOfUnitsInRandomisedUnitList != 0)
	{
		/*
		if(currentUnitInNonRandomisedUnitList->next == NULL)
		{
			cout << "(currentUnitInNonRandomisedUnitList->next == NULL)" << endl;
		}
		*/

		//cout << "0" << endl;
		randomNumberBetween0And1 = ((double(abs((short)rand())))/(ABS_OF_SHORT));
		randomUnitSelected = (long)(randomNumberBetween0And1*(double)currentNumberOfUnitsInRandomisedUnitList);
		//cout << "randomUnitSelected = " << randomUnitSelected << endl;

		/*
		if(randomUnitSelected == (currentNumberOfUnitsInRandomisedUnitList-1))
		{
			exit(0);
		}
		*/

		if(indexOfCurrentUnitInNonRandomisedUnitList == randomUnitSelected)
		{
			//cout << "1" << endl;

			UnitListClass* newUnit = new UnitListClass();
			newUnit->number = currentUnitInNonRandomisedUnitList->number;
			ModelDetails* newUnitDetails = new ModelDetails();
			newUnit->unitDetails = newUnitDetails;
			copyAllUnitDetails(newUnit->unitDetails, currentUnitInNonRandomisedUnitList->unitDetails);
			currentUnitInUnitList->next = newUnit;
			currentUnitInUnitList = currentUnitInUnitList->next;

			//remove currentUnitInNonRandomisedUnitList from possible units
			previousUnitInNonRandomisedUnitList->next = currentUnitInNonRandomisedUnitList->next;

			if(indexOfCurrentUnitInNonRandomisedUnitList == 0)
			{
				firstUnitInNonRandomisedUnitList->next = currentUnitInNonRandomisedUnitList->next;
			}

			currentNumberOfUnitsInRandomisedUnitList--;

			//cout << "2" << endl;
		}
		else
		{
			previousUnitInNonRandomisedUnitList = previousUnitInNonRandomisedUnitList->next;
		}
		currentUnitInNonRandomisedUnitList = currentUnitInNonRandomisedUnitList->next;
		indexOfCurrentUnitInNonRandomisedUnitList++;
		//cout << "indexOfCurrentUnitInNonRandomisedUnitList = " << indexOfCurrentUnitInNonRandomisedUnitList << endl;

		if(currentUnitInNonRandomisedUnitList->next == NULL)	//indexOfCurrentUnitInNonRandomisedUnitList == currentNumberOfUnits
		{//go back to start of loop...
			//cout << "here" << endl;
			indexOfCurrentUnitInNonRandomisedUnitList = 0;
			currentUnitInNonRandomisedUnitList = firstUnitInNonRandomisedUnitList->next;
			previousUnitInNonRandomisedUnitList = firstUnitInNonRandomisedUnitList;
		}
		//cout << "4" << endl;

	}
	currentUnitInUnitList->next = NULL;

	return true;
}





int THtestANNusingCombatExperiences()
{
	//************************
	//PART 0: Standard Initialisations
	//************************

	bool result = true;

	if(!parseLRRCrulesXMLfile())
	{
		result = false;
	}

	fillInCombatExternVariables();
	fillInGameExternVariables();
	fillInModelClassExternVariables();
	fillInParserExternVariables();
	fillInPlayerClassExternVariables();
	fillInLRRCSpriteExternVariables();

	Player* currentPlayer = new Player();

	#define IRRELEVANT (0)

	//************************
	//PART 1: Standard Initialisations; create neural network 1 (for unit properties and unit combat decisions)
	//************************

	long numberOfInputNeurons;
	long numberOfOutputNeurons;

#ifdef TH_LRRC_GAME_USE_CC_EXPERIENCES
	initialiseNeuralNetwork(GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, currentPlayer, GAME_PHASE_CLOSECOMBAT);		//create neural network 1 (for unit/group targetting/combat decisions)
	initialiseNeuralNetwork(GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, currentPlayer, GAME_PHASE_CLOSECOMBAT);		//create neural network 2 (for unit/group targetting/combat decisions)
#endif
#ifdef TH_LRRC_GAME_USE_LD_EXPERIENCES
#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
	initialiseNeuralNetwork(GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, currentPlayer, GAME_PHASE_LONGDISTANCECOMBAT);	//create neural network 1 (for unit/group targetting/combat decisions)
	initialiseNeuralNetwork(GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, currentPlayer, GAME_PHASE_LONGDISTANCECOMBAT);	//create neural network 2 (for unit/group targetting/combat decisions)
#endif
#endif

/*
#ifdef TH_LRRC_GAME_USE_GLOBAL_EXPERIENCES
	//create neural network 3 (for global targetting/combat decisions)
	initialiseNeuralNetwork(GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN, currentPlayer, GAME_PHASE_CLOSECOMBAT);
#endif
*/

#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
	//create neural network 4 (for object recognition)
	initialiseNeuralNetwork(GAME_INDEX_OF_OBJECT_EXPERIENCE_NN, currentPlayer, GAME_PHASE_CLOSECOMBAT);
#endif

	//************************
	//PART 2: Generate a list of Units with Random Unit Properties
	//************************

	UnitListClass* firstUnitInNonRandomisedUnitList = new UnitListClass();


	int numberOfUnitsBeforeCull = THgenerateUnitListwithAllUnitProperties(firstUnitInNonRandomisedUnitList);

	UnitListClass* firstUnitInUnitList = new UnitListClass();
	firstUnitInUnitList->number = 0;
	ModelDetails* newUnitDetails = new ModelDetails();
	firstUnitInUnitList->unitDetails = newUnitDetails;
	copyAllUnitDetails(firstUnitInUnitList->unitDetails, firstUnitInNonRandomisedUnitList->unitDetails);

	randomiseUnitList(firstUnitInNonRandomisedUnitList, firstUnitInUnitList, (numberOfUnitsBeforeCull-1));

	int numberOfUnits = TH_LRRC_GAME_NUMBER_OF_UNITS_TO_FEED;
	cullUnitList(firstUnitInUnitList, numberOfUnits);

	currentPlayer->firstUnitInUnitList = firstUnitInUnitList;


	/*
	//temp - print out unit list
	//int uin = 0;
	UnitListClass* currentUnitInList = firstUnitInUnitList;
	while(currentUnitInList->next != NULL)
	{
		//uin++;

		cout << "currentUnitInList->number = " << currentUnitInList->number << endl;

		//cout << "currentUnitInList->closeCombatAttackValue = " << currentUnitInList->unitDetails->closeCombatAttackValue << endl;
		//cout << "currentUnitInList->longDistanceAttackValue = " << currentUnitInList->unitDetails->longDistanceAttackValue << endl;
		//cout << "currentUnitInList->closeCombatAttackTotal = " << currentUnitInList->unitDetails->closeCombatAttackTotal << endl;
		//cout << "currentUnitInList->longDistanceAttackTotal = " << currentUnitInList->unitDetails->longDistanceAttackTotal << endl;
		//cout << "currentUnitInList->defenceTotal = " << currentUnitInList->unitDetails->defenceTotal << endl;


		currentUnitInList = currentUnitInList->next;
	}
	//cout << "uin = " << uin << endl;
	*/

	//************************
	//PART 3: perform Generic Combat With All Permutations Of Units InList And Add Experiences
	//************************

	//Perform testing over the following neural networks [NNBeingTested] and scenarios [currentPhase];
	int numberOfNetworks = GAME_NUMBER_OF_EXPERIENCE_NN_UTILISED;

#ifdef TH_LRRC_GAME_USE_RANDOM_DISTANCES_TO_TRAIN_NETWORKS
	int numberOfDistancesToTrainNetwork = TH_LRRC_GAME_NUM_RANDOM_DISTANCES_TO_TRAIN_NETWORKS;
#else
	int numberOfDistancesToTrainNetwork = 1;
#endif

#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
	#ifdef TH_LRRC_GAME_USE_CC_EXPERIENCES
		#ifdef TH_LRRC_GAME_USE_LD_EXPERIENCES
			int numberOfScenarios = 2;
		#else
			int numberOfScenarios = 1;
		#endif
	#else
		int numberOfScenarios = 1;
	#endif
#else
	int numberOfScenarios = 1;
#endif


#ifndef TH_LRRC_GAME_DO_NOT_CHECK_EVASION_IN_NN_ERROR
	int numberOfExperiencesPerUnitPerScenario = 2;
#else
	int numberOfExperiencesPerUnitPerScenario = 1;
#endif
	int currentPhase;
	int NNBeingTested;


#ifdef TH_LRRC_GAME_USE_CC_EXPERIENCES
	currentPhase = GAME_PHASE_CLOSECOMBAT;
	NNBeingTested = GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN;
	THperformGenericCombatWithAllPermutationsOfUnitsInListAndAddOrCompareExperiences(currentPhase, firstUnitInUnitList, currentPlayer, NNBeingTested, ADD_EXPERIENCE, numberOfDistancesToTrainNetwork);

	currentPhase = GAME_PHASE_CLOSECOMBAT;
	NNBeingTested = GAME_INDEX_OF_COMBAT_EXPERIENCE_NN;
	THperformGenericCombatWithAllPermutationsOfUnitsInListAndAddOrCompareExperiences(currentPhase, firstUnitInUnitList, currentPlayer, NNBeingTested, ADD_EXPERIENCE, numberOfDistancesToTrainNetwork);
#endif

#ifdef TH_LRRC_GAME_USE_LD_EXPERIENCES
	currentPhase = GAME_PHASE_LONGDISTANCECOMBAT;
	NNBeingTested = GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN;
	THperformGenericCombatWithAllPermutationsOfUnitsInListAndAddOrCompareExperiences(currentPhase, firstUnitInUnitList, currentPlayer, NNBeingTested, ADD_EXPERIENCE, numberOfDistancesToTrainNetwork);

	currentPhase = GAME_PHASE_LONGDISTANCECOMBAT;
	NNBeingTested = GAME_INDEX_OF_COMBAT_EXPERIENCE_NN;
	THperformGenericCombatWithAllPermutationsOfUnitsInListAndAddOrCompareExperiences(currentPhase, firstUnitInUnitList, currentPlayer, NNBeingTested, ADD_EXPERIENCE, numberOfDistancesToTrainNetwork);
#endif

#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
	NNBeingTested = GAME_INDEX_OF_OBJECT_EXPERIENCE_NN;
	//cout << "here1"
	addOrCompareAllObjectExperiences(currentPlayer, NNBeingTested, ADD_EXPERIENCE, false);
#endif



	//************************
	//PART 4: feed neural network with all experiences
	//************************

	/*
	if(numberOfUnits <= 100)	//for debugging speed, do not generate large files
	{
	*/

	int currentGame = 1;
		//CHECKTHIS should start from 0


	int startNN = 0;
	int endNN = (numberOfNetworks*numberOfScenarios);

	#ifdef TEMP_OUTPUT_HIGH_ERROR_EXPERIENCES
		#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
		startNN=3;
		endNN = 4;
		#endif
	#endif

#ifdef DEMONSTRATION_SCENARIO2
	int nn = GAME_INDEX_OF_OBJECT_EXPERIENCE_NN;
#elif defined DEMONSTRATION_SCENARIO3
	int nn = GAME_INDEX_OF_OBJECT_EXPERIENCE_NN;
#elif defined DEMONSTRATION_SCENARIO1
	int nn = GAME_INDEX_OF_COMBAT_EXPERIENCE_NN;
#else
	for(int nn=startNN; nn < endNN; nn++)		//by default this is GAME_NUMBER_OF_EXPERIENCE_NN (3), ie, do not currently use global experiences, generateExperienceFromGlobalDecision
	{
#endif

		//create player experience list
		//currentPlayer->currentExperience[nn] = currentPlayer->firstExperience[nn];	//assumed true already
		mergeAllUnitExperiencesIntoPlayerExperienceList(currentPlayer, currentPlayer->firstUnitInUnitList, nn);

		/*
		feedNeuralNetworkWithGameUnitExperiences(firstInputNeuronInNetwork,firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, firstUnitInUnitList, nn);
		*/

		bool addSprites;
		bool allowRaytrace;

	#ifndef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES

		//only add sprites for combat experience neural network(s);
		if((nn >= GAME_INDEX_OF_COMBAT_EXPERIENCE_NN*numberOfScenarios) && (nn < (GAME_INDEX_OF_COMBAT_EXPERIENCE_NN*numberOfScenarios+numberOfScenarios)))
		{
			/*
			#ifdef TH_USE_RT_FOR_NEURAL_NETWORK_VEC_GRAPHICS
				addSprites = false;

			#else
			*/
				//always add sprites if using povray and when not raytracing [as povray is an optimised raytracer and can handle large scenes efficiently]
				addSprites = true;
			/*
			#endif
			*/
		}
		else
		{
			addSprites = false;
		}
	#else
		addSprites = false;
	#endif

	#ifdef TH_GAME_RAY_TRACE_NEURAL_NETWORK_VEC_GRAPHICS
		allowRaytrace = true;
	#else
		allowRaytrace = false;
	#endif

		trainAndOutputNeuralNetwork(currentPlayer->firstInputNeuronInNetwork[nn], currentPlayer->firstOutputNeuronInNetwork[nn], currentPlayer->numberOfInputNeurons[nn], currentPlayer->numberOfOutputNeurons[nn], currentPlayer->firstExperience[nn], addSprites, allowRaytrace, nn, currentGame);


		//cout << "H7" << endl;
#ifdef DEMONSTRATION_SCENARIO2

#elif defined DEMONSTRATION_SCENARIO3

#elif defined DEMONSTRATION_SCENARIO1

#else
	}
#endif


	/*
	}
	*/

	//************************
	//PART 5: Now Test Performance of Network; perform Generic Combat With All Permutations Of Units In List And Compare Experiences With NN
	//************************

	//Now Test Performance of Network;




#ifdef TH_LRRC_GAME_USE_CC_EXPERIENCES
	double totalErrorCCPropertiesExperience = 0;
	double totalErrorCCCombatExperience = 0;

	currentPhase = GAME_PHASE_CLOSECOMBAT;
	NNBeingTested = GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN;
	totalErrorCCPropertiesExperience = THperformGenericCombatWithAllPermutationsOfUnitsInListAndAddOrCompareExperiences(currentPhase, firstUnitInUnitList, currentPlayer, NNBeingTested, COMPARE_EXPERIENCE, numberOfDistancesToTrainNetwork);

	currentPhase = GAME_PHASE_CLOSECOMBAT;
	NNBeingTested = GAME_INDEX_OF_COMBAT_EXPERIENCE_NN;
	totalErrorCCCombatExperience = THperformGenericCombatWithAllPermutationsOfUnitsInListAndAddOrCompareExperiences(currentPhase, firstUnitInUnitList, currentPlayer, NNBeingTested, COMPARE_EXPERIENCE, numberOfDistancesToTrainNetwork);

	double averageErrorCCPropertiesExperience = totalErrorCCPropertiesExperience/((pow((double)numberOfUnits, 2)* (double)numberOfExperiencesPerUnitPerScenario* (double)numberOfDistancesToTrainNetwork));

	//cout << "averageErrorPropertiesExperience = " << averageErrorPropertiesExperience << endl;
	cout << "averageAccuracyCCPropertiesExperience = " << (1.0F-averageErrorCCPropertiesExperience)*100.0F << "%" <<endl;

	double averageErrorCCCombatExperience = totalErrorCCCombatExperience/((pow((double)numberOfUnits, 2)* (double)numberOfExperiencesPerUnitPerScenario* (double)numberOfDistancesToTrainNetwork));

	//cout << "averageErrorCombatExperience = " << averageErrorCombatExperience << endl;
	cout << "averageAccuracyCCCombatExperience = " << (1.0F-averageErrorCCCombatExperience)*100.0F << "%" <<endl;

#endif

#ifdef TH_LRRC_GAME_USE_LD_EXPERIENCES
	double totalErrorLDPropertiesExperience = 0;
	double totalErrorLDCombatExperience = 0;

	currentPhase = GAME_PHASE_LONGDISTANCECOMBAT;
	NNBeingTested = GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN;
	totalErrorLDPropertiesExperience = THperformGenericCombatWithAllPermutationsOfUnitsInListAndAddOrCompareExperiences(currentPhase, firstUnitInUnitList, currentPlayer, NNBeingTested, COMPARE_EXPERIENCE, numberOfDistancesToTrainNetwork);

	currentPhase = GAME_PHASE_LONGDISTANCECOMBAT;
	NNBeingTested = GAME_INDEX_OF_COMBAT_EXPERIENCE_NN;
	totalErrorLDCombatExperience = THperformGenericCombatWithAllPermutationsOfUnitsInListAndAddOrCompareExperiences(currentPhase, firstUnitInUnitList, currentPlayer, NNBeingTested, COMPARE_EXPERIENCE, numberOfDistancesToTrainNetwork);

	double averageErrorLDPropertiesExperience = totalErrorLDPropertiesExperience/((pow((double)numberOfUnits, 2)* (double)numberOfExperiencesPerUnitPerScenario* (double)numberOfDistancesToTrainNetwork));

	//cout << "averageErrorPropertiesExperience = " << averageErrorPropertiesExperience << endl;
	cout << "averageAccuracyLDPropertiesExperience = " << (1.0F-averageErrorLDPropertiesExperience)*100.0F << "%" <<endl;

	double averageErrorLDCombatExperience = totalErrorLDCombatExperience/((pow((double)numberOfUnits, 2)* (double)numberOfExperiencesPerUnitPerScenario* (double)numberOfDistancesToTrainNetwork));

	//cout << "averageErrorCombatExperience = " << averageErrorCombatExperience << endl;
	cout << "averageAccuracyLDCombatExperience = " << (1.0F-averageErrorLDCombatExperience)*100.0F << "%" <<endl;

#endif

#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
	double totalErrorObjectExperience = 0;

	NNBeingTested = GAME_INDEX_OF_OBJECT_EXPERIENCE_NN;
	totalErrorObjectExperience = addOrCompareAllObjectExperiences(currentPlayer, NNBeingTested, COMPARE_EXPERIENCE, false);

	long numberOfObjectExperiences = (long)addOrCompareAllObjectExperiences(NULL, NULL, NULL, true);
	double averageErrorObjectExperience = totalErrorObjectExperience/((double)numberOfObjectExperiences);

	//cout << "averageErrorObjectExperience = " << averageErrorObjectExperience << endl;
	cout << "averageAccuracyObjectExperience = " << (1.0F-averageErrorObjectExperience)*100.0F << "%" <<endl;
#endif

	return 1;
}



	//assume units can only have 1 part of each part type (Ie - NO UNIT GROUPS!)
long THgenerateUnitListwithAllUnitProperties(UnitListClass* firstUnitInUnitList)
{
	bool result = true;
	long numberOfUnits = 0;

	UnitListClass* currentUnitInUnitList = firstUnitInUnitList;


	//count number of units in each
	XMLrulesClass* currentReferenceRulesClass;

	currentReferenceRulesClass = LRRCrulesUnitTypeCatagories;
	int numberUnitTypeCatagories = 0;
	while(currentReferenceRulesClass->next != NULL)
	{
		numberUnitTypeCatagories++;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackCloseCombat;
	int numberUnitCombatDetailsAttackCloseCombat = 0;
	while(currentReferenceRulesClass->next != NULL)
	{
		numberUnitCombatDetailsAttackCloseCombat ++;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
	int numberUnitCombatDetailsAttackLongDistance = 0;
	while(currentReferenceRulesClass->next != NULL)
	{
		numberUnitCombatDetailsAttackLongDistance ++;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceHead;
	int numberUnitCombatDetailsDefenceHead = 0;
	while(currentReferenceRulesClass->next != NULL)
	{
		numberUnitCombatDetailsDefenceHead++;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceTorso;
	int numberUnitCombatDetailsDefenceTorso = 0;
	while(currentReferenceRulesClass->next != NULL)
	{
		numberUnitCombatDetailsDefenceTorso++;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceShield;
	int numberUnitCombatDetailsDefenceShield = 0;
	while(currentReferenceRulesClass->next != NULL)
	{
		numberUnitCombatDetailsDefenceShield++;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	//for(int indexUnitTypeCatagories = 0; indexUnitTypeCatagories < numberUnitTypeCatagories; indexUnitTypeCatagories++)
	//{
	int indexUnitTypeCatagories = 1;	//1 = person		//no knights/horses used currently

		for(int indexUnitCombatDetailsAttackCloseCombat = -1; indexUnitCombatDetailsAttackCloseCombat < numberUnitCombatDetailsAttackCloseCombat; indexUnitCombatDetailsAttackCloseCombat++)
		{//-1 if no item from this category will be added to the unit
			for(int indexUnitCombatDetailsAttackLongDistance = -1; indexUnitCombatDetailsAttackLongDistance < numberUnitCombatDetailsAttackLongDistance; indexUnitCombatDetailsAttackLongDistance++)
			{//-1 if no item from this category will be added to the unit
				for(int indexUnitCombatDetailsDefenceHead = 0; indexUnitCombatDetailsDefenceHead < numberUnitCombatDetailsDefenceHead; indexUnitCombatDetailsDefenceHead++)
				{
					for(int indexUnitCombatDetailsDefenceTorso = 0; indexUnitCombatDetailsDefenceTorso < numberUnitCombatDetailsDefenceTorso; indexUnitCombatDetailsDefenceTorso++)
					{
						for(int indexUnitCombatDetailsDefenceShield = 0; indexUnitCombatDetailsDefenceShield < numberUnitCombatDetailsDefenceShield; indexUnitCombatDetailsDefenceShield++)
						{
							numberOfUnits++;

							ModelDetails* currentUnitDetails = new ModelDetails();
							if(!generateUnitDetails(currentUnitDetails, indexUnitTypeCatagories, indexUnitCombatDetailsAttackCloseCombat, indexUnitCombatDetailsAttackLongDistance, indexUnitCombatDetailsDefenceHead, indexUnitCombatDetailsDefenceTorso, indexUnitCombatDetailsDefenceShield))
							{
								result = false;
							}
							//performFinalUnitClassCalculations(currentUnitDetails);	- this is now done in LRRCcombat.cpp
							currentUnitInUnitList->unitDetails = currentUnitDetails;
							currentUnitInUnitList->number = numberOfUnits;	//record unit number - for debugging
							UnitListClass* newUnit = new UnitListClass();
							currentUnitInUnitList->next = newUnit;
							currentUnitInUnitList=currentUnitInUnitList->next;
						}
					}
				}
			}
		}
	//}

		/*
		cout << "numberOfUnits = " << numberOfUnits << endl;
		cout << "NOT USED: numberUnitTypeCatagories = " << numberUnitTypeCatagories << endl;
		cout << "numberUnitCombatDetailsAttackCloseCombat = " << numberUnitCombatDetailsAttackCloseCombat << endl;
		cout << "numberUnitCombatDetailsAttackLongDistance = " << numberUnitCombatDetailsAttackLongDistance << endl;
		cout << "numberUnitCombatDetailsDefenceHead = " << numberUnitCombatDetailsDefenceHead << endl;
		cout << "numberUnitCombatDetailsDefenceTorso = " << numberUnitCombatDetailsDefenceTorso << endl;
		cout << "numberUnitCombatDetailsDefenceShield = " << numberUnitCombatDetailsDefenceShield << endl;
		*/

	return numberOfUnits;
}

bool generateUnitDetails(ModelDetails* unitDetails, int selectionUnitTypeCatagories, int selectionUnitCombatDetailsAttackCloseCombat, int selectionUnitCombatDetailsAttackLongDistance, int selectionUnitCombatDetailsDefenceHead, int selectionUnitCombatDetailsDefenceTorso, int selectionUnitCombatDetailsDefenceShield)
{
	RecordClass* currentRecord;



	currentRecord = unitDetails->recordOfUnitTypeDetails;
	int indexUnitTypeCatagories = 0;
	while(currentRecord->next != NULL)
	{
		if(indexUnitTypeCatagories == selectionUnitTypeCatagories)
		{
			currentRecord->numberOfThisPartIDInTheUnit = 1;
		}
		indexUnitTypeCatagories++;
		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetails->recordOfUnitCombatDetailsAttackCloseCombat;
	int indexUnitCombatDetailsAttackCloseCombat = 0;
	while(currentRecord->next != NULL)
	{
		if(indexUnitCombatDetailsAttackCloseCombat == selectionUnitCombatDetailsAttackCloseCombat)
		{
			currentRecord->numberOfThisPartIDInTheUnit = 1;
		}
		indexUnitCombatDetailsAttackCloseCombat ++;
		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetails->recordOfUnitCombatDetailsAttackLongDistance;
	int indexUnitCombatDetailsAttackLongDistance = 0;
	while(currentRecord->next != NULL)
	{
		if(indexUnitCombatDetailsAttackLongDistance == selectionUnitCombatDetailsAttackLongDistance)
		{
			currentRecord->numberOfThisPartIDInTheUnit = 1;
		}
		indexUnitCombatDetailsAttackLongDistance ++;
		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetails->recordOfUnitCombatDetailsDefenceHead;
	int indexUnitCombatDetailsDefenceHead = 0;
	while(currentRecord->next != NULL)
	{
		if(indexUnitCombatDetailsDefenceHead == selectionUnitCombatDetailsDefenceHead)
		{
			currentRecord->numberOfThisPartIDInTheUnit = 1;
		}
		indexUnitCombatDetailsDefenceHead++;
		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetails->recordOfUnitCombatDetailsDefenceTorso;
	int indexUnitCombatDetailsDefenceTorso = 0;
	while(currentRecord->next != NULL)
	{
		if(indexUnitCombatDetailsDefenceTorso == selectionUnitCombatDetailsDefenceTorso)
		{
			currentRecord->numberOfThisPartIDInTheUnit = 1;
		}
		indexUnitCombatDetailsDefenceTorso++;
		currentRecord = currentRecord->next;
	}

	currentRecord = unitDetails->recordOfUnitCombatDetailsDefenceShield;
	int indexUnitCombatDetailsDefenceShield = 0;
	while(currentRecord->next != NULL)
	{
		if(indexUnitCombatDetailsDefenceShield == selectionUnitCombatDetailsDefenceShield)
		{
			currentRecord->numberOfThisPartIDInTheUnit = 1;
		}
		indexUnitCombatDetailsDefenceShield++;
		currentRecord = currentRecord->next;
	}

	return true;

}

double THperformGenericCombatWithAllPermutationsOfUnitsInListAndAddOrCompareExperiences(int currentPhase, UnitListClass* firstUnitInUnitList, Player* currentPlayer, int NNBeingTested, bool addOrCompareExperience, int numberOfDistancesToTrainNetwork)	//firstPlayerInList contains the NN
{
	//bool result = true;
	double totalError = 0.0;

	UnitListClass* currentUnitAttackerInList = firstUnitInUnitList;

	while(currentUnitAttackerInList -> next != NULL)
	{


		UnitListClass* currentUnitDefenderInList = firstUnitInUnitList;
		while(currentUnitDefenderInList -> next != NULL)
		{

			totalError = totalError + THperformGenericCombatWithTwoCombatReadyUnitsAndAddOrCompareExperience(currentPhase, currentUnitAttackerInList, currentUnitDefenderInList, currentPlayer, NNBeingTested, addOrCompareExperience, numberOfDistancesToTrainNetwork);
			currentUnitDefenderInList = currentUnitDefenderInList->next;
		}
		currentUnitAttackerInList = currentUnitAttackerInList->next;
	}

	//return result;
	return totalError;
}





double THperformGenericCombatWithTwoCombatReadyUnitsAndAddOrCompareExperience(int currentPhase, UnitListClass* unitAttacker, UnitListClass* unitDefender, Player* currentPlayer, int NNBeingTested, bool addOrCompareExperience, int numberOfDistancesToTrainNetwork)
{
	double totalError = 0.0;
	//bool result = true;

	LDreference* unitAttackerReference = new LDreference();
	LDreference* unitDefenderReference = new LDreference();
	unitAttackerReference->subModelDetails = unitAttacker->unitDetails;
	unitDefenderReference->subModelDetails = unitDefender->unitDetails;

	//cout << "unitAttacker->unitDetails->recordOfUnitTypeDetails->numberOfThisPartIDInTheUnit = " << unitAttacker->unitDetails->recordOfUnitTypeDetails->numberOfThisPartIDInTheUnit << endl;

	//cout << "unitReference->name = " << unitReference->name << endl;
	//cout << "unitReferenceOpponent->name = " << unitReferenceOpponent->name << endl;



	//by default, use ideal distances between units to train networks...
	if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
	{
					//Long Distance Consideration
					//find range of the ideal long range weapon (most powerful)
		XMLrulesClass* currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
		RecordClass* currentReferenceRecordClass = unitAttackerReference->subModelDetails->recordOfUnitCombatDetailsAttackLongDistance;
		bool foundOptimumLongDistanceWeapon = false;
		int longDistanceAttackBaseRange = 0;
		while((currentReferenceRecordClass->next != NULL) && (!foundOptimumLongDistanceWeapon))
		{
			if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
			{
				foundOptimumLongDistanceWeapon = true;
				longDistanceAttackBaseRange = currentReferenceRulesClass->attribute5;
			}

			currentReferenceRecordClass = currentReferenceRecordClass->next;
			currentReferenceRulesClass = currentReferenceRulesClass->next;
		}
								//set range of the ideal long range weapon (most powerful)
		vec LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition;
		LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.x = 0;
		LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.y = 0;
		LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.z = 0;
		vec LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition;
	#ifndef DEBUG_TH_DO_NOT_USE_ADVANCED_LEARNING_BASED_UPON_DISTANCES
		LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.x = (longDistanceAttackBaseRange-1)*LDRAW_UNITS_PER_CM;
	#else
		LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.x = 4.5*LDRAW_UNITS_PER_CM;
	#endif
		LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.y = 0;
		LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.z = 0;
		copyVectors(&(unitAttackerReference->absolutePosition), &LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition);
		copyVectors(&(unitDefenderReference->absolutePosition), &LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition);
		//cout << "longDistanceAttackBaseRange = " << longDistanceAttackBaseRange << endl;
	}
	else if(currentPhase == GAME_PHASE_CLOSECOMBAT)
	{
				//Close Combat Consideration [CHECK THIS; this needs to be done only for the units most probable for LD attack]
					//set range of close combat
		vec LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition;
		LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.x = 0;
		LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.y = 0;
		LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.z = 0;
		vec LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition;
		LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.x = (CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY-1)*LDRAW_UNITS_PER_CM;
		LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.y = 0;
		LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.z = 0;
		copyVectors(&(unitAttackerReference->absolutePosition), &LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition);
		copyVectors(&(unitDefenderReference->absolutePosition), &LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition);
	}

	//Save ideal positions
	vec savedUnitAttackerReferenceAbsolutePosition;
	vec savedUnitDefenderReferenceAbsolutePosition;
	copyVectors(&savedUnitAttackerReferenceAbsolutePosition, &(unitAttackerReference->absolutePosition));
	copyVectors(&savedUnitDefenderReferenceAbsolutePosition, &(unitDefenderReference->absolutePosition));

#ifdef TH_LRRC_GAME_USE_RANDOM_DISTANCES_TO_TRAIN_NETWORKS
	for(int i=0; i<numberOfDistancesToTrainNetwork; i++)
	{

		double randomNumberBetween0And1 = ((double(abs((short)rand())))/(ABS_OF_SHORT));
		if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
		{
			//Long Distance Consideration
			//find range of the ideal long range weapon (most powerful)
			double randomDistanceSelected = (long)(randomNumberBetween0And1*(double)TH_LRRC_GAME_RANGE_OF_RANDOM_LD_DISTANCES_TO_TRAIN_NETWORK);
						//set range of the ideal long range weapon (most powerful)
			vec LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition;
			LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.x = 0;
			LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.y = 0;
			LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.z = 0;
			vec LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition;
			LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.x = randomDistanceSelected*LDRAW_UNITS_PER_CM;
			LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.y = 0;
			LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.z = 0;
			copyVectors(&(unitAttackerReference->absolutePosition), &LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition);
			copyVectors(&(unitDefenderReference->absolutePosition), &LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition);
		}
		else if(currentPhase == GAME_PHASE_CLOSECOMBAT)
		{
		//Close Combat Consideration [CHECK THIS; this needs to be done only for the units most probable for LD attack]
			//set range of close combat
			double randomDistanceSelected = (long)(randomNumberBetween0And1*(double)TH_LRRC_GAME_RANGE_OF_RANDOM_LD_DISTANCES_TO_TRAIN_NETWORK);

			vec LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition;
			LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.x = 0;
			LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.y = 0;
			LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.z = 0;
			vec LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition;
			LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.x = randomDistanceSelected*LDRAW_UNITS_PER_CM;
			LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.y = 0;
			LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.z = 0;
			copyVectors(&(unitAttackerReference->absolutePosition), &LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition);
			copyVectors(&(unitDefenderReference->absolutePosition), &LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition);
		}
	#endif

		int combatResult;
	#ifdef TH_LRRC_GAME_USE_AVERAGED_COMBAT_OUTCOMES
		combatResult = repeatCombatAndFindMostSuccessfulContendor(currentPhase, unitAttackerReference, unitDefenderReference, TH_LRRC_GAME_NUM_AVERAGED_COMBAT_OUTCOMES, addOrCompareExperience);
	#else
		bool unit1TakesTheInitative = true;
		bool unit2TakesTheInitative = true;
		bool unit1intendsToPerformCloseCombatAttack = true;
		bool unit2intendsToPerformCloseCombatAttack = true;
		bool unit1intendsToPerformLongDistanceAttack = true;
		bool unit2intendsToPerformLongDistanceAttack = true;
		if(currentPhase == GAME_PHASE_CLOSECOMBAT)
		{
			combatResult = performCloseCombatWithReferences(unitAttackerReference, unitDefenderReference,  unit1intendsToPerformCloseCombatAttack,  unit2intendsToPerformCloseCombatAttack, unit1TakesTheInitative, unit2TakesTheInitative);
		}
		else if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
		{
			combatResult = performLongDistanceCombatWithReferences(unitAttackerReference, unitDefenderReference,  unit1intendsToPerformLongDistanceAttack, unit2intendsToPerformLongDistanceAttack);
		}
		else
		{
			//result = false;
			cout << "error: THperformGenericCombatWithTwoCombatReadyUnitsAndCompareExperienceWithNN{} illegal phase " << endl;
		}
	#endif

		int evadeDecision;
		int attackDecision;
		if(currentPhase == GAME_PHASE_CLOSECOMBAT)
		{
			attackDecision = COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_CLOSECOMBAT;
			evadeDecision = COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_CLOSECOMBAT;
		}
		else if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
		{
			attackDecision = COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_LONGDISTANCE;
			evadeDecision = COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_LONGDISTANCE;
		}
		else
		{
			//result = false;
			cout << "error: THperformGenericCombatWithTwoCombatReadyUnitsAndCompareExperienceWithNN{} illegal phase " << endl;
		}


		//cout << "a1" << endl;

		//add experience info for units

		int combatExperienceResult;
		long unitTheoreticalBestDecisionToCompare;
		if((combatResult == ATTACK_NEITHER_UNIT_CAN_STRIKE) || (combatResult == ATTACK_NEITHER_STRIKES_UNIT2_CAN_STRIKE) || (combatResult == ATTACK_NEITHER_STRIKES_UNIT1_CAN_STRIKE))
		{
			//cout << "SCENARIO A" << endl;
		}
		else if((combatResult == ATTACK_BOTH_UNITS_STRIKE_BOTH_CAN_STRIKE) || (combatResult == ATTACK_NEITHER_UNITS_STRIKE_BOTH_CAN_STRIKE))
		{
			//cout << "SCENARIO B" << endl;
		}
		else if((combatResult == ATTACK_UNIT1_STRIKES_BOTH_CAN_STRIKE) || (combatResult == ATTACK_UNIT1_STRIKES_UNIT1_CAN_STRIKE))
		{

			//cout << "SCENARIO C" << endl;

				/*
			 	cout << "ATTACK_UNIT1_STRIKES" << endl;
				cout << "unitAttacker->unitDetails->numLongBow = " << unitAttacker->unitDetails->numLongBow << endl;
				cout << "unitAttacker->unitDetails->numQuiver = " << unitAttacker->unitDetails->numQuiver << endl;
				cout << "unitAttacker->unitDetails->defenceTotal = " << unitAttacker->unitDetails->defenceTotal << endl;
				cout << "unitAttacker->unitDetails->longDistanceAttackTotal = " << unitAttacker->unitDetails->longDistanceAttackTotal << endl;
				cout << "unitAttacker->unitDetails->longDistanceAttackBaseRange = " << unitAttacker->unitDetails->longDistanceAttackBaseRange << endl;
			 	cout << "unitDefender->unitDetails->numLongBow = " << unitDefender->unitDetails->numLongBow << endl;
				cout << "unitDefender->unitDetails->numQuiver = " << unitDefender->unitDetails->numQuiver << endl;
				cout << "unitDefender->unitDetails->defenceTotal = " << unitDefender->unitDetails->defenceTotal << endl;
				cout << "unitDefender->unitDetails->longDistanceAttackTotal = " << unitDefender->unitDetails->longDistanceAttackTotal << endl;
				cout << "unitDefender->unitDetails->longDistanceAttackBaseRange = " << unitDefender->unitDetails->longDistanceAttackBaseRange << endl;
				*/

			unitTheoreticalBestDecisionToCompare = attackDecision;
			totalError = totalError + addOrCompareExperienceFromUnitDecision(currentPhase, unitAttacker, unitAttackerReference, unitDefenderReference, unitTheoreticalBestDecisionToCompare, currentPlayer, NNBeingTested, addOrCompareExperience, NULL);		//arbitrary player, OLD=attackerPlayer


			//cout << "\terror = " << addOrCompareExperienceFromUnitDecision(currentPhase, unitAttacker, unitAttackerReference, unitDefenderReference, unitTheoreticalBestDecisionToCompare, currentPlayer, NNBeingTested, addOrCompareExperience, NULL) << endl;


			unitTheoreticalBestDecisionToCompare = evadeDecision;
		#ifndef TH_LRRC_GAME_DO_NOT_CHECK_EVASION_IN_NN_ERROR
			totalError = totalError + addOrCompareExperienceFromUnitDecision(currentPhase, unitDefender, unitDefenderReference, unitAttackerReference, unitTheoreticalBestDecisionToCompare, currentPlayer, NNBeingTested, addOrCompareExperience, NULL);		//arbitrary player, OLD=attackerPlayer
		#else
			addOrCompareExperienceFromUnitDecision(currentPhase, unitDefender, unitDefenderReference, unitAttackerReference, unitTheoreticalBestDecisionToCompare, currentPlayer, NNBeingTested, addOrCompareExperience, NULL);		//arbitrary player, OLD=attackerPlayer
		#endif

		}
		else if((combatResult == ATTACK_UNIT2_STRIKES_BOTH_CAN_STRIKE) || (combatResult == ATTACK_UNIT2_STRIKES_UNIT2_CAN_STRIKE))
		{
			//cout << "SCENARIO D" << endl;

				/*
			 	cout << "ATTACK_UNIT2_STRIKES" << endl;
			 	cout << "unitDefender->unitDetails->numLongBow = " << unitDefender->unitDetails->numLongBow << endl;
				cout << "unitDefender->unitDetails->numQuiver = " << unitDefender->unitDetails->numQuiver << endl;
				cout << "unitDefender->unitDetails->defenceTotal = " << unitDefender->unitDetails->defenceTotal << endl;
				cout << "unitDefender->unitDetails->longDistanceAttackTotal = " << unitDefender->unitDetails->longDistanceAttackTotal << endl;
				cout << "unitDefender->unitDetails->longDistanceAttackBaseRange = " << unitDefender->unitDetails->longDistanceAttackBaseRange << endl;
				cout << "unitAttacker->unitDetails->numLongBow = " << unitAttacker->unitDetails->numLongBow << endl;
				cout << "unitAttacker->unitDetails->numQuiver = " << unitAttacker->unitDetails->numQuiver << endl;
				cout << "unitAttacker->unitDetails->defenceTotal = " << unitAttacker->unitDetails->defenceTotal << endl;
				cout << "unitAttacker->unitDetails->longDistanceAttackTotal = " << unitAttacker->unitDetails->longDistanceAttackTotal << endl;
				cout << "unitAttacker->unitDetails->longDistanceAttackBaseRange = " << unitAttacker->unitDetails->longDistanceAttackBaseRange << endl;
			 	*/

			unitTheoreticalBestDecisionToCompare = attackDecision;
			totalError = totalError + addOrCompareExperienceFromUnitDecision(currentPhase, unitDefender, unitDefenderReference, unitAttackerReference, unitTheoreticalBestDecisionToCompare, currentPlayer, NNBeingTested, addOrCompareExperience, NULL);		//arbitrary player, OLD=attackerPlayer

			//cout << "\terror = " << addOrCompareExperienceFromUnitDecision(currentPhase, unitDefender, unitDefenderReference, unitAttackerReference, unitTheoreticalBestDecisionToCompare, currentPlayer, NNBeingTested, addOrCompareExperience, NULL) << endl;

			unitTheoreticalBestDecisionToCompare = evadeDecision;
		#ifndef TH_LRRC_GAME_DO_NOT_CHECK_EVASION_IN_NN_ERROR
			totalError = totalError + addOrCompareExperienceFromUnitDecision(currentPhase, unitAttacker, unitAttackerReference, unitDefenderReference, unitTheoreticalBestDecisionToCompare, currentPlayer, NNBeingTested, addOrCompareExperience, NULL);		//arbitrary player, OLD=attackerPlayer
		#else
			addOrCompareExperienceFromUnitDecision(currentPhase, unitAttacker, unitAttackerReference, unitDefenderReference, unitTheoreticalBestDecisionToCompare, currentPlayer, NNBeingTested, addOrCompareExperience, NULL);		//arbitrary player, OLD=attackerPlayer
		#endif
		}
#ifdef TH_LRRC_GAME_USE_RANDOM_DISTANCES_TO_TRAIN_NETWORKS
	}
#endif

	//Restore ideal positions
	copyVectors(&(unitAttackerReference->absolutePosition), &savedUnitAttackerReferenceAbsolutePosition);
	copyVectors(&(unitDefenderReference->absolutePosition), &savedUnitDefenderReferenceAbsolutePosition);


	unitAttackerReference->subModelDetails = NULL;
	unitDefenderReference->subModelDetails = NULL;
	delete unitAttackerReference;
	delete unitDefenderReference;


	//return result;
	return totalError;
}


#ifdef TH_LRRC_GAME_USE_AVERAGED_COMBAT_OUTCOMES
int repeatCombatAndFindMostSuccessfulContendor(int currentPhase, LDreference* unitAttackerReference, LDreference* unitDefenderReference, int numberOfCombatSequences, bool addOrCompareExperience)
{
	int result;

	bool unit1TakesTheInitative = true;
	bool unit2TakesTheInitative = true;
	bool unit1intendsToPerformCloseCombatAttack = true;
	bool unit2intendsToPerformCloseCombatAttack = true;
	bool unit1intendsToPerformLongDistanceAttack = true;
	bool unit2intendsToPerformLongDistanceAttack = true;

	int* resultsForAllCombatSequences = new int[TOTAL_NUMBER_OF_COMBAT_OUTCOMES];
	int numberOfUnit1Wins = 0;
	int numberOfUnit2Wins = 0;

	for(int i = 0; i < numberOfCombatSequences; i++)
	{
		int combatResult;

		if(currentPhase == GAME_PHASE_CLOSECOMBAT)
		{
			combatResult = performCloseCombatWithReferences(unitAttackerReference, unitDefenderReference,  unit1intendsToPerformCloseCombatAttack,  unit2intendsToPerformCloseCombatAttack, unit1TakesTheInitative, unit2TakesTheInitative);
			resultsForAllCombatSequences[combatResult] = resultsForAllCombatSequences[combatResult]+1;
		}
		else if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
		{
			combatResult = performLongDistanceCombatWithReferences(unitAttackerReference, unitDefenderReference,  unit1intendsToPerformLongDistanceAttack, unit2intendsToPerformLongDistanceAttack);
			resultsForAllCombatSequences[combatResult] = resultsForAllCombatSequences[combatResult]+1;
		}
		else
		{
			//result = false;
			cout << "error: THperformGenericCombatWithTwoCombatReadyUnitsAndCompareExperienceWithNN{} illegal phase " << endl;
		}


		if((combatResult == ATTACK_NEITHER_UNIT_CAN_STRIKE) || (combatResult == ATTACK_NEITHER_STRIKES_UNIT2_CAN_STRIKE) || (combatResult == ATTACK_NEITHER_STRIKES_UNIT1_CAN_STRIKE))
		{

		}
		else if((combatResult == ATTACK_BOTH_UNITS_STRIKE_BOTH_CAN_STRIKE) || (combatResult == ATTACK_NEITHER_UNITS_STRIKE_BOTH_CAN_STRIKE))
		{

		}
		else if((combatResult == ATTACK_UNIT1_STRIKES_BOTH_CAN_STRIKE) || (combatResult == ATTACK_UNIT1_STRIKES_UNIT1_CAN_STRIKE))
		{
			//cout << "\t\th3" << endl;
			numberOfUnit1Wins++;
		}
		else if((combatResult == ATTACK_UNIT2_STRIKES_BOTH_CAN_STRIKE) || (combatResult == ATTACK_UNIT2_STRIKES_UNIT2_CAN_STRIKE))
		{
			//cout << "\t\th4" << endl;
			numberOfUnit2Wins++;
		}
	}


	double combatOutcomesGainRequired;
	if(currentPhase == GAME_PHASE_CLOSECOMBAT)
	{
		combatOutcomesGainRequired = TH_LRRC_GAME_AVERAGED_COMBAT_OUTCOMES_GAIN_REQUIRED_PERCENTAGE;	//percent

	}
	else if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
	{
		combatOutcomesGainRequired = TH_LRRC_GAME_AVERAGED_COMBAT_OUTCOMES_GAIN_REQUIRED_PERCENTAGE;	//percent
	}
	else
	{
		//result = false;
		cout << "error: THperformGenericCombatWithTwoCombatReadyUnitsAndCompareExperienceWithNN{} illegal phase " << endl;
	}



	if(numberOfUnit1Wins > (numberOfUnit2Wins + (int)((double)numberOfCombatSequences*combatOutcomesGainRequired/100.0)))		//must have > x% chance of winning
	{
		//cout << "h1" << endl;
		if(numberOfUnit2Wins > 0)
		{
			result = ATTACK_UNIT1_STRIKES_BOTH_CAN_STRIKE;
		}
		else
		{
			result = ATTACK_UNIT1_STRIKES_UNIT1_CAN_STRIKE;
		}

	}
	else if(numberOfUnit2Wins > (numberOfUnit1Wins + (int)((double)numberOfCombatSequences*combatOutcomesGainRequired/100.0)))	//must have > x% chance of winning
	{
		//cout << "h2" << endl;
		if(numberOfUnit1Wins > 0)
		{
			result = ATTACK_UNIT2_STRIKES_BOTH_CAN_STRIKE;
		}
		else
		{
			result = ATTACK_UNIT2_STRIKES_UNIT2_CAN_STRIKE;
		}
	}
	else
	{
		if((numberOfUnit1Wins > 0) && (numberOfUnit2Wins > 0))
		{
			result = ATTACK_BOTH_UNITS_STRIKE_BOTH_CAN_STRIKE;
		}
		else
		{
			result = ATTACK_NEITHER_UNIT_CAN_STRIKE;
		}
	}

	return result;
}
#endif












