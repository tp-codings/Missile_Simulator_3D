#include "GunTowerMaster.h"

GunTowerMaster::GunTowerMaster()
{
	this->gunTowerModel = new ModelHandler(R"(resources\models\gunTower\gunTower.obj)");
	this->gunTowerModel->Scale(1.0f);
	this->barrelModel = new ModelHandler(R"(resources\models\gunTower\barrel.obj)");
	this->barrelModel->Scale(1.0f);

	this->particleMaster = new ParticleMaster();
	this->particleAtlas = loader.loadTextures(R"(resources/textures/cosmic.png)");

	this->gunTowerShader = Shader("Shader/torret.vs", "Shader/torret.fs");
	this->barrelShader = Shader("Shader/missile.vs", "Shader/missile.fs");
}

void GunTowerMaster::update(float deltaTime, Camera& camera, std::vector<Planes*> planes, BulletMaster* bulletMaster, bool shootGunTower)
{
	for (size_t i = 0; i < this->gunTowers.size(); ++i) {

		int nearest;
		float nearestDistance = 1001;
		glm::vec3 direction;
		float rotSpeed = 10.0f * deltaTime;

		if (planes.size() > 0) {
			std::tuple<int, float> result = this->updateNearestPlane(gunTowers[i]->getTower(), planes);
			nearest = std::get<0>(result);
			nearestDistance = std::get<1>(result);

			direction = glm::normalize(planes[nearest]->getPosition() - this->gunTowers[i]->getPosition());
			float holdUpFactor = 0.09f;
			glm::vec3 shootDirection = direction + planes[nearest]->getDirection() * holdUpFactor;

			this->gunTowers[i]->setDirection(glm::vec3(this->gunTowers[i]->getDirection().x + shootDirection.x * rotSpeed, 0.0f, this->gunTowers[i]->getDirection().z + shootDirection.z * rotSpeed));
			this->gunTowers[i]->getTower()->setDirection(glm::vec3(this->gunTowers[i]->getTower()->getDirection().x + rotSpeed * shootDirection.x, this->gunTowers[i]->getTower()->getDirection().y + rotSpeed * shootDirection.y, this->gunTowers[i]->getTower()->getDirection().z + rotSpeed * shootDirection.z));

			if (shootGunTower) {

				for (int l = 0; l < (int)this->gunTowers[i]->getSpeed(); l++) {
					Particle* p = new Particle(ParticleTextureHandler(this->particleAtlas, 4),
						this->gunTowers[i]->getPosition() + glm::normalize(shootDirection) * (float)l,
						gunTowers[i]->getSpeed() * glm::normalize(shootDirection),
						0.1f,
						5.0f,
						0.0f,
						0.5f);
					bulletMaster->addBullet(p);
				}
				this->explosion(this->gunTowers[i]->getTower()->getPosition() + shootDirection, glm::vec3(0.0f), 1000, 0.0001, 5, 10, 0.01, 0.02);
			}
		}
		else {
			this->gunTowers[i]->setDirection(glm::vec3(this->gunTowers[i]->getTower()->getDirection().x + rotSpeed * 0.0f, this->gunTowers[i]->getTower()->getDirection().y + rotSpeed * 0.0f, this->gunTowers[i]->getTower()->getDirection().z + rotSpeed * -1.0f));
			this->gunTowers[i]->getTower()->setDirection(glm::vec3(this->gunTowers[i]->getTower()->getDirection().x + rotSpeed * 0.0f, this->gunTowers[i]->getTower()->getDirection().y + rotSpeed * 0.0f, this->gunTowers[i]->getTower()->getDirection().z + rotSpeed * -1.0f));
		}
		this->gunTowers[i]->update(deltaTime);
	}
	this->particleMaster->update(deltaTime, camera);
}

void GunTowerMaster::render(glm::mat4 projection, Camera& camera)
{
	glm::mat4 view = camera.GetViewMatrix();

	for (auto i : gunTowers) {
		this->gunTowerModel->Translate(i->getPosition());
		this->gunTowerModel->Rotate(i->getRotationAngle(), i->getRotationAxis());
		this->gunTowerModel->Draw(&this->gunTowerShader, projection,view, i->getColor());

		this->barrelShader.use();
		this->barrelShader.setVec3("viewPos", camera.Position);

		this->barrelModel->Translate(i->getPosition());
		this->barrelModel->Rotate(i->getTower()->getRotationAngle(), i->getTower()->getRotationAxis());
		this->barrelModel->Draw(&this->barrelShader, projection, view, i->getColor());
	}
	this->particleMaster->render(projection, camera);
}

void GunTowerMaster::addGunTower(GunTower* gunTower)
{
	this->gunTowers.push_back(gunTower);
}


std::tuple<int, float> GunTowerMaster::updateNearestPlane(Missile* missile, vector<Planes*> planes)
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

void GunTowerMaster::explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact = 0.001f, float scale = 0.5f)
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