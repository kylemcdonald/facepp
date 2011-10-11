/*
 *  triangleUtils.cpp
 *  emptyExample
 *
 *  Created by zachary lieberman on 10/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "triangleUtils.h"


float sign(ofPoint p1, ofPoint p2, ofPoint p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle(ofPoint pt, ofPoint v1, ofPoint v2, ofPoint v3)
{
	
	// this is hack to handle point on point in a triangle. 
	if ((pt-v1).length() < 0.0001 ||
		(pt-v2).length() < 0.0001 ||
		(pt-v3).length() < 0.0001){
		return true;
	}
	
	bool b1, b2, b3;
	
	b1 = sign(pt, v1, v2) < 0.0f;
	b2 = sign(pt, v2, v3) < 0.0f;
	b3 = sign(pt, v3, v1) < 0.0f;
	
	return ((b1 == b2) && (b2 == b3));
}
