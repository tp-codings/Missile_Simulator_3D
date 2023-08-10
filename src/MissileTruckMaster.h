#pragma once
#include <Camera/camera.h>
#include "ModelHandler.h"
#include <glm/gtx/vector_angle.hpp>

#include "Missile.h"
#include "Planes.h"
#include "Torret.h"
#include "ParticleMaster.h"
#include "S400Master.h"
#include "Loader.h"

#include "InputController.h"

class MissileTruckMaster
{
public:
	MissileTruckMaster();

	void update(float deltaTime, Camera& camera, std::vector<Planes*> planes, S400Master *s400Master);

	void render(glm::mat4 projection, Camera& camera);

	void addMissileTrucks(Torret* torret);

private:
	ParticleMaster* particleMaster;
	std::vector<Torret*> missileTrucks;

	ModelHandler* missileTruckModel;
	ModelHandler* s400Model;

	unsigned int particleAtlas;

	Shader missileTruckShader;
	Shader s400Shader;

	Loader loader;

	void explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact, float scale);
};

