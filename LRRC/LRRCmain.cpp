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
 * File Name: LRRCmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3j1a 14-January-2017
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

int main()
{
	bool UIstatus = true;

	string answerAsString = "";
	long answerAsInt;

	srand((unsigned)time(NULL));	//seeds randomness

//#define TESTXMLPARSER
#ifdef TESTXMLPARSER
	executeLRRCfunctionsIndependantly();
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
		cout << "3. Execute LRRC functions with AI (Advanced untested game mode) - no GUI\n";
		cout << " ---\n";
		cout << "0. Exit\n";
		cout << " ---\n";
		cout <<	"Enter Answer (0,1,2,3... etc):\n\n>> ";


		cin >> answerAsString;
		answerAsInt = convertStringToLong(answerAsString);

		if(answerAsInt == 1)
		{
			executeLRRCfunctionsIndependantly();
			UIstatus = false;
		}
		else if(answerAsInt == 2)
		{
			executeLRRCfunctionsInOrder();
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






