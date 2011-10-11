

#include "ofMain.h"


inline  ofPoint ApplyBarycentric( ofPoint a, ofPoint b, ofPoint c, float alpha, float beta, float gamma ){
    
	ofPoint transform;
	
	transform.x = alpha*a.x + beta*b.x + gamma*c.x;
	transform.y = alpha*a.y + beta*b.y + gamma*c.y;
	transform.z = alpha*a.z + beta*b.z + gamma*c.z;
	
    return transform;
}

inline  ofVec2f ApplyBarycentric( ofVec2f a, ofVec2f b, ofVec2f c, float alpha, float beta, float gamma ){
    
	ofVec2f transform;
	
	transform.x = alpha*a.x + beta*b.x + gamma*c.x;
	transform.y = alpha*a.y + beta*b.y + gamma*c.y;
	
    return transform;
}



inline void CalcBarycentric( ofPoint a, ofPoint b, ofPoint c, ofPoint p, float &alpha, float &beta, float &gamma){
	
	ofPoint v1 = a;
	ofPoint v2 = b;
	ofPoint v3 = c;
	
    float denom =
	( v2.x - v1.x ) * ( v3.y - v1.y ) -
	( v3.x - v1.x ) * ( v2.y - v1.y );
    
	
	alpha = ( ( v2.x - p.x ) * ( v3.y - p.y ) -
			 ( v3.x - p.x ) * ( v2.y - p.y ) ) / denom;
	beta = ( ( v3.x - p.x ) * ( v1.y - p.y ) -
			( v1.x - p.x ) * ( v3.y - p.y ) ) / denom;
	gamma = ( ( v1.x - p.x ) * ( v2.y - p.y ) -
			 ( v2.x - p.x ) * ( v1.y - p.y ) ) / denom;
}
