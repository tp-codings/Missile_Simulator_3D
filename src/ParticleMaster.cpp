#include "ParticleMaster.h"
#include <iostream>

ParticleMaster::ParticleMaster()
{
	this->renderer = ParticleRenderer();
}

void ParticleMaster::update(float deltaTime)
{
    std::vector<int> removeMarker;

    for (size_t i = 0; i < this->particles.size(); ++i) {
        bool alive = this->particles[i]->update(deltaTime);
        if (!alive) {
            removeMarker.push_back(i);
        }
    }

    for (int i = removeMarker.size() - 1; i >= 0; --i) {
        int indexToRemove = removeMarker[i];
        this->particles.erase(this->particles.begin() + indexToRemove);
    }
}

void ParticleMaster::render(glm::mat4 projection, Camera& camera)
{
    this->renderer.render(this->particles, projection, camera);
}

void ParticleMaster::addParticle(Particle* particle)
{
    this->particles.push_back(particle);
}
