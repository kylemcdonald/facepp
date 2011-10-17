#include "ofApp.h"

float scale = 4;
		
void drawRing(ofPoint pos, float rad, bool bInward, float startAngle, int ndivisions) {
	float angle = 0;
	float angle2;
	float radius = (rad/4) * scale;
	float ndiv = ndivisions;
	float ndiv_m_1 = ndiv - 1;
	for (int i = 0; i < (ndivisions); i++){
		angle = startAngle + ((float)i/(ndiv)) * TWO_PI;
		angle2 = angle + ((float)1.0/(ndiv)) * TWO_PI;
		ofPoint ptA, ptB, ptC;
		float angle3 = (angle+angle2)/2.0;
		ptA.set(pos.x + radius * cos(angle), pos.y + radius * sin(angle));
		ptB.set(pos.x + radius * cos(angle2), pos.y + radius * sin(angle2));
		float dist = (ptA-ptB).length();
		ptC.set(pos.x + (radius + (bInward ? -1 : 1) * (sqrt(3)/2.0)*dist) * cos(angle3), 
				pos.y + (radius +  (bInward ? -1 : 1) * (sqrt(3)/2.0)*dist) * sin(angle3));
		ofTriangle(ptA, ptB, ptC);
	}
}

void drawMandala(ofVec2f position, float radius) {
	ofSetColor(255, 48);
	drawRing(position, 130, false, ofGetElapsedTimef()/2, 40);
	drawRing(position, 128, true, -ofGetElapsedTimef()/2, 40);
	drawRing(position, 110, true, -(ofGetElapsedTimef()/3.0), 60);
}

void ofApp::setup() {
}

void ofApp::update() {
}

void ofApp::draw() {
	ofBackground(0);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	drawMandala(ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2), mouseX);
}
