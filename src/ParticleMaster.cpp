#include "ParticleMaster.h"
#include <iostream>

ParticleMaster::ParticleMaster()
{
	this->renderer = ParticleRenderer();
}


void ParticleMaster::update(float deltaTime)
{
    auto mapIterator = this->particles.begin();

    while (mapIterator != this->particles.end()) {
        // Erhalte eine Referenz auf die Liste von Partikeln für das aktuelle ParticleTextureHandler
        std::vector<Particle*>& particleList = mapIterator->second;

        // Erstelle einen Iterator für die Liste von Partikeln
        auto listIterator = particleList.begin();

        while (listIterator != particleList.end()) {
            Particle* p = *listIterator;
            bool stillAlive = p->update(deltaTime);

            if (!stillAlive) {
                // Entferne das Particle aus der Liste
                listIterator = particleList.erase(listIterator);

                if (particleList.empty()) {
                    // Entferne den Eintrag aus der unordered_map, wenn die Liste leer ist
                    mapIterator = particles.erase(mapIterator);
                }
            }
            else {
                // Gehe zum nächsten Particle in der Liste
                ++listIterator;
            }
        }

        // Gehe zum nächsten Eintrag in der unordered_map
        ++mapIterator;
    }
}

void ParticleMaster::render(glm::mat4 projection, Camera& camera)
{
    this->renderer.render(this->particles, projection, camera);
}

void ParticleMaster::addParticle(Particle* particle)
{
    // Finde die Liste der Partikel, die mit dem Texture*-Objekt verbunden sind
    std::vector<Particle*>& particleList = this->particles[particle->getTexture().getTextureID()];

    // Wenn die Liste noch nicht existiert, füge eine neue Liste hinzu
    if (particleList.empty()) {
        particleList = std::vector<Particle*>();
        this->particles[particle->getTexture().getTextureID()] = particleList;
    }

    // Füge das Particle zur Liste hinzu
    particleList.push_back(particle);
}

int ParticleMaster::getParticlesAlive()
{
    return this->particles.size();
}

