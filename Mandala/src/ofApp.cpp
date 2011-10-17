#include "ofApp.h"

float scale = 4;
		
void drawTriangleRing(float radius, float theta, float height, bool direction, int count) {
	radius *= scale / 4;
	float angle = 0;
	float angle2;
	height *= (sqrt(3) / 2) * (direction ? -1 : 1);
	for (int i = 0; i < (count); i++){
		angle = theta + (i * TWO_PI) / count;
		angle2 = angle + TWO_PI / count;
		ofPoint ptA, ptB, ptC;
		float angle3 = (angle+angle2)/2.0;
		ptA.set(radius * cos(angle), radius * sin(angle));
		ptB.set(radius * cos(angle2), radius * sin(angle2));
		float dist = (ptA-ptB).length();
		ptC.set((radius + height * dist) * cos(angle3), 
				(radius +  height * dist) * sin(angle3));
		ofTriangle(ptA, ptB, ptC);
	}
}

void drawMandala(ofVec2f position, float radius) {
	ofSetColor(255, 128);
	ofTranslate(position.x, position.y);
	drawTriangleRing(130, -(ofGetElapsedTimef()/8.0), 8, false, 80);
	drawTriangleRing(130, ofGetElapsedTimef()/2, 1, false, 40);
	drawTriangleRing(128, -ofGetElapsedTimef()/2, 1, true, 40);
	drawTriangleRing(110, -(ofGetElapsedTimef()/3.0), 1, true, 60);
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
