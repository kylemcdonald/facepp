/*
 *  eyebrowScene.h
 *  emptyExample
 *
 *  Created by zachary lieberman on 5/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once


#include "ofMain.h"
#include "ofxFaceTracker.h"
#include "ofxBox2d.h"
#include "box2dObj.h"

class testApp;

class eyebrowScene {
	
public:
	
	ofxFaceTracker * tracker;
	
	eyebrowScene(){};
	void setup();
	void update();
	void draw();
	void clear();
	void keyPressed(int key){
		if (key == 'l') {
			bDrawBox2dLine = !bDrawBox2dLine;
		}
		if(key == 'c'){
			clearParticles();
		}
			
			
	}
	
	void mousePressed(int x, int y, int button){
		CustomCircle circle;
		circle.setPhysics(3.0, 0.53, 0.1);
		circle.setup(physics.getWorld(), x, y, 2);
		circles.push_back(circle);
	}
	void mouseDragged(int x, int y, int button){
		CustomCircle circle;
//		circle.setPhysics(3.0, 0.53, 0.1);		
		circle.setPhysics(1.0, 0.33, 0.1);				
		circle.setup(physics.getWorld(), x, y, 2);
		circles.push_back(circle);
	}
	void makeLine(int lineID, int a, int b, bool bConnectBeginAndEnd, bool disapear = false){
		lineStrips[lineID].setWorld(physics.getWorld());
		lineStrips[lineID].clear();
		lines[lineID].clear();
		if (disapear == true) return;
		
		for (int j=0; j<66; j++) {
			if (j >= a && j <= b) {
				lineStrips[lineID].addPoint(tracker->getImagePoint(j).x, tracker->getImagePoint(j).y);
				ofPoint tmpLine;
				tmpLine.set(tracker->getImagePoint(j).x, tracker->getImagePoint(j).y);
				lines[lineID].push_back(tmpLine);
			}				
		}
		if (bConnectBeginAndEnd) {
				lineStrips[lineID].addPoint(tracker->getImagePoint(a).x, tracker->getImagePoint(a).y);
			ofPoint tmpLine;
			tmpLine.set(tracker->getImagePoint(a).x, tracker->getImagePoint(b).y);
			lines[lineID].push_back(tmpLine);
		}
		lineStrips[lineID].createShape();	
		
	}
	
	void clearParticles(){
		int i;
		// update physics	
		for (i=circles.size()-1; i>=0; i--) {
			physics.world->DestroyBody(circles[i].body);
		}
		circles.clear();		
	}
	
	void draw(float width, float height);	// for the FBO
	vector		<CustomCircle>	circles;		  //	default box2d circles	
	
	int currentTriangle;
	vector<int>listFacePointVol;
	ofxBox2dLine					lineStrip;		  //	a linestrip for drawing	
	ofxBox2d				physics;
	testApp * appPtr;
	vector <ofxBox2dLine> lineStrips;
	vector <vector<ofPoint> > lines;
	typedef vector<ofPoint>::iterator v_point;
	typedef vector <vector<ofPoint> >::iterator v_v_point;
	ofPoint ptsPrevFrame[66];
	float vel[66];
	
	bool bBox2dOutlineBounce; // < 
	bool bDrawBox2dLine;
	
	
};