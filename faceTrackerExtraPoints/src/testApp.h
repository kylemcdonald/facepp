#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "meshPointAdder.h"
#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofxFaceTracker tracker;
	ofImage andrea;
	meshPointAdder MPA;
	ofVideoGrabber cam;
	
	
};
