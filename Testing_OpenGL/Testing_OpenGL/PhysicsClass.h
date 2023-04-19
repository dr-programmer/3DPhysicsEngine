#ifndef PHYSICSCLASS_H
#define PHYSICSCLASS_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm/glm.hpp>

struct Particle {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 color;
	float life;
	Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
};

class Physics {
protected:
	glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);
	glm::vec3 collision = glm::vec3(1.0f);
	std::vector<glm::vec3> forces;
	std::vector<Particle>* particles;
	unsigned int maxNumParticles;
	unsigned int ppf;
	glm::vec3 particleSpawnPos;
	unsigned int lastUnusedParticle = 0;

public:
	glm::vec3 position;
	float mass;
	glm::vec3 resultVelocity;

	Physics(glm::vec3 startPosition, float mass);
	Physics(std::vector<Particle>* particles, unsigned int number, unsigned int ppf, float mass);

	unsigned int gravity();
	void checkCollisions();
	unsigned int createForce(glm::vec3 force);
	glm::vec3 changeForce(unsigned int forceIndex, glm::vec3 force);
	void applyResultForce(float deltaTime);
	void applyResultForce(Particle& particle, float deltaTime);
	void createParticleSpawn(glm::vec3 position);
	void spawnParticles(float deltaTime);
	unsigned int firstUnusedParticle();
	void respawnParticle(Particle& particle);
};

Physics::Physics(glm::vec3 startPosition, float mass) {
	this->position = startPosition;
	this->mass = mass;
}

Physics::Physics(std::vector<Particle>* particles, unsigned int number, unsigned int ppf, float mass) {
	this->particles = particles;
	this->mass = mass;
	this->ppf = ppf;
	for (int i = 0; i < number; i++) {
		particles->push_back(Particle());
	}
	this->maxNumParticles = number;
	createForce(glm::vec3(0.0f, 0.1f, 0.0f));
}

unsigned int Physics::gravity() {
	glm::vec3 gravitaionalForce = g * this->mass;
	forces.push_back(gravitaionalForce);
	unsigned int size = forces.size();
	size--;
	return size;
}

void Physics::checkCollisions() {
	if (this->position.y <= -6) {
		this->collision.y = 0.0f;
	}
	else {
		this->collision.y = 1.0f;
	}
}

unsigned int Physics::createForce(glm::vec3 force) {
	forces.push_back(force);
	unsigned int size = forces.size();
	size--;
	return size;
}

glm::vec3 Physics::changeForce(unsigned int forceIndex, glm::vec3 force) {
	glm::vec3 oldForceValue = forces[forceIndex];
	forces[forceIndex] = force;
	return oldForceValue;
}

void Physics::applyResultForce(float deltaTime) {
	checkCollisions();

	glm::vec3 resultForce = glm::vec3(0.0f);
	for (int i = 0; i < forces.size(); i++) {
		resultForce += forces[i];
	}
	glm::vec3 a = resultForce / this->mass;
	glm::vec3 oldResultVelocity = resultVelocity;
	resultVelocity += a * deltaTime;
	resultVelocity *= collision;
	glm::vec3 avgResultVelocity = oldResultVelocity + resultVelocity;
	avgResultVelocity /= 2;
	this->position += avgResultVelocity * deltaTime;
}

void Physics::applyResultForce(Particle& particle, float deltaTime) {
	checkCollisions();

	glm::vec3 resultForce = glm::vec3(0.0f);
	for (int i = 0; i < forces.size(); i++) {
		resultForce += forces[i];
	}
	glm::vec3 a = resultForce / this->mass;
	glm::vec3 oldResultVelocity = particle.velocity;
	particle.velocity += a * deltaTime;
	particle.velocity *= collision;
	glm::vec3 avgResultVelocity = oldResultVelocity + particle.velocity;
	avgResultVelocity /= 2;
	particle.position += avgResultVelocity * deltaTime;
}

void Physics::createParticleSpawn(glm::vec3 position) {
	this->particleSpawnPos = position;
	for (unsigned int i = 0; i < this->maxNumParticles; i++) {
		Particle& p = (*particles)[i];
		p.position = this->particleSpawnPos;
	}
}

void Physics::spawnParticles(float deltaTime) {
	for (unsigned int i = 0; i < this->ppf; i++) {
		unsigned int unusedParticle = firstUnusedParticle();
		respawnParticle((*particles)[unusedParticle]);
		this->lastUnusedParticle = unusedParticle;
	}

	for (unsigned int i = 0; i < this->maxNumParticles; i++) {
		Particle& p = (*particles)[i];
		p.position.x = (float)i / 100;
		p.life -= deltaTime;
		p.color -= deltaTime;
		if (p.life > 0.0f) {
			//this->position = p.position;
			applyResultForce(p, deltaTime);
			//p.position = this->position;
		}
	}
}

unsigned int Physics::firstUnusedParticle() {
	for (unsigned int i = this->lastUnusedParticle; i < this->maxNumParticles; i++) {
		if ((*particles)[i].life <= 0.0f) {
			return i;
		}
	}
	for (unsigned int i = 0; i < this->maxNumParticles; i++) {
		if ((*particles)[i].life <= 0.0f) {
			return i;
		}
	}
	return 0;
}

void Physics::respawnParticle(Particle& particle) {
	particle.life = 1.0f;
	particle.color = glm::vec4(1.0f);
	particle.velocity = glm::vec3(0.0f);
	particle.position = this->particleSpawnPos;
}

#endif