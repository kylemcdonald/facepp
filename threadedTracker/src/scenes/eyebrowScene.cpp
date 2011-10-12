/*
 *  eyebrowScene.cpp
 *  emptyExample
 *
 *  Created by zachary lieberman on 5/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "eyebrowScene.h"
#include "testApp.h"


void eyebrowScene::setup(){
	
	currentTriangle = 0;
	
	for (int i=0; i<60; i++) {
		listFacePointVol.push_back(0);
	}
	
	physics.init();
	//	physics.createBounds();
	//	physics.createBounds(0,0, CAM_WIDTH,CAM_HEIGHT);	
	
	physics.setGravity(0, 100);	
	physics.checkBounds(true);
	for (int i=0; i<8; i++) {
		ofxBox2dLine tmpline;
		tmpline.setWorld(physics.getWorld());
		lineStrips.push_back(tmpline);
		
		ofPoint pt;
		vector<ofPoint> v_pt;
		v_pt.push_back(pt);
		lines.push_back(v_pt);
	}
	bBox2dOutlineBounce = true; // 
	bDrawBox2dLine = false;

}

void eyebrowScene::update(){
	
	physics.setGravity(0, 30);	
	physics.update();	
	
	if (tracker->getFound()  == false){
		if(bBox2dOutlineBounce){
			for (int i=0; i<8; i++) {
				if (i ==0) {
					makeLine(i, 0, 16, false, true);
				}
				else if(i==1){
					makeLine(i, 17, 21, true,true);
				}
				else if(i==2){
					makeLine(i, 22, 26, true,true);
				}
				else if(i==3){
					makeLine(i, 36, 41, true,true);
				}
				else if(i==4){
					makeLine(i, 42, 47, true,true);
				}
				else if(i==5){
					makeLine(i, 48, 59, true,true);
				}
				else if(i==6){
					makeLine(i, 60, 65, true,true);
				}
				else if(i==7){
					makeLine(i, 27, 35, false,true);
				}
			}
		}
	}
	
	
	if (tracker->getFound()  == true){
	if(bBox2dOutlineBounce){
		for (int i=0; i<8; i++) {
			if (i ==0) {
				makeLine(i, 0, 16, false);
			}
			else if(i==1){
				makeLine(i, 17, 21, true);
			}
			else if(i==2){
				makeLine(i, 22, 26, true);
			}
			else if(i==3){
				makeLine(i, 36, 41, true);
			}
			else if(i==4){
				makeLine(i, 42, 47, true);
			}
			else if(i==5){
				makeLine(i, 48, 59, true);
			}
			else if(i==6){
				makeLine(i, 60, 65, true);
			}
			else if(i==7){
				makeLine(i, 27, 35, false);
			}
		}
	}
	
	for (int i = 0; i < 66; i++){
		
		//if (i>36 && 47>i) {
		if (i>=17 && i <= 27) {
			int x = (int)tracker->getImagePoint(i).x;
			int y = (int)tracker->getImagePoint(i).y;
			vel[i] = ofDist(x, y,  ptsPrevFrame[i].x, ptsPrevFrame[i].y);
			if (vel[i] > 1.0){			
				CustomCircle circle;
				circle.setPhysics(3.0, 0.53, 0.1);
				circle.setup(physics.getWorld(), tracker->getImagePoint(i).x, tracker->getImagePoint(i).y, 
							 ofMap(vel[i], 0., 20.,3., 8, true));
				circles.push_back(circle);	
				//printf(" here \n");
				//ofxDaito::bang("crying", x);
			}
			
			ptsPrevFrame[i].set(x,y);
		}
	}	
	}
	
	if (circles.size()>200) {
		clearParticles();
	}

	// erase circle
	for(int i=circles.size()-1; i>=0; i--) {
		ofPoint mpos = circles[i].getPosition();
		
		//cout << mpos << endl;
		
		if (mpos.y > ofGetHeight()) {
			
			circles[i].destroyShape();
			vector<CustomCircle>::iterator it = circles.begin() + i;
			circles.erase(it);				
		}
	}
	
}

void eyebrowScene::draw(float w, float h){

	glPushMatrix();
	
	ofSetLineWidth(6);
	glScalef(w / 640.0f, h / 480.0f, 1.0);
//	if (bDrawLine) {		
//		for (int i=0; i<lineStrips.size(); i++) {
//			lineStrips[i].draw();	
//		}
//	}

	if (bDrawBox2dLine) {		
		ofEnableAlphaBlending();
		ofSetColor(55, 155, 255, 180);
		int i=0;
		ofSetLineWidth(6);
		for (v_v_point p = lines.begin(); p != lines.end(); ++p) {		
			vector<ofPoint> vl = (*p);
			ofBeginShape();		
			if (i==0) {
				ofNoFill();
			}else {
				ofFill();
			}
			
			for (v_point q = vl.begin();q!= vl.end(); ++q) {
				ofPoint l = (*q);
				ofVertex(l.x, l.y);
			}
			ofEndShape(false);
			i++;
		}
	}

	for(int i=0; i<circles.size(); i++) {
		circles[i].draw();
		//cout << "drawing " << i << endl;
	}
	
	
	glPopMatrix();	
	ofSetLineWidth(1);	
}

void eyebrowScene::draw(){
	
	
}


void eyebrowScene::clear(){
	
}




