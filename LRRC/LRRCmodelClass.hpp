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
 * File Name: LRRCmodelClass.hpp
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


#ifndef HEADER_LRRC_MODEL_CLASS
#define HEADER_LRRC_MODEL_CLASS

#include "LRRCglobalDefs.hpp"

/*#define GAME_USE_COLOUR_AS_UNIQUE_UNIT_IDS*/
#ifndef GAME_USE_COLOUR_AS_UNIQUE_UNIT_IDS
	#define GAME_USE_FILENAME_AS_UNIQUE_UNIT_IDS 	/*(with colour being equivalent to the player number)*/
#endif

#include "LRRCrecordClass.hpp"

#ifdef USE_ANN
	#include "ANNexperienceClass.hpp"
#endif
#include "LRRCrules.hpp"
#include "SHAREDvars.hpp"



//shared variables [not model class specific]
#define GAME_PHASES_NUM_PER_TURN (3)
#define GAME_PHASE_MOVEMENT (0)
#define GAME_PHASE_LONGDISTANCECOMBAT (1)
#define GAME_PHASE_CLOSECOMBAT (2)
#define GAME_PHASE_DEFAULT (GAME_PHASE_MOVEMENT)
#define GAME_PHASE_GENERIC (10)
	#define GAME_PHASE_AIONLY_TARGET_SELECTION (4)	//special phase used by AI only


#define HORSE_NAME "HORSE"
#define PERSON_HEAD_NAME "PERSON_HEAD"
#define SADDLE_NAME "SADDLE"
#define VEHICLE_SMALL_WHEEL_NAME "VEHICLE_SMALL_WHEEL"
#define VEHICLE_LARGE_WHEEL_NAME "VEHICLE_LARGE_WHEEL"
#define BOAT_SMALL_HULL_NAME "BOAT_SMALL_HULL"
#define BOAT_LARGE_HULL_NAME "BOAT_LARGE_HULL"
#define VEHICLE_WHEEL_HOLDER_NAME "VEHICLE_WHEEL_HOLDER"
#define VEHICLE_WHEEL_HOLDER_DUAL_NAME "VEHICLE_WHEEL_HOLDER_DUAL"
#define VEHICLE_HORSE_HITCH_NAME "VEHICLE_HORSE_HITCH"
#define VEHICLE_HORSE_HITCH_WITH_HINGE_NAME "VEHICLE_HORSE_HITCH_WITH_HINGE"
#define VEHICLE_HORSE_HITCH_HINGE_ROTATABLE_NAME "VEHICLE_HORSE_HITCH_HINGE_ROTATABLE"
#define CATAPULT_BUCKET_NAME "CATAPULT_BUCKET"
#define CATAPULT_AXEL_NAME "CATAPULT_AXEL"


//#define USE_UNACCEPTED_2006_RULES



	//unrecommmended debug options
	//#define DEBUG
	//#define DEBUG_MOVEMENT_CPP_ELABORATE
//Recommended debug options
//#define DEBUG_MOVEMENT_CPP
//#define DEBUG_MOVEMENT_CPP_DISPLACEMENT

//these variables define the file (/ModelDetails) type

#define UNIT_TYPE_UNIDENTIFIED 0	//file is unidentified
#define UNIT_TYPE_FOOTSOLDIER 1	//file contains a man
#define UNIT_TYPE_KNIGHT 2	//file contains a man, horse, and saddle
#define UNIT_TYPE_HORSE_AND_SADDLE 3	//file contains a horse and saddle		//NB 'horse and saddle' ModelDetails types are treated as knights for movement purposes as they may contain embedded footsoldiers
//#define UNIT_TYPE_CART 10
#define UNIT_TYPE_CART_SMALL 4	//file contains a small cart (with horse)
#define UNIT_TYPE_CART_LARGE 5	//file contains a large cart (with horse)
#define UNIT_TYPE_WARMACHINE_SMALL 6	//file contains a small warmachine
#define UNIT_TYPE_WARMACHINE_LARGE 7	//file contains a large warmachine
#define UNIT_TYPE_BOAT_SMALL 8	//file contains a boat
#define UNIT_TYPE_BOAT_LARGE 9	//file contains a large boat

/*
#define UNIT_TYPE_UNIDENTIFIED_MAX_TRAVEL 0
#define UNIT_TYPE_FOOTSOLDIER_MAX_TRAVEL 10
#define UNIT_TYPE_KNIGHT_MAX_TRAVEL 20
#define UNIT_TYPE_HORSE_AND_SADDLE_MAX_TRAVEL 20	//f//NB 'horse and saddle' ModelDetails types are treated as knights for movement purposes as they may contain embedded footsoldiers
//#define UNIT_TYPE_CART_MAX_TRAVEL 15
#define UNIT_TYPE_CART_SMALL_MAX_TRAVEL 15
#define UNIT_TYPE_CART_LARGE_MAX_TRAVEL 15
#define UNIT_TYPE_WARMACHINE_SMALL_MAX_TRAVEL 5
#define UNIT_TYPE_WARMACHINE_LARGE_MAX_TRAVEL 5
#define UNIT_TYPE_BOAT_SMALL_MAX_TRAVEL 15
#define UNIT_TYPE_BOAT_LARGE_MAX_TRAVEL 25
*/

#define UNIT_TYPE_UNIDENTIFIED_NAME "UNIT_TYPE_UNIDENTIFIED"	//file is unidentified
#define UNIT_TYPE_FOOTSOLDIER_NAME "UNIT_TYPE_FOOTSOLDIER"	//file contains a man
#define UNIT_TYPE_KNIGHT_NAME "UNIT_TYPE_KNIGHT"	//file contains a man, horse, and saddle
#define UNIT_TYPE_HORSE_AND_SADDLE_NAME "UNIT_TYPE_HORSE_AND_SADDLE"	//file contains a horse and saddle		//NB 'horse and saddle' ModelDetails types are treated as knights for movement purposes as they may contain embedded footsoldiers
#define UNIT_TYPE_CART_SMALL_NAME "UNIT_TYPE_CART_SMALL"	//file contains a small cart (with horse)
#define UNIT_TYPE_CART_LARGE_NAME "UNIT_TYPE_CART_LARGE"	//file contains a large cart (with horse)
#define UNIT_TYPE_WARMACHINE_SMALL_NAME "UNIT_TYPE_WARMACHINE_SMALL"	//file contains a small warmachine
#define UNIT_TYPE_WARMACHINE_LARGE_NAME "UNIT_TYPE_WARMACHINE_LARGE"	//file contains a large warmachine
#define UNIT_TYPE_BOAT_SMALL_NAME "UNIT_TYPE_BOAT_SMALL"	//file contains a boat
#define UNIT_TYPE_BOAT_LARGE_NAME "UNIT_TYPE_BOAT_LARGE"	//file contains a large boat



/*these definitions do not belong here. they should be in a separate header file shared by all files.*/
#define DICE_MODIFIER_NAME "DICE_MODIFIER"
#define LDRAW_UNITS_PER_CM_NAME "LDRAW_UNITS_PER_CM"
//#define DICE_MODIFIER 2.0
//#define LDRAW_UNITS_PER_CM 25.189		//or 0.397?




extern double DICE_MODIFIER;
extern double LDRAW_UNITS_PER_CM;

class ModelDetails{
private:
	/*There are currently no private attributes of this class*/
public:

	ModelDetails(void); // constructor
	~ModelDetails();	// and destructor.

	/*ModelDetails Type Details Relevant To Units Movement - these could be transfered to a separate File class*/
	int unitType;
	int groupIndex;		//if model represents a group of units, the number of units in the unit group.
	int movementSpeed;

	/*ModelDetails Type Details - these could be transfered to a separate File class*/

	RecordClass* recordOfUnitTypeDetails;
		//repetative information:
	int numPerson;
	int numHorse;
	/*
	int numSaddle;
	int numSmallHull;
	int numLargeHull;
	int numSmallWheel;
	int numLargeWheel;
	int	numWheelHolder;
	int numWheelHolderDual;
	int	numHorseHitch;
	int	numHorseHitchWithHinge;
	int	numHorseHitchHingeRotatable;
	int	numCatapultBucket;
	int	numCatapultAxel;
		//Additional ModelDetails Type Details Details
	int numSkeleton;
	int numGhost;
	*/

	/*Combat Details*/
	RecordClass* recordOfUnitCombatDetailsDefenceHead;
	RecordClass* recordOfUnitCombatDetailsDefenceTorso;
	RecordClass* recordOfUnitCombatDetailsDefenceShield;
	RecordClass* recordOfUnitCombatDetailsAttackCloseCombat;
	RecordClass* recordOfUnitCombatDetailsAttackLongDistance;

	int breastDefenceValue;
	int helmetDefenceValue;
	int shieldDefenceValue;
	int armourDefenceValue;
	int numShields;

	int numLongBow;	//these are derivable
	int numQuiver;	//these are derivable
		/*
		int numHandAxe;
		int numLance;
		int numSpear;
		int numPlateNeck;
		int numLongBow;
		int numQuiver;
		int numCrossBow;
		int numSword;
		int numAxe;
		int numLargeAxe;
		int numLargeSword;
		*/
		/*
		int hasLongDistancePrimaryCombatWeapon;
		int hasLongDistanceSecondaryCombatWeapon;
		*/
	int hasLongDistanceCombatWeapon;

	//int catapultAttackValue;
	int closeCombatAttackValue;
		/*
		int longDistancePrimaryAttackValue;
		int longDistanceSecondaryAttackValue;
		*/
	int longDistanceAttackValue;
	int defenceTotal;
	int attackTotal;
	int closeCombatAttackTotal;
	int longDistanceAttackTotal;
	int longDistanceAttackBaseRange;
		/*
		int longDistancePrimaryAttackBaseRange;
		int longDistanceSecondaryAttackBaseRange;
		*/

	int defenceBonus;
	int longDistanceAttackBonus;
	int closeCombatAttackBonus;

	/*building details - these could be transfered to separate Building class*/
	int numBuildingOther;
	RecordClass* recordOfBuildingDetails;
	/*
	int numBuildingWallWindow;
	int numBuildingWallCorner;
	int numBuildingBattlementTower;
	int numBuildingRockSmall;
	int numBuildingRockLarge;
	int numBuildingBricks;
	*/


	//money details
	int numGem;
	int numGold;
	int numGoblet;

	//terrain/structure details (child object values of these are currently passed to their parents immediatly during initial parse of scene file - this is required to calculate the defence bonuses of units efficiently during combat.
	int numBush;
	int numBattlement;

};


class LRRCmodelClassClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: void fillInModelClassExternVariables();

	public: void copyAllUnitDetails(ModelDetails* u, ModelDetails* unitToCopy);
	public: void copyEnvironmentRelevantChildUnitDetailsIntoParentObject(ModelDetails* unitChild, ModelDetails* unitParent);
	public: void addAllCombatRelevantChildModelDetailsIntoAParentUnit(ModelDetails* unitChild, ModelDetails* unitParent);
	public: void determineUnitTypeAndMinSpeedOfUnitGroup(ModelDetails* u);

	private: bool calculateMovementSpeedAndDefenceTotalOfUnitTypeFromRulesList(ModelDetails* u);
	private: void copyRecordLists(RecordClass* recordToFill, RecordClass* recordToCopy);
	private: void addRecordLists(RecordClass* recordToFill, RecordClass* recordToAdd);

	//no longer used due to algorithm break down
	//void copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject(ModelDetails* unitChild, ModelDetails* unitParent);

	//generic
	//int maxInt(int a, int b);
	public: int invertLevel(const int level);
};

#endif
