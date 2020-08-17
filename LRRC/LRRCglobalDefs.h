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
 * File Name: LRRCglobalDefs.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Lego Rules CG Rounds Checker
 * Project Version: 3j1b 14-January-2017
 * Description: LRRC specific global definitions
 *
 *******************************************************************************/


#ifndef HEADER_LRRC_GLOBAL_DEFS
#define HEADER_LRRC_GLOBAL_DEFS

#include "SHAREDglobalDefs.h"


#define DEBUG_IN_TEXTPAD
//#define DEBUG_SPRITES
//#define DEBUG_ADD_INDIVIDUAL_SPRITES

#ifdef USE_ANN

	#ifdef TH_USE_ANN_TEST_HARNESS

		//#define DEMONSTRATION_SCENARIO1		//combat/properties experience testing	//LRRCTHgame.cpp + LRRCTHgame.h
		#define DEMONSTRATION_SCENARIO2			//object experience testing		//LRRCTHgame.cpp + LRRCTHgame.h
		//#define DEMONSTRATION_SCENARIO3		//contains functions not yet tested	//LRRCTHgame.cpp + LRRCTHgame.h


			//Primary variables been tested by TH;
		#ifdef DEMONSTRATION_SCENARIO1
			//#define TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
				//#define TH_USE_POVRAY_FOR_OBJECT_RECOGNITION_VEC_GRAPHICS
				//#define TH_USE_RT_FOR_OBJECT_RECOGNITION_VEC_GRAPHICS
			#define TH_LRRC_GAME_USE_CC_EXPERIENCES
			#define TH_LRRC_GAME_USE_LD_EXPERIENCES
			//#define TH_LRRC_GAME_USE_GLOBAL_EXPERIENCES
			#define TH_GAME_RAY_TRACE_NEURAL_NETWORK_VEC_GRAPHICS

		#elif defined DEMONSTRATION_SCENARIO2
			#define TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
				#define TH_USE_POVRAY_FOR_OBJECT_RECOGNITION_VEC_GRAPHICS
				//#define TH_USE_RT_FOR_OBJECT_RECOGNITION_VEC_GRAPHICS
			//#define TH_LRRC_GAME_USE_CC_EXPERIENCES
			//#define TH_LRRC_GAME_USE_LD_EXPERIENCES
			//#define TH_LRRC_GAME_USE_GLOBAL_EXPERIENCES
			#define TH_GAME_RAY_TRACE_NEURAL_NETWORK_VEC_GRAPHICS

		#elif defined DEMONSTRATION_SCENARIO3
			#define TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
				//#define TH_USE_POVRAY_FOR_OBJECT_RECOGNITION_VEC_GRAPHICS
				#define TH_USE_RT_FOR_OBJECT_RECOGNITION_VEC_GRAPHICS
			//#define TH_LRRC_GAME_USE_CC_EXPERIENCES
			//#define TH_LRRC_GAME_USE_LD_EXPERIENCES
			//#define TH_LRRC_GAME_USE_GLOBAL_EXPERIENCES
			#define TH_GAME_RAY_TRACE_NEURAL_NETWORK_VEC_GRAPHICS

		#else
			//#define TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES
				//#define TH_USE_POVRAY_FOR_OBJECT_RECOGNITION_VEC_GRAPHICS
				//#define TH_USE_RT_FOR_OBJECT_RECOGNITION_VEC_GRAPHICS
			#define TH_LRRC_GAME_USE_CC_EXPERIENCES
			#define TH_LRRC_GAME_USE_LD_EXPERIENCES
			//#define TH_LRRC_GAME_USE_GLOBAL_EXPERIENCES
			//#define TH_GAME_RAY_TRACE_NEURAL_NETWORK_VEC_GRAPHICS

		#endif

			//conditions required for TH
		#ifndef TH_LRRC_GAME_USE_GLOBAL_EXPERIENCES
			#define DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES
		#endif
		#define DEBUG_DO_NOT_PERFORM_KILL_RATIO_CHECKS_BEFORE_ADDING_UNIT_EXPERIENCES_TO_NN
		#define DEBUG_DO_NOT_DISPLAY_COMBAT_RESULTS
		#define DEBUG_STORE_EXPERIENCES_IN_PLAYER_NOT_UNITS
		#define DEBUG_HAVNT_YET_STARTED_TESTING_UNITGROUPS


			//variables no longer changed
		#ifndef DEMONSTRATION_SCENARIO1
			#define TH_LRRC_GAME_EMULATE_GAME_AI_REQUIREMENTS			//!! //Default=defined	[will use randomised distances if defined]
		#endif
			#define DEBUG_DO_NOT_USE_RANDOM_DICE_DURING_COMBAT


			//Old tests no longer changed
		//#define ANN_SEPARATE_CC_FROM_LD_NETWORK		//Default=not defined

		/*TEMP DEBUG PERIOD SHOULD NEVER BE REQUIRED AGAIN;
		//#define DEGDISREGARD_HIGH_ERROR_EXPERIENCES
			//TEMP!
			//#define TEMP_OUTPUT_HIGH_ERROR_EXPERIENCES				//ANNTraining.cpp
		*/

		/*LOW RAM ALGORITHM DOES NOT WORK YET;
		//#define TH_LRRC_LOW_RAM_AVAILABLE	//default= not defined			//ANNdisplay.cpp
		#ifdef TH_LRRC_LOW_RAM_AVAILABLE
			#define TH_LRRC_LOW_RAM_NUM_CONSEC_TRAINING_EPOCHS_PER_EXPERIENCE 50
		#endif
		*/

	#endif

	//#define DEBUG_DO_NOT_USE_ADVANCED_LEARNING_BASED_UPON_DISTANCES			//LRRCgame.cpp
	#ifdef DEBUG_DO_NOT_USE_ADVANCED_LEARNING_BASED_UPON_DISTANCES
		#define DEBUG_DO_NOT_USE_GLOBAL_EXPERIENCES					//LRRCgameAI.cpp / LRRCgame.cpp / LRRCgameAI.h / LRRCunitClass.h:
	#endif

	#ifdef TH_GAME_USE_OBJECT_RECOGNITION_EXPERIENCES					//LRRCgameAI.h + LRRCgameAI.cpp + LRRCTHgame.cpp
		#define DEBUG_ANN_OBJECT_RECOGNITION_EXPERIENCES_OUTPUT_ALL_MAPS		//LRRCgameAI.cpp {OLD: + ORTHimageCategorisationNN.cpp}
		#define USE_HI_RES_PRIMITIVE_REPLACEMENT_DURING_RENDERING			//ANNexperienceClass.h
		//#define ANN_OBJECT_RECOGNITION_EXPERIENCES_RANDOMISE_LIGHTING_CONDITIONS	//LRRCgameAI.cpp	//not available yet

		//#define GAME_OBJECT_RECOGNITION_EXPERIENCE_FEED_RGB_MAP			//LRRCgameAI.cpp
		//#define GAME_OBJECT_RECOGNITION_EXPERIENCE_FEED_LUMINOSITY_MAP		//LRRCgameAI.cpp
		//#define GAME_OBJECT_RECOGNITION_EXPERIENCE_FEED_LUMINOSITY_BOOLEAN_MAP	//LRRCgameAI.cpp
		//#define GAME_OBJECT_RECOGNITION_EXPERIENCE_LUMINOSITY_CONTRAST_MAP		//LRRCgameAI.cpp
		#define GAME_OBJECT_RECOGNITION_EXPERIENCE_LUMINOSITY_CONTRAST_BOOLEAN_MAP	/LRRCgameAI.cpp
	#endif

	//#define LRRC_GAME_DO_NOT_USE_FOLDS_DURING_1DNN_TRAINING				//LRRCgame.cpp {with USE_ANN}
	//Debug variables
	//Program Type variables

	//#define DEBUG_NO_UNITGROUP_AI		//LRRCgame.cpp
	//#define DEBUG_DO_NOT_PERFORM_KILL_RATIO_CHECKS_BEFORE_ADDING_UNIT_EXPERIENCES_TO_NN	//LRRCgame.cpp / LRRCgameAI.cpp
	//#define DEBUG_NO_CREATIVITY_ALWAYS_PERFORM_COMBAT_WITH_LOWEST_ERROR_UNIT		//LRRCgame.cpp
	//#define DEBUG_NO_CREATIVITY_ALWAYS_PERFORM_MOVEMENT_INSTEAD_OF_STAND_FOR_LDCOMBAT	//LRRCgame.cpp
#endif


#endif
