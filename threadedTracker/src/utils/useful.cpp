/*
 *  useful.cpp
 *  BasicExample
 *
 *  Created by zachary lieberman on 10/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "useful.h"

string ZeroPadNumber(int num){
	stringstream ss;
	// the number is converted to string with the help of stringstream
	ss << num; 
	string ret;
	ss >> ret;
	// Append zero chars
	int str_length = ret.length();
	for (int i = 0; i < 7 - str_length; i++)
		ret = "0" + ret;
	return ret;
}