/*
 *  expressionManager.h
 *  BasicExample
 *
 *  Created by zachary lieberman on 10/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"

class expressionManager {
public:
	void setup();
	void update(ofxFaceTracker & tracker);
	void draw();
	
	ExpressionClassifier classifier;
	bool addExpression, addSample, saveData, loadData;
	
	
	
	
};
