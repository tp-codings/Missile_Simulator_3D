#include "PlaneMaster.h"

PlaneMaster::PlaneMaster()
{
	this->plane = new ModelHandler(R"(resources\models\plane\plane.obj)");
	this->loader = new Loader();
	this->particleAtlas = this->loader->loadTextures(R"(resources/textures/particleAtlas.png)");

	this->plane->Scale(1.0f);
	this->particleMaster = new ParticleMaster();

	this->shader = Shader("Shader/plane.vs", "Shader/plane.fs");

}

void PlaneMaster::update(float deltaTime, Camera& camera)
{
	for (auto i : this->planes) {
		i->update(deltaTime);
		i->setDirection(glm::vec3(i->getDirection().x, i->getDirection().y, i->getDirection().z));

		int spreadFactor = 5;
		float spread = 0.1;
		this->particleMaster->addParticle(new Particle(ParticleTextureHandler(this->particleAtlas, 4), i->getPosition() - 10.0f * i->getDirection(), 0.2f * -glm::normalize(glm::vec3(i->getDirection().x + spread * (rand() % spreadFactor - (spreadFactor / 2)), i->getDirection().y + spread * (rand() % spreadFactor - (spreadFactor / 2)), i->getDirection().z + spread * (rand() % spreadFactor - (spreadFactor / 2)))), 0.001, (rand() % 40 + 10) / 20, 0, 0.5));
	}
}

void PlaneMaster::render(glm::mat4 projection, Camera& camera)
{
	this->shader.use();
	this->shader.setVec3("viewPos", camera.Position);
	glm::mat4 view = camera.GetViewMatrix();

	for (auto i : planes) {
		this->plane->Translate(i->getPosition());
		this->plane->Rotate(i->getRotationAngle(), i->getRotationAxis());
		//this->plane->Rotate(glfwGetTime() * i->getCrahsRotationSpeed(), i->getDirection());

		this->plane->Draw(&this->shader, projection, view, i->getColor());
	}
	this->particleMaster->render(projection, camera);
}

void PlaneMaster::addPlane(Planes* plane)
{
	this->planes.push_back(plane);
}


