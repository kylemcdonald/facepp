#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
#include "ofxEdsdk.h"
#include "threadedTracker.h"

#define USE_CANON




class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void exit();
	
#ifndef USE_CANON
	ofVideoGrabber cam;
#else
	ofxEdsdk::Camera camera;
	ofPixels tempToBeResized;
#endif
	
	threadedTracker TT;
	ofxFaceTracker tracker;
};
