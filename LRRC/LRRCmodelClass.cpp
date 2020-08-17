/*******************************************************************************
 *
 * File Name: LRRCmodelClass.cpp
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

#include "LRRCmodelClass.h"

#include "XMLrulesClass.h"
#include "SHAREDvars.h"

double DICE_MODIFIER;
double LDRAW_UNITS_PER_CM;

void fillInModelClassExternVariables()
{
	RulesClass * currentReferenceRulesClass = LRRCrulesMiscellaneous;

	while(currentReferenceRulesClass->next != NULL)
	{
		if(currentReferenceRulesClass->name == DICE_MODIFIER_NAME)
		{
			DICE_MODIFIER = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == LDRAW_UNITS_PER_CM_NAME)
		{
			LDRAW_UNITS_PER_CM = currentReferenceRulesClass->fractionalValue;
		}
		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
}


ModelDetails::ModelDetails(void)
{
	//Define default values for ModelDetails


	/*ModelDetails Type Details Relevant To Units Movement*/
	unitType = UNIT_TYPE_UNIDENTIFIED;					/*this has a derivable value based upon the unit details, and is not currently used*/
	groupIndex = 1;											//default number of units in a model file	//if model represents a group of units, the number of units in the unit group.
	movementSpeed = 0;

	/*ModelDetails Type Details*/
	RulesClass * currentReferenceRulesClass;
	RecordClass * currentReferenceRecordClass;


	//assumes fill and initialise recordClass linked list with same number of references as the RulesClass linked list
	currentReferenceRulesClass = LRRCrulesUnitTypeDetails;
	recordOfUnitTypeDetails = new RecordClass();
	currentReferenceRecordClass = recordOfUnitTypeDetails;
	while(currentReferenceRulesClass->next != NULL)
	{
		currentReferenceRecordClass->name = currentReferenceRulesClass->name;
		RecordClass * newObject = new RecordClass();
		currentReferenceRecordClass->next = newObject;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
		//repetative information:
	numPerson = 0;
	numHorse = 0;			//- this information is inherited during combat from the immediate parent



	/*Combat Details*/

	//assumes fill and initialise recordClass linked list with same number of references as the RulesClass linked list
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceHead;
	recordOfUnitCombatDetailsDefenceHead = new RecordClass();
	currentReferenceRecordClass = recordOfUnitCombatDetailsDefenceHead;
	while(currentReferenceRulesClass->next != NULL)
	{
		currentReferenceRecordClass->name = currentReferenceRulesClass->name;
		RecordClass * newObject = new RecordClass();
		currentReferenceRecordClass->next = newObject;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
	//assumes fill and initialise recordClass linked list with same number of references as the RulesClass linked list
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceTorso;
	recordOfUnitCombatDetailsDefenceTorso = new RecordClass();
	currentReferenceRecordClass = recordOfUnitCombatDetailsDefenceTorso;
	while(currentReferenceRulesClass->next != NULL)
	{
		currentReferenceRecordClass->name = currentReferenceRulesClass->name;
		RecordClass * newObject = new RecordClass();
		currentReferenceRecordClass->next = newObject;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
	//assumes fill and initialise recordClass linked list with same number of references as the RulesClass linked list
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsDefenceShield;
	recordOfUnitCombatDetailsDefenceShield = new RecordClass();
	currentReferenceRecordClass = recordOfUnitCombatDetailsDefenceShield;
	while(currentReferenceRulesClass->next != NULL)
	{
		currentReferenceRecordClass->name = currentReferenceRulesClass->name;
		RecordClass * newObject = new RecordClass();
		currentReferenceRecordClass->next = newObject;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
	//assumes fill and initialise recordClass linked list with same number of references as the RulesClass linked list
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackCloseCombat;
	recordOfUnitCombatDetailsAttackCloseCombat = new RecordClass();
	currentReferenceRecordClass = recordOfUnitCombatDetailsAttackCloseCombat;
	while(currentReferenceRulesClass->next != NULL)
	{
		currentReferenceRecordClass->name = currentReferenceRulesClass->name;
		RecordClass * newObject = new RecordClass();
		currentReferenceRecordClass->next = newObject;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
	//assumes fill and initialise recordClass linked list with same number of references as the RulesClass linked list
	currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
	recordOfUnitCombatDetailsAttackLongDistance = new RecordClass();
	currentReferenceRecordClass = recordOfUnitCombatDetailsAttackLongDistance;
	while(currentReferenceRulesClass->next != NULL)
	{
		currentReferenceRecordClass->name = currentReferenceRulesClass->name;
		RecordClass * newObject = new RecordClass();
		currentReferenceRecordClass->next = newObject;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	breastDefenceValue = 0;			//- this information is inherited during combat
	helmetDefenceValue = 0;			//- this information is inherited during combat
	shieldDefenceValue = 0;			//- this information is inherited during combat
	armourDefenceValue = 0;			//- this information is inherited during combat (breastDefenceValue + helmetDefenceValue + shieldDefenceValue)
	numShields = 0;					//- this information is inherited during combat from the immediate parent

	numLongBow = 0;		//these are derivable
	numQuiver = 0;		//these are derivable

	hasLongDistanceCombatWeapon = 0;			/*this has a derivable value based upon the unit details*/		/*either primary or secondary attack value depending upon combat situation*/

	closeCombatAttackValue = 0;		/*this has a derivable value based upon the unit details*/

	defenceTotal = 0;					//this has a derivable value based upon the unit details
	attackTotal = 0;					//this has a derivable value based upon the unit details
	closeCombatAttackTotal = 0;			//this has a derivable value based upon the unit details
	longDistanceAttackTotal = 0;		//this has a derivable value based upon the unit details
	longDistanceAttackBaseRange = 0;	//this has a derivable value based upon the unit details

	longDistanceAttackValue = 0;	/*this has a derivable value based upon the unit details*/ 	/*either primary or secondary attack value depending upon combat situation*/

	defenceBonus = 0;
	longDistanceAttackBonus = 0;	//not currently used by rules
	closeCombatAttackBonus = 0;		//not currently used by rules

	/*building details*/
	numBuildingOther = 0;

	//assumes fill and initialise recordClass linked list with same number of references as the RulesClass linked list
	currentReferenceRulesClass = LRRCrulesBuildingDetails;
	recordOfBuildingDetails = new RecordClass();
	currentReferenceRecordClass = recordOfBuildingDetails;
	while(currentReferenceRulesClass->next != NULL)
	{
		currentReferenceRecordClass->name = currentReferenceRulesClass->name;
		RecordClass * newObject = new RecordClass();
		currentReferenceRecordClass->next = newObject;
		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	//money details
	numGem = 0;
	numGold = 0;
	numGoblet = 0;

	//terrain/structure details (child object values of these are currently passed to their parents immediatly during initial parse of scene file - this is required to calculate the defence bonuses of units efficiently during combat.
	numBush = 0;
	numBattlement = 0;



}

ModelDetails::~ModelDetails()
{

	if(recordOfUnitTypeDetails != NULL)
	{
		delete recordOfUnitTypeDetails;
	}
	if(recordOfUnitCombatDetailsDefenceHead != NULL)
	{
		delete recordOfUnitCombatDetailsDefenceHead;
	}
	if(recordOfUnitCombatDetailsDefenceTorso != NULL)
	{
		delete recordOfUnitCombatDetailsDefenceTorso;
	}
	if(recordOfUnitCombatDetailsDefenceShield != NULL)
	{
		delete recordOfUnitCombatDetailsDefenceShield;
	}
	if(recordOfUnitCombatDetailsAttackCloseCombat != NULL)
	{
		delete recordOfUnitCombatDetailsAttackCloseCombat;
	}
	if(recordOfUnitCombatDetailsAttackLongDistance != NULL)
	{
		delete recordOfUnitCombatDetailsAttackLongDistance;
	}
	if(recordOfBuildingDetails != NULL)
	{
		delete recordOfBuildingDetails;
	}


	/*OLD:
	RecordClass * currentReferenceRulesClass;

	currentReferenceRulesClass = recordOfUnitTypeDetails;
	while(currentReferenceRulesClass->next != NULL)
	{
		previousReferenceRulesClass = currentReferenceRulesClass;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		delete previousReferenceRulesClass;
	}

	currentReferenceRulesClass = recordOfUnitCombatDetailsDefenceHead;
	while(currentReferenceRulesClass->next != NULL)
	{
		previousReferenceRulesClass = currentReferenceRulesClass;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		delete previousReferenceRulesClass;
	}

	currentReferenceRulesClass = recordOfUnitCombatDetailsDefenceTorso;
	while(currentReferenceRulesClass->next != NULL)
	{
		previousReferenceRulesClass = currentReferenceRulesClass;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		delete previousReferenceRulesClass;
	}

	currentReferenceRulesClass = recordOfUnitCombatDetailsDefenceShield;
	while(currentReferenceRulesClass->next != NULL)
	{
		previousReferenceRulesClass = currentReferenceRulesClass;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		delete previousReferenceRulesClass;
	}

	currentReferenceRulesClass = recordOfUnitCombatDetailsAttackCloseCombat;
	while(currentReferenceRulesClass->next != NULL)
	{
		previousReferenceRulesClass = currentReferenceRulesClass;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		delete previousReferenceRulesClass;
	}

	currentReferenceRulesClass = recordOfUnitCombatDetailsAttackLongDistance;
	while(currentReferenceRulesClass->next != NULL)
	{
		previousReferenceRulesClass = currentReferenceRulesClass;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		delete previousReferenceRulesClass;
	}

	currentReferenceRulesClass = recordOfBuildingDetails;
	while(currentReferenceRulesClass->next != NULL)
	{
		previousReferenceRulesClass = currentReferenceRulesClass;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
		delete previousReferenceRulesClass;
	}
	*/
}


/*
void determineUnitTypeAndMinSpeedOfUnitGroup(ModelDetails * u)

preconditions:
to calculate groupIndex (of unit group) correctly, all units within group must be of same type (and if the units are machines/vehicles, they must be of the same form, eg same number of wheels etc)
to calculate movementSpeed (min of unit group) correctly there is currenlty no known preconditions.
to calculate unitType (min of unit group) correctly there is currenlty no known preconditions
*/
void determineUnitTypeAndMinSpeedOfUnitGroup(ModelDetails * u)
{
	/*
	2x large wheels && >=2x wheel axels && 1x horses && (numHorseHitch || numHorseHitchWithHinge) && > 1x person && < 2x persons = 15 (small/large cart),
	2x large wheels && >=2x wheel axels  < 1x horses || 2x small wheels = 5 (small/large warmachine),
	2x small hull && > 1x person && < 4x person = 15 (small boat),
	2x large hull && >1x person && < 8x person = 15 (large boat)
	1x horses && < 1x person = 20 (horse/knight),
	*/

	int numPerson = 0;
	int numHorse = 0;

	int numSaddle = 0;
	int numSmallHull = 0;
	int numLargeHull = 0;
	int numSmallWheel = 0;
	int numLargeWheel = 0;
	int	numWheelHolder = 0;
	int numWheelHolderDual = 0;
	int	numHorseHitch = 0;
	int	numHorseHitchWithHinge = 0;
	int	numHorseHitchHingeRotatable = 0;
	int	numCatapultBucket = 0;
	int	numCatapultAxel = 0;
		//Additional ModelDetails Type Details Details
	int numSkeleton = 0;
	int numGhost = 0;

	RecordClass * currentReferenceRecordClass = u->recordOfUnitTypeDetails;
	while(currentReferenceRecordClass->next != NULL)
	{
		if(currentReferenceRecordClass->name == PERSON_HEAD_NAME)
		{
			numPerson = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
			//cout << "DEBUG: u->numPerson = " << numPerson << endl;
		}
		else if(currentReferenceRecordClass->name == SADDLE_NAME)
		{
			numSaddle = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else if(currentReferenceRecordClass->name == HORSE_NAME)
		{
			numHorse = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else if(currentReferenceRecordClass->name == BOAT_SMALL_HULL_NAME)
		{
			numSmallHull = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else if(currentReferenceRecordClass->name == BOAT_LARGE_HULL_NAME)
		{
			numLargeHull = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else if(currentReferenceRecordClass->name == VEHICLE_SMALL_WHEEL_NAME)
		{
			numSmallWheel = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else if(currentReferenceRecordClass->name == VEHICLE_LARGE_WHEEL_NAME)
		{
			numLargeWheel = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else if(currentReferenceRecordClass->name == VEHICLE_WHEEL_HOLDER_NAME)
		{
			numWheelHolder = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else if(currentReferenceRecordClass->name == VEHICLE_WHEEL_HOLDER_DUAL_NAME)
		{
			numWheelHolderDual = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else if(currentReferenceRecordClass->name == VEHICLE_HORSE_HITCH_NAME)
		{
			numHorseHitch = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else if(currentReferenceRecordClass->name == VEHICLE_HORSE_HITCH_WITH_HINGE_NAME)
		{
			numHorseHitchWithHinge = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else if(currentReferenceRecordClass->name == VEHICLE_HORSE_HITCH_HINGE_ROTATABLE_NAME)
		{
			numHorseHitchHingeRotatable = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else if(currentReferenceRecordClass->name == CATAPULT_BUCKET_NAME)
		{
			numCatapultBucket = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else if(currentReferenceRecordClass->name == CATAPULT_AXEL_NAME)
		{
			numCatapultAxel = currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
		}
		else
		{
			//no notification made in fileDetails object
		}

		currentReferenceRecordClass = currentReferenceRecordClass->next;
	}

	//u->numPerson = numPerson;		//2007 NB this may not be required; added for backwards compatability for movment.cpp searchThisPhaseStartSceneReferenceListForReferenceComparisonInitiation()'s determineUnitTypeAndMinSpeedOfUnitGroup() reference
	//u->numHorse = numHorse;			//2007 NB this may not be required; added for backwards compatability for movment.cpp searchThisPhaseStartSceneReferenceListForReferenceComparisonInitiation()'s determineUnitTypeAndMinSpeedOfUnitGroup() reference


	if((numLargeWheel >= 2) || (numSmallWheel >= 4))
	{
		if((numHorse >= (numLargeWheel/2 + numSmallWheel/4)) && (numPerson >= (numLargeWheel/2 + numSmallWheel/4)) && ((numWheelHolder + numWheelHolderDual*2) >= (numLargeWheel + numSmallWheel/2)) && (numHorseHitch + numHorseHitchWithHinge + numHorseHitchHingeRotatable))
		{
			if(numPerson >= (numLargeWheel + numSmallWheel*2))
			{
				//too many people to sit on carts, max movement speed is the speed of a person
				u->groupIndex = numPerson;
				u->unitType = UNIT_TYPE_FOOTSOLDIER;
			}
			else
			{
				u->groupIndex = maxInt(numHorseHitch, numHorseHitchHingeRotatable);	/*OLD: groupIndex = numHorseHitch + numHorseHitchWithHinge  -> this is not currently very accurate, a better check would be groupIndex = hasPivotPoint*/
				if(numHorse >= 2*u->groupIndex)
				{
					u->unitType = UNIT_TYPE_CART_LARGE;
				}
				else
				{
					u->unitType = UNIT_TYPE_CART_SMALL;
				}
			}
		}
		else
		{//not enough (people, horses, wheel axels or horse hitches) to drive the large vehicle(s) efficiently

				//used to be "if(((numPerson && numHorse*2) >= numLargeWheel) && ..."
			if((numPerson >= numLargeWheel) && ((numWheelHolder + numWheelHolderDual*2) >= numLargeWheel) && (numLargeWheel >= 4))
			{
				u->groupIndex = numLargeWheel/4;
				u->unitType = UNIT_TYPE_WARMACHINE_LARGE;
			}
			else if((numPerson >= numSmallWheel/2) && ((numWheelHolder + numWheelHolderDual*2) >= numSmallWheel))
			{
				u->groupIndex = numSmallWheel/2;
				u->unitType = UNIT_TYPE_WARMACHINE_SMALL;
			}
			else
			{
				//not enough people/horses to drive the large vehicle(s) OR not enough axils to support the number of wheels of the vehicle
				u->unitType = UNIT_TYPE_UNIDENTIFIED;
			}

		}
	}
	else if(numSmallWheel == 2)
	{
		if((numPerson >= numSmallWheel/2) &&  ((numWheelHolder + numWheelHolderDual*2) >= numSmallWheel))
		{
				u->groupIndex = numSmallWheel/2;
				u->unitType = UNIT_TYPE_WARMACHINE_SMALL;
		}
		else
		{//not enough people to drive the small vehicle OR not enough axils to support the number of wheels of the vehicle
			u->unitType = UNIT_TYPE_UNIDENTIFIED;
		}
	}
	else if(numSmallHull >= 2)
	{
		if((numPerson >= numSmallHull/2) && (numPerson <= numSmallHull*2))
		{
			u->groupIndex = numSmallHull/2;
			u->unitType = UNIT_TYPE_BOAT_SMALL;
		}
		else
		{//not enough people to drive the boats
			u->unitType = UNIT_TYPE_UNIDENTIFIED;
		}
	}
	else if(numLargeHull >= 4)
	{
		if((numPerson >= numLargeHull/2) && (numPerson <= numLargeHull*2))
		{
			u->groupIndex = numLargeHull/4;
			u->unitType = UNIT_TYPE_BOAT_LARGE;
		}
		else
		{//not enough people to drive the boats
			u->unitType = UNIT_TYPE_UNIDENTIFIED;
		}

	}
	else if((numHorse >= 1) && (numPerson == numHorse) && (numSaddle == numPerson))
	{
		u->groupIndex = numHorse;
		u->unitType = UNIT_TYPE_KNIGHT;
	}
	else if(numPerson)
	{
		u->groupIndex = numPerson;
		u->unitType = UNIT_TYPE_FOOTSOLDIER;
	}
	else
	{
		u->unitType = UNIT_TYPE_UNIDENTIFIED;
	}


	//fill in unit details
	u->numPerson = numPerson;
	u->numHorse = numHorse;

	calculateMovementSpeedAndDefenceTotalOfUnitTypeFromRulesList(u);
}


bool calculateMovementSpeedAndDefenceTotalOfUnitTypeFromRulesList(ModelDetails * u)
{
	bool result = false;
	RulesClass * currentReferenceRulesClass = LRRCrulesUnitTypeCatagories;
	while(currentReferenceRulesClass->next != NULL)
	{
		if(int(currentReferenceRulesClass->fractionalValue) == u->unitType)
		{
			u->movementSpeed  = currentReferenceRulesClass->attribute4;
			u->defenceTotal = maxInt(u->defenceTotal, currentReferenceRulesClass->attribute5);
			result = true;
		}
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	return result;
}




void copyRecordLists(RecordClass * recordToFill, RecordClass * recordToCopy)
{
	RecordClass * currentReferenceRecordToFill = recordToFill;
	RecordClass * currentReferenceRecordToCopy = recordToCopy;
	while(currentReferenceRecordToFill->next != NULL)
	{
		//currentReferenceRecordToFill->name = currentReferenceRecordToCopy->name;	//not required
		currentReferenceRecordToFill->numberOfThisPartIDInTheUnit = currentReferenceRecordToCopy->numberOfThisPartIDInTheUnit;

		currentReferenceRecordToFill = currentReferenceRecordToFill->next;
		currentReferenceRecordToCopy = currentReferenceRecordToCopy->next;
	}
}


void addRecordLists(RecordClass * recordToFill, RecordClass * recordToAdd)
{
	RecordClass * currentReferenceRecordToFill = recordToFill;
	RecordClass * currentReferenceRecordToAdd = recordToAdd;
	while(currentReferenceRecordToFill->next != NULL)
	{
		//currentReferenceRecordToFill->name = currentReferenceRecordToCopy->name;	//not required
		currentReferenceRecordToFill->numberOfThisPartIDInTheUnit = currentReferenceRecordToFill->numberOfThisPartIDInTheUnit + currentReferenceRecordToAdd->numberOfThisPartIDInTheUnit;

		currentReferenceRecordToFill = currentReferenceRecordToFill->next;
		currentReferenceRecordToAdd = currentReferenceRecordToAdd->next;
	}
}




void copyAllUnitDetails(ModelDetails * u, ModelDetails * unitToCopy)
{

	u->unitType = unitToCopy->unitType;
	u->movementSpeed = unitToCopy->movementSpeed;

	copyRecordLists(u->recordOfUnitTypeDetails, unitToCopy->recordOfUnitTypeDetails);

	u->numPerson = unitToCopy->numPerson;			//these are derivable
	u->numHorse = unitToCopy->numHorse;			//these are derivable

	u->numShields = unitToCopy->numShields;			//these are derivable

	copyRecordLists(u->recordOfUnitCombatDetailsAttackCloseCombat, unitToCopy->recordOfUnitCombatDetailsAttackCloseCombat);
	copyRecordLists(u->recordOfUnitCombatDetailsAttackLongDistance, unitToCopy->recordOfUnitCombatDetailsAttackLongDistance);

	copyRecordLists(u->recordOfUnitCombatDetailsDefenceShield, unitToCopy->recordOfUnitCombatDetailsDefenceShield);
	copyRecordLists(u->recordOfUnitCombatDetailsDefenceTorso, unitToCopy->recordOfUnitCombatDetailsDefenceTorso);
	copyRecordLists(u->recordOfUnitCombatDetailsDefenceHead, unitToCopy->recordOfUnitCombatDetailsDefenceHead);

	u->breastDefenceValue = unitToCopy->breastDefenceValue;	//these are derivable
	u->helmetDefenceValue = unitToCopy->helmetDefenceValue;	//these are derivable
	u->shieldDefenceValue = unitToCopy->shieldDefenceValue;	//these are derivable
	u->armourDefenceValue = unitToCopy->armourDefenceValue;	//these are derivable

	u->numLongBow = unitToCopy->numLongBow;			//these are derivable
	u->numQuiver = unitToCopy->numQuiver;			//these are derivable

	u->closeCombatAttackValue = unitToCopy->closeCombatAttackValue;			//these are derivable
	u->longDistanceAttackValue = unitToCopy->longDistanceAttackValue;		//these are derivable
	u->defenceTotal = unitToCopy->defenceTotal;					//these are derivable
	u->attackTotal = unitToCopy->attackTotal;					//these are derivable
	u->closeCombatAttackTotal = unitToCopy->closeCombatAttackTotal;			//these are derivable
	u->longDistanceAttackTotal = unitToCopy->longDistanceAttackTotal;		//these are derivable
	u->longDistanceAttackBaseRange = unitToCopy->longDistanceAttackBaseRange;	//these are derivable

	u->defenceBonus = unitToCopy->defenceBonus;					//these are derivable
	u->longDistanceAttackBonus = unitToCopy->longDistanceAttackBonus;		//these are derivable
	u->closeCombatAttackBonus = unitToCopy->closeCombatAttackBonus;			//these are derivable

	u->hasLongDistanceCombatWeapon = unitToCopy->hasLongDistanceCombatWeapon;	//these are derivable


			//ADDED IN 2007 - this may not be required
	//u->numBuildingOther = unitToCopy->numBuildingOther;
	//copyRecordLists(u->recordOfBuildingDetails, unitToCopy->recordOfBuildingDetails);

	//money details
	u->numGem = unitToCopy->numGem;
	u->numGold = unitToCopy->numGold;
	u->numGoblet = unitToCopy->numGoblet;

	//terrain/structure details
	u->numBush = unitToCopy->numBush;
	u->numBattlement = unitToCopy->numBattlement;
}


//This is an example of a generic function that does not need to be defined within unitClass.cpp
void copyEnvironmentRelevantChildUnitDetailsIntoParentObject(ModelDetails * unitChild, ModelDetails * unitParent)
{
	//terrain/structure details
	unitParent->numBattlement = unitParent->numBattlement + unitChild->numBattlement; //BEFORE 2007: + unitChild->numBuildingBricks;	//used to potentially hide behind
	unitParent->numBush = unitParent->numBush + unitChild->numBush;																//used to potentially hide behind
}


//This is an example of a generic function that does not need to be defined within unitClass.cpp
void addAllCombatRelevantChildModelDetailsIntoAParentUnit(ModelDetails * unitChild, ModelDetails * unitParent)
{
	addRecordLists(unitParent->recordOfUnitTypeDetails, unitChild->recordOfUnitTypeDetails);

	unitParent->numPerson = unitParent->numPerson + unitChild->numPerson;
	unitParent->numHorse = unitParent->numHorse + unitChild->numHorse;

		/*
		unitParent->numSaddle = unitParent->numSaddle + unitChild->numSaddle;
		unitParent->numSmallHull = unitParent->numSmallHull + unitChild->numSmallHull;
		unitParent->numLargeHull = unitParent->numLargeHull + unitChild->numLargeHull;
		unitParent->numSmallWheel = unitParent->numSmallWheel + unitChild->numSmallWheel;
		unitParent->numLargeWheel = unitParent->numLargeWheel + unitChild->numLargeWheel;
		unitParent->numWheelHolder = unitParent->numWheelHolder + unitChild->numWheelHolder;
		unitParent->numWheelHolderDual = unitParent->numWheelHolderDual + unitChild->numWheelHolderDual;
		unitParent->numHorseHitch = unitParent->numHorseHitch + unitChild->numHorseHitch;
		unitParent->numHorseHitchWithHinge = unitParent->numHorseHitchWithHinge + unitChild->numHorseHitchWithHinge;
		unitParent->numHorseHitchHingeRotatable = unitParent->numHorseHitchHingeRotatable + unitChild->numHorseHitchHingeRotatable;
		unitParent->numCatapultBucket = unitParent->numCatapultBucket + unitChild->numCatapultBucket;
		unitParent->numCatapultAxel = unitParent->numCatapultAxel + unitChild->numCatapultAxel;

		unitParent->numSkeleton = unitParent->numSkeleton + unitChild->numSkeleton;
		unitParent->numGhost = unitParent->numGhost + unitChild->numGhost;
		*/

	unitParent->numShields = unitParent->numShields + unitChild->numShields;

	addRecordLists(unitParent->recordOfUnitCombatDetailsAttackCloseCombat, unitChild->recordOfUnitCombatDetailsAttackCloseCombat);
	addRecordLists(unitParent->recordOfUnitCombatDetailsAttackLongDistance, unitChild->recordOfUnitCombatDetailsAttackLongDistance);

		/*
		unitParent->numHandAxe = unitParent->numHandAxe + unitChild->numHandAxe;
		unitParent->numLance = unitParent->numLance + unitChild->numLance;
		unitParent->numSpear = unitParent->numSpear + unitChild->numSpear;
		unitParent->numPlateNeck = unitParent->numPlateNeck + unitChild->numPlateNeck;
		unitParent->numLongBow = unitParent->numLongBow + unitChild->numLongBow;
		unitParent->numQuiver = unitParent->numQuiver + unitChild->numQuiver;
		unitParent->numCrossBow = unitParent->numCrossBow + unitChild->numCrossBow;
		unitParent->numSword = unitParent->numSword + unitChild->numSword;
		unitParent->numAxe = unitParent->numAxe + unitChild->numAxe;
		unitParent->numLargeAxe = unitParent->numLargeAxe + unitChild->numLargeAxe;
		unitParent->numLargeSword = unitParent->numLargeSword + unitChild->numLargeSword;
		*/

	addRecordLists(unitParent->recordOfUnitCombatDetailsDefenceShield, unitChild->recordOfUnitCombatDetailsDefenceShield);
	addRecordLists(unitParent->recordOfUnitCombatDetailsDefenceTorso, unitChild->recordOfUnitCombatDetailsDefenceTorso);
	addRecordLists(unitParent->recordOfUnitCombatDetailsDefenceHead, unitChild->recordOfUnitCombatDetailsDefenceHead);

	/*
	unitParent->breastDefenceValue = maxInt(unitParent->breastDefenceValue, unitChild->breastDefenceValue);		//Check this: is this required?
	unitParent->helmetDefenceValue = maxInt(unitParent->helmetDefenceValue, unitChild->helmetDefenceValue);		//Check this: is this required?
	unitParent->shieldDefenceValue = maxInt(unitParent->shieldDefenceValue, unitChild->shieldDefenceValue);		//Check this: is this required?
	unitParent->armourDefenceValue = maxInt(unitParent->armourDefenceValue, unitChild->armourDefenceValue);		//Check this: is this required?
	*/

	//money details		//Check if this needs to be inherited - does it serve a purpose wrt to the purpose of the addAllCombatRelevantChildModelDetailsIntoAParentUnit() function
	unitParent->numGem = unitParent->numGem + unitChild->numGem;
	unitParent->numGold = unitParent->numGold + unitChild->numGold;
	unitParent->numGoblet = unitParent->numGoblet + unitChild->numGoblet;

	//terrain/structure details	//this may not be required as combat unit files might be defined not include building bricks (catapults could be the exception)
	unitParent->numBush  = unitParent->numBush + unitChild->numBush;
	unitParent->numBattlement  = unitParent->numBattlement + unitChild->numBattlement;

}


//no longer used due to algorithm break down
/*
void copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject(ModelDetails * unitChild, ModelDetails * unitParent)
{
	//unitChild->numShields = unitChild->numShields + unitParent->numShields;				//not inherited (armour must be placed on child unit)
		//weaponary from a person's higher level unit can be used by the person (Eg swords attached to catapults, castle walls, horse saddle clips etc)
	unitChild->numHandAxe = unitChild->numHandAxe + unitParent->numHandAxe;
	unitChild->numLance = unitChild->numLance + unitParent->numLance;
	unitChild->numSpear = unitChild->numSpear + unitParent->numSpear;
	unitChild->numPlateNeck = unitChild->numPlateNeck + unitParent->numPlateNeck;
	unitChild->numLongBow = unitChild->numLongBow + unitParent->numLongBow;
	unitChild->numQuiver = unitChild->numQuiver + unitParent->numQuiver;
	unitChild->numCrossBow = unitChild->numCrossBow + unitParent->numCrossBow;
	unitChild->numSword = unitChild->numSword + unitParent->numSword;
	unitChild->numAxe = unitChild->numAxe + unitParent->numAxe;
	unitChild->numLargeAxe = unitChild->numLargeAxe + unitParent->numLargeAxe;
	unitChild->numLargeSword = unitChild->numLargeSword + unitParent->numLargeSword;

	//terrain/structure details
	unitChild->numBush = unitChild->numBush + unitParent->numBush;
	unitChild->numBattlement = unitChild->numBattlement + unitParent->numBattlement;

	//The following Code has been added for when the rules of embedded units are relaxed, for example when having a (horse+saddle+equipment) submodel within footsoldier files is allowed. Note this is currently not allowed, and only footsoldier submodels may be embedded within horse+saddle+equipment files.
	//unitChild->numPerson = unitChild->numPerson + unitParent->numPerson;			//not currently inherited		//not inherited (a person must not contain submodels representing the persons's weaponary/armour)
	//unitChild->numSkeleton = unitChild->numSkeleton + unitParent->numSkeleton;	//not currently inherited
	//unitChild->numGhost = unitChild->numGhost + unitParent->numGhost;				//not currently inherited

	//unitChild->breastDefenceValue = maxInt(unitChild->breastDefenceValue, unitParent->breastDefenceValue);		//not inherited (armour must be placed on child unit)
	//unitChild->helmetDefenceValue = maxInt(unitChild->helmetDefenceValue, unitParent->helmetDefenceValue);		//not inherited (armour must be placed on child unit)
	//unitChild->shieldDefenceValue = maxInt(unitChild->shieldDefenceValue, unitParent->shieldDefenceValue);		//not inherited (armour must be placed on child unit)
}
*/


//generic functions
/*
int maxInt(int a, int b)
{
	if(a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
*/

int invertLevel(int level)
{
	return (7-level);
}
