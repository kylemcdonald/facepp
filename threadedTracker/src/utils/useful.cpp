/*
 *  useful.cpp
 *  BasicExample
 *
 *  Created by zachary lieberman on 10/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "useful.h"

string ZeroPadNumber(int num){
	stringstream ss;
	// the number is converted to string with the help of stringstream
	ss << num; 
	string ret;
	ss >> ret;
	// Append zero chars
	int str_length = ret.length();
	for (int i = 0; i < 7 - str_length; i++)
		ret = "0" + ret;
	return ret;
}


ofMesh convertFromIndices(const ofMesh& mesh) {
	ofMesh result;
	// have to do a const_cast because ofMesh::get*() is not const correct
	ofMesh& cmesh = const_cast<ofMesh&>(mesh);
	int vertices = mesh.getNumVertices();
	int colors = mesh.getNumColors();
	int normals = mesh.getNumNormals();
	int texcoords = mesh.getNumTexCoords();
	int indices = mesh.getNumIndices();
	for(int i = 0; i < indices; i++) {
		int cur = cmesh.getIndex(i);
		if(vertices > 0) {
			result.addVertex(cmesh.getVertex(cur));
		}
		if(colors > 0) {
			result.addColor(cmesh.getColor(cur));
		}
		if(normals > 0) {
			result.addNormal(cmesh.getNormal(cur));
		}
		if(texcoords > 0) {
			result.addTexCoord(cmesh.getTexCoord(cur));
		}
	}
	return result;
}



ofMesh convertToIndices(const ofMesh& mesh);



ofMesh convertToIndices(const ofMesh& mesh){	
	ofMesh result;
	ofMesh& cmesh = const_cast<ofMesh&>(mesh);
	int vertices = mesh.getNumVertices();
	int colors = mesh.getNumColors();
	int normals = mesh.getNumNormals();
	int texcoords = mesh.getNumTexCoords();
	//int indices = mesh.getNumIndices();
	
	for (int i = 0; i < vertices; i++){
		// check if this vertex already exists. 
		//int who = isPointInVector(
	}
}

int isPointInVector(ofPoint ptToSearchFor, vector < ofPoint > & pts){

	int who = -1;
	// use some small number?  epsilon?
	for (int i = 0; i < pts.size(); i++){
		float distance = (ptToSearchFor - pts[i]).length();
		if (distance < 0.000001){
			who = i;
			break;
		}
	}
	
	return who;
}

ofPoint getCentroid2D(ofPolyline  temp){
	
	vector < ofPoint > pts = temp.getVertices();
	ofPoint midPt;
	midPt.set(0,0,0);
	for (int i = 0; i <pts.size(); i++){
		midPt+= pts[i];
	}
	midPt /= MAX(1, pts.size());
	return midPt;
	
}


