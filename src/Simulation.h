#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader/shader.h>
#include <Camera/camera.h>
#include <ModelLoader/model.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#include <imgui/imconfig.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <TextRenderer/TextRenderer.h>
#include <ModelHandler/ModelHandler.h>
#include <SkyBox/Skybox.h>

#include "TerrrainGenerator.h"

#include "Helper.h"
#include "Loader.h"

#include "PlaneMaster.h"
#include "MissileMaster.h"
#include "CollisionMaster.h"
#include "TorretMaster.h"
#include "S400Master.h"
#include "MissileTruckMaster.h"
#include "GunTowerMaster.h"
#include "BulletMaster.h"

#include "ScreenRenderer.h"

class Simulation
{
public:
	Simulation(GLFWwindow* window, int WINDOW_WIDTH, int WINDOW_HEIGHT);

	void update(float deltaTime, int FPS, Camera camera);
	void render();

	float getCameraSpeed();
	bool getViewMode();

private:
	//Window
	GLFWwindow* window;
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;

	ScreenRenderer* screenRenderer;

	//Text
	Loader* loader;
	TextRenderer* textRenderer;
	int fontSize;

	//Matrizen
	glm::mat4 projection;
	glm::mat4 view;
	
	//TIMING
	float deltaTime;
	float FPS;
	float timeFactor;

	//Debug
	int missilesSelfDestruct = 0;
	int planesSelfDestruct = 0;
	string debug = " ";
	bool shot;

	//Worldobjects
	PlaneMaster *planeMaster;
	MissileMaster* missileMaster;
	CollisionMaster* collisionMaster;
	TorretMaster* torretMaster;
	S400Master* s400Master;
	MissileTruckMaster* missileTruckMaster;
	GunTowerMaster* gunTowerMaster;
	BulletMaster* bulletMaster;
	TerrainGenerator *terrain;

	Camera camera;

	//Settings
	float cameraSpeed;

	bool viewMode;
	bool start;

	bool startKeyPressed;
	bool settingsKeyPressed;
	bool rangeKeyPressed;
	bool showRange;
	bool shootGunTower;
	bool shootMissileTruck;

	ImVec4 dirLightColor;
	glm::vec3 dirLightPos;

	unsigned int framebuffer;
	unsigned int texColorBuffer;
	unsigned int rbo;

	//Skybox
	Skybox* skybox;
	int skyBoxChoice;

	//Inits------------------------------------------------------------------------------

	void init();
	void initMaster();
	void initBuffer();
	void initSettings();
	void initHUD();
	void initMatrices();
	void initPlanes();
	void initTorrets();
	void initGunTower();

	//Input------------------------------------------------------------------------------

	void processInput();

	//Updates------------------------------------------------------------------------------

	void updateSimulation();

	//Rendering------------------------------------------------------------------------------

	void renderSimulation();
	void renderSettings();
	void renderHUD();
};

