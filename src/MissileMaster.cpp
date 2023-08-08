#include "MissileMaster.h"

MissileMaster::MissileMaster()
{
	this->model = new ModelHandler(R"(resources\models\missile\missile.obj)");
	this->model->Scale(1.0f);
	this->particleMaster = new ParticleMaster();
	this->particleAtlas = loader.loadTextures(R"(resources/textures/particleAtlas.png)");
	this->shader = Shader("Shader/missile.vs", "Shader/missile.fs");
}

void MissileMaster::update(float deltaTime, Camera& camera, std::vector<Planes *> planes)
{
	if (eraseMissiles.size() > 0) {
		for (auto it = eraseMissiles.rbegin(); it != eraseMissiles.rend(); ++it) {
			this->missiles.erase(this->missiles.begin() + *it);
			//this->camKeys.erase(this->camKeys.begin() + *it);
		}
		this->eraseMissiles.clear();
	}

	for (size_t i = 0; i < this->missiles.size(); ++i) {
		this->missiles[i]->update(deltaTime);

		if (planes.size() > 0) {
			this->missiles[i]->setAccAcc(glm::vec3(50.0f));
			this->missiles[i]->setMaxVelocity(glm::vec3(120.0f));

			int nearest;
			float nearestDistance;
			std::tuple<int, float> result = this->updateNearestPlane(missiles[i], planes);
			nearest = std::get<0>(result);
			nearestDistance = std::get<1>(result);
			glm::vec3 direction = glm::normalize(planes[nearest]->getPosition() - missiles[i]->getPosition());

			float angleToTarget = glm::angle(glm::normalize(this->missiles[i]->getDirection()), direction);
			if (angleToTarget < 0.9) {
				if (nearestDistance > 50 && nearestDistance < 1000)
				{
					float acc = 3.0f *deltaTime;
					this->missiles[i]->setDirection(glm::vec3(this->missiles[i]->getDirection().x + direction.x * acc, this->missiles[i]->getDirection().y + direction.y * acc, this->missiles[i]->getDirection().z + direction.z * acc));
				}

				else if (nearestDistance <= 50) {
					float acc = 8.0f * deltaTime;
					this->missiles[i]->setDirection(glm::vec3(this->missiles[i]->getDirection().x + direction.x * acc, this->missiles[i]->getDirection().y + direction.y * acc, this->missiles[i]->getDirection().z + direction.z * acc));
				}
			}
		}
		int spreadFactor = 5;
		float spread = 0.05;

		for (int m = 1; m < 20; m++) {
			glm::vec3 vel = this->missiles[i]->getVelocity();
			float speed = sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);

			this->particleMaster->addParticle(new Particle(
				ParticleTextureHandler(this->particleAtlas, 4),
				//position particles along traveled distance
				this->missiles[i]->getPosition() - glm::normalize(this->missiles[i]->getDirection()) * speed * deltaTime * (float)(1 / m),
				0.2f * -glm::normalize(glm::vec3(this->missiles[i]->getDirection().x + spread * (rand() % spreadFactor - (spreadFactor / 2)), this->missiles[i]->getDirection().y + spread * (rand() % spreadFactor - (spreadFactor / 2)), this->missiles[i]->getDirection().z + spread * (rand() % spreadFactor - (spreadFactor / 2)) / 10)),
				0.01, (rand() % 40 + 10) / 20, 0, 1.0));
		}
		//if (this->missiles[i]->getPosition().y < this->terrain->getHeightAtPosition(missiles[i]->getPosition().x, missiles[i]->getPosition().z)) {
		//	this->eraseMissiles.insert(i);
		//	this->explosion(this->missiles[i]->getPosition(), -this->missiles[i]->getDirection(), 1000, 0.001, 150, 70, 0.09);
		//}
		if (this->missiles[i]->getTimer() > 35.0f) {
			this->eraseMissiles.insert(i);
			CameraMaster::removeCamera(camKeys[i]);
			this->explosion(this->missiles[i]->getPosition(), this->missiles[i]->getDirection(), 1000, 0.001, 150, 70, 0.09, 0.5);
		}
		
		CameraMaster::updateCamera(this->camKeys[i], glm::vec3(this->missiles[i]->getPosition().x , this->missiles[i]->getPosition().y , this->missiles[i]->getPosition().z));
	}


	this->particleMaster->update(deltaTime, camera);
}

void MissileMaster::render(glm::mat4 projection, Camera& camera)
{
	glm::mat4 view = camera.GetViewMatrix();
	this->shader.use();
	this->shader.setVec3("viewPos", camera.Position);

	for (auto i : missiles) {
		this->model->Translate(i->getPosition());
		this->model->Rotate(i->getRotationAngle(), i->getRotationAxis());
		this->model->Draw(&this->shader, projection, view, i->getColor());
	}
	this->particleMaster->render(projection, camera);
}

void MissileMaster::addMissile(Missile* missile)
{
	this->missiles.push_back(missile);
	this->camKeys.push_back(CameraMaster::addCamera(new Camera(missile->getPosition()), true));
}

std::vector<Missile*> MissileMaster::getMissiles()
{
	return this->missiles;
}

void MissileMaster::removeMissile(int index)
{
	this->eraseMissiles.insert(index);
	CameraMaster::removeCamera(camKeys[index]);
}

std::tuple<int, float> MissileMaster::updateNearestPlane(Missile* missile, vector<Planes*> planes)
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

void MissileMaster::explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact = 0.001f, float scale = 0.5f)
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
