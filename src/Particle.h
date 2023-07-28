/**************************************************************
 *  Name: Particle.h
 *  Author: Tobias Paul
 *  Date: 25.07.2023
 *  Description:
	The Particle class represents a particle used in particle effects, such as those created by a turret.
	It is positioned in 3D space using a given start position and can move in a specific direction.
	The particle's appearance can be customized with a color.
	The class provides methods to update the particle's state, retrieve its current position, color, direction, and velocity.
	Additionally, it allows setting the direction, position, velocity, and type of the particle.
 **************************************************************/
#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <Camera/camera.h>

#include "ParticleTextureHandler.h"

class Particle
{
public:
	// Constructor to create a particle with given properties
	Particle(ParticleTextureHandler particleTexture, glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale);

	// Getters for retrieving particle properties
	glm::vec3 getPosition();
	float getRotation();
	float getScale();
	float getElapsedTime();
	float getLifeLength();
	float getDistanceToCamera();
	bool update(float deltaTime, Camera* camera);

	// Getters for particle texture information
	ParticleTextureHandler getTexture();
	glm::vec2 getTexOffset1();
	glm::vec2 getTexOffset2();
	float getBlendFactor();

private:
	glm::vec3 position;         // Current position of the particle
	glm::vec3 velocity;         // Current velocity of the particle
	float gravityEffect;        // Effect of gravity on the particle's movement
	float lifeLength;           // Total lifespan of the particle
	float rotation;             // Current rotation angle of the particle
	float scale;                // Current scale of the particle
	float elapsedTime;         // Time elapsed since the particle's creation
	float distance;             // Distance of the particle to the camera
	ParticleTextureHandler texture;  // Particle texture handler
	glm::vec2 texOffset1;       // Texture offset for the first texture atlas
	glm::vec2 texOffset2;       // Texture offset for the second texture atlas
	float blendFactor;          // Blend factor for interpolating between textures

	// Update the texture coordinate information based on the particle's age
	void updateTextureCoordInfo();

	// Set the texture offset based on the given index (for texture atlas)
	void setTextureOffset(glm::vec2* offset, int index);
};

#endif // PARTICLE_H
