#ifndef SPRING_H
#define SPRING_H

#include "ofMain.h"
//#define OF_ADDON_USING_OFXVECTORMATH
//#include "ofAddons.h"

#include "particle.h"

class spring {

	public:

		spring();
		
		particle * particleA;
		particle * particleB;
		
		float distance;
		float springiness;	 // this is the k, springiness constant
	
		void update();
		void draw();
	
	
};


#endif