/*
 *  useful.h
 *  BasicExample
 *
 *  Created by zachary lieberman on 10/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"


// this for the odd thing that crept into testApp, cleaning up.  

// stuff that doesn't belong to a class, but is useful across the board. 

string ZeroPadNumber(int num);

ofMesh convertFromIndices(const ofMesh& mesh);
ofMesh convertToIndices(const ofMesh& mesh);

ofPoint getCentroid2D( ofPolyline temp);
