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
 * File Name: LRRCsprite.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3i19d 15-December-2016
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


/*spriteCode.cpp - 10-Sept-06 12:30pm - for integration into legoRulesRoundsChecker. This code allows the addition of a sprite into a given scene file where a sprite is a paragraph of text. [The text is to be rendered in 3D, and point towards the user POV - see CHECK THIS for missing code]*/

#ifndef HEADER_LRRC_SPRITE
#define HEADER_LRRC_SPRITE

#include "LDreferenceClass.h"

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


void fillInLRRCSpriteExternVariables();

/*top level sprite routine*/
bool LRRCaddUnitDetailsSpritesToSceneFile(string sceneFileName, string sceneFileNameWithSprites, bool addTextualSpriteInfo, bool addRangeSpriteInfo, int currentPhase, int currentPlayerTurn);
bool LRRCaddUnitDetailsSpritesToScene(string sceneFileName, string sceneFileNameWithSprites, bool addTextualSpriteInfo, bool addRangeSpriteInfo, int currentPhase, int currentPlayerTurn, LDreference* firstReferenceInScene);
	void LRRCsearchSceneRefListAddUnitDetailsSpriteForSubmodels(LDreference* referenceInSceneFile, LDreference* spriteListInitialReference, vec* eyeCoords, int* numSpritesAdded, string sceneFileName, LDreference* initialReferenceInSceneFile, bool addTextualSpriteInfo, bool addRangeSpriteInfo, int currentPhase, int currentPlayerTurn);
		bool LRRCdetermineSpriteInfoAndAddSpriteToSpriteRefList(LDreference* unitReference, LDreference* targetReference, LDreference* spriteListInitialReference, vec* eyeCoords, int* numSpritesAdded, string sceneFileName, bool addTextualSpriteInfo, bool addRangeSpriteInfo, bool addTargetSpriteInfo, int currentPhase, int currentPlayerTurn);
			void LRRCgenerateTextualSpriteInfoString(LDreference* unitReferenceInSceneFile, string* spriteTextString, int spriteColourArray[]);
			LDreference* LRRCaddRangeSpriteInfoToReferenceList(LDreference* spriteSubmodelInitialReference, LDreference* unitReference, int currentPhase, int* numSpritesAdded);
			LDreference* LRRCaddTargetSpriteInfoToReferenceList(LDreference* spriteSubmodelInitialReference, LDreference* unitReference, LDreference* targetReference, int currentPhase, int* numSpritesAdded);


#endif

