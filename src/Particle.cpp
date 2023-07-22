#include "Particle.h"
#define GRAVITY 10

Particle::Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale):
	position(position), velocity(velocity), gravityEffect(gravityEffect), lifeLength(lifeLength), rotation(rotation), scale(scale)
{
	this->elapsedTime = 0.0f;
}

glm::vec3 Particle::getPosition()
{
	return this->position;
}

float Particle::getRotation()
{
	return this->rotation;
}

float Particle::getScale()
{
	return this->scale;
}

bool Particle::update(float deltaTime)
{
	this->velocity += GRAVITY * this->gravityEffect * deltaTime;
	this->position += this->velocity;
	this->elapsedTime += deltaTime;

	return elapsedTime < this->lifeLength;
}
