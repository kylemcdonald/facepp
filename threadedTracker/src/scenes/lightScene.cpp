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



class IndexedPoint {
public:
	const ofVec3f* vertex;
	const ofFloatColor* color;
	const ofVec3f* normal;
	const ofVec2f* texCoord;
	
	IndexedPoint(const ofMesh& mesh, int i) {
		vertex = mesh.getNumVertices() > 0 ? &(mesh.getVerticesPointer()[i]) : NULL;
		color = mesh.getNumColors() > 0 ? &(mesh.getColorsPointer()[i]) : NULL;
		normal = mesh.getNumNormals() > 0 ? &(mesh.getNormalsPointer()[i]) : NULL;
		texCoord = mesh.getNumTexCoords() > 0 ? &(mesh.getTexCoordsPointer()[i]) : NULL;
	}
	// used for map operator[]
	int operator<(const IndexedPoint& point) const {
		return (vertex->x + vertex->y) < (point.vertex->x + point.vertex->y);
	}
};

#include <map>
ofMesh convertToIndices(ofMesh& mesh) {
	ofMesh result;
	
	int vertices = mesh.getNumVertices(); // ofVec3f
	int colors = mesh.getNumColors(); // ofFloatColor
	int normals = mesh.getNumNormals(); // ofVec3f
	int texcoords = mesh.getNumTexCoords();	// ofVec2f
	
	bool usingColors = colors > 0;
	bool usingNormals = normals > 0;
	bool usingTexcoords = texcoords > 0;
	
	map<IndexedPoint, int> indexedMap;
	vector<vector<int> > shared;
	int index = 0;
	for(int i = 0; i < vertices; i++) {
		IndexedPoint cur(mesh, i);
		if(indexedMap.find(cur) == indexedMap.end()) {
			indexedMap[cur] = index;
			shared.push_back(vector<int>());
			index++;
		}
		int curIndex = indexedMap[cur];
		result.addIndex(curIndex);
		shared[curIndex].push_back(i);
	}
	
	for(int i = 0; i < shared.size(); i++) {
		vector<int>& cur = shared[i];
		
		ofVec3f vertex = mesh.getVerticesPointer()[cur[0]];
		ofFloatColor color(0, 0, 0, 0);
		ofVec3f normal(0, 0, 0);
		ofVec2f texCoord(0, 0);
		float normalize = 1. / cur.size();
		
		for(int j = 0; j < cur.size(); j++) {
			if(usingColors) {
				color += mesh.getColorsPointer()[cur[j]] * normalize;
			}
			if(usingNormals) {
				normal += mesh.getNormalsPointer()[cur[j]] * normalize;
			}
			if(usingTexcoords) {
				texCoord += mesh.getTexCoordsPointer()[cur[j]] * normalize;
			}
		}
		
		result.addVertex(vertex);
		if(usingColors) {
			result.addColor(color);
		}
		if(usingNormals) {
			result.addNormal(normal);
		}
		if(usingTexcoords) {
			result.addTexCoord(texCoord);
		}		
	}
	
	return result;
}






	void spinningLight::setup(){
		cosVal.set(150,0,0);
		sinVal.set(0,0,150);
		light.enable();
		color.set(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255));
		color.y = ofRandom(0,255);
		//if (ofRandom(0,1) > 0.5) direction = -1;
		//else
			direction = 1;
		sinSpeed = ofRandom(2,4);
		cosSpeed = sinSpeed;//ofRandom(2,4);
		
		angleChange = ofRandom(-PI, PI);
		randomOffset = ofRandom(-100,100);
	}
	void spinningLight::update(){
		position = centerPoint;
		position.y += randomOffset;
		position += cos(angleChange*direction) * cosVal;
		position += sin(angleChange*direction) * sinVal;
		
		angleChange += 0.05;;
		energy *= 0.967f;
	}
	
	void spinningLight::draw(){
		ofEnableAlphaBlending();
		if (position.z > 0){
			ofSetColor(255,255,255,255*(energy*10.0));
			ofSphere(position.x, position.y, position.z, 3);
		}
	}
	
	void spinningLight::applyLights(){
		float tt = MIN(1, (energy*10.0));
		light.setPosition(position.x, position.y, position.z);
		light.setDiffuseColor(ofColor(color.x*tt,color.y*tt, color.z*tt));
	}
	






// --------------------------------------------- sorry this is a bit of a mess. 


using namespace cv;

float energy;


void lightScene::setup(){
	
	shader.load("shaders/lighting.vert", "shaders/lighting.frag");	
	
	for (int i = 0; i < 5; i++){
		lights.push_back(spinningLight());
	}
	
	for (int i = 0; i < lights.size(); i++){
		lights[i].setup();
	}
	
	
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
	
	for (int i = 0; i < lights.size(); i++){
		lights[i].update();
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


void lightScene::drawTriangleFake ( trianglez temp, trianglez d){
	
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
	
	//shader.begin();
	
	int id0 = 0;
	int id1 = 1;
	int id2 = 2;
	
	
	if (orientation2D_Triangle( pts[0],  pts[id1],  pts[id2]) < 0){
		id0 = 2;
		id1 = 1;
		id2 = 0;
	}
	
	ofSetColor(255,255,255);
	
	newMesh.addVertex(ofPoint(pts[id0].x, pts[id0].y, 0));
	newMesh.addNormal(ofPoint(nn.x, nn.y, nn.z));
	newMesh.addVertex(ofPoint(pts[id1].x, pts[id1].y,0));
	newMesh.addNormal(ofPoint(nn.x, nn.y, nn.z));
	newMesh.addVertex(ofPoint(pts[id2].x, pts[id2].y, 0));
	newMesh.addNormal(ofPoint(nn.x, nn.y, nn.z));
		
	
	//shader.end();
}






void lightScene::draw(float w, float h){
	


	
	glPushMatrix();
	ofEnableLighting();
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);	
	ofVec2f pos = ((testApp*)ofGetAppPtr())->tracker.getPosition();
	ofVec3f orientation =  ((testApp*)ofGetAppPtr())->tracker.getOrientation();
	ofPoint posT = pos;
	posT += orientation * 400;
	
	ptSmootLastFrame= ptSmoot;
	ptSmoot = 0.97f * ptSmoot + 0.03f * pos;
	speedMoving = (ptSmoot - ptSmootLastFrame).length();
	
	
	
	
	for (int i = 0; i < lights.size(); i++){
		
		lights[i].energy += speedMoving/50.0;
		
		lights[i].energy = MIN(lights[i].energy, 0.1);
		lights[i].centerPoint = ptSmoot;
		lights[i].applyLights();
	}
	//vector < spinningLight > lights;
	/*
	light.setPosition(ptSmoot.x + 200 * cos(ofGetElapsedTimef()*4), ptSmoot.y , 0 + 200 * sin(ofGetElapsedTimef()*4));
	light.setDiffuseColor(ofColor(0,0,255));
	
	lightGreen.setPosition(ptSmoot.x + 200 * cos(ofGetElapsedTimef()*3), ptSmoot.y , 0 + 200 * sin(ofGetElapsedTimef()*3));
	lightGreen.setDiffuseColor(ofColor(0,255, 0));
	
	lightBlue.setPosition(ptSmoot.x, ptSmoot.y + 200 * cos(ofGetElapsedTimef()*2), 0 + 200 * sin(ofGetElapsedTimef()*3));
	lightBlue.setDiffuseColor(ofColor(255,0,0));
	
	lights[0].set(ptSmoot.x + 200 * cos(ofGetElapsedTimef()*4), ptSmoot.y , 0 + 200 * sin(ofGetElapsedTimef()*4));
	lights[1].set(ptSmoot.x + 200 * cos(ofGetElapsedTimef()*3), ptSmoot.y , 0 + 200 * sin(ofGetElapsedTimef()*3));
	lights[2].set(ptSmoot.x, ptSmoot.y + 200 * cos(ofGetElapsedTimef()*2), 0 + 200 * sin(ofGetElapsedTimef()*3));
	*/
	
	
	
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
		
		
		//newMesh.clear();
		for (int i = 0; i < temp.getVertices().size()/3; i++){
			t.set(temp.getVertices()[i*3],
				   temp.getVertices()[i*3+1],
				   temp.getVertices()[i*3+2]);
			d.set(verticesMMR[i*3],
				  verticesMMR[i*3+1],
				  verticesMMR[i*3+2]);
			drawTriangle(t,d);
			
			//drawTriangleFake(t,d);
		}
		
		//ofMesh temp2 = convertToIndices(newMesh);
//		temp2.draw();
		
		ofDisableBlendMode();
	}
	
	
	glPopMatrix();

	
	ofDisableLighting();
	ofSetColor(255,255,255);

	
	for (int i = 0; i < lights.size(); i++){
		lights[i].draw();
	}
	
		
}





void lightScene::clear(){
	
}


