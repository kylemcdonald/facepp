/*
 *  basicScene.h
 *  emptyExample
 *
 *  Created by zachary lieberman on 5/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once


#include "ofMain.h"
#include "ofShader.h"
#include "spring.h"
#include "particle.h"


class spinningLight {
	
public: 
	void setup();
	void update();
	void draw();
	void applyLights();
	
	ofPoint		centerPoint;
	ofPoint		cosVal;
	ofPoint		sinVal;	// to do rotation. 
	ofLight		light;
	float		energy;
	ofPoint		position; 
	ofPoint		color;
	
	float		angleChange;
	float direction;
	
	float sinSpeed;
	float cosSpeed;
	
	float randomOffset;
	
};


class  trianglez{
public: 
	void set(ofPoint pt1, ofPoint pt2, ofPoint pt3){
		pta = pt1;
		ptb = pt2;
		ptc = pt3;
	}
	ofPoint pta;
	ofPoint ptb;
	ofPoint ptc;
} ;


class lightScene {
	
public:
	
	lightScene(){};
	void setup();
	void update(ofMesh & faceMesh);
	void draw();
	void clear();
	
	ofMesh myFaceMesh;
	
	void drawTriangle ( trianglez temp, trianglez fromDepth);
	
	void draw(float width, float height);	// for the FBO
	
	ofShader shader;
	
	vector < spinningLight > lights;
	
	ofPoint ptSmoot;
	ofPoint ptSmootLastFrame;
	float speedMoving;
	
	void drawTriangleFake ( trianglez temp, trianglez fromDepth);	// draw a triangle into the newMesh
	ofMesh newMesh;
	
};