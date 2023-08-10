#pragma once
#include <set>
#include <Camera/camera.h>
#include "ModelHandler.h"
#include <glm/gtx/vector_angle.hpp>

#include "Missile.h"
#include "Planes.h"
#include "ParticleMaster.h"
#include "Loader.h"

#include "Settings.h"

class MissileMaster
{
public:
	MissileMaster();


	void update(float deltaTime, Camera& camera, std::vector<Planes *> planes);

	void render(glm::mat4 projection, Camera& camera);

	void addMissile(Missile* missile);

	std::vector<Missile*> getMissiles();

	void removeMissile(int index);

private:
	ParticleMaster* particleMaster;
	std::vector<Missile*> missiles;

	set<int> eraseMissiles;

	std::vector<int> camKeys;					 // Key for identifying corresponding camera in CameraMaster

	ModelHandler* model;
	unsigned int particleAtlas;
	Shader shader;

	Loader loader;

	std::tuple<int, float> updateNearestPlane(Missile* missile, vector<Planes*> planes);

	void explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact, float scale);
	
};