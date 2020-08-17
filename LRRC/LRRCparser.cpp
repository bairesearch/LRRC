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
 * File Name: LRRCparser.cpp
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


#include "LRRCparser.h"
#include "LRRCmodelClass.h"
#include "LRRCrules.h"


#ifdef USE_LRRC
int HAND_DAGGER_MOD;

void fillInParserExternVariables()
{
	XMLrulesClass* currentReferenceRulesClass = LRRCrulesMiscellaneous;

	while(currentReferenceRulesClass->next != NULL)
	{
		if(currentReferenceRulesClass->name == HAND_DAGGER_MOD_NAME)
		{
			HAND_DAGGER_MOD = int(currentReferenceRulesClass->fractionalValue);
		}
		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
}
	//white space removal required for messy dat files from ldraw parts library


void updateUnitDetails(string subPartFileName, ModelDetails* u)
{
	updateUnitDetailsWithTypeDetails(subPartFileName, u);
	updateUnitDetailsWithCombatDetails(subPartFileName, u);
	updateUnitDetailsWithBuildingDetails(subPartFileName, u);		//this is currently not needed here - but could be used in the future for battlment protection rules during combat.
	updateUnitDetailsWithTerrainDefenceDetails(subPartFileName, u);
}

void updateUnitDetailsWithCombatDetails(string subPartFileName, ModelDetails* u)
{
	/*defence levels
	breast
	0: none			[=> leather mod]
	1: leather		[=> leather mod]
	2: crest		[=> chain mail mod]
	3: chain mail	[=> chain mail mod]
	4: metal		[=> metal mod]
	5: plate		[=> metal mod]
		these currently equate to either metal, chainmail, or leather armour modifications

	shield:
	0: none			[=> no shield mod]
	1: small		[=> shield mod]
	2: large		[=> shield mod]
		these currently equate to either shield or no shield armour modifications

	helmet:
	0: none						[=> no helmet mod]
	1: hat						[=> hat helmet mod (may be same as chaimail helmet mod depending upon ROBIN_HATS_PROVIDE_HELMET_MOD]
	2: neckprot/chinguard		[=> chainmail helmet mod (may be same as ]
	3: metal helmet				[=> metal helmet mod]
	4: plate helmet				[=> metal helmet mod]
		these currently equate to either shield or no shield armour modifications
	*/

	//////////////////////////////
	//defence level Modification
	//////////////////////////////


	XMLrulesClass* currentReferenceRulesClass;
	RecordClass* currentReferenceRecordClass;



	//Helmet Defence
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceHead;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsDefenceHead;

	//assumes recordClass linked list already has the same number of references as the XMLrulesClass linked list
	while(currentReferenceRulesClass->next != NULL)
	{
		if(strncmp(subPartFileName.c_str(), currentReferenceRulesClass->stringValue.c_str(), int(currentReferenceRulesClass->fractionalValue)) == 0)
		{
			currentReferenceRecordClass->numberOfThisPartIDInTheUnit = currentReferenceRecordClass->numberOfThisPartIDInTheUnit + 1;
		}
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
	}

	/*
	else if(strncmp(subPartFileName, HEAD_CHIN_GUARD, HEAD_CHIN_GUARD_LEN) == 0)
	{//Castle Helmet with Chin-Guard
		u->helmetDefenceValue = HEAD_CHIN_GUARD_MOD;
	}
	else if(strncmp(subPartFileName, HEAD_NECK_PROTECTOR, HEAD_NECK_PROTECTOR_LEN) == 0)
	{//Castle Helmet with Neck Protector
		u->helmetDefenceValue = HEAD_NECK_PROTECTOR_MOD;
	}

	else if(strncmp(subPartFileName, HEAD_ROBIN, HEAD_ROBIN_LEN) == 0)
	{
		u->helmetDefenceValue = HEAD_ROBIN_MOD;
	}

	else if(strncmp(subPartFileName, HEAD_PLATE, HEAD_PLATE_LEN) == 0)
	{
		u->helmetDefenceValue = HEAD_PLATE_MOD;
	}


	else if(strncmp(subPartFileName, HEAD_CROWN, HEAD_CROWN_LEN) == 0)
	{
		u->helmetDefenceValue = HEAD_CROWN_MOD;
	}
	*/




	//torso Defence
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceTorso;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsDefenceTorso;

	//assumes recordClass linked list already has the same number of references as the XMLrulesClass linked list
	while(currentReferenceRulesClass->next != NULL)
	{
		if(strncmp(subPartFileName.c_str(), currentReferenceRulesClass->stringValue.c_str(), int(currentReferenceRulesClass->fractionalValue)) == 0)
		{
			currentReferenceRecordClass->numberOfThisPartIDInTheUnit = currentReferenceRecordClass->numberOfThisPartIDInTheUnit + 1;
		}
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
	}



	/*
	//moderate armour
	else if(strncmp(subPartFileName, TORSO_CHAINMAIL, TORSO_CHAINMAIL_LEN) == 0)
	{//standard chain mail armour
		u->breastDefenceValue = TORSO_CHAINMAIL_MOD; //default
	}
	else if(strncmp(subPartFileName, TORSO_LION_CREST_A, TORSO_LION_CREST_A_LEN) == 0)
	{//Lion Crest Lion Head
		u->breastDefenceValue = TORSO_LION_CREST_A_MOD;
	}
	else if(strncmp(subPartFileName, TORSO_LION_CREST_B, TORSO_LION_CREST_B_LEN) == 0)
	{//Lion Crest Pikes
		u->breastDefenceValue = TORSO_LION_CREST_B_MOD;
	}
	else if(strncmp(subPartFileName, TORSO_ROYAL_CHAINMAIL, TORSO_ROYAL_CHAINMAIL_LEN) == 0)
	{//Royal Chainamil
		u->breastDefenceValue = TORSO_ROYAL_CHAINMAIL_MOD;
	}
	else if(strncmp(subPartFileName, TORSO_ROYAL_CREST, TORSO_ROYAL_CREST_LEN) == 0)
	{//Royal Crest
		u->breastDefenceValue = TORSO_ROYAL_CREST_MOD;
	}
	else if(strncmp(subPartFileName, TORSO_EAGLE_CREST, TORSO_EAGLE_CREST_LEN) == 0)
	{//Eagle Crest
		u->breastDefenceValue = TORSO_EAGLE_CREST_MOD;
	}
	else if(strncmp(subPartFileName, TORSO_DRAGON_CREST, TORSO_DRAGON_CREST_LEN) == 0)
	{//Dragon Crest
		u->breastDefenceValue = TORSO_DRAGON_CREST_MOD;
	}
	//low armour
	else if(strncmp(subPartFileName, TORSO_PLAIN_LADY, TORSO_PLAIN_LADY_LEN) == 0)
	{//none
		u->breastDefenceValue = TORSO_PLAIN_LADY_MOD;
	}
	else if(strncmp(subPartFileName, TORSO_ROBIN_A, TORSO_ROBIN_A_LEN) == 0)
	{//none
		u->breastDefenceValue = TORSO_ROBIN_A_MOD;
	}
	else if(strncmp(subPartFileName, TORSO_ROBIN_B, TORSO_ROBIN_B_LEN) == 0)
	{//none
		u->breastDefenceValue = TORSO_ROBIN_B_MOD;
	}
	else if(strncmp(subPartFileName, TORSO_ROBIN_C, TORSO_ROBIN_C_LEN) == 0)
	{//none
		u->breastDefenceValue = TORSO_ROBIN_C_MOD;
	}
	else if(strncmp(subPartFileName, TORSO_ROBIN_D, TORSO_ROBIN_D_LEN) == 0)
	{//none
		u->breastDefenceValue = TORSO_ROBIN_D_MOD;
	}
	//heavy armour
	else if(strncmp(subPartFileName, TORSO_METAL, TORSO_METAL_LEN) == 0)
	{//metal
		u->breastDefenceValue = TORSO_METAL_MOD;
	}
	else if(strncmp(subPartFileName, NECK_PLATE, NECK_PLATE_LEN) == 0)
	{//plate
		//u->breastDefenceValue = NECK_PLATE_MOD;		//This is applied in performFinalUnitClassCalculations() as breastDefenceValue may be overwritten at this time by torso breastdefence value.
		u->numPlateNeck++;
	}
	//special character
	else if(strncmp(subPartFileName, TORSO_SKELETON, TORSO_SKELETON_LEN) == 0)
	{//numSkeleton
		u->numSkeleton++;
	}
	else if(strncmp(subPartFileName, TORSO_GHOST, TORSO_GHOST_LEN) == 0)
	{//numGhost
		u->numGhost++;
	}
	*/



	//shield Defence
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceShield;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsDefenceShield;

	//assumes recordClass linked list already has the same number of references as the XMLrulesClass linked list
	while(currentReferenceRulesClass->next != NULL)
	{
		if(strncmp(subPartFileName.c_str(), currentReferenceRulesClass->stringValue.c_str(), int(currentReferenceRulesClass->fractionalValue)) == 0)
		{
			currentReferenceRecordClass->numberOfThisPartIDInTheUnit = currentReferenceRecordClass->numberOfThisPartIDInTheUnit + 1;
		}
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
	}



	/*
	if(strncmp(subPartFileName, SHIELD_TRIANGULAR, SHIELD_TRIANGULAR_LEN) == 0)
		//if(strcmp(subPartFileName, SHIELD_TRIANGULAR) == 0)
	{//Shield Triangular
		u->shieldDefenceValue = SHIELD_TRIANGULAR_MOD;
		u->numShields++;
	}
	else if(strncmp(subPartFileName, SHIELD_OVAL, SHIELD_OVAL_LEN) == 0)
	{//Shield Ovoid
		u->shieldDefenceValue = SHIELD_OVAL_MOD;
		u->numShields++;
	}
	*/






	//////////////////////////////
	//attack level modification
	//////////////////////////////
	//only primary weapons (Eg sword/axe) immediatly effect the attack level modifier, secondary weapons (Eg longrange + lance) effect the modifier at a later stage

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackCloseCombat;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsAttackCloseCombat;

	//assumes recordClass linked list already has the same number of references as the XMLrulesClass linked list
	while(currentReferenceRulesClass->next != NULL)
	{
		if(strncmp(subPartFileName.c_str(), currentReferenceRulesClass->stringValue.c_str(), int(currentReferenceRulesClass->fractionalValue)) == 0)
		{
			currentReferenceRecordClass->numberOfThisPartIDInTheUnit = currentReferenceRecordClass->numberOfThisPartIDInTheUnit + 1;
		}
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
	}

	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
	currentReferenceRecordClass = u->recordOfUnitCombatDetailsAttackLongDistance;

	//assumes recordClass linked list already has the same number of references as the XMLrulesClass linked list
	while(currentReferenceRulesClass->next != NULL)
	{
		if(strncmp(subPartFileName.c_str(), currentReferenceRulesClass->stringValue.c_str(), int(currentReferenceRulesClass->fractionalValue)) == 0)
		{
			currentReferenceRecordClass->numberOfThisPartIDInTheUnit = currentReferenceRecordClass->numberOfThisPartIDInTheUnit + 1;
		}
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
	}


	/*
	else if(strncmp(subPartFileName, HAND_LANCE, HAND_LANCE_LEN) == 0)
	{
		u->numLance++;
		//cout << "DEBUG: lance found" << endl;
	}
	else if(strncmp(subPartFileName, HAND_SWORD, HAND_SWORD_LEN) == 0)
	{
		u->numSword++;
	}
	else if(strncmp(subPartFileName, HAND_LARGESWORD, HAND_LARGESWORD_LEN) == 0)
	{
		u->numLargeSword++;
	}
	else if(strncmp(subPartFileName, HAND_BOW, HAND_BOW_LEN) == 0)
	{
		u->numLongBow++;
	}
	else if(strncmp(subPartFileName, HAND_CROSS_BOW, HAND_CROSS_BOW_LEN) == 0)
	{
		u->numCrossBow++;
	}
	else if(strncmp(subPartFileName, HAND_HANDAXE, HAND_HANDAXE_LEN) == 0)
	{
		u->numHandAxe++;
	}
	else if(strncmp(subPartFileName, HAND_AXE, HAND_AXE_LEN) == 0)
	{
		u->numAxe++;
	}
	else if(strncmp(subPartFileName, HAND_LARGEAXE, HAND_LARGEAXE_LEN) == 0)
	{
		u->numLargeAxe++;
	}
	else if(strncmp(subPartFileName, HAND_SPEAR, HAND_SPEAR_LEN) == 0)
	{
		u->numSpear++;
	}
	*/




	//miscellaneous modifiers
	//u.hasCape.. ? , u.hasBucket.. ?
}

void updateUnitDetailsWithTypeDetails(string subPartFileName, ModelDetails* u)
{
	//////////////////////////////
	//ModelDetails Type modification
	//////////////////////////////


	XMLrulesClass* currentReferenceRulesClass = LRRCrulesUnitTypeDetails;
	RecordClass* currentReferenceRecordClass = u->recordOfUnitTypeDetails;

	//assumes recordClass linked list already has the same number of references as the XMLrulesClass linked list
	while(currentReferenceRulesClass->next != NULL)
	{
		if(strncmp(subPartFileName.c_str(), currentReferenceRulesClass->stringValue.c_str(), int(currentReferenceRulesClass->fractionalValue)) == 0)
		{
			currentReferenceRecordClass->numberOfThisPartIDInTheUnit = currentReferenceRecordClass->numberOfThisPartIDInTheUnit + 1;
		}
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
	}

	/*
	if(strncmp(subPartFileName, PERSON_HEAD, PERSON_HEAD_LEN) == 0)
	{
		u->numPerson++;
		//cout << "DEBUG: u->numPerson = true" << endl;
	}
	else if(strncmp(subPartFileName, SADDLE, SADDLE_LEN) == 0)
	{
		u->numSaddle++;
	}
	else if(strncmp(subPartFileName, HORSE, HORSE_LEN) == 0)
	{
		u->numHorse++;
	}
	else if(strncmp(subPartFileName, BOAT_SMALL_HULL, BOAT_SMALL_HULL_LEN) == 0)
	{
		u->numSmallHull++;
	}
	else if(strncmp(subPartFileName, BOAT_LARGE_HULL, BOAT_LARGE_HULL_LEN) == 0)
	{
		u->numLargeHull++;
	}
	else if(strncmp(subPartFileName, VEHICLE_SMALL_WHEEL, VEHICLE_SMALL_WHEEL_LEN) == 0)
	{
		u->numSmallWheel++;
	}
	else if(strncmp(subPartFileName, VEHICLE_LARGE_WHEEL, VEHICLE_LARGE_WHEEL_LEN) == 0)
	{
		u->numLargeWheel++;
	}
	else if(strncmp(subPartFileName, VEHICLE_WHEEL_HOLDER, VEHICLE_WHEEL_HOLDER_LEN) == 0)
	{
		u->numWheelHolder++;
	}
	else if(strncmp(subPartFileName, VEHICLE_WHEEL_HOLDER_DUAL, VEHICLE_WHEEL_HOLDER_DUAL_LEN) == 0)
	{
		u->numWheelHolderDual++;
	}
	else if(strncmp(subPartFileName, VEHICLE_HORSE_HITCH, VEHICLE_HORSE_HITCH_LEN) == 0)
	{
		u->numHorseHitch++;
	}
	else if(strncmp(subPartFileName, VEHICLE_HORSE_HITCH_WITH_HINGE, VEHICLE_HORSE_HITCH_WITH_HINGE_LEN) == 0)
	{
		u->numHorseHitchWithHinge++;
	}
	else if(strncmp(subPartFileName, VEHICLE_HORSE_HITCH_HINGE_ROTATABLE, VEHICLE_HORSE_HITCH_HINGE_ROTATABLE_LEN) == 0)
	{
		u->numHorseHitchHingeRotatable++;
	}
	else if(strncmp(subPartFileName, CATAPULT_BUCKET, CATAPULT_BUCKET_LEN) == 0)
	{
		u->numCatapultBucket++;
	}
	else if(strncmp(subPartFileName, CATAPULT_AXEL, CATAPULT_AXEL_LEN) == 0)
	{
		u->numCatapultAxel++;
	}
	else
	{
		//no notification made in fileDetails object
	}
	*/
}




void updateUnitDetailsWithTerrainDefenceDetails(string subPartFileName, ModelDetails* u)
{
	XMLrulesClass* currentReferenceRulesClass = LRRCrulesTerrainDetails;
	while(currentReferenceRulesClass->next != NULL)
	{
		if(strncmp(subPartFileName.c_str(), currentReferenceRulesClass->stringValue.c_str(), int(currentReferenceRulesClass->fractionalValue)) == 0)
		{
			u->numBush++;
		}
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	/*
	if(strncmp(subPartFileName, PLANT_TREE_PINE_LARGE, PLANT_TREE_PINE_LARGE_LEN) == 0)
	{
		u->numBush++;
	}
	else if(strncmp(subPartFileName, PLANT_TREE_PINE_SMALL, PLANT_TREE_PINE_SMALL_LEN) == 0)
	{
		u->numBush++;
	}
	else if(strncmp(subPartFileName, PLANT_TREE_FRUIT, PLANT_TREE_FRUIT_LEN) == 0)
	{
		u->numBush++;
	}
	else if(strncmp(subPartFileName, PLANT_BUSH, PLANT_BUSH_LEN) == 0)
	{
		u->numBush++;
	}
	*/
}


void updateUnitDetailsWithBuildingDetails(string subPartFileName, ModelDetails* u)
{
	bool foundBuildingPartInList = false;

	XMLrulesClass* currentReferenceRulesClass = LRRCrulesBuildingDetails;
	RecordClass* currentReferenceRecordClass = u->recordOfBuildingDetails;

	//assumes recordClass linked list already has the same number of references as the XMLrulesClass linked list
	while(currentReferenceRulesClass->next != NULL)
	{
		if(strncmp(subPartFileName.c_str(), currentReferenceRulesClass->stringValue.c_str(), int(currentReferenceRulesClass->fractionalValue)) == 0)
		{
			currentReferenceRecordClass->numberOfThisPartIDInTheUnit = currentReferenceRecordClass->numberOfThisPartIDInTheUnit + 1;

			foundBuildingPartInList = true;
		}
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
	}


	if(!foundBuildingPartInList)
	{
		u->numBuildingOther++;
	}

	/*
	if(strncmp(subPartFileName, BUILDING_WALL_WINDOW, BUILDING_WALL_WINDOW_LEN) == 0)
	{
		u->numBuildingWallWindow++;
	}
	else if(strncmp(subPartFileName, BUILDING_WALL_CORNER, BUILDING_WALL_CORNER_LEN) == 0)
	{
		u->numBuildingWallCorner++;
	}
	else if(strncmp(subPartFileName, BUILDING_BATTLEMENT_TOWER, BUILDING_BATTLEMENT_TOWER_LEN) == 0)
	{
		u->numBuildingBattlementTower++;
	}
	else if(strncmp(subPartFileName, BUILDING_BRICK_1X1, BUILDING_BRICK_1X1_LEN) == 0)
	{
		u->numBuildingBricks = u->numBuildingBricks+1;
	}
	else if(strncmp(subPartFileName, BUILDING_BRICK_1X2, BUILDING_BRICK_1X2_LEN) == 0)
	{
		u->numBuildingBricks = u->numBuildingBricks+2;
	}
	else if(strncmp(subPartFileName, BUILDING_BRICK_1X3, BUILDING_BRICK_1X3_LEN) == 0)
	{
		u->numBuildingBricks = u->numBuildingBricks+3;
	}
	else if(strncmp(subPartFileName, BUILDING_BRICK_1X4, BUILDING_BRICK_1X4_LEN) == 0)
	{
		u->numBuildingBricks = u->numBuildingBricks+4;
	}
	else if(strncmp(subPartFileName, BUILDING_BRICK_1X6, BUILDING_BRICK_1X6_LEN) == 0)
	{
		u->numBuildingBricks = u->numBuildingBricks+6;
	}
	else if(strncmp(subPartFileName, BUILDING_ROCK_SMALL, BUILDING_ROCK_SMALL_LEN) == 0)
	{
		u->numBuildingRockSmall = u->numBuildingRockSmall+1;
	}
	else if(strncmp(subPartFileName, BUILDING_ROCK_SMALL, BUILDING_ROCK_SMALL_LEN) == 0)
	{
		u->numBuildingRockLarge = u->numBuildingRockLarge+1;
	}
	else
	{
		u->numBuildingOther++;
	}
	*/
}

#endif






