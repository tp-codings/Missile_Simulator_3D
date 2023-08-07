#include "CameraMaster.h"
#include <iostream>
#include <algorithm>

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
	//update(key);
}

void CameraMaster::update(int key, glm::vec3 translation)
{
    if (key < cameras.size()) {
        cameras[key]->setTranslation(translation);
    }

    if (!eraseCameras.empty()) {
        std::vector<int> keysToRemove;
        for (int eraseKey : eraseCameras) {
            if (eraseKey < cameras.size()) {
                keysToRemove.push_back(eraseKey);
            }
        }

        // Sort the keys in reverse order
        std::sort(keysToRemove.rbegin(), keysToRemove.rend());

        // Erase elements from cameras vector
        for (int eraseKey : keysToRemove) {
            cameras.erase(cameras.begin() + eraseKey);
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

std::vector<Camera*> CameraMaster::getCameras()
{
	return cameras;
}

std::vector<Camera*> CameraMaster::cameras;
std::set<int> CameraMaster::eraseCameras;