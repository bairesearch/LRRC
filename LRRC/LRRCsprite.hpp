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
 * File Name: LRRCsprite.hpp
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


/*spriteCode.cpp - 10-Sept-06 12:30pm - for integration into LRRC. This code allows the addition of a sprite into a given scene file where a sprite is a paragraph of text. [The text is to be rendered in 3D, and point towards the user POV - see CHECK THIS for missing code]*/

#ifndef HEADER_LRRC_SPRITE
#define HEADER_LRRC_SPRITE

#include "LDreferenceClass.hpp"
#include "LRRCglobalDefs.hpp"
#include "LRRCcombat.hpp"
#include "LRRCrules.hpp"
#include "LDsprite.hpp"
#include "LDparser.hpp"
#include "LDreferenceManipulation.hpp"
#include "SHAREDvector.hpp"
#include "math.h"

#define CPLUSPLUSERRORCORRECTION1 (10000)
//#define CPLUSPLUSERRORCORRECTION3

#define LRRC_SPRITE_HEADER_NAME "\n0 Start OpenLRRC.exe Sprites\n"
#define LRRC_SPRITE_TRAILER_NAME "0 End OpenLRRC.exe Sprites\n"
#define LRRC_SPRITE_HEADER_NUM_LINES (2)
#define LRRC_SPRITE_TRAILER_NUM_LINES (1)
#define LRRC_SPRITE_HEADER_LENGTH 26
#define LRRC_SPRITE_TRAILER_LENGTH 23


#define SPRITE_SUBMODEL_RANGE_FILE_NAME_NAME "SPRITE_SUBMODEL_RANGE_FILE_NAME"		//CHECKTHIS this can be changed between circle and ring depending upon High Resolution renderer; Ray Tracer versus LDVIEW [and taking into account other factors Eg LGEO parts library]
#define SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_TRANS_NAME "SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_TRANS"
#define SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_TRANS_NAME "SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_TRANS"
#define SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_TRANS_NAME "SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_TRANS"
#define SPRITE_SUBMODEL_RANGE_TARGET_SELECTION_COLOUR_TRANS_NAME "SPRITE_SUBMODEL_RANGE_TARGET_SELECTION_COLOUR_TRANS"
#define SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_OPAQ_NAME "SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_OPAQ"
#define SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_OPAQ_NAME "SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_OPAQ"
#define SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_OPAQ_NAME "SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_OPAQ"
#define SPRITE_SUBMODEL_RANGE_TARGET_SELECTION_COLOUR_OPAQ_NAME "SPRITE_SUBMODEL_RANGE_TARGET_SELECTION_COLOUR_OPAQ"
#define SPRITE_SUBMODEL_RANGE_SCALE_FACTOR_NAME "SPRITE_SUBMODEL_RANGE_SCALE_FACTOR"		//this value is now multiplied by LDRAW_UNITS_PER_CM 							//theoretically should be: LDRAW_UNITS_PER_CM	- old = 3.0 with ring
/*
#define SPRITE_SUBMODEL_RANGE_FILE_NAME "48\\4-4EDGE.DAT"		//CHECKTHIS this should be changed to a part that can be rendered in High Resolution in a Ray Tracer [Eg via LGEO parts library]
#define SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_TRANS (DAT_FILE_COLOUR_GREEN+DAT_FILE_COLOUR_TRANPARENT_MOD)
#define SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_TRANS (DAT_FILE_COLOUR_RED+DAT_FILE_COLOUR_TRANPARENT_MOD)	//OLD: (DAT_FILE_COLOUR_YELLOW+DAT_FILE_COLOUR_TRANPARENT_MOD)
#define SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_TRANS (DAT_FILE_COLOUR_RED+DAT_FILE_COLOUR_TRANPARENT_MOD)
#define SPRITE_SUBMODEL_RANGE_MOVEMENT_COLOUR_OPAQ (DAT_FILE_COLOUR_GREEN)
#define SPRITE_SUBMODEL_RANGE_LONGDISTANCE_COMBAT_COLOUR_OPAQ (DAT_FILE_COLOUR_RED) //OLD: (DAT_FILE_COLOUR_YELLOW)
#define SPRITE_SUBMODEL_RANGE_CLOSE_COMBAT_COLOUR_OPAQ (DAT_FILE_COLOUR_RED)
#define SPRITE_SUBMODEL_RANGE_SCALE_FACTOR (LDRAW_UNITS_PER_CM)		//theoretically should be: LDRAW_UNITS_PER_CM	- old = 3.0 with ring
*/

#define SPRITE_TEXTUAL_INCLUDE_DESCRIPTION_TEXT
#define SPRITE_TEXTUAL_NUM_OF_LINES (7) /*defence level, LD attack level, CC attack level, movement level*/
#define SPRITE_TEXTUAL_INCLUDE_NAME_INFO
#define SPRITE_TEXTUAL_INCLUDE_PLAYERID_INFO


class LRRCspriteClass
{
	private: LDspriteClass LDsprite;
	private: LDparserClass LDparser;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: LDreferenceClassClass LDreferenceClass;
	private: LRRCmodelClassClass LRRCmodelClass;
	private: LRRCcombatClass LRRCcombat;
	private: SHAREDvarsClass SHAREDvars;
	private: SHAREDvectorClass SHAREDvector;
	
	public: void fillInLRRCSpriteExternVariables();

	/*top level sprite routine*/
	public: bool LRRCaddUnitDetailsSpritesToSceneFile(string sceneFileName, string sceneFileNameWithSprites, const bool addTextualSpriteInfo, const bool addRangeSpriteInfo, const int currentPhase, const int currentPlayerTurn);
	public: bool LRRCaddUnitDetailsSpritesToScene(const string sceneFileName, string sceneFileNameWithSprites, const bool addTextualSpriteInfo, const bool addRangeSpriteInfo, const int currentPhase, const int currentPlayerTurn, LDreference* firstReferenceInScene);
		private: void LRRCsearchSceneRefListAddUnitDetailsSpriteForSubmodels(LDreference* referenceInSceneFile, LDreference* spriteListInitialReference, const vec* eyeCoords, int* numSpritesAdded, string sceneFileName, const LDreference* initialReferenceInSceneFile, const bool addTextualSpriteInfo, const bool addRangeSpriteInfo, const int currentPhase, const int currentPlayerTurn);
			public: bool LRRCdetermineSpriteInfoAndAddSpriteToSpriteRefList(LDreference* unitReference, LDreference* targetReference, LDreference* spriteListInitialReference, const vec* eyeCoords, int* numSpritesAdded, string sceneFileName, const bool addTextualSpriteInfo, const bool addRangeSpriteInfo, const bool addTargetSpriteInfo, const int currentPhase, const int currentPlayerTurn);
				private: void LRRCgenerateTextualSpriteInfoString(LDreference* unitReferenceInSceneFile, string* spriteTextString, int spriteColourArray[]);
				private: LDreference* LRRCaddRangeSpriteInfoToReferenceList(LDreference* spriteSubmodelInitialReference, LDreference* unitReference, const int currentPhase, int* numSpritesAdded);
				private: LDreference* LRRCaddTargetSpriteInfoToReferenceList(LDreference* spriteSubmodelInitialReference, LDreference* unitReference, LDreference* targetReference, const int currentPhase, int* numSpritesAdded);
};


#endif

