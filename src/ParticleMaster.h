/**************************************************************
 *  Name: ParticleMaster.h
 *  Author: Tobias Paul
 *  Date: 25.07.2023
 *  Description:
	The ParticleMaster class manages particles, such as those created by a missile.
	Particles are positioned in 3D space using a given start position and can move in a specific direction.
	Their appearance can be customized with textures using a particle texture handler.
	The class provides methods to update particle states, retrieve their current positions, colors, directions, and velocities.
	Additionally, it allows adding particles with specific properties such as direction, position, velocity, and type.
 **************************************************************/

#ifndef PARTICLEMASTER_H
#define PARTICLEMASTER_H

#include <vector>
#include <unordered_map>
#include "Particle.h"
#include "ParticleRenderer.h"
#include <Camera/camera.h>
#include "ParticleTextureHandler.h"

class ParticleMaster
{
public:
	// Constructor to create the ParticleMaster
	ParticleMaster();

	// Update all particles and remove dead particles
	void update(float deltaTime, Camera* camera);

	// Render all particles using the provided projection and camera matrices
	void render(glm::mat4 projection, Camera& camera);

	// Add a particle to the ParticleMaster for management
	void addParticle(Particle* particle);

private:
	// A container to store particles grouped by their texture IDs
	std::unordered_map<int, std::vector<Particle*>> particles;

	// ParticleRenderer for rendering particles
	ParticleRenderer renderer;

	// Sorting function to sort particles based on their distance to the camera
	void InsertionSortHighToLow(std::vector<Particle*>& list);
};

#endif // PARTICLEMASTER_H
