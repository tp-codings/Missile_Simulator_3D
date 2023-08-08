#include "CameraMaster.h"
#include <iostream>
#include <algorithm>

CameraMaster::CameraMaster()
{
}

int CameraMaster::addCamera(Camera* camera, bool camMode)
{
	cameras.push_back(camera);
    cameras[cameras.size() - 1]->setCamMode(camMode);
	return cameras.size()-1;
}

void CameraMaster::removeCamera(int key)
{
	eraseCameras.insert(key);
}

void CameraMaster::updateCamera(int key, glm::vec3 translation)
{
    if (key < cameras.size()) {
        cameras[key]->setTranslation(translation);
    }
}

void CameraMaster::update()
{
    if (!eraseCameras.empty()) {
        for (int eraseKey : eraseCameras) {
            if (eraseKey < cameras.size()) {
                cameras.erase(cameras.begin() + eraseKey);
            }
        }
        eraseCameras.clear();
    }
}

void CameraMaster::updateCamSpeed(int key, float camSpeed)
{
	if (key < cameras.size()) {
		cameras[key]->setSpeed(camSpeed);
	}
}

void CameraMaster::setCamMode(int key, bool thirdPerson)
{
    if (key < cameras.size()) {
        cameras[key]->setCamMode(thirdPerson);
    }
}

std::vector<Camera*> CameraMaster::getCameras()
{
	return cameras;
}

std::vector<Camera*> CameraMaster::cameras;
std::set<int> CameraMaster::eraseCameras;