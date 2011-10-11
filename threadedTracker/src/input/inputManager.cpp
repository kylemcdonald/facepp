/*
 *  inputManager.cpp
 *  BasicExample
 *
 *  Created by zachary lieberman on 10/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "inputManager.h"
#include "testApp.h"
#include "useful.h"
#include "ofxCv.h"

using namespace ofxCv;






void inputManager::setup() {
	
	bSaveImages = false;	// for live input, for making a movie.
	bFrameNew = false;
	
	inputType = file;
	
	if (inputType == file){
		video.loadMovie("face.mov");
		video.play();
		float scale = 480.0 / (float)video.height;
		pixelsSmaller.allocate(video.width*scale, video.height*scale, OF_IMAGE_COLOR);
	} else if (inputType == edsdk){
		camera.setup();
		float scale = 480.0 / (float)704.0;
		pixelsSmaller.allocate(1056*scale, 704*scale, OF_IMAGE_COLOR);
	} else {
		cam.initGrabber(640, 480);
	}

	pixelsSmallerTex.allocate(640,480, GL_RGB);
	pixelsToTrack = new unsigned char [640*480*3];
	
	smallAsOfPixels.allocate(640, 480, OF_PIXELS_RGB);
	
}

void inputManager::update() {
	
	static int count = 0;
	
	bFrameNew = false;

	
	if (inputType == file){
		video.update();
		if(video.isFrameNew()) {
			preparePixelsToTrack(video);
			bFrameNew = true;
		}
	} else if (inputType == edsdk){
		camera.update();
		if(camera.isFrameNew()) {
			preparePixelsToTrack(camera.getLivePixels());
			bFrameNew = true;
			if (bSaveImages == true){
				string fileName = ZeroPadNumber(count) + ".jpg";
				ofSaveImage(camera.getLivePixels(), fileName);
				count ++;
			}
		}
	} else {
		cam.update();
		if(cam.isFrameNew()) {
			//preparePixelsToTrack(video);
			memcpy(pixelsToTrack, cam.getPixels(), 640*480*3);
			pixelsSmallerTex.loadData(pixelsToTrack, 640, 480, GL_RGB); 
			bFrameNew = true;
		   if (bSaveImages == true){
			   string fileName = ZeroPadNumber(count) + ".jpg";
			   smallAsOfPixels.setFromExternalPixels(pixelsToTrack, 640, 480, 3);
			   ofSaveImage(	smallAsOfPixels	, fileName);
			   count ++;
		   }	
		}		
	}
}

void inputManager::draw() {
	ofSetColor(255);
	
	
	ofSetColor(255,255,255);
	pixelsSmallerTex.draw(0,0);
	

	
	//camera.draw(0, 0);
//	
//	if(camera.isLiveReady()) {
//		stringstream status;
//		status << camera.getWidth() << "x" << camera.getHeight() << " @ " <<
//		(int) ofGetFrameRate() << " app-fps " << " / " <<
//		(int) camera.getFrameRate() << " cam-fps";
//		ofDrawBitmapString(status.str(), 10, 20);
//	}
		
	
}




void inputManager::preparePixelsToTrack(ofPixels & pix){
	
	int offsetFor640 = (pixelsSmaller.width - 640)/2;
	cv::Mat src = toCv(pix);
	cv::Mat dst = toCv(pixelsSmaller);
	resize(src, dst, dst.size(), 0, 0, INTER_LINEAR);
	pixelsSmaller.update();
	unsigned char * colorPixels = pixelsSmaller.getPixels() + offsetFor640*3;
	int widthBytesSmaller = pixelsSmaller.width* 3;;
	for (int i = 0; i < 480; i++){
		memcpy(pixelsToTrack + i*640*3, colorPixels, 640*3);
		colorPixels += widthBytesSmaller;
	}
	pixelsSmallerTex.loadData(pixelsToTrack, 640, 480, GL_RGB); 
	
}


void inputManager::preparePixelsToTrack(ofBaseHasPixels & pix){
	
	
	//resize(input, pixelsSmaller);
	int offsetFor640 = (pixelsSmaller.width - 640)/2;
	
	cv::Mat src = toCv(pix);
	cv::Mat dst = toCv(pixelsSmaller);
	
	resize(src, dst, dst.size(), 0, 0, INTER_LINEAR);
	pixelsSmaller.update();
	
	unsigned char * colorPixels = pixelsSmaller.getPixels() + offsetFor640*3;
	int widthBytesSmaller = pixelsSmaller.width* 3;;
	for (int i = 0; i < 480; i++){
		memcpy(pixelsToTrack + i*640*3, colorPixels, 640*3);
		colorPixels += widthBytesSmaller;
	}
	
	pixelsSmallerTex.loadData(pixelsToTrack, 640, 480, GL_RGB); 
}