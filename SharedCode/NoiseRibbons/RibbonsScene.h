#pragma once

#include "NoiseParticle.h"

// the particle system is based on 0,0 as the center of the screen
// so you might need to apply an offset when sending the mouth position

class RibbonsScene {
public:
	void setup() {
	}
	void update(bool mouthIsOpen, ofVec2f mouthPosition = ofVec2f()) {
		if(mouthIsOpen) {
			NoiseParticle::setLeaving(true);
			NoiseParticle::setup(10, ofVec2f(mouthPosition.x - ofGetWidth() / 2, mouthPosition.y - ofGetHeight() / 2), 1);
			NoiseParticle::addForce(ofVec2f(mouthPosition.x - ofGetWidth() / 2, mouthPosition.y - ofGetHeight() / 2), 3);
		}
		NoiseParticle::updateAll(60 * ofGetLastFrameTime());
	}
	void draw() {
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
		
		ofEnableSmoothing();
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofSetLineWidth(4);
		NoiseParticle::drawAllLines();
		ofPopMatrix();
	}
};