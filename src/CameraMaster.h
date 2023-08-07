#pragma once
#include <Camera/camera.h>
#include <vector>
#include <map>
#include <set>

class CameraMaster
{
public:
	CameraMaster();

	static int addCamera(Camera* camera, bool camMode = false);

	static void removeCamera(int key);

	static void update(int key, glm::vec3 translation = glm::vec3(0.0f));

	static void updateCamSpeed(int key, float camSpeed);

	static void setCamMode(int key, bool thirdPerson = true);

	static std::vector<Camera*> getCameras();

private:
	static std::vector<Camera*> cameras;

	static std::set<int> eraseCameras;
};

