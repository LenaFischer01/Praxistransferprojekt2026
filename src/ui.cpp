#include "ui.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

UI::UI(GLFWwindow* window) : window_(window) {
    InitImGui(window);
}

void UI::InitImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::startFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::defineStyleAndUi() {
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Checkbox("Spur", &params.showTrace);
    ImGui::SliderFloat("Länge 1", &params.length1, 0.1f, 5.0f);
    ImGui::SliderFloat("Länge 2", &params.length2, 0.1f, 5.0f);

    ImGui::SliderFloat("Masse 1", &params.mass1, 0.1f, 10.0f);
    ImGui::SliderFloat("Masse 2", &params.mass2, 0.1f, 10.0f);

    ImGui::SliderFloat("Gravitation", &params.gravity, 0.0f, 20.0f);

    if (ImGui::SliderFloat("Startwinkel 1", &params.angle1, -180.0f, 180.0f)) {
        params.run = false;
    }
    if (ImGui::SliderFloat("Startwinkel 2", &params.angle2, -180.0f, 180.0f)) {
        params.run = false;
    }

    if (ImGui::Button("Start simulation")) {
        params.run = true;
    }
    ImGui::SameLine();

    if (ImGui::Button("Stop simulation")) {
        params.run = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset simulation")) {
        resetParameters();
    }

    ImGui::End();
}

void UI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}