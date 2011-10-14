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
	panel.hide();
	
	
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
	
	whoToDraw = 1;
		
	//trackingCopy = new unsigned char [640*480*3];
	colorImg.allocate(640,480);
	maskArea.allocate(640,480);
	colorImg2.allocate(640,480, OF_IMAGE_COLOR);
	
	//ofToggleFullscreen();
	ofHideCursor();
	
	lastChange = ofGetElapsedTimef();
	
	// display something on no one present
	nonFaceFoundEnergy = 0;
	nFramesNotFoundFace = 0;
	logo.loadImage("images/logo.png");
	
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
	
	if (tracker.getFound() == true){
		nFramesNotFoundFace=0;
		nonFaceFoundEnergy = 0.8f * nonFaceFoundEnergy + 0.2 * 0.0;	
	} else {
		nFramesNotFoundFace++;
		if (nFramesNotFoundFace > 40){
			nonFaceFoundEnergy = 0.995f * nonFaceFoundEnergy + 0.005 * 1.0;	
		}
		
	}
	
	
	//TT.copyFaceTracker(tracker);
	
	EM.update(tracker);	
	
	//MMR.clear();

	ofMesh temp;
	temp = tracker.getImageMesh();
	
	
	
	if (whoToDraw == 3) LS.update(temp);
	if (whoToDraw == 2) ES.update();
	
	// float mouth: 
	//cout << " m w " << tracker.getGesture(ofxFaceTracker::MOUTH_WIDTH) << endl;
	//cout << " m h " << tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT) << endl;
	
	//cout << " h / w " << tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT) / tracker.getGesture(ofxFaceTracker::MOUTH_WIDTH)<< endl;
	bool bMouth = false;
	if (tracker.getFound()){
		
		
	//cout << " h / w " << tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT) / tracker.getGesture(ofxFaceTracker::MOUTH_WIDTH)<< endl;	
	if ((tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT) / tracker.getGesture(ofxFaceTracker::MOUTH_WIDTH)) > 0.13){
		bMouth = true;
	}
	}
	
	//cout << bMouth << endl;
	if (whoToDraw == 1) RS.update(bMouth, getCentroid2D(tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH)));
	//RS.update(true, // need to switch this for a detector based on thresholding the mouth openness
	// tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH).getCentroid2D());
	
	
	if (whoToDraw == 0) {
		
		TS.generateMaskTex(tracker, IM.getTrackingPixels());
		
		if (tracker.getFound()){
		
			// all cyclops related, and didn't work so great with the actual camera. 
			// looks pretty ok against the test video
			
		//static int pts[13] = {1,0,17,18,19,20,23,24,25,26,16,15,28};
//		ofPoint min, max;
//		min.set(640,480,0);
//		max.set(0,0,0);
//		for (int i = 0; i < 13; i++){
//			min.x = MIN(min.x, tracker.getImagePoint(pts[i]).x);
//			min.y = MIN(min.y, tracker.getImagePoint(pts[i]).y);
//			max.x = MAX(max.x, tracker.getImagePoint(pts[i]).x);
//			max.y = MAX(max.y, tracker.getImagePoint(pts[i]).y);
//		}
//		
//		maskArea.set(0);
//		ofxCvBlob blob;
//		for (int i = 0; i < 13; i++){
//			blob.pts.push_back(	ofPoint(tracker.getImagePoint(pts[i]).x, tracker.getImagePoint(pts[i]).y+3));
//		}
//		blob.nPts = 13;
//		maskArea.drawBlobIntoMe(blob, 255);
//			
//		//maskArea.setROI(min.x, min.y, max.x - min.x, max.y-min.y);
//		maskArea.blur(11);
//		maskArea.blur(3);	
//		maskArea.blur(3);	
//			
//		colorImg.setFromPixels(IM.getTrackingPixels(), 640, 480);
//		colorImg.setROI(min.x-2, min.y-2, max.x - min.x+4, max.y-min.y+4);
//		colorImg.blur(11);
//		colorImg.blur(11);
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.dilate();
//		colorImg.blur(11);
//		colorImg.blur(11);
//		colorImg.blur(11);
//		colorImg.blur(11);
//		
//		colorImg2.setFromPixels(IM.getTrackingPixels(), 640, 480, OF_IMAGE_COLOR);
//		
//			unsigned char * colPixes = colorImg.getPixels();
//			unsigned char * colPixes2 = colorImg2.getPixels();
//			unsigned char * grayPixels = maskArea.getPixels();
//			for (int i = 0; i < 640*480; i++){
//				if (grayPixels[i] > 0){
//					float pct = (float)grayPixels[i]/255.0;
//					colPixes2[i*3+0] = pct * colPixes[i*3+0] + (1-pct) * colPixes2[i*3+0];
//					colPixes2[i*3+1] = pct * colPixes[i*3+1] + (1-pct) * colPixes2[i*3+1];
//					colPixes2[i*3+2] = pct * colPixes[i*3+2] + (1-pct) * colPixes2[i*3+2];
//					
//				}
//			}
//			
//			colorImg2.update();
//			
//		} else {
//			colorImg2.setFromPixels(IM.getTrackingPixels(), 640, 480, OF_IMAGE_COLOR);
//		}
		
			colorImg2.setFromPixels(IM.getTrackingPixels(), 640, 480, OF_IMAGE_COLOR);
			TS.texy = &colorImg2.getTextureReference();
			
		}
		
		TS.update(tracker, IM.img.getTextureReference());
	}
	
	if ((ofGetElapsedTimef() - lastChange)  > 51){
		//keyPressed(OF_KEY_RIGHT);
		//lastChange = ofGetElapsedTimef();
	}
	
// for audio
		ofxDaito::clearBundle();
	
	if(tracker.getFound()) {
		ofxDaito::addMessage("/found", 1);
		
		ofVec2f position = tracker.getPosition();
		ofxDaito::addMessage("/pose/position", position);
		float scale = tracker.getScale();
		ofxDaito::addMessage("/pose/scale", scale);
		ofVec3f orientation = tracker.getOrientation();
		ofxDaito::addMessage("/pose/orientation", orientation);
		ofxDaito::addMessage("/gesture/mouth/width", tracker.getGesture(ofxFaceTracker::MOUTH_WIDTH));
		ofxDaito::addMessage("/gesture/mouth/height", tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT));
		ofxDaito::addMessage("/gesture/eyebrow/left", tracker.getGesture(ofxFaceTracker::LEFT_EYEBROW_HEIGHT));
		ofxDaito::addMessage("/gesture/eyebrow/right", tracker.getGesture(ofxFaceTracker::RIGHT_EYEBROW_HEIGHT));
		ofxDaito::addMessage("/gesture/eye/left", tracker.getGesture(ofxFaceTracker::LEFT_EYE_OPENNESS));
		ofxDaito::addMessage("/gesture/eye/right", tracker.getGesture(ofxFaceTracker::RIGHT_EYE_OPENNESS));
		ofxDaito::addMessage("/gesture/jaw", tracker.getGesture(ofxFaceTracker::JAW_OPENNESS));
		ofxDaito::addMessage("/gesture/nostrils", tracker.getGesture(ofxFaceTracker::NOSTRIL_FLARE));
	} else {
		ofxDaito::addMessage("/found", 0);
	}

	ofxDaito::sendBundle();
}

void testApp::draw() {
	
	
	//tracker.draw();
	
	//EM.draw();
	
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
	
	ofPushMatrix();
	ofTranslate(1024, 0,0);
	ofScale(-1,1,1);
	
	ofScale(1024/640.0, 768/480.0, 1);
	
	
	ofSetColor(255);
	IM.draw();
	
	ofSetLineWidth(1);
	
	// --------------------------------------------- the light scene
	if (whoToDraw == 0) TS.draw();
	if (whoToDraw == 1) RS.draw();
	if (whoToDraw == 2) ES.draw(640,480);
	if (whoToDraw == 3) LS.draw(640,480);
	
	ofPopMatrix();
	
	if (nonFaceFoundEnergy > 0.01){
	
		ofEnableAlphaBlending();
		ofSetColor(255,255,255,255*nonFaceFoundEnergy);
		logo.draw(21, 548);
		//cout << mouseX << " " << mouseY << endl;
	}
	
	//colorImg2.draw(mouseX, mouseY);
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
		//if (whoToDraw == 0) keyPressed(OF_KEY_LEFT);
		if (whoToDraw == 3) keyPressed(OF_KEY_LEFT);
		ofxDaito::bang("scene", whoToDraw);
		
	} else if (key == OF_KEY_RIGHT){
		whoToDraw++;
		whoToDraw %= 4;
		//if (whoToDraw == 0) keyPressed(OF_KEY_RIGHT);
		if (whoToDraw == 3) keyPressed(OF_KEY_RIGHT);
		ofxDaito::bang("scene", whoToDraw);

	}
	
	if (key == 'f'){
		ofToggleFullscreen();
		
	} 
}

void testApp::exit() {
	TT.stop();
	ofSleepMillis(50);
}

