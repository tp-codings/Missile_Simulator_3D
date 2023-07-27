#pragma once
#include <vector>
#include <unordered_map>
#include "Particle.h"
#include "ParticleRenderer.h"
#include <Camera/camera.h>
#include "ParticleTextureHandler.h"

class ParticleMaster
{
public: 
	ParticleMaster();
	void update(float deltaTime);
	void render(glm::mat4 projection, Camera& camera);
	void addParticle(Particle* particle);

	int getParticlesAlive();

private:

	std::unordered_map<int, std::vector<Particle*>> particles;


	ParticleRenderer renderer;


};

