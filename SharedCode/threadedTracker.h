#pragma once


#include "ofMain.h"
#include "ofxFaceTracker.h"
#include "ofxCv.h"

//using namespace ofxCv;


class threadedTracker : public ofThread{
	
public:
	
	
	unsigned char * pixelForTracker;
	unsigned char * pixelInThreadForTracker;
	
	ofPoint pts[66];
	
	ofxFaceTracker * tracker;
	ofxFaceTracker * trackerInThread;
	
	ofPixels colorPix;
	
	
	bool bHavePixels;
	bool bResetTracker;
	float scale;
	
	int nFramesGotOne;
	
	
	void setup(){
		
		// having this in the constructor failed because ofToDataPath() fails pre setup();
		// why?
		
		bHavePixels = false;
		tracker = new ofxFaceTracker();
		tracker->setup();
		tracker->setRescale(.5);
		
		bResetTracker = false;
		
		colorPix.allocate(640, 480, OF_PIXELS_RGB);
		pixelForTracker = new unsigned char[640*480*3];
		pixelInThreadForTracker = new unsigned char[640*480*3];
		
		scale = 0.5;
		
		nFramesGotOne = 0;
		
	}
	//--------------------------
	threadedTracker(){
		// nada
	}
	~threadedTracker(){
		stop();
		ofSleepMillis(50);
		//delete tracker;
		//delete trackerInThread;
	}
	
	void setScale(float _scaleIn){
		scale = _scaleIn;	
	}
	
	void start(){
		startThread(true, false);   // blocking, verbose
	}
	
	void stop(){
		stopThread();
	}
	
	//--------------------------
	void threadedFunction(){

		// make the tracker in this thread, helpful?
		trackerInThread = new ofxFaceTracker();
		trackerInThread->setup();
		trackerInThread->setRescale(scale);
		float myScale = scale;
		
		ofPoint myPts[66];
		
		while( isThreadRunning() != 0 ){
			if (bHavePixels == true){
				if( lock() ){
					memcpy(pixelInThreadForTracker, pixelForTracker, 640*480*3);
					unlock();
				}
				
				colorPix.setFromExternalPixels(pixelInThreadForTracker, 640, 480, 3);
				trackerInThread->update(ofxCv::toCv(colorPix));
				if (trackerInThread->getFound()){
					nFramesGotOne++;
				} else {
					nFramesGotOne = 0;	
				}
				
				if (bResetTracker == true){
					trackerInThread->reset();
					bResetTracker = false;
				}

				if (fabs(myScale - scale) > 0.05){
					delete trackerInThread;
					trackerInThread = new ofxFaceTracker();
					trackerInThread->setup();
					trackerInThread->setRescale(scale);
					myScale = scale;
				}
				
				if (trackerInThread->getFound()){
					if (trackerInThread->getImageMesh().getVertices().size() == 66){
						for (int i = 0; i < 66; i++){
							myPts[i] = 	trackerInThread->getImageMesh().getVertices()[i];
						}
					}
				}
				
				if( lock() ){
					//printf("copy \n");
					memcpy(pts, myPts, 66*sizeof(ofPoint));
					//memcpy(tracker, trackerInThread, sizeof(ofxFaceTracker));
					unlock();
				}
				ofSleepMillis(80);
				//printf("-- tracked --\n");
			} else {
				ofSleepMillis(10);
			}
		}
		
		delete trackerInThread;
	}
	
	//--------------------------
	void copyPixels(unsigned char * input){
		
		if( lock() ){
			memcpy(pixelForTracker, input, 640*480*3);
			bHavePixels = true;
			unlock();
		}
	}
	
	void setReset(){
		bResetTracker = true;	
	}
	
	void copyPts(ofPoint * ptsToCopyTo){
		if (lock()){
			memcpy(ptsToCopyTo, pts, 66*sizeof(ofPoint));
			unlock();
		}
	}
	
	void copyFaceTracker(ofxFaceTracker & trackerOut){
		// could this be a different lock?
		if (lock()){
			//memcpy(&trackerOut, tracker); 
			memcpy(&trackerOut, tracker, sizeof(ofxFaceTracker));
			unlock();
		}
	}
	
	
	
};
