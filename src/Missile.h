/**************************************************************
 *  Name: Missile.h
 *  Author: Tobias Paul
 *  Date: 21.07.2023
 *  Description:
	The Missile class represents a projectile fired by a Torret.
	It is positioned in 3D space using a given start position and can move in a specific direction.
	The missile's appearance can be customized with a color.
	The class provides methods to update the missile's state, retrieve its current position, color, direction, and velocity.
	Additionally, it allows setting the direction, position, velocity, and shooting status.
 **************************************************************/

#ifndef MISSILE_H
#define MISSILE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Missile
{
public:
	// Constructor to initialize the missile's properties
	Missile(glm::vec3 startPos, glm::vec3 startDirection, glm::vec3 startVelocity, glm::vec3 color = glm::vec3(0.2, 0.2, 0.2));

	// Update the missile's position based on time passed
	void update(float deltaTime);

	// Getters for retrieving various missile properties
	glm::vec3 getPosition();
	glm::vec3 getStartPosition();
	glm::vec3 getStartDirection();
	glm::vec3 getTrajectory();
	glm::vec3 getColor();
	glm::vec3 getDirection();
	glm::vec3 getAcceleration();
	glm::vec3 getMaxVelocity();
	glm::vec3 getAccAcc();

	// Get the rotation angle and axis for the missile's orientation
	float getRotationAngle();
	glm::vec3 getRotationAxis();

	glm::vec3 getVelocity();

	// Get the time elapsed since the missile was shot
	float getTimer();

	// Setters for modifying missile properties
	void setVelocity(glm::vec3 vel);
	void setMaxVelocity(glm::vec3 maxVel);
	void setDirection(glm::vec3 dir);
	void setPosition(glm::vec3 pos);
	void setAcceleration(glm::vec3 acc);
	void setAccAcc(glm::vec3 accacc);
	void setShot(bool shot);

	// Calculate the length of a given vector
	float calcVecLength(glm::vec3 vec);

private:
	glm::vec3 direction;         // Current direction of the missile
	glm::vec3 startDirection;    // Initial direction of the missile
	glm::vec3 position;          // Current position of the missile
	glm::vec3 startPosition;     // Initial position of the missile
	glm::vec3 color;             // Color of the missile

	glm::vec3 rotationAxis;      // Axis around which the missile is rotated
	float rotationAngle;         // Angle of rotation for the missile's orientation

	glm::vec3 velocity;          // Current velocity of the missile
	glm::vec3 maxVelocity;       // Maximum allowed velocity for the missile
	glm::vec3 acceleration;      // Acceleration applied to the missile
	glm::vec3 accacc;            // Acceleration acceleration (jerk) applied to the missile

	bool shot;                   // Flag indicating whether the missile has been shot
	float timer;                 // Timer to track the time elapsed since the missile was shot
};

#endif // MISSILE_H
