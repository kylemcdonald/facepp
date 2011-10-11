

// I draw into an FBO the mean mesh, to be used as a mask, etc. 
// you can draw into me too, and I can draw myself on the face. 
// I also grab the depth as a blurry image, and for that reason, have a 
// corresponding MPA mesh if you need to do lookups (like the light scene)

#pragma once


#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
#include "meshForeheadAdder.h"



class meanMeshRenderer {
public:
	
	void setup();
	void setupMPAMesh(); // needs to be called after mpa is setup
	void update();
	void draw();
	
	ofxFaceTracker	stillTracker;
	ofImage			still;
	ofImage			stillDepth;
	ofMesh			meanMesh;
	
	ofFbo			meanMeshFbo;
	ofFbo			meanMeshAsDepthFbo;
	int				width;
	int				height;
	
	void draw(ofxFaceTracker & tracker);
	
	void clear();
	
	// depth related stuff 
	
	ofPixels		pixelsDepth;
	ofImage			depthImage;
	ofMesh			meanMeshThroughMPA;		// this is useful, for depth of MPA, etc. 
	
	
};
