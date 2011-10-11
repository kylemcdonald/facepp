

// I draw into an FBO the mean mesh, to be used as a mask, etc. 
// you can draw into me too, and I can draw myself on the face. 

#pragma once


#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"




class meanMeshRenderer {
public:
	
	void setup();
	void update();
	void draw();
	
	ofxFaceTracker	stillTracker;
	ofImage			still;
	ofMesh			meanMesh;
	
	ofFbo			meanMeshFbo;
	int				width;
	int				height;
	
	void draw(ofxFaceTracker & tracker);
	
	void clear();
	
	
};
