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
 * File Name: LRRCmovement.cpp
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


#include "LRRCmovement.h"
#include "LRRCcombat.h"
#include "LRRCparser.h"
#include "LDparser.h"
#include "LDreferenceManipulation.h"

#include "LRRCrules.h"

static int maxNumBuildingRitualPointsAllowedDuringRound;
static int maxNumCombatRitualPointsAllowedDuringRound;
static int numberOfBuildingPointsUsedByPlayerSoFarDuringRound;
static int numberOfCombatPointsUsedByPlayerSoFarDuringRound;

	//NOTE THIS MUST BE DECLARED DYNAMICALLY IN THE FUTURE. CURRENTLY IT IS ASSUMED THE PLAYER HAS A BUDGET OF 1000


ModelDetails* buildingSection;













//preconditions 19-3-06 - unit parts relevant to movement (eg horses, persons, hulls, wheels) must be wrapped by at least one higher level unit. the higher level unit should be moved between scenes and not its child objects (wheel, horse, head etc).


/****************************************************************
/
/ compareSceneFilesMovementPhase
/
/****************************************************************/




bool compareSceneFilesMovementPhase(char* preMovementPhaseSceneFileName, char* thisPhaseStartSceneFileName, Player* currentPlayer, char* targetSpritesSceneFileName, bool addSprites)
{
	bool result = true;

	buildingSection = new ModelDetails();	//NEW

	//declare initial scene references
	LDreference* initialReferenceInThisPhaseStartScene = new LDreference();
	LDreference* initialReferenceInPreMovementPhaseScene = new LDreference();
	LDreference* topLevelReferenceInThisPhaseStartScene = new LDreference(true);
	LDreference* topLevelReferenceInPreMovementPhaseScene = new LDreference(true);

	if(!parseFile(thisPhaseStartSceneFileName, initialReferenceInThisPhaseStartScene, topLevelReferenceInThisPhaseStartScene, false))
	{//file does not exist
		cout << "The file: thisPhaseStartSceneFileName=" << thisPhaseStartSceneFileName << " does not exist in the directory" << endl;
		result = false;
	}

	if(!parseFile(preMovementPhaseSceneFileName, initialReferenceInPreMovementPhaseScene, topLevelReferenceInPreMovementPhaseScene, false))
	{//file does not exist
		cout << "The file: preMovementPhaseSceneFileName=" << preMovementPhaseSceneFileName << " does not exist in the directory" << endl;
		result = false;
	}

	if(result)
	{
		result = compareScenesMovementPhase(preMovementPhaseSceneFileName, initialReferenceInPreMovementPhaseScene, initialReferenceInThisPhaseStartScene, currentPlayer, targetSpritesSceneFileName, addSprites);
	}

	delete initialReferenceInThisPhaseStartScene;
	delete initialReferenceInPreMovementPhaseScene;
	delete topLevelReferenceInThisPhaseStartScene;
	delete topLevelReferenceInPreMovementPhaseScene;

	delete buildingSection;

	return result;
}





bool compareScenesMovementPhase(char* preMovementPhaseSceneFileName, LDreference* initialReferenceInPreMovementPhaseScene, LDreference* initialReferenceInThisPhaseStartScene, Player* currentPlayer, char* targetSpritesSceneFileName, bool addSprites)
{
	bool result = true;

	buildingSection = new ModelDetails();	//NEW

	numberOfCombatPointsUsedByPlayerSoFarDuringRound = 0;
	numberOfBuildingPointsUsedByPlayerSoFarDuringRound = 0;
	maxNumBuildingRitualPointsAllowedDuringRound = PLAYER_ROUND_RITUAL_BUILDING_POINTS_ALLOCATED;
	maxNumCombatRitualPointsAllowedDuringRound = PLAYER_ROUND_RITUAL_COMBAT_POINTS_ALLOCATED;

	//initialise sprite list variables
	LDreference* targetSpriteListInitialReference = new LDreference();
	int numTargetSpritesAdded = 0;

	//parsingUnitFileAndCalculatingItsCharacteristics = false; //this is assumed already!
	#ifdef DEBUG
	cout << "DEBUG1a " <<  endl;
	#endif
	bool isAChildOfAMovingReference = false;		//this, the value of this highest level instance of isAChildOfAMovingReference in the compareReferenceCharacteristicsToThoseInPreMovementPhaseSceneReferenceList recursive algorithm is not used.
	int parentUnitSpeed = 0;						//this, the value of this highest level instance of parentUnitSpeed in the compareReferenceCharacteristicsToThoseInPreMovementPhaseSceneReferenceList recursive algorithm is not used.
	if(searchThisPhaseStartSceneReferenceListForReferenceComparisonInitiation(initialReferenceInThisPhaseStartScene, initialReferenceInPreMovementPhaseScene, parentUnitSpeed, isAChildOfAMovingReference, currentPlayer, targetSpritesSceneFileName, targetSpriteListInitialReference, &numTargetSpritesAdded))
	{
		if(!performFinalRoundPointsCalculations(currentPlayer))
		{
			result = false;
		}
	}
	else
	{
		cout << "Player turn failed." << endl;
		result = false;
	}

	if(addSprites)
	{
		if(!addSpriteReferenceListToSceneFile(preMovementPhaseSceneFileName, targetSpritesSceneFileName, targetSpriteListInitialReference, numTargetSpritesAdded))
		{
			result = false;
		}
	}

	delete targetSpriteListInitialReference;
	delete buildingSection;

	return result;
}



bool searchThisPhaseStartSceneReferenceListForReferenceComparisonInitiation(LDreference* referenceInThisPhaseStartSceneFile, LDreference* initialReferenceInPreMovementPhaseScene, int parentUnitSpeed, bool isAChildOfAMovingReference, Player* currentPlayer, char* targetSpritesSceneFileName, LDreference* spriteListInitialReference, int* numTargetSpritesAdded)
{
	bool playerMoveStatus = true;
	LDreference* currentReference = referenceInThisPhaseStartSceneFile;
	while(currentReference->next != NULL)
	{
		#ifdef DEBUG_MOVEMENT_CPP
		cout << "\n*******************************************************************************************" << endl;
		cout << "*******************************************************************************************" << endl;
		cout << "New reference being compared: " << currentReference->name  << endl;
		#endif

		int currentReferenceUnitSpeed = parentUnitSpeed;

		//declare unit1 top level reference for Full unit calculations

		if(!isAChildOfAMovingReference)
		{
			if(currentReference->isSubModelReference)
			{

				LDreference* topLevelReferenceInUnit1 = new LDreference(currentReference->name, currentReference->colour, true);
				//copyReferences(topLevelReferenceInUnit1, currentReference, REFERENCE_TYPE_SUBMODEL);	//this is required to use topLevelReferenceInUnit1 in sprite addition

				copyAllUnitDetails(topLevelReferenceInUnit1->subModelDetails, currentReference->subModelDetails);

				searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(topLevelReferenceInUnit1, currentReference->firstReferenceWithinSubModel, topLevelReferenceInUnit1, true);

				determineUnitTypeAndMinSpeedOfUnitGroup(topLevelReferenceInUnit1->subModelDetails);

				currentReferenceUnitSpeed = topLevelReferenceInUnit1->subModelDetails->movementSpeed;
				#ifdef DEBUG_MOVEMENT_CPP
				cout << "Current reference being compared: " << currentReference->name  << endl;
				cout << "topLevelReferenceInUnit1.subModelDetails->numPerson = " << topLevelReferenceInUnit1.subModelDetails->numPerson << endl;
				cout << "LDreference unit speed based on determineUnitTypeAndMinSpeedOfUnitGroup = " <<  currentReferenceUnitSpeed << endl;
				#endif

				delete topLevelReferenceInUnit1;
			}
		}


		bool currentReferenceIsAMovingReference = false;

		/*	"aPartWithSameName" = a part with same name and colour as part being compared
		aPartWithSameIDMovedAbsoluteSinceLastRound = resultOfComparison[0]
		aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound = resultOfComparison[1]
		aPartWithSameIDHasMovedAndIsAMovingPartAndHasMovedIllegally = resultOfComparison[2]
		aPartWithSameIDMovedRelativeSinceLastRound = resultOfComparison[3]
		*/

		LDreference* referenceInPreviousSceneRefList;
		bool searchResult = true;
		bool unitIDFound = false;
		bool resultOfComparison[4];
		resultOfComparison[0] = false;
		resultOfComparison[1] = false;
		resultOfComparison[2] = false;
		resultOfComparison[3] = false;
		referenceInPreviousSceneRefList = compareReferenceCharacteristicsToThoseInPreMovementPhaseSceneReferenceList(currentReference, initialReferenceInPreMovementPhaseScene, resultOfComparison, currentReferenceUnitSpeed, &unitIDFound, &searchResult);

		if(unitIDFound == true)
		{
			referenceInPreviousSceneRefList->subModelDetails->movementSpeed = currentReferenceUnitSpeed;	//this is required for sprite addition
		}

		if(searchResult == true)
		{
			if(isAChildOfAMovingReference && resultOfComparison[3] && !resultOfComparison[1])	//&& aPartWithSameIDMovedRelativeSinceLastRound && !aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound . NB that the "&& !aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound" condition implies an equivalent part at the expected position (ie a part with no relative movement wrt its parent) has not been found
			{
				cout << "\nError: an invalid move occured. If a parent unit has moved, its children cannot move relative to it\n---" << endl;
				cout << "d:isAChildOfAMovingReference = " << isAChildOfAMovingReference << endl;
				cout << "d:currentReference->name ('parent') = " << currentReference->name << ". 'child' = " << "unknown" << endl;
				cout << "d:Parent unit speed = " << parentUnitSpeed << endl;
				cout << "d:first referenceInThisPhaseStartSceneFile->name = " << referenceInThisPhaseStartSceneFile->name << endl;
				cout << "d:currentReference->firstReferenceWithinSubModel->name = " << currentReference->firstReferenceWithinSubModel->name << endl;
				playerMoveStatus = false;
			}
			else if(!dealWithResultsOfComparison(currentReference, resultOfComparison, currentPlayer, spriteListInitialReference, referenceInPreviousSceneRefList, numTargetSpritesAdded, targetSpritesSceneFileName, unitIDFound, isAChildOfAMovingReference))
			{
				playerMoveStatus = false;
				//currentReferenceMoveAccepted = false;

				//cout << currentReference->name << " move failed." << endl;
			}
		}


		if(resultOfComparison[0]) //aPartWithSameIDMovedAbsoluteSinceLastRound
		{
			currentReferenceIsAMovingReference = true;
		}


		if((currentReference->isSubModelReference))
		{
			if(!searchThisPhaseStartSceneReferenceListForReferenceComparisonInitiation(currentReference->firstReferenceWithinSubModel, initialReferenceInPreMovementPhaseScene, currentReferenceUnitSpeed, currentReferenceIsAMovingReference|isAChildOfAMovingReference, currentPlayer, targetSpritesSceneFileName, spriteListInitialReference, numTargetSpritesAdded))
			{
				playerMoveStatus = false;
			}
		}



		currentReference = currentReference->next;
	}
	return playerMoveStatus;
}

LDreference* compareReferenceCharacteristicsToThoseInPreMovementPhaseSceneReferenceList(LDreference* referenceInThisPhaseStartSceneFileBeingLocated, LDreference* referenceInPreMovementPhaseSceneFile, bool resultOfComparison[], int parentUnitSpeed, bool* unitIDFound, bool* result)
{
	LDreference* currentReference = referenceInPreMovementPhaseSceneFile;
	LDreference* referenceInPreviousSceneFile;

	while(currentReference->next != NULL)
	{
		if(compareSubmodelNamesAndIfSameCheckIfValidMove(referenceInThisPhaseStartSceneFileBeingLocated, currentReference, resultOfComparison, parentUnitSpeed))
		{//unitIDFoundInThisComparison

			if(*unitIDFound == true)
			{
				*result = false;
				cout << "compareReferenceCharacteristicsToThoseInPreMovementPhaseSceneReferenceList: Invalid scene file - multiple units exist with same unit ID" << endl;
			}
			else
			{
				*unitIDFound = true;
				referenceInPreviousSceneFile = currentReference;
			}
		}

		/*Recursive Parsing of submodels support*/
			//if((currentReference->isSubModelReference) && (!currentReference->subModelDetails->numPerson))	/*only parse submodels that are not moveable entities??*/
		if(currentReference->isSubModelReference)
		{
			bool unitIDFoundInChild = false;
			LDreference* referenceOfUnitIDFoundInChild;
			referenceOfUnitIDFoundInChild = compareReferenceCharacteristicsToThoseInPreMovementPhaseSceneReferenceList(referenceInThisPhaseStartSceneFileBeingLocated, currentReference->firstReferenceWithinSubModel, resultOfComparison, parentUnitSpeed, &unitIDFoundInChild, result);
			if(unitIDFoundInChild == true)
			{
				if(*unitIDFound == true)
				{
					*result = false;
					cout << "compareReferenceCharacteristicsToThoseInPreMovementPhaseSceneReferenceList: Invalid scene file - multiple units exist with same unit ID" << endl;
				}
				else
				{
					*unitIDFound = true;
					referenceInPreviousSceneFile = referenceOfUnitIDFoundInChild;
				}
			}
		}

		currentReference = currentReference->next;
	}

	return referenceInPreviousSceneFile;
}


bool compareSubmodelNamesAndIfSameCheckIfValidMove(LDreference* referenceInThisPhaseStartSceneSearchedFor, LDreference* referenceInPreMovementPhaseSceneFoundDuringSearch, bool resultOfComparison[], int parentUnitSpeed)
{
	//cout << "DEBUG: here4" << endl;

	bool unitIDFoundInThisComparison = false;

	/*	"aPartWithSameName" = a part with same name and colour as part being compared
	aPartWithSameIDMovedAbsoluteSinceLastRound = resultOfComparison[0]
	aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound = resultOfComparison[1]
	aPartWithSameIDHasMovedAndIsAMovingPartAndHasMovedIllegally = resultOfComparison[2]
	aPartWithSameIDMovedRelativeSinceLastRound = resultOfComparison[3]
	*/

	bool aPartWithSameIDMovedAbsoluteSinceLastRound = false;
	bool aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound = false;
	bool aPartWithSameIDHasMovedAndIsAMovingPartAndHasMovedIllegally = false;
	bool aPartWithSameIDMovedRelativeSinceLastRound = false;

	#ifdef DEBUG_MOVEMENT_ELABORATE_CPP
	cout << "\n*******************************************************************************************" << endl;
	cout << "D:" <<  referenceInThisPhaseStartSceneSearchedFor->name << " is being compared with " << referenceInPreMovementPhaseSceneFoundDuringSearch->name << endl;
	#endif

	if((referenceInPreMovementPhaseSceneFoundDuringSearch->name == referenceInThisPhaseStartSceneSearchedFor->name) && (referenceInPreMovementPhaseSceneFoundDuringSearch->colour == referenceInThisPhaseStartSceneSearchedFor->colour))
	{

		if(referenceInPreMovementPhaseSceneFoundDuringSearch->isSubModelReference)
		{
			unitIDFoundInThisComparison = true;
		}

		/*assume child objects cannot make relative movements, if parent objects have made relative movements*/
			/*old - try to calculate movement rule obedience based upon absolute movement calculations - difficult and unnecessary*/

		#ifdef DEBUG_MOVEMENT_CPP
		cout << "D: LDreference names and colours have been found to be the same" << endl;
		#endif

		int preMovementPhaseSceneFileRelativeX = (int)referenceInPreMovementPhaseSceneFoundDuringSearch->relativePosition.x;
		int preMovementPhaseSceneFileRelativeY = (int)referenceInPreMovementPhaseSceneFoundDuringSearch->relativePosition.y;
		int preMovementPhaseSceneFileRelativeZ = (int)referenceInPreMovementPhaseSceneFoundDuringSearch->relativePosition.z;

		int thisPhaseStartSceneFileRelativeX = (int)referenceInThisPhaseStartSceneSearchedFor->relativePosition.x;
		int thisPhaseStartSceneFileRelativeY = (int)referenceInThisPhaseStartSceneSearchedFor->relativePosition.y;
		int thisPhaseStartSceneFileRelativeZ = (int)referenceInThisPhaseStartSceneSearchedFor->relativePosition.z;

		if((thisPhaseStartSceneFileRelativeX == preMovementPhaseSceneFileRelativeX) && (thisPhaseStartSceneFileRelativeY == preMovementPhaseSceneFileRelativeY) && (thisPhaseStartSceneFileRelativeZ == preMovementPhaseSceneFileRelativeZ))
		{
		}
		else
		{
			aPartWithSameIDMovedRelativeSinceLastRound = true;

			#ifdef DEBUG_MOVEMENT_CPP
			cout << "D: aPartWithSameIDMovedRelativeSinceLastRound = true;" << endl;
			#endif
			/*
			cout << "\nD:referenceInThisPhaseStartSceneSearchedFor->name = " << referenceInThisPhaseStartSceneSearchedFor->name << endl;
			cout << "D:parentUnitSpeed = " << parentUnitSpeed << endl;
			cout << "D:aPartWithSameIDMovedRelativeSinceLastRound = " << aPartWithSameIDMovedRelativeSinceLastRound << endl;
			cout << "D:preMovementPhaseSceneFileRelativeX = " << preMovementPhaseSceneFileRelativeX << endl;
			cout << "D:preMovementPhaseSceneFileRelativeY = " << preMovementPhaseSceneFileRelativeY << endl;
			cout << "D:preMovementPhaseSceneFileRelativeZ = " << preMovementPhaseSceneFileRelativeZ << endl;
			cout << "D:thisPhaseStartSceneFileRelativeX = " << thisPhaseStartSceneFileRelativeX << endl;
			cout << "D:thisPhaseStartSceneFileRelativeY = " << thisPhaseStartSceneFileRelativeY << endl;
			cout << "D:thisPhaseStartSceneFileRelativeZ = " << thisPhaseStartSceneFileRelativeZ << endl;
			*/
		}

		int preMovementPhaseSceneFileAbsoluteX = (int)referenceInPreMovementPhaseSceneFoundDuringSearch->absolutePosition.x;
		int preMovementPhaseSceneFileAbsoluteY = (int)referenceInPreMovementPhaseSceneFoundDuringSearch->absolutePosition.y;
		int preMovementPhaseSceneFileAbsoluteZ = (int)referenceInPreMovementPhaseSceneFoundDuringSearch->absolutePosition.z;

		int thisPhaseStartSceneFileAbsoluteX = (int)referenceInThisPhaseStartSceneSearchedFor->absolutePosition.x;
		int thisPhaseStartSceneFileAbsoluteY = (int)referenceInThisPhaseStartSceneSearchedFor->absolutePosition.y;
		int thisPhaseStartSceneFileAbsoluteZ = (int)referenceInThisPhaseStartSceneSearchedFor->absolutePosition.z;


		if((thisPhaseStartSceneFileAbsoluteX == preMovementPhaseSceneFileAbsoluteX) && (thisPhaseStartSceneFileAbsoluteY == preMovementPhaseSceneFileAbsoluteY) && (thisPhaseStartSceneFileAbsoluteZ == preMovementPhaseSceneFileAbsoluteZ))
		{
			aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound = true;
			//cout << "D: aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound = true; (abs. positions are the same)" << endl;
			//cout << "referenceInThisPhaseStartSceneSearchedFor->name" << referenceInThisPhaseStartSceneSearchedFor->name << endl;
			//cout << referenceInThisPhaseStartSceneSearchedFor->name << endl;
			#ifdef DEBUG_MOVEMENT_CPP
			cout << "D: aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound = true; (abs. positions are the same)" << endl;
			#endif
			//cout << "D:the following aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound: " <<  referenceInThisPhaseStartSceneSearchedFor->name << endl;
		}
		else
		{
			aPartWithSameIDMovedAbsoluteSinceLastRound = true;
			#ifdef DEBUG_MOVEMENT_CPP
			cout << "D: aPartWithSameIDMovedAbsoluteSinceLastRound = true;" << endl;
			#endif


			int maxTravelDistance;

			//maxTravelDistance = maxInt(referenceInThisPhaseStartSceneSearchedFor->subModelDetails->movementSpeed, parentUnitSpeed);
			maxTravelDistance = parentUnitSpeed;

			if(maxTravelDistance > 0)
			{
				double XYZDisplacementFromOriginalToPartPotentiallyFound;
				double differenceXDouble = ((double)thisPhaseStartSceneFileAbsoluteX - (double)preMovementPhaseSceneFileAbsoluteX)/(LDRAW_UNITS_PER_CM);
				double differenceYDouble = ((double)thisPhaseStartSceneFileAbsoluteY - (double)preMovementPhaseSceneFileAbsoluteY)/(LDRAW_UNITS_PER_CM);
				double differenceZDouble = ((double)thisPhaseStartSceneFileAbsoluteZ - (double)preMovementPhaseSceneFileAbsoluteZ)/(LDRAW_UNITS_PER_CM);
				XYZDisplacementFromOriginalToPartPotentiallyFound = sqrt(pow(differenceXDouble, 2) + pow(differenceYDouble, 2) + pow(differenceZDouble, 2));

				if((XYZDisplacementFromOriginalToPartPotentiallyFound) <= maxTravelDistance)
				{
					aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound = true;
					//cout << "D: aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound = true; (abs. positions are different but move is in range)" << endl;
					//cout << "referenceInThisPhaseStartSceneSearchedFor->name" << referenceInThisPhaseStartSceneSearchedFor->name << endl;
					//cout << referenceInThisPhaseStartSceneSearchedFor->name << endl;

					#ifdef DEBUG_MOVEMENT_CPP
					cout << "D: aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound = true; (abs. positions are different but move is in range)" << endl;
					#endif
					#ifdef DEBUG_MOVEMENT_CPP_DISPLACEMENT
					cout << "D: XYZDisplacementFromOriginalToPartPotentiallyFound = " << XYZDisplacementFromOriginalToPartPotentiallyFound << endl;
					#endif
					//cout << "d:2 the following aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound: " <<  referenceInThisPhaseStartSceneSearchedFor->name << endl;
				}
				else
				{

					aPartWithSameIDHasMovedAndIsAMovingPartAndHasMovedIllegally = true;
					#ifdef DEBUG_MOVEMENT_CPP
					cout << "D: aPartWithSameIDHasMovedAndIsAMovingPartAndHasMovedIllegally = true;" << endl;
					#endif
					/*
					cout << "D: Error: an invalid move occured. " << referenceInThisPhaseStartSceneSearchedFor->name << " was moved beyond its maximum distance allowed)" << endl;
					cout << "D: Error: Distance moved = " << XYZDisplacementFromOriginalToPartPotentiallyFound << ". Distance allowed = " << maxTravelDistance << endl;
					cout << "D: [this error may be wrong if there are more than reference to a same unit in a scene]" << endl;
					*/
				}
			}
			else
			{

			}
		}
	}

	if(aPartWithSameIDMovedAbsoluteSinceLastRound)
	{
		resultOfComparison[0] = true;
	}
	if(aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound)
	{
		resultOfComparison[1] = true;
	}
	if(aPartWithSameIDHasMovedAndIsAMovingPartAndHasMovedIllegally)
	{
		resultOfComparison[2] = true;
	}
	if(aPartWithSameIDMovedRelativeSinceLastRound)
	{
		resultOfComparison[3] = true;
	}

	return unitIDFoundInThisComparison;
}








bool dealWithResultsOfComparison(LDreference* referenceInThisPhaseStartSceneFile, bool resultOfComparison[], Player* currentPlayer, LDreference* spriteListInitialReference, LDreference* referenceInPreMovementPhaseSceneFile, int* numTargetSpritesAdded, char* targetSpritesSceneFileName, bool unitIDFound, bool isChildOfMovingReference)
{

	/*	"aPartWithSameName" = a part with same name and colour as part being compared
	aPartWithSameIDMovedAbsoluteSinceLastRound = resultOfComparison[0]
	aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound = resultOfComparison[1]
	//aPartWithSameIDHasMovedAndIsAMovingPartAndHasMovedIllegally = resultOfComparison[2]
	//aPartWithSameIDMovedRelativeSinceLastRound = resultOfComparison[3]
	*/

	bool aPartWithSameIDMovedAbsoluteSinceLastRound = resultOfComparison[0];
	bool aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound = resultOfComparison[1];
	bool aPartWithSameIDHasMovedAndIsAMovingPartAndHasMovedIllegally = resultOfComparison[2];
	//bool aPartWithSameIDMovedRelativeSinceLastRound = resultOfComparison[3];
	bool playerMoveStatus = true;

	if(aPartWithSameIDHasNotMovedOrMovedLegallySinceLastRound)
	{//ModelDetails was found in the same [or an allowed movement] location in [/ as compared to] the original scene file

		if(aPartWithSameIDMovedAbsoluteSinceLastRound)
		{

			//if(partIfMovedHasMovedWithinItsMaximumDistanceAllowed)		//this is implied
			//{
				if(referenceInThisPhaseStartSceneFile->isSubModelReference)
				{
					//if(unitIDFound == true)			//this is implied via isSubModelReference == true and the fact the unit ID has been found
					//{
						if(obtainReferencePlayerID(referenceInThisPhaseStartSceneFile) == (currentPlayer->id))
						{

							cout << referenceInThisPhaseStartSceneFile->name << " was found to have moved between scenes and has had its move accepted" << endl;

						//#define TEMP_NO_MOVEMENTSPRITES
						#ifndef TEMP_NO_MOVEMENTSPRITES
							//add target sprite info
							bool result = true;
							bool addTextualSpriteInfo = false;
							bool addRangeSpriteInfo = true;
							bool addTargetSpriteInfo = true;
							vec eyeCoords;
							eyeCoords.x = 0.0;
							eyeCoords.y = 0.0;
							eyeCoords.z = 0.0;

							/*
							cout << "cc1" << endl;
							cout << "(unitReference->name) = " << (referenceInPreMovementPhaseSceneFile->name) << endl;
							//cout << "spriteReferenceFileName = " << spriteReferenceFileName << endl;
							//cout << "spriteListInitialReference->name = " << spriteListInitialReference->name << endl;
							cout << "(unitReference->absolutePosition.x) = " << (referenceInPreMovementPhaseSceneFile->absolutePosition.x) << endl;
							cout << "cc2" << endl;
							*/
							if(isChildOfMovingReference == false)
							{
								if(!LRRCdetermineSpriteInfoAndAddSpriteToSpriteRefList(referenceInPreMovementPhaseSceneFile, referenceInThisPhaseStartSceneFile, spriteListInitialReference, &eyeCoords, numTargetSpritesAdded, targetSpritesSceneFileName, addTextualSpriteInfo, addRangeSpriteInfo, addTargetSpriteInfo, currentPlayer->currentPhase, currentPlayer->id))
								{
									result = false;
								}
							}
						#endif

						}
						else
						{
							cout << referenceInThisPhaseStartSceneFile->name << " was found to have moved between scenes and has not had its move accepted" << endl;
							cout << "\tOnly current player units can move during the current players turn" << endl;
							playerMoveStatus = false;

						}
					//}
					//else
					//{
					//	cout << "DEBUG: Old Movement Method says current submodel part was found, but it was not according to the new method" << endl;
					//}

				}
			//}

		}
		else
		{
			//if(partIfMovedHasMovedWithinItsMaximumDistanceAllowed)		//this is implied
			//{
				if(obtainReferencePlayerID(referenceInThisPhaseStartSceneFile) == (currentPlayer->id))
				{
					if(referenceInThisPhaseStartSceneFile->isSubModelReference)
					{
						cout << referenceInThisPhaseStartSceneFile->name << " was detected to have remained stationary between scenes" << endl;
					}
				}
			//}
		}
	}
	else
	{//aPartWithSameIDHasMovedIllegallySinceLastRound

		if(aPartWithSameIDHasMovedAndIsAMovingPartAndHasMovedIllegally)
		{//aPartWithSameIDHasMovedIllegallySinceLastRoundAndIsAllowedToMove

			cout << referenceInThisPhaseStartSceneFile->name << " was detected to have moved beyond its maximum allowed distance" << endl;
			playerMoveStatus = false;
		}
		else
		{//aPartWithSameIDHasMovedIllegallySinceLastRoundAndIsNotAMovingPart [ie the part is a building or a building part (not a unit)]

			int thisPhaseStartSceneFileX = (int)referenceInThisPhaseStartSceneFile->absolutePosition.x;			/*Note to self; Scene check code - option 3 - needs to be updated with absolute submodel position detection- by taking into account position of parent model. See advanced combat calulations for relevant code to do this*/
			int thisPhaseStartSceneFileY = (int)referenceInThisPhaseStartSceneFile->absolutePosition.y;
			int thisPhaseStartSceneFileZ = (int)referenceInThisPhaseStartSceneFile->absolutePosition.z;

			//see if the the new LDreference/ModelDetails lies within the players starting post.
			double XYZDisplacementFromStartingPost;
			double differenceXDouble = ((double)thisPhaseStartSceneFileX - (double)currentPlayer->startPosition.x)/(LDRAW_UNITS_PER_CM);
			double differenceYDouble = ((double)thisPhaseStartSceneFileY - (double)currentPlayer->startPosition.y)/(LDRAW_UNITS_PER_CM);
			double differenceZDouble = ((double)thisPhaseStartSceneFileZ - (double)currentPlayer->startPosition.z)/(LDRAW_UNITS_PER_CM); //Z distance modifier
			XYZDisplacementFromStartingPost = sqrt(pow(differenceXDouble, 2) + pow(differenceYDouble, 2) + pow(differenceZDouble, 2));

			if(XYZDisplacementFromStartingPost <= PLAYER_MAXIMUM_BUILD_DISTANCE_FROM_STARTING_POST)
			{
				//see if the user has enough A)Budget and B)ritual round points to complete the new LDreference creation

				if(referenceInThisPhaseStartSceneFile->isSubModelReference)
				{
					//cout << "referenceInThisPhaseStartSceneFile->name" << referenceInThisPhaseStartSceneFile->name << endl;

					//Building Calculations	- During parsing these are calculated now on a per part basis and a finalisation basis (instead of a per unit parsed basis). At this stage the user is just notified if a new building has been added to the scene
					int referencedSubmodelTotalDefinateBuildingSpecificPoints = 0;

					XMLrulesClass* currentReferenceRulesClass;
					RecordClass* currentReferenceRecordClass;

					currentReferenceRulesClass = LRRCrulesBuildingDetails;
					currentReferenceRecordClass = referenceInThisPhaseStartSceneFile->subModelDetails->recordOfBuildingDetails;
					while(currentReferenceRecordClass->next != NULL)
					{
						if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
						{
							referencedSubmodelTotalDefinateBuildingSpecificPoints+=(currentReferenceRulesClass->attribute4* currentReferenceRecordClass->numberOfThisPartIDInTheUnit);
						}

						currentReferenceRecordClass = currentReferenceRecordClass->next;
						currentReferenceRulesClass = currentReferenceRulesClass->next;
					}

					if(referencedSubmodelTotalDefinateBuildingSpecificPoints > 0)
					{
						//cout << referenceInThisPhaseStartSceneFile->name << " is considered a new Building in the scene (costs = " << referencedSubmodelTotalBuildingPoints  << " points)." << endl;
						cout << referenceInThisPhaseStartSceneFile->name << " is considered a new Building in the scene" << endl;
					}
					//numberOfBuildingPointsUsedByPlayerSoFarDuringRound = numberOfBuildingPointsUsedByPlayerSoFarDuringRound + referencedSubmodelTotalBuildingPoints;



					/*Combat Calculations*/	/*NB scene comparison unit cost calculations are performed without inherited values (equipment,speed etc) so as not to double count*/
					int combatBaseUnitModifer;
					int referencedSubmodelTotalCombatPoints;

					/*ModelDetails Calculations*/		/*NB scene comparison unit cost calculations are performed without inherited values (equipment,speed etc) so as not to double count*/
					combatBaseUnitModifer = referenceInThisPhaseStartSceneFile->subModelDetails->movementSpeed;
					//referencedSubmodelTotalCombatPoints = int(combatBaseUnitModifer + (referenceInThisPhaseStartSceneFile->subModelDetails->attackTotal+1)*DICE_MODIFIER + (referenceInThisPhaseStartSceneFile->subModelDetails->defenceTotal+1)*DICE_MODIFIER);
					referencedSubmodelTotalCombatPoints = int(combatBaseUnitModifer + (referenceInThisPhaseStartSceneFile->subModelDetails->attackTotal)*DICE_MODIFIER + (referenceInThisPhaseStartSceneFile->subModelDetails->defenceTotal)*DICE_MODIFIER);

					if(referencedSubmodelTotalCombatPoints > 0)
					{
						cout << referenceInThisPhaseStartSceneFile->name << " is considered a new Model in the scene (costs = " << referencedSubmodelTotalCombatPoints  << " points)." << endl;
					}

					numberOfCombatPointsUsedByPlayerSoFarDuringRound = numberOfCombatPointsUsedByPlayerSoFarDuringRound + referencedSubmodelTotalCombatPoints;
				}
				else
				{
					cout << "New LDreference detected in scene " << referenceInThisPhaseStartSceneFile->name << endl;
					//cout << "\td:Relative Positions: X = " << referenceInThisPhaseStartSceneFile->relativePosition.x << ", Y = " << referenceInThisPhaseStartSceneFile->relativePosition.y << ", Z = " << referenceInThisPhaseStartSceneFile->relativePosition.z << endl;


					const char* constantCharStarString = referenceInThisPhaseStartSceneFile->name.c_str();
					char* charStarString = new char[referenceInThisPhaseStartSceneFile->name.size()+1];
					strcpy(charStarString, constantCharStarString);

					updateUnitDetailsWithBuildingDetails(charStarString, buildingSection);

					delete charStarString;
					//delete charStarString;
					//delete constantCharStarString;


				}
			}
			else
			{
				cout << referenceInThisPhaseStartSceneFile->name << " cannot be considered a new ModelDetails/LDreference in the scene, as it cannot be placed greater than " << PLAYER_MAXIMUM_BUILD_DISTANCE_FROM_STARTING_POST << " units from the players starting post" << endl;
				playerMoveStatus = false;
			}
		}
	}


	return playerMoveStatus;
}








//Building Options

bool performFinalRoundPointsCalculations(Player* currentPlayer)
{
	bool result = true;

	//Finalise Building Calculations	- During parsing these are calculated now on a per part basis and a finalisation basis (instead of a per unit parsed basis)
	int referencedSubmodelTotalBuildingPoints;
	int referencedSubmodelTotalDefinateBuildingSpecificPoints;

	int pointsUsedByBuildingBricks = 0;
	int pointsUsedByBuildingOthers = buildingSection->numBuildingOther* BUILDING_DEFAULT_MOD;

	XMLrulesClass* currentReferenceRulesClass = LRRCrulesBuildingDetails;
	RecordClass* currentReferenceRecordClass = buildingSection->recordOfBuildingDetails;
	while(currentReferenceRecordClass->next != NULL)
	{
		if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
		{
			pointsUsedByBuildingBricks = pointsUsedByBuildingBricks + (currentReferenceRecordClass->numberOfThisPartIDInTheUnit* currentReferenceRulesClass->attribute4);
		}

		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	referencedSubmodelTotalBuildingPoints = pointsUsedByBuildingBricks+pointsUsedByBuildingOthers;
	referencedSubmodelTotalDefinateBuildingSpecificPoints = pointsUsedByBuildingBricks;

	numberOfBuildingPointsUsedByPlayerSoFarDuringRound = referencedSubmodelTotalBuildingPoints;


/*
	int referencedSubmodelTotalBuildingPoints;
	int referencedSubmodelTotalDefinateBuildingSpecificPoints;
	int pointsUsedByBuildingWalls;
	int numberOfFreeBuldingBricksBasedOnBuildingWalls;
	int pointsUsedByBuildingBricks;
	int pointsUsedByBuildingOthers;
	int pointsUsedByBuildingRocks;
	int pointsUsedByBuildingBattlements;

	pointsUsedByBuildingWalls = totalNumBuildingWallWindow + totalNumBuildingWallCorner;
	numberOfFreeBuldingBricksBasedOnBuildingWalls = pointsUsedByBuildingWalls*NUMBER_OF_FREE_BUILDING_BRICKS_BASED_ON_BUILDING_WALLS_RATIO;
	pointsUsedByBuildingBricks = totalNumBuildingBricks - numberOfFreeBuldingBricksBasedOnBuildingWalls;
	pointsUsedByBuildingOthers = totalNumBuildingOther* BUILDING_DEFAULT_MOD;
	pointsUsedByBuildingRocks = totalNumBuildingRockLarge*BUILDING_ROCK_LARGE_MOD + totalNumBuildingRockSmall*BUILDING_ROCK_SMALL_MOD;
	pointsUsedByBuildingBattlements = totalNumBuildingBattlementTower*BUILDING_BATTLEMENT_TOWER_MOD;

	referencedSubmodelTotalBuildingPoints =  pointsUsedByBuildingBattlements+pointsUsedByBuildingWalls+pointsUsedByBuildingBricks+pointsUsedByBuildingRocks+pointsUsedByBuildingOthers;
	referencedSubmodelTotalDefinateBuildingSpecificPoints = referencedSubmodelTotalBuildingPoints - pointsUsedByBuildingOthers;

	numberOfBuildingPointsUsedByPlayerSoFarDuringRound = referencedSubmodelTotalBuildingPoints;

*/


	if(numberOfBuildingPointsUsedByPlayerSoFarDuringRound > maxNumBuildingRitualPointsAllowedDuringRound)
	{
		cout << "Invalid Move by Player detected based on Building ritual points allocated to the player for this round" << endl;
		cout << "numberOfBuildingPointsUsedByPlayerSoFarDuringRound = " << numberOfBuildingPointsUsedByPlayerSoFarDuringRound << endl;
		cout << "maxNumBuildingRitualPointsAllowedDuringRound = " << maxNumBuildingRitualPointsAllowedDuringRound << endl;
		result = false;
	}
	if(numberOfCombatPointsUsedByPlayerSoFarDuringRound >  maxNumCombatRitualPointsAllowedDuringRound)
	{
		cout << "Invalid Move by Player detected based on ModelDetails ritual points allocated to the player for this round" << endl;
		cout << "numberOfCombatPointsUsedByPlayerSoFarDuringRound = " << numberOfCombatPointsUsedByPlayerSoFarDuringRound << endl;
		cout << "maxNumCombatRitualPointsAllowedDuringRound = " << maxNumCombatRitualPointsAllowedDuringRound << endl;
		result = false;
	}
	if(numberOfCombatPointsUsedByPlayerSoFarDuringRound+numberOfBuildingPointsUsedByPlayerSoFarDuringRound > currentPlayer->credits)
	{
		cout << "Invalid Move by Player detected based on ModelDetails+Building points and players bank account" << endl;
		cout << "numberOfBuildingPointsUsedByPlayerSoFarDuringRound = " << numberOfBuildingPointsUsedByPlayerSoFarDuringRound << endl;
		cout << "numberOfCombatPointsUsedByPlayerSoFarDuringRound = " << numberOfCombatPointsUsedByPlayerSoFarDuringRound << endl;
		cout << "currentPlayer->credits = " << currentPlayer->credits << endl;
		result = false;
	}

#ifdef GAME_MOVEMENT_PHASE_DISPLAY_RESULT
	if(result)
	{

		cout << "The movement turn of Player for this round has been accepted." << endl;
		cout << "Bank Account of player before move = " << currentPlayer->credits << endl;
		currentPlayer->credits = currentPlayer->credits - (numberOfCombatPointsUsedByPlayerSoFarDuringRound+numberOfBuildingPointsUsedByPlayerSoFarDuringRound);
		cout << "Bank Account of player after move = " << currentPlayer->credits << endl;
		cout << "\tnumberOfBuildingPointsUsedByPlayerSoFarDuringRound = " << numberOfBuildingPointsUsedByPlayerSoFarDuringRound << endl;
		cout << "\tnumberOfCombatPointsUsedByPlayerSoFarDuringRound = " << numberOfCombatPointsUsedByPlayerSoFarDuringRound << endl;
		cout << "\t\troundsBuildingRitualPointsAllocated = " << maxNumBuildingRitualPointsAllowedDuringRound << endl;
		cout << "\t\troundsCombatRitualPointsAllocated = " << maxNumCombatRitualPointsAllowedDuringRound << endl;
		cout << "\t\troundsSpareBuildingRitualPoints= " << (maxNumBuildingRitualPointsAllowedDuringRound - numberOfBuildingPointsUsedByPlayerSoFarDuringRound) << endl;
		cout << "\t\troundsSpareCombatRitualPoints = " << (maxNumCombatRitualPointsAllowedDuringRound -  numberOfCombatPointsUsedByPlayerSoFarDuringRound) << endl;
	}
#endif

	return result;
}






