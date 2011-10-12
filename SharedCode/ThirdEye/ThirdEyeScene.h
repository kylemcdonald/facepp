#pragma once

#include "ofMain.h"
#include "Clone.h"

class ThirdEyeScene {
public:
	
	Clone clone;
	ofFbo mask, src;
	float thirdHeight, thirdMaskSize, thirdSize, strength;
	
	void setup(int width, int height) {
		
		
		clone.setup(width, height);
		
		ofFbo::Settings settings;
		settings.width = width;
		settings.height = height;
		mask.allocate(settings);
		src.allocate(settings);
		
		thirdHeight = .15;
		thirdMaskSize = .4;
		thirdSize = .8;
		strength = 100;
	}
	
	void update(ofxFaceTracker& tracker, ofTexture& cam) {
		int inner[] = {36, 36, 37, 37, 38, 38, 39, 39, 39, 39, 40, 40, 40, 41, 41, 36};
		int outer[] = { 0, 17, 17, 18, 18, 19, 20, 20, 21, 27, 28, 29,  2,  2,  1,  1};
		int ringPoints = 16;
		
		// third eye location is above the center of the line between the left and right eyebrows	
		// and along a line from the mouth to this point, scaled by the length of that line
		ofVec2f leftEyebrowCenter = getCentroid2D(tracker.getImageFeature(ofxFaceTracker::LEFT_EYEBROW));
		ofVec2f rightEyebrowCenter = getCentroid2D(tracker.getImageFeature(ofxFaceTracker::RIGHT_EYEBROW));
		ofVec2f thirdBase = (leftEyebrowCenter + rightEyebrowCenter) / 2;
		ofVec2f mouthCenter = getCentroid2D(tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH));
		ofVec2f thirdDirection = thirdBase - mouthCenter;
		float thirdDistance = thirdDirection.length() * thirdHeight;
		ofVec2f thirdPosition = thirdBase + thirdDirection.normalize() * thirdDistance;
		ofVec2f leftEyeCenter = getCentroid2D(tracker.getImageFeature(ofxFaceTracker::LEFT_EYE));
		ofVec2f thirdOffset = thirdPosition - leftEyeCenter;
		
		thirdSize = 1.3 + sin(ofGetElapsedTimef()) * 0.4;
		
		ofPolyline eyeRing;
		for(int i = 0; i < ringPoints; i++) {
			ofVec2f curInner = tracker.getImagePoint(inner[i]);
			ofVec2f curOuter = tracker.getImagePoint(outer[i]);
			ofVec2f avg = curInner.interpolate(curOuter, thirdMaskSize * thirdSize);
			eyeRing.addVertex(avg.x, avg.y);
		}
		eyeRing.setClosed(true);
		eyeRing = eyeRing.getSmoothed(3);
		
		mask.begin();
		ofClear(0, 255);
		ofSetColor(255,255,255);
		ofTranslate(thirdOffset.x, thirdOffset.y);
		ofFill();
		ofBeginShape();
		for(int i = 0; i < eyeRing.size(); i++) {
			ofVertex(eyeRing[i].x, eyeRing[i].y);
		}
		ofEndShape(true);
		mask.end();
		
		
		src.begin();
		
		ofClear(0, 255);
		ofTranslate(thirdPosition.x, thirdPosition.y);
		ofScale(thirdSize, thirdSize);
		ofTranslate(-leftEyeCenter.x, -leftEyeCenter.y);
		ofSetColor(255,255,255);
		cam.draw(0, 0);
		src.end();
		
		clone.setStrength(ofMap(ofGetAppPtr()->mouseX, 0, ofGetWidth(), 0, 500, true));
		clone.update(src.getTextureReference(), cam, mask.getTextureReference());		
	}
	
	void draw()  {
		clone.draw();
	}
};