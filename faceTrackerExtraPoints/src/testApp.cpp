#include "testApp.h"

using namespace ofxCv;
using namespace cv;

ofMesh convertFromIndices(const ofMesh& mesh) {
	ofMesh result;
	// have to do a const_cast because ofMesh::get*() is not const correct
	ofMesh& cmesh = const_cast<ofMesh&>(mesh);
	int vertices = mesh.getNumVertices();
	int colors = mesh.getNumColors();
	int normals = mesh.getNumNormals();
	int texcoords = mesh.getNumTexCoords();
	int indices = mesh.getNumIndices();
	for(int i = 0; i < indices; i++) {
		int cur = cmesh.getIndex(i);
		if(vertices > 0) {
			result.addVertex(cmesh.getVertex(cur));
		}
		if(colors > 0) {
			result.addColor(cmesh.getColor(cur));
		}
		if(normals > 0) {
			result.addNormal(cmesh.getNormal(cur));
		}
		if(texcoords > 0) {
			result.addTexCoord(cmesh.getTexCoord(cur));
		}
	}
	return result;
}

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	cam.initGrabber(640, 480);
	
	andrea.loadImage("andrea.png");
	tracker.setup();
	tracker.setRescale(.5);
	
	ofBackground(0,0,0);
	tracker.update(toCv(andrea));
	
	
	ofMesh mesh = convertFromIndices(tracker.getMeanObjectMesh());
	for (int i = 0; i < mesh.getVertices().size(); i++){
		mesh.getVertices()[i] *= 10;
	}
	
	// get the holes: 
	ofPolyline leftEye = tracker.getMeanObjectFeature(ofxFaceTracker::LEFT_EYE);
	ofPolyline rightEye = tracker.getMeanObjectFeature(ofxFaceTracker::RIGHT_EYE);
	ofPolyline innerMouth = tracker.getMeanObjectFeature(ofxFaceTracker::INNER_MOUTH);
	
	for (int i = 0; i < leftEye.getVertices().size(); i++){
		leftEye.getVertices()[i].z = 0;
		leftEye.getVertices()[i] *= 10;
	}
	for (int i = 0; i < rightEye.getVertices().size(); i++){
		rightEye.getVertices()[i].z = 0;

		rightEye.getVertices()[i] *= 10;
	}
	for (int i = 0; i < innerMouth.getVertices().size(); i++){
		innerMouth.getVertices()[i].z = 0;
		innerMouth.getVertices()[i] *= 10;
	}
	
	MPA.holes.push_back(rightEye.getVertices());
	MPA.holes.push_back(leftEye.getVertices());
	MPA.holes.push_back(innerMouth.getVertices());		
	
	MPA.setup(mesh);
	
}

void testApp::update() {
	
	cam.update();
	tracker.update(toCv(cam));

	
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
	
	
	ofNoFill();
	ofSetColor(255,0,0);
	
	ofSeedRandom(0);
	if (tracker.getImageMesh().getVertices().size() > 0){
		ofMesh currentMesh = convertFromIndices(tracker.getImageMesh());
		ofMesh temp = MPA.returnMeshWarpedToThisMesh(currentMesh);
		//temp.drawWireframe();
		ofEnableAlphaBlending();
		ofFill();
		for (int i = 0; i < temp.getVertices().size()/3; i++){
			
			ofSetColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255), 70 );
			ofTriangle(temp.getVertices()[i*3],
					   temp.getVertices()[i*3+1],
					   temp.getVertices()[i*3+2]);
			
		}
	}

}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
}