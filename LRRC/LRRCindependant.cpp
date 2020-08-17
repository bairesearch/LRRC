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
 * File Name: LRRCindependant.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3i19c 15-December-2016
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


/*
/
/
/ Imminent Improvements: Implement these algorithms:
/
/ Release Notes:
/ 1. Assumptions: scene files and corresponding unit files are in current directory
/ ??? 2. unit files are generally not embedded within each other (this version of the rules checker cannot however parse special embedded unit files.
/		The following expections are allowed
/		a) a boat/vehicle file which includes separate units can be parsed.
/		[PENDING future coding: b) a Knight file which includes a separate horse and footsoldier file may be parsable]
/ ??? 3. General rule for parsing:
/		a) movements of 'units' (ie units that may contain sub units) are calculated based upon the unitType of the highest level unit (the unit that defines the group or sub units)
/		b) Combat (attack/defence rolls) of units that i) numPerson (ie the lower level units if grouped), ii) or is of unitType UNIT_TYPE_WARMACHINE
/ ??? 4. Modifications of rules for C++ version of code
/		a) this code currently does not have the ability to give mounted men an advantage when using ordinary weapons (ie non lances) if embedded units are supported (eg man within horse)
/			[Therefore the current Rules (version 1bxx, #12 / 2006) need to be changed to document this]
/
/ Version History:
/ 12-9-05	legoRulesRoundChecker.cpp Project Parser Modifications (inheritance differences)
/ 11-9-05	legoRulesRoundChecker.cpp Project Start	[only uses C functionailty]
/ 18-9-05	legoRulesRoundChecker.cpp Project Finished and debugged
/ 4-3-06	legoRulesRoundChecker.cpp Project simplified longo components [LRRCmain.cpp, scene.cpp, LRRCcombat.cpp, ModelDetails.cpp, building.cpp etc]
/ 4/5-3-06	legoRulesRoundChecker.cpp Project converted to C++
/ X-3-06	legoRulesRoundChecker.cpp Project Debugged
/
/ ...
/
/ Project First Internal Release: 1aXx 18-Sept-05 (C)
/ Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
/ Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
/ Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
/ Project Fifth Internal Release: 2d11a 3-June-07
*/

#include "LRRCindependant.h"
#include "LRRCmovement.h"
#include "LRRCcombat.h"
#include "LRRCsprite.h"
#include "LDreferenceManipulation.h"
#include "LRRCgameReferenceManipulation.h"
#include "LRRCplayerClass.h"
#include "LRRCgame.h"
#include "LRRCrules.h"
#include "LRRCparser.h"
#include "LRRCsprite.h"	//required for extern variable initialisation only

#include "LDparser.h"	//is this required?

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;

void executeLRRCfunctionsIndependantly()
{
	bool UIstatus = true;

	string answerAsString = "";
	long answerAsInt;

	if(!parseLRRCrulesXMLfile())
	{
		UIstatus = false;
	}
	fillInCombatExternVariables();
	fillInGameExternVariables();
	fillInModelClassExternVariables();
	fillInParserExternVariables();
	fillInPlayerClassExternVariables();
	fillInLRRCSpriteExternVariables();

	while(UIstatus == true)
	{
		cout << "\n ---\n";
		cout << "Do you wish to \n";
		cout << " ---\n";
		cout << "1. Compare two consecutive scene files (to see if they represent a valid move)\n";
		cout << " ---\n";
		cout << "2. Close Combat with two basic unit files\n";
		cout << "3. Long Distance combat with two basic unit files\n";
		cout << "4. Close Combat with two unit files\n";
		cout << "5. Long Distance Combat with two unit files\n";
		cout << "6. Close Combat with two unit files, their intentions, and a scene file\n";
		cout << "7. Long Distance with two unit files, their intentions, and a scene file\n";
		cout << "8. Close Combat with two unit files, their intentions, and consecutive scene files\n";
		cout << "9. Long Distance with two unit files, their intentions, and consecutive scene files\n";
		cout << " ---\n";
		cout << "10. Calculate the class of a basic unit file\n";
		cout << "11. Calculate the class of a unit file\n";
		cout << " ---\n";
		cout << "12. Add sprites to scene file\n";
		cout << " ---\n";
		cout << "0. Exit\n";
		cout << " ---\n";
		cout <<	"Enter Answer (0,1,2,3... etc):\n\n>> ";


		cin >> answerAsString;
		long answerAsInt = convertStringToLong(answerAsString);

		int unit1ID;
		int unit2ID;
		char unit1FileName[50];
		char unit2FileName[50];
		char preMovementPhaseSceneFileName[50];
		char thisPhaseStartSceneFileName[50];
		char currentSceneFileName[50];
		char previousSceneFileName[50];
		char sceneFileWithSpritesName[50];
		bool unit1intendsToPerformAttack;
		bool unit2intendsToPerformAttack;

		if(answerAsInt == 1)
		{
			cout << "To Compare two consecutive scene files, and see if the newer scene represents a valid move, please enter both the old and the new scene file names\n\n";

			obtainSceneFileNamesFromUser(thisPhaseStartSceneFileName, preMovementPhaseSceneFileName);

			Player* currentPlayer = new Player();
			currentPlayer->name = "Player1";
			currentPlayer->id = 1;
			currentPlayer->credits = PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT;

			compareSceneFilesMovementPhase(preMovementPhaseSceneFileName, thisPhaseStartSceneFileName, currentPlayer, thisPhaseStartSceneFileName, false);
		}
		else if(answerAsInt == 2)
		{
			cout << "To Perform close combat, and see which unit wins, please enter both the unit file names, and each units intentions\n\n";

			cout << "Enter Unit 1 Name (Eg 'unit1.ldr'):";
			cin >> unit1FileName;

			cout << "Enter Unit 2 Name (Eg 'unit2.ldr'):";
			cin >> unit2FileName;

			obtainAttackIntentionsFromUser(&unit1intendsToPerformAttack, &unit2intendsToPerformAttack);

			performCloseCombatBasic(unit1FileName, unit2FileName, unit1intendsToPerformAttack, unit2intendsToPerformAttack, true, true);

		}
		else if(answerAsInt == 3)
		{
			cout << "To Perform long distance combat, and see which unit wins, please enter both the unit file names, and each units intentions\n\n";

			cout << "Enter Unit 1 Name (Eg 'unit1.ldr'):";
			cin >> unit1FileName;

			cout << "Enter Unit 2 Name (Eg 'unit2.ldr'):";
			cin >> unit2FileName;

			obtainAttackIntentionsFromUser(&unit1intendsToPerformAttack, &unit2intendsToPerformAttack);

			performLongDistanceCombatBasic(unit1FileName, unit2FileName, unit1intendsToPerformAttack, unit2intendsToPerformAttack);

		}
		else if(answerAsInt == 4)
		{
			cout << "To Perform close combat, and see which unit wins, please enter both the unit file names, and each units intentions\n\n";

			cout << "Enter Unit 1 Name (Eg 'unit1.ldr'):";
			cin >> unit1FileName;

			cout << "Enter Unit 2 Name (Eg 'unit2.ldr'):";
			cin >> unit2FileName;

			obtainAttackIntentionsFromUser(&unit1intendsToPerformAttack, &unit2intendsToPerformAttack);

			performCloseCombatNormal(unit1FileName, unit2FileName, unit1intendsToPerformAttack, unit2intendsToPerformAttack);

		}
		else if(answerAsInt == 5)
		{
			cout << "To Perform long distance combat, and see which unit wins, please enter both the unit file names, and each units intentions\n\n";

			cout << "Enter Unit 1 Name (Eg 'unit1.ldr'):";
			cin >> unit1FileName;

			cout << "Enter Unit 2 Name (Eg 'unit2.ldr'):";
			cin >> unit2FileName;

			obtainAttackIntentionsFromUser(&unit1intendsToPerformAttack, &unit2intendsToPerformAttack);

			performLongDistanceCombatNormal(unit1FileName, unit2FileName, unit1intendsToPerformAttack, unit2intendsToPerformAttack);

		}
		else if(answerAsInt == 6)
		{
			cout << "To Perform close combat, and see which unit wins, please enter both the unit file names, their intentions, and the current scene file name \n\n";

			obtainSceneFileNameFromUser(currentSceneFileName);
			if(obtainUnitDetailsFromUserWOSceneRef(GAME_PHASE_CLOSECOMBAT, unit1FileName, unit2FileName, &unit1ID, &unit2ID, currentSceneFileName))
			{
				obtainAttackIntentionsFromUser(&unit1intendsToPerformAttack, &unit2intendsToPerformAttack);

				performCloseCombatWithSceneFile(unit1FileName, unit2FileName, unit1ID, unit2ID, unit1intendsToPerformAttack, unit2intendsToPerformAttack, currentSceneFileName);
			}

		}
		else if(answerAsInt == 7)
		{
			cout << "To Perform long distance combat, and see which unit wins, please enter both the unit file names, each units intentions, and the current scene file name \n\n";

			obtainSceneFileNameFromUser(currentSceneFileName);
			if(obtainUnitDetailsFromUserWOSceneRef(GAME_PHASE_LONGDISTANCECOMBAT, unit1FileName, unit2FileName, &unit1ID, &unit2ID, currentSceneFileName))
			{
				obtainAttackIntentionsFromUser(&unit1intendsToPerformAttack, &unit2intendsToPerformAttack);

				performLongDistanceCombatWithSceneFile(unit1FileName, unit2FileName, unit1ID, unit2ID, unit1intendsToPerformAttack, unit2intendsToPerformAttack, currentSceneFileName);
			}
		}
		else if(answerAsInt == 8)
		{
			cout << "To Perform close combat, and see which unit wins, please enter both the unit file names, their intentions, the previous scene file name, and the current scene file name \n\n";

			obtainSceneFileNamesFromUser(currentSceneFileName, previousSceneFileName);
			if(obtainUnitDetailsFromUserWOSceneRef(GAME_PHASE_CLOSECOMBAT, unit1FileName, unit2FileName, &unit1ID, &unit2ID, currentSceneFileName))
			{
				obtainAttackIntentionsFromUser(&unit1intendsToPerformAttack, &unit2intendsToPerformAttack);

				performCloseCombatWithConsecutiveSceneFiles(unit1FileName, unit2FileName, unit1ID, unit2ID, unit1intendsToPerformAttack, unit2intendsToPerformAttack, currentSceneFileName, previousSceneFileName);

			}
		}
		else if(answerAsInt == 9)
		{
			cout << "To Perform long distance combat, and see which unit wins, please enter both the unit file names, each units intentions, the previous scene file name, and the current scene file name \n\n";

			obtainSceneFileNamesFromUser(currentSceneFileName, previousSceneFileName);
			if(obtainUnitDetailsFromUserWOSceneRef(GAME_PHASE_LONGDISTANCECOMBAT, unit1FileName, unit2FileName, &unit1ID, &unit2ID, currentSceneFileName))
			{
				obtainAttackIntentionsFromUser(&unit1intendsToPerformAttack, &unit2intendsToPerformAttack);

				performLongDistanceCombatWithConsecutiveSceneFiles(unit1FileName, unit2FileName, unit1ID, unit2ID, unit1intendsToPerformAttack, unit2intendsToPerformAttack, currentSceneFileName, previousSceneFileName);

			}
		}
		else if(answerAsInt == 10)
		{
			cout << "Enter unit File Name (Eg 'unit.ldr'):";
			//cin.get(unitFileName, 50);
			cin >> unit1FileName;

			//final calculations depend on precise version of 'Lego Rules' (and corresponding DEFINITIONS above)
			//version 12 (2005k)

			calculateUnitClassBasic(unit1FileName);
		}
		else if(answerAsInt == 11)
		{
			cout << "Enter unit File Name (Eg 'unit.ldr'):";
			//cin.get(unitFileName, 50);
			cin >> unit1FileName;

			//final calculations depend on precise version of 'Lego Rules' (and corresponding DEFINITIONS above)
			//version 12 (2005k)

			calculateUnitClassNormal(unit1FileName);
		}
		else if(answerAsInt == 12)
		{
			cout << "Enter scene File Name (Eg 'scene.ldr'):";
			cin >> preMovementPhaseSceneFileName;

			cout << "Enter scene File (with Sprites) Name (Eg 'sceneWithSprites.ldr'):";
			cin >> sceneFileWithSpritesName;

			bool addTextualSpriteInfo = true;
			bool addRangeSpriteInfo = true;
			int currentPhase = GAME_PHASE_GENERIC;
			int currentPlayerTurn = GAME_PLAYER_TURN_DEFAULT;
			LRRCaddUnitDetailsSpritesToSceneFile(preMovementPhaseSceneFileName, sceneFileWithSpritesName, addTextualSpriteInfo, addRangeSpriteInfo, currentPhase, currentPlayerTurn);
		}
		else if(answerAsInt == 0)
		{
			UIstatus = false;
		}
		else
		{
			cout << "\nInvalid answerAsInt, please try another command\n" << endl;
		}
	}
}

bool obtainAttackIntentionsFromUser(bool* unit1intendsToPerformAttack, bool* unit2intendsToPerformAttack)
{
	bool result = true;

	cout << "Enter Unit 1 Attack Intensions ('y' to fire, or 'n'):";
	char unit1intendsToPerformAttackAnswer;
	cin >> unit1intendsToPerformAttackAnswer;

	cout << "Enter Unit 2 Attack Intensions ('y' to fire, or 'n'):";
	char unit2intendsToPerformAttackAnswer;
	//cin.get(unit2FileName, 50);
	cin >> unit2intendsToPerformAttackAnswer;

	if(unit1intendsToPerformAttackAnswer == 'y')
	{
		*unit1intendsToPerformAttack = true;
	}
	else if(unit1intendsToPerformAttackAnswer == 'n')
	{
		*unit1intendsToPerformAttack = false;
	}
	else
	{
		cout << "\nInvalid answerAsInt, please run the program again to restart\n" << endl;
		exit(0);
	}

	if(unit2intendsToPerformAttackAnswer == 'y')
	{
		*unit2intendsToPerformAttack = true;
	}
	else if(unit2intendsToPerformAttackAnswer == 'n')
	{
		*unit2intendsToPerformAttack = false;
	}
	else
	{
		cout << "\nInvalid answerAsInt, please run the program again to restart\n" << endl;
		result = false;
	}

	return result;
}

bool obtainSceneFileNamesFromUser(char* currentSceneFileName, char* previousSceneFileName)
{
	bool result = true;

	cout << "Enter Current Scene File (Eg 'scene.ldr'):";
	//cin.get(unit2FileName, 50);
	cin >> currentSceneFileName;

	cout << "Enter Previous Scene File (Eg 'scene.ldr'):";
	//cin.get(unit2FileName, 50);
	cin >> previousSceneFileName;

	return result;
}

bool obtainSceneFileNameFromUser(char* currentSceneFileName)
{
	bool result = true;

	cout << "Enter Current Scene File (Eg 'scene.ldr'):";
	//cin.get(unit2FileName, 50);
	cin >> currentSceneFileName;

	return result;
}
