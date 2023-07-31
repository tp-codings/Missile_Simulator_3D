#pragma once
#include <set>
#include <Camera/camera.h>
#include <ModelHandler/ModelHandler.h>
#include <glm/gtx/vector_angle.hpp>

#include "Missile.h"
#include "Planes.h"
#include "ParticleMaster.h"
#include "Loader.h"

class S400Master
{
public:
	S400Master();

	void update(float deltaTime, Camera& camera, std::vector<Planes*> planes);

	void render(glm::mat4 projection, Camera& camera);

	void addS400(Missile* missile);

	std::vector<Missile*> getS400();

	void removeS400(int index);

private:
	ParticleMaster* particleMaster;
	std::vector<Missile*> s400;

	set<int> eraseS400;

	ModelHandler* model;
	unsigned int particleAtlas;
	Shader shader;

	Loader loader;

	std::tuple<int, float> updateNearestPlane(Missile* missile, vector<Planes*> planes);

	void explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact, float scale);

};