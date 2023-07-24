#include "GunTower.h"

#include <iostream>


GunTower::GunTower(glm::vec3 pos, float speed, glm::vec3 color)
{
    this->position = pos;
    this->speed = speed;
    this->direction = glm::vec3(1.0f, 0.0f, 0.0f);
    this->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    this->color = color;
    this->shot = false;

    this->barrel = new Missile(pos, glm::vec3(0.0, 1.0, 0.001), glm::vec3(0.0));
    // Berechnung der Rotationsachse und des Rotationswinkels
    glm::vec3 standardDirection(1.0f, 0.0f, 0.0f);
    this->rotationAngle = glm::acos(glm::dot(standardDirection, this->direction));
}

void GunTower::update(float deltaTime)
{
    glm::vec3 standardDirection(1.0f, 0.0f, 0.0f);
    this->rotationAngle = glm::acos(glm::dot(standardDirection, glm::normalize(this->direction)));
    this->barrel->update(deltaTime);

}

glm::vec3 GunTower::getPosition()
{
    return this->position;
}


glm::vec3 GunTower::getColor()
{
    return this->color;
}

glm::vec3 GunTower::getDirection()
{
    return this->direction;
}

bool GunTower::getShot()
{
    return this->shot;
}

float GunTower::getRotationAngle()
{
    return this->rotationAngle;
}

float GunTower::getSpeed()
{
    return this->speed;
}

glm::vec3 GunTower::getRotationAxis()
{
    return this->rotationAxis;
}

void GunTower::setDirection(glm::vec3 dir)
{
    this->direction = glm::normalize(dir);
}

void GunTower::setPosition(glm::vec3 pos)
{
    this->position = pos;
}

void GunTower::setShot(bool isShot)
{
    this->shot = isShot;
}

Missile* GunTower::getTower()
{
    return this->barrel;
}

