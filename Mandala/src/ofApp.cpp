#include "ofApp.h"

float scale = 4;
		
void drawTriangleRing(float radius, float theta, float height, bool direction, int count) {
	radius *= scale / 4;
	height *= (sqrt(3) / 2) * (direction ? -1 : 1);
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	for (int i = 0; i < (count); i++){
		float angle1 = theta + (i * TWO_PI) / count;
		float angle2 = angle1 + TWO_PI / count;
		float angle3 = (angle1 + angle2)/2.0;
		ofVec2f pt1(radius * cos(angle1), radius * sin(angle1));
		ofVec2f pt2(radius * cos(angle2), radius * sin(angle2));
		float side = pt1.distance(pt2);
		ofVec2f pt3((radius + height * side) * cos(angle3), 
				(radius +  height * side) * sin(angle3));
		mesh.addVertex(pt1);
		mesh.addVertex(pt2);
		mesh.addVertex(pt3);
	}
	mesh.draw();
}

void ofApp::drawMandala(ofVec2f position, float radius) {
	ofTranslate(position.x, position.y);
	
	ofColor cyan(0, 174, 239), magenta(236, 0, 140), yellow(255, 242, 0);
	
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	float pulseTheta = ofGetElapsedTimef() / 64;
	float pulseAlpha = 64;
	float pulseLength = 16;
	ofSetColor(cyan, pulseAlpha);
	drawTriangleRing(110, pulseTheta * 5, pulseLength, false, 79);
	ofSetColor(magenta, pulseAlpha);
	drawTriangleRing(120, pulseTheta * 7, pulseLength, false, 83);
	ofSetColor(yellow, pulseAlpha);
	drawTriangleRing(130, pulseTheta * 11, pulseLength, false, 89);
	ofPopStyle();
	
	ofSetColor(255, 128);
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
