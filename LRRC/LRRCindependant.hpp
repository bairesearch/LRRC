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
 * File Name: LRRCindependant.hpp
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


#ifndef HEADER_LRRC_INDEPENDANT
#define HEADER_LRRC_INDEPENDANT

#include "LRRCglobalDefs.hpp"
#include "LRRCmovement.hpp"
#include "LRRCcombat.hpp"
#include "LRRCsprite.hpp"
#include "LDreferenceManipulation.hpp"
#include "LRRCgameReferenceManipulation.hpp"
#include "LRRCplayerClass.hpp"
#include "LRRCgame.hpp"
#include "LRRCrules.hpp"
#include "LRRCparser.hpp"
#include "LRRCsprite.hpp"	//required for extern variable initialisation only
#include "LDparser.hpp"	//is this required?

class LRRCindependantClass
{
	private: LRRCmovementClass LRRCmovement;
	private: SHAREDvarsClass SHAREDvars;
	private: LRRCmodelClassClass LRRCmodelClass;
	private: LRRCrulesClass LRRCrules;
	private: LRRCplayerClassClass LRRCplayerClass;
	private: LRRCspriteClass LRRCsprite;
	private: LRRCcombatClass LRRCcombat;
	private: LRRCparserClass LRRCparser;
	private: LRRCgameReferenceManipulationClass LRRCgameReferenceManipulation;
	private: LRRCgameClass LRRCgame;
	public: void executeLRRCfunctionsIndependantly();

	private: bool obtainAttackIntentionsFromUser(bool* unit1intendsToPerformAttack, bool* unit2intendsToPerformAttack);
	private: bool obtainSceneFileNamesFromUser(string* currentSceneFileName, string* previousSceneFileName);
	private: bool obtainSceneFileNameFromUser(string* currentSceneFileName);
};

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




