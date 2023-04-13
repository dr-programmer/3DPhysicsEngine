#ifndef PHYSICSCLASS_H
#define PHYSICSCLASS_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm/glm.hpp>

class Physics {
protected:
	const float g = 9.8f;
	glm::vec3 collision = glm::vec3(1.0f);
	std::vector<glm::vec3> forces;

public:
	glm::vec3 position;
	float mass;
	glm::vec3 resultVelocity;

	Physics(glm::vec3 startPosition, float mass);

	void gravity(float deltaTime);
	void checkCollisions();
	unsigned int createForce(glm::vec3 force);
	glm::vec3 changeForce(unsigned int forceIndex, glm::vec3 force);
	void applyResultForce(float deltaTime);
};

Physics::Physics(glm::vec3 startPosition, float mass) {
	this->position = startPosition;
	this->mass = mass;
}

void Physics::gravity(float deltaTime) {
	checkCollisions();

	static float velocity = 0;
	float oldVelocity = velocity * collision.y;
	velocity = (velocity + this->g * deltaTime) * collision.y;
	float averageVelocity = (velocity + oldVelocity) / 2;
	this->position.y -= averageVelocity * deltaTime;
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
	glm::vec3 resultForce = glm::vec3(0.0f);
	for (int i = 0; i < forces.size(); i++) {
		resultForce += forces[i];
	}
	glm::vec3 a = resultForce / this->mass;
	static glm::vec3 resultVelocity = glm::vec3(0.0f);
	glm::vec3 oldResultVelocity = resultVelocity;
	resultVelocity += a * deltaTime;
	resultVelocity *= collision;
	glm::vec3 avgResultVelocity = oldResultVelocity + resultVelocity;
	avgResultVelocity /= 2;
	this->position += avgResultVelocity * deltaTime;
}

#endif