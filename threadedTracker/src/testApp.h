#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
#include "ofxEdsdk.h"
#include "threadedTracker.h"

//#define USE_CANON

#include "ofxAutoControlPanel.h"
#include "inputManager.h"
#include "expressionManager.h"
#include "meanMeshRenderer.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void exit();
	
	inputManager IM;
	threadedTracker TT;
	ofxFaceTracker tracker;
	ofxAutoControlPanel panel;
	
	expressionManager EM;
	
	meanMeshRenderer MMR;
	
	
	
};
