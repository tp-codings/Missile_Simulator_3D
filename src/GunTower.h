/**************************************************************
 *  Name: GunTower.h
 *  Author: Tobias Paul
 *  Date: 23.07.2023
 *  Description:
	The GunTower class represents a tower object with a gun that can shoot missiles. 
	It is positioned in 3D space using a given position and can rotate to aim in a specific direction. 
	The tower's appearance can be customized with a color. 
	The class provides methods to update the tower's state, retrieve its current position, color, direction, and shooting status. 
	Additionally, it allows setting the direction, position, and shooting status. 
	The GunTower class contains a Missile object as its barrel to fire projectiles.
 **************************************************************/

#ifndef GUNTOWER_H
#define GUNTOWER_H

#include <glm/glm.hpp>
#include "Missile.h"

class GunTower
{
public:
    // Constructor with optional color parameter
    GunTower(glm::vec3 pos, float speed, glm::vec3 color = glm::vec3(0.2, 0.2, 0.2));

    // Updates the tower's state based on the deltaTime
    void update(float deltaTime);

    // Getter methods for various attributes
    glm::vec3 getPosition();
    glm::vec3 getColor();
    glm::vec3 getDirection();

    bool getShot();

    float getRotationAngle();
    float getSpeed();
    glm::vec3 getRotationAxis();

    // Setter methods to modify direction, position, and shot status
    void setDirection(glm::vec3 dir);
    void setPosition(glm::vec3 pos);
    void setShot(bool isShot);

    // Getter method to retrieve the missile object associated with the tower
    Missile* getTower();

private:
    glm::vec3 direction; // The direction in which the tower is aiming
    glm::vec3 position; // The position of the tower in 3D space
    glm::vec3 color; // The color of the tower (default: dark gray)
    glm::vec3 rotationAxis; // The rotation axis for the tower's rotation

    float rotationAngle; // The current rotation angle of the tower
    float speed; // The speed of the tower's movements

    bool shot; // A flag to indicate if the tower has shot a missile

    Missile* barrel; // Pointer to the missile object serving as the tower's barrel
};

#endif // GUNTOWER_H


