#include "testApp.h"
#include "useful.h"

using namespace ofxCv;

void testApp::setup() {
	
	//ofSetVerticalSync(true);
	
	ofBackground(0,0,0);
	
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	
	ofSetFrameRate(100);
	
	// for audio
	ofxDaito::setup("oscsetting.xml");	
	
	TT.setup();
	
	panel.setup("Control Panel", 1024-310, 5, 300, 600);
	panel.addPanel("Cam");
	panel.setWhichPanel("Cam");
	panel.addSlider("camResize", "camResize", 0.5, 0.1, 1.0, false);
		
	
	trackingPixels.allocate(640,480, OF_PIXELS_RGB);
	
	tracker.setup();
	tracker.setRescale(.25);
	tracker.setIterations(20);

	
	MMR.setup();
	IM.setup();
	MPA.setup();
	MMR.setupMPAMesh();
	
	
	EM.setup();
	TT.start();
	ES.setup();
	ES.tracker = &tracker;
	LS.setup();
	TS.setup(640,480); // needs to match the input resolution
	
	whoToDraw = 0;
		
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

	ofMesh temp;
	temp = tracker.getImageMesh();
	LS.update(temp);
	TS.update(tracker, IM.img.getTextureReference());
	ES.update();
	RS.update(mouseY > 300 ? true : false, getCentroid2D(tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH)));
	//RS.update(true, // need to switch this for a detector based on thresholding the mouth openness
	// tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH).getCentroid2D());
}

void testApp::draw() {
	
	ofSetColor(255);
	IM.draw();
	
	ofSetLineWidth(1);
	//tracker.draw();
	
	EM.draw();
	
	//MMR.draw();
	
	// --------------------------------------------- draw the MMR on the face
	/*ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	MMR.draw(tracker);
	ofDisableBlendMode();
	*/
	
	
	// --------------------------------------------- show how foreheads work
	/*ofMesh temp = tracker.getImageMesh();
	addForheadToFaceMesh(temp);
	temp.drawWireframe(); //.draw();
	*/
	
	
	// --------------------------------------------- see MPA in action (colored triangles)
	//ofSeedRandom(0);
//	ofMesh temp = tracker.getImageMesh();
//	//cout << temp.getVertices().size() << endl;
//	if (temp.getVertices().size() > 0){
//		addForheadToFaceMesh(temp);
//		ofMesh currentMesh	= convertFromIndices(temp);
//		ofMesh temp2		= MPA.returnMeshWarpedToThisMesh(currentMesh);
//		//temp.drawWireframe();
//		ofEnableAlphaBlending();
//		ofFill();
//		for (int i = 0; i < temp2.getVertices().size()/3; i++){
//			
//			ofSetColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255), 70 );
//			ofTriangle(temp2.getVertices()[i*3],
//					   temp2.getVertices()[i*3+1],
//					   temp2.getVertices()[i*3+2]);
//			
//		}
//		
//		
//	}
	
	ofDisableBlendMode();
	ofDisableAlphaBlending();
	ofSetColor(255,255,255);
	// --------------------------------------------- the light scene
	if (whoToDraw == 0) TS.draw();
	if (whoToDraw == 1) RS.draw();
	if (whoToDraw == 2) ES.draw(640,480);
	if (whoToDraw == 3) LS.draw(640,480);
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
	
	if (key == OF_KEY_LEFT){
		
		whoToDraw--;
		if (whoToDraw < 0) whoToDraw += 4;
		ofxDaito::bang("scene", whoToDraw);
	} else if (key == OF_KEY_RIGHT){
		whoToDraw++;
		whoToDraw %= 4;
		ofxDaito::bang("scene", whoToDraw);
	}
}

void testApp::exit() {
	TT.stop();
	ofSleepMillis(50);
}

