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
 * File Name: LRRCsprite.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3n7c 17-August-2020
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/
/*This code allows the addition of a sprite into a given scene file where a sprite is a paragraph of text. [The text is to be rendered in 3D, and point towards the user POV - see CHECK THIS for missing code]*/


#include "LRRCsprite.hpp"




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


void LRRCspriteClass::fillInLRRCSpriteExternVariables()
{
	XMLrulesClass* currentReferenceRulesClass = LRRCrulesSprite;

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

	LDsprite.fillInLDspriteExternVariables();
}
/*top level sprite routines - required for independent LRRCsprite.cpp calculations*/

	//NB this function does not add round's player's phase specific [movement/attack] sprites - these have to be added in LRRCmovement.cpp, and LRRCgame.cpp functions respectively
bool LRRCspriteClass::LRRCaddUnitDetailsSpritesToSceneFile(string sceneFileName, string sceneFileNameWithSprites, const bool addTextualSpriteInfo, const bool addRangeSpriteInfo, const int currentPhase, const int currentPlayerTurn)
{
	bool result = true;

	//declare initial scene references
	LDreference* initialReferenceInSceneFile = new LDreference();
	LDreference* topLevelReferenceInSceneFile = new LDreference(true);		//The model details information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function

	//parse the scene files and build LDreference linked lists
	if(!LDparser.parseFile(sceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, false))
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

	LDreference* spriteListInitialReference = new LDreference();
	this->LRRCsearchSceneRefListAddUnitDetailsSpriteForSubmodels(initialReferenceInSceneFile, spriteListInitialReference, &eyeCoords, &numSpritesAdded, sceneFileNameWithSprites, initialReferenceInSceneFile, addTextualSpriteInfo, addRangeSpriteInfo, currentPhase, currentPlayerTurn);

	if(!LDreferenceManipulation.addSpriteReferenceListToSceneFile(sceneFileName, sceneFileNameWithSprites, spriteListInitialReference, numSpritesAdded))
	{
		result = false;
	}

	delete initialReferenceInSceneFile;
	delete topLevelReferenceInSceneFile;
	delete spriteListInitialReference;

	return result;
}

bool LRRCspriteClass::LRRCaddUnitDetailsSpritesToScene(const string sceneFileName, string sceneFileNameWithSprites, const bool addTextualSpriteInfo, const bool addRangeSpriteInfo, const int currentPhase, const int currentPlayerTurn, LDreference* firstReferenceInScene)
{
	bool result = true;

	//declare initial scene references
	LDreference* initialReferenceInSceneFile = firstReferenceInScene;

	int numSpritesAdded = 0;
	vec eyeCoords;
	eyeCoords.x = 0.0;
	eyeCoords.y = 0.0;
	eyeCoords.z = 0.0;

	LDreference* spriteListInitialReference = new LDreference();
	this->LRRCsearchSceneRefListAddUnitDetailsSpriteForSubmodels(initialReferenceInSceneFile, spriteListInitialReference, &eyeCoords, &numSpritesAdded, sceneFileNameWithSprites, initialReferenceInSceneFile, addTextualSpriteInfo, addRangeSpriteInfo, currentPhase, currentPlayerTurn);

	if(!LDreferenceManipulation.addSpriteReferenceListToSceneFile(sceneFileName, sceneFileNameWithSprites, spriteListInitialReference, numSpritesAdded))
	{
		result = false;
	}

	delete spriteListInitialReference;

	return result;
}




//preconditions; assumes scene file has more than 1 person
void LRRCspriteClass::LRRCsearchSceneRefListAddUnitDetailsSpriteForSubmodels(LDreference* referenceInSceneFile, LDreference* spriteListInitialReference, const vec* eyeCoords, int* numSpritesAdded, string sceneFileName, const LDreference* initialReferenceInSceneFile, const bool addTextualSpriteInfo, const bool addRangeSpriteInfo, const int currentPhase, const int currentPlayerTurn)
{
	//cout << "here03" << endl;

	LDreference* unusedTargetReference;
	bool addTargetSpriteInfo = false;

	bool result = true;

	LDreference* currentReference = referenceInSceneFile;

	while(currentReference->next != NULL)
	{
		if(currentReference->isSubModelReference)
		{
			LRRCcombat.performFinalUnitClassCalculations(currentReference->subModelDetails);

			/*a)  parse down the tree from the unit. NB this does not allow for copying of parent combat relevant items into child unit
				extract from performCloseCombatNormal*/

			//cout << "here031" << endl;

			//declare current submodel top level reference
			LDreference* topLevelReferenceInCurrentSubmodel = new LDreference(currentReference->name, currentReference->colour, true);
			//cout << "here0311" << endl;
				LDreferenceClass.copyReferencePosition(topLevelReferenceInCurrentSubmodel, currentReference);
				LRRCmodelClass.copyAllUnitDetails(topLevelReferenceInCurrentSubmodel->subModelDetails, currentReference->subModelDetails);
				//cout << "here0312" << endl;
			LRRCcombat.searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(topLevelReferenceInCurrentSubmodel, currentReference->firstReferenceWithinSubModel, topLevelReferenceInCurrentSubmodel, true);
				//cout << "here0313" << endl;
			LRRCmodelClass.determineUnitTypeAndMinSpeedOfUnitGroup(topLevelReferenceInCurrentSubmodel->subModelDetails);
				//cout << "here0314" << endl;
			LRRCcombat.performFinalUnitClassCalculations(topLevelReferenceInCurrentSubmodel->subModelDetails);


			//cout << "topLevelReferenceInCurrentSubmodel->colour = " << topLevelReferenceInCurrentSubmodel->colour << endl;
			//cout << "here032" << endl;

			/*recursive parsing enabled*/
			if(topLevelReferenceInCurrentSubmodel->subModelDetails->numPerson > 1)
			{
				//cout << "DEBUG 1c" << endl;
				this->LRRCsearchSceneRefListAddUnitDetailsSpriteForSubmodels(currentReference->firstReferenceWithinSubModel, spriteListInitialReference, eyeCoords, numSpritesAdded, sceneFileName, initialReferenceInSceneFile, addTextualSpriteInfo, addRangeSpriteInfo, currentPhase, currentPlayerTurn);
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
				if(!this->LRRCdetermineSpriteInfoAndAddSpriteToSpriteRefList(topLevelReferenceInCurrentSubmodel, unusedTargetReference, spriteListInitialReference, eyeCoords, numSpritesAdded, sceneFileName, addTextualSpriteInfo, addRangeSpriteInfo, addTargetSpriteInfo, currentPhase, currentPlayerTurn))
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

bool LRRCspriteClass::LRRCdetermineSpriteInfoAndAddSpriteToSpriteRefList(LDreference* unitReference, LDreference* targetReference, LDreference* spriteListInitialReference, const vec* eyeCoords, int* numSpritesAdded, string sceneFileName, const bool addTextualSpriteInfo, const bool addRangeSpriteInfo, const bool addTargetSpriteInfo, const int currentPhase, const int currentPlayerTurn)
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

	int* numSpritesAddedLocal;
	LDreference* spriteSubmodelCurrentReference;
#ifdef DEBUG_ADD_INDIVIDUAL_SPRITES
	//add sprite info [textual + range info] to reference list:
	LDreference* spriteSubmodelInitialReference = new LDreference();
	spriteSubmodelCurrentReference = spriteSubmodelInitialReference;
	int numSpritesAddedLocalUnused = 0;
	numSpritesAddedLocal = &numSpritesAddedLocalUnused;
#else
	numSpritesAddedLocal = numSpritesAdded;
	spriteSubmodelCurrentReference = LDreferenceManipulation.search1DrefListFindLastReference(spriteListInitialReference);
#endif

	//cout << "spriteSubmodelCurrentReference->deformationMatrix->a.x = " << (&((&(spriteSubmodelCurrentReference->deformationMatrix))->a))->x << endl;


	if(addTextualSpriteInfo)	//always if addTextualSpriteInfo is true
	{
		//cout << "here11" << endl;
		//create textual sprite info:
		string spriteTextString = "";

		int spriteNumberOfLines;
		spriteNumberOfLines = SPRITE_TEXTUAL_NUM_OF_LINES;
		int spriteColourArray[SPRITE_TEXTUAL_NUM_OF_LINES];
		this->LRRCgenerateTextualSpriteInfoString(unitReference, &spriteTextString, spriteColourArray);

		#ifdef DEBUG_ADD_INDIVIDUAL_SPRITES
		bool addIndividualSprites = true;
		#else
		bool addIndividualSprites = false;
		#endif
		spriteSubmodelCurrentReference = LDsprite.LDaddTextualSpriteInfoStringToReferenceList(unitReference, spriteTextString, spriteColourArray, spriteSubmodelCurrentReference, spriteNumberOfLines, numSpritesAddedLocal, addIndividualSprites);
	}

	//cout << "here1b" << endl;

	if((addRangeSpriteInfo) && ((LDreferenceClass.obtainReferencePlayerID(unitReference) == currentPlayerTurn) || (currentPhase == GAME_PHASE_GENERIC)))
	{
		//cout << "here1b1" << endl;

		//cout << "LDreferenceClass.obtainReferencePlayerID(unitReference) = " << LDreferenceClass.obtainReferencePlayerID(unitReference) << endl;
		//cout << "currentPlayerTurn = " << currentPlayerTurn << endl;
		//cout << "here12" << endl;
		spriteSubmodelCurrentReference = this->LRRCaddRangeSpriteInfoToReferenceList(spriteSubmodelCurrentReference, unitReference, currentPhase, numSpritesAddedLocal);
	}

	//cout << "here1c" << endl;

	if(addTargetSpriteInfo)
	{
		//cout << "here13" << endl;
		spriteSubmodelCurrentReference = this->LRRCaddTargetSpriteInfoToReferenceList(spriteSubmodelCurrentReference, unitReference, targetReference, currentPhase, numSpritesAddedLocal);
	}





#ifdef DEBUG_ADD_INDIVIDUAL_SPRITES

	//cout << "here2" << endl;

	//generate sprite reference name


	//cout << "here2a" << endl;
	string spriteReferenceFileName = LDsprite.LDcreateSpriteReferenceName(*numSpritesAdded, sceneFileName);
	#ifdef DEBUG_SPRITES
	cout << "spriteReferenceFileName = " << spriteReferenceFileName << endl;
	#endif

	//cout << "DEBUG unitBeingSprited = " << unitReference->name << endl;

	//cout << "here3" << endl;

	//writeReferencesToFile
	if(!LDreferenceManipulation.writeReferencesToFile(spriteReferenceFileName, spriteSubmodelInitialReference))
	{
		result = false;
	}

	delete spriteSubmodelInitialReference;

	int spriteDefaultColour = SPRITE_DEFAULT_COLOUR; //unitReference->colour doesnt work,, //CHECK THIS - this should be the player ID of the player who owns the the unit, unitDetails

	//cout << "here4" << endl;

	//addSpriteReferenceToSpriteListByteArray
	if(!LDsprite.LDaddSpriteToSpriteReferenceList(&(unitReference->absolutePosition), eyeCoords, spriteListInitialReference, spriteReferenceFileName, spriteDefaultColour, 1))
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
LDreference* LRRCspriteClass::LRRCaddTargetSpriteInfoToReferenceList(LDreference* spriteSubmodelInitialReference, LDreference* unitReference, LDreference* targetReference, const int currentPhase, int* numSpritesAdded)
{
	bool result = true;
	LDreference* spriteSubmodelCurrentReference = spriteSubmodelInitialReference;

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
		cout << "error: LRRCaddTargetSpriteInfoToReferenceList{} illegal phase " << endl;
	}

	#ifndef DEBUG_ADD_INDIVIDUAL_SPRITES
	SHAREDvector.copyVectors(&(spriteSubmodelCurrentReference->vertex1relativePosition), &(unitReference->absolutePosition));
	SHAREDvector.copyVectors(&(spriteSubmodelCurrentReference->vertex2relativePosition), &(targetReference->absolutePosition));
	#else
	vec localvec;
	SHAREDvector.subtractVectors(&localvec, &(unitReference->absolutePosition), &(unitReference->absolutePosition));
	SHAREDvector.copyVectors(&(spriteSubmodelCurrentReference->vertex1relativePosition), &localvec);
	SHAREDvector.subtractVectors(&localvec, &(targetReference->absolutePosition), &(unitReference->absolutePosition));
	SHAREDvector.copyVectors(&(spriteSubmodelCurrentReference->vertex2relativePosition), &localvec);
	#endif

	LDreference* newReference1 = new LDreference();
	spriteSubmodelCurrentReference -> next = newReference1;
	spriteSubmodelCurrentReference = spriteSubmodelCurrentReference -> next;

	*numSpritesAdded = *numSpritesAdded + 1;

	return spriteSubmodelCurrentReference;
}

LDreference* LRRCspriteClass::LRRCaddRangeSpriteInfoToReferenceList(LDreference* spriteSubmodelInitialReference, LDreference* unitReference, const int currentPhase, int* numSpritesAdded)
{
	LDreference* spriteSubmodelCurrentReference = spriteSubmodelInitialReference;

	mat currentDeformationMatrix;

	ModelDetails* unitDetails = unitReference->subModelDetails;

	//cout << "currentPhase = " << GAME_PHASE_GENERIC << endl;

	if((currentPhase == GAME_PHASE_MOVEMENT) || (currentPhase == GAME_PHASE_GENERIC))
	{
		//add movement range info [green ring/sphere/cylinder]
		//cout << "(unitDetails->movementSpeed)" << (unitDetails->movementSpeed) << endl;
		//cout << "vv1" << endl;
		SHAREDvector.createIdentityMatrix(&currentDeformationMatrix);
		SHAREDvector.scaleMatrix(&currentDeformationMatrix, ((unitDetails->movementSpeed)*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM)));
		//cout << "vv1a" << endl;
		//cout << "spriteSubmodelCurrentReference->deformationMatrix->a.x = " << (&((&(spriteSubmodelCurrentReference->deformationMatrix))->a))->x << endl;

		SHAREDvector.copyMatrixTwoIntoMatrixOne(&(spriteSubmodelCurrentReference->deformationMatrix), &(currentDeformationMatrix));
		//cout << "vv1b" << endl;
		spriteSubmodelCurrentReference->type = REFERENCE_TYPE_SUBMODEL;
		spriteSubmodelCurrentReference->colour = SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_OPAQ;
		spriteSubmodelCurrentReference->name = SPRITE_SUBMODEL_RANGE_FILE_NAME;
		#ifndef DEBUG_ADD_INDIVIDUAL_SPRITES
		SHAREDvector.addVectors(&(spriteSubmodelCurrentReference->relativePosition), &(spriteSubmodelCurrentReference->relativePosition), &(unitReference->absolutePosition));
		#endif
		//cout << "vv1c" << endl;
		LDreference* newReference1 = new LDreference();
		spriteSubmodelCurrentReference -> next = newReference1;
		//cout << "vv1d" << endl;
		spriteSubmodelCurrentReference = spriteSubmodelCurrentReference -> next;

		*numSpritesAdded = *numSpritesAdded + 1;

	}
	if((currentPhase == GAME_PHASE_LONGDISTANCECOMBAT) || (currentPhase == GAME_PHASE_GENERIC))
	{
		//add long distance combat range info [orange ring/sphere/cylinder]
		//cout << "vv2" << endl;
		SHAREDvector.createIdentityMatrix(&currentDeformationMatrix);
		SHAREDvector.scaleMatrix(&currentDeformationMatrix, ((unitDetails->longDistanceAttackBaseRange)*SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM));
		SHAREDvector.copyMatrixTwoIntoMatrixOne(&(spriteSubmodelCurrentReference->deformationMatrix), &(currentDeformationMatrix));
		spriteSubmodelCurrentReference->type = REFERENCE_TYPE_SUBMODEL;
		spriteSubmodelCurrentReference->colour = SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_OPAQ;
		spriteSubmodelCurrentReference->name = SPRITE_SUBMODEL_RANGE_FILE_NAME;
		#ifndef DEBUG_ADD_INDIVIDUAL_SPRITES
		SHAREDvector.addVectors(&(spriteSubmodelCurrentReference->relativePosition), &(spriteSubmodelCurrentReference->relativePosition), &(unitReference->absolutePosition));
		#endif
		LDreference* newReference2 = new LDreference();
		spriteSubmodelCurrentReference -> next = newReference2;
		spriteSubmodelCurrentReference = spriteSubmodelCurrentReference -> next;

		*numSpritesAdded = *numSpritesAdded + 1;
	}
	if((currentPhase == GAME_PHASE_CLOSECOMBAT) || (currentPhase == GAME_PHASE_GENERIC))
	{
		//add close combat range info [red ring/sphere/cylinder - only a few units in diameter - ie quite small]
		//cout << "vv3" << endl;
		SHAREDvector.createIdentityMatrix(&currentDeformationMatrix);
		SHAREDvector.scaleMatrix(&currentDeformationMatrix, (CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM)));
		SHAREDvector.copyMatrixTwoIntoMatrixOne(&(spriteSubmodelCurrentReference->deformationMatrix), &(currentDeformationMatrix));
		spriteSubmodelCurrentReference->type = REFERENCE_TYPE_SUBMODEL;
		spriteSubmodelCurrentReference->colour = SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_OPAQ;
		spriteSubmodelCurrentReference->name = SPRITE_SUBMODEL_RANGE_FILE_NAME;
		#ifndef DEBUG_ADD_INDIVIDUAL_SPRITES
		SHAREDvector.addVectors(&(spriteSubmodelCurrentReference->relativePosition), &(spriteSubmodelCurrentReference->relativePosition), &(unitReference->absolutePosition));
		#endif
		LDreference* newReference3 = new LDreference();
		spriteSubmodelCurrentReference -> next = newReference3;
		spriteSubmodelCurrentReference = spriteSubmodelCurrentReference -> next;

		*numSpritesAdded = *numSpritesAdded + 1;
	}

	return spriteSubmodelCurrentReference;
}







/*UP TO HERE*/


/*primary sprite routines*/


void LRRCspriteClass::LRRCgenerateTextualSpriteInfoString(LDreference* unitReferenceInSceneFile, string* spriteTextString, int spriteColourArray[])
{
	*spriteTextString = "";

	ModelDetails* unitDetailsInSceneFile = unitReferenceInSceneFile->subModelDetails;

	/*Start Sprite Text Creation*/
#ifdef SPRITE_TEXTUAL_INCLUDE_ALL_COMBAT_INFO
	spriteColourArray[0] = DAT_FILE_COLOUR_WHITE;
	spriteColourArray[1] = DAT_FILE_COLOUR_WHITE;
	spriteColourArray[2] = DAT_FILE_COLOUR_BLUE;
	spriteColourArray[3] = DAT_FILE_COLOUR_YELLOW;
	spriteColourArray[4] = DAT_FILE_COLOUR_RED;
	spriteColourArray[5] = DAT_FILE_COLOUR_MAGENTA;
	spriteColourArray[6] = DAT_FILE_COLOUR_MAGENTA;
#else
	spriteColourArray[0] = DAT_FILE_COLOUR_WHITE;
	spriteColourArray[1] = DAT_FILE_COLOUR_WHITE;
	spriteColourArray[2] = DAT_FILE_COLOUR_BLUE;
	spriteColourArray[3] = DAT_FILE_COLOUR_RED;
	spriteColourArray[4] = DAT_FILE_COLOUR_MAGENTA;
	spriteColourArray[5] = DAT_FILE_COLOUR_MAGENTA;
	spriteColourArray[6] = DAT_FILE_COLOUR_MAGENTA;
#endif

	//ii)  perform sprite calculations
	int baseDefenceLevel = LRRCmodelClass.invertLevel(unitDetailsInSceneFile->armourDefenceValue);
	int baseCloseCombatAttackLevel = LRRCmodelClass.invertLevel(unitDetailsInSceneFile->closeCombatAttackValue);
	int baseLongDistanceAttackLevel = LRRCmodelClass.invertLevel(unitDetailsInSceneFile->longDistanceAttackValue);

	//cout << "DEBUG: movementSpeed = " << movementSpeed << endl;

	//ii)
	//cout << "DEBUG 1h" << endl;

	//cout << "DEBUG baseDefenceLevel = " << baseDefenceLevel << endl;
	//cout << "DEBUG unitDetailsInSceneFile->defenceBonus = " << unitDetailsInSceneFile->defenceBonus << endl;
	//cout << "DEBUG unitDetailsInSceneFile->defenceTotal = " << unitDetailsInSceneFile->defenceTotal << endl;


	/*Unit Name Information*/

#ifdef SPRITE_TEXTUAL_INCLUDE_NAME_INFO
	bool fullstopFound = false;
	*spriteTextString = unitReferenceInSceneFile->name;
	int positionOfFullStop = (unitReferenceInSceneFile->name).find(CHAR_FULLSTOP);
	if(positionOfFullStop != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		*spriteTextString = (unitReferenceInSceneFile->name).substr(0, positionOfFullStop);
		fullstopFound = true;
	}
	//cout << "*spriteTextString=" <<* spriteTextString << endl;
#endif

	/*Player ID Information*/
	*spriteTextString = *spriteTextString + '\n';
#ifdef SPRITE_TEXTUAL_INCLUDE_PLAYERID_INFO
	string tempString = SHAREDvars.convertIntToString(LDreferenceClass.obtainReferencePlayerID(unitReferenceInSceneFile));

	*spriteTextString = *spriteTextString + "ID = " + tempString;
	//cout << "*spriteTextString=" <<* spriteTextString << endl;
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
		if(unitDetailsInSceneFile->defenceBonus > 0)
		{
			string tempString = SHAREDvars.convertIntToString(unitDetailsInSceneFile->defenceBonus);
			*spriteTextString = *spriteTextString + " - " + tempString;
		}
	#endif
	#ifdef SPRITES_DISPLAY_DICE
		if((unitDetailsInSceneFile->defenceTotal >= MIN_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED) && (unitDetailsInSceneFile->defenceTotal <= MAX_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED))
		{
			char charTemp = (unitDetailsInSceneFile->defenceTotal + SPRITE_CHARACTER_DICE_OFFSET);
			*spriteTextString = *spriteTextString + charTemp;
		}
		else
		{
	#ifndef SPRITE_ALWAYS_ADD_TEXT
			string tempString = SHAREDvars.convertIntToString((unitDetailsInSceneFile->defenceTotal)*2);
			*spriteTextString = *spriteTextString + tempString;
	#endif
		}
	#endif
	}
	else
	{
		*spriteTextString = *spriteTextString + CHAR_DASH;
	}
	//cout << "*spriteTextString=" <<* spriteTextString << endl;

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
			string tempString = SHAREDvars.convertIntToString(baseLongDistanceAttackLevel);
			*spriteTextString = *spriteTextString + tempString;
			if(unitDetailsInSceneFile->longDistanceAttackBonus > 0)
			{
				string tempString = SHAREDvars.convertIntToString(unitDetailsInSceneFile->longDistanceAttackBonus);
				*spriteTextString = *spriteTextString + " - " + tempString;
			}
		#endif
		#ifdef SPRITES_DISPLAY_DICE
			if((unitDetailsInSceneFile->longDistanceAttackTotal >= MIN_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED) && (unitDetailsInSceneFile->longDistanceAttackTotal <= MAX_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED))
			{
				//cout << "DEBUG 1i2" << endl;
				//cout << "DEBUG 1i3" << endl;
				*spriteTextString = *spriteTextString + (unitDetailsInSceneFile->longDistanceAttackTotal + SPRITE_CHARACTER_DICE_OFFSET);
			}
			else
			{
			#ifndef SPRITE_ALWAYS_ADD_TEXT
				string tempString = SHAREDvars.convertIntToString((unitDetailsInSceneFile->longDistanceAttackTotal)*2);
				*spriteTextString = *spriteTextString + tempString;
			#endif
			}
		#endif
		}
		else
		{
			//cout << "DEBUG 1i6" << endl;
			*spriteTextString = *spriteTextString + CHAR_DASH;
		}
		//cout << "*spriteTextString=" <<* spriteTextString << endl;

		//cout << "DEBUG 1j" << endl;

		/*Close Combat Attack Related Sprite Information*/
		*spriteTextString = *spriteTextString + '\n';
	#ifdef SPRITE_TEXTUAL_INCLUDE_DESCRIPTION_TEXT
		*spriteTextString = *spriteTextString + "ACC = ";
	#endif
		if(unitDetailsInSceneFile->closeCombatAttackTotal > 0)
		{
		#ifdef SPRITE_ALWAYS_ADD_TEXT
			string tempString = SHAREDvars.convertIntToString(baseCloseCombatAttackLevel);
			*spriteTextString = *spriteTextString + tempString;
			if(unitDetailsInSceneFile->closeCombatAttackBonus > 0)
			{
				string tempString = SHAREDvars.convertIntToString(unitDetailsInSceneFile->closeCombatAttackBonus);
				*spriteTextString = *spriteTextString + " + " + tempString;
			}
		#endif
		#ifdef SPRITES_DISPLAY_DICE
			if((unitDetailsInSceneFile->closeCombatAttackTotal >= MIN_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED) && (unitDetailsInSceneFile->closeCombatAttackTotal <= MAX_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED))
			{
				*spriteTextString = *spriteTextString + (unitDetailsInSceneFile->closeCombatAttackTotal + SPRITE_CHARACTER_DICE_OFFSET);
			}
			else
			{
			#ifndef SPRITE_ALWAYS_ADD_TEXT
				string tempString = SHAREDvars.convertIntToString((unitDetailsInSceneFile->closeCombatAttackTotal)*2);
				*spriteTextString = *spriteTextString + tempString;
			#endif
			}
		#endif
		}
		else
		{
			*spriteTextString = *spriteTextString + CHAR_DASH;
		}
		//cout << "*spriteTextString=" <<* spriteTextString << endl;

#else

	/*Generic Attack Related Sprite Information*/
	*spriteTextString = *spriteTextString + '\n';

	#ifdef SPRITE_TEXTUAL_INCLUDE_DESCRIPTION_TEXT
		*spriteTextString = *spriteTextString + "A = ";
	#endif
		if((unitDetailsInSceneFile->closeCombatAttackTotal > 0) || (unitDetailsInSceneFile->longDistanceAttackTotal > 0))
		{
			if((unitDetailsInSceneFile->closeCombatAttackValue) > (unitDetailsInSceneFile->longDistanceAttackValue))
			{
			#ifdef SPRITE_ALWAYS_ADD_TEXT
				string tempString = SHAREDvars.convertIntToString(baseCloseCombatAttackLevel);
				*spriteTextString = *spriteTextString + tempString;
				if(unitDetailsInSceneFile->closeCombatAttackBonus > 0)
				{
					string tempString = SHAREDvars.convertIntToString(unitDetailsInSceneFile->closeCombatAttackBonus);
					*spriteTextString = *spriteTextString + " + " + tempString;
				}
			#endif
			#ifdef SPRITES_DISPLAY_DICE
				if((unitDetailsInSceneFile->closeCombatAttackTotal >= MIN_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED) && (unitDetailsInSceneFile->closeCombatAttackTotal <= MAX_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED))
				{
					char charTemp = (unitDetailsInSceneFile->closeCombatAttackTotal + SPRITE_CHARACTER_DICE_OFFSET);
					*spriteTextString = *spriteTextString + charTemp;
				}
				else
				{
				#ifndef SPRITE_ALWAYS_ADD_TEXT
					string tempString = SHAREDvars.convertIntToString((unitDetailsInSceneFile->closeCombatAttackTotal)*2);
					*spriteTextString = *spriteTextString + tempString;
				#endif
				}
			#endif
			}
			else
			{
			#ifdef SPRITE_ALWAYS_ADD_TEXT
				string tempString = SHAREDvars.convertIntToString(baseLongDistanceAttackLevel);
				*spriteTextString = *spriteTextString + tempString;
				if(unitDetailsInSceneFile->longDistanceAttackBonus > 0)
				{
					string tempString = SHAREDvars.convertIntToString(unitDetailsInSceneFile->longDistanceAttackBonus);
					*spriteTextString = *spriteTextString + " - " + tempString;
				}
			#endif
			#ifdef SPRITES_DISPLAY_DICE
				if((unitDetailsInSceneFile->longDistanceAttackTotal >= MIN_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED) && (unitDetailsInSceneFile->longDistanceAttackTotal <= MAX_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED))
				{
					//cout << "DEBUG 1i2" << endl;
					//cout << "DEBUG 1i3" << endl;
					char charTemp = (unitDetailsInSceneFile->longDistanceAttackTotal + SPRITE_CHARACTER_DICE_OFFSET);
					*spriteTextString = *spriteTextString + charTemp;
				}
				else
				{
				#ifndef SPRITE_ALWAYS_ADD_TEXT
					string tempString = SHAREDvars.convertIntToString((unitDetailsInSceneFile->longDistanceAttackTotal)*2);
					*spriteTextString = *spriteTextString + tempString;
				#endif
				}
			#endif
			}
		}
		else
		{
			*spriteTextString = *spriteTextString + CHAR_DASH;
		}
		//cout << "*spriteTextString=" <<* spriteTextString << endl;

#endif

	//cout << "DEBUG 1k" << endl;

#ifdef SPRITE_TEXTUAL_INCLUDE_MOVEMENT
	/*Movement Related Sprite Information*/
	*spriteTextString = *spriteTextString + CHAR_NEWLINE;
	//*spriteTextString = *spriteTextString + '\n';
#ifdef SPRITE_TEXTUAL_INCLUDE_DESCRIPTION_TEXT
	*spriteTextString = *spriteTextString + "     M = ";
#endif
	if(unitDetailsInSceneFile->movementSpeed > 0)
	{
		//itoa(unitDetailsInSceneFile->movementSpeed, tempString, 10);
		string tempString = SHAREDvars.convertIntToString(unitDetailsInSceneFile->movementSpeed);
		*spriteTextString = *spriteTextString + tempString;

		if(unitDetailsInSceneFile->longDistanceAttackBonus > 0)
		{
			string tempString = SHAREDvars.convertIntToString(unitDetailsInSceneFile->longDistanceAttackBonus);
			*spriteTextString = *spriteTextString + " + " + tempString;
		}
	}
	else
	{
		*spriteTextString = *spriteTextString + CHAR_DASH;
	}
#endif

	//cout << "DEBUG 1l" << endl;

	//cout << "DEBUG:* spriteTextString = \n" <<* spriteTextString << "\n\n" << endl;
	/*End Start Sprite Text Creation*/
}









