#include "MissileMaster.h"

MissileMaster::MissileMaster()
{
	this->model = new ModelHandler(R"(resources\models\missile\missile.obj)");
	this->model->Scale(1.0f);
	this->particleMaster = new ParticleMaster();
	this->particleAtlas = loader.loadTextures(R"(resources/textures/particleAtlas.png)");
	this->shader = Shader("Shader/plane.vs", "Shader/plane.fs");
}

void MissileMaster::update(float deltaTime, Camera& camera)
{

}
