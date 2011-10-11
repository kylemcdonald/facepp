/*
 *  lightScene.cpp
 *  emptyExample
 *
 *  Created by zachary lieberman on 5/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "lightScene.h"
#include "testApp.h"


// --------------------------------------------- sorry this is a bit of a mess. 


using namespace cv;

float energy;


void lightScene::setup(){
	
	//cout << "-----------" << endl;
	
	shader.load("shaders/lighting.vert", "shaders/lighting.frag");
	//cout << "-----------" << endl;
	
	light.enable();	
	lightBlue.enable();
	lightGreen.enable();
	
	
	energy = 0;
	
	
	
	for (int i = 0; i < 4; i++){
		particle myParticle;
		myParticle.setInitialCondition(ofRandom(500,550),ofRandom(500,550),0,0);
		particles.push_back(myParticle);
	}
	
	particles[0].bFixed = true;
	
	for (int i = 1; i < particles.size(); i++){
		spring mySpring;
		mySpring.distance		= 100;
		mySpring.springiness	= 0.2f;
		mySpring.particleA = & (particles[0  ]);
		mySpring.particleB = & (particles[i]);
		springs.push_back(mySpring);
	}
	
	
	tempA.setInitialCondition(200,200,0,0);
	tempB.setInitialCondition(200,200,0,0);
	tempA.damping = 0.01;
	tempB.damping = 0.01;
	mouseSpring.particleA = &tempA;
	mouseSpring.particleB = &tempB;
	mouseSpring.distance = 1;
	mouseSpring.springiness = 0.3;
		
	
}

void lightScene::update(ofMesh & faceMesh){
	
	myFaceMesh = faceMesh;
	ofPoint midPt;
	for (int i = 0; i < myFaceMesh.getVertices().size(); i++){
		midPt +=  myFaceMesh.getVertices()[i];
	}
	midPt /= MAX(1,  myFaceMesh.getVertices().size());
	
	
	energy = 1;
	
	ofVec2f pos = ((testApp*)ofGetAppPtr())->tracker.getPosition();
	ofVec3f orientation = ((testApp*)ofGetAppPtr())->tracker.getOrientation();
	ofPoint posT = pos;
	posT += orientation * 400;
	
	tempA.setInitialCondition(pos.x, pos.y, 0,0);
	tempB.resetForce();
	mouseSpring.update();
	tempB.addDampingForce();
	tempB.update();
	
	particles[0].setInitialCondition(tempB.pos.x, tempB.pos.y, 0,0);
	
	
	//ofVec2f pos = TM->FT.getPosition();
	//particles[0].setInitialCondition(pos.x, pos.y, 0, 0);
	
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
	}
	
	for (int i = 0; i < particles.size(); i++){
		
		//particles[i].addRepulsionForce(mouseX, mouseY, 200, 0.7f);
		
		for (int j = 0; j < i; j++){
			particles[i].addRepulsionForce(particles[j], 400, 0.06);
		}
	}
	
	for (int i = 0; i < springs.size(); i++){
		springs[i].update();
	}
	
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].addDampingForce();
		particles[i].update();
	}
	
	
	
	
	
}


inline int
isLeft( ofPoint P0, ofPoint P1, ofPoint P2 )
{
    return ( (P1.x - P0.x) * (P2.y - P0.y)
            - (P2.x - P0.x) * (P1.y - P0.y) );
}
//===================================================================

// orientation2D_Triangle(): test the orientation of a triangle
//    Input:  three vertex points V0, V1, V2
//    Return: >0 for counterclockwise 
//            =0 for none (degenerate)
//            <0 for clockwise
inline int
orientation2D_Triangle( ofPoint V0, ofPoint V1, ofPoint V2 )
{
    return isLeft(V0, V1, V2);
}



void lightScene::drawTriangle ( trianglez temp, trianglez d){
	
	unsigned char * pix = ((testApp*)ofGetAppPtr())->MMR.depthImage.getPixels();
	int w = ((testApp*)ofGetAppPtr())->MMR.width;
	int h = ((testApp*)ofGetAppPtr())->MMR.height;
	
	
	ofPoint pts[3];
	pts[0] = temp.pta;
	pts[1] = temp.ptb;
	pts[2] = temp.ptc;
	
	ofPoint ptsD[3];
	ptsD[0] = d.pta;
	ptsD[1] = d.ptb;
	ptsD[2] = d.ptc;
	
	
	for(int i = 0; i < 3; i++){
		//cout << ptsD[i].x << " " << ptsD[i].y << " " << w << " " << h << endl;
		int which = (int)ptsD[i].y * w + (int)(ptsD[i].x);
		if (which < 0) which = 0;
		if (which > w*h-1) which = w*h-1;
		float height = pix[ which ] * 2.5;
		pts[i].z = height;
	}
	
	ofVec3f nn ;
	if (true){
		
		int id0 = 0;
		int id1 = 1;
		int id2 = 2;

		if (orientation2D_Triangle( pts[0],  pts[id1],  pts[id2]) < 0){
			id0 = 2;
			id1 = 1;
			id2 = 0;
		}

		ofPoint d0 = pts[id0] - pts[id1];
		ofPoint d1 = pts[id1] - pts[id2];
		d0.normalize();
		d1.normalize();
		nn = d0.cross(d1);
	}

	shader.begin();

	int id0 = 0;
	int id1 = 1;
	int id2 = 2;


	if (orientation2D_Triangle( pts[0],  pts[id1],  pts[id2]) < 0){
		id0 = 2;
		id1 = 1;
		id2 = 0;
	}

	ofSetColor(255,255,255);

	glBegin(GL_TRIANGLES);
	glNormal3f(nn.x, nn.y, nn.z);
	glVertex3f(pts[id0].x, pts[id0].y, 0);
	glNormal3f(nn.x, nn.y, nn.z);
	glVertex3f(pts[id1].x, pts[id1].y,0);
	glNormal3f(nn.x, nn.y, nn.z);
	glVertex3f(pts[id2].x, pts[id2].y, 0);
	glEnd();


	shader.end();


}


void lightScene::draw(float w, float h){
	
	
	
	
//ofPoint extras[12];
//	ofPoint extrasUp[12];
//	
//	extras[0] = TM->asmPtsSmoothed[0];
//	extras[1] = TM->asmPtsSmoothed[17];
//	extras[2] = TM->asmPtsSmoothed[18];
//	extras[3] = TM->asmPtsSmoothed[19];
//	extras[4] = TM->asmPtsSmoothed[20];
//	extras[5] = TM->asmPtsSmoothed[21];
//	extras[6] = TM->asmPtsSmoothed[22];
//	extras[7] = TM->asmPtsSmoothed[23];
//	extras[8] = TM->asmPtsSmoothed[24];
//	extras[9] = TM->asmPtsSmoothed[25];
//	extras[10] = TM->asmPtsSmoothed[26];
//	extras[11] = TM->asmPtsSmoothed[16];
//	
//	for (int i = 0; i < 12; i++){
//		extrasUp[i] = extras[i];
//		extrasUp[i] += (TM->asmPtsSmoothed[27] - TM->asmPtsSmoothed[33]) ;
//	}

	
	glPushMatrix();
	
	//glScalef(w / 640.0f, h / 480.0f, 1.0);
	
	
	//ofPoint light = ofPoint( ofMap(((testApp*)ofGetAppPtr())->mouseX, 0, ofGetWidth(), -13000, 13000),ofMap(((testApp*)ofGetAppPtr())->mouseY, 0, ofGetHeight(), -13000, 13000), 1000 * sin(ofGetElapsedTimef()));
	
	//light.normalize();
	
		ofEnableLighting();
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);	
	//	
		//ofPoint lightpos = ofPoint( ofMap(((testApp*)ofGetAppPtr())->mouseX, 0, ofGetWidth(), -13000, 13000),ofMap(((testApp*)ofGetAppPtr())->mouseY, 0, ofGetHeight(), -13000, 13000), 100);
	//	
	//	
	//	light.setPosition(lightpos.x, lightpos.y, lightpos.z);
	//	
	
	ofVec2f pos = ((testApp*)ofGetAppPtr())->tracker.getPosition();
	ofVec3f orientation =  ((testApp*)ofGetAppPtr())->tracker.getOrientation();
	ofPoint posT = pos;
	posT += orientation * 400;
	
	
	float speed = particles[1].vel.length() / (((testApp*)ofGetAppPtr())->mouseY/30.0 + 1.0);
	if (speed > 1) speed = 1;
	light.setPosition(particles[1].pos.x, particles[1].pos.y, 200);
	light.setDiffuseColor(ofColor(255*1, 0, 0));
	speed = particles[2].vel.length() / (((testApp*)ofGetAppPtr())->mouseY/30.0 + 1.0);
	if (speed > 1) speed = 1;
	lightGreen.setPosition(particles[2].pos.x, particles[2].pos.y, 200);
	lightGreen.setDiffuseColor(ofColor(0,255*1, 0));
	speed = particles[3].vel.length() / (((testApp*)ofGetAppPtr())->mouseY/30.0 + 1.0);
	if (speed > 1) speed = 1;
	lightBlue.setPosition(particles[3].pos.x, particles[3].pos.y, 200);
	lightBlue.setDiffuseColor(ofColor(0,0,1*255));
	
	//lightBlue.setPosition(particles[2].pos.x, particles[2].pos.y, 500);


	ofSeedRandom(0);
	if (((testApp*)ofGetAppPtr())->tracker.getImageMesh().getVertices().size() > 0){
		
		ofMesh currentMesh = ((testApp*)ofGetAppPtr())->tracker.getImageMesh();
		addForheadToFaceMesh(currentMesh);
		ofMesh forUsage = convertFromIndices(currentMesh);
		ofMesh temp = ((testApp*)ofGetAppPtr())->MPA.returnMeshWarpedToThisMesh(forUsage);
		//temp.drawWireframe();
		ofEnableAlphaBlending();
		ofFill();
		
		trianglez t;
		trianglez d;
		vector <ofPoint> & verticesMMR = ((testApp*)ofGetAppPtr())->MMR.meanMeshThroughMPA.getVertices();
		
		ofEnableBlendMode(OF_BLENDMODE_SCREEN);
		for (int i = 0; i < temp.getVertices().size()/3; i++){
			t.set(temp.getVertices()[i*3],
				   temp.getVertices()[i*3+1],
				   temp.getVertices()[i*3+2]);
			d.set(verticesMMR[i*3],
				  verticesMMR[i*3+1],
				  verticesMMR[i*3+2]);
			//ofSetColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255), 70 );
			//ofTriangle(temp.getVertices()[i*3],
			//		   temp.getVertices()[i*3+1],
			//		   temp.getVertices()[i*3+2]);
			drawTriangle(t,d);
			
		}
		ofDisableBlendMode();
	}
	
	
	
	ofDisableLighting();
ofSetColor(255,255,255);
	//for (int i = 0; i < particles.size(); i++){
//		particles[i].draw();
//	}
//	
//	for (int i = 0; i < springs.size(); i++){
//		springs[i].draw();
//	}
	
	
	glPopMatrix();
	
}





void lightScene::clear(){
	
}


