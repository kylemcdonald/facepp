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
	ofLight light;
	ofLight lightBlue;
	ofLight lightGreen;
	
	vector <particle> particles;
	vector <spring> springs;
	
	spring mouseSpring;
	particle tempA;
	particle tempB;

	
};