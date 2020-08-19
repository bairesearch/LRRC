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
 * File Name: LRRCgameReferenceManipulation.cpp
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


#include "LRRCgameReferenceManipulation.hpp"



/*secondary game routines*/

LDreference* LRRCgameReferenceManipulationClass::searchReferenceListRemoveReference(LDreference* initialReferenceInSceneFile, const string unitFileName, const int unitColour, bool* unitIDFound, bool* result)
{
	LDreference* modInitialReferenceInSceneFile = initialReferenceInSceneFile;

	LDreference* currentReference = initialReferenceInSceneFile;
	LDreference* previousReference = NULL;

	while(currentReference->next != NULL)
	{
		bool unitIDFoundInCurrentReference = false;

		if(currentReference->isSubModelReference)
		{
			if(LDreferenceManipulation.compareReferenceNameAndColour(currentReference, unitFileName, unitColour))
			{
				if(*unitIDFound == true)
				{
					*result = false;
					cout << "Invalid Scene File - multiple units exist with same unit name and ID (colour)" << endl;
					cout << "searchReferenceListRemoveReference, unitFileName=" << unitFileName << " unitColour=" << unitColour;
				}
				else
				{
					*unitIDFound = true;
					unitIDFoundInCurrentReference = true;

					if(previousReference != NULL)
					{
						previousReference->next = currentReference->next;
					}
					else
					{
						modInitialReferenceInSceneFile = currentReference->next;
					}

				}
			}
			else
			{
				searchReferenceListRemoveReference(currentReference->firstReferenceWithinSubModel, unitFileName, unitColour, unitIDFound, result);
			}
		}

		previousReference = currentReference;
		currentReference = currentReference->next;

		if(unitIDFoundInCurrentReference == true)
		{
			previousReference->firstReferenceWithinSubModel = NULL;
			previousReference->next = NULL;

			delete (previousReference);	//remove found unit from reference tree
		}
	}


	return modInitialReferenceInSceneFile;

}

/*
void searchReferenceListAssignHasPerformedCombat(int currentPhase, LDreference* initialReferenceInSceneFile, string unitFileName, int unitColour, bool* unitIDFound, bool* result)
{
	LDreference* currentReference = initialReferenceInSceneFile;
	while(currentReference->next != NULL)
	{
		if(currentReference->isSubModelReference)
		{
			if(compareReferenceNameAndColour(currentReference, unitFileName, unitColour))
			{
				if(*unitIDFound == true)
				{
					*result = false;
					cout << "Invalid Scene File - multiple units exist with same unit name and ID (colour)" << endl;
					cout << "searchReferenceListAssignHasPerformedCombat, unitFileName=" << unitFileName << " unitColour=" << unitColour;
				}
				else
				{
					*unitIDFound = true;
					currentReference->subModelDetails->hasPerformedAction[currentPhase] = true;
				}
			}

			searchReferenceListAssignHasPerformedCombat(currentPhase, currentReference->firstReferenceWithinSubModel, unitFileName, unitColour, unitIDFound, result);
		}

		currentReference = currentReference->next;
	}
}
*/

LDreference* LRRCgameReferenceManipulationClass::searchReferenceListFindReference(LDreference* initialReferenceInSceneFile, const string unitFileName, const int unitColour, bool* unitIDFound, bool* result)
{
	LDreference* foundReference = NULL;
	LDreference* currentReference = initialReferenceInSceneFile;
	while(currentReference->next != NULL)
	{
		if(currentReference->isSubModelReference)
		{
			if(LDreferenceManipulation.compareReferenceNameAndColour(currentReference, unitFileName, unitColour))
			{
				if(*unitIDFound == true)
				{
					*result = false;
				}
				else
				{
					*unitIDFound = true;

					foundReference = currentReference;
				}
			}

			bool tempUnitIDFound = false;
			LDreference* tempReferenceFound;
			tempReferenceFound = searchReferenceListFindReference(currentReference->firstReferenceWithinSubModel, unitFileName, unitColour, &tempUnitIDFound , result);
			if(tempUnitIDFound)
			{
				foundReference = tempReferenceFound;
				*unitIDFound = true;
			}
		}

		currentReference = currentReference->next;
	}

	return foundReference;
}


bool LRRCgameReferenceManipulationClass::obtainUserInputInt(int* userInputInt)
{
	bool result = true;

	bool UIstatus = true;
	string answerAsString = "";
	int answerAsInt;

	while(UIstatus == true)
	{
		cout << ">> ";
		cin >> answerAsString;
		answerAsInt = SHAREDvars.convertStringToInt(answerAsString);

		if(answerAsInt > 0)
		{
			*userInputInt = answerAsInt;

			UIstatus = false;
		}
		else
		{
			cout << "Invalid answer, please try again" << endl;
		}
	}

	return result;
}



bool LRRCgameReferenceManipulationClass::obtainUnitDetailsFromUserWOSceneRef(const int currentPhase, string* unit1FileName, string* unit2FileName, int* unit1ID, int* unit2ID, string sceneFileName)
{
	bool result = true;

	LDreference* initialReferenceInSceneFile = new LDreference();
	LDreference* topLevelReferenceInSceneFile = new LDreference(sceneFileName, 1, true);	//The submodel information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function

	if(!LDparser.parseFile(sceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, false))
	{//file does not exist
		cout << "The file: " << sceneFileName << " does not exist in the directory" << endl;
		result = false;
	}

	obtainUnitDetailsFromUserForCombat(unit1FileName, unit2FileName, unit1ID, unit2ID, initialReferenceInSceneFile);

#ifdef GAME_USE_COLOUR_AS_UNIQUE_UNIT_IDS
	if(!determineUnitNamesWithColours(currentPhase, unit1FileName, unit2FileName, *unit1ID, *unit2ID, initialReferenceInSceneFile))
	{
		result = false;
	}
#else

	if(!determineIfUnitsExists(currentPhase, *unit1FileName, *unit2FileName, *unit1ID, *unit2ID, initialReferenceInSceneFile))
	{
		result = false;
	}
#endif

	delete initialReferenceInSceneFile;
	delete topLevelReferenceInSceneFile;

	return result;
}

bool LRRCgameReferenceManipulationClass::obtainUnitDetailsFromUserForCombat(string* unit1FileName, string* unit2FileName, int* unit1ID, int* unit2ID, const LDreference* initialReferenceInSceneFile)
{
	bool result = true;

	string answerAsString = "";
	int64_t answerAsInt;

	cout << "Enter Unit 1 ID (Eg '1'):";
	cin >> answerAsString;
	answerAsInt = SHAREDvars.convertStringToInt(answerAsString);
	*unit1ID = answerAsInt;

	cout << "Enter Unit 2 ID (Eg '2'):";
	cin >> answerAsString;
	answerAsInt = SHAREDvars.convertStringToInt(answerAsString);
	*unit2ID = answerAsInt;


#ifndef GAME_USE_COLOUR_AS_UNIQUE_UNIT_IDS
	cout << "Enter Unit 1 Name (Eg 'unit1.ldr'):";
	cin >> *unit1FileName;

	cout << "Enter Unit 2 Name (Eg 'unit2.ldr'):";
	//cin.get(unit2FileName, 50);
	cin >> *unit2FileName;

#endif

	return result;
}


bool LRRCgameReferenceManipulationClass::determineIfUnitsExists(const int currentPhase, const string unitAttackerFileName, const string unitDefenderFileName, const int unitAttackerPlayerID, const int unitDefenderPlayerID, const LDreference* initialReferenceInSceneFile)
{
	bool result = true;

	bool unitIDFound;

	unitIDFound = false;
	parseRefListCheckUnitExists(currentPhase, initialReferenceInSceneFile, unitAttackerFileName, unitAttackerPlayerID, &unitIDFound, &result);
	if(!unitIDFound)
	{
		cout << "Problem: Combat Ready player " << unitAttackerPlayerID << " unit name " << unitAttackerFileName << " cannot be found," << endl;
		result = false;
	}
	unitIDFound = false;
	parseRefListCheckUnitExists(currentPhase, initialReferenceInSceneFile, unitDefenderFileName, unitDefenderPlayerID, &unitIDFound, &result);
	if(!unitIDFound)
	{
		cout << "Problem: Combat Ready player " << unitDefenderPlayerID << " unit name " << unitDefenderFileName << " cannot be found," << endl;
		result = false;
	}

	return result;
}

void LRRCgameReferenceManipulationClass::parseRefListCheckUnitExists(const int currentPhase, const LDreference* initialReferenceInSceneFile, const string unitFileName, const int unitColour, bool* unitIDFound, bool* result)
{
	const LDreference* currentReference = initialReferenceInSceneFile;
	while(currentReference->next != NULL)
	{
		if(currentReference->isSubModelReference)
		{
			if(LDreferenceManipulation.compareReferenceNameAndColour(currentReference, unitFileName, unitColour))
			{

				if(*unitIDFound == true)
				{
					*result = false;
					cout << "parseRefListCheckUnitExists, Invalid Scene File - multiple units exist with same unit name and ID (colour), unitFileName=" << unitFileName << " unitColour=" << unitColour << endl;
				}
				else
				{
					*unitIDFound = true;
				}
			}

			parseRefListCheckUnitExists(currentPhase, currentReference->firstReferenceWithinSubModel, unitFileName, unitColour, unitIDFound, result);
		}

		currentReference = currentReference->next;
	}
}

bool LRRCgameReferenceManipulationClass::determineUnitNamesWithColours(const int currentPhase, string* unit1FileName, string* unit2FileName, const int unit1ID, const int unit2ID, LDreference* initialReferenceInSceneFile)
{
	bool result = true;

	bool unitIDFound;
	unitIDFound = false;
	parseRefListDetRefNames(currentPhase, initialReferenceInSceneFile, unit1FileName, unit1ID, &unitIDFound, &result);
	if(!unitIDFound)
	{
		cout << "Problem: Combat Ready Unit 1 ID " << unit1ID << "not found." << endl;
		result = false;
	}
	unitIDFound = false;
	parseRefListDetRefNames(currentPhase, initialReferenceInSceneFile, unit2FileName, unit2ID, &unitIDFound, &result);
	if(!unitIDFound)
	{
		cout << "Problem: Combat Ready Unit 2 ID " << unit2ID << "not found." << endl;
		result = false;
	}

	return result;

}

void LRRCgameReferenceManipulationClass::parseRefListDetRefNames(const int currentPhase, LDreference* reference, string* referenceName, const int referenceColour, bool* unitIDFound, bool* result)
{
	LDreference* currentReference = reference;
	while(currentReference->next != NULL)
	{
		if(currentReference->isSubModelReference)
		{
			if(currentReference->colour == referenceColour)
			{
				if(*unitIDFound == true)
				{
					cout << "Invalid Scene File - multiple units exist with same unit ID (colour)" << endl;
					*result = false;
				}
				else
				{
					*unitIDFound = true;
					*referenceName = currentReference->name;
				}
			}

			parseRefListDetRefNames(currentPhase, currentReference->firstReferenceWithinSubModel, referenceName, referenceColour, unitIDFound, result);
		}
		currentReference = currentReference->next;
	}
}



void LRRCgameReferenceManipulationClass::searchReferenceListPrintReferenceDetails(const LDreference* initialReferenceInSceneFile)
{
	const LDreference* currentReference = initialReferenceInSceneFile;
	while(currentReference->next != NULL)
	{
		if(currentReference->isSubModelReference)
		{
			cout << "currentReference->name = " << currentReference->name << endl;
			/*
			cout << "currentReference->colour = " << currentReference->colour << endl;
			cout << "x rel = " << currentReference->relativePosition.x << endl;
			cout << "y rel = " << currentReference->relativePosition.y << endl;
			cout << "z rel = " << currentReference->relativePosition.z << endl;
			cout << "x abs = " << currentReference->absolutePosition.x << endl;
			cout << "y abs = " << currentReference->absolutePosition.y << endl;
			cout << "z abs = " << currentReference->absolutePosition.z << endl;
			*/
			
			searchReferenceListPrintReferenceDetails(currentReference->firstReferenceWithinSubModel);
		}

		currentReference = currentReference->next;
	}
}


