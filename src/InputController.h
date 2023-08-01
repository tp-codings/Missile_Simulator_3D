#pragma once
#include <GLFW/glfw3.h>
#include "Settings.h"

class InputController
{
public:
	InputController(GLFWwindow* window);

	static bool startKeyPressed;
	static bool settingsKeyPressed;
	static bool rangeKeyPressed;
	static bool shootGunTower;
	static bool shootMissileTruck;

	void update();

private:
	GLFWwindow* window;
};

