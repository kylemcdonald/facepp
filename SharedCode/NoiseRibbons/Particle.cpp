#include "Particle.h"

vector<Force>
	Particle::forces;
	
vector<Particle>
	Particle::particles;

ofVec2f
	Particle::centeringForce,
	Particle::globalOffset,
	Particle::externalForce;

float
	Particle::speed,
	Particle::spread,
	Particle::viscosity,
	Particle::independence,
	Particle::neighborhood,
	Particle::confusion,
	Particle::indecision,
	Particle::age,
	Particle::globalDirection;

void Particle::setup(int n, ofVec2f origin, float radius) {
	globalOffset.set(1. / 3, 2. / 3);
	speed = 1;
  spread = 25;
  viscosity = .1;
  independence = 1;
  neighborhood = 100;
	confusion = .01;
	indecision = .01;
	age = 0;
	globalDirection = 0;
	
	for(int i = 0; i < n; i++) {
		Particle::particles.push_back(Particle(origin, radius));
	}
}

void Particle::drawAllPoints() {
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	for(int i = 0; i < particles.size(); i++) {
		mesh.addVertex(particles[i].position);
	}
	mesh.drawVertices();
}

void Particle::drawAllLines() {
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	for(int i = 0; i < particles.size(); i++) {
		mesh.clear();
		list<ofVec2f>& trail = particles[i].trail;
		int size = trail.size();
		list<ofVec2f>::iterator it;
		int i = 0;
		for(it = trail.begin(); it != trail.end(); it++) {
			mesh.addVertex(*it);
			mesh.addColor(ofFloatColor(1., (float) i / size));
			i++;
		}
		mesh.draw();
	}
}

bool isInvisible(const Particle& p) {
	return !p.visible;
}

void Particle::updateAll(float dt) {
	for(int i = 0; i < particles.size(); i++) {
		particles[i].update(dt);
	}
	globalDirection += ofSignedNoise(indecision * age);
	ofVec2f direction(0, 1);
	direction.rotate(globalDirection);
	globalOffset += confusion * direction * dt;
	age += dt;
	forces.clear();
	ofRemove(particles, isInvisible);
}

void Particle::setLeaving(bool leaving) {
	for(int i = 0; i < particles.size(); i++) {
		particles[i].leaving = leaving;
	}
}

void Particle::addForce(ofVec2f position, float magnitude) {
	forces.push_back(Force(position, magnitude));
}