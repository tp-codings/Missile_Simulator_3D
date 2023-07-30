#include "PlaneMaster.h"

PlaneMaster::PlaneMaster()
{
	this->model = new ModelHandler(R"(resources\models\plane\plane.obj)");
	this->model->Scale(1.0f);
	this->particleMaster = new ParticleMaster();
	this->particleAtlas = loader.loadTextures(R"(resources/textures/particleAtlas.png)");
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

	if (erasePlanes.size() > 0) {
		for (auto it = erasePlanes.rbegin(); it != erasePlanes.rend(); ++it) {
			this->planes.erase(this->planes.begin() + *it);
		}
		this->erasePlanes.clear();
	}

	if(planes.size()>0)
		this->particleMaster->update(deltaTime, camera);

	//Missile_Simulator_3D.exe!std::_Vector_const_iterator<std::_Vector_val<std::_Simple_types<Particle*>>>::_Vector_const_iterator<std::_Vector_val<std::_Simple_types<Particle*>>>(Particle * *_Parg, const std::_Container_base12 * _Pvector) Zeile 45	C++
	//Missile_Simulator_3D.exe!std::_Vector_iterator<std::_Vector_val<std::_Simple_types<Particle*>>>::_Vector_iterator<std::_Vector_val<std::_Simple_types<Particle*>>>(Particle**)	C++

}

void PlaneMaster::render(glm::mat4 projection, Camera& camera)
{
	this->shader.use();
	this->shader.setVec3("viewPos", camera.Position);
	glm::mat4 view = camera.GetViewMatrix();

	for (auto i : planes) {
		this->model->Translate(i->getPosition());
		this->model->Rotate(i->getRotationAngle(), i->getRotationAxis());
		//this->model->Rotate(glfwGetTime() * i->getCrahsRotationSpeed(), i->getDirection());

		this->model->Draw(&this->shader, projection, view, i->getColor());
	}
	for (auto i : crashingPlanes) {
		this->model->Translate(i->getPosition());

		this->model->Rotate(i->getRotationAngle(), i->getRotationAxis());
		//this->model->Rotate(glfwGetTime()*i->getCrahsRotationSpeed(), glm::vec3(1.0, 0.0, 0.0));
		this->model->Draw(&this->shader, projection, view, i->getColor());
	}
	this->particleMaster->render(projection, camera);
}

void PlaneMaster::addPlane(Planes* plane)
{
	this->planes.push_back(plane);
}

std::vector<Planes*> PlaneMaster::getPlanes()
{
	return this->planes;
}

void PlaneMaster::removePlanes(int index)
{
	this->erasePlanes.insert(index);
}
