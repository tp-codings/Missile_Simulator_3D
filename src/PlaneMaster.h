#pragma once
#include "Planes.h"
#include <Camera/camera.h>
#include <ModelHandler/ModelHandler.h>
#include "ParticleMaster.h"
#include "Loader.h"

class PlaneMaster
{
public:
	PlaneMaster();

	void update(float deltaTime, Camera& camera);

	void render(glm::mat4 projection, Camera& camera);

	void addPlane(Planes* plane);

private:
	Loader* loader;
	std::vector<Planes*> planes;
	ModelHandler* plane;
	ParticleMaster* particleMaster;
	unsigned int particleAtlas;
	Shader shader;

};

