#include "ofApp.h"

void ofApp::setup() {
}

void ofApp::update() {
}

void ofApp::draw() {
	ofBackground(0);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	mandala.drawMandala(ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2));
	ofSetColor(255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " " + ofToString(mouseX), 10, 20);
}
