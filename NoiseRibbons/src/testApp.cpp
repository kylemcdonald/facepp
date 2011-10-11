#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
}

void testApp::update() {
	if(ofGetMousePressed()) {
		Particle::setLeaving(true);
		Particle::setup(10, ofVec2f(mouseX - ofGetWidth() / 2, mouseY - ofGetHeight() / 2), 1);
		Particle::addForce(ofVec2f(mouseX - ofGetWidth() / 2, mouseY - ofGetHeight() / 2), 10);
	}
	Particle::updateAll(60 * ofGetLastFrameTime());
}

void testApp::draw() {
	ofBackground(0);
	
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	
	ofEnableSmoothing();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetLineWidth(4);
	Particle::drawAllLines();
	ofPopMatrix();
	
	ofSetColor(0);
	ofRect(0, 0, 80, 40);
	ofSetColor(255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}

void testApp::keyPressed(int key) {
	if(key == 'f')
		ofToggleFullscreen();
}