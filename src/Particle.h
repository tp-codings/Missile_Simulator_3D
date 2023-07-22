#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Particle
{
public:
	Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale);
	glm::vec3 getPosition();
	float getRotation();
	float getScale();
	bool update(float deltaTime);

private:
	glm::vec3 position;
	glm::vec3 velocity;
	float gravityEffect;
	float lifeLength;
	float rotation;
	float scale;
	float elapsedTime;

};

