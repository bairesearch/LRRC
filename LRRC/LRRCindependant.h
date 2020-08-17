/*******************************************************************************
 *
 * File Name: LRRCindependant.h
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




