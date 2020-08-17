/*******************************************************************************
 *
 * File Name: LRRCrecordClass.h
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

#ifndef HEADER_LRRC_RECORD_CLASS
#define HEADER_LRRC_RECORD_CLASS

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;

class RecordClass
{
private:
	/*There are currently no private attributes of this class*/
public:

	RecordClass(void); // constructor
	~RecordClass();	// and destructor.

	string name;
	int numberOfThisPartIDInTheUnit;

	RecordClass * next;


};

#endif

