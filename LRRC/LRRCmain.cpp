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
 * File Name: LRRCmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3n7d 17-August-2020
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#include "LRRCmain.hpp"

int main()
{
	bool UIstatus = true;

	string answerAsString = "";
	int64_t answerAsInt;

	srand((unsigned)time(NULL));	//seeds randomness

//#define TESTXMLPARSER
#ifdef TESTXMLPARSER
	LRRCindependantClass().executeLRRCfunctionsIndependantly();
#else
	while(UIstatus == true)
	{
		cout << "\n ---\n";
		cout << "Welcome To LRRC\n";
		cout << " ---\n";
		cout << "Do you wish to \n";
		cout << " ---\n";
		cout << "1. Execute LRRC functions independantly (Advanced game mode) - no GUI\n";
		cout << "2. Execute LRRC functions in order (Normal game mode) - no GUI\n";
		cout << "3. Execute LRRC functions with AI (Advanced untested game mode) - no GUI\n";
		cout << " ---\n";
		cout << "0. Exit\n";
		cout << " ---\n";
		cout <<	"Enter Answer (0,1,2,3... etc):\n\n>> ";


		cin >> answerAsString;
		answerAsInt = SHAREDvarsClass().convertStringToLong(answerAsString);

		if(answerAsInt == 1)
		{
			LRRCindependantClass().executeLRRCfunctionsIndependantly();
			UIstatus = false;
		}
		else if(answerAsInt == 2)
		{
			LRRCgameClass().executeLRRCfunctionsInOrder();
			UIstatus = false;
		}
		else if(answerAsInt == 3)
		{
			#ifdef USE_ANN
			LRRCgameClass().executeLRRCfunctionsWithAI();
			#else
			cout << "error: no ANN - no AI players allowed - must compile with ANN" << endl;
			#endif
			UIstatus = false;
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
#endif
}






