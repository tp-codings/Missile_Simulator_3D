#pragma once
#include <Camera/camera.h>
#include <ModelHandler/ModelHandler.h>
#include <glm/gtx/vector_angle.hpp>

#include "Missile.h"
#include "Planes.h"
#include "Torret.h"
#include "ParticleMaster.h"
#include "MissileMaster.h"
#include "Loader.h"

class TorretMaster
{
public:
	TorretMaster();

	void update(float deltaTime, Camera& camera, std::vector<Planes*> planes, MissileMaster* missileMaster);

	void render(glm::mat4 projection, Camera& camera);

	void addTorrets(Torret* torret);

private:
	ParticleMaster* particleMaster;
	std::vector<Torret*> torrets;

	ModelHandler* torretModel;
	ModelHandler* missileModel;
	unsigned int particleAtlas;

	Shader torretShader;
	Shader missileShader;

	Loader loader;

	std::tuple<int, float> updateNearestPlane(Missile* missile, vector<Planes*> planes);

	//void explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact, float scale);

};

