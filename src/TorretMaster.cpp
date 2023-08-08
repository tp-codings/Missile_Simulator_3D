#include "TorretMaster.h"

TorretMaster::TorretMaster()
{
	this->torretModel = new ModelHandler(R"(resources\models\torret\torret.obj)");
	this->torretModel->Scale(1.0f);	
	this->missileModel = new ModelHandler(R"(resources\models\missile\missile.obj)");
	this->missileModel->Scale(1.0f);

	this->particleMaster = new ParticleMaster();
	this->particleAtlas = loader.loadTextures(R"(resources/textures/particleAtlas.png)");

	this->rangeRenderer = new RangeRenderer(1300.0f);

	this->torretShader = Shader("Shader/torret.vs", "Shader/torret.fs");
	this->missileShader = Shader("Shader/missile.vs", "Shader/missile.fs");
}

void TorretMaster::update(float deltaTime, Camera& camera, std::vector<Planes*> planes, MissileMaster* missileMaster)
{
	for (size_t i = 0; i < this->torrets.size(); ++i) {

		int nearest;
		float nearestDistance = 1500;
		glm::vec3 direction;

		if (!this->torrets[i]->getShot() && planes.size() > 0) {
			std::tuple<int, float> result = this->updateNearestPlane(torrets[i]->getMissile(), planes);
			nearest = std::get<0>(result);
			nearestDistance = std::get<1>(result);

			direction = glm::normalize(planes[nearest]->getPosition() - this->torrets[i]->getPosition());
		}

		if (nearestDistance < 1500) {
			if (!this->torrets[i]->getShot()) {
				float rotSpeed = 0.1f;
				this->torrets[i]->setDirection(glm::vec3(this->torrets[i]->getDirection().x + direction.x * rotSpeed, 0.0f, this->torrets[i]->getDirection().z + direction.z * rotSpeed));
				this->torrets[i]->getMissile()->setDirection(glm::vec3(this->torrets[i]->getMissile()->getDirection().x + rotSpeed * direction.x, this->torrets[i]->getMissile()->getDirection().y + rotSpeed * direction.y, this->torrets[i]->getMissile()->getDirection().z + rotSpeed * direction.z));
			}
			this->torrets[i]->update(deltaTime);
		}
		float angleToTarget = glm::angle(glm::normalize(this->torrets[i]->getMissile()->getDirection()), direction);

		if (nearestDistance < 1300 && planes.size()>0 && angleToTarget < 0.5) {
			this->torrets[i]->setShot(true);
			missileMaster->addMissile(this->torrets[i]->getMissile());
		}
	}
	//this->rangeRenderer->update(1300.0f);
}

void TorretMaster::render(glm::mat4 projection, Camera& camera, bool showRange)
{
	glm::mat4 view = camera.GetViewMatrix();

	this->torretShader.use();
	this->torretShader.setVec3("viewPos", camera.Position);

	for (auto i : torrets) {
		this->torretModel->Translate(i->getPosition());
		this->torretModel->Rotate(i->getRotationAngle(), i->getRotationAxis());
		this->torretModel->Draw(&this->torretShader, projection, view, i->getColor());

		if (!i->getShot()) {
			this->missileShader.use();
			this->missileShader.setVec3("viewPos", camera.Position);

			this->missileModel->Translate(i->getPosition());
			this->missileModel->Rotate(i->getMissile()->getRotationAngle(), i->getMissile()->getRotationAxis());
			this->missileModel->Draw(&this->missileShader, projection, view, i->getColor());
		}
	}
	if (showRange) {
		this->rangeRenderer->render(projection, camera);
	}
}

void TorretMaster::addTorrets(Torret* torret)
{
	this->torrets.push_back(torret);
}

std::tuple<int, float> TorretMaster::updateNearestPlane(Missile* missile, vector<Planes*> planes)
{
	int nearest = 0;
	float tempDistance = 100000000.0f;

	for (size_t i = 0; i < planes.size(); i++) {
		float distance;
		glm::vec3 connectionVector = planes[i]->getPosition() - missile->getPosition();
		distance = sqrt(connectionVector.x * connectionVector.x + connectionVector.y * connectionVector.y + connectionVector.z * connectionVector.z);
		if (distance < tempDistance) {
			tempDistance = distance;
			nearest = i;
		}
	}

	return std::make_tuple(nearest, tempDistance);
}
