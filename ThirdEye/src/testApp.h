#pragma once

#include "ofMain.h"
#include "Clone.h"

#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	Clone clone;
	
	ofFbo mask;
	ofFbo src;
	
	ofxFaceTracker tracker;
	ofVideoGrabber cam;
	
	float thirdHeight, thirdMaskSize, thirdSize;
};
