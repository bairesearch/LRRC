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
 * File Name: LRRCgame.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: LD Rules Rounds Checker
 * Project Version: 3n7e 17-August-2020
 * Project First Internal Release: 1aXx 18-Sept-05 (C)
 * Project Second Internal Release: 2aXx 02-April-06 (convert to C++)
 * Project Third Internal Release: 2b7d 26-Sept-06 (added sprites)
 * Project Fourth Internal Release: 2c4a 25-Feb-07 [rotation of models supported, copyCombatRelevantInheritedImmediateParentUnitDetailsIntoChildObject() no longer used]
 * Project Fifth Internal Release: 2d11a 3-June-07
 *
 * NB phase specific sprites are yet to be added to scene files based upon movement/combat outcomes
 * NB phase specific sprites are yet to be added to scene files based upon future movement/combat options
 *******************************************************************************/



#include "LRRCgame.hpp"

#ifdef USE_ANN
#endif


static int currentRound = GAME_ROUND_DEFAULT;							//1, 2, 3 .. etc
static int currentPlayerTurn = GAME_PLAYER_TURN_DEFAULT;				//1, 2, 3... etc [total number of players]
static int currentPhase = GAME_PHASE_DEFAULT;							//movement, longdist combat, or closecombat
static int currentNumberPlayers = GAME_PLAYERS_NUM_DEFAULT;		//arbitrary


static string SCENE_FILE_NAME_START;
static string SCENE_FILE_NAME_GAME_HEADER;
static string SCENE_FILE_NAME_ROUND_HEADER;
static string SCENE_FILE_NAME_PLAYER_HEADER;
static string SCENE_FILE_NAME_PHASE_START;
static string SCENE_FILE_NAME_PHASE_RANGESPRITES;
static string SCENE_FILE_NAME_PHASE_TARGETSPRITES;
static string SCENE_FILE_NAME_PHASE_END;
static string SCENE_FILE_NAME_PHASE_MOVEMENT;
static string SCENE_FILE_NAME_PHASE_LONGDISTANCE;
static string SCENE_FILE_NAME_PHASE_CLOSECOMBAT;
static string SCENE_FILE_NAME_EXTENSION;

void LRRCgameClass::fillInGameExternVariables()
{
	XMLrulesClass* currentReferenceRulesClass = LRRCrulesMiscellaneous;

	while(currentReferenceRulesClass->next != NULL)
	{
		if(currentReferenceRulesClass->name == SCENE_FILE_NAME_START_NAME)
		{
			SCENE_FILE_NAME_START = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SCENE_FILE_NAME_GAME_HEADER_NAME)
		{
			SCENE_FILE_NAME_GAME_HEADER = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SCENE_FILE_NAME_ROUND_HEADER_NAME)
		{
			SCENE_FILE_NAME_ROUND_HEADER = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SCENE_FILE_NAME_PLAYER_HEADER_NAME)
		{
			SCENE_FILE_NAME_PLAYER_HEADER = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SCENE_FILE_NAME_PHASE_START_NAME)
		{
			SCENE_FILE_NAME_PHASE_START = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SCENE_FILE_NAME_PHASE_RANGESPRITES_NAME)
		{
			SCENE_FILE_NAME_PHASE_RANGESPRITES = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SCENE_FILE_NAME_PHASE_TARGETSPRITES_NAME)
		{
			SCENE_FILE_NAME_PHASE_TARGETSPRITES = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SCENE_FILE_NAME_PHASE_END_NAME)
		{
			SCENE_FILE_NAME_PHASE_END = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SCENE_FILE_NAME_PHASE_MOVEMENT_NAME)
		{
			SCENE_FILE_NAME_PHASE_MOVEMENT = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SCENE_FILE_NAME_PHASE_LONGDISTANCE_NAME)
		{
			SCENE_FILE_NAME_PHASE_LONGDISTANCE = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SCENE_FILE_NAME_PHASE_CLOSECOMBAT_NAME)
		{
			SCENE_FILE_NAME_PHASE_CLOSECOMBAT = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SCENE_FILE_NAME_EXTENSION_NAME)
		{
			SCENE_FILE_NAME_EXTENSION = currentReferenceRulesClass->stringValue;
		}
		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
}

#ifdef USE_ANN



bool LRRCgameClass::executeLRRCfunctionsWithAI()
{
	bool result = true;

	int initialRound;
	int initialPlayerTurn;
	int initialPhase;
	int numberPlayers;

	if(!LRRCrules.parseLRRCrulesXMLfile())
	{
		result = false;
	}

	LRRCcombat.fillInCombatExternVariables();
	fillInGameExternVariables();
	LRRCmodelClass.fillInModelClassExternVariables();
	LRRCparser.fillInParserExternVariables();
	LRRCplayerClass.fillInPlayerClassExternVariables();
	LRRCsprite.fillInLRRCSpriteExternVariables();

	#ifdef DEBUG_IN_TEXTPAD
		numberPlayers = 2;
		cout << "numberPlayers automatically chosen; DEBUG_IN_TEXTPAD" << endl;
		cout << "numberPlayers = " << numberPlayers << endl;
	#else
		gameObtainNumberPlayers(&numberPlayers);
	#endif

	Player* initialPlayerInList = new Player();
	bool allPlayersAI;

	allPlayersAI = generatePlayerList(numberPlayers, initialPlayerInList);



	//create AI neural network and share neural network between players (ie give each player the same neural network)

	//create neural network 1 (for unit/group targetting/combat decisions)


	#define IRRELEVANT (0)

#ifndef USE_SEPARATE_AI_PER_PLAYER
		//create neural network 1 (for unit/group targetting/combat decisions)
		LRRCgameAI.initialiseNeuralNetwork(GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, initialPlayerInList, GAME_PHASE_CLOSECOMBAT);
	#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
		LRRCgameAI.initialiseNeuralNetwork(GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, initialPlayerInList, GAME_PHASE_LONGDISTANCECOMBAT);
	#endif

		//create neural network 2 (for unit/group targetting/combat decisions)
		LRRCgameAI.initialiseNeuralNetwork(GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, initialPlayerInList, GAME_PHASE_CLOSECOMBAT);
	#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
		LRRCgameAI.initialiseNeuralNetwork(GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, initialPlayerInList, GAME_PHASE_LONGDISTANCECOMBAT);
	#endif

	#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
		//create neural network 3 (for global targetting/combat decisions)
		LRRCgameAI.initialiseNeuralNetwork(GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN, initialPlayerInList, GAME_PHASE_CLOSECOMBAT);
	#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
		LRRCgameAI.initialiseNeuralNetwork(GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN, initialPlayerInList, GAME_PHASE_LONGDISTANCECOMBAT);
	#endif
	#endif

#endif



	Player* currentPlayerInList = initialPlayerInList;
	while(currentPlayerInList->next != NULL)
	{
	#ifdef USE_SEPARATE_AI_PER_PLAYER
			//create neural network 1 (for unit/group targetting/combat decisions)
			LRRCgameAI.initialiseNeuralNetwork(GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, currentPlayer, GAME_PHASE_CLOSECOMBAT);
		#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
			LRRCgameAI.initialiseNeuralNetwork(GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, currentPlayer, GAME_PHASE_LONGDISTANCECOMBAT);
		#endif

			//create neural network 2 (for unit/group targetting/combat decisions)
			LRRCgameAI.initialiseNeuralNetwork(GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, currentPlayer, GAME_PHASE_CLOSECOMBAT);
		#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
			LRRCgameAI.initialiseNeuralNetwork(GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, currentPlayer, GAME_PHASE_LONGDISTANCECOMBAT);
		#endif


		#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
			//create neural network 3 (for global targetting/combat decisions)
			LRRCgameAI.initialiseNeuralNetwork(GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN, currentPlayer, GAME_PHASE_CLOSECOMBAT);
		#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
			LRRCgameAI.initialiseNeuralNetwork(GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN, currentPlayer, GAME_PHASE_LONGDISTANCECOMBAT);
		#endif
		#endif

	#else
		for(int nn=0; nn<GAME_NUMBER_OF_EXPERIENCE_NN; nn++)
		{
			currentPlayerInList->firstInputNeuronInNetwork[nn] = initialPlayerInList->firstInputNeuronInNetwork[nn];
			currentPlayerInList->firstOutputNeuronInNetwork[nn] = initialPlayerInList->firstOutputNeuronInNetwork[nn];
			currentPlayerInList->numberOfInputNeurons[nn] = initialPlayerInList->numberOfInputNeurons[nn];
			currentPlayerInList->numberOfOutputNeurons[nn] = initialPlayerInList->numberOfOutputNeurons[nn];
		}
	#endif
		currentPlayerInList = currentPlayerInList->next;


	}

	//create neural network 2 (for -) 2D Array with Radial Connectivity



	for(int gameNumber=1; gameNumber<=GAME_NUM_MAX_NUM; gameNumber++)
	{
		//NB it is assumed in the first game, first round, first player turn, no scene file with appropriate naming convention exists; copy scene.ldr to first game scene, eg sceneG1R1P1S1moveA.ldr
		initialRound = GAME_ROUND_DEFAULT;
		initialPlayerTurn = GAME_PLAYER_TURN_DEFAULT;
		initialPhase = GAME_PHASE_DEFAULT;
		string preMovementPhaseSceneFileName = "";
		generateSceneFileName(gameNumber, initialRound, initialPlayerTurn, initialPhase, GAME_PHASE_EXECUTION_DISPLAY_START, &preMovementPhaseSceneFileName);
		string initialSceneFileInGame = SCENE_FILE_NAME_START + SCENE_FILE_NAME_EXTENSION;
		//OLD: copyFiles(preMovementPhaseSceneFileName, initialSceneFileInGameCharArray);
		LDreference* initialReferenceInSceneFile = new LDreference();
		LDreference* topLevelReferenceInSceneFile = new LDreference(initialSceneFileInGame, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
		if(!LDparser.parseFile(initialSceneFileInGame, initialReferenceInSceneFile, topLevelReferenceInSceneFile, false))
		{//file does not exist
			cout << "The file: " << preMovementPhaseSceneFileName << " does not exist in the directory" << endl;
			result = false;
		}

		//NEW: collapse reference list into new file after parsing reference list;
		if(allPlayersAI)
		{
			LDreferenceManipulation.write2DreferenceListCollapsedTo1DtoFile(preMovementPhaseSceneFileName, initialReferenceInSceneFile);
		}
		else
		{//1D LDreference list required - no unit groups
			LDreferenceManipulation.writeReferencesToFile(preMovementPhaseSceneFileName, initialReferenceInSceneFile);
		}


		//DEBUG:
		LRRCgameReferenceManipulation.searchReferenceListPrintReferenceDetails(initialReferenceInSceneFile);

		UnitListClass* firstUnitInUnitList = new UnitListClass();
		LRRCgameAI.fillUnitList(initialReferenceInSceneFile, firstUnitInUnitList, currentRound);

		//debug:
		/*
		UnitListClass* currentUnitInList = firstUnitInUnitList;
		while(currentUnitInList->next != NULL)
		{
			cout << "currentUnitInList->name = " << currentUnitInList->name << endl;

			if(currentUnitInList->isUnitGroup)
			{
				UnitListClass* currentUnitInLLList = firstUnitInUnitList->firstUnitInUnitGroup;
				while(currentUnitInLLList->next != NULL)
				{
					cout << "currentUnitInLLList->name = " << currentUnitInLLList->name << endl;
					cout << "currentUnitInLLList->next->name = " << currentUnitInLLList->next->name << endl;

					currentUnitInLLList = currentUnitInLLList->next;
				}
				searchUnitListAssignHasNotPerformedAction(currentUnitInList->firstUnitInUnitGroup);
			}

			currentUnitInList = currentUnitInList->next;
		}
		*/
		
		/*
		UnitListClass* currentUnitInList = firstUnitInUnitList;
		while(currentUnitInList->next != NULL)
		{
			cout << "currentUnitInList->name = " << currentUnitInList->name << endl;
			if(currentUnitInList->isUnitGroup)
			{
				searchUnitListAssignHasNotPerformedAction(currentUnitInList->firstUnitInUnitGroup);
			}
			currentUnitInList = currentUnitInList->next;
		}
		*/

		//fill UnitLists for each player
		Player* currentPlayer = initialPlayerInList;
		while(currentPlayer->next != NULL)
		{
			currentPlayer->firstUnitInUnitList = firstUnitInUnitList;
			currentPlayer->status = ALIVE;
			currentPlayer = currentPlayer->next;
		}


		/*OLD:
		if(gameNumber != 1)
		{
			string preMovementPhaseSceneFileNameForCurrentGame = "";
			generateSceneFileName(gameNumber, currentRound, currentPlayerTurn, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_START, &preMovementPhaseSceneFileNameForCurrentGame);

			copyFiles(preMovementPhaseSceneFileNameForCurrentGame, preMovementPhaseSceneFileName);

			//reparse scene file
			initialReferenceInSceneFile = new LDreference();
			topLevelReferenceInSceneFile = new LDreference(preMovementPhaseSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!LDparser.parseFile(preMovementPhaseSceneFileNameForCurrentGame, initialReferenceInSceneFile, topLevelReferenceInSceneFile, false))
			{//file does not exist
				cout << "The file: " << preMovementPhaseSceneFileNameForCurrentGame << " does not exist in the directory" << endl;
				result = false;
			}

			//refill unit lists every game
			firstUnitInUnitList = new UnitListClass();
			LRRCgameAI.fillUnitList(initialReferenceInSceneFile, firstUnitInUnitList, currentRound);
			Player* currentPlayer = initialPlayerInList;
			while(currentPlayer->next != NULL)
			{
				currentPlayer->firstUnitInUnitList = firstUnitInUnitList;
				currentPlayer->status = ALIVE;
				currentPlayer = currentPlayer->next;
			}
		}
		*/


		if(!gamePlay(initialRound, initialPlayerTurn, initialPhase, numberPlayers, initialPlayerInList, gameNumber, initialReferenceInSceneFile, allPlayersAI))
		{
			result = false;
		}

		//delete reference list
		delete initialReferenceInSceneFile;
		cout << "finished deleting initialReferenceInSceneFile";
		delete topLevelReferenceInSceneFile;

		//delete common unit list (used by all players)
		delete firstUnitInUnitList;
	}

	//delete all players (including their individual experience lists)
	Player* currentPlayer = initialPlayerInList;
	while(currentPlayer->next != NULL)
	{
		Player* tempPlayer = currentPlayer;
		currentPlayer = currentPlayer->next;
		delete tempPlayer;
	}

	return result;
}
#endif


bool LRRCgameClass::executeLRRCfunctionsInOrder()
{
	bool result;

	bool UIstatus = true;
	string answerAsString = "";
	int answerAsInt;

	int initialGame;
	int initialRound;
	int initialPlayerTurn;
	int initialPhase;
	int numberPlayers;

	if(!LRRCrules.parseLRRCrulesXMLfile())
	{
		UIstatus = false;
	}
	LRRCcombat.fillInCombatExternVariables();
	fillInGameExternVariables();
	LRRCmodelClass.fillInModelClassExternVariables();
	LRRCparser.fillInParserExternVariables();
	LRRCplayerClass.fillInPlayerClassExternVariables();
	LRRCsprite.fillInLRRCSpriteExternVariables();


	while(UIstatus == true)
	{
		cout << "\n ---\n";
		cout << "Do you wish to \n";
		cout << " ---\n";
		cout << "1. start LRRC at an arbitrary round, player turn, and phase\n";
		cout << "2. start LRRC at round 1, player turn 1, phase 1\n";
		cout << "3. start LRRC at the current recorded round, player turn, and phase\n";
		cout << " ---\n";
		cout << "0. Exit\n";
		cout << " ---\n";
		cout <<	"Enter Answer (0,1,2,3... etc):\n\n>> ";


		cin >> answerAsString;
		answerAsInt = SHAREDvars.convertStringToInt(answerAsString);

		if(answerAsInt == 1)
		{
			initialGame = GAME_NUM_DEFAULT;
			gameObtainRoundPlayerTurnAndPhase(&initialRound, &initialPlayerTurn, &initialPhase);
			gameObtainNumberPlayers(&numberPlayers);

			Player* initialPlayerInList = new Player();
			UnitListClass* firstUnitInUnitList = new UnitListClass();
			generatePlayerList(numberPlayers, initialPlayerInList, firstUnitInUnitList);

			if(!gamePlay(initialRound, initialPlayerTurn, initialPhase, numberPlayers, initialPlayerInList, initialGame, NULL, false))
			{
				result = false;
			}
			UIstatus = false;
		}
		else if(answerAsInt == 2)
		{
			initialGame = GAME_NUM_DEFAULT;
			initialRound = GAME_ROUND_DEFAULT;
			initialPlayerTurn = GAME_PLAYER_TURN_DEFAULT;
			initialPhase = GAME_PHASE_DEFAULT;
			gameObtainNumberPlayers(&numberPlayers);

			Player* initialPlayerInList = new Player();
			UnitListClass* firstUnitInUnitList = new UnitListClass();
			generatePlayerList(numberPlayers, initialPlayerInList, firstUnitInUnitList);

			if(!gamePlay(initialRound, initialPlayerTurn, initialPhase, numberPlayers, initialPlayerInList, initialGame, NULL, false))
			{
				result = false;
			}
			UIstatus = false;
		}
		else if(answerAsInt == 3)
		{
			initialGame = GAME_NUM_DEFAULT;
			initialRound = currentRound;
			initialPlayerTurn = currentPlayerTurn;
			initialPhase = currentPhase;
			numberPlayers = currentNumberPlayers;

			Player* initialPlayerInList = new Player();
			UnitListClass* firstUnitInUnitList = new UnitListClass();
			generatePlayerList(numberPlayers, initialPlayerInList, firstUnitInUnitList);

			if(!gamePlay(initialRound, initialPlayerTurn, initialPhase, numberPlayers, initialPlayerInList, initialGame, NULL, false))
			{
				result = false;
			}
			UIstatus = false;
		}
		else if(answerAsInt == 0)
		{
			UIstatus = false;
		}
		else
		{
			cout << "\nInvalid answer, please try another command\n" << endl;
		}
	}

	return result;
}

bool LRRCgameClass::gameObtainNumberPlayers(int* numberOfPlayers)
{
	bool result = true;

	cout << "\nEnter Number of Players (1, 2, 3, ... MAX_NUMBER_PLAYERS) \n" << endl;

	if(!LRRCgameReferenceManipulation.obtainUserInputInt(numberOfPlayers))
	{
		result = false;
	}

	cout << "\nNumber of players selected = " <<* numberOfPlayers << "." << endl;

	return result;
}

bool LRRCgameClass::gameObtainRoundPlayerTurnAndPhase(int* initialRound, int* initialPlayerTurn, int* initialPhase)
{
	bool result = true;

	cout << "Enter Round Number (1, 2, 3, ...) \n" << endl;
	if(!LRRCgameReferenceManipulation.obtainUserInputInt(initialRound))
	{
		result = false;
	}
	cout << "\nRound Number selected = " <<* initialRound << "." << endl;

	cout << "Enter Player Turn (1, 2, 3, ... MAX_NUMBER_PLAYERS) \n" << endl;
	if(!LRRCgameReferenceManipulation.obtainUserInputInt(initialPlayerTurn))
	{
		result = false;
	}
	cout << "\nPlayer Turn selected = " <<* initialPlayerTurn << "." << endl;

	cout << "Enter Phase (1 - movement, 2 - int distance combat, 3 - close combat) \n" << endl;
	if(!LRRCgameReferenceManipulation.obtainUserInputInt(initialPhase))
	{
		result = false;
	}
	cout << "\nInitial Phase selected = " <<* initialPhase << "." << endl;

	return result;
}

bool LRRCgameClass::generatePlayerList(const int numberOfPlayers, Player* initialPlayerInList, UnitListClass* firstUnitInUnitList)
{
	bool allPlayersAI = true;

	Player* currentPlayer = initialPlayerInList;

	string answerAsString = "";

	for(int index = 1; index < (numberOfPlayers+1); index++)
	{
	#ifdef USE_PLAYER_NAMES
		cout << "Player number " << index << " is to be created.\n" << endl;
		cout << "Enter Player Name >> ";
		cin >> answerAsString;
	#else
		answerAsString = "Player" + index;
	#endif

	#ifdef USE_ANN
		cout << "\nPlayer number " << index << " is to be created.\n" << endl;
		cout << "Is Player AI? (y/n) >> ";

		#ifdef DEBUG_IN_TEXTPAD
		cout << "player AI automatically chosen; DEBUG_IN_TEXTPAD" << endl;
		cout << "player AI = yes " << endl;
		char answerAsChar = 'y';
		#else
		cin >> answerAsString;
		char answerAsChar = answerAsString[0];
		#endif

		if(answerAsChar == 'y')
		{
			currentPlayer->playerIsAI = true;
		}
		else
		{
			currentPlayer->playerIsAI = false;
			allPlayersAI = false;
		}
	#endif
		LRRCplayerClass.fillPlayerDetails(currentPlayer, answerAsString, index, PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT);
		currentPlayer->firstUnitInUnitList = firstUnitInUnitList;

		//code to create a new reference object
		Player* nextPlayer = new Player();
		currentPlayer -> next = nextPlayer;
		currentPlayer = currentPlayer->next;
	}

	return allPlayersAI;

}

bool LRRCgameClass::generatePlayerList(const int numberOfPlayers, Player* initialPlayerInList)
{
	bool allPlayersAI = true;

	Player* currentPlayer = initialPlayerInList;

	string answerAsString = "";

	for(int index = 1; index < (numberOfPlayers+1); index++)
	{
	#ifdef USE_PLAYER_NAMES
		cout << "Player number " << index << " is to be created.\n" << endl;
		cout << "Enter Player Name >> ";
		cin >> answerAsString;
	#else
		answerAsString = "Player" + index;
	#endif

	#ifdef USE_ANN
		cout << "\nPlayer number " << index << " is to be created.\n" << endl;
		cout << "Is Player AI? (y/n) >> ";

		#ifdef DEBUG_IN_TEXTPAD
		cout << "player AI automatically chosen; DEBUG_IN_TEXTPAD" << endl;
		cout << "player AI = yes " << endl;
		char answerAsChar = 'y';
		#else
		cin >> answerAsString;
		char answerAsChar = answerAsString[0];
		#endif

		if(answerAsChar == 'y')
		{
			currentPlayer->playerIsAI = true;
		}
		else
		{
			currentPlayer->playerIsAI = false;
			allPlayersAI = false;
		}
	#endif
		LRRCplayerClass.fillPlayerDetails(currentPlayer, answerAsString, index, PLAYER_BANK_ACCOUNT_INITIAL_DEFAULT);

		//code to create a new reference object
		Player* nextPlayer = new Player();
		currentPlayer -> next = nextPlayer;
		currentPlayer = currentPlayer->next;
	}

	return allPlayersAI;

}



bool LRRCgameClass::gamePlay(const int initialRound, int initialPlayerTurn, int initialPhase, const int numberOfPlayers, Player* initialPlayerInList, const int currentGame, LDreference* initialReferenceInThisPhaseStartSceneGlobal, const bool allPlayersAI)
{

	bool result = true;

	int currentRound;
	int currentPlayerTurn;
	int currentPhase;
	string answerAsString = "";

	int numberOfPlayersStillAlive = numberOfPlayers;	//an arbitrary number >= 2

	currentRound = initialRound;
	while((currentRound < GAME_ROUNDS_MAX_NUM) && (result == true) && (numberOfPlayersStillAlive >= 2))
	{

		//initialRound = GAME_ROUND_DEFAULT;
		currentPlayerTurn = initialPlayerTurn;
		Player* currentPlayer = initialPlayerInList;

		LRRCunitClass.searchUnitListAssignHasNotPerformedAction(initialPlayerInList->firstUnitInUnitList);

		//numberOfPlayersStillAlive = 0;	//get ready to count numberOfPlayersStillAlive
		
		while((currentPlayerTurn <= numberOfPlayers) && (result == true))
		{
			initialPlayerTurn = GAME_PLAYER_TURN_DEFAULT;
			currentPhase = initialPhase;


		#ifdef USE_ANN
			if(!allPlayersAI)
			{
				//update UnitLists for each player
				string preMovementPhaseSceneFileName = "";
				generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_START, &preMovementPhaseSceneFileName);
				LRRCgameAI.parseSceneFileAndUpdateUnitList(preMovementPhaseSceneFileName, initialPlayerInList->firstUnitInUnitList, currentRound);	//extract the unit list reference from an arbitrary player
			}
		#endif

			//updatePlayerStatus(initialPlayerInList);

			if(currentPlayer->status == ALIVE)
			{
				numberOfPlayersStillAlive++;

				while((currentPhase < GAME_PHASES_NUM_PER_TURN) && (result == true))
				{
					cout << "\n\nExecuting Phase: Game = " << currentGame << ", Round = " << currentRound << ", Player Turn = " << currentPlayerTurn << ", Phase = " << currentPhase << endl;

					initialPhase = GAME_PHASE_DEFAULT;

					bool stillPerformingPlayerTurnPhaseTrials = true;
					while(stillPerformingPlayerTurnPhaseTrials == true)
					{
						if(!executePhase(currentGame, currentRound, currentPlayerTurn, currentPhase, initialPlayerInList, numberOfPlayers, initialReferenceInThisPhaseStartSceneGlobal, allPlayersAI))
						{
							if(currentPlayer->playerIsAI == false)
							{
								cout << "Player Turn Phase Failed. Do you wish to try again (y/n)? \n\n>> ";
								cin >> answerAsString;
								if(answerAsString[0] == 'n')
								{
									stillPerformingPlayerTurnPhaseTrials = false;
									result = false;
								}
							}
							else
							{
								stillPerformingPlayerTurnPhaseTrials = false;
							}
						}
						else
						{
							stillPerformingPlayerTurnPhaseTrials = false;
						}
					}

					currentPhase++;
				}
			}

			currentPlayer = currentPlayer->next;
			currentPlayerTurn++;

		}

	#ifdef USE_ANN
		if(!allPlayersAI)
		{
			//update UnitLists for each player
			string preMovementPhaseSceneFileName = "";
			generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_START, &preMovementPhaseSceneFileName);
			LRRCgameAI.parseSceneFileAndUpdateUnitList(preMovementPhaseSceneFileName, initialPlayerInList->firstUnitInUnitList, currentRound);	//extract the unit list reference from an arbitrary player
		}
	#endif

		updatePlayerStatus(initialPlayerInList);
		Player* currentPlayerTest = initialPlayerInList;
		numberOfPlayersStillAlive = 0;	//get ready to count numberOfPlayersStillAlive
		while(currentPlayerTest->next != NULL)
		{
			if(currentPlayerTest->status == ALIVE)
			{
				numberOfPlayersStillAlive++;
			}
			currentPlayerTest = currentPlayerTest->next;
		}

		currentRound++;
	}

#ifdef USE_ANN
	//now for the player which won the game, train the shared neural network with all of it's unit's experiences [CHECKTHIS FUTURE: bias experiences for alive units?]
	//do not train network with unit experiences from teams that lost the game [CHECKTHIS FUTURE: bias experiences for units from loser teams?]

	//cout << "H3" << endl;

	//find alive player
	Player* currentPlayer = initialPlayerInList;
	while((currentPlayer->next->next != NULL) && (currentPlayer->status == false))
	{
		currentPlayer = currentPlayer->next;
	}

	//cout << "H3b currentPlayer->id = " << currentPlayer->id << endl;

	for(int nn=0; nn < GAME_NUMBER_OF_EXPERIENCE_NN_UTILISED; nn++)
	{
		bool addSprites;
		if((nn >= GAME_INDEX_OF_COMBAT_EXPERIENCE_NN*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS) && (nn < (GAME_INDEX_OF_COMBAT_EXPERIENCE_NN*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS+ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)))
		{
			addSprites = true;
		}
		else
		{
			addSprites = false;
		}

		trainAndOutputNeuralNetwork(currentPlayer->firstInputNeuronInNetwork[nn], currentPlayer->firstOutputNeuronInNetwork[nn], currentPlayer->numberOfInputNeurons[nn], currentPlayer->numberOfOutputNeurons[nn], currentPlayer->firstExperience[nn], addSprites, false, nn, currentGame);
	}

#endif

	return result;
}

#ifdef USE_ANN
void LRRCgameClass::feedNeuralNetworkWithGameUnitExperiences(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, const int64_t numberOfInputNeurons, const int64_t numberOfOutputNeurons, UnitListClass* firstUnitInUnitGroup, int nn)
{
	UnitListClass* currentUnitInList = firstUnitInUnitGroup;

	while(currentUnitInList->next != NULL)
	{
		#ifndef DEBUG_DO_NOT_PERFORM_KILL_RATIO_CHECKS_BEFORE_ADDING_UNIT_EXPERIENCES_TO_NN
		if(LRRCgameAI.checkAverageKillRatioForUnitGroup(currentUnitInList) || (currentUnitInList->unitDetails->numPerson == 1))
		{
		#endif
			ANNalgorithmBackpropagationTraining.feedNeuralNetworkWithASetOfExperiencesBackpropagation(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, currentUnitInList->firstExperience[nn]);

			if(currentUnitInList->isUnitGroup)
			{
				feedNeuralNetworkWithGameUnitExperiences(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, currentUnitInList->firstUnitInUnitGroup, nn);
			}
		#ifndef DEBUG_DO_NOT_PERFORM_KILL_RATIO_CHECKS_BEFORE_ADDING_UNIT_EXPERIENCES_TO_NN
		}
		#endif


		currentUnitInList = currentUnitInList->next;
	}
}
#endif


bool LRRCgameClass::executePhase(const int currentGame, const int currentRound, const int currentPlayerTurn, const int currentPhase, Player* initialPlayerInList, const int numberOfPlayers, LDreference* initialReferenceInThisPhaseStartSceneGlobal, const bool allPlayersAI)
{


	bool result = true;

	if(currentPhase == GAME_PHASE_MOVEMENT)
	{
		cout << "\tExecuting Phase Movement." << endl;
		if(!executeMovement(currentGame, currentRound, currentPlayerTurn, initialPlayerInList, numberOfPlayers, initialReferenceInThisPhaseStartSceneGlobal, allPlayersAI))
		{
			result = false;
		}
	}
	else if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
	{
		cout << "\tExecuting Phase Long Distance Combat." << endl;
		if(!executeLongDistanceCombat(currentGame, currentRound, currentPlayerTurn, initialPlayerInList, numberOfPlayers, initialReferenceInThisPhaseStartSceneGlobal, allPlayersAI))
		{
			result = false;
		}
	}
	else if(currentPhase == GAME_PHASE_CLOSECOMBAT)
	{
		cout << "\tExecuting Phase Close Combat." << endl;
		if(!executeCloseCombat(currentGame, currentRound, currentPlayerTurn, initialPlayerInList, numberOfPlayers, initialReferenceInThisPhaseStartSceneGlobal, allPlayersAI))
		{
			result = false;
		}
	}
	else
	{
		result = false;
		cout << "error: executePhase{} illegal phase " << endl;
	}

	return result;
}


bool LRRCgameClass::executeMovement(const int currentGame, const int currentRound, const int currentPlayerTurn, Player* initialPlayerInList, const int numberOfPlayers, LDreference* initialReferenceInThisPhaseStartSceneGlobal, const bool allPlayersAI)
{
	bool result = true;

	string preMovementPhaseSceneFileName = "";
	string thisPhaseStartSceneFileName = "";

	string targetSpritesSceneFileName = "";

	string nextSceneFileName = "";
	string rangeSpritesNextSceneFile = "";

	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_START, &preMovementPhaseSceneFileName);
	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_END, &thisPhaseStartSceneFileName);

	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_TARGETSPRITES, &targetSpritesSceneFileName);

	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_LONGDISTANCECOMBAT, GAME_PHASE_EXECUTION_DISPLAY_START, &nextSceneFileName);
	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_LONGDISTANCECOMBAT, GAME_PHASE_EXECUTION_DISPLAY_RANGESPRITES, &rangeSpritesNextSceneFile);


	Player* currentPlayer = LRRCplayerClass.findPlayer(initialPlayerInList, currentPlayerTurn);
	currentPlayer->currentPhase = GAME_PHASE_MOVEMENT;		//this is required for compareSceneFilesMovementPhase.


	if((currentPlayerTurn == 1) && (currentRound == 1))
	{
		/*this is now done before first game is initialted:

		//NB it is assumed in the first round, first player turn, no scene file with appropriate naming convention exists; copy scene.ldr to first game scene, eg sceneG1R1P1S1moveA.ldr
		string initialSceneFileInGame = SCENE_FILE_NAME_START + SCENE_FILE_NAME_EXTENSION;

		char* initialSceneFileInGameCharArray = new char[(initialSceneFileInGame).size()+1];
		const char* initialSceneFileInGameConstCharArray = (initialSceneFileInGame).c_str();
		strcpy(initialSceneFileInGameCharArray, initialSceneFileInGameConstCharArray);

		copyFiles(preMovementPhaseSceneFileName, initialSceneFileInGameCharArray);
		*/

		//NB it is assumed in the first round, first player turn, no scene file with movement range sprites exists and so one should be created...
		string rangeSpritesThisSceneFile = "";
		generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_RANGESPRITES, &rangeSpritesThisSceneFile);

		bool addTextualSpriteInfo = true;
		bool addRangeSpriteInfo = true;
		if(allPlayersAI)
		{
			if(!LRRCsprite.LRRCaddUnitDetailsSpritesToScene(preMovementPhaseSceneFileName, rangeSpritesThisSceneFile, addTextualSpriteInfo, addRangeSpriteInfo, GAME_PHASE_MOVEMENT, currentPlayerTurn, initialReferenceInThisPhaseStartSceneGlobal))
			{
				result = false;
			}
		}
		else
		{
			if(!LRRCsprite.LRRCaddUnitDetailsSpritesToSceneFile(preMovementPhaseSceneFileName, rangeSpritesThisSceneFile, addTextualSpriteInfo, addRangeSpriteInfo, GAME_PHASE_MOVEMENT, currentPlayerTurn))
			{
				result = false;
			}
		}

	}


	if(currentPlayer->playerIsAI == true)
	{
#ifdef USE_ANN

		LDreference* initialReferenceInPreMovementPhaseScene;
		if(allPlayersAI)
		{
			initialReferenceInPreMovementPhaseScene = initialReferenceInThisPhaseStartSceneGlobal;
		}
		else
		{
			//declare initial scene references
			initialReferenceInPreMovementPhaseScene = new LDreference();
			LDreference* topLevelReferenceInPreMovementPhaseScene = new LDreference(true);
			//parse the scene files and build LDreference linked lists
			if(!LDparser.parseFile(preMovementPhaseSceneFileName, initialReferenceInPreMovementPhaseScene, topLevelReferenceInPreMovementPhaseScene, false))
			{//file does not exist
				cout << "The file: " << preMovementPhaseSceneFileName << " does not exist in the directory" << endl;
				return false;
			}
			delete topLevelReferenceInPreMovementPhaseScene;
		}


		//initialise sprite list variables
		LDreference* targetSpriteListInitialReference = new LDreference();
		int numTargetSpritesAdded = 0;

		//initialPlayerInList->firstUnitInUnitList

		//not complete yet

		//searchReferenceListPrintReferenceDetails(initialReferenceInPreMovementPhaseScene);

		if(!AIsearchUnitListForPhaseActionSelectionInitialisation(currentRound, currentPlayerTurn, currentPhase, initialReferenceInPreMovementPhaseScene, preMovementPhaseSceneFileName, NULL, targetSpritesSceneFileName, targetSpriteListInitialReference, &numTargetSpritesAdded, initialPlayerInList, initialPlayerInList->firstUnitInUnitList, initialPlayerInList->firstUnitInUnitList))
		{
			result = false;
		}

		//searchReferenceListPrintReferenceDetails(initialReferenceInPreMovementPhaseScene);

		if(allPlayersAI)
		{
			//write reference list to file.
			if(!LDreferenceManipulation.write2DreferenceListCollapsedTo1DtoFile(thisPhaseStartSceneFileName, initialReferenceInPreMovementPhaseScene))
			{
				result = false;
			}
		}
		else
		{
			//write reference list to file.
			if(!LDreferenceManipulation.writeReferencesToFile(thisPhaseStartSceneFileName, initialReferenceInPreMovementPhaseScene))
			{
				result = false;
			}
		}

		bool addSprites = true;
		if(addSprites)
		{
			if(!LDreferenceManipulation.addSpriteReferenceListToSceneFile(preMovementPhaseSceneFileName, targetSpritesSceneFileName, targetSpriteListInitialReference, numTargetSpritesAdded))
			{
				result = false;
			}
		}


		int nextPhase = GAME_PHASE_LONGDISTANCECOMBAT;
		int nextPhasePlayerTurn = currentPlayerTurn;


		if(!prepareNextPhaseSceneFiles(nextPhase, nextPhasePlayerTurn, thisPhaseStartSceneFileName, nextSceneFileName, rangeSpritesNextSceneFile, initialReferenceInPreMovementPhaseScene, allPlayersAI))
		{
			result = false;
		}

		//clean up memory

		delete targetSpriteListInitialReference;

		if(!allPlayersAI)
		{
			delete initialReferenceInPreMovementPhaseScene;
		}
	#else
		cout << "error: no ANN - no AI players allowed" << endl;
	#endif

	}
	else
	{

		//declare initial scene references
		LDreference* initialReferenceInPostMovementPhaseScene = new LDreference();
		LDreference* initialReferenceInPreMovementPhaseScene = new LDreference();
		LDreference* topLevelReferenceInPostMovementPhaseScene = new LDreference(true);
		LDreference* topLevelReferenceInPreMovementPhaseScene = new LDreference(true);
		//parse the scene files and build LDreference linked lists
		if(!LDparser.parseFile(thisPhaseStartSceneFileName, initialReferenceInPostMovementPhaseScene, topLevelReferenceInPostMovementPhaseScene, false))
		{//file does not exist
			cout << "The file: " << thisPhaseStartSceneFileName << " does not exist in the directory" << endl;
			return false;
		}
		if(!LDparser.parseFile(preMovementPhaseSceneFileName, initialReferenceInPreMovementPhaseScene, topLevelReferenceInPreMovementPhaseScene, false))
		{//file does not exist
			cout << "The file: " << preMovementPhaseSceneFileName << " does not exist in the directory" << endl;
			return false;
		}


		//assumes preMovementPhaseSceneFileName and thisPhaseStartSceneFileName already exist
		if(!LRRCmovement.compareScenesMovementPhase(preMovementPhaseSceneFileName, initialReferenceInPreMovementPhaseScene, initialReferenceInPostMovementPhaseScene, currentPlayer, targetSpritesSceneFileName, true))
		{
			result = false;
		}

		int nextPhase = GAME_PHASE_LONGDISTANCECOMBAT;
		int nextPhasePlayerTurn = currentPlayerTurn;
		if(!prepareNextPhaseSceneFiles(nextPhase, nextPhasePlayerTurn, thisPhaseStartSceneFileName, nextSceneFileName, rangeSpritesNextSceneFile, initialReferenceInPreMovementPhaseScene, allPlayersAI))
		{
			result = false;
		}

		delete initialReferenceInPostMovementPhaseScene;
		delete initialReferenceInPreMovementPhaseScene;
		delete topLevelReferenceInPostMovementPhaseScene;
		delete topLevelReferenceInPreMovementPhaseScene;
	}



	//....*****************************************************************************************************************************
	//... CHECKTHIS (fill in rest here)
	//adds sprites to scene file indicating unit movements
	//addSpritesToSceneFileWithPhaseResults(preMovementPhaseSceneFileName, thisPhaseStartSceneFileName, thisPhaseStartSceneFileName, SPRITES_MOVEMENT_SPECIFIC);
		//NB remove all current sprites, and these new sprites should indicate unit IDs and player movement markers

			//add sprites relevant for current
			//add sprites relevant for next phase

	return result;
}


bool LRRCgameClass::executeLongDistanceCombat(const int currentGame, const int currentRound, const int currentPlayerTurn, Player* initialPlayerInList, const int numberOfPlayers, LDreference* initialReferenceInThisPhaseStartSceneGlobal, const bool allPlayersAI)
{
	bool result = true;

	string preMovementPhaseSceneFileNameMovement = "";
	string thisPhaseStartSceneFileNameMovement = "";

	string preCombatPhaseSceneFileName = "";
	string postCombatPhaseSceneFileName = "";

	string targetSpritesSceneFileName = "";

	string nextSceneFileName = "";
	string rangeSpritesNextSceneFile = "";


	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_START, &preMovementPhaseSceneFileNameMovement);
	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_END, &thisPhaseStartSceneFileNameMovement);

	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_LONGDISTANCECOMBAT, GAME_PHASE_EXECUTION_DISPLAY_START, &preCombatPhaseSceneFileName);
	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_LONGDISTANCECOMBAT, GAME_PHASE_EXECUTION_DISPLAY_END, &postCombatPhaseSceneFileName);

	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_LONGDISTANCECOMBAT, GAME_PHASE_EXECUTION_DISPLAY_TARGETSPRITES, &targetSpritesSceneFileName);

	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_CLOSECOMBAT, GAME_PHASE_EXECUTION_DISPLAY_START, &nextSceneFileName);
	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_CLOSECOMBAT, GAME_PHASE_EXECUTION_DISPLAY_RANGESPRITES, &rangeSpritesNextSceneFile);

	LDreference* initialReferenceInThisPhaseStartScene;
	LDreference* initialReferenceInPreMovementPhaseScene;
	if(allPlayersAI)
	{
		initialReferenceInThisPhaseStartScene = initialReferenceInThisPhaseStartSceneGlobal;
		initialReferenceInPreMovementPhaseScene = NULL;;
	}
	else
	{
		//declare initial scene references
		initialReferenceInThisPhaseStartScene = new LDreference();
		initialReferenceInPreMovementPhaseScene = new LDreference();
		LDreference* topLevelReferenceInThisPhaseStartScene = new LDreference(true);
		LDreference* topLevelReferenceInPreMovementPhaseScene = new LDreference(true);
		//parse the scene files and build LDreference linked lists
		if(!LDparser.parseFile(thisPhaseStartSceneFileNameMovement, initialReferenceInThisPhaseStartScene, topLevelReferenceInThisPhaseStartScene, false))
		{//file does not exist
			cout << "The file: " << thisPhaseStartSceneFileNameMovement << " does not exist in the directory" << endl;
			return false;
		}
		if(!LDparser.parseFile(preMovementPhaseSceneFileNameMovement, initialReferenceInPreMovementPhaseScene, topLevelReferenceInPreMovementPhaseScene, false))
		{//file does not exist
			cout << "The file: " << preMovementPhaseSceneFileNameMovement << " does not exist in the directory" << endl;
			return false;
		}
		delete topLevelReferenceInThisPhaseStartScene;
		delete topLevelReferenceInPreMovementPhaseScene;
	}


	int nextPhase = GAME_PHASE_CLOSECOMBAT;
	int nextPhasePlayerTurn = currentPlayerTurn;
	if(!executeGenericCombat(currentRound, currentPlayerTurn, GAME_PHASE_LONGDISTANCECOMBAT,  initialReferenceInPreMovementPhaseScene, initialReferenceInThisPhaseStartScene, preCombatPhaseSceneFileName, postCombatPhaseSceneFileName, targetSpritesSceneFileName, initialPlayerInList, allPlayersAI))
	{
		result = false;
	}
	if(!prepareNextPhaseSceneFiles(nextPhase, nextPhasePlayerTurn, postCombatPhaseSceneFileName, nextSceneFileName, rangeSpritesNextSceneFile, initialReferenceInThisPhaseStartScene, allPlayersAI))
	{
		result = false;
	}

	if(!allPlayersAI)
	{
		delete initialReferenceInThisPhaseStartScene;
		delete initialReferenceInPreMovementPhaseScene;
	}

	return result;
}

bool LRRCgameClass::executeCloseCombat(const int currentGame, const int currentRound, const int currentPlayerTurn, Player* initialPlayerInList, const int numberOfPlayers, LDreference* initialReferenceInThisPhaseStartSceneGlobal, const bool allPlayersAI)
{
	bool result = true;

	string preMovementPhaseSceneFileNameMovement = "";
	string thisPhaseStartSceneFileNameMovement = "";

	string preCombatPhaseSceneFileName = "";
	string postCombatPhaseSceneFileName = "";

	string targetSpritesSceneFileName = "";

	string nextSceneFileName = "";
	string rangeSpritesNextSceneFile = "";

	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_START, &preMovementPhaseSceneFileNameMovement);
	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_END, &thisPhaseStartSceneFileNameMovement);

	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_CLOSECOMBAT, GAME_PHASE_EXECUTION_DISPLAY_START, &preCombatPhaseSceneFileName);
	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_CLOSECOMBAT, GAME_PHASE_EXECUTION_DISPLAY_END, &postCombatPhaseSceneFileName);

	generateSceneFileName(currentGame, currentRound, currentPlayerTurn, GAME_PHASE_CLOSECOMBAT, GAME_PHASE_EXECUTION_DISPLAY_TARGETSPRITES, &targetSpritesSceneFileName);


	LDreference* initialReferenceInThisPhaseStartScene;
	LDreference* initialReferenceInPreMovementPhaseScene;
	if(allPlayersAI)
	{
		initialReferenceInThisPhaseStartScene = initialReferenceInThisPhaseStartSceneGlobal;
		initialReferenceInPreMovementPhaseScene = NULL;;
	}
	else
	{
		//declare initial scene references
		initialReferenceInThisPhaseStartScene = new LDreference();
		initialReferenceInPreMovementPhaseScene = new LDreference();
		LDreference* topLevelReferenceInThisPhaseStartScene = new LDreference(true);
		LDreference* topLevelReferenceInPreMovementPhaseScene = new LDreference(true);
		//parse the scene files and build LDreference linked lists
		if(!LDparser.parseFile(thisPhaseStartSceneFileNameMovement, initialReferenceInThisPhaseStartScene, topLevelReferenceInThisPhaseStartScene, false))
		{//file does not exist
			cout << "The file: " << thisPhaseStartSceneFileNameMovement << " does not exist in the directory" << endl;
			return false;
		}
		if(!LDparser.parseFile(preMovementPhaseSceneFileNameMovement, initialReferenceInPreMovementPhaseScene, topLevelReferenceInPreMovementPhaseScene, false))
		{//file does not exist
			cout << "The file: " << preMovementPhaseSceneFileNameMovement << " does not exist in the directory" << endl;
			return false;
		}
		delete topLevelReferenceInThisPhaseStartScene;
		delete topLevelReferenceInPreMovementPhaseScene;
	}


	int nextPhase = GAME_PHASE_MOVEMENT;
	int nextPhasePlayerTurn;
	if(currentPlayerTurn == numberOfPlayers)
	{
		generateSceneFileName(currentGame, (currentRound+1), GAME_PLAYER_TURN_DEFAULT, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_START, &nextSceneFileName);
		generateSceneFileName(currentGame, (currentRound+1), GAME_PLAYER_TURN_DEFAULT, GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_RANGESPRITES, &rangeSpritesNextSceneFile);
		nextPhasePlayerTurn = GAME_PLAYER_TURN_DEFAULT;
	}
	else
	{
		generateSceneFileName(currentGame, currentRound, (currentPlayerTurn+1), GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_START, &nextSceneFileName);
		generateSceneFileName(currentGame, currentRound, (currentPlayerTurn+1), GAME_PHASE_MOVEMENT, GAME_PHASE_EXECUTION_DISPLAY_RANGESPRITES, &rangeSpritesNextSceneFile);
		nextPhasePlayerTurn = (currentPlayerTurn+1);
	}

	if(!executeGenericCombat(currentRound, currentPlayerTurn, GAME_PHASE_CLOSECOMBAT, initialReferenceInPreMovementPhaseScene, initialReferenceInThisPhaseStartScene, preCombatPhaseSceneFileName, postCombatPhaseSceneFileName, targetSpritesSceneFileName, initialPlayerInList, allPlayersAI))
	{
		result = false;
	}
	if(!prepareNextPhaseSceneFiles(nextPhase, nextPhasePlayerTurn, postCombatPhaseSceneFileName, nextSceneFileName, rangeSpritesNextSceneFile, initialReferenceInThisPhaseStartScene, allPlayersAI))
	{
		result = false;
	}

	if(!allPlayersAI)
	{
		delete initialReferenceInThisPhaseStartScene;
		delete initialReferenceInPreMovementPhaseScene;

	}

	return result;
}


bool LRRCgameClass::prepareNextPhaseSceneFiles(const int nextPhase, const int nextPlayerTurn, string previousPhaseSceneFileName, const string nextSceneFileName, string rangeSpritesNextSceneFileName, LDreference* firstReferenceInPreviousScene, const bool allPlayersAI)
{
	bool result = true;

	//create next scene file
	SHAREDvars.copyFiles(nextSceneFileName, previousPhaseSceneFileName);

	/* METHOD2;*/
	bool addTextualSpriteInfo = true;
	bool addRangeSpriteInfo = true;
	if(allPlayersAI)
	{
		if(!LRRCsprite.LRRCaddUnitDetailsSpritesToScene(previousPhaseSceneFileName, rangeSpritesNextSceneFileName, addTextualSpriteInfo, addRangeSpriteInfo, nextPhase, nextPlayerTurn, firstReferenceInPreviousScene))
		{
			result = false;
		}
	}
	else
	{
		if(!LRRCsprite.LRRCaddUnitDetailsSpritesToSceneFile(previousPhaseSceneFileName, rangeSpritesNextSceneFileName, addTextualSpriteInfo, addRangeSpriteInfo, nextPhase, nextPlayerTurn))
		{
			result = false;
		}
	}

	return result;
}



bool LRRCgameClass::executeGenericCombat(const int currentRound, const int currentPlayerTurn, int currentPhase, LDreference* initialReferenceInPreMovementPhaseScene, LDreference* initialReferenceInThisPhaseStartScene, const string preCombatPhaseSceneFileName, const string postCombatPhaseSceneFileName, string targetSpritesSceneFileName, Player* initialPlayerInList, const bool allPlayersAI)
{
	bool result = true;

	/*
	LDreference* initialReferenceInSceneFile = new LDreference();
	LDreference* topLevelReferenceInSceneFile = new LDreference(preCombatPhaseSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
	if(!parseFile(preCombatPhaseSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, false))
	{//file does not exist
		cout << "The file: " << preCombatPhaseSceneFileName << " does not exist in the directory" << endl;
		result = false;
	}
	*/


	//assumes only preMovementPhaseSceneFileName exists. thisPhaseStartSceneFileName will be created by PC after PC computes combat outcomes
	//....
	//... CHECKTHIS (fill in rest here)

	bool UIstatus = true;
	string answerAsString = "";
	int answerAsInt;
	string unitAttackerFileName = "";
	string unitDefenderFileName = "";
	int unitAttackerPlayerID;
	int unitDefenderPlayerID;

	//initialise sprite list variables
	LDreference* targetSpriteListInitialReference = new LDreference();
	int numTargetSpritesAdded = 0;

	Player* currentPlayer = LRRCplayerClass.findPlayer(initialPlayerInList, currentPlayerTurn);
	if(currentPlayer->playerIsAI == true)
	{
		//not complete yet
		#ifdef USE_ANN
		if(!AIsearchUnitListForPhaseActionSelectionInitialisation(currentRound, currentPlayerTurn, currentPhase, initialReferenceInThisPhaseStartScene, preCombatPhaseSceneFileName, postCombatPhaseSceneFileName, targetSpritesSceneFileName, targetSpriteListInitialReference, &numTargetSpritesAdded, initialPlayerInList, initialPlayerInList->firstUnitInUnitList, initialPlayerInList->firstUnitInUnitList))
		{
			result = false;
		}
		#else
		cout << "error: no ANN - no AI players allowed" << endl;
		#endif
	}
	else
	{
		while(UIstatus == true)
		{
			bool unitIDLocationSuccess = true;

			if(currentPhase == GAME_PHASE_CLOSECOMBAT)
			{
				cout << "Do you wish to perform close combat with a (new) unit? (y/n) \n\n>> ";
			}
			else if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
			{
				cout << "Do you wish to perform long distance combat with a (new) unit? (y/n) \n\n>> ";
			}
			else
			{
				result = false;
				cout << "error: executeGenericCombat{} illegal phase " << endl;
			}

			cin >> answerAsString;

			if(answerAsString[0] == 'n')
			{
				UIstatus = false;
			}

			if(UIstatus == true)
			{
				LRRCgameReferenceManipulation.obtainUnitDetailsFromUserForCombat(&unitAttackerFileName, &unitDefenderFileName, &unitAttackerPlayerID, &unitDefenderPlayerID, initialReferenceInThisPhaseStartScene);


				UnitListClass* unit;
				bool unitIDFound = false;
				unit = LRRCunitClass.searchUnitListFindUnit(initialPlayerInList->firstUnitInUnitList, unitAttackerFileName, unitAttackerPlayerID, &unitIDFound);
				if(!unitIDFound)
				{
					cout << "Error: unit " << unitAttackerFileName << " of the player " << currentPlayerTurn << " could not be found in unit list" << endl;
					result = false;
					unitIDLocationSuccess = false;
				}
				if(unit->hasPerformedAction[currentPhase])
				{
					unitIDLocationSuccess = false;
					cout << "unit " << unitAttackerFileName << " of the player " << currentPlayerTurn << " could not attack unit " << unitDefenderFileName << ". Unit has already performed action. Please retry with another unit" << endl;
				}

				/*
				#ifdef GAME_USE_COLOUR_AS_UNIQUE_UNIT_IDS

				if(!determineUnitNamesWithColours(currentPhase, unitAttackerFileName, unitDefenderFileName, &unitAttackerPlayerID, &unitDefenderPlayerID, initialReferenceInSceneFile))
				{
					cout << "unit " << unitAttackerFileName << " of the player " << currentPlayerTurn << " could not attack unit " << unitDefenderFileName << ". Please retry with another target" << endl;
					unitIDLocationSuccess = false;
				}

				#else

				if(!determineIfUnitsExists(currentPhase, unitAttackerFileName, unitDefenderFileName, unitAttackerPlayerID, unitDefenderPlayerID, initialReferenceInThisPhaseStartScene))
				{
					cout << "unit " << unitAttackerFileName << " of the player " << currentPlayerTurn << " could not attack unit " << unitDefenderFileName << ". Please retry with another target" << endl;
					unitIDLocationSuccess = false;
				}

				#endif
				*/

				if(unitAttackerPlayerID != currentPlayerTurn)
				{
					cout << "invalid answer - only units owned by the current Player Turn Player can initiate an attack" << endl;
					unitIDLocationSuccess = false;
				}


				if(unitIDLocationSuccess == true)
				{
					performGenericCombatWithTwoCombatReadyUnitsAndAddSprites(currentRound, currentPlayerTurn, currentPhase, initialReferenceInPreMovementPhaseScene, initialReferenceInThisPhaseStartScene, targetSpritesSceneFileName, unitAttackerFileName, unitDefenderFileName, unitAttackerPlayerID, unitDefenderPlayerID, targetSpriteListInitialReference, &numTargetSpritesAdded, initialPlayerInList, true);
				}
			}
		}
	}

	//write reference list to file.
	if(allPlayersAI)
	{
		if(!LDreferenceManipulation.write2DreferenceListCollapsedTo1DtoFile(postCombatPhaseSceneFileName, initialReferenceInThisPhaseStartScene))
		{
			result = false;
		}
	}
	else
	{//1D LDreference list required - no unit groups
		if(!LDreferenceManipulation.writeReferencesToFile(postCombatPhaseSceneFileName, initialReferenceInThisPhaseStartScene))
		{
			result = false;
		}
	}

	//add target sprites to target sprites scene file
	if(!LDreferenceManipulation.addSpriteReferenceListToSceneFile(preCombatPhaseSceneFileName, targetSpritesSceneFileName, targetSpriteListInitialReference, numTargetSpritesAdded))
	{
		result = false;
	}

	delete targetSpriteListInitialReference;

	return result;
}



#ifdef USE_ANN

	#define DOUBLE_COMPARISON_CORRECTION (0.00001)



bool LRRCgameClass::updateAbsolutePositionOfAllSubModels(LDreference* firstReferenceWithinSubModel, LDreference* parentReference)
{
	bool result = true;

	LDreference* currentReference = firstReferenceWithinSubModel;

	while(currentReference->next != NULL)
	{
		SHAREDvector.multiplyMatricies(&(currentReference->absoluteDeformationMatrix), &(parentReference->absoluteDeformationMatrix), &(currentReference->deformationMatrix));

		currentReference->absolutePosition.x = LDparser.calcModXPosBasedUponRotate(&(currentReference->relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.x;
		currentReference->absolutePosition.y = LDparser.calcModYPosBasedUponRotate(&(currentReference->relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.y;
		currentReference->absolutePosition.z = LDparser.calcModZPosBasedUponRotate(&(currentReference->relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.z;

		if(currentReference->isSubModelReference)
		{
			if(!updateAbsolutePositionOfAllSubModels(currentReference->firstReferenceWithinSubModel, currentReference))
			{
				result = false;
			}
		}

		currentReference = currentReference->next;
	}

	return result;
}


bool LRRCgameClass::moveUnitTowardsOpponent(LDreference* unitReference, LDreference* opponentReference)
{//move to within long distance boundary, else move as close as possible to unit

	//cout << "EXECUTING: moveUnitTowardsOpponent{}" << endl;

	bool result = true;

	vec vectorBetweenUnitAndOpponent;
	SHAREDvector.subtractVectors(&vectorBetweenUnitAndOpponent, &(opponentReference->absolutePosition), &(unitReference->absolutePosition));

	double initialDistanceBetweenUnits = LRRCcombat.calculateTheDistanceBetweenTwoUnits(&(unitReference->absolutePosition), &(opponentReference->absolutePosition));

	double unitMaximumMoveDistance = unitReference->subModelDetails->movementSpeed;

	//new hard method - extract from combat.cpp;

	double closestDistanceToOpponentAfterMovementTest;
	double unit1MinimumRangeOfFire = CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY;
	if(unitMaximumMoveDistance > (initialDistanceBetweenUnits-unit1MinimumRangeOfFire))
	{
		closestDistanceToOpponentAfterMovementTest = unit1MinimumRangeOfFire;	//+DOUBLE_COMPARISON_CORRECTION
	}
	else
	{
		closestDistanceToOpponentAfterMovementTest = initialDistanceBetweenUnits-unitMaximumMoveDistance;
	}


	bool unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack = true;
	bool unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack = false;
	bool unit1CanPerformLongDistanceAttack = false;
	bool unit2CanPerformLongDistanceAttack = false;

	LRRCcombat.calculateLongDistanceAttackBonus(unitReference, opponentReference, unit1HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, unit2HasNotMovedInPreviousRoundAndIntendsToPerformLongDistanceAttack, &unit1CanPerformLongDistanceAttack, &unit2CanPerformLongDistanceAttack, closestDistanceToOpponentAfterMovementTest);

	bool foundOptimumLongDistanceWeapon = false;
	if(unit1CanPerformLongDistanceAttack)
	{
		foundOptimumLongDistanceWeapon = true;
	}
	
	/*OLD [before code generalisation];

	double unit1RangeModifier;

	unit1RangeModifier = calculateLongDistanceRangeModifier(opponentReference, unitReference);
	//longRangeDistanceBetweenTheTwoUnits = LRRCcombat.calculateTheDistanceBetweenTwoUnits(&(unitReference->absolutePosition), &(opponentReference->absolutePosition));

	XMLrulesClass* currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
	RecordClass* currentReferenceRecordClass = unitReference->subModelDetails->recordOfUnitCombatDetailsAttackLongDistance;
	bool foundOptimumLongDistanceWeapon = false;

	//cout << "here6b4b1a4" << endl;

	while((currentReferenceRecordClass->next != NULL) && (!foundOptimumLongDistanceWeapon))
	{
		if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
		{
			int currentWeaponsRange = currentReferenceRulesClass->attribute5;
			double currentWeaponMaximumRangeOfFireWithModifier = currentWeaponsRange + unit1RangeModifier;

			if(calculateIfUnitIsWithinAGivenLongRangeAttackDistance(closestDistanceToOpponentAfterMovementTest, (int)currentWeaponMaximumRangeOfFireWithModifier, (int)unit1MinimumRangeOfFire))
			{
				foundOptimumLongDistanceWeapon = true;

				if(currentReferenceRulesClass->name == CATAPULT_BUCKET_NAME)
				{
					unitReference->subModelDetails->longDistanceAttackValue = currentReferenceRulesClass->attribute4* currentReferenceRecordClass->numberOfThisPartIDInTheUnit;
				}
				else
				{
					unitReference->subModelDetails->longDistanceAttackValue = currentReferenceRulesClass->attribute4;
				}
				unitReference->subModelDetails->longDistanceAttackBaseRange = currentReferenceRulesClass->attribute5;
				unitReference->subModelDetails->hasLongDistanceCombatWeapon = true;		//not necessary, already filled in
			}
		}

		currentReferenceRecordClass = currentReferenceRecordClass->next;
		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}
	*/

	double longDistanceAttackBaseRange = unitReference->subModelDetails->longDistanceAttackBaseRange ;


	if(foundOptimumLongDistanceWeapon)
	{
		//position unit within firing range of the unit's best possible long range weapon
		SHAREDvector.multiplyVectorByScalar(&vectorBetweenUnitAndOpponent, (initialDistanceBetweenUnits-(longDistanceAttackBaseRange-DOUBLE_COMPARISON_CORRECTION))/initialDistanceBetweenUnits);
		SHAREDvector.addVectors(&(unitReference->relativePosition), &(unitReference->relativePosition), &vectorBetweenUnitAndOpponent);	//CHECK THIS: I am not sure if relative positions changes based upon movements are been calcualted correctly at this time...
		SHAREDvector.addVectors(&(unitReference->absolutePosition), &(unitReference->absolutePosition), &vectorBetweenUnitAndOpponent);

	}
	else
	{//unit does not appear to have an appropriate long range weapon

		if(initialDistanceBetweenUnits < (unitMaximumMoveDistance-CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY))
		{
			//position unit within the close combat boundary of the opponent
			SHAREDvector.multiplyVectorByScalar(&vectorBetweenUnitAndOpponent, (initialDistanceBetweenUnits-(CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY-DOUBLE_COMPARISON_CORRECTION))/initialDistanceBetweenUnits);
			SHAREDvector.addVectors(&(unitReference->relativePosition), &(unitReference->relativePosition), &(vectorBetweenUnitAndOpponent));	//CHECK THIS: I am not sure if relative positions changes based upon movements are been calcualted correctly at this time...
			SHAREDvector.addVectors(&(unitReference->absolutePosition), &(unitReference->absolutePosition), &(vectorBetweenUnitAndOpponent));
		}
		else
		{
			//position unit as close as possible to the opponent
			SHAREDvector.multiplyVectorByScalar(&vectorBetweenUnitAndOpponent, unitMaximumMoveDistance/initialDistanceBetweenUnits);
			SHAREDvector.addVectors(&(unitReference->relativePosition), &(unitReference->relativePosition), &vectorBetweenUnitAndOpponent);	//CHECK THIS: I am not sure if relative positions changes based upon movements are been calcualted correctly at this time...
			SHAREDvector.addVectors(&(unitReference->absolutePosition), &(unitReference->absolutePosition), &vectorBetweenUnitAndOpponent);
		}
	}

	//not finished yet...


	return result;
}



//required for AI player
	//cycle through each character the player owns on the board, and determine  and
		//make sure to check that a unit selected is combat ready, if a unit is selected and performs combat make it no longer combat ready
bool LRRCgameClass::AIsearchUnitListForPhaseActionSelectionInitialisation(const int currentRound, const int currentPlayerTurn, int currentPhase, LDreference* initialReferenceInThisPhaseStartScene, const string preCombatPhaseSceneFileName, const string postCombatPhaseSceneFileName, string targetSpritesSceneFileName, LDreference* targetSpriteListInitialReference, int* numTargetSpritesAdded, Player* initialPlayerInList, UnitListClass* firstUnitInUnitGroup, UnitListClass* firstUnitInOpponentUnitGroup)
{
	bool findOpponentsForAllUnitsWithinUnitGroupToTarget = true;		//assume true so far

	bool result = true;


	/*
	alternatively, can use a player's unit list to perform this search, ie;
	*/

	Player* thisPlayer = LRRCplayerClass.findPlayer(initialPlayerInList, currentPlayerTurn);
	UnitListClass* thisPlayerCurrentUnit = firstUnitInUnitGroup;

	while(thisPlayerCurrentUnit->next != NULL)
	{
		bool unitAllowedToPerformActionThisPhase = true;
		if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
		{
			if(thisPlayerCurrentUnit->hasPerformedAction[GAME_PHASE_MOVEMENT])
			{//rules currently do not allow a unit to both move and perform long distance attack
				unitAllowedToPerformActionThisPhase = false;
			}
		}
		if(thisPlayerCurrentUnit->status == false)
		{
			//do not perform action if unit is dead
			unitAllowedToPerformActionThisPhase = false;
		}

		if((thisPlayerCurrentUnit->team == currentPlayerTurn) && (thisPlayerCurrentUnit->status == ALIVE) && (unitAllowedToPerformActionThisPhase))
		{
			//1. create a dummy reference for details calculations
				//this may not be required .... but i currently assume determiniation of unit details is required for movement a unit towards an opponent [so eg that the maximum movement distance can be determined]

			LDreference* referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent = new LDreference(true);
			referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->colour = thisPlayerCurrentUnit->team;
			referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->name = thisPlayerCurrentUnit->name;
			if(!LRRCcombat.searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent, initialReferenceInThisPhaseStartScene, NULL, false))
			{
				cout << "error: cannot find player unit reference in scene file, name = " << referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->name << " id=" << referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->colour << endl;
				result = false;
			}
			LRRCcombat.performFinalUnitClassCalculations(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->subModelDetails);

			//2. create a reference to the reference in the scene file
			bool thisPlayerUnitIDFound = false;
			bool thisPlayerUnitFoundResult = true;
			LDreference* referenceForThisPlayersCurrentUnitThatIsFindingAnOpponentInScene;
			referenceForThisPlayersCurrentUnitThatIsFindingAnOpponentInScene = LRRCgameReferenceManipulation.searchReferenceListFindReference(initialReferenceInThisPhaseStartScene, thisPlayerCurrentUnit->name, thisPlayerCurrentUnit->team, &thisPlayerUnitIDFound, &thisPlayerUnitFoundResult);
			if(!thisPlayerUnitIDFound || !thisPlayerUnitFoundResult)
			{
				cout << "error: cannot find player unit reference in scene file, name = " << thisPlayerCurrentUnit->name << " id=" << thisPlayerCurrentUnit->team << endl;
				result = false;
			}

			//cout << "firstUnitInOpponentUnitGroup->name = " << firstUnitInOpponentUnitGroup->name << endl;
			if(!AIsearchUnitListAndCalculateWorthOfOpponents(currentPlayerTurn, currentPhase, initialReferenceInThisPhaseStartScene, initialPlayerInList, firstUnitInOpponentUnitGroup, thisPlayerCurrentUnit, referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent))		//firstUnitInOpponentUnitGroup: first unit in an arbitrary player list, as there is only 1 unit list
			{
				result = false;
			}


			bool performedActionYet = false;
			bool stillPossibleOpponentsToTryFighting = true;
			while(!performedActionYet && stillPossibleOpponentsToTryFighting)
			{
				bool performActionWithThisOpponentThisPhase = false;

				UnitListClass* opposingPlayerCurrentUnit;

				bool foundOpponent = false;

				double currentLowestError = MAX_ANN_BACK_PROPAGATION_ERROR;
				/*
			#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
				int NNcurrentPhase;
				if(currentPhase == GAME_PHASE_MOVEMENT)
				{
					NNcurrentPhase = ANN_SEPARATE_CC_FROM_LD_NETWORK_COMBAT_SCENARIO_USED_FOR_MOVEMENT_PHASE;
				}
				else
				{
					NNcurrentPhase = currentPhase;
				}
			#else
				#define NOT_USED (0)
				int NNcurrentPhase = NOT_USED;
			#endif
				*/
			#ifdef ANN_USE_MOVEMENT_EXPERIENCES
				int NNcurrentPhase = currentPhase;
			#else
				int NNcurrentPhase = currentPhase;
			#endif

				opposingPlayerCurrentUnit = LRRCunitClass.searchUnitListFindOpponentWithLowestError(currentPlayerTurn, GAME_INDEX_OF_ALL_EXPERIENCES_NN, firstUnitInOpponentUnitGroup, &currentLowestError, &foundOpponent, NNcurrentPhase);

				if(!foundOpponent)
				{
					//cout << "did not find opponent" << endl;
					stillPossibleOpponentsToTryFighting = false;
					performActionWithThisOpponentThisPhase = false;
					findOpponentsForAllUnitsWithinUnitGroupToTarget = false;
				}
				else
				{
					performActionWithThisOpponentThisPhase = true;
				}



				//now verify that unit should be chosen for combat this phase;

				if(performActionWithThisOpponentThisPhase)
				{
					LDreference* referenceForOpposingPlayerCurrentUnit = new LDreference(true);

					referenceForOpposingPlayerCurrentUnit->colour = opposingPlayerCurrentUnit->team;
					referenceForOpposingPlayerCurrentUnit->name = opposingPlayerCurrentUnit->name;

					if(!LRRCcombat.searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(referenceForOpposingPlayerCurrentUnit, initialReferenceInThisPhaseStartScene, NULL, false))
					{
						cout << "error: cannot find opposing player unit reference in scene file, name = " << referenceForOpposingPlayerCurrentUnit->name << " id=" << referenceForOpposingPlayerCurrentUnit->colour << endl;

						//searchReferenceListPrintReferenceDetails(initialReferenceInThisPhaseStartScene);

						result = false;
					}
					LRRCcombat.performFinalUnitClassCalculations(referenceForOpposingPlayerCurrentUnit->subModelDetails);

					double randomNumberBetween0And1;
					#ifdef DEBUG_NO_CREATIVITY_ALWAYS_PERFORM_COMBAT_WITH_LOWEST_ERROR_UNIT
					randomNumberBetween0And1 = 1.0;
					#else
					randomNumberBetween0And1 = ((double(abs((short)rand())))/(ABS_OF_SHORT));
					#endif

					if(randomNumberBetween0And1 > thisPlayer->creativity)
					{//if creative, do something different...

						if(thisPlayerCurrentUnit->isUnitGroup)
						{
							UnitListClass* firstUnitInOpposingPlayerUnitGroupForPlayerUnitGroupToTarget;
							if(opposingPlayerCurrentUnit->isUnitGroup)
							{
								#ifdef DEBUG_NO_UNITGROUP_AI
								firstUnitInOpposingPlayerUnitGroupForPlayerUnitGroupToTarget = firstUnitInOpponentUnitGroup;
								#else
								firstUnitInOpposingPlayerUnitGroupForPlayerUnitGroupToTarget = opposingPlayerCurrentUnit->firstUnitInUnitGroup;
								#endif
							}
							else
							{
								firstUnitInOpposingPlayerUnitGroupForPlayerUnitGroupToTarget = firstUnitInOpponentUnitGroup;
							}

							if(!AIsearchUnitListForPhaseActionSelectionInitialisation(currentRound, currentPlayerTurn, currentPhase, initialReferenceInThisPhaseStartScene, preCombatPhaseSceneFileName, postCombatPhaseSceneFileName, targetSpritesSceneFileName, targetSpriteListInitialReference, numTargetSpritesAdded, initialPlayerInList, thisPlayerCurrentUnit->firstUnitInUnitGroup, firstUnitInOpposingPlayerUnitGroupForPlayerUnitGroupToTarget))
							{
								opposingPlayerCurrentUnit->selfLearningTempVarUnitHasBeenTried = true;
							}
							else
							{
								thisPlayerCurrentUnit->hasPerformedAction[currentPhase] = true;
								performedActionYet = true;
								opposingPlayerCurrentUnit->selfLearningTempVarUnitHasBeenTried = true;	//this is redundant
							}

							//add experience information for unit group [assuming average kill:death ratio of units in group is > 1.0 {and unit group is still alive???}]
							if(!LRRCgameAI.determineIfUnitGroupHasAliveUnits(thisPlayerCurrentUnit->firstUnitInUnitGroup))
							{
								thisPlayerCurrentUnit->status = false;
							}

							//if(checkAverageKillRatioForUnitGroup(thisPlayerCurrentUnit->firstUnitInUnitGroup))
							//{
							int64_t unitTheoreticalBestDecisionToRecord;
							unitTheoreticalBestDecisionToRecord = currentPhase;

					#ifndef DEBUG_DO_NOT_USE_ADVANCED_LEARNING_BASED_UPON_DISTANCES
							/*
						#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
							int NNcurrentPhase;
							if(currentPhase == GAME_PHASE_MOVEMENT)
							{
								NNcurrentPhase = ANN_SEPARATE_CC_FROM_LD_NETWORK_COMBAT_SCENARIO_USED_FOR_MOVEMENT_PHASE;
							}
							else
							{
								NNcurrentPhase = currentPhase;
							}
						#else
							int NNcurrentPhase = currentPhase;
						#endif
							*/
						#ifdef ANN_USE_MOVEMENT_EXPERIENCES
							int NNcurrentPhase = currentPhase;
						#else
							int NNcurrentPhase = currentPhase;
						#endif
							if(!LRRCgameAI.addExperiencesFromUnitDecision(thisPlayerCurrentUnit, referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, unitTheoreticalBestDecisionToRecord, initialReferenceInThisPhaseStartScene, NNcurrentPhase, initialPlayerInList))
							{
								result = false;
								cout << "Error: cannot addExperiencesFromUnitDecision" << endl;
							}
					#endif
						}
						else
						{
							thisPlayerCurrentUnit->hasPerformedAction[currentPhase] = true;

							if(currentPhase == GAME_PHASE_MOVEMENT)
							{//movement phase
								//CHECKTHIS; add code here!
								//move towards unit

								//this may not be required .... but I currently assume this is required for movement towards opponent

								LDreference* backupOfUnitReferenceBeforeMovement = new LDreference(true);

								copyReferencesAndSubmodelDetails(backupOfUnitReferenceBeforeMovement, referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent, REFERENCE_TYPE_SUBMODEL);
								SHAREDvector.copyVectors(&(backupOfUnitReferenceBeforeMovement->absolutePosition), &(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->absolutePosition));		//copy in context absolute position details


									//CHECK THIS; moveUnitTowardsOpponent() is out by ~ a factor of 4
								if(moveUnitTowardsOpponent(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit))
								{
									//update absolute position of all subparts movement
									SHAREDvector.copyVectors(&(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponentInScene->relativePosition), &(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->relativePosition));	//CHECK THIS: I am not sure if relative positions changes based upon movements are been calcualted correctly at this time...  [NB they are not used by LRRC]		//copy relative position details into official reference in scene
									SHAREDvector.copyVectors(&(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponentInScene->absolutePosition), &(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->absolutePosition));							//copy absolute position details into official reference in scene
									updateAbsolutePositionOfAllSubModels(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponentInScene->firstReferenceWithinSubModel, referenceForThisPlayersCurrentUnitThatIsFindingAnOpponentInScene);

									/*
									//need to create new scene here
									//assumes initialReferenceInNewScene has been successfully created

									if(!compareScenesMovementPhase(preCombatPhaseSceneFileName, initialReferenceInThisPhaseStartScene, initialReferenceInNewScene, currentPlayer, targetSpritesSceneFileName, true))
									{
									#define ALLOW_FAULTY_MOVEMENTS_TO_BE_MADE_BY_NEURAL_NETWORK
									#ifdef ALLOW_FAULTY_MOVEMENTS_TO_BE_MADE_BY_NEURAL_NETWORK
											//currently allow faulty movements to be made by neural network...
										SHAREDvector.copyVectors(&(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->relativePosition), &backupOfUnitRelativePostion);
										SHAREDvector.copyVectors(&(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->absolutePosition), &backupOfUnitAbsolutePostion);
										opposingPlayerCurrentUnit->selfLearningTempVarUnitHasBeenTried  = true;	//this is redundant

									#else
											//if not allow faulty movements to be made by neural network:
										SHAREDvector.copyVectors(&(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->relativePosition), &backupOfUnitRelativePostion);
										SHAREDvector.copyVectors(&(referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->absolutePosition), &backupOfUnitAbsolutePostion);
										opposingPlayerCurrentUnit->selfLearningTempVarUnitHasBeenTried  = true;	//this is redundant
										performedActionYet = true;

									#endif

									}
									else
									{

									}
									*/
									
									thisPlayerCurrentUnit->hasPerformedAction[currentPhase] = true;
									performedActionYet = true;
									opposingPlayerCurrentUnit->selfLearningTempVarUnitHasBeenTried = true;	//this is redundant

									//add experience info for units
									UnitListClass* unitMoverInUnitList = thisPlayerCurrentUnit;

									int64_t unitTheoreticalBestDecisionToRecord;
									unitTheoreticalBestDecisionToRecord = currentPhase;

							#ifndef DEBUG_DO_NOT_USE_ADVANCED_LEARNING_BASED_UPON_DISTANCES

								/*
								#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
									int NNcurrentPhase;
									if(currentPhase == GAME_PHASE_MOVEMENT)
									{
										NNcurrentPhase = ANN_SEPARATE_CC_FROM_LD_NETWORK_COMBAT_SCENARIO_USED_FOR_MOVEMENT_PHASE;
									}
									else
									{
										NNcurrentPhase = currentPhase;
									}
								#else
									int NNcurrentPhase = currentPhase;
								#endif
								*/
								#ifdef ANN_USE_MOVEMENT_EXPERIENCES
									int NNcurrentPhase = currentPhase;
								#else
									int NNcurrentPhase = currentPhase;
								#endif

									if(!LRRCgameAI.addExperiencesFromUnitDecision(thisPlayerCurrentUnit, referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, unitTheoreticalBestDecisionToRecord, initialReferenceInThisPhaseStartScene, NNcurrentPhase, initialPlayerInList))
									{
										result = false;
										cout << "Error: cannot addExperiencesFromUnitDecision" << endl;
									}
							#endif

									//add target sprite info
									bool addTextualSpriteInfo;
									bool addRangeSpriteInfo ;
									bool addTargetSpriteInfo;
									int relevantPhaseForSprite;
									vec eyeCoords;
									eyeCoords.x = 0.0;
									eyeCoords.y = 0.0;
									eyeCoords.z = 0.0;

									addTextualSpriteInfo = false;
									addRangeSpriteInfo = true;
									addTargetSpriteInfo = true;
									relevantPhaseForSprite = currentPhase;	//GAME_PHASE_MOVEMENT
									if(!LRRCsprite.LRRCdetermineSpriteInfoAndAddSpriteToSpriteRefList(backupOfUnitReferenceBeforeMovement, referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent, targetSpriteListInitialReference, &eyeCoords, numTargetSpritesAdded, targetSpritesSceneFileName, addTextualSpriteInfo, addRangeSpriteInfo, addTargetSpriteInfo, relevantPhaseForSprite, currentPlayerTurn))
									{
										result = false;
									}

									addTextualSpriteInfo = false;
									addRangeSpriteInfo = false;
									addTargetSpriteInfo = true;

									relevantPhaseForSprite = GAME_PHASE_AIONLY_TARGET_SELECTION;
									if(!LRRCsprite.LRRCdetermineSpriteInfoAndAddSpriteToSpriteRefList(backupOfUnitReferenceBeforeMovement, referenceForOpposingPlayerCurrentUnit, targetSpriteListInitialReference, &eyeCoords, numTargetSpritesAdded, targetSpritesSceneFileName, addTextualSpriteInfo, addRangeSpriteInfo, addTargetSpriteInfo, relevantPhaseForSprite, currentPlayerTurn))
									{
										result = false;
									}
								}

								delete backupOfUnitReferenceBeforeMovement;

							}
							else
							{//combat phase

								string unitAttackerFileName = referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->name;
								string unitDefenderFileName =  referenceForOpposingPlayerCurrentUnit->name;

								int unitAttackerPlayerID = referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent->colour;
								int unitDefenderPlayerID = referenceForOpposingPlayerCurrentUnit->colour;

								int combatResult = performGenericCombatWithTwoCombatReadyUnitsAndAddSprites(currentRound, currentPlayerTurn, currentPhase, NULL, initialReferenceInThisPhaseStartScene, targetSpritesSceneFileName, unitAttackerFileName, unitDefenderFileName, unitAttackerPlayerID, unitDefenderPlayerID, targetSpriteListInitialReference, numTargetSpritesAdded, initialPlayerInList, false);

								if(!combatResult)
								{
									result = false;
									performedActionYet = true;	//escape out of loop - there was an IO error
									opposingPlayerCurrentUnit->selfLearningTempVarUnitHasBeenTried = true;	//this is redundant
								}
								else
								{
									if((combatResult == ATTACK_UNIT1_STRIKES_BOTH_CAN_STRIKE) || (combatResult == ATTACK_UNIT1_STRIKES_UNIT1_CAN_STRIKE))
									{
										cout << "ATTACK_UNIT1_STRIKES_BOTH_CAN_STRIKE, or ATTACK_UNIT1_STRIKES_UNIT1_CAN_STRIKE" << endl;

										opposingPlayerCurrentUnit->selfLearningTempVarUnitHasBeenTried = true;	//this is redundant
										performedActionYet = true;
									}
									else if((combatResult == ATTACK_UNIT2_STRIKES_BOTH_CAN_STRIKE) || (combatResult == ATTACK_UNIT2_STRIKES_UNIT2_CAN_STRIKE))
									{
										cout << "ATTACK_UNIT2_STRIKES_BOTH_CAN_STRIKE, or ATTACK_UNIT2_STRIKES_UNIT2_CAN_STRIKE" << endl;

										opposingPlayerCurrentUnit->selfLearningTempVarUnitHasBeenTried = true;	//this is redundant
										performedActionYet = true;
									}
									else if((combatResult == ATTACK_NEITHER_UNIT_CAN_STRIKE) || (combatResult == ATTACK_NEITHER_STRIKES_UNIT2_CAN_STRIKE))
									{//unit 1 cannot strike this particualar opponent; try another opponent
										//cout << "ATTACK_NEITHER_UNIT_CAN_STRIKE, ATTACK_NEITHER_STRIKES_UNIT1_CAN_STRIKE, or ATTACK_NEITHER_STRIKES_UNIT2_CAN_STRIKE" << endl;

										opposingPlayerCurrentUnit->selfLearningTempVarUnitHasBeenTried = true;
									}
									else if((combatResult == ATTACK_NEITHER_UNITS_STRIKE_BOTH_CAN_STRIKE) || (combatResult == ATTACK_NEITHER_STRIKES_UNIT1_CAN_STRIKE))
									{
										if(currentPhase == GAME_PHASE_CLOSECOMBAT)
										{
											cout << "ATTACK_NEITHER_UNITS_STRIKE_BOTH_CAN_STRIKE, ATTACK_NEITHER_STRIKES_UNIT1_CAN_STRIKE" << endl;
											//allow another attempt for this unit to fight the same opponent during this phase ... [NB this has an issue, whether the unit may now select another/diferent opponent to fight, thereby reciving a chance to attack 2 people in 1 turn" << endl;
										}
										else if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
										{
											opposingPlayerCurrentUnit->selfLearningTempVarUnitHasBeenTried = true;	//this is redundant
											performedActionYet = true;
										}
										else
										{
											cout << "illegal phase detetcted, exiting now" << endl;
											exit(0);
										}
									}
								}
							}
						}
					}
					else
					{
						//try performing an action with another unit due to creativity
						opposingPlayerCurrentUnit->selfLearningTempVarUnitHasBeenTried = true;
					}
					delete referenceForOpposingPlayerCurrentUnit;
				}

			}
			delete referenceForThisPlayersCurrentUnitThatIsFindingAnOpponent;

		}


		thisPlayerCurrentUnit = thisPlayerCurrentUnit->next;
	}

	return findOpponentsForAllUnitsWithinUnitGroupToTarget;
}



//bool AIsearchUnitListAndCalculateWorthOfOpponents(int currentPlayerTurn, int currentPhase, string preMovementPhaseSceneFileNameMovement, string thisPhaseStartSceneFileNameMovement, string preCombatPhaseSceneFileName, string postCombatPhaseSceneFileName, string targetSpritesSceneFileName, LDreference* initialReferenceInSceneFile, LDreference* targetSpriteListInitialReference, int* numTargetSpritesAdded, Player* initialPlayerInList, UnitListClass* firstUnitInUnitGroup, UnitListClass* playerUnitThatIsFindingAnOpponent, LDreference* referenceToPlayerUnitThatIsFindingAnOpponent)
bool LRRCgameClass::AIsearchUnitListAndCalculateWorthOfOpponents(const int currentPlayerTurn, const int currentPhase, LDreference* initialReferenceInThisPhaseStartScene, Player* initialPlayerInList, UnitListClass* firstUnitInOpponentUnitGroup, const UnitListClass* playerUnitThatIsFindingAnOpponent, LDreference* referenceToPlayerUnitThatIsFindingAnOpponent)
{
	bool result = true;

	/*
	//NEW;
	self learning algorithm update;
		work out what want to do (Eg close combat);
			for every player A unit, search through unit list and calc the error for fighting a particular unit;
			perform combat with the the unit with lowest error
	*/

	UnitListClass* currentUnit = firstUnitInOpponentUnitGroup;		//arbitrary player; as there is currently one 1 unit list, and this is referenced by all players
	UnitListClass* recordOfUnitWithLeastError;

	while(currentUnit->next != NULL)
	{
		if((currentUnit->team != currentPlayerTurn) && (currentUnit->status == ALIVE))
		{
			bool currentUnitIDFound = false;
			bool currentUnitFoundResult = true;


			//1. create a dummy reference for details calculations
			LDreference* referenceForOpposingPlayerCurrentUnit = new LDreference(true);
			referenceForOpposingPlayerCurrentUnit->colour = currentUnit->team;
			referenceForOpposingPlayerCurrentUnit->name = currentUnit->name;
			if(!LRRCcombat.searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(referenceForOpposingPlayerCurrentUnit, initialReferenceInThisPhaseStartScene, NULL, false))
			{
				cout << "error: cannot find player unit reference in scene file, name = " << referenceForOpposingPlayerCurrentUnit->name << " id=" << referenceForOpposingPlayerCurrentUnit->colour << endl;
				result = false;
			}
			LRRCcombat.performFinalUnitClassCalculations(referenceForOpposingPlayerCurrentUnit->subModelDetails);

			Player* currentPlayer = LRRCplayerClass.findPlayer(initialPlayerInList, currentPlayerTurn);

				//create an experience object (inputs only) given the current unit and its envirornment

			//int combatExperienceResult = IRRELEVANT_NOT_USED;

			//fire the current experience object inputs into the network

			double experienceBackPropagationPassError;
			int nn;
			int64_t unitHypotheticalDecisionToTest;

			double optimumPositionAttackLDpropertiesExperienceBackPropagationPassError;
			double optimumPositionAttackLDcombatExperienceBackPropagationPassError;
			double optimumPositionEvadeLDpropertiesExperienceBackPropagationPassError;
			double optimumPositionEvadeLDcombatExperienceBackPropagationPassError;
			double optimumPositionAttackCCpropertiesExperienceBackPropagationPassError;
			double optimumPositionAttackCCcombatExperienceBackPropagationPassError;
			double optimumPositionEvadeCCpropertiesExperienceBackPropagationPassError;
			double optimumPositionEvadeCCcombatExperienceBackPropagationPassError;
			double attackLDpropertiesExperienceBackPropagationPassError;
			double attackLDcombatExperienceBackPropagationPassError;
			double evadeLDpropertiesExperienceBackPropagationPassError;
			double evadeLDcombatExperienceBackPropagationPassError;
			double attackCCpropertiesExperienceBackPropagationPassError;
			double attackCCcombatExperienceBackPropagationPassError;
			double evadeCCpropertiesExperienceBackPropagationPassError;
			double evadeCCcombatExperienceBackPropagationPassError;


			//Save real positions
			vec savedPlayerUnitThatIsFindingAnOpponentAbsolutePosition;
			vec savedOpposingPlayerCurrentUnitAbsolutePosition;
			SHAREDvector.copyVectors(&savedPlayerUnitThatIsFindingAnOpponentAbsolutePosition, &(referenceToPlayerUnitThatIsFindingAnOpponent->absolutePosition));
			SHAREDvector.copyVectors(&savedOpposingPlayerCurrentUnitAbsolutePosition, &(referenceForOpposingPlayerCurrentUnit->absolutePosition));

			if((currentPhase == GAME_PHASE_MOVEMENT) || (currentPhase == GAME_PHASE_LONGDISTANCECOMBAT))
			{
				//Long Distance Consideration
					//find range of the ideal long range weapon (most powerful)
				XMLrulesClass* currentReferenceRulesClass = LRRCrulesUnitCombatDetailsAttackLongDistance;
				RecordClass* currentReferenceRecordClass = referenceToPlayerUnitThatIsFindingAnOpponent->subModelDetails->recordOfUnitCombatDetailsAttackLongDistance;
				bool foundOptimumLongDistanceWeapon = false;
				int longDistanceAttackBaseRange = 0;
				while((currentReferenceRecordClass->next != NULL) && (!foundOptimumLongDistanceWeapon))
				{
					if(currentReferenceRecordClass->numberOfThisPartIDInTheUnit > 0)
					{
						foundOptimumLongDistanceWeapon = true;
						longDistanceAttackBaseRange = currentReferenceRulesClass->attribute5;
					}

					currentReferenceRecordClass = currentReferenceRecordClass->next;
					currentReferenceRulesClass = currentReferenceRulesClass->next;
				}
					//set range of the ideal long range weapon (most powerful)
				vec LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition;
				LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.x = 0;
				LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.y = 0;
				LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.z = 0;
				vec LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition;
				LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.x = 0;
				LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.y = longDistanceAttackBaseRange;
				LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.z = 0;
				SHAREDvector.copyVectors(&(referenceToPlayerUnitThatIsFindingAnOpponent->absolutePosition), &LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition);
				SHAREDvector.copyVectors(&(referenceForOpposingPlayerCurrentUnit->absolutePosition), &LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition);
					//find error of LD attack/LD evasion when at optimum LD attack distance



				optimumPositionAttackLDpropertiesExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_LONGDISTANCECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_LONGDISTANCE, currentPlayer, GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

				optimumPositionAttackLDcombatExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_LONGDISTANCECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_LONGDISTANCE, currentPlayer, GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

				optimumPositionEvadeLDpropertiesExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_LONGDISTANCECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_LONGDISTANCE, currentPlayer, GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

				optimumPositionEvadeLDcombatExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_LONGDISTANCECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_LONGDISTANCE, currentPlayer, GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

			}

			if((currentPhase == GAME_PHASE_MOVEMENT) || (currentPhase == GAME_PHASE_CLOSECOMBAT))
			{
				//Close Combat Consideration [CHECK THIS; this needs to be done only for the units most probable for LD attack]
					//set range of close combat
				vec LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition;
				LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.x = 0;
				LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.y = 0;
				LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition.z = 0;
				vec LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition;
				LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.x = 0;
				LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.y = (CLOSE_AND_LONGDISTANCE_COMBAT_BOUNDARY-1);
				LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition.z = 0;
				SHAREDvector.copyVectors(&(referenceToPlayerUnitThatIsFindingAnOpponent->absolutePosition), &LDhypotheticalPlayerUnitThatIsFindingAnOpponentAbsolutePosition);
				SHAREDvector.copyVectors(&(referenceForOpposingPlayerCurrentUnit->absolutePosition), &LDhypotheticalOpposingPlayerCurrentUnitAbsolutePosition);
					//find error of CC attack/CC evasion when at optimum CC attack distance

				optimumPositionAttackCCpropertiesExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_CLOSECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_CLOSECOMBAT, currentPlayer, GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

				optimumPositionAttackCCcombatExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_CLOSECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_CLOSECOMBAT, currentPlayer, GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

				optimumPositionEvadeCCpropertiesExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_CLOSECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_CLOSECOMBAT, currentPlayer, GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

				optimumPositionEvadeCCcombatExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_CLOSECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_CLOSECOMBAT, currentPlayer, GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

			}

			//Restore real positions
			SHAREDvector.copyVectors(&(referenceToPlayerUnitThatIsFindingAnOpponent->absolutePosition), &savedPlayerUnitThatIsFindingAnOpponentAbsolutePosition);
			SHAREDvector.copyVectors(&(referenceForOpposingPlayerCurrentUnit->absolutePosition), &savedOpposingPlayerCurrentUnitAbsolutePosition);


			if((currentPhase == GAME_PHASE_MOVEMENT) || (currentPhase == GAME_PHASE_LONGDISTANCECOMBAT))
			{
				//Long Distance Consideration
					//find error of LD attack/LD evasion when at optimum LD attack distance

				attackLDpropertiesExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_LONGDISTANCECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_LONGDISTANCE, currentPlayer, GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

				attackLDcombatExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_LONGDISTANCECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_LONGDISTANCE, currentPlayer, GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

				evadeLDpropertiesExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_LONGDISTANCECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_LONGDISTANCE, currentPlayer, GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

				evadeLDcombatExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_LONGDISTANCECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_LONGDISTANCE, currentPlayer, GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);
			}

			if((currentPhase == GAME_PHASE_MOVEMENT) || (currentPhase == GAME_PHASE_CLOSECOMBAT))
			{
				//Close Combat Consideration [CHECK THIS; this needs to be done only for the units most probable for LD attack]
					//find error of CC attack/CC evasion when at optimum CC attack distance


				attackCCpropertiesExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_CLOSECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_CLOSECOMBAT, currentPlayer, GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

				attackCCcombatExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_CLOSECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_CLOSECOMBAT, currentPlayer, GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

				evadeCCpropertiesExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_CLOSECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_CLOSECOMBAT, currentPlayer, GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);

				evadeCCcombatExperienceBackPropagationPassError = LRRCgameAI.addOrCompareExperienceFromUnitDecision(GAME_PHASE_CLOSECOMBAT, NULL, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_CLOSECOMBAT, currentPlayer, GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, COMPARE_EXPERIENCE, NULL);
			}

			/*CHECK THIS - need to add creativity into the following calculations !!!
			double randomNumberBetween0And1;
			#ifdef DEBUG_NO_CREATIVITY_ALWAYS_PERFORM_MOVEMENT_INSTEAD_OF_STAND_FOR_LDCOMBAT
			randomNumberBetween0And1 = 1.0;
			#else
			randomNumberBetween0And1 = ((double(abs((short)rand())))/(ABS_OF_SHORT));
			#endif

			(randomNumberBetween0And1 < currentPlayer->creativity)
			*/

			//CHECK THIS - this needs to be completed....
			currentUnit->selfLearningTempVarUnitHasBeenTried = false;
			currentUnit->selfLearningTempVarUnitPrefersLongDistanceCombatToMovement = false;

			if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
			{//always perform long distance combat with best current opponent (if can; ie if have not moved in current round)
			#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
				nn = (GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(GAME_PHASE_LONGDISTANCECOMBAT-1);
			#else
				nn = GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN;
			#endif
				currentUnit->selfLearningTempVarBackPropagationPassError[nn] = attackLDpropertiesExperienceBackPropagationPassError;

			#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
				nn = (GAME_INDEX_OF_COMBAT_EXPERIENCE_NN*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(GAME_PHASE_LONGDISTANCECOMBAT-1);
			#else
				nn = GAME_INDEX_OF_COMBAT_EXPERIENCE_NN;
			#endif

				currentUnit->selfLearningTempVarBackPropagationPassError[nn] = attackLDcombatExperienceBackPropagationPassError;

			}
			else if(currentPhase == GAME_PHASE_CLOSECOMBAT)
			{//always perform close combat with best current opponent

			#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
				nn = (GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(GAME_PHASE_CLOSECOMBAT-1);
			#else
				nn = GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN;
			#endif
				currentUnit->selfLearningTempVarBackPropagationPassError[nn] = attackCCpropertiesExperienceBackPropagationPassError;

			#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
				nn = (GAME_INDEX_OF_COMBAT_EXPERIENCE_NN*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(GAME_PHASE_CLOSECOMBAT-1);
			#else
				nn = GAME_INDEX_OF_COMBAT_EXPERIENCE_NN;
			#endif
				currentUnit->selfLearningTempVarBackPropagationPassError[nn] = attackCCcombatExperienceBackPropagationPassError;
			}
			else if(currentPhase == GAME_PHASE_MOVEMENT)
			{//do not always move into opponent (may prefer to stop this round in order to perform LD combat in next round)
				//CHECKTHIS at later stages in development; this section could be simplified to check neural network for preferred movements. However in order to generate a NN with preferred movements, preferred long distance and prefferred close combat opponents should be used as an indicator.

				if((attackLDpropertiesExperienceBackPropagationPassError < (optimumPositionAttackLDpropertiesExperienceBackPropagationPassError+GAME_ANN_MAX_ERROR))
				&& (attackLDcombatExperienceBackPropagationPassError < (optimumPositionAttackLDcombatExperienceBackPropagationPassError+GAME_ANN_MAX_ERROR))
				&& (optimumPositionAttackLDpropertiesExperienceBackPropagationPassError < (optimumPositionAttackCCpropertiesExperienceBackPropagationPassError))
				&& (optimumPositionAttackLDcombatExperienceBackPropagationPassError < (optimumPositionAttackCCcombatExperienceBackPropagationPassError)))
				{//stop (dont move) and perform LD combat if LD combat is better than or equal to optimum LD combat in this round, and optimum LD combat is better than optimum CC combat

					currentUnit->selfLearningTempVarUnitPrefersLongDistanceCombatToMovement = true;
				}
				else
				{//keep moving towards opponent (for either LD combat in a future round or CC combat in this round or a future round)

				#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
					nn = (GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(GAME_PHASE_CLOSECOMBAT-1);
					currentUnit->selfLearningTempVarBackPropagationPassError[nn] = optimumPositionAttackCCpropertiesExperienceBackPropagationPassError;
					nn = (GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(GAME_PHASE_LONGDISTANCECOMBAT-1);
					currentUnit->selfLearningTempVarBackPropagationPassError[nn] = optimumPositionAttackLDpropertiesExperienceBackPropagationPassError;
				#else
					nn = GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN;

					double minErrorProperties;
					if(optimumPositionAttackLDpropertiesExperienceBackPropagationPassError < optimumPositionAttackCCpropertiesExperienceBackPropagationPassError)
					{
						minErrorProperties = optimumPositionAttackLDpropertiesExperienceBackPropagationPassError;
					}
					else
					{
						minErrorProperties = optimumPositionAttackCCpropertiesExperienceBackPropagationPassError;
					}

					currentUnit->selfLearningTempVarBackPropagationPassError[nn] = minErrorProperties;
				#endif


				#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
					nn = (GAME_INDEX_OF_COMBAT_EXPERIENCE_NN*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(GAME_PHASE_CLOSECOMBAT-1);
					currentUnit->selfLearningTempVarBackPropagationPassError[nn] = optimumPositionAttackCCcombatExperienceBackPropagationPassError;
					nn = (GAME_INDEX_OF_COMBAT_EXPERIENCE_NN*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(GAME_PHASE_LONGDISTANCECOMBAT-1);
					currentUnit->selfLearningTempVarBackPropagationPassError[nn] = optimumPositionAttackLDcombatExperienceBackPropagationPassError;
				#else
					nn = GAME_INDEX_OF_COMBAT_EXPERIENCE_NN;

					double minErrorCombat;
					if(optimumPositionAttackLDcombatExperienceBackPropagationPassError < optimumPositionAttackCCcombatExperienceBackPropagationPassError)
					{
						minErrorCombat = optimumPositionAttackLDcombatExperienceBackPropagationPassError;
					}
					else
					{
						minErrorCombat = optimumPositionAttackCCcombatExperienceBackPropagationPassError;
					}

					currentUnit->selfLearningTempVarBackPropagationPassError[nn] = minErrorCombat;
				#endif
				}
			}

		#ifndef DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
		#ifdef ANN_SEPARATE_CC_FROM_LD_NETWORK
			nn = (GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN*ANN_TOTAL_NUMBER_OF_COMBAT_SCENARIOS)+(GAME_PHASE_CLOSECOMBAT-1);
		#else
			nn = GAME_INDEX_OF_GLOBAL_EXPERIENCE_NN;
		#endif
			ANNexperience* experienceWithoutKnownOutput = new ANNexperience[GAME_NUMBER_OF_EXPERIENCE_NN];
			LRRCgameAI.generateExperienceFromGlobalDecision(currentPlayer->firstUnitInUnitList, initialReferenceInThisPhaseStartScene, referenceToPlayerUnitThatIsFindingAnOpponent, referenceForOpposingPlayerCurrentUnit, &(experienceWithoutKnownOutput[nn]));
			experienceBackPropagationPassError = ANNalgorithmBackpropagationTraining.calculateExperienceErrorForHypotheticalDecisionBackpropagation(currentPlayer->firstInputNeuronInNetwork[nn], currentPlayer->firstOutputNeuronInNetwork[nn], currentPlayer->numberOfInputNeurons[nn], currentPlayer->numberOfOutputNeurons[nn], &(experienceWithoutKnownOutput[nn]));
			currentUnit->selfLearningTempVarBackPropagationPassError[nn] = experienceBackPropagationPassError;
			delete[] experienceWithoutKnownOutput;
		#endif

			delete referenceForOpposingPlayerCurrentUnit;
		}
		else
		{
			//cout << "currentUnit->name = " << currentUnit->name << endl;
		}

		if(currentUnit->isUnitGroup)
		{
			if(!AIsearchUnitListAndCalculateWorthOfOpponents(currentPlayerTurn, currentPhase, initialReferenceInThisPhaseStartScene, initialPlayerInList, currentUnit->firstUnitInUnitGroup, playerUnitThatIsFindingAnOpponent, referenceToPlayerUnitThatIsFindingAnOpponent))
			{
				result = false;
			}
		}

		currentUnit = currentUnit->next;
	}

	return result;
}






#endif



int LRRCgameClass::performGenericCombatWithTwoCombatReadyUnitsAndAddSprites(const int currentRound, const int currentPlayerTurn, int currentPhase, LDreference* initialReferenceInPreMovementPhaseScene, LDreference* initialReferenceInThisPhaseStartScene, string targetSpritesSceneFileName, string unitAttackerFileName, string unitDefenderFileName, int unitAttackerPlayerID, int unitDefenderPlayerID, LDreference* targetSpriteListInitialReference, int* numTargetSpritesAdded, Player* initialPlayerInList, const bool checkPreviousSceneFile)
{
	bool result = true;

	int combatResult;
	if(currentPhase == GAME_PHASE_CLOSECOMBAT)
	{
		if(checkPreviousSceneFile)
		{
			combatResult = LRRCcombat.performCloseCombatWithConsecutiveScenes(unitAttackerFileName, unitDefenderFileName, unitAttackerPlayerID, unitDefenderPlayerID, true, true, initialReferenceInThisPhaseStartScene, initialReferenceInPreMovementPhaseScene);
		}
		else
		{
			combatResult = LRRCcombat.performCloseCombatWithScene(unitAttackerFileName, unitDefenderFileName, unitAttackerPlayerID, unitDefenderPlayerID, true, true, initialReferenceInThisPhaseStartScene);
		}
			//slight deviation from current rules: NB player 2 will not have moved in previous round [therefore player 2/the defender cannot ever get a charge advantage]
	}
	else if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
	{
		if(checkPreviousSceneFile)
		{
			combatResult = LRRCcombat.performLongDistanceCombatWithConsecutiveScenes(unitAttackerFileName, unitDefenderFileName, unitAttackerPlayerID, unitDefenderPlayerID, true, false, initialReferenceInThisPhaseStartScene, initialReferenceInPreMovementPhaseScene);
		}
		else
		{
			//cout << "perform LD..." << endl;
			combatResult = LRRCcombat.performLongDistanceCombatWithScene(unitAttackerFileName, unitDefenderFileName, unitAttackerPlayerID, unitDefenderPlayerID, true, false, initialReferenceInThisPhaseStartScene);
		}
	}
	else
	{
		result = false;
		cout << "error: performGenericCombatWithTwoCombatReadyUnitsAndAddSprites{} illegal phase " << endl;
	}

	if((combatResult == ATTACK_ERROR_FILE_IO) || (combatResult == ATTACK_NEITHER_UNIT_CAN_STRIKE) || (combatResult == ATTACK_UNIT2_STRIKES_UNIT2_CAN_STRIKE) || (combatResult == ATTACK_NEITHER_STRIKES_UNIT2_CAN_STRIKE))
	{
		#ifndef USE_ANN
		cout << "unit " << unitAttackerFileName << " of the player " << currentPlayerTurn << " could not attack unit " << unitDefenderFileName << ". Please retry with another target" << endl;
		#endif
	}
	else
	{
		bool locationResult;

		//generate reference record for sprite info generation

		LDreference* topLevelReferenceInSceneFile = new LDreference(true);
		LDreference* unitReference = new LDreference(unitAttackerFileName, unitAttackerPlayerID, true);
		LDreference* targetReference = new LDreference(unitDefenderFileName, unitDefenderPlayerID, true);

		locationResult = true;

		if(!LRRCcombat.searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(unitReference, initialReferenceInThisPhaseStartScene, topLevelReferenceInSceneFile, false))
		{
			cout << "error: cannot refind unit unitAttackerFileName=" << unitAttackerFileName << " unitAttackerPlayerID=" << unitAttackerPlayerID << endl;
			result = false;
		}
		LRRCcombat.performFinalUnitClassCalculations(unitReference->subModelDetails);

		if(!LRRCcombat.searchSceneReferenceListAndDetermineTheDetailsOfAParticularUnitSubmodel(targetReference, initialReferenceInThisPhaseStartScene, topLevelReferenceInSceneFile, false))
		{
			cout << "error: cannot refind unit unitDefenderFileName=" << unitDefenderFileName << " unitDefenderPlayerID=" << unitDefenderPlayerID << endl;
			result = false;
		}
		LRRCcombat.performFinalUnitClassCalculations(targetReference->subModelDetails);


		//CHECKTHIS: new rule: only assign - has performed combat - if combat resulted in a strike (death)
		/*
		if((combatResult == ATTACK_UNIT1_STRIKES_BOTH_CAN_STRIKE) || (combatResult == ATTACK_UNIT1_STRIKES_UNIT1_CAN_STRIKE) || (combatResult == ATTACK_UNIT2_STRIKES_BOTH_CAN_STRIKE) || (combatResult == ATTACK_UNIT2_STRIKES_UNIT2_CAN_STRIKE))
		{
			//assign hasPerformedAction to attacker
			locationResult = false;
			searchReferenceListAssignHasPerformedCombat(currentPhase, initialReferenceInThisPhaseStartScene, unitAttackerFileName, unitAttackerPlayerID, &locationResult, &result);
			if(!locationResult)
			{
				result = false;
				cout << "error: cannot assign hasPerformedAction to unit unitAttackerFileName=" << unitAttackerFileName << " unitAttackerPlayerID=" << unitAttackerPlayerID << endl;
			}
		}
		*/




		locationResult = false;
		UnitListClass* unitDefenderInUnitList = LRRCunitClass.searchUnitListFindUnit(initialPlayerInList->firstUnitInUnitList, unitDefenderFileName, unitDefenderPlayerID, &locationResult);
		if(!locationResult)
		{
			result = false;
			cout << "error: cannot find unit in unit list, unitDefenderFileName=" << unitDefenderFileName << " unitDefenderPlayerID=" << unitDefenderPlayerID << endl;
		}
		locationResult = false;
		UnitListClass* unitAttackerInUnitList = LRRCunitClass.searchUnitListFindUnit(initialPlayerInList->firstUnitInUnitList, unitAttackerFileName, unitAttackerPlayerID, &locationResult);
		if(!locationResult)
		{
			result = false;
			cout << "error: cannot find unit in unit list, unitAttackerFileName=" << unitAttackerFileName << " unitAttackerPlayerID=" << unitAttackerPlayerID << endl;
		}



		unitDefenderInUnitList->hasPerformedAction[currentPhase] = true;
		if((combatResult == ATTACK_UNIT1_STRIKES_BOTH_CAN_STRIKE) || (combatResult == ATTACK_UNIT1_STRIKES_UNIT1_CAN_STRIKE))
		{//remove dead units from reference list:

			unitAttackerInUnitList->killPoints = unitAttackerInUnitList->killPoints + LRRCunitClass.determineUnitWorthInPoints(unitDefenderInUnitList->unitDetails);
			unitAttackerInUnitList->numKills = unitAttackerInUnitList->numKills + 1;

			locationResult = false;

			//cout << "DEBUG: scene before removing reference: " << endl;
			//searchReferenceListPrintReferenceDetails(initialReferenceInThisPhaseStartScene);

			initialReferenceInThisPhaseStartScene = LRRCgameReferenceManipulation.searchReferenceListRemoveReference(initialReferenceInThisPhaseStartScene, unitDefenderFileName, unitDefenderPlayerID, &locationResult, &result);

			//cout << "DEBUG: scene after removing reference: " << endl;
			//searchReferenceListPrintReferenceDetails(initialReferenceInThisPhaseStartScene);


			if(!locationResult)
			{
				result = false;
				cout << "error: cannot remove dead unit from reference list, unitDefenderFileName=" << unitDefenderFileName << " unitDefenderPlayerID=" << unitDefenderPlayerID << endl;
			}
			else
			{
				cout << "unitDefenderFileName, " << unitDefenderFileName << " has been removed from the scene" << endl;
			}

			unitDefenderInUnitList->status = false;
			unitDefenderInUnitList->roundRemoved = currentRound;
		}

		unitAttackerInUnitList->hasPerformedAction[currentPhase] = true;

		if((combatResult == ATTACK_UNIT2_STRIKES_BOTH_CAN_STRIKE) || (combatResult == ATTACK_UNIT2_STRIKES_UNIT2_CAN_STRIKE))
		{//remove dead units from reference list:

			unitDefenderInUnitList->killPoints = unitDefenderInUnitList->killPoints + LRRCunitClass.determineUnitWorthInPoints(unitAttackerInUnitList->unitDetails);
			unitDefenderInUnitList->numKills = unitDefenderInUnitList->numKills + 1;

			locationResult = false;
			initialReferenceInThisPhaseStartScene = LRRCgameReferenceManipulation.searchReferenceListRemoveReference(initialReferenceInThisPhaseStartScene, unitAttackerFileName, unitAttackerPlayerID, &locationResult, &result);
			if(!locationResult)
			{
				result = false;
				cout << "error: cannot remove dead unit from reference list, unitAttackerFileName=" << unitAttackerFileName << " unitAttackerPlayerID=" << unitAttackerPlayerID << endl;
			}
			else
			{
				cout << "unitAttackerFileName, " << unitAttackerFileName << " has been removed from the scene" << endl;
			}

			unitAttackerInUnitList->status = false;
			unitAttackerInUnitList->roundRemoved = currentRound;
		}

	#ifdef USE_ANN


		//add experience info for units

		int evadeDecision;
		int attackDecision;
		if(currentPhase == GAME_PHASE_CLOSECOMBAT)
		{
			attackDecision = COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_CLOSECOMBAT;
			evadeDecision = COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_CLOSECOMBAT;
		}
		else if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
		{
			attackDecision = COMBAT_EXPERIENCE_OUTPUT_DECISION_ATTACK_LONGDISTANCE;
			evadeDecision = COMBAT_EXPERIENCE_OUTPUT_DECISION_EVADE_LONGDISTANCE;
		}
		else
		{
			//result = false;
			cout << "error: THperformGenericCombatWithTwoCombatReadyUnitsAndCompareExperienceWithNN{} illegal phase " << endl;
		}

		int combatExperienceResult;
		int unitTheoreticalBestDecisionToRecord;
		if((combatResult == ATTACK_NEITHER_UNIT_CAN_STRIKE) || (combatResult == ATTACK_NEITHER_STRIKES_UNIT2_CAN_STRIKE) || (combatResult == ATTACK_NEITHER_STRIKES_UNIT1_CAN_STRIKE))
		{

		}
		else if((combatResult == ATTACK_BOTH_UNITS_STRIKE_BOTH_CAN_STRIKE) || (combatResult == ATTACK_NEITHER_UNITS_STRIKE_BOTH_CAN_STRIKE))
		{

		}
		else if((combatResult == ATTACK_UNIT1_STRIKES_BOTH_CAN_STRIKE) || (combatResult == ATTACK_UNIT1_STRIKES_UNIT1_CAN_STRIKE))
		{
			unitTheoreticalBestDecisionToRecord = attackDecision;
			LRRCgameAI.addExperiencesFromUnitDecision(unitAttackerInUnitList, unitReference, targetReference, unitTheoreticalBestDecisionToRecord, initialReferenceInThisPhaseStartScene, currentPhase, initialPlayerInList);
			/*
			if(!LRRCgameAI.addExperiencesFromUnitDecision(unitReference, targetReference, unitTheoreticalBestDecisionToRecord, initialPlayerInList->firstUnitInUnitList, initialReferenceInThisPhaseStartScene))		//arbitrary player, OLD=attackerPlayer
			{
				result = false;
				cout << "Error: cannot addExperiencesFromUnitDecision" << endl;
			}
			*/

			unitTheoreticalBestDecisionToRecord = evadeDecision;
			LRRCgameAI.addOrCompareExperienceFromUnitDecision(currentPhase, unitDefenderInUnitList, targetReference, unitReference, unitTheoreticalBestDecisionToRecord, initialPlayerInList, GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, ADD_EXPERIENCE, initialReferenceInThisPhaseStartScene);		//arbitrary player, OLD=attackerPlayer
			LRRCgameAI.addOrCompareExperienceFromUnitDecision(currentPhase, unitDefenderInUnitList, targetReference, unitReference, unitTheoreticalBestDecisionToRecord, initialPlayerInList, GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, ADD_EXPERIENCE, initialReferenceInThisPhaseStartScene);		//arbitrary player, OLD=attackerPlayer

			/*
			if(!addExperienceFromUnitPropertiesDecision(targetReference, unitReference, unitTheoreticalBestDecisionToRecord, initialPlayerInList->firstUnitInUnitList))		//arbitrary player, OLD=defenderPlayer
			{
				result = false;
				cout << "Error: cannot addExperiencesFromUnitDecision" << endl;
			}
			if(!addExperienceFromUnitCombatDecision(targetReference, unitReference, unitTheoreticalBestDecisionToRecord, initialPlayerInList->firstUnitInUnitList))		//arbitrary player, OLD=defenderPlayer
			{
				result = false;
				cout << "Error: cannot addExperiencesFromUnitDecision" << endl;
			}
			*/
		}
		else if((combatResult == ATTACK_UNIT2_STRIKES_BOTH_CAN_STRIKE) || (combatResult == ATTACK_UNIT2_STRIKES_UNIT2_CAN_STRIKE))
		{
			unitTheoreticalBestDecisionToRecord = attackDecision;
			LRRCgameAI.addExperiencesFromUnitDecision(unitDefenderInUnitList, targetReference, unitReference, unitTheoreticalBestDecisionToRecord, initialReferenceInThisPhaseStartScene, currentPhase, initialPlayerInList);

			/*
			if(!LRRCgameAI.addExperiencesFromUnitDecision(targetReference, unitReference, unitTheoreticalBestDecisionToRecord, initialPlayerInList->firstUnitInUnitList, initialReferenceInThisPhaseStartScene))		//arbitrary player, OLD=defenderPlayer
			{
				result = false;
				cout << "Error: cannot addExperiencesFromUnitDecision" << endl;
			}
			*/

			unitTheoreticalBestDecisionToRecord = evadeDecision;
			LRRCgameAI.addOrCompareExperienceFromUnitDecision(currentPhase, unitAttackerInUnitList, unitReference, targetReference, unitTheoreticalBestDecisionToRecord, initialPlayerInList, GAME_INDEX_OF_PROPERTIES_EXPERIENCE_NN, ADD_EXPERIENCE, initialReferenceInThisPhaseStartScene);		//arbitrary player, OLD=attackerPlayer
			LRRCgameAI.addOrCompareExperienceFromUnitDecision(currentPhase, unitAttackerInUnitList, unitReference, targetReference, unitTheoreticalBestDecisionToRecord, initialPlayerInList, GAME_INDEX_OF_COMBAT_EXPERIENCE_NN, ADD_EXPERIENCE, initialReferenceInThisPhaseStartScene);		//arbitrary player, OLD=attackerPlayer

			/*
			if(!addExperienceFromUnitPropertiesDecision(unitReference, targetReference, unitTheoreticalBestDecisionToRecord, initialPlayerInList->firstUnitInUnitList))		//arbitrary player, OLD=attackerPlayer
			{
				result = false;
				cout << "Error: cannot addExperiencesFromUnitDecision" << endl;
			}
			if(!addExperienceFromUnitCombatDecision(unitReference, targetReference, unitTheoreticalBestDecisionToRecord, initialPlayerInList->firstUnitInUnitList))		//arbitrary player, OLD=attackerPlayer
			{
				result = false;
				cout << "Error: cannot addExperiencesFromUnitDecision" << endl;
			}
			*/
		}
	#endif
	

		//add target sprite info
		bool addTextualSpriteInfo = false;
		bool addRangeSpriteInfo = true;
		bool addTargetSpriteInfo = true;
		vec eyeCoords;
		eyeCoords.x = 0.0;
		eyeCoords.y = 0.0;
		eyeCoords.z = 0.0;
		if(!LRRCsprite.LRRCdetermineSpriteInfoAndAddSpriteToSpriteRefList(unitReference, targetReference, targetSpriteListInitialReference, &eyeCoords, numTargetSpritesAdded, targetSpritesSceneFileName, addTextualSpriteInfo, addRangeSpriteInfo, addTargetSpriteInfo, currentPhase, currentPlayerTurn))
		{
			result = false;
		}
		delete unitReference;
		delete targetReference;
		delete topLevelReferenceInSceneFile;
	}

	return result;
}




bool LRRCgameClass::generateSceneFileName(const int currentGame, const int currentRound, const int currentPlayerTurn, const int currentPhase, const int phaseExecutionStage, string* sceneFileName)
{
	bool result = true;

	string currentRoundString = "";
	string currentPlayerTurnString = "";
	string currentGameString = "";

	currentRoundString = SHAREDvars.convertIntToString(currentRound);
	currentPlayerTurnString = SHAREDvars.convertIntToString(currentPlayerTurn);
	currentGameString = SHAREDvars.convertIntToString(currentGame);

	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_START;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_GAME_HEADER;
	*sceneFileName = *sceneFileName + currentGameString;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_ROUND_HEADER;
	*sceneFileName = *sceneFileName + currentRoundString;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_PLAYER_HEADER;
	*sceneFileName = *sceneFileName + currentPlayerTurnString;

	if(currentPhase == GAME_PHASE_MOVEMENT)
	{
		*sceneFileName = *sceneFileName + SCENE_FILE_NAME_PHASE_MOVEMENT;
	}
	else if(currentPhase == GAME_PHASE_LONGDISTANCECOMBAT)
	{
		*sceneFileName = *sceneFileName + SCENE_FILE_NAME_PHASE_LONGDISTANCE;
	}
	else if(currentPhase == GAME_PHASE_CLOSECOMBAT)
	{
		*sceneFileName = *sceneFileName + SCENE_FILE_NAME_PHASE_CLOSECOMBAT;
	}
	else
	{
		result = false;
		cout << "error: generateSceneFileName{} illegal phase " << endl;
	}
	if(phaseExecutionStage ==  GAME_PHASE_EXECUTION_DISPLAY_START)
	{
		*sceneFileName = *sceneFileName + SCENE_FILE_NAME_PHASE_START;
	}
	else if(phaseExecutionStage == GAME_PHASE_EXECUTION_DISPLAY_END)
	{
		*sceneFileName = *sceneFileName + SCENE_FILE_NAME_PHASE_END;
	}
	else if(phaseExecutionStage == GAME_PHASE_EXECUTION_DISPLAY_TARGETSPRITES)
	{
		*sceneFileName = *sceneFileName + SCENE_FILE_NAME_PHASE_TARGETSPRITES;
	}
	else if(phaseExecutionStage == GAME_PHASE_EXECUTION_DISPLAY_RANGESPRITES)
	{
		*sceneFileName = *sceneFileName + SCENE_FILE_NAME_PHASE_RANGESPRITES;
	}
	else
	{
		result = false;
	}
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_EXTENSION;

	return result;
}


void LRRCgameClass::generateXMLNNSceneFileName(const int currentGame, string* sceneFileName, const int nnIndex)
{
	string currentGameString = SHAREDvars.convertIntToString(currentGame);

	string currentNeuralNetworkString = SHAREDvars.convertIntToString(nnIndex);

	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_START;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_GAME_HEADER;
	*sceneFileName = *sceneFileName + currentGameString;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_NEURALNET_HEADER;
	*sceneFileName = *sceneFileName + currentNeuralNetworkString;
	*sceneFileName = *sceneFileName + XML_NN_SCENE_FILE_NAME_EXTENSION;
}


void LRRCgameClass::generateVectorGraphicsLDRNNSceneFileName(const int currentGame, string* sceneFileName, const int nnIndex)
{
	string currentGameString = SHAREDvars.convertIntToString(currentGame);

	string currentNeuralNetworkString = SHAREDvars.convertIntToString(nnIndex);

	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_START;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_GAME_HEADER;
	*sceneFileName = *sceneFileName + currentGameString;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_NEURALNET_HEADER;
	*sceneFileName = *sceneFileName + currentNeuralNetworkString;
	*sceneFileName = *sceneFileName + VECGRAPHICS_LDR_NN_SCENE_FILE_NAME_EXTENSION;
}

void LRRCgameClass::generateVectorGraphicsLDRNNSceneFileNameWithSprites(const int currentGame, string* sceneFileName, const int nnIndex)
{
	string currentGameString = SHAREDvars.convertIntToString(currentGame);

	string currentNeuralNetworkString = SHAREDvars.convertIntToString(nnIndex);

	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_START;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_GAME_HEADER;
	*sceneFileName = *sceneFileName + currentGameString;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_NEURALNET_HEADER;
	*sceneFileName = *sceneFileName + currentNeuralNetworkString;
	*sceneFileName = *sceneFileName + "withSprites";
	*sceneFileName = *sceneFileName + VECGRAPHICS_LDR_NN_SCENE_FILE_NAME_EXTENSION;
}



void LRRCgameClass::generateVectorGraphicsTALNNSceneFileName(const int currentGame, string* sceneFileName, const int nnIndex)
{
	string currentGameString = SHAREDvars.convertIntToString(currentGame);

	string currentNeuralNetworkString = SHAREDvars.convertIntToString(nnIndex);

	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_START;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_GAME_HEADER;
	*sceneFileName = *sceneFileName + currentGameString;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_NEURALNET_HEADER;
	*sceneFileName = *sceneFileName + currentNeuralNetworkString;
	*sceneFileName = *sceneFileName + VECGRAPHICS_TAL_NN_SCENE_FILE_NAME_EXTENSION;
}

void LRRCgameClass::generateRaytracedImagePPMNNSceneFileName(const int currentGame, string* sceneFileName, const int nnIndex)
{
	string currentGameString = SHAREDvars.convertIntToString(currentGame);

	string currentNeuralNetworkString = SHAREDvars.convertIntToString(nnIndex);

	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_START;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_GAME_HEADER;
	*sceneFileName = *sceneFileName + currentGameString;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_NEURALNET_HEADER;
	*sceneFileName = *sceneFileName + currentNeuralNetworkString;
	*sceneFileName = *sceneFileName + RAYTRACEDIMAGE_PPM_NN_SCENE_FILE_NAME_EXTENSION;
}





void LRRCgameClass::generateExperiencesNNSceneFileName(const int currentGame, string* sceneFileName, const int nnIndex)
{
	string currentGameString = SHAREDvars.convertIntToString(currentGame);

	string currentNeuralNetworkString = SHAREDvars.convertIntToString(nnIndex);

	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_START;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_GAME_HEADER;
	*sceneFileName = *sceneFileName + currentGameString;
	*sceneFileName = *sceneFileName + SCENE_FILE_NAME_NEURALNET_HEADER;
	*sceneFileName = *sceneFileName + currentNeuralNetworkString;
	*sceneFileName = *sceneFileName + EXPERIENCES_NN_SCENE_FILE_NAME_EXTENSION;
}


void LRRCgameClass::copyReferencesAndSubmodelDetails(LDreference* referenceNew, LDreference* referenceToCopy, const int type)
{
	referenceNew->type = referenceToCopy->type;
	referenceNew->colour = referenceToCopy->colour;

	if(type == REFERENCE_TYPE_SUBMODEL)
	{
		SHAREDvector.copyVectors(&(referenceNew->relativePosition),  &(referenceToCopy->relativePosition));
		SHAREDvector.copyMatricies(&(referenceNew->deformationMatrix),  &(referenceToCopy->deformationMatrix));
		referenceNew->name = referenceToCopy->name;
		LRRCmodelClass.copyAllUnitDetails(referenceNew->subModelDetails, referenceToCopy->subModelDetails);
	}
	else
	{
		if((type == REFERENCE_TYPE_LINE) || (type == REFERENCE_TYPE_TRI) || (type == REFERENCE_TYPE_QUAD) || (type == REFERENCE_TYPE_OPTIONALLINE))
		{
			SHAREDvector.copyVectors(&(referenceNew->vertex1relativePosition),  &(referenceToCopy->vertex1relativePosition));
			SHAREDvector.copyVectors(&(referenceNew->vertex2relativePosition),  &(referenceToCopy->vertex2relativePosition));
		}
		if((type == REFERENCE_TYPE_TRI) || (type == REFERENCE_TYPE_QUAD) || (type == REFERENCE_TYPE_OPTIONALLINE))
		{
			SHAREDvector.copyVectors(&(referenceNew->vertex3relativePosition),  &(referenceToCopy->vertex3relativePosition));
		}
		if((type == REFERENCE_TYPE_QUAD) || (type == REFERENCE_TYPE_OPTIONALLINE))
		{
			SHAREDvector.copyVectors(&(referenceNew->vertex4relativePosition),  &(referenceToCopy->vertex4relativePosition));
		}
	}
}


#ifdef USE_ANN
void LRRCgameClass::trainAndOutputNeuralNetwork(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, const int numberOfInputNeurons, int numberOfOutputNeurons, ANNexperience* firstExperienceInList, bool addSprites, const bool allowRaytrace, const int nn, const int currentGame)
{

	string* XMLNNSceneFileName = new string();
	generateXMLNNSceneFileName(currentGame, XMLNNSceneFileName, nn);
	cout << "XMLNNSceneFileName = " <<* XMLNNSceneFileName << endl;

	//now output the network to vector graphics file
	string* vectorGraphicsLDRNNSceneFileName = new string();
	generateVectorGraphicsLDRNNSceneFileName(currentGame, vectorGraphicsLDRNNSceneFileName, nn);
	cout << "vectorGraphicsLDRNNSceneFileName = " <<* vectorGraphicsLDRNNSceneFileName << endl;
	char* charstarvectorGraphicsLDRNNSceneFileName = const_cast<char*>(vectorGraphicsLDRNNSceneFileName->c_str());

	string* vectorGraphicsLDRNNSceneFileNameWithSprites = new string();
	generateVectorGraphicsLDRNNSceneFileNameWithSprites(currentGame, vectorGraphicsLDRNNSceneFileNameWithSprites, nn);
	cout << "vectorGraphicsLDRNNSceneFileName = " <<* vectorGraphicsLDRNNSceneFileNameWithSprites << endl;
	char* charstarvectorGraphicsLDRNNSceneFileNameWithSprites = const_cast<char*>(vectorGraphicsLDRNNSceneFileNameWithSprites->c_str());

	//now output the vector graphics file to image file via ray tracer
	string* vectorGraphicsTALNNSceneFileName = new string();
	generateVectorGraphicsTALNNSceneFileName(currentGame, vectorGraphicsTALNNSceneFileName, nn);
	cout << "vectorGraphicsTALNNSceneFileName = " <<* vectorGraphicsTALNNSceneFileName << endl;
	char* charstarvectorGraphicsTALNNSceneFileName = const_cast<char*>(vectorGraphicsTALNNSceneFileName->c_str());

	string* raytracedImagePPMNNSceneFileName = new string();
	generateRaytracedImagePPMNNSceneFileName(currentGame, raytracedImagePPMNNSceneFileName, nn);
	cout << "raytracedImagePPMNNSceneFileName = " <<* raytracedImagePPMNNSceneFileName << endl;
	char* charstarraytracedImagePPMNNSceneFileName = const_cast<char*>(raytracedImagePPMNNSceneFileName->c_str());


	//now output the experiences to experience data set file
	string* experienceNNSceneFileName = new string();
	generateExperiencesNNSceneFileName(currentGame, experienceNNSceneFileName, nn);
	cout << "experienceNNSceneFileName = " <<* experienceNNSceneFileName << endl;
	char* charstarexperienceNNSceneFileName = const_cast<char*>(experienceNNSceneFileName->c_str());

	bool useFoldsDuringTraining;
	int maxOrSetNumEpochs;
	if(nn == GAME_INDEX_OF_OBJECT_EXPERIENCE_NN)
	{
		useFoldsDuringTraining = false;
		maxOrSetNumEpochs = LRRC_OBJECT_RECOGNITION_EXPERIENCES_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS;

	}
	else
	{
		#ifdef LRRC_GAME_DO_NOT_USE_FOLDS_DURING_1DNN_TRAINING
		useFoldsDuringTraining = false;
		maxOrSetNumEpochs = LRRC_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS;
		#else
		useFoldsDuringTraining = true;
		maxOrSetNumEpochs = LRRC_MAX_NUMBER_OF_EPOCHS;
		#endif
	}

	ANNmain.trainAndOutputNeuralNetworkWithFileNames(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, firstExperienceInList, addSprites, allowRaytrace, XMLNNSceneFileName, charstarvectorGraphicsLDRNNSceneFileName, charstarvectorGraphicsTALNNSceneFileName, charstarraytracedImagePPMNNSceneFileName, charstarexperienceNNSceneFileName, useFoldsDuringTraining, maxOrSetNumEpochs);

}
#endif


void LRRCgameClass::updatePlayerStatus(Player* initialPlayerInList)
{
	Player* currentPlayer = initialPlayerInList;

	while(currentPlayer->next != NULL)
	{
		if(!searchUnitListAndCheckThatSomePlayerUnitsAreAlive(currentPlayer->id, initialPlayerInList->firstUnitInUnitList))		//arbitrary player, OLD=defenderPlayer
		{
			currentPlayer->status = false;
		}

		currentPlayer = currentPlayer->next;
	}
}



bool LRRCgameClass::searchUnitListAndCheckThatSomePlayerUnitsAreAlive(const int playerTeam, const UnitListClass* firstUnitInUnitGroup)
{
	bool playerStillHasAUnitAlive = false;

	const UnitListClass* currentUnitInList = firstUnitInUnitGroup;

	while(currentUnitInList->next != NULL)
	{
		if((currentUnitInList->team == playerTeam) && (currentUnitInList->status == ALIVE))
		{
			playerStillHasAUnitAlive = true;
		}

		if(currentUnitInList->isUnitGroup)
		{
			if(searchUnitListAndCheckThatSomePlayerUnitsAreAlive(playerTeam, currentUnitInList->firstUnitInUnitGroup))
			{
				playerStillHasAUnitAlive = true;
			}
		}

		currentUnitInList = currentUnitInList->next;
	}

	return playerStillHasAUnitAlive;

}









/*
-----------------------------------------

Newer Rules

p1mov
	scenefileismodified
p1LDsettingAndOperation [if no movement]
	scenefileismodified
p1CCsettingAndOperation [NB Eg if p1 character x decides to fight p2 character y in close combat, p2 will fight back in CC during p1's phase [as part of standard CC operations], and p2 character y can still move in their movement phase,
	scenefileismodified
p2mov
	scenefileismodified
p2LDsettingAndOperation [if no movement]
	scenefileismodified
p2CCsettingAndOperation
	scenefileismodified
p3mov
	scenefileismodified
p3LDsettingAndOperation [if no movement]
	scenefileismodified
p3CCsettingAndOperation
	scenefileismodified

-----------------------------------------

old2 Rules [doesnt work, if P2 char y knocks P1 char x out in combat - either LD or CC, p3 char z's specified attack against p1 char x will no longer be valid];
	{assume p1 movement phase is before p2 movement phase, if p2 character y has moved longo close combat with p1 character x, p1 character x cannot fire int distance against p2 character y [even if p1 character x has not moved in his movement phase]}
	{however, if in another scenario p2 character y has moved out of range with p1 character x [after p1 character x had already specified to attack int distance], p1 character x can still fire int distance attack}

p1mov
p1LDsetting [if no movement]
p1CCsetting [NB Eg if p1 character x decides to fight p2 character y in close combat, p2 character y cannot move in their movement phase].
	scenefileismodified
p2mov
p2LDsetting [if no movement]
p2CCsetting
	scenefileismodified
p3mov
p3LDsetting [if no movement]
p3CCsetting
	scenefileismodified
...
LDoperations
	scenefileismodified
CCoperations
	scenefileismodified

-----------------------------------------

Old1 Rules:
	{assume p1 movement phase is before p2 movement phase, if p2 character y has moved longo close combat with p1 character x, p1 character x can still fire int distance against p2 character y [assuming p1 character x has not moved in his movement phase]}

p1mov
p1CCinitialsetting [NB Eg if p1 character x decides to fight p2 character y in close combat, p2 character y cannot move in their movement phase].
	scenefileismodified
p2mov
p2CCinitialsetting
	scenefileismodified
p3mov
p3CCinitialsetting
	scenefileismodified
...
p1LDsetting [if no movement]
p2LDsetting [if no movement]
p3LDsetting [if no movement]
LDoperations
	scenefileismodified
...
[NOTREQUIRED: p1CCfinalsetting]
[NOTREQUIRED: p2CCfinalsetting]
[NOTREQUIRED: p3CCfinalsetting]
CCoperations
	scenefileismodified

being first to move in a round gives the ability to lock other players longo CC.
being last to move in a round gives the ability to avoid combat.

-----------------------------------------
*/





