#include "testApp.h"
#include "useful.h"

using namespace ofxCv;

void testApp::setup() {
	
	//ofSetVerticalSync(true);
	
	ofBackground(0,0,0);
	
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	
	
	
	
	TT.setup();
	
	panel.setup("Control Panel", 1024-310, 5, 300, 600);
	panel.addPanel("Cam");
	panel.setWhichPanel("Cam");
	panel.addSlider("camResize", "camResize", 0.5, 0.1, 1.0, false);
		
	
	trackingPixels.allocate(640,480, OF_PIXELS_RGB);
	
	tracker.setup();
	tracker.setRescale(.25);
	tracker.setIterations(20);

	EM.setup();
	
	TT.start();
	
	MMR.setup();
	
	 IM.setup();
	
}

void testApp::update() {

	tracker.setRescale(panel.getValueF("camResize"));
	TT.setScale(panel.getValueF("camResize"));
	
	
	IM.update();
	if (IM.isFrameNew()){
		TT.copyPixels(IM.getTrackingPixels());
		if (TT.nFramesGotOne > 0){
			trackingPixels.setFromExternalPixels(IM.getTrackingPixels(), 640, 480,3);
			tracker.update(ofxCv::toCv(trackingPixels));
			
		}
	} else {
		
	}
	//TT.copyFaceTracker(tracker);
	
	EM.update(tracker);	
	
	MMR.clear();


}

void testApp::draw() {
	
	ofSetColor(255);
	IM.draw();
	
	ofSetLineWidth(1);
	tracker.draw();
	
	ofPolyline leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
	ofPolyline rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
	
	//ofSetLineWidth(2);
	//ofSetColor(ofColor::red);
	//leftEye.draw();
	//ofSetColor(ofColor::green);
	//rightEye.draw();
	
	
	EM.draw();
	
	//MMR.draw();
	ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	MMR.draw(tracker);
	ofDisableBlendMode();
	
	
	
	/*ofMesh temp = tracker.getImageMesh();
	addForheadToFaceMesh(temp);
	temp.drawWireframe(); //.draw();
	*/
	
	
}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		TT.setReset();
		tracker.reset();
		EM.classifier.reset();
	}
	if (key == ' '){
		IM.bSaveImages = !IM.bSaveImages;	
	}
	
	if(key == 'e') {
		EM.addExpression = true;
	}
	if(key == 'a') {
		EM.addSample = true;
	}
	if(key == 's') {
		EM.saveData = true;
	}
	if(key == 'l') {
		EM.loadData = true;
	}
}

void testApp::exit() {
	TT.stop();
	ofSleepMillis(50);
}

