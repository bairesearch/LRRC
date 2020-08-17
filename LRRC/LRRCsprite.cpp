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
 * File Name: LRRCsprite.cpp
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
/*This code allows the addition of a sprite into a given scene file where a sprite is a paragraph of text. [The text is to be rendered in 3D, and point towards the user POV - see CHECK THIS for missing code]*/


#include "LRRCglobalDefs.h"


#include "LRRCsprite.h"
#include "LDsprite.h"
#include "LDparser.h"
#include "math.h"
#include "LRRCcombat.h"
#include "LDreferenceManipulation.h"
#include "SHAREDvector.h"


#include "XMLrulesClass.h"





static string SPRITE_SUBMODEL_RANGE_FILE_NAME;
static int SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_TRANS;
static int SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_TRANS;
static int SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_TRANS;
static int SPRITE_SUBMODEL_RANGE_TARGET_SELECTION_COLOUR_TRANS;
static int SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_OPAQ;
static int SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_OPAQ;
static int SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_OPAQ;
static int SPRITE_SUBMODEL_RANGE_TARGET_SELECTION_COLOUR_OPAQ;
static double SPRITE_SUBMODEL_RANGE_SCALE_FACTOR;


void fillInLRRCSpriteExternVariables()
{
	RulesClass * currentReferenceRulesClass = LRRCrulesSprite;

	while(currentReferenceRulesClass->next != NULL)
	{
		if(currentReferenceRulesClass->name == SPRITE_SUBMODEL_RANGE_FILE_NAME_NAME)
		{
			SPRITE_SUBMODEL_RANGE_FILE_NAME = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_TRANS_NAME)
		{
			SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_TRANS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_TRANS_NAME)
		{
			SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_TRANS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_TRANS_NAME)
		{
			SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_TRANS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_SUBMODEL_RANGE_TARGET_SELECTION_COLOUR_TRANS_NAME)
		{
			SPRITE_SUBMODEL_RANGE_TARGET_SELECTION_COLOUR_TRANS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_OPAQ_NAME)
		{
			SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_OPAQ = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_OPAQ_NAME)
		{
			SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_OPAQ = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_OPAQ_NAME)
		{
			SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_OPAQ = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_SUBMODEL_RANGE_TARGET_SELECTION_COLOUR_OPAQ_NAME)
		{
			SPRITE_SUBMODEL_RANGE_TARGET_SELECTION_COLOUR_OPAQ = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_SUBMODEL_RANGE_SCALE_FACTOR_NAME)
		{
			SPRITE_SUBMODEL_RANGE_SCALE_FACTOR = currentReferenceRulesClass->fractionalValue;
		}

		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	fillInLDSpriteExternVariables();
}
/*top level sprite routines - required for independent LRRCsprite.cpp calculations*/

	//NB this function does not add round's player's phase specific [movement/attack] sprites - these have to be added in LRRCmovement.cpp, and LRRCgame.cpp functions respectively
bool LRRCaddUnitDetailsSpritesToSceneFile(char* sceneFileName, char* sceneFileNameWithSprites, bool addTextualSpriteInfo, bool addRangeSpriteInfo, int currentPhase, int currentPlayerTurn)
{
	bool result = true;

	//declare initial scene references
	Reference * initialReferenceInSceneFile = new Reference();
	Reference * topLevelReferenceInSceneFile = new Reference(true);		//The model details information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function

	//parse the scene files and build Reference linked lists
	if(!parseFile(sceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, false))
	{//file does not exist
		cout << "The file: " << sceneFileName << " does not exist in the directory" << endl;
		return false;
	}
	//cout << "DEBUG: b1" << endl;

	int numSpritesAdded = 0;
	vec eyeCoords;
	eyeCoords.x = 0.0;
	eyeCoords.y = 0.0;
	eyeCoords.z = 0.0;

	Reference * spriteListInitialReference = new Reference();
	LRRCsearchSceneRefListAddUnitDetailsSpriteForSubmodels(initialReferenceInSceneFile, spriteListInitialReference, &eyeCoords, &numSpritesAdded, sceneFileNameWithSprites, initialReferenceInSceneFile, addTextualSpriteInfo, addRangeSpriteInfo, currentPhase, currentPlayerTurn);

	if(!addSpriteReferenceListToSceneFile(sceneFileName, sceneFileNameWithSprites, spriteListInitialReference, numSpritesAdded))
	{
		result = false;
	}

	delete initialReferenceInSceneFile;
	delete topLevelReferenceInSceneFile;
	delete spriteListInitialReference;

	return result;
}

bool LRRCaddUnitDetailsSpritesToScene(char* sceneFileName, char* sceneFileNameWithSprites, bool addTextualSpriteInfo, bool addRangeSpriteInfo, int currentPhase, int currentPlayerTurn, Reference * firstReferenceInScene)
{
	bool result = true;

	//declare initial scene references
	Reference * initialReferenceInSceneFile = firstReferenceInScene;

	int numSpritesAdded = 0;
	vec eyeCoords;
	eyeCoords.x = 0.0;
	eyeCoords.y = 0.0;
	eyeCoords.z = 0.0;

	Reference * spriteListInitialReference = new Reference();
	LRRCsearchSceneRefListAddUnitDetailsSpriteForSubmodels(initialReferenceInSceneFile, spriteListInitialReference, &eyeCoords, &numSpritesAdded, sceneFileNameWithSprites, initialReferenceInSceneFile, addTextualSpriteInfo, addRangeSpriteInfo, currentPhase, currentPlayerTurn);

	if(!addSpriteReferenceListToSceneFile(sceneFileName, sceneFileNameWithSprites, spriteListInitialReference, numSpritesAdded))
	{
		result = false;
	}

	delete spriteListInitialReference;

	return result;
}




//preconditions; assumes scene file has more than 1 person
void LRRCsearchSceneRefListAddUnitDetailsSpriteForSubmodels(Reference * referenceInSceneFile, Reference * spriteListInitialReference, vec * eyeCoords, int * numSpritesAdded, char* sceneFileName, Reference * initialReferenceInSceneFile, bool addTextualSpriteInfo, bool addRangeSpriteInfo, int currentPhase, int currentPlayerTurn)
{
	//cout << "here03" << endl;

	Reference * unusedTargetReference;
	bool addTargetSpriteInfo = false;

	bool result = true;

	Reference * currentReference = referenceInSceneFile;

	while(currentReference->next != NULL)
	{
		if(currentReference->isSubModelReference)
		{
			performFinalUnitClassCalculations(currentReference->subModelDetails);

			/*a)  parse down the tree from the unit. NB this does not allow for copying of parent combat relevant items into child unit
				extract from performCloseCombatNormal*/

			//cout << "here031" << endl;

			//declare current submodel top level reference
			Reference * topLevelReferenceInCurrentSubmodel = new Reference(currentReference->name, currentReference->colour, true);
			//cout << "here0311" << endl;
				copyReferencePosition(topLevelReferenceInCurrentSubmodel, currentReference);
				copyAllUnitDetails(topLevelReferenceInCurrentSubmodel->subModelDetails, currentReference->subModelDetails);
				//cout << "here0312" << endl;
			searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(topLevelReferenceInCurrentSubmodel, currentReference->firstReferenceWithinSubModel, topLevelReferenceInCurrentSubmodel, true);
				//cout << "here0313" << endl;
			determineUnitTypeAndMinSpeedOfUnitGroup(topLevelReferenceInCurrentSubmodel->subModelDetails);
				//cout << "here0314" << endl;
			performFinalUnitClassCalculations(topLevelReferenceInCurrentSubmodel->subModelDetails);


			//cout << "topLevelReferenceInCurrentSubmodel->colour = " << topLevelReferenceInCurrentSubmodel->colour << endl;
			//cout << "here032" << endl;

			/*recursive parsing enabled*/
			if(topLevelReferenceInCurrentSubmodel->subModelDetails->numPerson > 1)
			{
				//cout << "DEBUG 1c" << endl;
				LRRCsearchSceneRefListAddUnitDetailsSpriteForSubmodels(currentReference->firstReferenceWithinSubModel, spriteListInitialReference, eyeCoords, numSpritesAdded, sceneFileName, initialReferenceInSceneFile, addTextualSpriteInfo, addRangeSpriteInfo, currentPhase, currentPlayerTurn);
				//cout << "DEBUG 1d" << endl;
			}

			//cout << "here033" << endl;

			//cout << "DEBUG topLevelReferenceInCurrentSubmodel->subModelDetails->unitType = " << topLevelReferenceInCurrentSubmodel->subModelDetails->unitType << endl;
			//cout << "DEBUG topLevelReferenceInCurrentSubmodel->subModelDetails->numPerson  = " << topLevelReferenceInCurrentSubmodel->subModelDetails->numPerson  << endl;

			/*
			OLD;
				only adds sprites to units with either an attack and defence value, a defence and movement value; therefore sprites are not currently added for stationary catapults
				if(((topLevelReferenceInCurrentSubmodel->subModelDetails->defenceTotal > 0) && (topLevelReferenceInCurrentSubmodel->subModelDetails->closeCombatAttackTotal > 0)) || ((topLevelReferenceInCurrentSubmodel->subModelDetails->defenceTotal > 0) && (topLevelReferenceInCurrentSubmodel->subModelDetails->movementSpeed > 0)))
				NB assumes horses do not have a defence value
			*/

			if((topLevelReferenceInCurrentSubmodel->subModelDetails->numPerson == 1) || ((topLevelReferenceInCurrentSubmodel->subModelDetails->unitType > UNIT_TYPE_HORSE_AND_SADDLE) && (topLevelReferenceInCurrentSubmodel->subModelDetails->numPerson > 0)))
			{
				//cout << "here033" << endl;
				if(!LRRCdetermineSpriteInfoAndAddSpriteToSpriteRefList(topLevelReferenceInCurrentSubmodel, unusedTargetReference, spriteListInitialReference, eyeCoords, numSpritesAdded, sceneFileName, addTextualSpriteInfo, addRangeSpriteInfo, addTargetSpriteInfo, currentPhase, currentPlayerTurn))
				{
					result = false;
				}
			}

			delete topLevelReferenceInCurrentSubmodel;
		}

		currentReference = currentReference->next;
	}
}














































/*medium level sprite routines - these can be used by LRRCsprite.cpp top level routines or by LRRCgame.cpp routines*/

bool LRRCdetermineSpriteInfoAndAddSpriteToSpriteRefList(Reference * unitReference, Reference * targetReference, Reference * spriteListInitialReference, vec * eyeCoords, int * numSpritesAdded, char* sceneFileName, bool addTextualSpriteInfo, bool addRangeSpriteInfo, bool addTargetSpriteInfo, int currentPhase, int currentPlayerTurn)
{
	bool result = true;

	//cout << "here0" << endl;

	/*
	cout << "bb1" << endl;
	cout << "(unitReference->name) = " << (unitReference->name) << endl;
	//cout << "spriteReferenceFileName = " << spriteReferenceFileName << endl;
	//cout << "spriteListInitialReference->name = " << spriteListInitialReference->name << endl;
	cout << "(unitReference->absolutePosition.x) = " << (unitReference->absolutePosition.x) << endl;
	cout << "bb2" << endl;
	*/




	//increment numSpritesAdded
#ifdef DEBUG_ADD_INDIVIDUAL_SPRITES
	*numSpritesAdded = (*numSpritesAdded + 1);
#endif


	//cout << "here1" << endl;

	int * numSpritesAddedLocal;
	Reference * spriteSubmodelCurrentReference;
#ifdef DEBUG_ADD_INDIVIDUAL_SPRITES
	//add sprite info [textual + range info] to reference list:
	Reference * spriteSubmodelInitialReference = new Reference();
	spriteSubmodelCurrentReference = spriteSubmodelInitialReference;
	int numSpritesAddedLocalUnused = 0;
	numSpritesAddedLocal = &numSpritesAddedLocalUnused;
#else
	numSpritesAddedLocal = numSpritesAdded;
	spriteSubmodelCurrentReference = search1DRefListFindLastReference(spriteListInitialReference);
#endif

	//cout << "spriteSubmodelCurrentReference->deformationMatrix->a.x = " << (&((&(spriteSubmodelCurrentReference->deformationMatrix))->a))->x << endl;


	if(addTextualSpriteInfo)	//always if addTextualSpriteInfo is true
	{
		//cout << "here11" << endl;
		//create textual sprite info:
		string * spriteTextString = new string();
		//char * spriteTextString = new char[100000];

		int spriteNumberOfLines;
		spriteNumberOfLines = SPRITE_TEXTUAL_NUM_OF_LINES;
		int * spriteColourArray = new int[spriteNumberOfLines];
		LRRCgenerateTextualSpriteInfoString(unitReference, spriteTextString, spriteColourArray);

		#ifdef DEBUG_ADD_INDIVIDUAL_SPRITES
		bool addIndividualSprites = true;
		#else
		bool addIndividualSprites = false;
		#endif
		spriteSubmodelCurrentReference = LDaddTextualSpriteInfoStringToReferenceList(unitReference, spriteTextString, spriteColourArray, spriteSubmodelCurrentReference, spriteNumberOfLines, numSpritesAddedLocal, addIndividualSprites);

		delete spriteTextString;
		delete spriteColourArray;
	}

	//cout << "here1b" << endl;

	if((addRangeSpriteInfo) && ((obtainReferencePlayerID(unitReference) == currentPlayerTurn) || (currentPhase == GAME_PHASE_GENERIC)))
	{
		//cout << "here1b1" << endl;

		//cout << "obtainReferencePlayerID(unitReference) = " << obtainReferencePlayerID(unitReference) << endl;
		//cout << "currentPlayerTurn = " << currentPlayerTurn << endl;
		//cout << "here12" << endl;
		spriteSubmodelCurrentReference = LRRCaddRangeSpriteInfoToReferenceList(spriteSubmodelCurrentReference, unitReference, currentPhase, numSpritesAddedLocal);
	}

	//cout << "here1c" << endl;

	if(addTargetSpriteInfo)
	{
		//cout << "here13" << endl;
		spriteSubmodelCurrentReference = LRRCaddTargetSpriteInfoToReferenceList(spriteSubmodelCurrentReference, unitReference, targetReference, currentPhase, numSpritesAddedLocal);
	}





#ifdef DEBUG_ADD_INDIVIDUAL_SPRITES

	//cout << "here2" << endl;

	//generate sprite reference name
	char * spriteReferenceFileName = new char[DAT_FILE_REF_SUBMODEL_NAME_LENGTH_MAX];



	//cout << "here2a" << endl;
	LDcreateSpriteReferenceName(spriteReferenceFileName, *numSpritesAdded, sceneFileName);
	#ifdef DEBUG_SPRITES
	cout << "spriteReferenceFileName = " << spriteReferenceFileName << endl;
	#endif

	//cout << "DEBUG unitBeingSprited = " << unitReference->name << endl;

	//cout << "here3" << endl;

	#ifdef CPLUSPLUSERRORCORRECTION3
		char * CHICKENDEBUG = new char[DAT_FILE_REF_MAX_SIZE*DAT_FILE_MAX_NUM_OF_REFERENCES];
	#endif


	//writeReferencesToFile
	if(!writeReferencesToFile(spriteReferenceFileName, spriteSubmodelInitialReference))
	{
		result = false;
	}

	delete spriteSubmodelInitialReference;

	int spriteDefaultColour = SPRITE_DEFAULT_COLOUR; //unitReference->colour doesnt work,, //CHECK THIS - this should be the player ID of the player who owns the the unit, unitDetails

	//cout << "here4" << endl;

	//addSpriteReferenceToSpriteListByteArray
	if(!LDaddSpriteToSpriteReferenceList(&(unitReference->absolutePosition), eyeCoords, spriteListInitialReference, spriteReferenceFileName, spriteDefaultColour, 1))
	{
		result = false;
	}


	//cout << "here5" << endl;

	delete spriteReferenceFileName;
	//cout << "DEBUG 1m" << endl;
#endif




	return result;
}


//currently this function just adds a plain line between the unit and the target, in the future it could be more complex
Reference * LRRCaddTargetSpriteInfoToReferenceList(Reference * spriteSubmodelInitialReference, Reference * unitReference, Reference * targetReference, int currentPhase, int * numSpritesAdded)
{
	bool result = true;
	Reference * spriteSubmodelCurrentReference = spriteSubmodelInitialReference;

	//add movement/longdistance/closecombat target line [red/orange/green line]
	spriteSubmodelCurrentReference->type = REFERENCE_TYPE_LINE;
	if(currentPhase == GAME_PHASE_MOVEMENT)
	{
		spriteSubmodelCurrentReference->colour = SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_OPAQ;
	}
	else if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
	{
		spriteSubmodelCurrentReference->colour = SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_OPAQ;
	}
	else if(currentPhase == GAME_PHASE_CLOSECOMBAT)
	{
		spriteSubmodelCurrentReference->colour = SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_OPAQ;
	}
	else if(currentPhase == GAME_PHASE_AIONLY_TARGET_SELECTION)
	{
		spriteSubmodelCurrentReference->colour = SPRITE_SUBMODEL_RANGE_TARGET_SELECTION_COLOUR_OPAQ;
	}
	else
	{
		result = false;
		cout << "error: LRRCaddTargetSpriteInfoToReferenceList() illegal phase " << endl;
	}

	#ifndef DEBUG_ADD_INDIVIDUAL_SPRITES
	copyVectors(&(spriteSubmodelCurrentReference->vertex1relativePosition), &(unitReference->absolutePosition));
	copyVectors(&(spriteSubmodelCurrentReference->vertex2relativePosition), &(targetReference->absolutePosition));
	#else
	vec localvec;
	subtractVectors(&localvec, &(unitReference->absolutePosition), &(unitReference->absolutePosition));
	copyVectors(&(spriteSubmodelCurrentReference->vertex1relativePosition), &localvec);
	subtractVectors(&localvec, &(targetReference->absolutePosition), &(unitReference->absolutePosition));
	copyVectors(&(spriteSubmodelCurrentReference->vertex2relativePosition), &localvec);
	#endif

	Reference * newReference1 = new Reference();
	spriteSubmodelCurrentReference -> next = newReference1;
	spriteSubmodelCurrentReference = spriteSubmodelCurrentReference -> next;

	*numSpritesAdded = *numSpritesAdded + 1;

	return spriteSubmodelCurrentReference;
}

Reference * LRRCaddRangeSpriteInfoToReferenceList(Reference * spriteSubmodelInitialReference, Reference * unitReference, int currentPhase, int * numSpritesAdded)
{
	Reference * spriteSubmodelCurrentReference = spriteSubmodelInitialReference;

	mat currentDeformationMatrix;

	ModelDetails * unitDetails = unitReference->subModelDetails;

	//cout << "currentPhase = " << GAME_PHASE_GENERIC << endl;

	if((currentPhase == GAME_PHASE_MOVEMENT) || (currentPhase == GAME_PHASE_GENERIC))
	{
		//add movement range info [green ring/sphere/cylinder]
		//cout << "(unitDetails->movementSpeed)" << (unitDetails->movementSpeed) << endl;
		//cout << "vv1" << endl;
		createIdentityMatrix(&currentDeformationMatrix);
		scaleMatrix(&currentDeformationMatrix, ((unitDetails->movementSpeed)*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM)));
		//cout << "vv1a" << endl;
		//cout << "spriteSubmodelCurrentReference->deformationMatrix->a.x = " << (&((&(spriteSubmodelCurrentReference->deformationMatrix))->a))->x << endl;

		copyMatrix2IntoMatrix1(&(spriteSubmodelCurrentReference->deformationMatrix), &(currentDeformationMatrix));
		//cout << "vv1b" << endl;
		spriteSubmodelCurrentReference->type = REFERENCE_TYPE_SUBMODEL;
		spriteSubmodelCurrentReference->colour = SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_OPAQ;
		spriteSubmodelCurrentReference->name = SPRITE_SUBMODEL_RANGE_FILE_NAME;
		#ifndef DEBUG_ADD_INDIVIDUAL_SPRITES
		addVectors(&(spriteSubmodelCurrentReference->relativePosition), &(spriteSubmodelCurrentReference->relativePosition), &(unitReference->absolutePosition));
		#endif
		//cout << "vv1c" << endl;
		Reference * newReference1 = new Reference();
		spriteSubmodelCurrentReference -> next = newReference1;
		//cout << "vv1d" << endl;
		spriteSubmodelCurrentReference = spriteSubmodelCurrentReference -> next;

		*numSpritesAdded = *numSpritesAdded + 1;

	}
	if((currentPhase == GAME_PHASE_LONGDISTANCECOMBAT) || (currentPhase == GAME_PHASE_GENERIC))
	{
		//add long distance combat range info [orange ring/sphere/cylinder]
		//cout << "vv2" << endl;
		createIdentityMatrix(&currentDeformationMatrix);
		scaleMatrix(&currentDeformationMatrix, ((unitDetails->longDistanceAttackBaseRange)*SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM));
		copyMatrix2IntoMatrix1(&(spriteSubmodelCurrentReference->deformationMatrix), &(currentDeformationMatrix));
		spriteSubmodelCurrentReference->type = REFERENCE_TYPE_SUBMODEL;
		spriteSubmodelCurrentReference->colour = SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_OPAQ;
		spriteSubmodelCurrentReference->name = SPRITE_SUBMODEL_RANGE_FILE_NAME;
		#ifndef DEBUG_ADD_INDIVIDUAL_SPRITES
		addVectors(&(spriteSubmodelCurrentReference->relativePosition), &(spriteSubmodelCurrentReference->relativePosition), &(unitReference->absolutePosition));
		#endif
		Reference * newReference2 = new Reference();
		spriteSubmodelCurrentReference -> next = newReference2;
		spriteSubmodelCurrentReference = spriteSubmodelCurrentReference -> next;

		*numSpritesAdded = *numSpritesAdded + 1;
	}
	if((currentPhase == GAME_PHASE_CLOSECOMBAT) || (currentPhase == GAME_PHASE_GENERIC))
	{
		//add close combat range info [red ring/sphere/cylinder - only a few units in diameter - ie quite small]
		//cout << "vv3" << endl;
		createIdentityMatrix(&currentDeformationMatrix);
		scaleMatrix(&currentDeformationMatrix, (CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM)));
		copyMatrix2IntoMatrix1(&(spriteSubmodelCurrentReference->deformationMatrix), &(currentDeformationMatrix));
		spriteSubmodelCurrentReference->type = REFERENCE_TYPE_SUBMODEL;
		spriteSubmodelCurrentReference->colour = SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_OPAQ;
		spriteSubmodelCurrentReference->name = SPRITE_SUBMODEL_RANGE_FILE_NAME;
		#ifndef DEBUG_ADD_INDIVIDUAL_SPRITES
		addVectors(&(spriteSubmodelCurrentReference->relativePosition), &(spriteSubmodelCurrentReference->relativePosition), &(unitReference->absolutePosition));
		#endif
		Reference * newReference3 = new Reference();
		spriteSubmodelCurrentReference -> next = newReference3;
		spriteSubmodelCurrentReference = spriteSubmodelCurrentReference -> next;

		*numSpritesAdded = *numSpritesAdded + 1;
	}

	return spriteSubmodelCurrentReference;
}







/*UP TO HERE*/


/*primary sprite routines*/


void LRRCgenerateTextualSpriteInfoString(Reference * unitReferenceInSceneFile, string * spriteTextString, int spriteColourArray[])
{
	ModelDetails * unitDetailsInSceneFile = unitReferenceInSceneFile->subModelDetails;

	/*Start Sprite Text Creation*/
#ifdef SPRITE_TEXTUAL_INCLUDE_ALL_COMBAT_INFO
	spriteColourArray[0] = DAT_FILE_COLOUR_WHITE;
	spriteColourArray[1] = DAT_FILE_COLOUR_WHITE;
	spriteColourArray[2] = DAT_FILE_COLOUR_BLUE;
	spriteColourArray[3] = DAT_FILE_COLOUR_YELLOW;
	spriteColourArray[4] = DAT_FILE_COLOUR_RED;
	spriteColourArray[5] = DAT_FILE_COLOUR_MAGENTA;
	spriteColourArray[6] = DAT_FILE_COLOUR_MAGENTA;
	//spriteColourArray[4] = DAT_FILE_COLOUR_CYAN;
	//spriteColourArray[5] = DAT_FILE_COLOUR_MAGENTA;
	//spriteColourArray[6] = DAT_FILE_COLOUR_BLACK;
	//spriteColourArray[5] = DAT_FILE_COLOUR_DARKGREY;
#else
	spriteColourArray[0] = DAT_FILE_COLOUR_WHITE;
	spriteColourArray[1] = DAT_FILE_COLOUR_WHITE;
	spriteColourArray[2] = DAT_FILE_COLOUR_BLUE;
	spriteColourArray[3] = DAT_FILE_COLOUR_RED;
	spriteColourArray[4] = DAT_FILE_COLOUR_MAGENTA;
	spriteColourArray[5] = DAT_FILE_COLOUR_MAGENTA;
	spriteColourArray[6] = DAT_FILE_COLOUR_MAGENTA;
#endif

	char * tempString = new char[DAT_FILE_DATA_VALUE_MAX_LENGTH*CPLUSPLUSERRORCORRECTION1];		//for some reason tempString must be very large or else the heap will get corrupted
	//char * tempString = new char[DAT_FILE_DATA_VALUE_MAX_LENGTH];

	//ii)  perform sprite calculations
	//cout << "DEBUG 1g" << endl;
	int baseDefenceLevel = invertLevel(unitDetailsInSceneFile->armourDefenceValue);
	int baseCloseCombatAttackLevel = invertLevel(unitDetailsInSceneFile->closeCombatAttackValue);
	int baseLongDistanceAttackLevel = invertLevel(unitDetailsInSceneFile->longDistanceAttackValue);

	//cout << "DEBUG: movementSpeed = " << movementSpeed << endl;

	//ii)
	//cout << "DEBUG 1h" << endl;

	//cout << "DEBUG baseDefenceLevel = " << baseDefenceLevel << endl;
	//cout << "DEBUG unitDetailsInSceneFile->defenceBonus = " << unitDetailsInSceneFile->defenceBonus << endl;
	//cout << "DEBUG unitDetailsInSceneFile->defenceTotal = " << unitDetailsInSceneFile->defenceTotal << endl;


	/*Unit Name Information*/

#ifdef SPRITE_TEXTUAL_INCLUDE_NAME_INFO
	int iTemp = 0;
	char cTemp =  unitReferenceInSceneFile->name[iTemp];
	bool fullstopFound = false;
	while((cTemp != '\0') && (!fullstopFound))
	{
		cTemp = unitReferenceInSceneFile->name[iTemp];
		if(cTemp == '.')
		{
			fullstopFound = true;
			tempString[iTemp] = '\0';
		}
		else
		{
			tempString[iTemp] = cTemp;
		}
		iTemp++;
	}
	*spriteTextString = *spriteTextString + tempString;
	//cout << "*spriteTextString=" << *spriteTextString << endl;
#endif

	/*Player ID Information*/
*spriteTextString = *spriteTextString + '\n';
#ifdef SPRITE_TEXTUAL_INCLUDE_PLAYERID_INFO
	tempString[0] = '\0';

	//itoa(obtainReferencePlayerID(unitReferenceInSceneFile), tempString, 10);
	sprintf(tempString, "%d", obtainReferencePlayerID(unitReferenceInSceneFile));

	*spriteTextString = *spriteTextString + "ID = " + tempString;
	//cout << "*spriteTextString=" << *spriteTextString << endl;
#endif


	/*Defence Related Sprite Information*/
*spriteTextString = *spriteTextString + '\n';
#ifdef SPRITE_TEXTUAL_INCLUDE_DESCRIPTION_TEXT
	#ifdef SPRITE_TEXTUAL_INCLUDE_ALL_COMBAT_INFO
		*spriteTextString = *spriteTextString + "     D = ";
	#else
		*spriteTextString = *spriteTextString + "D = ";
	#endif
#endif
	if(unitDetailsInSceneFile->defenceTotal > 0)
	{
	#ifdef SPRITE_ALWAYS_ADD_TEXT
		tempString[0] = '\0';
		//itoa(baseDefenceLevel, tempString, 10);
		*spriteTextString = *spriteTextString + tempString;
		if(unitDetailsInSceneFile->defenceBonus > 0)
		{
			tempString[0] = '\0';
			//itoa(unitDetailsInSceneFile->defenceBonus, tempString, 10);
			sprintf(tempString, "%d", unitDetailsInSceneFile->defenceBonus);
			*spriteTextString = *spriteTextString + " - " + tempString;
		}
	#endif
	#ifdef SPRITES_DISPLAY_DICE
		if((unitDetailsInSceneFile->defenceTotal >= MIN_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED) && (unitDetailsInSceneFile->defenceTotal <= MAX_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED))
		{
			tempString[0] = (unitDetailsInSceneFile->defenceTotal + SPRITE_CHARACTER_DICE_OFFSET);
			tempString[1] = '\0';
			*spriteTextString = *spriteTextString + tempString;
		}
		else
		{
	#ifndef SPRITE_ALWAYS_ADD_TEXT
			tempString[0] = '\0';
			//itoa(((unitDetailsInSceneFile->defenceTotal)*2), tempString, 10);
			sprintf(tempString, "%d", (unitDetailsInSceneFile->defenceTotal)*2);
			*spriteTextString = *spriteTextString + tempString;
	#endif
		}
	#endif
	}
	else
	{
		tempString[0] = '-';
		tempString[1] = '\0';
		*spriteTextString = *spriteTextString + tempString;
	}
	//cout << "*spriteTextString=" << *spriteTextString << endl;

	//cout << "DEBUG 1i" << endl;

#ifdef SPRITE_TEXTUAL_INCLUDE_ALL_COMBAT_INFO

		/*Long Distance Attack Related Sprite Information*/
	*spriteTextString = *spriteTextString + '\n';
	#ifdef SPRITE_TEXTUAL_INCLUDE_DESCRIPTION_TEXT
		*spriteTextString = *spriteTextString + "ALD = ";
	#endif
		//cout << "DEBUG 1i0" << endl;
		if(unitDetailsInSceneFile->longDistanceAttackTotal > 0)
		{
			//cout << "DEBUG 1i1" << endl;
		#ifdef SPRITE_ALWAYS_ADD_TEXT
			tempString[0] = '\0';
			//itoa(baseLongDistanceAttackLevel, tempString, 10);
			sprintf(tempString, "%d", baseLongDistanceAttackLevel);
			*spriteTextString = *spriteTextString + tempString;
			if(unitDetailsInSceneFile->longDistanceAttackBonus > 0)
			{
				tempString[0] = '\0';
				//itoa(unitDetailsInSceneFile->longDistanceAttackBonus, tempString, 10);
				sprintf(tempString, "%d", unitDetailsInSceneFile->longDistanceAttackBonus);
				*spriteTextString = *spriteTextString + " - " + tempString;
			}
		#endif
		#ifdef SPRITES_DISPLAY_DICE
			if((unitDetailsInSceneFile->longDistanceAttackTotal >= MIN_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED) && (unitDetailsInSceneFile->longDistanceAttackTotal <= MAX_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED))
			{
				//cout << "DEBUG 1i2" << endl;
				tempString[0] = (unitDetailsInSceneFile->longDistanceAttackTotal + SPRITE_CHARACTER_DICE_OFFSET);
				tempString[1] = '\0';
				//cout << "DEBUG 1i3" << endl;
				*spriteTextString = *spriteTextString + tempString;
			}
			else
			{
			#ifndef SPRITE_ALWAYS_ADD_TEXT
					tempString[0] = '\0';
					//itoa(((unitDetailsInSceneFile->longDistanceAttackTotal)*2), tempString, 10);
					sprintf(tempString, "%d", (unitDetailsInSceneFile->longDistanceAttackTotal)*2);
					*spriteTextString = *spriteTextString + tempString;
			#endif
			}
		#endif
		}
		else
		{
			//cout << "DEBUG 1i6" << endl;
			tempString[0] = '-';
			tempString[1] = '\0';
			*spriteTextString = *spriteTextString + tempString;
		}
		//cout << "*spriteTextString=" << *spriteTextString << endl;

		//cout << "DEBUG 1j" << endl;

		/*Close Combat Attack Related Sprite Information*/
	*spriteTextString = *spriteTextString + '\n';
	#ifdef SPRITE_TEXTUAL_INCLUDE_DESCRIPTION_TEXT
		*spriteTextString = *spriteTextString + "ACC = ";
	#endif
		if(unitDetailsInSceneFile->closeCombatAttackTotal > 0)
		{
		#ifdef SPRITE_ALWAYS_ADD_TEXT
			tempString[0] = '\0';
			//itoa(baseCloseCombatAttackLevel, tempString, 10);
			sprintf(tempString, "%d", baseCloseCombatAttackLevel);
			*spriteTextString = *spriteTextString + tempString;
			if(unitDetailsInSceneFile->closeCombatAttackBonus > 0)
			{
				tempString[0] = '\0';
				//itoa(unitDetailsInSceneFile->closeCombatAttackBonus, tempString, 10);
				sprintf(tempString, "%d", unitDetailsInSceneFile->closeCombatAttackBonus);
				*spriteTextString = *spriteTextString + " + " + tempString;
			}
		#endif
		#ifdef SPRITES_DISPLAY_DICE
			if((unitDetailsInSceneFile->closeCombatAttackTotal >= MIN_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED) && (unitDetailsInSceneFile->closeCombatAttackTotal <= MAX_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED))
			{
				tempString[0] = (unitDetailsInSceneFile->closeCombatAttackTotal + SPRITE_CHARACTER_DICE_OFFSET);
				tempString[1] = '\0';
				*spriteTextString = *spriteTextString + tempString;
			}
			else
			{
		#ifndef SPRITE_ALWAYS_ADD_TEXT
			tempString[0] = '\0';
			//itoa(((unitDetailsInSceneFile->closeCombatAttackTotal)*2), tempString, 10);
			sprintf(tempString, "%d", (unitDetailsInSceneFile->closeCombatAttackTotal)*2);
			*spriteTextString = *spriteTextString + tempString;
		#endif
			}
		#endif
		}
		else
		{
			tempString[0] = '-';
			tempString[1] = '\0';
			*spriteTextString = *spriteTextString + tempString;
		}
		//cout << "*spriteTextString=" << *spriteTextString << endl;

#else

		/*Generic Attack Related Sprite Information*/
	//tempString[0] = '\n';
	//tempString[1] = '\0';
	//*spriteTextString = *spriteTextString + tempString;
	*spriteTextString = *spriteTextString + '\n';
	//*spriteTextString = *spriteTextString + '\n';		//this triggers error bad
	//*spriteTextString = *spriteTextString + '\n';		//this triggers error very bad

	#ifdef SPRITE_TEXTUAL_INCLUDE_DESCRIPTION_TEXT
		*spriteTextString = *spriteTextString + "A = ";
	#endif
		if((unitDetailsInSceneFile->closeCombatAttackTotal > 0) || (unitDetailsInSceneFile->longDistanceAttackTotal > 0))
		{
			if((unitDetailsInSceneFile->closeCombatAttackValue) > (unitDetailsInSceneFile->longDistanceAttackValue))
			{
			#ifdef SPRITE_ALWAYS_ADD_TEXT
				tempString[0] = '\0';
				//itoa(baseCloseCombatAttackLevel, tempString, 10);
				sprintf(tempString, "%d", baseCloseCombatAttackLevel);
				*spriteTextString = *spriteTextString + tempString;
				if(unitDetailsInSceneFile->closeCombatAttackBonus > 0)
				{
					tempString[0] = '\0';
					//itoa(unitDetailsInSceneFile->closeCombatAttackBonus, tempString, 10);
					sprintf(tempString, "%d", unitDetailsInSceneFile->closeCombatAttackBonus);
					*spriteTextString = *spriteTextString + " + " + tempString;
				}
			#endif
			#ifdef SPRITES_DISPLAY_DICE
				if((unitDetailsInSceneFile->closeCombatAttackTotal >= MIN_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED) && (unitDetailsInSceneFile->closeCombatAttackTotal <= MAX_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED))
				{
					tempString[0] = (unitDetailsInSceneFile->closeCombatAttackTotal + SPRITE_CHARACTER_DICE_OFFSET);
					tempString[1] = '\0';
					*spriteTextString = *spriteTextString + tempString;
				}
				else
				{
				#ifndef SPRITE_ALWAYS_ADD_TEXT
					tempString[0] = '\0';
					//itoa(((unitDetailsInSceneFile->closeCombatAttackTotal)*2), tempString, 10);
					sprintf(tempString, "%d", (unitDetailsInSceneFile->closeCombatAttackTotal)*2);
					*spriteTextString = *spriteTextString + tempString;
				#endif
				}
			#endif
			}
			else
			{
			#ifdef SPRITE_ALWAYS_ADD_TEXT
				tempString[0] = '\0';
				//itoa(baseLongDistanceAttackLevel, tempString, 10);
				sprintf(tempString, "%d", baseLongDistanceAttackLevel);
				*spriteTextString = *spriteTextString + tempString;
				if(unitDetailsInSceneFile->longDistanceAttackBonus > 0)
				{
					tempString[0] = '\0';
					//itoa(unitDetailsInSceneFile->longDistanceAttackBonus, tempString, 10);
					sprintf(tempString, "%d", unitDetailsInSceneFile->longDistanceAttackBonus);
					*spriteTextString = *spriteTextString + " - " + tempString;
				}
			#endif
			#ifdef SPRITES_DISPLAY_DICE
				if((unitDetailsInSceneFile->longDistanceAttackTotal >= MIN_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED) && (unitDetailsInSceneFile->longDistanceAttackTotal <= MAX_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED))
				{
					//cout << "DEBUG 1i2" << endl;
					tempString[0] = (unitDetailsInSceneFile->longDistanceAttackTotal + SPRITE_CHARACTER_DICE_OFFSET);
					tempString[1] = '\0';
					//cout << "DEBUG 1i3" << endl;
					*spriteTextString = *spriteTextString + tempString;
				}
				else
				{
				#ifndef SPRITE_ALWAYS_ADD_TEXT
					tempString[0] = '\0';
					//itoa(((unitDetailsInSceneFile->longDistanceAttackTotal)*2), tempString, 10);
					sprintf(tempString, "%d", (unitDetailsInSceneFile->longDistanceAttackTotal)*2);
					*spriteTextString = *spriteTextString + tempString;
				#endif
				}
			#endif
			}
		}
		else
		{
			tempString[0] = '-';
			tempString[1] = '\0';
			*spriteTextString = *spriteTextString + tempString;
		}
		//cout << "*spriteTextString=" << *spriteTextString << endl;

#endif

	//cout << "DEBUG 1k" << endl;

#ifdef SPRITE_TEXTUAL_INCLUDE_MOVEMENT
	/*Movement Related Sprite Information*/
	*spriteTextString = *spriteTextString + '\n';
	//*spriteTextString = *spriteTextString + '\n';
#ifdef SPRITE_TEXTUAL_INCLUDE_DESCRIPTION_TEXT
	*spriteTextString = *spriteTextString + "     M = ";
#endif
	if(unitDetailsInSceneFile->movementSpeed > 0)
	{
		tempString[0] = '\0';
		//itoa(unitDetailsInSceneFile->movementSpeed, tempString, 10);
		sprintf(tempString, "%d", unitDetailsInSceneFile->movementSpeed);
		*spriteTextString = *spriteTextString + tempString;

		if(unitDetailsInSceneFile->longDistanceAttackBonus > 0)
		{
			tempString[0] = '\0';
			//itoa(unitDetailsInSceneFile->longDistanceAttackBonus, tempString, 10);
			sprintf(tempString, "%d", unitDetailsInSceneFile->longDistanceAttackBonus);
			*spriteTextString = *spriteTextString + " + " + tempString;
		}
	}
	else
	{
		tempString[0] = '-';
		tempString[1] = '\0';
		*spriteTextString = *spriteTextString + tempString;
	}
#endif

	//cout << "DEBUG 1l" << endl;

	delete tempString;



	//cout << "DEBUG: *spriteTextString = \n" << *spriteTextString << "\n\n" << endl;
	/*End Start Sprite Text Creation*/
}









