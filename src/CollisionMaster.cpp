#include "CollisionMaster.h"

CollisionMaster::CollisionMaster()
{
	this->particleMaster = new ParticleMaster();
	this->particleAtlas = this->particleAtlas = loader.loadTextures(R"(resources/textures/particleAtlas.png)");

}

void CollisionMaster::updateMissileCollision(PlaneMaster* planeMaster, MissileMaster* missileMaster)
{
	std::vector<Planes*> planes = planeMaster->getPlanes();
	std::vector < Missile* > missiles = missileMaster->getMissiles();

	float radius = 10;
	for (size_t i = 0; i < missiles.size(); ++i) {
		for (size_t j = 0; j < planes.size(); ++j) {

			glm::vec3 connectionVector = planes[j]->getPosition() - missiles[i]->getPosition();
			float distance = sqrt(connectionVector.x * connectionVector.x + connectionVector.y * connectionVector.y + connectionVector.z * connectionVector.z);

			if (distance <= radius) {
				int crashProb = rand() % 2 + 1;
				if (crashProb == 1) {
					planes[j]->setCrashRotationSpeed(rand() % 10 + -5);
					//this->crashingPlanes.push_back(planes[j]);
				}
				planeMaster->removePlanes(j);
				missileMaster->removeMissile(i);
				this->explosion(planes[j]->getPosition(), planes[j]->getDirection(), 1000, 0.001, 1000, 100, 0.001f, 3.0f);
			}
		}
	}
}

void CollisionMaster::updateS400Collision(PlaneMaster* planeMaster, S400Master* s400Master)
{
	std::vector<Planes*> planes = planeMaster->getPlanes();
	std::vector < Missile* > missiles = s400Master->getS400();

	float radius = 10;
	for (size_t i = 0; i < missiles.size(); ++i) {
		for (size_t j = 0; j < planes.size(); ++j) {

			glm::vec3 connectionVector = planes[j]->getPosition() - missiles[i]->getPosition();
			float distance = sqrt(connectionVector.x * connectionVector.x + connectionVector.y * connectionVector.y + connectionVector.z * connectionVector.z);

			if (distance <= radius) {
				int crashProb = rand() % 2 + 1;
				if (crashProb == 1) {
					planes[j]->setCrashRotationSpeed(rand() % 10 + -5);
					//this->crashingPlanes.push_back(planes[j]);
				}
				planeMaster->removePlanes(j);
				s400Master->removeS400(i);
				this->explosion(planes[j]->getPosition(), planes[j]->getDirection(), 1000, 0.001, 1000, 100, 0.001f, 3.0f);
			}
		}
	}
}

void CollisionMaster::render(glm::mat4 projection, Camera& camera, float deltaTime)
{
	this->particleMaster->update(deltaTime, camera);
	this->particleMaster->render(projection, camera);
}

void CollisionMaster::explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact = 0.001f, float scale = 0.5f)
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
