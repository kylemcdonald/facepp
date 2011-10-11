/*
 *  meanMeshFbo.cpp
 *  BasicExample
 *
 *  Created by zachary lieberman on 10/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "meanMeshRenderer.h"




void meanMeshRenderer::setup() {
	
	stillTracker.setup();
	stillTracker.setRescale(1.0);
	stillTracker.setIterations(20);
	still.loadImage("face.png");
	stillTracker.update(ofxCv::toCv(still));
	stillTracker.update(ofxCv::toCv(still));
	stillTracker.update(ofxCv::toCv(still));
	
	// find the bounding box; 
	meanMesh = stillTracker.getMeanObjectMesh();
	
	addForheadToFaceMesh(meanMesh);	// forehead time!
	
	
	ofPoint min, max;
	for (int i = 0; i < meanMesh.getVertices().size(); i++){
		if (i == 0){
			min.x = meanMesh.getVertices()[i].x;
			min.y =meanMesh.getVertices()[i].y;
			max.x =meanMesh.getVertices()[i].x;
			max.y =meanMesh.getVertices()[i].y;
		} else {
			min.x = MIN(min.x,meanMesh.getVertices()[i].x);
			min.y = MIN(min.y,meanMesh.getVertices()[i].y);
			max.x = MAX(max.x,meanMesh.getVertices()[i].x);
			max.y = MAX(max.y,meanMesh.getVertices()[i].y);
		}
		
		meanMesh.getVertices()[i].z = 0; // set it to zero, for this purpose
	}
	
	
	for (int i = 0; i < meanMesh.getVertices().size(); i++){
		meanMesh.getVertices()[i].x -= min.x;
		meanMesh.getVertices()[i].y -= min.y;
	}
	
	width = (max.x - min.x) * 10.0;
	height = (max.y - min.y) * 10.0;
	meanMeshFbo.allocate(width, height, GL_RGBA, 4);
	meanMeshFbo.begin();
	ofClear(0,0,0,255);
	
	ofSetColor(255,255,255);
	ofPushMatrix();
	//ofTranslate(-min.x*5, -min.y*5, 0); //, -min.y, 0);
	ofScale(10,10,1);
	ofSetColor(255,127,0);
	meanMesh.draw();
	ofPopMatrix();
	
	meanMeshFbo.end();
	
	
	for (int i = 0; i < meanMesh.getTexCoords().size(); i++){
		//cout << meanMesh.getTexCoords()[i] << endl;
		meanMesh.getTexCoords()[i].x = meanMesh.getVertices()[i].x * 10;
		meanMesh.getTexCoords()[i].y = meanMesh.getVertices()[i].y * 10;
		
	}
	//std::exit(0);
}

void meanMeshRenderer::clear(){
	
	meanMeshFbo.begin();
	ofClear(0,0,0,0);
	ofSetColor(255,255,255);
	ofPushMatrix();
	ofScale(10,10,1);
	ofSetColor(255,255,255);
	//meanMesh.draw();
	ofPopMatrix();
	for (int i = 0; i < 10; i++){
		ofSetColor(255,0,0);
		ofFill();
		ofCircle(width * (0.5*sin(ofGetElapsedTimef()*(3+i/7.0) ) + 0.5), height * (0.5*sin(ofGetElapsedTimef()*(6+i/3.0) ) + 0.5), 40);
	}
	meanMeshFbo.end();
}

void meanMeshRenderer::draw(){
		
	ofSetColor(255);
	meanMeshFbo.draw(0,0);
}


void meanMeshRenderer::draw(ofxFaceTracker & tracker){
	
	
	ofMesh target = tracker.getImageMesh();
	addForheadToFaceMesh(target);
	
	
	if (target.getVertices().size() != meanMesh.getVertices().size()) return;
	
	
	ofMesh copy = meanMesh;
	
	
	//cout << copy.getVertices().size() << " " << target.getVertices().size() << endl;
	for (int i =0; i < copy.getVertices().size(); i++){
		copy.getVertices()[i] = target.getVertices()[i];
	}
	
	// copy vertices;
	meanMeshFbo.getTextureReference().bind();
	ofSetColor(255,255,255);
	copy.draw();
	meanMeshFbo.getTextureReference().unbind();
}

