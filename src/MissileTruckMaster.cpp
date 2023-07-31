#include "MissileTruckMaster.h"

MissileTruckMaster::MissileTruckMaster()
{
	this->missileTruckModel = new ModelHandler(R"(resources\models\missileTruck\missileTruck.obj)");
	this->missileTruckModel->Scale(1.0f);
	this->s400Model = new ModelHandler(R"(resources\models\missile\s400.obj)");
	this->s400Model->Scale(1.0f);

	this->particleMaster = new ParticleMaster();
	this->particleAtlas = loader.loadTextures(R"(resources/textures/particleAtlas.png)");

	this->missileTruckShader = Shader("Shader/torret.vs", "Shader/torret.fs");
	this->s400Shader = Shader("Shader/missile.vs", "Shader/missile.fs");
}

void MissileTruckMaster::update(float deltaTime, Camera& camera, std::vector<Planes*> planes, S400Master* s400Master, bool shootMissileTruck)
{
	for (size_t i = 0; i < this->missileTrucks.size(); ++i) {
		if (shootMissileTruck && planes.size() > 0) {
			if (!this->missileTrucks[i]->getShot()) {
				this->missileTrucks[i]->setShot(true);
				s400Master->addS400(this->missileTrucks[i]->getMissile());
				this->explosion(this->missileTrucks[i]->getPosition() + glm::vec3(0.0f, 4.13f, 0.0f), glm::vec3(0.0f), 1000.0f, 0.0001, 5, 5, 0.01, 0.32);
			}
		}
	}
	this->particleMaster->update(deltaTime, camera);
}

void MissileTruckMaster::render(glm::mat4 projection, Camera& camera)
{
	glm::mat4 view = camera.GetViewMatrix();

	this->missileTruckShader.use();
	this->missileTruckShader.setVec3("viewPos", camera.Position);

	for (auto i : missileTrucks) {
		this->missileTruckModel->Translate(i->getPosition());
		this->missileTruckModel->Rotate(i->getRotationAngle(), i->getRotationAxis());
		this->missileTruckModel->Draw(&this->missileTruckShader, projection, view, i->getColor());

		if (!i->getShot()) {
			this->s400Shader.use();
			this->s400Shader.setVec3("viewPos", camera.Position);

			this->s400Model->Translate(i->getPosition());
			this->s400Model->Rotate(i->getMissile()->getRotationAngle(), i->getMissile()->getRotationAxis());
			this->s400Model->Draw(&this->s400Shader, projection, view, i->getColor());
		}
	}
	this->particleMaster->render(projection, camera);
}

void MissileTruckMaster::addMissileTrucks(Torret* torret)
{
	this->missileTrucks.push_back(torret);
}

void MissileTruckMaster::explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact, float scale)
{
	for (int k = 0; k < amount; k++) {
		spreadDiversity += rand() % (int)(spreadDiversity / 5) - spreadDiversity / 10;
		this->particleMaster->addParticle(new Particle(
			ParticleTextureHandler(this->particleAtlas, 4),
			pos,
			glm::vec3(spreadFactor * (rand() % spreadDiversity - (spreadDiversity / 2)), spreadFactor * (rand() % spreadDiversity - (spreadDiversity / 2)), spreadFactor * (rand() % spreadDiversity - (spreadDiversity / 2))) + direction,
			gravityImpact, (rand() % 5 * maxDuration + 1) / maxDuration, 0, scale));
	}
}
