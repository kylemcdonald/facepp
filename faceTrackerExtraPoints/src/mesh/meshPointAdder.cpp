/*
 *  meshPointAdder.cpp
 *  emptyExample
 *
 *  Created by zachary lieberman on 10/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

//http://kratos.cimne.upc.es/kratosdoxygen/html/d4/d91/tetgen__volume__mesher_8h-source.html

#include "meshPointAdder.h"

#ifdef REAL
#undef REAL
#include "ofxCv.h"
using namespace ofxCv;
using namespace cv;
#endif


void meshPointAdder::setup(ofMesh & basis){
	myBasis = basis; // copy mesh in to local copy
	// delete z information please
	
	for (int i = 0; i < myBasis.getVertices().size(); i++){
		myBasis.getVertices()[i].z = 0;
	}
	calcluateTriangulation();
}

void meshPointAdder::calcluateTriangulation(){
	
	struct triangulateio in;
	struct triangulateio out;
	memset((void * )&in, 0, sizeof(triangulateio));
	memset((void * )&out, 0, sizeof(triangulateio));
	
	vector < ofPoint > ptsToAdd;
	//ptsToAdd = myBasis.getVertices();
	
	
	if (myBasis.getVertices().size() > 0){
		
		vector< Point2f > points;
		vector < ofPoint > pts = myBasis.getVertices();
		for(int i = 0; i < pts.size(); i++ ){
			Point2f pt;
			pt.x = pts[i].x;
			pt.y =  pts[i].y;
			points.push_back(pt);
		}
		
		vector<int> hull;
		convexHull(Mat(points), hull, CV_CLOCKWISE);
		
		for (int i = 0; i < hull.size(); i++){
			ptsToAdd.push_back(  ofPoint( points[hull[i]].x, points[hull[i]].y) );
		}
	}
	
	
	
	
	
	
	
	in.numberofpoints = ptsToAdd.size();
	for (int i = 0; i < holes.size(); i++){
		in.numberofpoints += holes[i].size(); 
	}
	//in.numberofpoints += myBasis.getVertices().size();
	
	in.numberofpointattributes = 0;
	in.pointlist = (double *) malloc(in.numberofpoints * 2 * sizeof(double));
	in.pointattributelist = NULL;
	in.pointmarkerlist = (int *) malloc(sizeof(int) * in.numberofpoints);
	for (int i = 0 ; i < in.numberofpoints ; i++)
		in.pointmarkerlist [i] = 1;
	
	in.numberofholes =  holes.size();
	if (in.numberofholes)
		in.holelist = (double *) malloc(2 * in.numberofholes * sizeof(double));
	else
		in.holelist = NULL;
	
	for (int i = 0; i < holes.size(); i++){
		ofPoint midPt;
		for (int j = 0; j < holes[i].size(); j++){
			midPt += holes[i][j];
		}
		midPt /= holes[i].size();
		in.holelist[i*2+0] = midPt.x;
		in.holelist[i*2+1] = midPt.y;
	}
	
	
	in.numberofregions = 0;
	in.numberofsegments = ptsToAdd.size();
	for (int i = 0; i < holes.size(); i++){
		in.numberofsegments += holes[i].size(); 
	}
	
	in.segmentlist = (int *) malloc(2 * in.numberofsegments * sizeof(int));
	in.segmentmarkerlist = (int *) malloc(sizeof(int) * in.numberofsegments);
	for (int i = 0 ; i < in.numberofsegments ; i++)
		in.segmentmarkerlist [i] = 1;
	
	int n = 0;
	int m = 0;
	int count = 0;
	for (int j = 0 ; j < ptsToAdd.size() ; j++) {
		in.pointlist	[n]     = ptsToAdd[j].x;
		in.pointlist	[n + 1] = ptsToAdd[j].y;
		in.segmentlist	[n]     = m;
		in.segmentlist	[n + 1] = (m+1)%ptsToAdd.size();
		n += 2;
		m += 1;
	}
	count = n;
	for (int i = 0; i < holes.size(); i++){
		m = 0;
		for (int j = 0 ; j < holes[i].size() ; j++) {
			in.pointlist	[n]     = holes[i][j].x;
			in.pointlist	[n + 1] = holes[i][j].y;
			in.segmentlist	[n]     = count/2 + m;
			in.segmentlist	[n + 1] = count/2 + (m+1)%holes[i].size();
			n += 2;
			m += 1;
		}
		count = n;
	}
	
	//for (int j = 0 ; j < myBasis.getVertices().size() ; j++) {
	//		in.pointlist	[count]     = myBasis.getVertices()[j].x;
	//		in.pointlist	[count + 1] = myBasis.getVertices().y;
	//		count +=2;
	//	}
	//	
	//	
	
	
	
	out.pointlist                  = (double *) NULL;
	out.pointattributelist         = (double *) NULL;
	out.pointmarkerlist            = (int *) NULL;
	out.trianglelist               = (int *) NULL;
	out.numberoftriangleattributes = 0;
	out.triangleattributelist      = (double *) NULL;
	out.segmentmarkerlist          = (int *) NULL;
	out.segmentlist                = (int *) NULL;
	
	char opts[100];
	double fAcrit = 0.2;
	
	sprintf (opts, "pqa500z");		// <------ magic happens here ! 
	//sprintf (opts, "pq36z");		// <------ magic happens here ! 
	
	triangulate(opts, &in, &out, NULL);
	
	int ne = out.numberoftriangles;
	int nn = out.numberofpoints;
	if (ne <= 0 || nn <= 0) {
		//printf ("nothing to generate \n");
		//return 1;
	}
	
	for ( int i = 0; i < out.numberofpoints; i++ ) {
		ptsList.push_back(ofPoint(out.pointlist[i * 2 + 0], out.pointlist[i * 2 + 1]));
	}
	
	for ( int i = 0; i < out.numberoftriangles; i++ ){
		trianglePts temp;
		temp.a =  out.trianglelist[i * 3 + 0];
		temp.b =  out.trianglelist[i * 3 + 1];
		temp.c =  out.trianglelist[i * 3 + 2];
		triangles.push_back(temp);
	}
	
	
	for ( int i = 0; i < ptsList.size(); i++ ) {
		int insideOf = -1;
		for (int j = 0; j < myBasis.getVertices().size()/3; j++){
			
			
			if (PointInTriangle(ptsList[i], 
													myBasis.getVertices()[j*3+0], 
													myBasis.getVertices()[j*3+1],
													myBasis.getVertices()[j*3+2])){
				insideOf = j; 
			}
		}
		
		if (insideOf == -1){
			
			// ok this is a hack, but some points are not inside of mesh triangles, let's find something "close" and roll with that. 
			
			float minDist = 100000000;
			for (int j = 0; j < myBasis.getVertices().size()/3; j++){
				
				ofPoint midPt = (myBasis.getVertices()[j*3+0], 
												 myBasis.getVertices()[j*3+1],
												 myBasis.getVertices()[j*3+2]) / 3;
				
				if ( (ptsList[i] - midPt).length() < minDist){
					minDist = 	(ptsList[i] - midPt).length();
					insideOf = j;
				}
				
				//for (int k = 0; k < 3; k++){
				//					if ( (ptsList[i] - myBasis.getVertices()[j*3+k]).length() < minDist){
				//						minDist = 	(ptsList[i] - midPt).length();
				//						insideOf = j;
				//					}
				//				}
			}
			
		}
		
		float a,b,c;
		CalcBarycentric(myBasis.getVertices()[insideOf*3+0],
										myBasis.getVertices()[insideOf*3+1],
										myBasis.getVertices()[insideOf*3+2],ptsList[i], a,b,c);
		
		//cout << "----------" << endl;
		//		cout << ptsList[i] << "\n" << myBasis.getVertices()[insideOf*3+0] << "\n" << 
		//		myBasis.getVertices()[insideOf*3+1] << "\n" <<
		//		myBasis.getVertices()[insideOf*3+2] << endl;
		//		cout << "----------" << endl;
		
		barycentricInfo BI;
		BI.whichTriangle = insideOf;
		BI.a = a;
		BI.b = b;
		BI.c = c;
		
		ptsListBarycentric.push_back(BI);
	}
	
}

void meshPointAdder::draw(){
	
	ofSeedRandom(0);
	
	ofFill();
	
	
	
	for (int i = 0; i < triangles.size(); i++){
		
		ofSetHexColor(ofRandom(0,1)*0xFFFFFF);
		ofTriangle(ptsList[triangles[i].a], 
							 ptsList[triangles[i].b],
							 ptsList[triangles[i].c]);
		
	}
	
	
	
}



ofMesh  meshPointAdder::returnMeshWarpedToThisMesh(ofMesh & meshToMatch){
	
	// this could / should be optimized;
	
	ofMesh newMesh; 
	
	vector < ofPoint > newPoints;
	vector < ofPoint > newTexPoints;
	
	for (int i = 0; i < ptsList.size(); i++){
		int who = ptsListBarycentric[i].whichTriangle;
		float a = ptsListBarycentric[i].a;
		float b = ptsListBarycentric[i].b;
		float c = ptsListBarycentric[i].c;
		
		ofPoint newPos = ApplyBarycentric(
																			meshToMatch.getVertices()[who*3+0],
																			meshToMatch.getVertices()[who*3+1],
																			meshToMatch.getVertices()[who*3+2],a,b,c);
		ofVec2f newTex = ApplyBarycentric(
																			meshToMatch.getTexCoord(who*3+0),
																			meshToMatch.getTexCoord(who*3+0),
																			meshToMatch.getTexCoord(who*3+0),a,b,c);
		
		newPoints.push_back(newPos);
		newTexPoints.push_back(newTex);
		
	}
	
	ofMesh objectMesh;
	for(int i = 0; i < triangles.size(); i++){
		
		ofVec3f p1 = newPoints[triangles[i].a];
		ofVec3f p2 = newPoints[triangles[i].b];
		ofVec3f p3 = newPoints[triangles[i].c];
		
		objectMesh.addVertex(p1);
		objectMesh.addVertex(p2);
		objectMesh.addVertex(p3);
		
		ofVec3f t1 = newTexPoints[triangles[i].a];
		ofVec3f t2 = newTexPoints[triangles[i].b];
		ofVec3f t3 = newTexPoints[triangles[i].c];
		
		objectMesh.addTexCoord(t1);
		objectMesh.addTexCoord(t2);
		objectMesh.addTexCoord(t3);
	}
	return objectMesh;
	
}
