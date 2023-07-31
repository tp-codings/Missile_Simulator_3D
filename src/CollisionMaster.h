#pragma once
#include  "PlaneMaster.h"
#include "MissileMaster.h"
#include "S400Master.h"
#include "Planes.h"
#include "Missile.h"
#include "ParticleMaster.h"
#include "Loader.h"
#include <Camera/camera.h>
#include <vector>

class CollisionMaster
{
public:
	CollisionMaster();

	void updateMissileCollision(PlaneMaster* planeMaster, MissileMaster* missileMaster);
	void updateS400Collision(PlaneMaster* planeMaster, S400Master* missileMaster);
	void render(glm::mat4 projection, Camera& camera, float deltaTime);


private:
	ParticleMaster* particleMaster;
	unsigned int particleAtlas;
	Loader loader;

	void explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact, float scale);
};

