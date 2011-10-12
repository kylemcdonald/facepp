/*
 *  box2dObj.h
 *  face_
 *
 *  Created by 真鍋 大度 on 11/05/30.
 *  Copyright 2011 Rhizomatiks. All rights reserved.
 *
 */

#pragma once


//#include "baseScene.h"
#include "ofMain.h"

#include "ofxBox2d.h"


class CustomCircle : public ofxBox2dCircle {
	
public:
	
	float radius;
	bool bCollition;
	ofPoint previousVelocity;
	ofPoint currentVelocity;
	CustomCircle() {
		bCollition = false;
		currentVelocity.set(0, 0);
		previousVelocity.set(0, 0);
		color.r = ofRandom(155);
		color.b = ofRandom(255); 
		color.g = ofRandom(255); 
	} 
	
	void update(){
		currentVelocity=getVelocity();
		if (previousVelocity.x * currentVelocity.x < -1 || previousVelocity.y * currentVelocity.y < -1) {
			bCollition = true;
		}
		previousVelocity = currentVelocity;
	}
	
	ofColor color;
	void draw() {
		radius = getRadius();
		
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
		
		ofSetColor(color.r, color.g, color.b);
//		ofNoFill();
		ofFill();
		ofEnableSmoothing();
		ofSetCircleResolution(44);		
		ofCircle(0, 0, radius);	
		glPopMatrix();
		//printf("%f %f \n", getPosition().x, getPosition().y);
		bCollition = false;
	}
};


class CustomPoly : public ofxBox2dPolygon {
	
public:
	CustomPoly(){
		
	}
	ofColor color;
	bool bFill;
	
	void draw2(){
		if(body != NULL ) {
			ofNoFill();
			ofSetColor(2, 255, 255, 255);
			
			ofBeginShape();
			for(int i=0; i<vertices.size(); i++) {
				ofVertex(vertices[i].x, vertices[i].y);
			}
			
			ofEndShape();
//			
//			ofSetColor(255, 255,255);
//			ofPoint c = getCenter();
//			ofCircle(c.x, c.y, 1);
//			ofSetColor(0, 0, 0);
			return;
			
		}
		
	}
	
	
};
