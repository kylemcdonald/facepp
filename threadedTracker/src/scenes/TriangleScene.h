#pragma once

#include "ofMain.h"
#include "meanMeshRenderer.h"
#include "ofxFaceTracker.h"



class triangleScene {

	public:
	
		
		void update(ofxFaceTracker & tracker);
		void draw(ofTexture & cam); 
	
		float overallEnergy;
		ofMesh imageMesh; 
		ofFbo temp;
		
		
	
};