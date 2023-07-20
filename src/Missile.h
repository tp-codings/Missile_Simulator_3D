#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Missile
{
public:
	Missile(glm::vec3 startPos, glm::vec3 startDirection, float startVelocity, glm::vec3 color = glm::vec3(0.2, 0.2, 0.2));

	void update(float deltaTime);

	glm::vec3 getPosition();
	glm::vec3 getStartPosition();
	glm::vec3 getStartDirection();
	glm::vec3 getTrajectory();
	glm::vec3 getColor();
	glm::vec3 getDirection();

	float getRotationAngle();
	glm::vec3 getRotationAxis();

	float getVelocity();


	void setVelocity(float vel);
	void setDirection(glm::vec3 dir);
	void setPosition(glm::vec3 pos);

private:
	glm::vec3 direction;
	glm::vec3 startDirection;
	glm::vec3 position;
	glm::vec3 startPosition;
	glm::vec3 color;

	glm::vec3 rotationAxis;
	float rotationAngle;

	float velocity;
};

