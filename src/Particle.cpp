#include "Particle.h"
#define GRAVITY -10

Particle::Particle(ParticleTextureHandler particleTexture, glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale, std::string type):
	texture(particleTexture), position(position), velocity(velocity), gravityEffect(gravityEffect), lifeLength(lifeLength), rotation(rotation), scale(scale), type(type)
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

float Particle::getElapsedTime()
{
	return this->elapsedTime;
}

float Particle::getLifeLength()
{
	return this->lifeLength;
}

std::string Particle::getType()
{
	return this->type;
}

bool Particle::update(float deltaTime)
{
	this->velocity *= (1 - deltaTime*0.2);
	this->velocity.y += GRAVITY * this->gravityEffect * deltaTime;
	this->position += this->velocity;
	this->elapsedTime += deltaTime;
	this->updateTextureCoordInfo();
	return elapsedTime < this->lifeLength;
}

ParticleTextureHandler Particle::getTexture()
{
	return this->texture;
}

glm::vec2 Particle::getTexOffset1()
{
	return this->texOffset1;
}

glm::vec2 Particle::getTexOffset2()
{
	return this->texOffset2;
}

float Particle::getBlendFactor()
{
	return this->blendFactor;
}
#include <iostream>
void Particle::updateTextureCoordInfo()
{
	float lifeFactor = this->elapsedTime / this->lifeLength;
	int stageCount = texture.getNumberOfRows() * texture.getNumberOfRows();
	float atlasProgression = lifeFactor * stageCount;
	int index1 = (int)floor(atlasProgression);
	int index2 = index1 < stageCount - 1 ? index1 + 1 : index1;
	this->blendFactor = atlasProgression - static_cast<int>(atlasProgression);
	setTextureOffset(&texOffset1, index1);
	setTextureOffset(&texOffset2, index2);
}

void Particle::setTextureOffset(glm::vec2* offset, int index)
{
	int col = index % this->texture.getNumberOfRows();
	int row = index / this->texture.getNumberOfRows();
	offset->x = (float)col / this->texture.getNumberOfRows();
	offset->y = (float)row / this->texture.getNumberOfRows();
}