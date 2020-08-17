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
 * File Name: LRRCgame.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3f4a 11-July-2015
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 *******************************************************************************/


#ifndef HEADER_LRRC_GAME
#define HEADER_LRRC_GAME

//#include "LRRCunitClass.h"
#include "LRRCplayerClass.h"
#include "LRRCglobalDefs.h"
#include "LDreferenceClass.h"


#define XML_NN_SCENE_FILE_NAME_EXTENSION ".xml"
#define VECGRAPHICS_LDR_NN_SCENE_FILE_NAME_EXTENSION ".ldr"
#define VECGRAPHICS_TAL_NN_SCENE_FILE_NAME_EXTENSION ".tal"
#define RAYTRACEDIMAGE_PPM_NN_SCENE_FILE_NAME_EXTENSION ".ppm"
#define EXPERIENCES_NN_SCENE_FILE_NAME_EXTENSION ".data"


/*
#define GAME_MOVEMENT_PHASE_DISPLAY_RESULT
#define GAME_OUTPUT_COMBAT_ENTRY_STATISTICS
*/
//see modelclass.h for important definitions regarding game rules

#define GAME_PHASE_EXECUTION_DISPLAY_START (1)
#define GAME_PHASE_EXECUTION_DISPLAY_RANGESPRITES (2)
#define GAME_PHASE_EXECUTION_DISPLAY_TARGETSPRITES (3)
#define GAME_PHASE_EXECUTION_DISPLAY_END (4)

#define GAME_ROUNDS_MAX_NUM (1000000)
#define GAME_ROUND_STRING_MAX_LEN (8)
#define GAME_ROUND_DEFAULT (1)
#define GAME_NUM_DEFAULT (1)

#ifdef DEBUG_PERFORM_ONLY_1_GAME
#define GAME_NUM_MAX_NUM (1)
#else
#define GAME_NUM_MAX_NUM (10)
#endif

#define GAME_PLAYERS_NUM_DEFAULT (4)
#define GAME_PLAYERS_MAX_NUM (1000000)
#define GAME_PLAYERS_STRING_MAX_LEN (8)
#define GAME_PLAYER_TURN_DEFAULT (1)


#define SCENE_FILE_NAME_START_NAME "SCENE_FILE_NAME_START"
#define SCENE_FILE_NAME_GAME_HEADER_NAME "SCENE_FILE_NAME_GAME_HEADER"
#define SCENE_FILE_NAME_ROUND_HEADER_NAME "SCENE_FILE_NAME_ROUND_HEADER"
#define SCENE_FILE_NAME_PLAYER_HEADER_NAME "SCENE_FILE_NAME_PLAYER_HEADER"

#define SCENE_FILE_NAME_PHASE_START_NAME "SCENE_FILE_NAME_PHASE_START"
#define SCENE_FILE_NAME_PHASE_RANGESPRITES_NAME "SCENE_FILE_NAME_PHASE_RANGESPRITES"
#define SCENE_FILE_NAME_PHASE_TARGETSPRITES_NAME "SCENE_FILE_NAME_PHASE_TARGETSPRITES"
#define SCENE_FILE_NAME_PHASE_END_NAME "SCENE_FILE_NAME_PHASE_END"
#define SCENE_FILE_NAME_PHASE_MOVEMENT_NAME "SCENE_FILE_NAME_PHASE_MOVEMENT"
#define SCENE_FILE_NAME_PHASE_LONGDISTANCE_NAME "SCENE_FILE_NAME_PHASE_LONGDISTANCE"
#define SCENE_FILE_NAME_PHASE_CLOSECOMBAT_NAME "SCENE_FILE_NAME_PHASE_CLOSECOMBAT"
#define SCENE_FILE_NAME_EXTENSION_NAME "SCENE_FILE_NAME_EXTENSION"

#define SCENE_FILE_NAME_NEURALNET_HEADER "nn"

/*
#define SCENE_FILE_NAME_START "scene"
#define SCENE_FILE_NAME_ROUND_HEADER "R"
#define SCENE_FILE_NAME_PLAYER_HEADER "P"
#define SCENE_FILE_NAME_PHASE_START "A"
#define SCENE_FILE_NAME_PHASE_RANGESPRITES "B"
#define SCENE_FILE_NAME_PHASE_TARGETSPRITES "C"
#define SCENE_FILE_NAME_PHASE_END "D"
#define SCENE_FILE_NAME_PHASE_MOVEMENT "move"
#define SCENE_FILE_NAME_PHASE_LONGDISTANCE "long"
#define SCENE_FILE_NAME_PHASE_CLOSECOMBAT "close"
#define SCENE_FILE_NAME_EXTENSION ".ldr"
*/
#define SCENE_FILE_NAME_MAX_LEN (50)





void fillInGameExternVariables();

//Initiation
#ifdef USE_ANN
bool executeLRRCfunctionsWithAI();
#endif
bool executeLRRCfunctionsInOrder();

bool gameObtainNumberPlayers(int* numberOfPlayers);
bool gameObtainRoundPlayerTurnAndPhase(int* initialRound, int* initialPlayerTurn, int* initialPhase);
bool gamePlay(int initialRound, int initialPlayerTurn, int initialPhase, int numberOfPlayers, Player* initialPlayerInList, int currentGame, LDreference* initialReferenceInThisPhaseStartSceneGlobal, bool allPlayersAI);
	bool executePhase(int currentGame, int currentRound, int currentPlayerTurn, int currentPhase, Player* initialPlayerInList, int numberOfPlayers, LDreference* initialReferenceInThisPhaseStartSceneGlobal, bool allPlayersAI);
		bool executeMovement(int currentGame, int currentRound, int currentPlayerTurn, Player* initialPlayerInList, int numberOfPlayers, LDreference* initialReferenceInThisPhaseStartSceneGlobal, bool allPlayersAI);
		bool executeLongDistanceCombat(int currentGame, int currentRound, int currentPlayerTurn, Player* initialPlayerInList, int numberOfPlayers, LDreference* initialReferenceInThisPhaseStartSceneGlobal, bool allPlayersAI);
		bool executeCloseCombat(int currentGame, int currentRound, int currentPlayerTurn, Player* initialPlayerInList, int numberOfPlayers, LDreference* initialReferenceInThisPhaseStartSceneGlobal, bool allPlayersAI);
			bool executeGenericCombat(int currentRound, int currentPlayerTurn, int currentPhase, LDreference* initialReferenceInPreMovementPhaseScene, LDreference* initialReferenceInThisPhaseStartScene, char preCombatPhaseSceneFileName[], char postCombatPhaseSceneFileName[], char targetSpritesSceneFileName[], Player* initialPlayerInList, bool allPlayersAI);
			#ifdef USE_ANN
				bool AIsearchUnitListForPhaseActionSelectionInitialisation(int currentRound, int currentPlayerTurn, int currentPhase, LDreference* initialReferenceInThisPhaseStartScene, char preCombatPhaseSceneFileName[], char postCombatPhaseSceneFileName[], char targetSpritesSceneFileName[], LDreference* targetSpriteListInitialReference, int* numTargetSpritesAdded, Player* initialPlayerInList, UnitListClass* firstUnitInUnitGroup, UnitListClass* firstUnitInOpponentUnitGroup);
					bool updateAbsolutePositionOfAllSubModels(LDreference* firstReferenceWithinSubModel, LDreference* parentReference);
					bool moveUnitTowardsOpponent(LDreference* unitReference, LDreference* opponentReference);
					bool AIsearchUnitListAndCalculateWorthOfOpponents(int currentPlayerTurn, int currentPhase, LDreference* initialReferenceInThisPhaseStartScene, Player* initialPlayerInList, UnitListClass* firstUnitInOpponentUnitGroup, UnitListClass* playerUnitThatIsFindingAnOpponent, LDreference* referenceToPlayerUnitThatIsFindingAnOpponent);
			#endif
					int performGenericCombatWithTwoCombatReadyUnitsAndAddSprites(int currentRound, int currentPlayerTurn, int currentPhase, LDreference* initialReferenceInPreMovementPhaseScene, LDreference* initialReferenceInThisPhaseStartScene, char targetSpritesSceneFileName[], char* unitAttackerFileName, char* unitDefenderFileName, int unitAttackerPlayerID, int unitDefenderPlayerID, LDreference* targetSpriteListInitialReference, int* numTargetSpritesAdded, Player* initialPlayerInList, bool checkPreviousSceneFile);

			bool prepareNextPhaseSceneFiles(int nextPhase, int nextPlayerTurn, char* previousPhaseSceneFileName, char* nextSceneFileName, char* rangeSpritesNextSceneFileName, LDreference* firstReferenceInPreviousScene, bool allPlayersAI);
		//bool AIsearchSceneFileListAndPerformMovements(int currentPlayerTurn, int currentPhase, char preMovementPhaseSceneFileNameMovement[], char thisPhaseStartSceneFileNameMovement[], char preCombatPhaseSceneFileName[], char postCombatPhaseSceneFileName[], char targetSpritesSceneFileName[], LDreference* initialReferenceInSceneFile, LDreference* targetSpriteListInitialReference, int* numTargetSpritesAdded, Player* initialPlayerInList);
bool generateSceneFileName(int currentGame, int currentRound, int currentPlayerTurn, int currentPhase, int phaseExecutionStage, char* sceneFileName);
bool generatePlayerList(int numberOfPlayers, Player* initialPlayerInList, UnitListClass* firstUnitInUnitList);
bool generatePlayerList(int numberOfPlayers, Player* initialPlayerInList);

void generateXMLNNSceneFileName(int currentGame, string* sceneFileName, int nnIndex);
void generateVectorGraphicsLDRNNSceneFileName(int currentGame, string* sceneFileName, int nnIndex);
void generateVectorGraphicsLDRNNSceneFileNameWithSprites(int currentGame, string* sceneFileName, int nnIndex);
void generateExperiencesNNSceneFileName(int currentGame, string* sceneFileName, int nnIndex);
void generateVectorGraphicsTALNNSceneFileName(int currentGame, string* sceneFileName, int nnIndex);
void generateRaytracedImagePPMNNSceneFileName(int currentGame, string* sceneFileName, int nnIndex);

void copyReferencesAndSubmodelDetails(LDreference* referenceNew, LDreference* referenceToCopy, int type);

#ifdef USE_ANN
void feedNeuralNetworkWithGameUnitExperiences(ANNneuronContainer* firstInputNeuronInNetwork, ANNneuronContainer* firstOutputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, UnitListClass* firstUnitInUnitGroup, int nn);
void trainAndOutputNeuralNetwork(ANNneuronContainer* firstInputNeuronInNetwork, ANNneuronContainer* firstOutputNeuronInNetwork, int numberOfInputNeurons, int numberOfOutputNeurons, ANNexperience* firstExperienceInList, bool addSprites, bool allowRaytrace, int nn, int currentGame);
#endif

void updatePlayerStatus(Player* initialPlayerInList);
	bool searchUnitListAndCheckThatSomePlayerUnitsAreAlive(int playerTeam, UnitListClass* firstUnitInUnitGroup);


#endif



//NEW Examples of scene files;
//
//scene file (with 8 players): sceneR15P1moveA.ldr
//scene file (with 8 players): sceneR15P1moveB.ldr
//scene file (with 8 players): sceneR15P1longA.ldr
//scene file (with 8 players): sceneR15P1longB.ldr
//scene file (with 8 players): sceneR15P1closeA.ldr
//scene file (with 8 players): sceneR15P1closeB.ldr
//scene file (with 8 players): sceneR15P2moveA.ldr
//scene file (with 8 players): sceneR15P2moveB.ldr
//scene file (with 8 players): sceneR15P2closeA.ldr
//scene file (with 8 players): sceneR15P2closeB.ldr
//scene file (with 8 players): sceneR15P2longA.ldr
//scene file (with 8 players): sceneR15P2longB.ldr
//scene file (with 8 players): sceneR15P3moveA.ldr
//scene file (with 8 players): sceneR15P3moveB.ldr
// ...
//...
//scene file (with 8 players): sceneR15P8moveA.ldr
//scene file (with 8 players): sceneR15P8moveB.ldr
//scene file (with 8 players): sceneR15P8longA.ldr
//scene file (with 8 players): sceneR15P8longB.ldr
//scene file (with 8 players): sceneR15P8closeA.ldr
//scene file (with 8 players): sceneR15P8closeB.ldr
//scene file (with 8 players): sceneR16P1moveA.ldr
//scene file (with 8 players): sceneR16P1moveB.ldr
//scene file (with 8 players): sceneR16P1closeA.ldr
//scene file (with 8 players): sceneR16P1closeB.ldr
//scene file (with 8 players): sceneR16P1longA.ldr
//scene file (with 8 players): sceneR16P1longB.ldr
//scene file (with 8 players): sceneR16P2moveA.ldr
//scene file (with 8 players): sceneR16P2moveB.ldr
//scene file (with 8 players): sceneR16P2closeA.ldr
//scene file (with 8 players): sceneR16P2closeB.ldr
//scene file (with 8 players): sceneR16P2longA.ldr
//scene file (with 8 players): sceneR16P2longB.ldr
//scene file (with 8 players): sceneR16P3moveA.ldr
//scene file (with 8 players): sceneR16P3moveB.ldr
//...
//...


//OLD Examples of scene files;
//
//scene file (with 8 players): sceneR15P1move.ldr
//scene file (with 8 players): sceneR15P1long.ldr
//scene file (with 8 players): sceneR15P1close.ldr
//scene file (with 8 players): sceneR15P2move.ldr
//scene file (with 8 players): sceneR15P2close.ldr
//scene file (with 8 players): sceneR15P2long.ldr
//scene file (with 8 players): sceneR15P3move.ldr
// ...
//...
//scene file (with 8 players): sceneR15P8move.ldr
//scene file (with 8 players): sceneR15P8long.ldr
//scene file (with 8 players): sceneR15P8close.ldr
//scene file (with 8 players): sceneR16P0close.ldr (this is an exact copy of sceneR15P8close.ldr, used for quick referencing purposes)
//scene file (with 8 players): sceneR16P1move.ldr
//scene file (with 8 players): sceneR16P1close.ldr
//scene file (with 8 players): sceneR16P1long.ldr
//scene file (with 8 players): sceneR16P2move.ldr
//scene file (with 8 players): sceneR16P2close.ldr
//scene file (with 8 players): sceneR16P2long.ldr
//scene file (with 8 players): sceneR16P3move.ldr
//...
//...


