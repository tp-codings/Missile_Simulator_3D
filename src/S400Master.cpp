#include "s400Master.h"

S400Master::S400Master()
{
	this->model = new ModelHandler(R"(resources\models\missile\s400.obj)");
	this->model->Scale(1.0f);

	this->particleMaster = new ParticleMaster();
	this->particleAtlas = loader.loadTextures(R"(resources/textures/particleAtlas.png)");

	this->shader = Shader("Shader/missile.vs", "Shader/missile.fs");
}

void S400Master::update(float deltaTime, Camera& camera, std::vector<Planes*> planes)
{
	if (eraseS400.size() > 0) {
		for (auto it = eraseS400.rbegin(); it != eraseS400.rend(); ++it) {
			this->s400.erase(this->s400.begin() + *it);
			//this->camKeys.erase(this->camKeys.begin() + *it);
		}
		this->eraseS400.clear();
	}

	for (size_t i = 0; i < this->s400.size(); ++i) {
		this->s400[i]->update(deltaTime);
		int nearest;
		float nearestDistance = 1001;
		std::tuple<int, float> result;
		glm::vec3 direction = this->s400[i]->getDirection();

		if (planes.size() > 0) {
			result = this->updateNearestPlane(this->s400[i], planes);
			nearest = std::get<0>(result);
			nearestDistance = std::get<1>(result);
			direction = glm::normalize(planes[nearest]->getPosition() - this->s400[i]->getPosition());
		}

		if (this->s400[i]->getTimer() < 1.3f) {
			this->s400[i]->setAccAcc(glm::vec3(0.0f));
			float acc = 2.0f * deltaTime;
			this->s400[i]->setVelocity(glm::vec3(0.0f, 14.0f, 0.0f) - glm::vec3(0.0f, 10 * this->s400[i]->getTimer(), 0.0f));
			this->s400[i]->setMaxVelocity(glm::vec3(1000.0f));
			//this->s400[i]->setDirection(glm::vec3(this->s400[i]->getDirection().x + direction.x * acc, this->s400[i]->getDirection().y + direction.y * acc, this->s400[i]->getDirection().z + direction.z * acc));
		}

		else {
			this->s400[i]->setAccAcc(glm::vec3(40.0f));
			if (this->s400[i]->getTimer() < 1.6f) {
				this->s400[i]->setDirection(glm::vec3(0.0f, 1.0f, 0.001f));

			}
			else if (this->s400[i]->getTimer() < 2.4f) {
				this->s400[i]->setAcceleration(glm::vec3(this->s400[i]->getAcceleration().x, this->s400[i]->getAcceleration().x + 60, this->s400[i]->getAcceleration().z));
				float acc = 4.0f * deltaTime;
				this->s400[i]->setDirection(glm::vec3(this->s400[i]->getDirection().x + direction.x * acc, this->s400[i]->getDirection().y + direction.y * acc, this->s400[i]->getDirection().z + direction.z * acc));
			}

			else {
				this->s400[i]->setAcceleration(glm::vec3(this->s400[i]->getAcceleration().z));

				float angleToTarget = glm::angle(glm::normalize(this->s400[i]->getDirection()), direction);
				if (angleToTarget < 0.5) {
					if (nearestDistance > 50)
					{
						float acc = 80.0f * deltaTime;
						this->s400[i]->setDirection(glm::vec3(this->s400[i]->getDirection().x + direction.x * acc, this->s400[i]->getDirection().y + direction.y * acc, this->s400[i]->getDirection().z + direction.z * acc));
					}

					else if (nearestDistance <= 50) {
						float acc = 100.0f * deltaTime;
						this->s400[i]->setDirection(glm::vec3(this->s400[i]->getDirection().x + direction.x * acc, this->s400[i]->getDirection().y + direction.y * acc, this->s400[i]->getDirection().z + direction.z * acc));
					}
				}
			}
			int spreadFactor = 5;
			float spread = 0.05;
			for (int m = 1; m < 10; m++) {
				glm::vec3 vel = this->s400[i]->getVelocity();
				float speed = sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);

				this->particleMaster->addParticle(new Particle(
					ParticleTextureHandler(this->particleAtlas, 4),
					this->s400[i]->getPosition() - glm::normalize(this->s400[i]->getDirection()) * speed * deltaTime * (float)(1 / m),
					0.2f * -glm::normalize(glm::vec3(this->s400[i]->getDirection().x + spread * (rand() % spreadFactor - (spreadFactor / 2)), this->s400[i]->getDirection().y + spread * (rand() % spreadFactor - (spreadFactor / 2)), this->s400[i]->getDirection().z + spread * (rand() % spreadFactor - (spreadFactor / 2)) / 10)),
					0.01, (rand() % 40 + 10) / 20, 0, 5.5));
			}
		}

		/*if (this->s400[i]->getPosition().y < this->terrain->getHeightAtPosition(cruiseMissiles[i]->getPosition().x, cruiseMissiles[i]->getPosition().z)) {
			this->eraseCruiseMissiles.insert(i);
			this->explosion(this->s400[i]->getPosition(), -this->s400[i]->getDirection(), 1000, 0.001, 150, 70, 0.09, 2.0f);
		}*/
		if (this->s400[i]->getTimer() > 35.0f) {
			this->eraseS400.insert(i);
			CameraMaster::removeCamera(camKeys[i]);
			this->explosion(this->s400[i]->getPosition(), this->s400[i]->getDirection(), 1000, 0.001, 150, 70, 0.09, 2.0f);
		}
		CameraMaster::updateCamera(this->camKeys[i], glm::vec3(this->s400[i]->getPosition().x, this->s400[i]->getPosition().y, this->s400[i]->getPosition().z));
	}

	this->particleMaster->update(deltaTime, camera);

}

void S400Master::render(glm::mat4 projection, Camera& camera)
{
	glm::mat4 view = camera.GetViewMatrix();

	this->shader.use();
	this->shader.setVec3("viewPos", camera.Position);
	for (auto i : s400) {
		this->model->Translate(i->getPosition());
		this->model->Rotate(i->getRotationAngle(), i->getRotationAxis());
		this->model->Draw(&this->shader, projection, view, i->getColor());
	}
	this->particleMaster->render(projection, camera);
}

void S400Master::addS400(Missile* missile)
{
	this->s400.push_back(missile);
	this->camKeys.push_back(CameraMaster::addCamera(new Camera(missile->getPosition()), true));
}


std::vector<Missile*> S400Master::getS400()
{
	return this->s400;
}

void S400Master::removeS400(int index)
{
	this->eraseS400.insert(index);
	CameraMaster::removeCamera(camKeys[index]);
}

std::tuple<int, float> S400Master::updateNearestPlane(Missile* missile, vector<Planes*> planes)
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

void S400Master::explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact = 0.001f, float scale = 0.5f)
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