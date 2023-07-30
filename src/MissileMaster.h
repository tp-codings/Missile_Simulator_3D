#pragma once
#include <set>
#include <Camera/camera.h>
#include <ModelHandler/ModelHandler.h>
#include "Planes.h"
#include "ParticleMaster.h"
#include "Loader.h"

class MissileMaster
{
public:
	MissileMaster();

	void update(float deltaTime, Camera& camera);

	void render(glm::mat4 projection, Camera& camera);

	void addPlane(Planes* plane);

	std::vector<Planes*> getMissiles();

	void removeMissile(int index);

private:
	ParticleMaster* particleMaster;
	std::vector<Planes*> missiles;

	set<int> eraseMissiles;

	ModelHandler* model;
	unsigned int particleAtlas;
	Shader shader;

	Loader loader;
};