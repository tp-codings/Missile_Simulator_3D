#include "Simulation.h"
#include <thread>
#include <random>

Simulation::Simulation(GLFWwindow* window, int WINDOW_WIDTH, int WINDOW_HEIGHT)
{
	this->window = window;
	this->WINDOW_WIDTH = WINDOW_WIDTH;
	this->WINDOW_HEIGHT = WINDOW_HEIGHT;


	this->init();
}

void Simulation::update(float deltaTime, int FPS, Camera camera)
{

	//Update Timing
	this->deltaTime = deltaTime;
	this->FPS = (float)FPS;

	this->camera = camera;

	//Update view and projection matrix
	this->projection = glm::perspective(glm::radians(this->camera.Zoom), (float)this->WINDOW_WIDTH / (float)this->WINDOW_HEIGHT, 0.1f, 100000.0f);
	this->view = this->camera.GetViewMatrix();

	if (InputController::camChoice > 0 && CameraMaster::getCameras().size() > 0) {
		if (InputController::camChoice > CameraMaster::getCameras().size()) {
			InputController::camChoice = 0;
		}
		else {
			this->camera = *CameraMaster::getCameras()[InputController::camChoice-1];
			this->view = this->camera.GetViewMatrix();
		}
	}

	this->inputController->update();

	if (Settings::start)
	{
		this->planeMaster->update(this->deltaTime * Settings::timeFactor, this->camera);
		this->missileMaster->update(this->deltaTime * Settings::timeFactor, this->camera, this->planeMaster->getPlanes());
		this->collisionMaster->updateMissileCollision(this->planeMaster, this->missileMaster);
		this->collisionMaster->updateS400Collision(this->planeMaster, this->s400Master);
		this->collisionMaster->updateBulletCollision(this->planeMaster, this->bulletMaster);
		this->missilesSelfDestruct += this->collisionMaster->updateMMCollision(this->missileMaster);
		this->missilesSelfDestruct += this->collisionMaster->updateSSCollision(this->s400Master);
		this->torretMaster->update(this->deltaTime * Settings::timeFactor, this->camera, this->planeMaster->getPlanes(), this->missileMaster);
		this->s400Master->update(this->deltaTime * Settings::timeFactor, this->camera, this->planeMaster->getPlanes());
		this->missileTruckMaster->update(this->deltaTime * Settings::timeFactor, this->camera, this->planeMaster->getPlanes(), this->s400Master);
		this->gunTowerMaster->update(this->deltaTime * Settings::timeFactor, this->camera, this->planeMaster->getPlanes(), this->bulletMaster, InputController::shootGunTower);
		this->bulletMaster->update(this->deltaTime * Settings::timeFactor, this->camera);
		CameraMaster::update();
		//this->skybox->updateSkybox(Settings:skyBoxChoice) //Noch Probleme
	}
}

void Simulation::render()
{
	//render scene to framebuffer which will be rendered as a texture for post processing purposes
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	glEnable(GL_DEPTH_TEST);
	//Sets Backgroundcolor to dimmed color of the directional light color
	glClearColor(Settings::dirLightColor.x * 0.5f, Settings::dirLightColor.y * 0.5f, Settings::dirLightColor.z * 0.5f, 1.0f);

	//Bufferclear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render
	this->renderSimulation();

	this->screenRenderer->render(this->texColorBuffer);

	this->renderHUD();

	this->settings->render();
}

float Simulation::getCameraSpeed()
{
	return Settings::cameraSpeed;
}

bool Simulation::getViewMode()
{
	return Settings::viewMode;
}

//Inits------------------------------------------------------------------------------

void Simulation::init()
{
	this->screenRenderer = new ScreenRenderer();
	this->settings = new Settings(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, this->window);
	this->inputController = new InputController(this->window);
	this->skybox = new Skybox();
	this->loader = new Loader();
	this->sun = new Sun();

	this->initMaster();
	this->initHUD();
	this->initMatrices();
	this->initPlanes();
	this->initTorrets();
	this->initGunTower();
	this->initBuffer();
}

void Simulation::initMaster()
{
	this->terrain = new TerrainGenerator(R"(resources\textures\city_heightmap.png)");
	this->planeMaster = new PlaneMaster();
	this->missileMaster = new MissileMaster();
	this->collisionMaster = new CollisionMaster();
	this->torretMaster = new TorretMaster();
	this->s400Master = new S400Master();
	this->missileTruckMaster = new MissileTruckMaster();
	this->gunTowerMaster = new GunTowerMaster();
	this->bulletMaster = new BulletMaster();
}

void Simulation::initBuffer()
{
	this->loader->loadFrameBuffer(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, this->framebuffer, this->texColorBuffer);
}

void Simulation::initHUD()
{
	this->textRenderer = new TextRenderer(10, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
	this->fontSize = 10;
}

void Simulation::initMatrices()
{
	this->projection = glm::mat4(1.0f);
	this->view = glm::mat4(1.0f);
}

void Simulation::initPlanes()
{
	int amount = rand()%100 + 5;
	//amount = 5;

	for (int i = 0; i < amount; i++) {
		this->planeMaster->addPlane(new Planes(Helper::getRandomPlanePosition(this->terrain), Helper::getRandomDirection(), 70, Colors::WHITE));
	}
}

void Simulation::initTorrets()
{
	int amount = rand() % 30 + 5;
	//amount = 2;

	for (int i = 0; i < amount; i++) {
		this->torretMaster->addTorrets(new Torret(Helper::getRandomPosition(this->terrain)));
	}
	this->torretMaster->addTorrets(new Torret(glm::vec3(40.0f)));

	amount = rand() % 10 + 5;
	//amount = 2;

	for (int i = 0; i < amount; i++) {
		this->missileTruckMaster->addMissileTrucks(new Torret(Helper::getRandomPosition(this->terrain)));
	}
}

void Simulation::initGunTower()
{
	glm::vec2 randomPos = glm::vec2(10.0f, 10.0f);
	float height = this->terrain->getHeightAtPosition(randomPos.x, randomPos.y);
	this->gunTowerMaster->addGunTower(new GunTower(glm::vec3(randomPos.x, height + 1.5, randomPos.y), 15.0f));
}

//Rendering------------------------------------------------------------------------------

void Simulation::renderSimulation()
{
	if (Settings::skyBoxChoice != 4) {
		this->skybox->render(this->camera, this->projection);
	}

	this->terrain->render(this->deltaTime, this->projection, this->view);
	this->planeMaster->render(this->projection, this->camera);
	this->missileMaster->render(this->projection, this->camera);
	this->collisionMaster->render(this->projection, this->camera, this->deltaTime * Settings::timeFactor);
	this->torretMaster->render(this->projection, this->camera, Settings::showRange);
	this->s400Master->render(this->projection, this->camera);
	this->missileTruckMaster->render(this->projection, this->camera);
	this->gunTowerMaster->render(this->projection, this->camera);
	this->bulletMaster->render(this->projection, this->camera);	
	this->sun->render(this->projection, this->view);
	glBindVertexArray(0);
}

void Simulation::renderHUD()
{
	this->textRenderer->render("FPS: " + std::to_string((int)this->FPS), 0.0f, (float)this->WINDOW_HEIGHT - 1 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("Pos: " + std::to_string(this->camera.Position.x) + ", " + std::to_string(this->camera.Position.y) + ", " + std::to_string(this->camera.Position.z), 0.0f, (float)this->WINDOW_HEIGHT - 2 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("CameraView: " + std::to_string(this->camera.Front.x) + ", " + std::to_string(this->camera.Front.y) + ", " + std::to_string(this->camera.Front.z), 0.0f, (float)this->WINDOW_HEIGHT - 3 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("DeltaTime: " + std::to_string(this->deltaTime), 0.0f, (float)this->WINDOW_HEIGHT - 4 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	this->textRenderer->render("Start: " + std::to_string(Settings::start), (float)this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 1 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("Cam: " + std::to_string(InputController::camChoice) + "/" + std::to_string(CameraMaster::getCameras().size()), (float)this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 2 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	
	std::string skyboxes[] = { "No Skybox", "Ocean", "Space", "Forest", "City" };
	this->textRenderer->render("Skybox: " + skyboxes[Settings::skyBoxChoice], (float)this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 3 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("Planes: " + std::to_string(this->planeMaster->getPlanes().size()), (float)this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 4 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("Missiles: " + std::to_string(this->missileMaster->getMissiles().size()), (float)this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 5 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("MissilesSelfDestruct: " + std::to_string(this->missilesSelfDestruct), (float)this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 6 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("PlanesSelfDestruct: " + std::to_string(this->planesSelfDestruct), (float)this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 7 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("Debug: " + this->debug, (float)this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 8 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}