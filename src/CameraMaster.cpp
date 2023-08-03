#include "CameraMaster.h"
#include <iostream>
CameraMaster::CameraMaster()
{
}

int CameraMaster::addCamera(Camera* camera)
{
	cameras.push_back(camera);
	return cameras.size()-1;
}

void CameraMaster::removeCamera(int key)
{
	eraseCameras.insert(key);
	update(key);
}

void CameraMaster::update(int key, glm::vec3 translation, glm::vec3 rotationAxis, float rotationAngle)
{
	if (key < cameras.size()) {
		cameras[key]->setTranslation(translation);
		cameras[key]->setRotationAngle(rotationAngle);
		cameras[key]->setRotationAxis(rotationAxis);
	}

	if (eraseCameras.size() > 0) {
		for (auto it = eraseCameras.rbegin(); it != eraseCameras.rend(); ++it) {
			cameras.erase(cameras.begin() + *it);
		}
		eraseCameras.clear();
	}
}

std::vector<Camera*> CameraMaster::getCameras()
{
	return cameras;
}

std::vector<Camera*> CameraMaster::cameras;
std::set<int> CameraMaster::eraseCameras;