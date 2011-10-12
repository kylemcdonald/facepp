#include "NoiseParticle.h"

vector<Force>
	NoiseParticle::forces;
	
vector<NoiseParticle>
	NoiseParticle::particles;

ofVec2f
	NoiseParticle::centeringForce,
	NoiseParticle::globalOffset,
	NoiseParticle::externalForce;

float
	NoiseParticle::speed,
	NoiseParticle::spread,
	NoiseParticle::viscosity,
	NoiseParticle::independence,
	NoiseParticle::neighborhood,
	NoiseParticle::confusion,
	NoiseParticle::indecision,
	NoiseParticle::age,
	NoiseParticle::globalDirection;

void NoiseParticle::setup(int n, ofVec2f origin, float radius) {
	globalOffset.set(1. / 3, 2. / 3);
	speed = 1;
  spread = 25;
  viscosity = .1;
  independence = 0.7;
  neighborhood = 200;
	confusion = .01;
	indecision = .01;
	age = 0;
	globalDirection = 0;
	
	for(int i = 0; i < n; i++) {
		NoiseParticle::particles.push_back(NoiseParticle(origin, radius));
	}
}

void NoiseParticle::drawAllPoints() {
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	for(int i = 0; i < particles.size(); i++) {
		mesh.addVertex(particles[i].position);
	}
	mesh.drawVertices();
}

void NoiseParticle::drawAllLines() {
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	for(int i = 0; i < particles.size(); i++) {
		mesh.clear();
		list<ofVec2f>& trail = particles[i].trail;
		int size = trail.size();
		list<ofVec2f>::iterator it;
		int j = 0;	// wow, crazy bug
		for(it = trail.begin(); it != trail.end(); it++) {
			mesh.addVertex(*it);
			mesh.addColor(ofFloatColor(1.*0.3, ((float) j / size) * particles[i].energy));
			j++;
		}
		mesh.draw();
	}
}

bool isInvisible(const NoiseParticle& p) {
	return !p.visible || p.energy < 0.001;
}


void NoiseParticle::updateAll(float dt) {
	
	for(int i = 0; i < particles.size(); i++) {
		particles[i].update(dt);
	}

//--------- for osc audio
	ofVec2f stdDevVelocity(0,0);
	ofVec2f avgVelocity(0,0);
	int nVisible = 0;
	stdDevVelocity.set(0,0);
	
	for(int i = 0; i < particles.size(); i++) {
		if (particles[i].visible) {
			avgVelocity += particles[i].velocity;
			nVisible++;
		}
	}
	if (nVisible > 0){
		avgVelocity /= (float)nVisible;
	}
	ofVec2f diffVelocity;
	
	for (int i = 0; i < particles.size(); i++){
		if (particles[i].visible == true){
			diffVelocity = particles[i].velocity - avgVelocity;
			stdDevVelocity += diffVelocity * diffVelocity;
		}
	}

	if (nVisible > 0){
		stdDevVelocity /= (float)nVisible;
		stdDevVelocity.x = sqrt(stdDevVelocity.x);
		stdDevVelocity.y = sqrt(stdDevVelocity.y);
	}
	
	ofxDaito::bangVec2f("noiseParticleStdDevVelocity", stdDevVelocity);
	ofxDaito::bangVec2f("avgVelocity", avgVelocity);
	ofxDaito::bang("numNoiseParticles", nVisible);
	
		
//--------- for osc audi0 
	
	
	globalDirection += ofSignedNoise(indecision * age);
	ofVec2f direction(0, 1);
	direction.rotate(globalDirection);
	globalOffset += confusion * direction * dt;
	age += dt;
	forces.clear();
	ofRemove(particles, isInvisible);
}

void NoiseParticle::setLeaving(bool leaving) {
	for(int i = 0; i < particles.size(); i++) {
		particles[i].leaving = leaving;
	}
}

void NoiseParticle::addForce(ofVec2f position, float magnitude) {
	forces.push_back(Force(position, magnitude));
}