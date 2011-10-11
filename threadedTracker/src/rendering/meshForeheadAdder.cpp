/*
 *  meshForeheadAdder.cpp
 *  BasicExample
 *
 *  Created by zachary lieberman on 10/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "meshForeheadAdder.h"

void addForheadToFaceMesh(ofMesh & input){
	
	if (input.getVertices().size() != 66) return;
	
	static int forehead[10] = {0,17,18,19,20,/*21,22,*/23,24,25,26,16};
	ofPoint extras[10];
	ofVec2f texture[10];
	
	for (int i = 0; i < 10; i++){
		extras[i] = input.getVertices()[forehead[i]] + (input.getVertices()[27] - input.getVertices()[33]);
		texture[i] = input.getTexCoords()[forehead[i]] + (input.getTexCoords()[27] - input.getTexCoords()[33]);
		input.addVertex(extras[i]);
		input.addTexCoord(texture[i]);				 
	}
	
	for (int i = 0; i < (10-1); i++){
	
		// a b c
		// b c d;
		
		int a = forehead[i];
		int b = 66 + i;
		int c = forehead[i+1];
		
		input.addIndex(a);
		input.addIndex(b);
		input.addIndex(c);
		
		int d = 66 + i + 1;
		
		input.addIndex(b);
		input.addIndex(c);
		input.addIndex(d);
		
	}
	
	
	
	
	//printf("-- %i \n", input.getVertices().size());
	//std::exit(0);	
}
