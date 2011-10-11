/*
 *  expressionManager.cpp
 *  BasicExample
 *
 *  Created by zachary lieberman on 10/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "expressionManager.h"


void expressionManager::setup(){
	// expressions
	addExpression = false;
	addSample = false;
	saveData = false;
	loadData = false;	
	
}


void expressionManager::update(ofxFaceTracker & tracker){
	
	if (tracker.getFound()){
		
		classifier.classify(tracker);
		if(addExpression) {
			addExpression = false;
			classifier.addExpression();
		}			
		if(addSample) {
			addSample = false;
			classifier.addSample(tracker);
		}				
		if(loadData) {
			loadData = false;
			classifier.load("expressions");
		}			
		if(saveData) {
			saveData = false;
			classifier.save("expressions");
		}
	}
	
}
void expressionManager::draw(){
	int w = 100, h = 12;
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(5, 10);
	int n = classifier.size();
	int primary = classifier.getPrimaryExpression();
	for(int i = 0; i < n; i++){
		ofSetColor(i == primary ? ofColor::red : ofColor::black);
		ofRect(0, 0, w * classifier.getProbability(i) + .5, h);
		ofSetColor(255);
		ofDrawBitmapString(classifier.getDescription(i), 5, 9);
		ofTranslate(0, h + 5);
	}
	ofPopMatrix();
	ofPopStyle();
	
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 20, ofGetHeight() - 10);
	ofDrawBitmapString(
					   string() +
					   "r - reset\n" +
					   "e - add expression\n" +
					   "a - add sample\n" +
					   "s - save expressions\n"
					   "l - load expressions",
					   14, ofGetHeight() - 6 * 12);	
}
	
