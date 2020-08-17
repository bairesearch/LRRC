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
 * File Name: LRRCcombat.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3n7c 17-August-2020
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#include "LRRCcombat.hpp"

double CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY;
double RANGE_MODIFIER_BASED_ON_INCREASED_HEIGHT;



void LRRCcombatClass::fillInCombatExternVariables()
{
	XMLrulesClass* currentReferenceRulesClass = LRRCrulesMiscellaneous;

	while(currentReferenceRulesClass->next != NULL)
	{
		if(currentReferenceRulesClass->name == CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY_NAME)
		{
			CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == RANGE_MODIFIER_BASED_ON_INCREASED_HEIGHT_NAME)
		{
			RANGE_MODIFIER_BASED_ON_INCREASED_HEIGHT = currentReferenceRulesClass->fractionalValue;
		}
		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
}


























































































/*Basic: This method does not work with embedded units, and assumes the unit does not contain submodels relevant to its class*/
bool LRRCcombatClass::calculateUnitClassBasic(string unitFileName)
{
	//reinitalise ModelDetails specific values if necessary
	LDreference* initialReferenceInUnitFile = new LDreference();
	LDreference* topLevelReferenceInFile = new LDreference(true);
	ModelDetails* u = topLevelReferenceInFile->subModelDetails;

	if(!LDparser.parseFile(unitFileName, initialReferenceInUnitFile, topLevelReferenceInFile, false))
	{//file does not exist
		return false;
	}
	else
	{
		this->performFinalUnitClassCalculations(u);
		if(!this->performUnitOrdinatesCheck(u))
		{
			return false;
		}
		cout << "Defence Level = " << LRRCmodelClass.invertLevel(u->defenceTotal) << endl;
		cout << "Attack Level = " << LRRCmodelClass.invertLevel(u->attackTotal) << endl;
		return true;
	}

	delete initialReferenceInUnitFile;
	delete topLevelReferenceInFile;
}

//new
bool LRRCcombatClass::calculateUnitClassNormal(string unitFileName)
{
	//declare initial scene references
	LDreference* initialReferenceInUnit1 = new LDreference();
	LDreference* topLevelReferenceInUnit1 = new LDreference(true);

	//parse the unit files and build LDreference linked lists
	LDparser.parseFile(unitFileName, initialReferenceInUnit1, topLevelReferenceInUnit1, false);

	//initialise unit1/unit2 references
	topLevelReferenceInUnit1->colour = int(DEFAULT_COLOUR_OF_SUB_MODEL_CURRENTLY_BEING_SEARCHED_FOR);	// this should be modified appropriatly in the future when multiple references to the same ModelDetails can be included in a scene file (of different colours)
	topLevelReferenceInUnit1->name = unitFileName;

	//determing the current position of Unit 1
	this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(topLevelReferenceInUnit1, initialReferenceInUnit1, NULL, true);

			//the third parameter is not used, as the forth parameter is set true.
	this->performFinalUnitClassCalculations(topLevelReferenceInUnit1->subModelDetails);
	if(!this->performUnitOrdinatesCheck(topLevelReferenceInUnit1->subModelDetails))
	{
		return false;
	}

	cout << "Defence Level = " << LRRCmodelClass.invertLevel(topLevelReferenceInUnit1->subModelDetails->defenceTotal) << endl;
	cout << "Attack Level = " << LRRCmodelClass.invertLevel(topLevelReferenceInUnit1->subModelDetails->attackTotal) << endl;

	delete initialReferenceInUnit1;
	delete topLevelReferenceInUnit1;

	return true;
}

























































































































































/*
//preconditions: Assumes particular ModelDetails files exist and are available. Assumes ModelDetails files are fully self contained ModelDetails files - ie each ModelDetails file must contain all information including that about relevant horses etc
//
//postconditions: returns 1 if both strike, 2 if unit1 wins, 3 if unit2 wins, 4 if neither strike, 5 if neither can strike
	//							6 if unit1 wins and only Unit 1 can strike, 7 if unit1 does not strike, and only Unit 1 can strike, 8 if  3 if unit2 wins, 4 if neither strike
	//							8 if unit2 wins and only Unit 2 can strike, 9 if unit2 does not strike, and only Unit 2 can strike.
	//	0/10 if error
*/
int LRRCcombatClass::performCloseCombatBasic(string unit1FileName, string unit2FileName, const bool unit1performsCloseCombatAttack, const bool unit2performsCloseCombatAttack, const bool unit1TakesTheInitative, const bool unit2TakesTheInitative)
{
	//declare variables references
	LDreference* initialReferenceInUnit1 = new LDreference();
	LDreference* initialReferenceInUnit2 = new LDreference();
	LDreference* topLevelReferenceInUnit1 = new LDreference(true);
	LDreference* topLevelReferenceInUnit2 = new LDreference(true);

	//parse the scene files and build LDreference linked lists
	if(!LDparser.parseFile(unit1FileName, initialReferenceInUnit1, topLevelReferenceInUnit1, false))
	{//file does not exist
		cout << "The unit unit1FileName=" << unit1FileName << " does not exist in the directory" << endl;
		return false;
	}

	if(!LDparser.parseFile(unit2FileName, initialReferenceInUnit2, topLevelReferenceInUnit2, false))
	{//file does not exist
		cout << "The unit unit2FileName=" << unit2FileName << " does not exist in the directory" << endl;
		return false;
	}

	this->performFinalUnitClassCalculations(topLevelReferenceInUnit1->subModelDetails);
	this->performFinalUnitClassCalculations(topLevelReferenceInUnit2->subModelDetails);

	if(!this->performUnitOrdinatesCheck(topLevelReferenceInUnit2->subModelDetails))
	{
		return false;
	}
	if(!this->performUnitOrdinatesCheck(topLevelReferenceInUnit2->subModelDetails))
	{
		return false;
	}

	int result = this->performCloseCombat(topLevelReferenceInUnit1->subModelDetails, topLevelReferenceInUnit2->subModelDetails, unit1performsCloseCombatAttack, unit2performsCloseCombatAttack, unit1TakesTheInitative, unit2TakesTheInitative);

	delete initialReferenceInUnit1;
	delete initialReferenceInUnit2;
	delete topLevelReferenceInUnit1;
	delete topLevelReferenceInUnit2;

	return result;
}



//returns 1 if both strike, 2 if unit1 wins, 3 if unit2 wins, 4 if neither strike. Return 5 if close combat could not occur (neither could strike).
	//							6 if unit1 wins and only Unit 1 can strike, 7 if unit1 does not strike, and only Unit 1 can strike, 8 if  3 if unit2 wins, 4 if neither strike
	//							8 if unit2 wins and only Unit 2 can strike, 9 if unit2 does not strike, and only Unit 2 can strike.
	//							0/10 if error in finding required files.
int LRRCcombatClass::performCloseCombatNormal(string unit1FileName, string unit2FileName, const bool unit1performsCloseCombatAttack, const bool unit2performsCloseCombatAttack)
{
	//declare variables references
	LDreference* initialReferenceInUnit1 = new LDreference();
	LDreference* initialReferenceInUnit2 = new LDreference();
	LDreference* topLevelReferenceInUnit1 = new LDreference(true);
	LDreference* topLevelReferenceInUnit2 = new LDreference(true);

	//parse the unit files and build LDreference linked lists
	if(!LDparser.parseFile(unit1FileName, initialReferenceInUnit1, topLevelReferenceInUnit1, false))
	{//file does not exist
		cout << "The unit unit1FileName=" << unit1FileName << " does not exist in the directory" << endl;
		return false;
	}

	//parse the unit files and build LDreference linked lists
	if(!LDparser.parseFile(unit2FileName, initialReferenceInUnit2, topLevelReferenceInUnit2, false))
	{//file does not exist
		cout << "The unit unit2FileName=" << unit2FileName << " does not exist in the directory" << endl;
		return false;
	}


	//initialise unit1/unit2 references
	topLevelReferenceInUnit1->colour = int(DEFAULT_COLOUR_OF_SUB_MODEL_CURRENTLY_BEING_SEARCHED_FOR);	// this should be modified appropriatly in the future when multiple references to the same ModelDetails can be included in a scene file (of different colours)
	topLevelReferenceInUnit1->name = unit1FileName;
	topLevelReferenceInUnit2->colour = int(DEFAULT_COLOUR_OF_SUB_MODEL_CURRENTLY_BEING_SEARCHED_FOR);	// this should be modified appropriatly in the future when multiple references to the same ModelDetails can be included in a scene file (of different colours)
	topLevelReferenceInUnit2->name = unit2FileName;


	//determing the current position of Unit 1
	this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(topLevelReferenceInUnit1, initialReferenceInUnit1, NULL, true);

	this->performFinalUnitClassCalculations(topLevelReferenceInUnit1->subModelDetails);

	if(!this->performUnitOrdinatesCheck(topLevelReferenceInUnit1->subModelDetails))
	{
		return false;
	}

	//determing the current position of Unit 2
	this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(topLevelReferenceInUnit2, initialReferenceInUnit2, NULL, true);

	this->performFinalUnitClassCalculations(topLevelReferenceInUnit2->subModelDetails);
	if(!this->performUnitOrdinatesCheck(topLevelReferenceInUnit2->subModelDetails))
	{
		return false;
	}

	bool unit1TakesTheInitative = true;
	bool unit2TakesTheInitative = true;


	int result = this->performCloseCombat(topLevelReferenceInUnit1->subModelDetails, topLevelReferenceInUnit2->subModelDetails,  unit1performsCloseCombatAttack,  unit2performsCloseCombatAttack, unit1TakesTheInitative, unit2TakesTheInitative);
			//in the future, before performCloseCombat is invoked, must check in current scene file, and previous scene file whether both units could strike during close combat
			//Eg the knight charge first strike policy will prevent at some times some units from being able to strike first

	delete initialReferenceInUnit1;
	delete initialReferenceInUnit2;
	delete topLevelReferenceInUnit1;
	delete topLevelReferenceInUnit2;

	return result;
}

//preconditions: units are assumed to have moved in their previous round (ie they both take the initative)
//returns 1 if both strike, 2 if unit1 wins, 3 if unit2 wins, 4 if neither strike. Return 5 if close combat could not occur (neither could strike).
	//							6 if unit1 wins and only Unit 1 can strike, 7 if unit1 does not strike, and only Unit 1 can strike, 8 if  3 if unit2 wins, 4 if neither strike
	//							8 if unit2 wins and only Unit 2 can strike, 9 if unit2 does not strike, and only Unit 2 can strike.
	//							0/10 if error in finding required files.
int LRRCcombatClass::performCloseCombatWithSceneFile(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, const bool unit1intendsToPerformCloseCombatAttack, const bool unit2intendsToPerformCloseCombatAttack, string thisPhaseStartSceneFileName)
{

	//declare initial scene references
	LDreference* initialReferenceInThisPhaseStartScene = new LDreference();
	LDreference* topLevelReferenceInThisPhaseStartScene = new LDreference(true);	//the modelDetails information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function

	//parse the scene files and build LDreference linked lists
	if(!LDparser.parseFile(thisPhaseStartSceneFileName, initialReferenceInThisPhaseStartScene, topLevelReferenceInThisPhaseStartScene, false))
	{//file does not exist
		cout << "The file thisPhaseStartSceneFileName " << thisPhaseStartSceneFileName << " does not exist in the directory" << endl;
		return false;
	}
	//cout << "DEBUG: b1" << endl;

	int result = this->performCloseCombatWithScene(unit1FileName, unit2FileName, unit1ID, unit2ID, unit1intendsToPerformCloseCombatAttack, unit2intendsToPerformCloseCombatAttack, initialReferenceInThisPhaseStartScene);

	delete initialReferenceInThisPhaseStartScene;
	delete topLevelReferenceInThisPhaseStartScene;	//the modelDetails information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function

	return result;
}

int LRRCcombatClass::performCloseCombatWithScene(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, const bool unit1intendsToPerformCloseCombatAttack, const bool unit2intendsToPerformCloseCombatAttack, LDreference* initialReferenceInThisPhaseStartScene)
{
	//initialise unit1/unit2 references		[NB in the future an appropriate constructor should be created for the LDreference class that accepts a name, and automatically creates a subModelDetails ModelDetails*/
	LDreference* unit1ReferenceInThisPhaseStartSceneFile = new LDreference(unit1FileName, unit1ID, true);
	LDreference* unit2ReferenceInThisPhaseStartSceneFile = new LDreference(unit2FileName, unit2ID, true);

	//determing the current position of Unit 1
	if(!this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unit1ReferenceInThisPhaseStartSceneFile, initialReferenceInThisPhaseStartScene, NULL, false))
	{
		cout << "The unit unit1FileName=" << unit1FileName << " unit1ID=" << unit1ID << " does not exist in the given scene" << endl;
		//cout << "DEBUG: b3" << endl;
		return ATTACK_ERROR_FILE_IO;
	}

	//determing the current position of Unit 2
	if(!this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unit2ReferenceInThisPhaseStartSceneFile, initialReferenceInThisPhaseStartScene, NULL, false))
	{
		cout << "The unit unit2FileName=" << unit2FileName << " unit2ID=" << unit2ID << " does not exist in the given scene" << endl;
		//cout << "DEBUG: b7" << endl;
		return ATTACK_ERROR_FILE_IO;
	}

	bool unit1TakesTheInitative = true;
	bool unit2TakesTheInitative = true;

	int result;

	result = this->performCloseCombatWithReferences(unit1ReferenceInThisPhaseStartSceneFile, unit2ReferenceInThisPhaseStartSceneFile, unit1intendsToPerformCloseCombatAttack, unit2intendsToPerformCloseCombatAttack, unit1TakesTheInitative, unit2TakesTheInitative);

	delete unit1ReferenceInThisPhaseStartSceneFile;
	delete unit2ReferenceInThisPhaseStartSceneFile;

	return result;
}




//postconditions: the validity of the movement of the units(') between scene files is not checked - this should be done in the round's movement checker routine
//returns 1 if both strike, 2 if unit1 wins, 3 if unit2 wins, 4 if neither strike. Return 5 if close combat could not occur (neither could strike).
	//							6 if unit1 wins and only Unit 1 can strike, 7 if unit1 does not strike, and only Unit 1 can strike, 8 if  3 if unit2 wins, 4 if neither strike
	//							8 if unit2 wins and only Unit 2 can strike, 9 if unit2 does not strike, and only Unit 2 can strike.
	//							0/10 if error in finding required files.
int LRRCcombatClass::performCloseCombatWithConsecutiveSceneFiles(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, const bool unit1intendsToPerformCloseCombatAttack, const bool unit2intendsToPerformCloseCombatAttack, string thisPhaseStartSceneFileName, string preMovementPhaseSceneFileName)
{

	//declare initial scene references
	LDreference* initialReferenceInThisPhaseStartScene = new LDreference();
	LDreference* initialReferenceInPreMovementPhaseScene = new LDreference();
	LDreference* topLevelReferenceInThisPhaseStartScene = new LDreference(true);	//the smodelDetails information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
	LDreference* topLevelReferenceInPreMovementPhaseScene = new LDreference(true);	//the modelDetails information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function

	//parse the scene files and build LDreference linked lists
	if(!LDparser.parseFile(thisPhaseStartSceneFileName, initialReferenceInThisPhaseStartScene, topLevelReferenceInThisPhaseStartScene, false))
	{//file does not exist
		cout << "The file: " << thisPhaseStartSceneFileName << " does not exist in the directory" << endl;
		return false;
	}
	//cout << "DEBUG: b1" << endl;

	if(!LDparser.parseFile(preMovementPhaseSceneFileName, initialReferenceInPreMovementPhaseScene, topLevelReferenceInPreMovementPhaseScene, false))
	{//file does not exist
		cout << "The file: " << preMovementPhaseSceneFileName << " does not exist in the directory" << endl;
		return false;
	}
	//cout << "DEBUG: b2" << endl;


	int result = this->performCloseCombatWithConsecutiveScenes(unit1FileName, unit2FileName, unit1ID, unit2ID, unit1intendsToPerformCloseCombatAttack, unit2intendsToPerformCloseCombatAttack, initialReferenceInThisPhaseStartScene, initialReferenceInPreMovementPhaseScene);

	delete initialReferenceInThisPhaseStartScene;
	delete initialReferenceInPreMovementPhaseScene;
	delete topLevelReferenceInThisPhaseStartScene;	//the smodelDetails information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
	delete topLevelReferenceInPreMovementPhaseScene;	//the modelDetails information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function

	return result;
}



int LRRCcombatClass::performCloseCombatWithConsecutiveScenes(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, const bool unit1intendsToPerformCloseCombatAttack, const bool unit2intendsToPerformCloseCombatAttack, LDreference* initialReferenceInThisPhaseStartScene, LDreference* initialReferenceInPreMovementPhaseScene)
{
	//initialise unit1/unit2 references		[NB in the future an appropriate constructor should be created for the LDreference class that accepts a name, and automatically creates a subModelDetails ModelDetails*/

	LDreference* unit1ReferenceInThisPhaseStartSceneFile = new LDreference(unit1FileName, unit1ID, true);
	LDreference* unit2ReferenceInThisPhaseStartSceneFile = new LDreference(unit2FileName, unit2ID, true);
	LDreference* unit1ReferenceInPreMovementPhaseSceneFile = new LDreference(unit1FileName, unit1ID, true);
	LDreference* unit2ReferenceInPreMovementPhaseSceneFile = new LDreference(unit2FileName, unit2ID, true);

	//determing the current position of Unit 1
	if(!this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unit1ReferenceInThisPhaseStartSceneFile, initialReferenceInThisPhaseStartScene, NULL, false))
	{
		cout << "The unit unit1FileName=" << unit1FileName << " unit1ID=" << unit1ID << " does not exist in the given scene" << endl;
		cout << "unit1ReferenceInThisPhaseStartSceneFile->colour = " << unit1ReferenceInThisPhaseStartSceneFile->colour << endl;
		cout << "unit1ReferenceInThisPhaseStartSceneFile->name = " << unit1ReferenceInThisPhaseStartSceneFile->name << endl;
		cout << "DEBUG: b3" << endl;
		return ATTACK_ERROR_FILE_IO;
	}

	//determing the previous  position of Unit 1
	if(!this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unit1ReferenceInPreMovementPhaseSceneFile, initialReferenceInPreMovementPhaseScene, NULL, false))
	{
		cout << "The unit unit1FileName=" << unit1FileName << " unit1ID=" << unit1ID << " does not exist in the given" << endl;
		cout << "DEBUG: b5" << endl;
		return ATTACK_ERROR_FILE_IO;
	}

	//determing the current position of Unit 2
	if(!this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unit2ReferenceInThisPhaseStartSceneFile, initialReferenceInThisPhaseStartScene, NULL, false))
	{
		cout << "The unit unit2FileName=" << unit1FileName << " unit2ID=" << unit2ID << " does not exist in the given scene" << endl;
		cout << "DEBUG: b7" << endl;
		return ATTACK_ERROR_FILE_IO;
	}

	//determing the previous position of Unit 2
	if(!this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unit2ReferenceInPreMovementPhaseSceneFile, initialReferenceInPreMovementPhaseScene, NULL, false))
	{
		cout << "The unit unit2FileName=" << unit1FileName << " unit2ID=" << unit2ID << " does not exist in the given scene" << endl;
		return ATTACK_ERROR_FILE_IO;
	}

	//check whether a ModelDetails has advanced into combat, and therefore takes the initative to strike first.

	bool unit1HasMovedInPreviousRound = false;
	bool unit2HasMovedInPreviousRound = false;

		//26-3-06 'absolute' changed to 'relative' reasoning: /*"A movement turn does not only count for the individual " ...change rules to accomodate long distance attack if an absolute movement has occured but a relative has not for any given unit.*/
	double distanceMovedByUnit1DuringRound = this->calculateTheDistanceBetweenTwoUnits(unit1ReferenceInThisPhaseStartSceneFile->relativePosition.x, unit1ReferenceInPreMovementPhaseSceneFile->relativePosition.x, unit1ReferenceInThisPhaseStartSceneFile->relativePosition.y, unit1ReferenceInPreMovementPhaseSceneFile->relativePosition.y, unit1ReferenceInThisPhaseStartSceneFile->relativePosition.z, unit1ReferenceInPreMovementPhaseSceneFile->relativePosition.z);
	double distanceMovedByUnit2DuringRound = this->calculateTheDistanceBetweenTwoUnits(unit2ReferenceInThisPhaseStartSceneFile->relativePosition.x, unit2ReferenceInPreMovementPhaseSceneFile->relativePosition.x, unit2ReferenceInThisPhaseStartSceneFile->relativePosition.y, unit2ReferenceInPreMovementPhaseSceneFile->relativePosition.y, unit2ReferenceInThisPhaseStartSceneFile->relativePosition.z, unit2ReferenceInPreMovementPhaseSceneFile->relativePosition.z);

	if(int(distanceMovedByUnit1DuringRound) != 0)
	{
		unit1HasMovedInPreviousRound = true;
	}
	if(int(distanceMovedByUnit2DuringRound) != 0)
	{
		unit2HasMovedInPreviousRound = true;
	}

	bool unit1TakesTheInitative = false;
	bool unit2TakesTheInitative = false;

	if(unit1HasMovedInPreviousRound)		/*before 26-3-06: if(unit1HasMovedInPreviousRound && !unit2HasMovedInPreviousRound)*/
	{
		unit1TakesTheInitative = true;
	}
	else if(unit2HasMovedInPreviousRound)	/*before 26-3-06: else if(unit2HasMovedInPreviousRound && !unit1HasMovedInPreviousRound)*/
	{
		unit2TakesTheInitative = true;
	}

	//cout << "DEBUG: b5" << endl;

	int result;

	result = this->performCloseCombatWithReferences(unit1ReferenceInThisPhaseStartSceneFile, unit2ReferenceInThisPhaseStartSceneFile, unit1intendsToPerformCloseCombatAttack, unit2intendsToPerformCloseCombatAttack, unit1TakesTheInitative, unit2TakesTheInitative);

	delete unit1ReferenceInThisPhaseStartSceneFile;
	delete  unit2ReferenceInThisPhaseStartSceneFile;
	delete  unit1ReferenceInPreMovementPhaseSceneFile;
	delete  unit2ReferenceInPreMovementPhaseSceneFile;

	return result;
}






int LRRCcombatClass::performCloseCombatWithReferences(LDreference* unit1ReferenceInThisPhaseStartSceneFile, LDreference* unit2ReferenceInThisPhaseStartSceneFile, const bool unit1intendsToPerformCloseCombatAttack, const bool unit2intendsToPerformCloseCombatAttack, const bool unit1TakesTheInitative, const bool unit2TakesTheInitative)
{
	//cout << "DEBUG: b5" << endl;

	int result;

	bool unit1PassesOrdinatesCheck = true;
	this->performFinalUnitClassCalculations(unit1ReferenceInThisPhaseStartSceneFile->subModelDetails);
	if(!this->performUnitOrdinatesCheck(unit1ReferenceInThisPhaseStartSceneFile->subModelDetails))
	{
		unit1PassesOrdinatesCheck = false;
	}
	bool unit2PassesOrdinatesCheck = true;
	this->performFinalUnitClassCalculations(unit2ReferenceInThisPhaseStartSceneFile->subModelDetails);
	if(!this->performUnitOrdinatesCheck(unit2ReferenceInThisPhaseStartSceneFile->subModelDetails))
	{
		unit2PassesOrdinatesCheck = false;
	}

	//check whether current units are within Close Combat Range
	double distanceBetweenTheTwoUnits = this->calculateTheDistanceBetweenTwoUnits(unit1ReferenceInThisPhaseStartSceneFile->absolutePosition.x, unit2ReferenceInThisPhaseStartSceneFile->absolutePosition.x, unit1ReferenceInThisPhaseStartSceneFile->absolutePosition.y, unit2ReferenceInThisPhaseStartSceneFile->absolutePosition.y, unit1ReferenceInThisPhaseStartSceneFile->absolutePosition.z, unit2ReferenceInThisPhaseStartSceneFile->absolutePosition.z);
	if(distanceBetweenTheTwoUnits <= CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY)
	{
	#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		cout << "unit 1: " << unit1ReferenceInThisPhaseStartSceneFile->name << endl;
		cout << "unit 2: " << unit2ReferenceInThisPhaseStartSceneFile->name << endl;
	#endif
		//perform close combat
		result = this->performCloseCombat(unit1ReferenceInThisPhaseStartSceneFile->subModelDetails, unit2ReferenceInThisPhaseStartSceneFile->subModelDetails,  unit1intendsToPerformCloseCombatAttack&unit1PassesOrdinatesCheck,  unit2intendsToPerformCloseCombatAttack&unit2PassesOrdinatesCheck, unit1TakesTheInitative, unit2TakesTheInitative);
			//in the future, before performCloseCombat is invoked, must check in current scene file, and previous scene file whether both units could strike during close combat
			//Eg the knight charge first strike policy will prevent at some times some units from being able to strike first
	}
	else
	{
		/*
		cout << "\nDEBUG: Units are out of Range. Distance between units = " << distanceBetweenTheTwoUnits << endl;
		cout << "DEBUG: Maximum Distance = " << CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY << endl;
		*/

		result = ATTACK_NEITHER_UNIT_CAN_STRIKE;
	}

	return result;
}





/*
//preconditions: Assumes particular ModelDetails objects (unit1 and unit2) are fully self contained - ie they contain all information including that about horses etc
//		(even though a knights respective horse may have been found in a higher level file during initial parse)
//
//postconditions: returns 1 if both strike, 2 if unit1 wins, 3 if unit2 wins, 4 if neither strike, 5 if neither can strike
	//							6 if unit1 wins and only Unit 1 can strike, 7 if unit1 does not strike, and only Unit 1 can strike, 8 if  3 if unit2 wins, 4 if neither strike
	//							8 if unit2 wins and only Unit 2 can strike, 9 if unit2 does not strike, and only Unit 2 can strike.
	//	0/10 if error
*/
int LRRCcombatClass::performCloseCombat(ModelDetails* unit1, ModelDetails* unit2, const bool unit1performsCloseCombatAttack, const bool unit2performsCloseCombatAttack, const bool unit1TakesTheInitative, const bool unit2TakesTheInitative)
{
	int result;

	int unit1Attack;
	int unit1Defence;
	unit1Defence = unit1->defenceTotal;
	unit1Attack = this->calculateCloseCombatAttackBonus(unit1, (bool)(unit2->numHorse), unit1TakesTheInitative);

	int unit2Attack;
	int unit2Defence;
	unit2Defence = unit2->defenceTotal;
	unit2Attack = this->calculateCloseCombatAttackBonus(unit2, (bool)(unit1->numHorse), unit2TakesTheInitative);

#ifdef GAME_OUTPUT_COMBAT_ENTRY_STATISTICS
	cout << "\nUnit 1 Combat Entry:" << endl;
	cout << "Defence Level = " << LRRCmodelClass.invertLevel(unit1->defenceTotal) << endl;
	cout << "Close Combat Attack Level = " << unit1Attack << endl;

	cout << "\nUnit 2 Combat Entry:" << endl;
	cout << "Defence Level = " << LRRCmodelClass.invertLevel(unit2->defenceTotal) << endl;
	cout << "Close Combat Attack Level = " << unit2Attack << endl;
#endif


	/*ADD_IF_USING_DIFFERENT_VERSION_OF_RULES_WHERE_CLOSE_COMBAT_FINISHES_IN_ONE_ROUND
	bool stillFighting = true;
	while(stillFighting)
	{
	*/

	bool unit1Strikes = false;
	bool unit2Strikes = false;


	//allow both units to strike equally
	if(unit1performsCloseCombatAttack)
	{

		int diceThrown1 = int(((double(abs((short)rand())))/(ABS_OF_SHORT))*((double)unit1Attack)*DICE_MODIFIER+1.0);	//+1 for 0.0001->11.9999 ... 1.0001->12.9999 ... 1->12 (round down)
		int diceThrown2 = int(((double(abs((short)rand())))/(ABS_OF_SHORT))*((double)unit2Defence)*DICE_MODIFIER+1.0);

		#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		cout << "unit 1 throws: " << diceThrown1 << " in attack using d" << (unit1Attack)*DICE_MODIFIER <<endl;
		cout << "unit 2 throws: " << diceThrown2 << " in defence using d" << (unit2Defence)*DICE_MODIFIER << endl;
		#endif

		if(diceThrown1 > diceThrown2)
		{
			unit1Strikes = true;
		}
	}

	if(unit2performsCloseCombatAttack)
	{
		int diceThrown3 = int(((double(abs((short)rand())))/(ABS_OF_SHORT))*((double)unit2Attack)*DICE_MODIFIER+1.0);
		int diceThrown4 = int(((double(abs((short)rand())))/(ABS_OF_SHORT))*((double)unit1Defence)*DICE_MODIFIER+1.0);

		#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		cout << "unit 2 throws: " << diceThrown3 << " in attack using d" << (unit2Attack)*DICE_MODIFIER << endl;
		cout << "unit 1 throws: " << diceThrown4 << " in defence using d" << (unit1Defence)*DICE_MODIFIER << endl;
		#endif

		if(diceThrown3 > diceThrown4)
		{
			unit2Strikes = true;
		}
	}


	#ifdef DEBUG_DO_NOT_USE_RANDOM_DICE_DURING_COMBAT
	if(unit1performsCloseCombatAttack)
	{
		if((unit1Attack + unit1Defence) > (unit2Attack + unit2Defence))
		{
			unit1Strikes = true;
		}
		else
		{
			unit1Strikes = false;
		}
	}

	if(unit2performsCloseCombatAttack)
	{

		if((unit2Attack + unit2Defence) > (unit1Attack + unit1Defence))
		{
			unit2Strikes = true;
		}
		else
		{
			unit2Strikes = false;
		}
	}
	#endif



	if(unit1Strikes && unit2Strikes)
	{
		//nothing happens, no blows were fatal
		#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		cout << "Both units strike. No strikes were fatal. [No units should be removed from scene file]." << endl;
		#endif
		result = ATTACK_BOTH_UNITS_STRIKE_BOTH_CAN_STRIKE;
	}
	else if(unit1Strikes && !unit2Strikes)
	{
		#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		cout << "Unit 1 wins. [Unit 2 should be removed from the scene file]." << endl;
		#endif
		if(unit2performsCloseCombatAttack)
		{
			result = ATTACK_UNIT1_STRIKES_BOTH_CAN_STRIKE;
		}
		else
		{
			result = ATTACK_UNIT1_STRIKES_UNIT1_CAN_STRIKE;
		}
	}
	else if(!unit1Strikes && unit2Strikes)
	{
		#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		cout << "Unit 2 wins. [Unit 1 should be removed from the scene file]." << endl;
		#endif
		if(unit1performsCloseCombatAttack)
		{
			result = ATTACK_UNIT2_STRIKES_BOTH_CAN_STRIKE;
		}
		else
		{
			result = ATTACK_UNIT2_STRIKES_UNIT2_CAN_STRIKE;
		}

	}
	else if(!unit1Strikes && !unit2Strikes)
	{
		#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		cout << "The unit(s) intending to strike did not. [No units should be removed from scene file]." << endl;
		#endif

		if(unit1performsCloseCombatAttack && unit2performsCloseCombatAttack)
		{
			result = ATTACK_NEITHER_UNITS_STRIKE_BOTH_CAN_STRIKE;
		}
		else if(unit1performsCloseCombatAttack && !unit2performsCloseCombatAttack)
		{
			result = ATTACK_NEITHER_STRIKES_UNIT1_CAN_STRIKE;
		}
		else if(!unit1performsCloseCombatAttack && unit2performsCloseCombatAttack)
		{
			result = ATTACK_NEITHER_STRIKES_UNIT2_CAN_STRIKE;
		}
		else if(!unit1performsCloseCombatAttack && !unit2performsCloseCombatAttack)
		{
			result = ATTACK_NEITHER_UNIT_CAN_STRIKE;
		}
		else
		{
			cout << "error in computer" << endl;
			result = ATTACK_ERROR_FILE_IO;
		}
	}
	else
	{
		cout << "error in computer" << endl;
		result = ATTACK_ERROR_FILE_IO;
	}

	return result;

	/*ADD_IF_USING_DIFFERENT_VERSION_OF_RULES_WHERE_CLOSE_COMBAT_FINISHES_IN_ONE_ROUND
	}
	*/
}






















































//preconditions: Assumes particular ModelDetails files exist and are available. Assumes ModelDetails files are fully self contained ModelDetails files - ie each ModelDetails file must contain all information including that about relevant horses etc
//
//postconditions: returns 1 if both strike, 2 if unit1 wins, 3 if unit2 wins, 4 if neither strike, 5 if neither can strike
	//							6 if unit1 wins and only Unit 1 can strike, 7 if unit1 does not strike, and only Unit 1 can strike, 8 if  3 if unit2 wins, 4 if neither strike
	//							8 if unit2 wins and only Unit 2 can strike, 9 if unit2 does not strike, and only Unit 2 can strike.
	//	10 if error

int LRRCcombatClass::performLongDistanceCombatBasic(string unit1FileName, string unit2FileName, bool unit1performsLongDistanceAttack, bool unit2performsLongDistanceAttack)
{

	//declare variables references
	LDreference* initialReferenceInUnit1 = new LDreference();
	LDreference* initialReferenceInUnit2 = new LDreference();
	LDreference* topLevelReferenceInUnit1 = new LDreference(true);
	LDreference* topLevelReferenceInUnit2 = new LDreference(true);

	//parse the scene files and build LDreference linked lists
	if(!LDparser.parseFile(unit1FileName, initialReferenceInUnit1, topLevelReferenceInUnit1, false))
	{//file does not exist
		cout << "The unit unit1FileName=" << unit1FileName << " does not exist in the directory" << endl;
		return false;
	}
	if(!LDparser.parseFile(unit2FileName, initialReferenceInUnit2, topLevelReferenceInUnit2, false))
	{//file does not exist
		cout << "The unit unit2FileName=" << unit2FileName << " does not exist in the directory" << endl;
		return false;
	}

	this->performFinalUnitClassCalculations(topLevelReferenceInUnit1->subModelDetails);
	this->performFinalUnitClassCalculations(topLevelReferenceInUnit2->subModelDetails);
	if(!this->performUnitOrdinatesCheck(topLevelReferenceInUnit1->subModelDetails))
	{
		return false;
	}
	if(!this->performUnitOrdinatesCheck(topLevelReferenceInUnit2->subModelDetails))
	{
		return false;
	}
	int result = this->performLongDistanceCombat(topLevelReferenceInUnit1->subModelDetails, topLevelReferenceInUnit2->subModelDetails, unit1performsLongDistanceAttack, unit2performsLongDistanceAttack);

	delete initialReferenceInUnit1;
	delete initialReferenceInUnit2;
	delete topLevelReferenceInUnit1;
	delete topLevelReferenceInUnit2;

	return result;
}


int LRRCcombatClass::performLongDistanceCombatNormal(string unit1FileName, string unit2FileName, bool unit1performsLongDistanceAttack, bool unit2performsLongDistanceAttack)
{
	//declare initial scene references
	LDreference* initialReferenceInUnit1 = new LDreference();
	LDreference* initialReferenceInUnit2 = new LDreference();
	LDreference* topLevelReferenceInUnit1 = new LDreference(true);
	LDreference* topLevelReferenceInUnit2 = new LDreference(true);

	//parse the unit files and build LDreference linked lists
	if(!LDparser.parseFile(unit1FileName, initialReferenceInUnit1, topLevelReferenceInUnit1, false))
	{//file does not exist
		cout << "The unit unit1FileName=" << unit1FileName << " does not exist in the directory" << endl;
		return false;
	}

	//parse the unit files and build LDreference linked lists
	if(!LDparser.parseFile(unit2FileName, initialReferenceInUnit2, topLevelReferenceInUnit2, false))
	{//file does not exist
		cout << "The unit unit2FileName=" << unit2FileName << " does not exist in the directory" << endl;
		return false;
	}

	//initialise unit1/unit2 references
	topLevelReferenceInUnit1->colour = int(DEFAULT_COLOUR_OF_SUB_MODEL_CURRENTLY_BEING_SEARCHED_FOR);	// this should be modified appropriatly in the future when multiple references to the same ModelDetails can be included in a scene file (of different colours)
	topLevelReferenceInUnit1->name = unit1FileName;
	topLevelReferenceInUnit2->colour = int(DEFAULT_COLOUR_OF_SUB_MODEL_CURRENTLY_BEING_SEARCHED_FOR);	// this should be modified appropriatly in the future when multiple references to the same ModelDetails can be included in a scene file (of different colours)
	topLevelReferenceInUnit2->name = unit2FileName;


	//determing the current position of Unit 1
	this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(topLevelReferenceInUnit1, initialReferenceInUnit1, NULL, true);

	this->performFinalUnitClassCalculations(topLevelReferenceInUnit1->subModelDetails);
	if(!this->performUnitOrdinatesCheck(topLevelReferenceInUnit1->subModelDetails))
	{
		return false;
	}

	//determing the current position of Unit 2
	this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(topLevelReferenceInUnit2, initialReferenceInUnit2, NULL, true);

	this->performFinalUnitClassCalculations(topLevelReferenceInUnit2->subModelDetails);
	if(!this->performUnitOrdinatesCheck(topLevelReferenceInUnit2->subModelDetails))
	{
		return false;
	}

	//perform long distance combat
	int result = this->performLongDistanceCombat(topLevelReferenceInUnit1->subModelDetails, topLevelReferenceInUnit2->subModelDetails, unit1performsLongDistanceAttack,  unit2performsLongDistanceAttack);

	delete initialReferenceInUnit1;
	delete initialReferenceInUnit2;
	delete topLevelReferenceInUnit1;
	delete topLevelReferenceInUnit2;

	return result;
}



//preconditions: units are assumed not to have moved in their previous round (ie they both are allowed to perform long distance combat in their current turn)
int LRRCcombatClass::performLongDistanceCombatWithSceneFile(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, bool unit1intendsToPerformLongDistanceAttack, bool unit2intendsToPerformLongDistanceAttack, string thisPhaseStartSceneFileName)
{
	//declare initial scene references
	LDreference* initialReferenceInThisPhaseStartScene = new LDreference();
	LDreference* topLevelReferenceInThisPhaseStartScene = new LDreference(true);			//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function

	//parse the scene files and build LDreference linked lists
	if(!LDparser.parseFile(thisPhaseStartSceneFileName, initialReferenceInThisPhaseStartScene, topLevelReferenceInThisPhaseStartScene, false))
	{//file does not exist
		cout << "The unit unit1FileName=" << unit1FileName << " unit1ID=" << unit1ID << " does not exist in the directory" << endl;
		return false;
	}

	int result = this->performLongDistanceCombatWithScene(unit1FileName, unit2FileName, unit1ID, unit2ID, unit1intendsToPerformLongDistanceAttack, unit2intendsToPerformLongDistanceAttack, initialReferenceInThisPhaseStartScene);

	delete initialReferenceInThisPhaseStartScene;
	delete topLevelReferenceInThisPhaseStartScene;			//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function

	return result;
}




int LRRCcombatClass::performLongDistanceCombatWithScene(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, bool unit1intendsToPerformLongDistanceAttack, bool unit2intendsToPerformLongDistanceAttack, LDreference* initialReferenceInThisPhaseStartScene)
{
	int result;

	//initialise unit1/unit2 references
	LDreference* unit1ReferenceInThisPhaseStartSceneFile = new LDreference(unit1FileName, unit1ID, true);
	LDreference* unit2ReferenceInThisPhaseStartSceneFile = new LDreference(unit2FileName, unit2ID, true);

	//determing the current position of Unit 1
	if(!this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unit1ReferenceInThisPhaseStartSceneFile, initialReferenceInThisPhaseStartScene, NULL, false))
	{
		cout << "The unit unit1FileName=" << unit1FileName << " unit1ID=" << unit1ID << " does not exist in the given scene" << endl;
		return ATTACK_ERROR_FILE_IO;
	}

	//determing the current position of Unit 2
	if(!this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unit2ReferenceInThisPhaseStartSceneFile, initialReferenceInThisPhaseStartScene, NULL, false))
	{
		cout << "The unit unit2FileName=" << unit2FileName << " unit2ID=" << unit2ID << " does not exist in the given scene" << endl;
		return ATTACK_ERROR_FILE_IO;
	}

	this->performLongDistanceCombatWithReferences(unit1ReferenceInThisPhaseStartSceneFile, unit2ReferenceInThisPhaseStartSceneFile, unit1intendsToPerformLongDistanceAttack, unit2intendsToPerformLongDistanceAttack);

	delete unit1ReferenceInThisPhaseStartSceneFile;
	delete unit2ReferenceInThisPhaseStartSceneFile;

	return result;
}





//postconditions: the validity of the movement of the units(') between scene files is not checked - this should be done in the round's movement checker routine
int LRRCcombatClass::performLongDistanceCombatWithConsecutiveSceneFiles(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, const bool unit1intendsToPerformLongDistanceAttack, const bool unit2intendsToPerformLongDistanceAttack, string thisPhaseStartSceneFileName, string preMovementPhaseSceneFileName)
{
	//declare initial scene references
	LDreference* initialReferenceInThisPhaseStartScene = new LDreference();
	LDreference* initialReferenceInPreMovementPhaseScene = new LDreference();
	LDreference* topLevelReferenceInThisPhaseStartScene = new LDreference(true);
	LDreference* topLevelReferenceInPreMovementPhaseScene = new LDreference(true);

	//parse the scene files and build LDreference linked lists
	if(!LDparser.parseFile(thisPhaseStartSceneFileName, initialReferenceInThisPhaseStartScene, topLevelReferenceInThisPhaseStartScene, false))
	{//file does not exist
		cout << "The unit unit1FileName=" << unit1FileName << " unit1ID=" << unit1ID << " does not exist in the directory" << endl;
		return false;
	}

	if(!LDparser.parseFile(preMovementPhaseSceneFileName, initialReferenceInPreMovementPhaseScene, topLevelReferenceInPreMovementPhaseScene, false))
	{//file does not exist
		cout << "The unit unit2FileName=" << unit2FileName << " unit2ID=" << unit2ID << " does not exist in the directory" << endl;
		return false;
	}

	int result = this->performLongDistanceCombatWithConsecutiveScenes(unit1FileName, unit2FileName, unit1ID, unit2ID, unit1intendsToPerformLongDistanceAttack, unit2intendsToPerformLongDistanceAttack, initialReferenceInThisPhaseStartScene, initialReferenceInPreMovementPhaseScene);

	delete initialReferenceInThisPhaseStartScene;
	delete initialReferenceInPreMovementPhaseScene;
	delete topLevelReferenceInThisPhaseStartScene;
	delete topLevelReferenceInPreMovementPhaseScene;

	return result;
}


int LRRCcombatClass::performLongDistanceCombatWithConsecutiveScenes(const string unit1FileName, const string unit2FileName, const int unit1ID, const int unit2ID, const bool unit1intendsToPerformLongDistanceAttack, const bool unit2intendsToPerformLongDistanceAttack, LDreference* initialReferenceInThisPhaseStartScene, LDreference* initialReferenceInPreMovementPhaseScene)
{
	//initialise unit1/unit2 references
	LDreference* unit1ReferenceInThisPhaseStartSceneFile = new LDreference(unit1FileName, unit1ID, true);
	LDreference* unit2ReferenceInThisPhaseStartSceneFile = new LDreference(unit2FileName, unit2ID, true);
	LDreference* unit1ReferenceInPreMovementPhaseSceneFile = new LDreference(unit1FileName, unit1ID, true);
	LDreference* unit2ReferenceInPreMovementPhaseSceneFile = new LDreference(unit2FileName, unit2ID, true);

	//determing the current position of Unit 1
	if(!this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unit1ReferenceInThisPhaseStartSceneFile, initialReferenceInThisPhaseStartScene, NULL, false))
	{
		cout << "The unit unit1FileName=" << unit1FileName << " unit1ID=" << unit1ID << " does not exist in the given scene" << endl;
		return ATTACK_ERROR_FILE_IO;
	}

	//determing the previous  position of Unit 1
	if(!this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unit1ReferenceInPreMovementPhaseSceneFile, initialReferenceInPreMovementPhaseScene, NULL, false))
	{
		cout << "The unit unit1FileName=" << unit1FileName << " unit1ID=" << unit1ID << " does not exist in the given scene" << endl;
		return ATTACK_ERROR_FILE_IO;
	}

	//determing the current position of Unit 2
	if(!this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unit2ReferenceInThisPhaseStartSceneFile, initialReferenceInThisPhaseStartScene, NULL, false))
	{
		cout << "The unit unit2FileName=" << unit2FileName << " unit2ID=" << unit2ID << " does not exist in the given scene" << endl;
		return ATTACK_ERROR_FILE_IO;
	}

	//determing the previous position of Unit 2
	if(!this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unit2ReferenceInPreMovementPhaseSceneFile, initialReferenceInPreMovementPhaseScene, NULL, false))
	{
		cout << "The unit unit2FileName=" << unit2FileName << " unit2ID=" << unit2ID << " does not exist in the given scene" << endl;
		return ATTACK_ERROR_FILE_IO;
	}

	//check whether chosen ModelDetails(s) can perform long distance attack during their turn based on whether or not they have moved during the round,
	/******************************************************/

	bool unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack = false;
	bool unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack = false;

		//26-3-06 'absolute' changed to 'relative' reasoning: /*"A movement turn does not only count for the individual " ...change rules to accomodate long distance attack if an absolute movement has occured but a relative has not for any given unit.*/
	double distanceMovedByUnit1DuringRound = this->calculateTheDistanceBetweenTwoUnits(unit1ReferenceInThisPhaseStartSceneFile->relativePosition.x, unit1ReferenceInPreMovementPhaseSceneFile->relativePosition.x, unit1ReferenceInThisPhaseStartSceneFile->relativePosition.y, unit1ReferenceInPreMovementPhaseSceneFile->relativePosition.y, unit1ReferenceInThisPhaseStartSceneFile->relativePosition.z, unit1ReferenceInPreMovementPhaseSceneFile->relativePosition.z);
	double distanceMovedByUnit2DuringRound = this->calculateTheDistanceBetweenTwoUnits(unit2ReferenceInThisPhaseStartSceneFile->relativePosition.x, unit2ReferenceInPreMovementPhaseSceneFile->relativePosition.x, unit2ReferenceInThisPhaseStartSceneFile->relativePosition.y, unit2ReferenceInPreMovementPhaseSceneFile->relativePosition.y, unit2ReferenceInThisPhaseStartSceneFile->relativePosition.z, unit2ReferenceInPreMovementPhaseSceneFile->relativePosition.z);


	if((int(distanceMovedByUnit1DuringRound) == 0) && unit1intendsToPerformLongDistanceAttack)
	{
		unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack = true;
	}
	else if(unit1intendsToPerformLongDistanceAttack)
	{
		cout << "unit 1 cannot perform long distance attack as it has moved in the current round" << endl;
	}

	if((int(distanceMovedByUnit2DuringRound) == 0) && unit2intendsToPerformLongDistanceAttack)
	{
		unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack = true;
	}
	else if(unit2intendsToPerformLongDistanceAttack)
	{
		cout << "unit 2 cannot perform long distance attack as it has moved in the current round" << endl;
	}

	int result = this->performLongDistanceCombatWithReferences(unit1ReferenceInThisPhaseStartSceneFile, unit2ReferenceInThisPhaseStartSceneFile, unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack);

	delete unit1ReferenceInThisPhaseStartSceneFile;
	delete unit2ReferenceInThisPhaseStartSceneFile;
	delete unit1ReferenceInPreMovementPhaseSceneFile;
	delete unit2ReferenceInPreMovementPhaseSceneFile;

	return result;
}



int LRRCcombatClass::performLongDistanceCombatWithReferences(LDreference* unit1ReferenceInThisPhaseStartSceneFile, LDreference* unit2ReferenceInThisPhaseStartSceneFile, bool unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, bool unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack)
{
	int result;

	bool unit1PassesOrdinatesCheck = true;
	this->performFinalUnitClassCalculations(unit1ReferenceInThisPhaseStartSceneFile->subModelDetails);
	if(!this->performUnitOrdinatesCheck(unit1ReferenceInThisPhaseStartSceneFile->subModelDetails))
	{
		unit1PassesOrdinatesCheck = false;
	}

	bool unit2PassesOrdinatesCheck = true;
	this->performFinalUnitClassCalculations(unit2ReferenceInThisPhaseStartSceneFile->subModelDetails);
	if(!this->performUnitOrdinatesCheck(unit2ReferenceInThisPhaseStartSceneFile->subModelDetails))
	{
		unit2PassesOrdinatesCheck = false;
	}

	//calculate whether a ModelDetails can perform long distance attack based on their range and the positions between units:
	bool unit1CanPerformLongDistanceAttack = false;
	bool unit2CanPerformLongDistanceAttack = false;
	double longRangeDistanceBetweenTheTwoUnits = this->calculateTheDistanceBetweenTwoUnits(unit1ReferenceInThisPhaseStartSceneFile->absolutePosition.x, unit2ReferenceInThisPhaseStartSceneFile->absolutePosition.x, unit1ReferenceInThisPhaseStartSceneFile->absolutePosition.y, unit2ReferenceInThisPhaseStartSceneFile->absolutePosition.y, unit1ReferenceInThisPhaseStartSceneFile->absolutePosition.z, unit2ReferenceInThisPhaseStartSceneFile->absolutePosition.z);
	this->calculateLongDistanceAttackBonus(unit1ReferenceInThisPhaseStartSceneFile, unit2ReferenceInThisPhaseStartSceneFile, unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, &unit1CanPerformLongDistanceAttack, &unit2CanPerformLongDistanceAttack, longRangeDistanceBetweenTheTwoUnits);

	if(!unit1CanPerformLongDistanceAttack && !unit2CanPerformLongDistanceAttack)
	{
		//neither ModelDetails is within range of each other or have moved in their previous round
		result = ATTACK_NEITHER_UNIT_CAN_STRIKE;
	}
	else
	{
	#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		cout << "unit 1: " << unit1ReferenceInThisPhaseStartSceneFile->name << endl;
		cout << "unit 2: " << unit2ReferenceInThisPhaseStartSceneFile->name << endl;
	#endif
		//perform long distance combat
		result = this->performLongDistanceCombat(unit1ReferenceInThisPhaseStartSceneFile->subModelDetails, unit2ReferenceInThisPhaseStartSceneFile->subModelDetails, unit1CanPerformLongDistanceAttack&unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack&unit1PassesOrdinatesCheck,  unit2CanPerformLongDistanceAttack&unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack&unit2PassesOrdinatesCheck);
	}

	return result;
}





//preconditions: Assumes particular ModelDetails objects (unit1 and unit2) are fully self contained - ie they contain all information including that about horses etc
//		(even though a knights respective horse may have been found in a higher level file during initial parse)
//
//postconditions: returns 1 if both strike, 2 if unit1 wins, 3 if unit2 wins, 4 if neither strike, 5 if neither can strike
	//							6 if unit1 wins and only Unit 1 can strike, 7 if unit1 does not strike, and only Unit 1 can strike, 8 if  3 if unit2 wins, 4 if neither strike
	//							8 if unit2 wins and only Unit 2 can strike, 9 if unit2 does not strike, and only Unit 2 can strike.
	//	10 if error
int LRRCcombatClass::performLongDistanceCombat(const ModelDetails* unit1, const ModelDetails* unit2, bool unit1performsLongDistanceAttack, bool unit2performsLongDistanceAttack)
{
	//cout << "\n\nDEBUG: unit1FileName = " << unit1FileName << endl;
	//cout << "\n\nDEBUG: unit2FileName = " << unit2FileName << endl;
	int result;

	int unit1Attack = unit1->longDistanceAttackValue;
	int unit1Defence = unit1->defenceTotal;
	int unit2Attack = unit2->longDistanceAttackValue;
	int unit2Defence = unit2->defenceTotal;

#ifdef GAME_OUTPUT_COMBAT_ENTRY_STATISTICS
	cout << "\nUnit 1 Statistics:\n" << endl;
	cout << "Defence Level = " << LRRCmodelClass.invertLevel(unit1->defenceTotal) << endl;
	cout << "Long Distance Attack Level = " << LRRCmodelClass.invertLevel(unit1->longDistanceAttackValue) << endl;
	cout << "\nUnit 2 Statistics:\n" << endl;
	cout << "Defence Level = " << LRRCmodelClass.invertLevel(unit2->defenceTotal) << endl;
	cout << "Long Distance Attack Level = " << LRRCmodelClass.invertLevel(unit2->longDistanceAttackValue) << endl;
#endif

	bool unit1Strikes = false;
	bool unit2Strikes = false;

	//added 18-3-06 check if units have long distance capable weapons

	if(unit1performsLongDistanceAttack)
	{
		if(!(unit1->hasLongDistanceCombatWeapon))
		{
			//#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
			cout << "Error, unit 1 does not have a long distance combat capable weapon" << endl;
			//#endif
			//result = ATTACK_NEITHER_UNIT_CAN_STRIKE;
			unit1performsLongDistanceAttack = false;
		}
		else
		{
			int diceThrown1 = int(((double(abs((short)rand())))/(ABS_OF_SHORT))*((double)unit1Attack)*DICE_MODIFIER+1.0);	 //+1 for 0.0001->11.9999 ... 1.0001->12.9999 ... 1->12 (round down)
			int diceThrown2 = int(((double(abs((short)rand())))/(ABS_OF_SHORT))*((double)unit2Defence)*DICE_MODIFIER+1.0);

			#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
			cout << "unit 1 throws: " << diceThrown1 << " in attack using d" << (unit1Attack)*DICE_MODIFIER <<endl;
			cout << "unit 2 throws: " << diceThrown2 << " in defence using d" << (unit2Defence)*DICE_MODIFIER << endl;
			#endif

			if(diceThrown1 > diceThrown2)
			{
				unit1Strikes = true;
			}
		}
	}

	//added 18-3-06 check if units have long distance capable weapons

	if(unit2performsLongDistanceAttack)
	{
		if(!(unit2->hasLongDistanceCombatWeapon))
		{
			//#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
			cout << "Error, unit 2 does not have a long distance combat capable weapon" << endl;
			//#endif
			//result = ATTACK_NEITHER_UNIT_CAN_STRIKE;
			unit2performsLongDistanceAttack = false;
		}
		else
		{
			int diceThrown3 = int(((double(abs((short)rand())))/(ABS_OF_SHORT))*((double)unit2Attack)*DICE_MODIFIER+1.0);
			int diceThrown4 = int(((double(abs((short)rand())))/(ABS_OF_SHORT))*((double)unit1Defence)*DICE_MODIFIER+1.0);

			#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
			cout << "unit 2 throws: " << diceThrown3 << " in attack using d" << (unit2Attack)*DICE_MODIFIER <<endl;
			cout << "unit 1 throws: " << diceThrown4 << " in defence using d" << (unit1Defence)*DICE_MODIFIER << endl;
			#endif

			if(diceThrown3 > diceThrown4)
			{
				unit2Strikes = true;
			}
		}
	}

	#ifdef DEBUG_DO_NOT_USE_RANDOM_DICE_DURING_COMBAT
	if(unit1performsLongDistanceAttack)
	{
		if((unit1Attack + unit1Defence) > (unit2Attack + unit2Defence))
		{
			unit1Strikes = true;
		}
		else
		{
			unit1Strikes = false;
		}
	}

	if(unit2performsLongDistanceAttack)
	{

		if((unit2Attack + unit2Defence) > (unit1Attack + unit1Defence))
		{
			unit2Strikes = true;
		}
		else
		{
			unit2Strikes = false;
		}
	}
	#endif



	if(unit1Strikes && unit2Strikes)
	{
		//nothing happens, no blows were fatal
		#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		cout << "Both units strike. No strikes were fatal. [No units should be removed from scene file]." << endl;
		#endif
		result = ATTACK_BOTH_UNITS_STRIKE_BOTH_CAN_STRIKE;
	}
	else if(unit1Strikes && !unit2Strikes)
	{
		#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		cout << "Unit 1 wins. [Unit 2 should be removed from the scene file]." << endl;
		#endif
		if(unit2performsLongDistanceAttack)
		{
			result = ATTACK_UNIT1_STRIKES_BOTH_CAN_STRIKE;
		}
		else
		{
			result = ATTACK_UNIT1_STRIKES_UNIT1_CAN_STRIKE;
		}
	}
	else if(!unit1Strikes && unit2Strikes)
	{
		#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		cout << "Unit 2 wins. [Unit 1 should be removed from the scene file]." << endl;
		#endif
		if(unit1performsLongDistanceAttack)
		{
			result = ATTACK_UNIT2_STRIKES_BOTH_CAN_STRIKE;
		}
		else
		{
			result = ATTACK_UNIT2_STRIKES_UNIT2_CAN_STRIKE;
		}

	}
	else if(!unit1Strikes && !unit2Strikes)
	{
		#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		cout << "The unit(s) intending to strike did not. [No units should be removed from scene file]." << endl;
		#endif
		if(unit1performsLongDistanceAttack && unit2performsLongDistanceAttack)
		{
			result = ATTACK_NEITHER_UNITS_STRIKE_BOTH_CAN_STRIKE;
		}
		else if(unit1performsLongDistanceAttack && !unit2performsLongDistanceAttack)
		{
			result = ATTACK_NEITHER_STRIKES_UNIT1_CAN_STRIKE;
		}
		else if(!unit1performsLongDistanceAttack && unit2performsLongDistanceAttack)
		{
			result = ATTACK_NEITHER_STRIKES_UNIT2_CAN_STRIKE;
		}
		else if(!unit1performsLongDistanceAttack && !unit2performsLongDistanceAttack)
		{
			result = ATTACK_NEITHER_UNIT_CAN_STRIKE;
		}
		else
		{
			cout << "error in computer" << endl;
			result = ATTACK_ERROR_FILE_IO;
		}
	}
	else
	{
		cout << "error in computer" << endl;
		result = ATTACK_ERROR_FILE_IO;
	}

	return result;
}











































//combat unit properties post parser (reference list searching)

/****************************************************************
/
/ searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel
/
/****************************************************************/

bool LRRCcombatClass::searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(LDreference* referenceBeingSearchedFor, LDreference* initialReference, const LDreference* parentReference, const bool isFillingParentUnitWithAllCombatRelevantChildModelDetails)
{
	bool particularSubModelFoundDuringSearch = false;
	LDreference* currentReference = initialReference;

	//cout << "DEBUG: c1" << endl;

	while(currentReference->next != NULL)
	{

		//cout << "\nDEBUG: c2" << endl;
		//cout << "currentReference->name = " << currentReference->name << endl;
		//cout << "referenceBeingSearchedFor->name = " << referenceBeingSearchedFor->name << endl;

		if(currentReference->isSubModelReference)
		{
			bool currentReferenceFoundIsTheParticularSubmodelBeingLookedFor = false;



			if(isFillingParentUnitWithAllCombatRelevantChildModelDetails)
			{
				LRRCmodelClass.addAllCombatRelevantChildModelDetailsIntoAParentUnit(currentReference->subModelDetails, referenceBeingSearchedFor->subModelDetails);

				/*
				//debug:

				RecordClass* currentReferenceRecordClass = currentReference->subModelDetails->recordOfUnitTypeDetails;
				while(currentReferenceRecordClass->next != NULL)
				{
					if(currentReferenceRecordClass->name == PERSON_HEAD_NAME)
					{
						cout << "currentReference->name = " << currentReference->name << endl;
						cout << "\t PERSON_HEAD_NAME currentReferenceRecordClass->numberOfThisPartIDInTheUnit = " << currentReferenceRecordClass->numberOfThisPartIDInTheUnit << endl;
					}

					currentReferenceRecordClass = currentReferenceRecordClass->next;
				}

				currentReferenceRecordClass = referenceBeingSearchedFor->subModelDetails->recordOfUnitTypeDetails;
				while(currentReferenceRecordClass->next != NULL)
				{
					if(currentReferenceRecordClass->name == PERSON_HEAD_NAME)
					{

						cout << "referenceBeingSearchedFor->name = " << referenceBeingSearchedFor->name << endl;
						cout << "\t PERSON_HEAD_NAME referenceBeingSearchedFor->numberOfThisPartIDInTheUnit = " << currentReferenceRecordClass->numberOfThisPartIDInTheUnit << endl;
					}

					currentReferenceRecordClass = currentReferenceRecordClass->next;
				}
				*/


				//cout << "\nDEBUG: c4" << endl;
				//cout << "currentReference->name = " << currentReference->name << endl;
				//cout << "referenceBeingSearchedFor->name = " << referenceBeingSearchedFor->name << endl;

			}

			if(!isFillingParentUnitWithAllCombatRelevantChildModelDetails)
			{
				if(this->compareSubmodelNamesAndIfSameCopySubmodelReference(referenceBeingSearchedFor, currentReference))
				{
					particularSubModelFoundDuringSearch = true;
					currentReferenceFoundIsTheParticularSubmodelBeingLookedFor = true;
				}
			}

			if(this->searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(referenceBeingSearchedFor, currentReference->firstReferenceWithinSubModel, currentReference, (currentReferenceFoundIsTheParticularSubmodelBeingLookedFor|isFillingParentUnitWithAllCombatRelevantChildModelDetails)))
			{
				if(!isFillingParentUnitWithAllCombatRelevantChildModelDetails) //always TRUE	//(!isFillingParentUnitWithAllCombatRelevantChildModelDetails) is implied true as searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel has returned true.
				{
					particularSubModelFoundDuringSearch = true;

					//no longer used due to algorithm break down
					//copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject(referenceBeingSearchedFor->subModelDetails, parentReference->subModelDetails);
				}
			}
		}

		//cout << "DEBUG: c6 currentReference->next = " << currentReference->next << endl;
		currentReference = currentReference->next;
		//cout << "DEBUG: c7 currentReference->next = " << currentReference->next << endl;
	}

	return particularSubModelFoundDuringSearch;
}

bool LRRCcombatClass::compareSubmodelNamesAndIfSameCopySubmodelReference(LDreference* referenceBeingSearchedFor, LDreference* referenceCurrentlyFoundDuringSearch)
{
	bool particularSubModelFoundDuringSearch = false;
	//cout << "DEBUG: d1" << endl;
	//cout << "DEBUG: d1 referenceBeingSearchedFor->name =" << referenceBeingSearchedFor->name << endl;
	//cout << "DEBUG: d1 referenceCurrentlyFoundDuringSearch->name =" << referenceCurrentlyFoundDuringSearch->name << endl;
	if(referenceBeingSearchedFor->name == referenceCurrentlyFoundDuringSearch->name)
	{
		//cout << "DEBUG: d2 referenceBeingSearchedFor->colour =" << referenceBeingSearchedFor->colour << endl;
		//cout << "DEBUG: d2 referenceCurrentlyFoundDuringSearch->colour =" << referenceCurrentlyFoundDuringSearch->colour << endl;
		if(referenceBeingSearchedFor->colour == referenceCurrentlyFoundDuringSearch->colour)
		{
			LDreferenceClass.copyReferencePosition(referenceBeingSearchedFor, referenceCurrentlyFoundDuringSearch);

			LRRCmodelClass.copyAllUnitDetails(referenceBeingSearchedFor->subModelDetails, referenceCurrentlyFoundDuringSearch->subModelDetails);

			particularSubModelFoundDuringSearch = true;
			//cout << "DEBUG: d5" << endl;
		}
		else
		{
			//do not do anything with this particular submodel/part/ModelDetails LDreference, it is the wrong one (it is not of its stated colour)
		}
	}
	else
	{
		//do not do anything with this particular submodel/part/ModelDetails LDreference, it is the wrong one (it is not of the required)
	}
	return particularSubModelFoundDuringSearch;
}









































































































































//yet to update these and many other functions with XML rules classes.... 29/12/07


bool LRRCcombatClass::performUnitOrdinatesCheck(const ModelDetails* u)
{
	bool result = true;

	//these specific checks require specific hard coded rules...
	if(u->numLongBow)
	{
		if(u->numQuiver)
		{
			//cout << "DEBUG: ModelDetails: Long Bow Equipped" << endl;
		}
		else
		{
		#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
			cout << "Invalid ModelDetails: Bowman without Quiver \n" << endl;
			cout << "u->numLongBow = " << u->numLongBow << endl;
		#endif
			result = false;
		}

		if(u->numShields)
		{
		#ifndef DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
			cout << "Invalid ModelDetails: Bowman with with Shield \n" << endl;
		#endif
			result = false;
		}
	}


	return result;
}


void LRRCcombatClass::performFinalUnitClassCalculations(ModelDetails* u)
{
	/*Perform Basic ModelDetails Class close combat Calculations*/
	//these are no longer calculated within the original updateUnitDetailsWithCombatDetails() routine

			//All persons have daggers and hence a minimum attack value of 1 (d2)

	/*
	u->numPerson = 0;
	u->numHorse = 0;
	*/

	//NB unit type calculations have been shifted forward
	LRRCmodelClass.determineUnitTypeAndMinSpeedOfUnitGroup(u);

	RecordClass* currentReferenceRecordClass;
	XMLrulesClass* currentReferenceRulesClass;

	currentReferenceRulesClass = LRRCrulesUnitTypeDetails;
	currentReferenceRecordClass = u->recordOfUnitTypeDetails;
	while(currentReferenceRecordClass->next != NULL)
	{
		/*
		if(currentReferenceRecordClass->name == PERSON_HEAD_NAME)
		{
			u->numPerson = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;	//CHECK THIS is this required here also? NB it is already calcualted in determineUnitTypeAndMinSpeedOfUnitGroup
		}
		else if(currentReferenceRecordClass->name == HORSE_NAME)
		{
			u->numHorse = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;		//CHECK THIS is this required here also? NB it is already calcualted in determineUnitTypeAndMinSpeedOfUnitGroup
		}
		*/
		if(currentReferenceRecordClass->name == NECK_QUIVER_NAME)
		{
			u->numQuiver = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;		//required for performUnitOrdinatesCheck
		}
		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass -> next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceHead;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsDefenceHead;
	while(currentReferenceRecordClass->next != NULL)
	{
		if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
		{
			u->helmetDefenceValue = SHAREDvars.maxInt(u->helmetDefenceValue, currentReferenceRulesClass->attribute4);
			//cout << "e1 u->helmetDefenceValue = " << u->helmetDefenceValue << endl;
		}

		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}


	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceTorso;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsDefenceTorso;
	while(currentReferenceRecordClass->next != NULL)
	{
		if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
		{
			u->breastDefenceValue = SHAREDvars.maxInt(u->breastDefenceValue, currentReferenceRulesClass->attribute4);
			//cout << "e2 u->breastDefenceValue = " << u->breastDefenceValue << endl;
		}

		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceShield;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsDefenceShield;
	while(currentReferenceRecordClass->next != NULL)
	{
		if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
		{
			u->shieldDefenceValue = SHAREDvars.maxInt(u->shieldDefenceValue, currentReferenceRulesClass->attribute4);
			//cout << "e3 u->shieldDefenceValue = " << u->shieldDefenceValue << endl;
			u->numShields++;
		}

		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsAttackLongDistance;
	while(currentReferenceRecordClass->next != NULL)
	{
		if(currentReferenceRecordClass->name == HAND_BOW_NAME)
		{
			u->numLongBow = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;		//required for performUnitOrdinatesCheck
		}
		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackCloseCombat;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsAttackCloseCombat;
	while(currentReferenceRecordClass->next != NULL)
	{
		if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
		{
			u->closeCombatAttackValue = SHAREDvars.maxInt(currentReferenceRulesClass->attribute4, u->closeCombatAttackValue);
		}

		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsAttackLongDistance;
	while(currentReferenceRecordClass->next != NULL)
	{
		if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
		{
			if(currentReferenceRulesClass->name == CATAPULT_BUCKET_NAME)
			{
				u->longDistanceAttackValue = SHAREDvars.maxInt(u->longDistanceAttackValue, (currentReferenceRulesClass->attribute4* currentReferenceRecordClass->numberOfThisPartIDInTheUnit));
			}
			else
			{
				u->longDistanceAttackValue = SHAREDvars.maxInt(u->longDistanceAttackValue, currentReferenceRulesClass->attribute4);
			}
			u->longDistanceAttackBaseRange = SHAREDvars.maxInt(u->longDistanceAttackBaseRange, currentReferenceRulesClass->attribute5);
			u->hasLongDistanceCombatWeapon = true;
		}

		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}


	/*peform summary calculations here*/

		//calculate default bonuses - these calculations here are only used for sprite display (these are recalculated during combat) once the circumstances for bonus calculations are known
	u->closeCombatAttackBonus = 0;
	this->calculateCloseCombatAttackBonus(u, false, true);		//this->calculateCloseCombatAttackBonus() cannot be peformed at this stage (without knowledge of opponents) - do this later
										//calculate close combat attack bonus here assuming the opponent does not have a horse (ie is a foot soldier)
										//this->calculateCloseCombatAttackBonus() is calculated here only for sprite display; and is recalculated during combat
	u->longDistanceAttackBonus = 0;					//calculateLongDistanceAttackBonus() cannot be peformed at this stage (without knowledge of opponents) - do this later

	/*long distance attack range value cannot be given without knowing the relative height of the attacker and his oppponent*/

	u->closeCombatAttackTotal = (u->closeCombatAttackValue + u->closeCombatAttackBonus);
	u->longDistanceAttackTotal = (u->longDistanceAttackValue + u->longDistanceAttackBonus);
		//u->longDistanceAttackTotal = (SHAREDvars.maxInt(u->longDistancePrimaryAttackValue, u->longDistanceSecondaryAttackValue) + u->longDistanceAttackBonus);

	u->attackTotal = SHAREDvars.maxInt((u->longDistanceAttackTotal), (u->closeCombatAttackTotal));

	u->armourDefenceValue = u->breastDefenceValue + u->helmetDefenceValue + u->shieldDefenceValue;

	if(u->numBattlement || u->numBush)
	{
		//u->defenceTotal = u->defenceTotal + 1;	23-Sept-06; the defence bonus modifier is now applied higher up the chain
		u->defenceBonus = 1;
	}

	/*
	//DEBUG:
	cout << "u->armourDefenceValue = " << u->armourDefenceValue << endl;
	cout << "u->breastDefenceValue = " << u->breastDefenceValue << endl;
	cout << "u->helmetDefenceValue = " << u->helmetDefenceValue << endl;
	cout << "u->shieldDefenceValue = " << u->shieldDefenceValue << endl;
	cout << "u->defenceBonus = " << u->defenceBonus << endl;
	*/

	u->defenceTotal = u->armourDefenceValue + u->defenceBonus;
	//cout << "e4 u->defenceTotal  = " << u->defenceTotal  << endl;

}

void LRRCcombatClass::performFinalUnitGroupClassCalculations(ModelDetails* u)
{
	/*Perform Basic ModelDetails Class close combat Calculations*/
	//these are no longer calculated within the original updateUnitDetailsWithCombatDetails() routine

			//All persons have daggers and hence a minimum attack value of 1 (d2)

	/*
	u->numPerson = 0;
	u->numHorse = 0;
	*/

	//NB unit type calculations have been shifted forward
	LRRCmodelClass.determineUnitTypeAndMinSpeedOfUnitGroup(u);


	RecordClass* currentReferenceRecordClass;
	XMLrulesClass* currentReferenceRulesClass;

	currentReferenceRecordClass = u->recordOfUnitTypeDetails;
	while(currentReferenceRecordClass->next != NULL)
	{
		/*
		if(currentReferenceRecordClass->name == PERSON_HEAD_NAME)
		{
			u->numPerson = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;	//CHECK THIS is this required here also? NB it is already calcualted in determineUnitTypeAndMinSpeedOfUnitGroup
		}
		else if(currentReferenceRecordClass->name == HORSE_NAME)
		{
			u->numHorse = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;		//CHECK THIS is this required here also? NB it is already calcualted in determineUnitTypeAndMinSpeedOfUnitGroup
		}
		*/
		if(currentReferenceRecordClass->name == NECK_QUIVER_NAME)
		{
			u->numQuiver = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;		//required for performUnitOrdinatesCheck
		}
		currentReferenceRecordClass = currentReferenceRecordClass->next;
	}

	currentReferenceRecordClass = u->recordOfUnitCombatDetailsAttackLongDistance;
	while(currentReferenceRecordClass->next != NULL)
	{
		if(currentReferenceRecordClass->name == HAND_BOW_NAME)
		{
			u->numLongBow = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;		//required for performUnitOrdinatesCheck
		}
		currentReferenceRecordClass = currentReferenceRecordClass->next;
	}


	u->helmetDefenceValue  = 0;
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceHead;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsDefenceHead;
	while(currentReferenceRecordClass->next != NULL)
	{
		u->helmetDefenceValue = u->helmetDefenceValue + (currentReferenceRulesClass->attribute4* currentReferenceRecordClass->numberOfThisPartIDInTheUnit);

		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	u->breastDefenceValue  = 0;
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceTorso;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsDefenceTorso;
	while(currentReferenceRecordClass->next != NULL)
	{

		u->breastDefenceValue = u->breastDefenceValue + (currentReferenceRulesClass->attribute4* currentReferenceRecordClass->numberOfThisPartIDInTheUnit);

		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	u->shieldDefenceValue  = 0;
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceShield;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsDefenceShield;
	while(currentReferenceRecordClass->next != NULL)
	{

		u->shieldDefenceValue = u->shieldDefenceValue + (currentReferenceRulesClass->attribute4* currentReferenceRecordClass->numberOfThisPartIDInTheUnit);
		if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
		{
			u->numShields++;
		}

		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	u->closeCombatAttackValue  = 0;
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackCloseCombat;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsAttackCloseCombat;
	while(currentReferenceRecordClass->next != NULL)
	{
		u->closeCombatAttackValue = u->closeCombatAttackValue + (currentReferenceRulesClass->attribute4* currentReferenceRecordClass->numberOfThisPartIDInTheUnit);

		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	u->longDistanceAttackValue  = 0;
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsAttackLongDistance;
	while(currentReferenceRecordClass->next != NULL)
	{

		u->longDistanceAttackValue = u->longDistanceAttackValue + (currentReferenceRulesClass->attribute4* currentReferenceRecordClass->numberOfThisPartIDInTheUnit);

		if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
		{
			u->longDistanceAttackBaseRange = SHAREDvars.maxInt(u->longDistanceAttackBaseRange, currentReferenceRulesClass->attribute5);
			u->hasLongDistanceCombatWeapon = true;
		}

		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	/*peform summary calculations here*/

		//calculate default bonuses - these calculations here are only used for sprite display (these are recalculated during combat) once the circumstances for bonus calculations are known
	u->closeCombatAttackBonus = 0;		//only done for single units, not unit groups
	u->longDistanceAttackBonus = 0;		//only done for single units, not unit groups

	/*long distance attack range value cannot be given without knowing the relative height of the attacker and his oppponent*/

	if(u->numPerson > 0)
	{
		//cout << "HAND_DAGGER_MOD = " << HAND_DAGGER_MOD << endl;
		u->closeCombatAttackValue = SHAREDvars.maxInt(int(HAND_DAGGER_MOD), u->closeCombatAttackValue);
	}
	u->closeCombatAttackTotal = (u->closeCombatAttackValue + u->closeCombatAttackBonus);

	u->longDistanceAttackTotal = (u->longDistanceAttackValue + u->longDistanceAttackBonus);
		//u->longDistanceAttackTotal = (SHAREDvars.maxInt(u->longDistancePrimaryAttackValue, u->longDistanceSecondaryAttackValue) + u->longDistanceAttackBonus);

	u->attackTotal = SHAREDvars.maxInt((u->longDistanceAttackTotal), (u->closeCombatAttackTotal));

	u->armourDefenceValue = u->breastDefenceValue + u->helmetDefenceValue + u->shieldDefenceValue;



	if(u->numBattlement || u->numBush)
	{
		//u->defenceTotal = u->defenceTotal + 1;	23-Sept-06; the defence bonus modifier is now applied higher up the chain
		u->defenceBonus = 1;
	}

	/*
	//DEBUG:
	cout << "u->armourDefenceValue = " << u->armourDefenceValue << endl;
	cout << "u->breastDefenceValue = " << u->breastDefenceValue << endl;
	cout << "u->helmetDefenceValue = " << u->helmetDefenceValue << endl;
	cout << "u->shieldDefenceValue = " << u->shieldDefenceValue << endl;
	cout << "u->defenceBonus = " << u->defenceBonus << endl;
	*/

	if((u->numPerson) && (u->closeCombatAttackValue == 0))
	{
		cout << "error: ((u->numPerson) && (u->closeCombatAttackValue == 0))" << endl;
		exit(0);
	}
	u->defenceTotal = u->armourDefenceValue + u->defenceBonus;

}





/*VERY OLD:

General Notes:
lances can only be used against other knights, or when charging against a footsoldier
Spears are especially good when used by a mounted unit (same close combat attack level as swords/axes)

int LRRCcombatClass::calculateCloseCombatAttackBonus(ModelDetails* unit, const bool unit2HasHorse, const bool unitTakesInitative)
{
	int unitMountedAttack;

	if(unit->numHorse && unitTakesInitative)
	{
		if(unit->numLance)
		{
			unitMountedAttack = HAND_LANCE_MOD + HAND_LANCE_MOUNTED_BONUS;
			unit->closeCombatAttackBonus = HAND_LANCE_MOUNTED_BONUS;
		}
		else if(unit->numSpear)
		{
			unitMountedAttack = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS;
			unit->closeCombatAttackBonus = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS - unit->closeCombatAttackValue;		//or just unit->closeCombatAttackBonus = HAND_SPEAR_MOUNTED_BONUS - assuming the unit does not have a sword or axe also
		}
		else
		{
			//mounted attackers generally get +1 on their attack weapons
			unitMountedAttack = unit->closeCombatAttackValue + HAND_OTHER_MOUNTED_BONUS;
			unit->closeCombatAttackBonus = HAND_OTHER_MOUNTED_BONUS;
		}
	}
	else if(unit->numHorse)
	{
		if((unit->numLance) && (unit2HasHorse))
		{
			unitMountedAttack = HAND_LANCE_MOD + HAND_LANCE_MOUNTED_BONUS;
			unit->closeCombatAttackBonus = HAND_LANCE_MOUNTED_BONUS;
		}
		else if(unit->numSpear)
		{
			unitMountedAttack = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS;
			unit->closeCombatAttackBonus = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS - unit->closeCombatAttackValue;		//or just unit->closeCombatAttackBonus = HAND_SPEAR_MOUNTED_BONUS - assuming the unit does not have a sword or axe also
		}
		else
		{
			if(unit->numLance)
			{
				unitMountedAttack = unit->closeCombatAttackValue;
			}
			else
			{
				unitMountedAttack = unit->closeCombatAttackValue + HAND_OTHER_MOUNTED_BONUS;
				unit->closeCombatAttackBonus = HAND_OTHER_MOUNTED_BONUS;
			}
		}
	}
	else
	{
		unitMountedAttack = unit->closeCombatAttackValue;
	}

	return unitMountedAttack;

}

*/


/*
General Notes:
#ifdef USE_UNACCEPTED_2006_RULES lances can only be used against other knights, or when charging against a footsoldier
Spears are especially good when used against a mounted unit (same close combat attack level as swords/axes)
*/

int LRRCcombatClass::calculateCloseCombatAttackBonus(ModelDetails* unit, const bool unit2HasHorse, const bool unitTakesInitative)
{
	int unitAttackWithBonus = 0;

#ifdef USE_UNACCEPTED_2006_RULES
	if(((unit->numHorse) == 0) && (unit2HasHorse) && (unit->numSpear))
	{
		unitAttackWithBonus = HAND_SPEAR_MOD + HAND_SPEAR_AGAINST_MOUNTED_BONUS;
		unit->closeCombatAttackBonus = HAND_SPEAR_MOD + HAND_SPEAR_AGAINST_MOUNTED_BONUS - unit->closeCombatAttackValue;		//or just unit->closeCombatAttackBonus = HAND_SPEAR_AGAINST_MOUNTED_BONUS - assuming the unit does not have a sword or axe also
	}
	else if(unit->numHorse)
	{
		if(unitTakesInitative)
		{
			if(unit->numLance)
			{
				unitAttackWithBonus = HAND_LANCE_MOD + HAND_LANCE_MOUNTED_BONUS;
				unit->closeCombatAttackBonus = HAND_LANCE_MOUNTED_BONUS;
			}
			else if(unit->numSpear)
			{
				unitAttackWithBonus = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS;
				unit->closeCombatAttackBonus = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS - unit->closeCombatAttackValue;		//or just unit->closeCombatAttackBonus = HAND_SPEAR_MOUNTED_BONUS - assuming the unit does not have a sword or axe also
			}
			else
			{
				//mounted attackers generally get +1 on their attack weapons
				unitAttackWithBonus = unit->closeCombatAttackValue + HAND_OTHER_MOUNTED_BONUS;
				unit->closeCombatAttackBonus = HAND_OTHER_MOUNTED_BONUS;
			}
		}
		else
		{
			if((unit->numLance) && (unit2HasHorse))
			{
				unitAttackWithBonus = HAND_LANCE_MOD + HAND_LANCE_MOUNTED_BONUS;
				unit->closeCombatAttackBonus = HAND_LANCE_MOUNTED_BONUS;
			}
			else if((unit->numSpear) && (unit2HasHorse))
			{
				unitAttackWithBonus = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS;
				unit->closeCombatAttackBonus = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS - unit->closeCombatAttackValue;		//or just unit->closeCombatAttackBonus = HAND_SPEAR_MOUNTED_BONUS - assuming the unit does not have a sword or axe also
			}
			else
			{
				unitAttackWithBonus = unit->closeCombatAttackValue + HAND_OTHER_MOUNTED_BONUS;
				unit->closeCombatAttackBonus = HAND_OTHER_MOUNTED_BONUS;
			}
		}

	}
	else
	{
		unitAttackWithBonus = unit->closeCombatAttackValue;
	}
#elif defined USE_UNACCEPTED_2007_2a11a_RULES
	if(((unit->numHorse) == 0) && (unit2HasHorse) && (unit->numSpear))
	{
		unitAttackWithBonus = HAND_SPEAR_MOD + HAND_SPEAR_AGAINST_MOUNTED_BONUS;
		unit->closeCombatAttackBonus = HAND_SPEAR_MOD + HAND_SPEAR_AGAINST_MOUNTED_BONUS - unit->closeCombatAttackValue;		//or just unit->closeCombatAttackBonus = HAND_SPEAR_AGAINST_MOUNTED_BONUS - assuming the unit does not have a sword or axe also
	}
	else if(unit->numHorse)
	{
			if(unit->numLance)
			{
				unitAttackWithBonus = HAND_LANCE_MOD + HAND_LANCE_MOUNTED_BONUS;
				unit->closeCombatAttackBonus = HAND_LANCE_MOUNTED_BONUS;
			}
			else if(unit->numSpear)
			{
				unitAttackWithBonus = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS;
				unit->closeCombatAttackBonus = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS - unit->closeCombatAttackValue;		//or just unit->closeCombatAttackBonus = HAND_SPEAR_MOUNTED_BONUS - assuming the unit does not have a sword or axe also
			}
			else
			{
				//mounted attackers generally get +1 on their attack weapons
				unitAttackWithBonus = unit->closeCombatAttackValue + HAND_OTHER_MOUNTED_BONUS;
				unit->closeCombatAttackBonus = HAND_OTHER_MOUNTED_BONUS;
			}
	}
	else
	{
		unitAttackWithBonus = unit->closeCombatAttackValue;
	}
#else

	//this now needs to be upgraded for rules xml file data...

	XMLrulesClass* currentReferenceRulesClass;
	RecordClass* currentReferenceRecordClass;

	if(unit2HasHorse && (unit->numHorse)==0)
	{
		currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackCloseCombat;
		currentReferenceRecordClass = unit->recordOfUnitCombatDetailsAttackCloseCombat;
		while(currentReferenceRecordClass->next != NULL)
		{
			if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
			{
				if((currentReferenceRulesClass->attribute4 + currentReferenceRulesClass->attribute6) > unitAttackWithBonus)
				{
					unitAttackWithBonus = (currentReferenceRulesClass->attribute4 + currentReferenceRulesClass->attribute6);
					unit->closeCombatAttackValue = currentReferenceRulesClass->attribute4;	//recalculate closeCombatAttackValue for optimum weapon (taking into account their current bonus')
					unit->closeCombatAttackBonus = currentReferenceRulesClass->attribute6;
				}
			}

			currentReferenceRecordClass = currentReferenceRecordClass->next;
			currentReferenceRulesClass = currentReferenceRulesClass->next;
		}

		unit->closeCombatAttackValue = SHAREDvars.maxInt(int(HAND_DAGGER_MOD), unit->closeCombatAttackValue);
		unitAttackWithBonus = SHAREDvars.maxInt(int(HAND_DAGGER_MOD), unitAttackWithBonus);
	}
	else if(unit->numHorse)
	{
		currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackCloseCombat;
		currentReferenceRecordClass = unit->recordOfUnitCombatDetailsAttackCloseCombat;
		while(currentReferenceRecordClass->next != NULL)
		{
			if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
			{
				if((currentReferenceRulesClass->attribute4 + currentReferenceRulesClass->attribute5) > unitAttackWithBonus)
				{
					unitAttackWithBonus = (currentReferenceRulesClass->attribute4 + currentReferenceRulesClass->attribute5);
					unit->closeCombatAttackValue = currentReferenceRulesClass->attribute4;	//recalculate closeCombatAttackValue for optimum weapon (taking into account their current bonus')
					unit->closeCombatAttackBonus = currentReferenceRulesClass->attribute5;
				}
			}

			currentReferenceRecordClass = currentReferenceRecordClass->next;
			currentReferenceRulesClass = currentReferenceRulesClass->next;
		}

		unit->closeCombatAttackValue = SHAREDvars.maxInt(int(HAND_DAGGER_MOD), unit->closeCombatAttackValue);
		unitAttackWithBonus = SHAREDvars.maxInt(int(HAND_DAGGER_MOD), unitAttackWithBonus);
	}
	else
	{
		currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackCloseCombat;
		currentReferenceRecordClass = unit->recordOfUnitCombatDetailsAttackCloseCombat;
		while(currentReferenceRecordClass->next != NULL)
		{
			if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
			{
				if((currentReferenceRulesClass->attribute4) > unitAttackWithBonus)
				{
					unitAttackWithBonus = (currentReferenceRulesClass->attribute4);
					unit->closeCombatAttackValue = currentReferenceRulesClass->attribute4;	//recalculate closeCombatAttackValue for optimum weapon (taking into account their current bonus')
					unit->closeCombatAttackBonus = 0;
				}
			}

			currentReferenceRecordClass = currentReferenceRecordClass->next;
			currentReferenceRulesClass = currentReferenceRulesClass->next;
		}


		unit->closeCombatAttackValue = SHAREDvars.maxInt(int(HAND_DAGGER_MOD), unit->closeCombatAttackValue);
		unitAttackWithBonus = unit->closeCombatAttackValue;
		unit->closeCombatAttackBonus = 0;
	}

	/*
	if(unit2HasHorse && (unit->numHorse)==0)
	{
		if(unit->numLance)
		{
			unitAttackWithBonus = HAND_LANCE_MOD + HAND_LANCE_MOUNTED_BONUS;
			unit->closeCombatAttackBonus = HAND_LANCE_MOUNTED_BONUS;
		}
		else if(unit->numSpear)
		{
			unitAttackWithBonus = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS;
			unit->closeCombatAttackBonus = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS - unit->closeCombatAttackValue;		//or just unit->closeCombatAttackBonus = HAND_SPEAR_MOUNTED_BONUS - assuming the unit does not have a sword or axe also
		}
		else
		{
			unitAttackWithBonus = unit->closeCombatAttackValue;
		}
	}
	else if(unit->numHorse)
	{
		if(unit->numLance)
		{
			unitAttackWithBonus = HAND_LANCE_MOD + HAND_LANCE_MOUNTED_BONUS;
			unit->closeCombatAttackBonus = HAND_LANCE_MOUNTED_BONUS;
		}
		else if(unit->numSpear)
		{
			unitAttackWithBonus = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS;
			unit->closeCombatAttackBonus = HAND_SPEAR_MOD + HAND_SPEAR_MOUNTED_BONUS - unit->closeCombatAttackValue;		//or just unit->closeCombatAttackBonus = HAND_SPEAR_MOUNTED_BONUS - assuming the unit does not have a sword or axe also
		}
		else
		{
			//mounted attackers generally get +1 on their attack weapons
			unitAttackWithBonus = unit->closeCombatAttackValue + HAND_OTHER_MOUNTED_BONUS;
			unit->closeCombatAttackBonus = HAND_OTHER_MOUNTED_BONUS;
		}
	}
	else
	{
		unitAttackWithBonus = unit->closeCombatAttackValue;
		unit->closeCombatAttackBonus = 0;
	}
	*/
#endif

	return unitAttackWithBonus;

}



void LRRCcombatClass::calculateLongDistanceAttackBonus(LDreference* unit1ReferenceInThisPhaseStartSceneFile, LDreference* unit2ReferenceInThisPhaseStartSceneFile, const bool unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, const bool unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, bool* unit1CanPerformLongDistanceAttack, bool* unit2CanPerformLongDistanceAttack, const double distanceBetweenUnitsForLDTest)
{
	//calculate the range of each units weapon based on the relative height between ModelDetails:
	/******************************************************/
	double longRangeDistanceBetweenTheTwoUnits;
	double unit1MinimumRangeOfFire;
	double unit1RangeModifier;
	double unit2MinimumRangeOfFire;
	double unit2RangeModifier;

	/*Method 3 - simplied - uses modified rules, assumes linear horiz/range advatange corresponding to height increase*/
	//(positionZOfUnit2-positionZOfUnit1) and not (positionZOfUnit2-positionZOfUnit1) for unit1 range modifier; as the lesser (or more negative) the z distance of a model, the higher the model is in the scene

	unit1RangeModifier = this->calculateLongDistanceRangeModifier(unit2ReferenceInThisPhaseStartSceneFile, unit1ReferenceInThisPhaseStartSceneFile);
	unit2RangeModifier = this->calculateLongDistanceRangeModifier(unit1ReferenceInThisPhaseStartSceneFile, unit2ReferenceInThisPhaseStartSceneFile);

	unit1MinimumRangeOfFire = CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY;	//CHECK this: possibly make this setting more advanced for catapults (eg a ratio of the max range of fire distance)
	unit2MinimumRangeOfFire = CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY;	//CHECK this: possibly make this setting more advanced for catapults (eg a ratio of the max range of fire distance)
	//cout << "(currentPositionYOfUnit2-currentPositionYOfUnit1) = " << (currentPositionYOfUnit2-currentPositionYOfUnit1) << endl;
	//cout << "(currentPositionYOfUnit1-currentPositionYOfUnit2) = " << (currentPositionYOfUnit1-currentPositionYOfUnit2) << endl;

	/******************************************************/

	/*
	cout << "DEBUG: unit1ReferenceInThisPhaseStartSceneFile->absolutePosition.x = " << unit1ReferenceInThisPhaseStartSceneFile->absolutePosition.x << endl;
	cout << "DEBUG: unit1ReferenceInThisPhaseStartSceneFile->absolutePosition.y = " << unit1ReferenceInThisPhaseStartSceneFile->absolutePosition.y << endl;
	cout << "DEBUG: unit1ReferenceInThisPhaseStartSceneFile->absolutePosition.z = " << unit1ReferenceInThisPhaseStartSceneFile->absolutePosition.z << endl;
	cout << "DEBUG: unit2ReferenceInThisPhaseStartSceneFile->absolutePosition.x = " << unit2ReferenceInThisPhaseStartSceneFile->absolutePosition.x << endl;
	cout << "DEBUG: unit2ReferenceInThisPhaseStartSceneFile->absolutePosition.y = " << unit2ReferenceInThisPhaseStartSceneFile->absolutePosition.y << endl;
	cout << "DEBUG: unit2ReferenceInThisPhaseStartSceneFile->absolutePosition.z = " << unit2ReferenceInThisPhaseStartSceneFile->absolutePosition.z << endl;
	*/

	if(!(unit1ReferenceInThisPhaseStartSceneFile->subModelDetails->hasLongDistanceCombatWeapon))
	{
#ifndef USE_ANN
		cout << "Unit 1 does not have a long distance combat capable weapon" << endl;
#endif
	}
	else if(unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack)
	{


		XMLrulesClass* currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
		RecordClass* currentReferenceRecordClass = unit1ReferenceInThisPhaseStartSceneFile->subModelDetails->recordOfUnitCombatDetailsAttackLongDistance;
		bool foundOptimumLongDistanceWeapon = false;
		while((currentReferenceRecordClass->next != NULL) && (!foundOptimumLongDistanceWeapon))
		{
			if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
			{
				int currentWeaponsRange = currentReferenceRulesClass->attribute5;
				double currentWeaponMaximumRangeOfFireWithModifier = currentWeaponsRange + unit1RangeModifier;
				if(this->calculateIfUnitIsWithinAGivenLongRangeAttackDistance(distanceBetweenUnitsForLDTest, (int)currentWeaponMaximumRangeOfFireWithModifier, (int)unit1MinimumRangeOfFire))
				{
					foundOptimumLongDistanceWeapon = true;

					if(currentReferenceRulesClass->name == CATAPULT_BUCKET_NAME)
					{
						unit1ReferenceInThisPhaseStartSceneFile->subModelDetails->longDistanceAttackValue = currentReferenceRulesClass->attribute4* currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
					}
					else
					{
						unit1ReferenceInThisPhaseStartSceneFile->subModelDetails->longDistanceAttackValue = currentReferenceRulesClass->attribute4;
					}
					unit1ReferenceInThisPhaseStartSceneFile->subModelDetails->longDistanceAttackBaseRange = currentReferenceRulesClass->attribute5;
					unit1ReferenceInThisPhaseStartSceneFile->subModelDetails->hasLongDistanceCombatWeapon = true;		//not necessary, already filled in
					*unit1CanPerformLongDistanceAttack = true;
				}
			}

			currentReferenceRecordClass = currentReferenceRecordClass->next;
			currentReferenceRulesClass = currentReferenceRulesClass->next;
		}

	}

	if(!(unit2ReferenceInThisPhaseStartSceneFile->subModelDetails->hasLongDistanceCombatWeapon))
	{
#ifndef USE_ANN
		cout << "unit 2 does not have a long distance combat capable weapon" << endl;
#endif
	}
	else if(unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack)
	{

		XMLrulesClass* currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
		RecordClass* currentReferenceRecordClass = unit2ReferenceInThisPhaseStartSceneFile->subModelDetails->recordOfUnitCombatDetailsAttackLongDistance;
		bool foundOptimumLongDistanceWeapon = false;
		while((currentReferenceRecordClass->next != NULL) && (!foundOptimumLongDistanceWeapon))
		{
			if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
			{
				int currentWeaponsRange = currentReferenceRulesClass->attribute5;
				double currentWeaponMaximumRangeOfFireWithModifier = currentWeaponsRange + unit2RangeModifier;
				if(this->calculateIfUnitIsWithinAGivenLongRangeAttackDistance(distanceBetweenUnitsForLDTest, (int)currentWeaponMaximumRangeOfFireWithModifier, (int)unit2MinimumRangeOfFire))
				{
					foundOptimumLongDistanceWeapon = true;

					if(currentReferenceRulesClass->name == CATAPULT_BUCKET_NAME)
					{
						unit2ReferenceInThisPhaseStartSceneFile->subModelDetails->longDistanceAttackValue = currentReferenceRulesClass->attribute4* currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
					}
					else
					{
						unit2ReferenceInThisPhaseStartSceneFile->subModelDetails->longDistanceAttackValue = currentReferenceRulesClass->attribute4;
					}
					unit2ReferenceInThisPhaseStartSceneFile->subModelDetails->longDistanceAttackBaseRange = currentReferenceRulesClass->attribute5;
					unit2ReferenceInThisPhaseStartSceneFile->subModelDetails->hasLongDistanceCombatWeapon = true;		//not necessary, already filled in
					*unit2CanPerformLongDistanceAttack = true;
				}
			}

			currentReferenceRecordClass = currentReferenceRecordClass->next;
			currentReferenceRulesClass = currentReferenceRulesClass->next;
		}
	}
}


//range calculations



bool LRRCcombatClass::calculateIfTwoUnitsAreWithinAGivenLongRangeAttackDistance(const double positionXOfUnit1, const double positionXOfUnit2, const double positionYOfUnit1, const double positionYOfUnit2, const double positionZOfUnit1, const double positionZOfUnit2, const int maximumDistance, const int minimumDistance)
{
	bool result;

	double LRRCdistanceBetweenTheTwoUnits = this->calculateTheDistanceBetweenTwoUnits(positionXOfUnit1, positionXOfUnit2, positionYOfUnit1, positionYOfUnit2, positionZOfUnit1, positionZOfUnit2);

	result = this->calculateIfUnitIsWithinAGivenLongRangeAttackDistance(LRRCdistanceBetweenTheTwoUnits, maximumDistance, minimumDistance);

	return result;
}

bool LRRCcombatClass::calculateIfUnitIsWithinAGivenLongRangeAttackDistance(const double distanceBetweenTheTwoUnits, const int maximumDistance, const int minimumDistance)
{
	bool result;

	if(((int)distanceBetweenTheTwoUnits <= maximumDistance) && ((int)distanceBetweenTheTwoUnits >= minimumDistance))
	{
		//cout << "\nDEBUG: Units are within Range for long range attack. Distance between units = " << distanceBetweenTheTwoUnits << endl;
		//cout << "Maximum Distance = " << maximumDistance << endl;
		result = true;
	}
	else
	{
		/*
		cout << "positionXOfUnit1 = " << positionXOfUnit1 << ", positionXOfUnit2 = " << positionXOfUnit2 << endl;
		cout << "positionYOfUnit1 = " << positionYOfUnit1 << ", positionYOfUnit2 = " << positionYOfUnit2 << endl;
		cout << "positionZOfUnit1 = " << positionZOfUnit1 << ", positionZOfUnit2 = " << positionZOfUnit2 << endl;
		*/
		result = false;
	}

	return result;
}


double LRRCcombatClass::calculateTheDistanceBetweenTwoUnits(vec* positionOfUnit1, vec* positionOfUnit2)
{
	vec positionOfUnit1ConvertedToLdraw;
	vec positionOfUnit2ConvertedToLdraw;
	SHAREDvector.copyVectors(&(positionOfUnit1ConvertedToLdraw), positionOfUnit1);
	SHAREDvector.copyVectors(&(positionOfUnit2ConvertedToLdraw), positionOfUnit2);
	SHAREDvector.multiplyVectorByScalar(&(positionOfUnit1ConvertedToLdraw), (1.0/LDRAW_UNITS_PER_CM));
	SHAREDvector.multiplyVectorByScalar(&(positionOfUnit2ConvertedToLdraw), (1.0/LDRAW_UNITS_PER_CM));

	return SHAREDvector.calculateTheDistanceBetweenTwoPoints(&(positionOfUnit1ConvertedToLdraw), &(positionOfUnit2ConvertedToLdraw));
}

double LRRCcombatClass::calculateTheDistanceBetweenTwoUnits(const double positionXOfUnit1, const double positionXOfUnit2, const double positionYOfUnit1, const double positionYOfUnit2, const double positionZOfUnit1, const double positionZOfUnit2)
{

#ifndef USE_OLD_LONG_RANGE_ATTACK_DISTANCE_CALC
	return SHAREDvector.calculateTheDistanceBetweenTwoPoints(positionXOfUnit1/LDRAW_UNITS_PER_CM, positionXOfUnit2/LDRAW_UNITS_PER_CM, positionYOfUnit1/LDRAW_UNITS_PER_CM, positionYOfUnit2/LDRAW_UNITS_PER_CM, positionZOfUnit1/LDRAW_UNITS_PER_CM, positionZOfUnit2/LDRAW_UNITS_PER_CM);
#else
	double xDistanceBetweenTheTwoUnits = fabs((positionXOfUnit1/LDRAW_UNITS_PER_CM) -  (positionXOfUnit2/LDRAW_UNITS_PER_CM));
	double zDistanceBetweenTheTwoUnits = fabs((positionZOfUnit1/LDRAW_UNITS_PER_CM) -  (positionZOfUnit2/LDRAW_UNITS_PER_CM));
	double distanceBetweenTheTwoUnits = sqrt(pow(xDistanceBetweenTheTwoUnits, 2) + pow(zDistanceBetweenTheTwoUnits, 2));
	return distanceBetweenTheTwoUnits;
#endif

}



//This function is not currently used (instead simplified calculations of range advantage are calculated based on original Rules
double calculateExtraHorizontalDistanceOfProjectileWithHeightAdvantage(double verticalHeightAdvantage, double maxHorizRangeOfWeapon)
{
	return false;
}



int LRRCcombatClass::calculateLongDistanceRangeModifier(const LDreference* targetUnitReference, const LDreference* unitReference)
{
	return (targetUnitReference->absolutePosition.y-unitReference->absolutePosition.y)/LDRAW_UNITS_PER_CM*RANGE_MODIFIER_BASED_ON_INCREASED_HEIGHT;

}














































