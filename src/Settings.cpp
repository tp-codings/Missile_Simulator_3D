#include "Settings.h"

Settings::Settings(int windowWidth, int windowHeight, GLFWwindow* window):WINDOW_WIDTH(windowWidth), WINDOW_HEIGHT(windowHeight), window(window)
{
	this->init();
}

void Settings::render()
{
    // Tell ImGUI it's a new Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (!viewMode) {
        ImGui::Begin("Steuerung", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImVec2 pos(5, 50);
        ImGui::SetWindowPos(pos);

        // Init Settingswindow
        ImVec2 size((float)WINDOW_WIDTH / 4, (float)WINDOW_HEIGHT * 0.9f);
        ImGui::SetWindowSize(size);

        // Settings
        ImGui::Text("Settings");
        ImGui::SetNextItemWidth((float)WINDOW_WIDTH / 5);
        ImGui::SliderFloat("Camspeed", &cameraSpeed, 1.0f, 600.0f);

        // Simulation control
        const char* play = "Start";
        if (start) {
            play = "Stopp";
        }
        if (ImGui::Button(play)) {
            start = !start;
        }
        ImGui::SliderFloat("Timefactor", &timeFactor, 0.0f, 1.0f);

        if (ImGui::Button("Ocean")) {
            skyBoxChoice = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Space")) {
            skyBoxChoice = 2;
        }
        ImGui::SameLine();
        if (ImGui::Button("Forest")) {
            skyBoxChoice = 3;
        }
        ImGui::SameLine();
        if (ImGui::Button("City")) {
            skyBoxChoice = 4;
        }

        ImGui::ColorPicker3("DirLight", (float*)&this->dirLightColor, ImGuiColorEditFlags_InputRGB);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    ImGui::EndFrame();

}

void Settings::init()
{
	//ImGUI Setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(this->window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}


bool Settings::viewMode = true;
bool Settings::start = false;
float Settings::cameraSpeed = 100.0f;
float Settings::timeFactor = 1.0f;
int Settings::skyBoxChoice = 1;
bool Settings::showRange = false;
ImVec4 Settings::dirLightColor = ImVec4(1.0, 1.0, 1.0, 1.0);