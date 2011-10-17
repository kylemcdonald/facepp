#pragma once

class Mandala {
public:
	float scale;
	
	Mandala()
	:scale(4) {
	}
			
	void drawRing(float radius, float theta, float height, bool direction, int count, bool decorated, int decorationResolution = 0, float decorationRadius = 0) {
		ofSetCircleResolution(decorationResolution);
		radius *= scale / 4;
		height *= (sqrt(3) / 2) * (direction ? -1 : 1);
		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);
		for (int i = 0; i < (count); i++){
			float angle1 = theta + (i * TWO_PI) / count;
			float angle2 = angle1 + TWO_PI / count;
			float angle3 = (angle1 + angle2)/2.0;
			ofVec2f pt1(cos(angle1), sin(angle1));
			ofVec2f pt2(cos(angle2), sin(angle2));
			pt1 *= radius;
			pt2 *= radius;
			float tip = radius + pt1.distance(pt2) * height;
			ofVec2f pt3(cos(angle3), sin(angle3));
			pt3 *= tip;
			mesh.addVertex(pt1);
			mesh.addVertex(pt2);
			mesh.addVertex(pt3);
			
			if(decorated) {
				ofPushMatrix();
				ofRotate(ofRadToDeg(angle3));
				ofCircle(tip, 0, decorationRadius);
				ofPopMatrix();
			}
		}
		mesh.drawFaces();
	}

	void drawMandala(ofVec2f position, float radius) {
		ofTranslate(position.x, position.y);
		
		ofColor cyan(0, 174, 239), magenta(236, 0, 140), yellow(255, 242, 0);
		
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		float pulseTheta = ofGetElapsedTimef() / 128;
		float pulseAlpha = 64;
		float pulseLength = 16;
		float pulseDeviation = .4;
		float phase1 = 1 + pulseDeviation * sin(ofGetElapsedTimef() + TWO_PI * 0 / 3);
		float phase2 = 1 + pulseDeviation * sin(ofGetElapsedTimef() + TWO_PI * 1 / 3);
		float phase3 = 1 + pulseDeviation * sin(ofGetElapsedTimef() + TWO_PI * 2 / 3);
		ofNoFill();
		ofSetColor(cyan, pulseAlpha);
		drawRing(110, pulseTheta * 5, pulseLength * phase1, false, 79, true, 3, 207);
		ofSetColor(magenta, pulseAlpha);
		drawRing(120, pulseTheta * 7, pulseLength * phase2, false, 83, true, 7, 88);
		ofSetColor(yellow, pulseAlpha);
		drawRing(130, -pulseTheta * 11, pulseLength * phase3, false, 89, true, 5, 160);
		ofPopStyle();
		
		ofPushStyle();
		ofNoFill();
		ofSetColor(255, 128);
		drawRing(110, -(ofGetElapsedTimef()/3.0), 1, true, 60, true, 6, 4);
		ofFill();
		drawRing(128, -ofGetElapsedTimef()/2, 1, true, 40, true, 4, 8);
		drawRing(130, ofGetElapsedTimef()/2, 1, false, 40, true, 3, 16);
		
		ofSetColor(255, 32);
		drawRing(240, ofGetElapsedTimef()/2, 1, false, 30, true, 3, 120);
		ofPopStyle();
	}
};
