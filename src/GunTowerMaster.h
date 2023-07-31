#pragma once
#include <Camera/camera.h>
#include <ModelHandler/ModelHandler.h>
#include <glm/gtx/vector_angle.hpp>

#include "Missile.h"
#include "Planes.h"
#include "GunTower.h"
#include "ParticleMaster.h"
#include "BulletMaster.h"
#include "Loader.h"

class GunTowerMaster
{
public:
	GunTowerMaster();

	void update(float deltaTime, Camera& camera, std::vector<Planes*> planes, BulletMaster* bulletMaster, bool shootGunTower);


	void render(glm::mat4 projection, Camera& camera);

	void addGunTower(GunTower* gunTower);

private:
	ParticleMaster* particleMaster;
	std::vector<GunTower*> gunTowers;

	ModelHandler* gunTowerModel;
	ModelHandler* barrelModel;
	unsigned int particleAtlas;

	Shader gunTowerShader;
	Shader barrelShader;

	Loader loader;

	std::tuple<int, float> updateNearestPlane(Missile* missile, vector<Planes*> planes);

	void explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact, float scale);

};