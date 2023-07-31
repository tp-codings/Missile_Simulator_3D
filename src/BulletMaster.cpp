#include "BulletMaster.h"

BulletMaster::BulletMaster()
{
	this->particleMaster = new ParticleMaster();
}

void BulletMaster::update(float deltaTime, Camera& camera)
{
	this->particleMaster->update(deltaTime, camera);

	for (int i = 0; i < this->particles.size(); i++) {
		if (this->particles[i]->getElapsedTime() > this->particles[i]->getLifeLength()) {
			this->eraseBullets.insert(i);
		}
	}
	if (this->eraseBullets.size() > 0) {
		for (auto it = this->eraseBullets.rbegin(); it != this->eraseBullets.rend(); ++it) {
			this->particles.erase(this->particles.begin() + *it);
		}
	}
	this->eraseBullets.clear();

	cout << this->particles.size() << endl;
}

void BulletMaster::render(glm::mat4 projection, Camera& camera)
{
	this->particleMaster->render(projection, camera);
}

void BulletMaster::addBullet(Particle* bullet)
{
	this->particles.push_back(bullet);
	this->particleMaster->addParticle(bullet);
}

void BulletMaster::removeBullet(int index)
{
	this->eraseBullets.insert(index);
}

std::vector<Particle*> BulletMaster::getBullets()
{
	return this->particles;
}