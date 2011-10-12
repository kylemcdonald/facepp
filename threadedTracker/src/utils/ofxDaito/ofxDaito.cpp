#include "ofxDaito.h"

ofxXmlSettings ofxDaito::xml;
vector<ofxOscSender> ofxDaito::senders;
ofxOscBundle bundle;	

void ofxDaito::send(ofxOscMessage& msg) {
	for(int i = 0; i < senders.size(); i++) {
		senders[i].sendMessage(msg);
	}
}

void ofxDaito::setup(string settings, bool verbose) {
	xml.setVerbose(verbose);
	if(xml.loadFile(settings)) {
		int receivers = xml.getNumTags("receiver");
		for(int i = 0; i < receivers; i++) {
			xml.pushTag("receiver", i);
			string host = xml.getValue("host", "localhost");
			int port = xml.getValue("port", 0);
			senders.push_back(ofxOscSender());
			senders.back().setup(host, port);
			cout << "Connected OSC to " << host << ":" << port << endl;
			xml.popTag();
		}
	} else {
		cout << "Couldn't load file " << settings << endl;
	}
}

void ofxDaito::sendCustom(ofxOscMessage &msg){
	send(msg);
}


void ofxDaito::clearBundle() {
	bundle.clear();
}

template <>
void ofxDaito::addMessage(string address, ofVec3f data) {
	ofxOscMessage msg;
	msg.setAddress(address);
	msg.addFloatArg(data.x);
	msg.addFloatArg(data.y);
	msg.addFloatArg(data.z);
	bundle.addMessage(msg);
}

template <>
void ofxDaito::addMessage(string address, ofVec2f data) {
	ofxOscMessage msg;
	msg.setAddress(address);
	msg.addFloatArg(data.x);
	msg.addFloatArg(data.y);
	bundle.addMessage(msg);
}

template <>
void ofxDaito::addMessage(string address, float data) {
	ofxOscMessage msg;
	msg.setAddress(address);
	msg.addFloatArg(data);
	bundle.addMessage(msg);
}

template <>
void ofxDaito::addMessage(string address, int data) {
	ofxOscMessage msg;
	msg.setAddress(address);
	msg.addIntArg(data);
	bundle.addMessage(msg);
}

void ofxDaito::sendBundle() {
	for(int i = 0; i < senders.size(); i++) {
		senders[i].sendBundle(bundle);
	}
}

void ofxDaito::bang(string eventName,
										float energy,
										float pan) {
	ofxOscMessage msg;
	msg.setAddress("/bang");
	msg.addStringArg(eventName);
	msg.addFloatArg(energy);
	msg.addFloatArg(pan);
	send(msg);
}

void ofxDaito::bang(string eventName){
	ofxOscMessage msg;
	msg.setAddress("/bang");
	msg.addStringArg(eventName);
	send(msg);	
}

void ofxDaito::bangVec3f(string eventName,
					ofVec3f _vec
					) {
	ofxOscMessage msg;
	msg.setAddress("/bang");
	msg.addStringArg(eventName);
	msg.addFloatArg(_vec.x);
	msg.addFloatArg(_vec.y);
	msg.addFloatArg(_vec.z);	
	send(msg);
}

void ofxDaito::bangVec2f(string eventName,
						 ofVec2f _vec
						 ) {
	ofxOscMessage msg;
	msg.setAddress("/bang");
	msg.addStringArg(eventName);
	msg.addFloatArg(_vec.x);
	msg.addFloatArg(_vec.y);
	send(msg);
}

void ofxDaito::bang(string eventName, int _val){
	ofxOscMessage msg;
	msg.setAddress("/bang");
	msg.addStringArg(eventName);
	msg.addIntArg(_val);
//	cout << "sending"<< endl;
	send(msg);
}

