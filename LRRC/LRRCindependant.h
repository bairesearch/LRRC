/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: LRRCindependant.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3a11b 09-July-2012
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/



#ifndef HEADER_LRRC_INDEPENDANT
#define HEADER_LRRC_INDEPENDANT

void executeLLRCfunctionsIndependantly();

bool obtainAttackIntentionsFromUser(bool * unit1intendsToPerformAttack, bool * unit2intendsToPerformAttack);
bool obtainSceneFileNamesFromUser(char * currentSceneFileName, char * previousSceneFileName);
bool obtainSceneFileNameFromUser(char * currentSceneFileName);

//User Options	//current routines supported by backend rules checker software
/*
1. Calculate the class of a known ModelDetails file\n";
2. Perform Close Combat with two known ModelDetails files and their intentions\n";
3. Compare two consecutive scene files (to see if they represent a valid move)\n";
4. Perform Long Distance combat with two known ModelDetails files and their intentions\n";
5. Perform Advanced Close Combat with two known ModelDetails files and their intentions\n";
6. Perform Advanced Long Distance combat with two known ModelDetails files and their \n   intentions\n\n Enter Answer (1,2,3,4,5,6):";
*/

#endif




