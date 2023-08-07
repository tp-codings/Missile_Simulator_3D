#include "InputController.h"
#include <iostream>

InputController::InputController(GLFWwindow* window):window(window)
{
}

void InputController::update()
{
	//Keyboard shortcuts
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !settingsKeyPressed)
	{
		Settings::viewMode = !Settings::viewMode;
		settingsKeyPressed = true;
	}      

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
	{
		settingsKeyPressed = false;
	}
	if (!Settings::viewMode) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !startKeyPressed)
	{
		Settings::start = !Settings::start;
		startKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
	{
		startKeyPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !rangeKeyPressed)
	{
		Settings::showRange = !Settings::showRange;
		rangeKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
	{
		rangeKeyPressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !camChoicePressed)
	{
		camChoice++;
		camChoicePressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
	{
		camChoicePressed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		Settings::skyBoxChoice = 4;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		Settings::skyBoxChoice = 3;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		Settings::skyBoxChoice = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		Settings::skyBoxChoice = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		Settings::skyBoxChoice = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		shootGunTower = true;
	}
	else {
		shootGunTower = false;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !shootMissilePressed) {
		shootMissilePressed = true;
		shootMissileTruck = true;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
	{
		shootMissilePressed = false;
	}

}

bool InputController::startKeyPressed = false;
bool InputController::settingsKeyPressed = false;
bool InputController::rangeKeyPressed = false;
bool InputController::shootGunTower = false;
bool InputController::shootMissileTruck = false;
bool InputController::shootMissilePressed = false;
bool InputController::camChoicePressed = false;
int InputController::camChoice = 0;
