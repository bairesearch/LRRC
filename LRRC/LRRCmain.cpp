/*******************************************************************************
 *
 * File Name: LRRCmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3a5g 01-Nov-2011
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#include "LRRCmain.h"
#include "LRRCindependant.h"
#include "LRRCgame.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;

int main()
{
	bool UIstatus = true;

	char answerAsString[100];
	long answerAsInt;

	srand( (unsigned)time(NULL) );	//seeds randomness

//#define TESTXMLPARSER
#ifdef TESTXMLPARSER
	executeLLRCfunctionsIndependantly();
#else
	while(UIstatus == true)
	{
		cout << "\n ---\n";
		cout << "Welecome To LRRC (Version 4dXy or higher)\n";
		cout << " ---\n";
		cout << "Do you wish to \n";
		cout << " ---\n";
		cout << "1. Execute LRRC functions independantly (Advanced game mode) - no GUI\n";
		cout << "2. Execute LRRC functions in order (Normal game mode) - no GUI\n";
		cout << "2. Execute LRRC functions with AI (Advanced untested game mode) - no GUI\n";
		cout << " ---\n";
		cout << "0. Exit\n";
		cout << " ---\n";
		cout <<	"Enter Answer (0,1,2,3... etc):\n\n>> ";


		cin >> answerAsString;
		answerAsInt = long(atof(answerAsString));

		if(answerAsInt == 1)
		{
			executeLLRCfunctionsIndependantly();
			UIstatus = false;
		}
		else if(answerAsInt == 2)
		{
			executeLLRCfunctionsInOrder();
			UIstatus = false;
		}
		else if(answerAsInt == 3)
		{
			#ifdef USE_ANN
			executeLRRCfunctionsWithAI();
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






