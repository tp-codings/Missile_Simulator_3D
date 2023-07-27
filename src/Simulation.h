#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/vector_angle.hpp>

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

#include <set>

#include "Planes.h"
#include "Missile.h"
#include "Torret.h"
#include "GunTower.h"
#include "ParticleMaster.h"


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

	//Text
	TextRenderer* textRenderer;
	int fontSize;

	//Shader
	Shader screenShader;
	Shader textShader;
	Shader cubeMapShader;
	Shader groundShader;
	Shader planeShader;
	Shader missileShader;
	Shader torretShader;

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

	//Erase Marker
	set<int> eraseCrashedPlanes;
	set<int> erasePlanes;
	set<int> eraseMissiles;
	set<int> eraseCruiseMissiles;

	//World objects
	ModelHandler* plane;
	ModelHandler* missile;
	ModelHandler* torret;
	ModelHandler* missileTruck;
	ModelHandler* s400;
	ModelHandler* gunTower;
	ModelHandler* barrel;

	vector<Planes*>planes;
	vector<Planes*>crashingPlanes;

	vector<Missile*>missiles;

	vector<Missile*>cruiseMissiles;

	vector<Torret*> torrets;
	vector<Torret*> missileTrucks;

	vector<GunTower*> gunTowers;

	vector<Particle*> particles;

	Camera camera;

	//Particlesystem
	ParticleMaster *particleMaster;

	//Settings
	float cameraSpeed;

	bool viewMode;
	bool start;

	bool startKeyPressed;
	bool settingsKeyPressed;
	bool shadingKeyPressed;
	bool shootGunTower;
	bool shootMissileTruck;

	ImVec4 dirLightColor;
	glm::vec3 dirLightPos;

	float rotSpeedX;
	float rotSpeedY;
	float rotSpeedZ;

	//Buffer
	unsigned int screenVAO;
	unsigned int screenVBO;
	unsigned int framebuffer;
	unsigned int texColorBuffer;
	unsigned int rbo;

	unsigned int groundVAO;
	unsigned int groundVBO;

	//Vertices
	float* quadVertices;
	float* groundVertices;

	//Skybox
	Skybox *oceanBox;
	Skybox *spaceBox;
	Skybox* forestBox;
	Skybox* cityBox;
	int skyBoxChoice;
	int shadingChoice;
	std::vector<std::string> ocean
	{
			R"(resources\textures\skybox\right.jpg)",
			R"(resources\textures\skybox\left.jpg)",
			R"(resources\textures\skybox\top.jpg)",
			R"(resources\textures\skybox\bottom.jpg)",
			R"(resources\textures\skybox\front.jpg)",
			R"(resources\textures\skybox\back.jpg)"
	};

	std::vector<std::string> space
	{
			R"(resources\textures\skybox\space_1_right.jpg)",
			R"(resources\textures\skybox\space_1_left.jpg)",
			R"(resources\textures\skybox\space_1_top.jpg)",
			R"(resources\textures\skybox\space_1_bottom.jpg)",
			R"(resources\textures\skybox\space_1_front.jpg)",
			R"(resources\textures\skybox\space_1_back.jpg)"
	};
	std::vector<std::string> forest
	{
			R"(resources\textures\skybox\forest_1_right.jpg)",
			R"(resources\textures\skybox\forest_1_left.jpg)",
			R"(resources\textures\skybox\forest_1_top.jpg)",
			R"(resources\textures\skybox\forest_1_bottom.jpg)",
			R"(resources\textures\skybox\forest_1_front.jpg)",
			R"(resources\textures\skybox\forest_1_back.jpg)"
	};
	std::vector<std::string> city
	{
			R"(resources\textures\skybox\city_2_right.jpg)",
			R"(resources\textures\skybox\city_2_left.jpg)",
			R"(resources\textures\skybox\city_2_top.jpg)",
			R"(resources\textures\skybox\city_2_bottom.jpg)",
			R"(resources\textures\skybox\city_2_front.jpg)",
			R"(resources\textures\skybox\city_2_back.jpg)"
	};

	//Inits------------------------------------------------------------------------------

	void initVertices();
	void initBuffer();
	void initShader();
	void initVariables();

	void initSettings();
	void initText();
	void initMatrices();
	void initSkybox();
	void initParticleSystem();
	void initPlanes();
	void initTorrets();
	void initGunTower();
	void initModels();

	//Input------------------------------------------------------------------------------

	void processInput(float deltaTime);

	//Helper------------------------------------------------------------------------------

	int random(int range, int start);
	void explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact, float scale);

	//Updates------------------------------------------------------------------------------

	void updateSimulation();
	void updatePlanes();
	void updateMissiles();
	void updateTorrets();
	void updateGunTower();
	std::tuple<int, float> updateNearestPlane(Missile* missile, vector<Planes*> planes);
	void updateHitPlane();
	void updateHitMissile();
	void updateCruiseMissile();
	void updatePlaneHitsPlane();
	void updateCrashingPlanes();
	void updateErasing();

	//Rendering------------------------------------------------------------------------------

	void DrawSimulation();
	void DrawGround();
	void DrawPlanes();
	void DrawMissiles();
	void DrawTorrets();
	void DrawGunTower();
	void DrawSettings();
	void DrawScreen();
	void DrawText();
	void DrawSkyBox();
};

