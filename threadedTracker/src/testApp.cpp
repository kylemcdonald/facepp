#include "testApp.h"
#include "useful.h"

using namespace ofxCv;

void testApp::setup() {
	
	//ofSetVerticalSync(true);

	
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	
	
	TT.setup();
	
	panel.setup("Control Panel", 1024-310, 5, 300, 600);
	panel.addPanel("Cam");
	panel.setWhichPanel("Cam");
	panel.addSlider("camResize", "camResize", 0.5, 0.1, 1.0, false);
		
	
	IM.setup();
	
	tracker.setup();
	tracker.setRescale(.25);
	tracker.setIterations(20);

	
	TT.start();
	
}

void testApp::update() {

	tracker.setRescale(panel.getValueF("camResize"));
	TT.setScale(panel.getValueF("camResize"));
	
	
	IM.update();
	if (IM.isFrameNew()){
		TT.copyPixels(IM.getTrackingPixels());
	}
	TT.copyFaceTracker(tracker);

}

void testApp::draw() {
	
	ofSetColor(255);
	IM.draw();
	
	ofSetLineWidth(1);
	tracker.draw();
	
	ofPolyline leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
	ofPolyline rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
	
	ofSetLineWidth(2);
	ofSetColor(ofColor::red);
	leftEye.draw();
	ofSetColor(ofColor::green);
	rightEye.draw();
	
	
}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		TT.setReset();
	}
	if (key == ' '){
		IM.bSaveImages = !IM.bSaveImages;	
	}
}

void testApp::exit() {
	TT.stop();
	ofSleepMillis(20);
}

