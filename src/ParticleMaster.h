#pragma once
#include <vector>
#include "Particle.h"
#include "ParticleRenderer.h"
#include <Camera/camera.h>

class ParticleMaster
{
public: 
	ParticleMaster();
	void update(float deltaTime);
	void render(glm::mat4 projection, Camera& camera);
	void addParticle(Particle* particle);

	int getParticlesAlive();

private:
	std::vector<Particle*> particles;
	ParticleRenderer renderer;


};

