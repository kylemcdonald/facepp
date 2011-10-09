
#pragma once

#include "ofMain.h"

#include "triangle.h"
#include "triangleUtils.h"
#include "barycentric.h"


typedef struct {
	int a, b, c;
} trianglePts;

typedef struct {
	int whichTriangle;
	float a,b,c;
	bool bConnectToPoint;
	int whichPoint;
} barycentricInfo;


// I add points to a mesh, and using the barycentric coordinates, reproject those points. 

class meshPointAdder {

public: 
	
	
	void setup(ofMesh & basisMesh);				// this gets triangulated. 
	ofMesh myBasis;								// I assume this is a triangle mesh.   this is for debugging
	ofMesh returnMeshWarpedToThisMesh(ofMesh & meshToMatch);		// I will alter mesh to match this. 
	
	void calcluateTriangulation();				// this is calling triangle library. 
	void reprojectPoints();						// this is updatating points. 
	
	void draw();
	
	// intermediary, before getting a mesh back. 
	vector < ofPoint > ptsList;
	vector < trianglePts > triangles; 
	vector < barycentricInfo > ptsListBarycentric;
	
	
	// pass through holes I guess too. this is a hack, but I think it will work. 
	vector < vector < ofPoint > > holes;
	
	
	
	
	
}; 

