#pragma once
#include <set>
#include <Camera/camera.h>
#include "ModelHandler.h"
#include <glm/gtx/vector_angle.hpp>

#include "Missile.h"
#include "Planes.h"
#include "ParticleMaster.h"
#include "Loader.h"

class BulletMaster
{
public:
	BulletMaster();

	void update(float deltaTime, Camera& camera);

	void render(glm::mat4 projection, Camera& camera);

	void addBullet(Particle* bullet);

	void removeBullet(int index);

	std::vector<Particle*> getBullets();

private:
	ParticleMaster* particleMaster;

	std::vector<Particle*> particles;

	Loader loader;

	set<int> eraseBullets;
};