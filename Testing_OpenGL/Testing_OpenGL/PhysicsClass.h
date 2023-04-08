#ifndef PHYSICSCLASS_H
#define PHYSICSCLASS_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>

class Physics {
protected:
	float velocity;
	const float g = 9.8f;
	bool collision = false;

public:
	glm::vec3 position;

	Physics(glm::vec3 startPosition);

	void gravity(float deltaTime);
	void checkCollisions();
};

Physics::Physics(glm::vec3 startPosition) {
	velocity = 0;
	position = startPosition;
}

void Physics::gravity(float deltaTime) {
	checkCollisions();
	if (!collision) {
		velocity += g * deltaTime;
		position.y -= velocity * deltaTime;
	}
	else {
		velocity = 0;
	}
}

void Physics::checkCollisions() {
	if (position.y <= -6) {
		collision = true;
	}
}

#endif