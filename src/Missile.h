#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Missile
{
public:
	Missile(glm::vec3 startPos, glm::vec3 startDirection, glm::vec3 startVelocity, glm::vec3 color = glm::vec3(0.2, 0.2, 0.2));

	void update(float deltaTime);

	glm::vec3 getPosition();
	glm::vec3 getStartPosition();
	glm::vec3 getStartDirection();
	glm::vec3 getTrajectory();
	glm::vec3 getColor();
	glm::vec3 getDirection();
	glm::vec3 getAcceleration();
	glm::vec3 getMaxVelocity();
	glm::vec3 getAccAcc();

	float getRotationAngle();
	glm::vec3 getRotationAxis();

	glm::vec3 getVelocity();

	float getTimer();

	void setVelocity(glm::vec3 vel);
	void setMaxVelocity(glm::vec3 maxVel);
	void setDirection(glm::vec3 dir);
	void setPosition(glm::vec3 pos);
	void setAcceleration(glm::vec3 acc);
	void setAccAcc(glm::vec3 accacc);
	void setShot(bool shot);

	float calcVecLength(glm::vec3 vec);

private:
	glm::vec3 direction;
	glm::vec3 startDirection;
	glm::vec3 position;
	glm::vec3 startPosition;
	glm::vec3 color;

	glm::vec3 rotationAxis;
	float rotationAngle;

	glm::vec3 velocity;
	glm::vec3 maxVelocity;
	glm::vec3 acceleration;
	glm::vec3 accacc;

	bool shot;
	float timer;
};

