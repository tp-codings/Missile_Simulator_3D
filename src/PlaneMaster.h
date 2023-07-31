#pragma once
#include <set>
#include <Camera/camera.h>
#include <ModelHandler/ModelHandler.h>
#include "Planes.h"
#include "ParticleMaster.h"
#include "Loader.h"

class PlaneMaster
{
public:
	PlaneMaster();

	void update(float deltaTime, Camera& camera);

	void render(glm::mat4 projection, Camera& camera);

	void addPlane(Planes* plane);

	std::vector<Planes*> getPlanes();

	void removePlanes(int index);

private:
	ParticleMaster *particleMaster;
	std::vector<Planes*> planes;
	std::vector<Planes*> crashingPlanes;

	set<int> erasePlanes;

	ModelHandler* model;
	unsigned int particleAtlas;
	Shader shader;

	Loader loader;
};

