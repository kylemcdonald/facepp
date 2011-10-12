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
	
	shader.load("shaders/lighting.vert", "shaders/lighting.frag");	
	light.enable();	
	lightBlue.enable();
	lightGreen.enable();
	
	energy = 0;

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
	


	
	glPushMatrix();
	ofEnableLighting();
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);	
	ofVec2f pos = ((testApp*)ofGetAppPtr())->tracker.getPosition();
	ofVec3f orientation =  ((testApp*)ofGetAppPtr())->tracker.getOrientation();
	ofPoint posT = pos;
	posT += orientation * 400;
	
	ptSmoot = 0.97f * ptSmoot + 0.03f * pos;
	
	light.setPosition(ptSmoot.x + 200 * cos(ofGetElapsedTimef()*4), ptSmoot.y , 0 + 200 * sin(ofGetElapsedTimef()*4));
	light.setDiffuseColor(ofColor(255*0.6, 255*0.6, 0));
	
	lightGreen.setPosition(ptSmoot.x + 200 * cos(ofGetElapsedTimef()*3), ptSmoot.y , 0 + 200 * sin(ofGetElapsedTimef()*3));
	lightGreen.setDiffuseColor(ofColor(0,255*0.6, 255*0.6));
	
	lightBlue.setPosition(ptSmoot.x, ptSmoot.y + 200 * cos(ofGetElapsedTimef()*2), 0 + 200 * sin(ofGetElapsedTimef()*3));
	lightBlue.setDiffuseColor(ofColor(255*0.6,0,0.6*255));
	
	lights[0].set(ptSmoot.x + 200 * cos(ofGetElapsedTimef()*4), ptSmoot.y , 0 + 200 * sin(ofGetElapsedTimef()*4));
	lights[1].set(ptSmoot.x + 200 * cos(ofGetElapsedTimef()*3), ptSmoot.y , 0 + 200 * sin(ofGetElapsedTimef()*3));
	lights[2].set(ptSmoot.x, ptSmoot.y + 200 * cos(ofGetElapsedTimef()*2), 0 + 200 * sin(ofGetElapsedTimef()*3));
	
	
	
	
	//ofSeedRandom(0);
	if (((testApp*)ofGetAppPtr())->tracker.getImageMesh().getVertices().size() > 0){
		
		ofMesh currentMesh = ((testApp*)ofGetAppPtr())->tracker.getImageMesh();
		addForheadToFaceMesh(currentMesh);
		ofMesh forUsage = convertFromIndices(currentMesh);
		ofMesh temp = ((testApp*)ofGetAppPtr())->MPA.returnMeshWarpedToThisMesh(forUsage);
		ofEnableAlphaBlending();
		ofFill();
		
		trianglez t;
		trianglez d;
		vector <ofPoint> & verticesMMR = ((testApp*)ofGetAppPtr())->MMR.meanMeshThroughMPA.getVertices();
		
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		for (int i = 0; i < temp.getVertices().size()/3; i++){
			t.set(temp.getVertices()[i*3],
				   temp.getVertices()[i*3+1],
				   temp.getVertices()[i*3+2]);
			d.set(verticesMMR[i*3],
				  verticesMMR[i*3+1],
				  verticesMMR[i*3+2]);
			drawTriangle(t,d);
		}
		ofDisableBlendMode();
	}
	
	
	glPopMatrix();

	
	ofDisableLighting();
	ofSetColor(255,255,255);

	
	for (int i = 0; i < 3; i++){
		ofPushMatrix();
		if (lights[i].z > 0){
			ofTranslate(lights[i].x, lights[i].y, lights[i].z);
			ofSphere(0, 0, 0, 3);
		}
		ofPopMatrix();
	}
	
		
}





void lightScene::clear(){
	
}


