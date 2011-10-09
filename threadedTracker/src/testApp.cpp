#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {
	
	//ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	
	
	TT.setup();
	
#ifndef USE_CANON	
	cam.initGrabber(640, 480);
#else
	camera.setup();
#endif
	
	
	tracker.setup();
	tracker.setRescale(.5);
	
	TT.start();
	
}

void testApp::update() {

#ifndef USE_CANON
	cam.update();
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
	}

#else

	camera.update();
	if(camera.isFrameNew()) {
		tempToBeResized = camera.getLivePixels();
		//float scale = 640.0f / 1056.0f;
		//tempToBeResized.resize(1056*scale, 704*scale);
		
		TT.copyPixels(tempToBeResized);
		
		//tracker.update(toCv(tempToBeResized));
		// process the live view with camera.getLivePixels()
	}
	if(camera.isPhotoNew()) {
		// process the photo with camera.getPhotoPixels()
		// or just save the photo to disk (jpg only):
		camera.savePhoto(ofToString(ofGetFrameNum()) + ".jpg");
	}
#endif

	TT.copyFaceTracker(tracker);

	//printf("- frame - \n");
}

void testApp::draw() {
	ofSetColor(255);
	
	
#ifndef USE_CANON	
	cam.draw(0, 0);
#else
	camera.draw(0, 0);
	// camera.drawPhoto(0, 0, 432, 288);
	
	if(camera.isLiveReady()) {
		stringstream status;
		status << camera.getWidth() << "x" << camera.getHeight() << " @ " <<
		(int) ofGetFrameRate() << " app-fps " << " / " <<
		(int) camera.getFrameRate() << " cam-fps";
		ofDrawBitmapString(status.str(), 10, 20);
	}
#endif
	
	//return;
	//ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	
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
		tracker.reset();
	}
}

void testApp::exit() {
	TT.stop();
	ofSleepMillis(20);
}

