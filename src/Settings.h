#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui/imconfig.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


#include <string>

class Settings {

public:
    Settings(int windowWidth, int windowHeight, GLFWwindow* window);

    void render();

    static bool viewMode;
    static bool start;
    static float cameraSpeed;
    static float timeFactor;
    static int skyBoxChoice;
    static bool showRange;
    static ImVec4 dirLightColor;
    
private:
    void init();
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;

    GLFWwindow* window;

};