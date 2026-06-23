#include "ui.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "config.h"
#include "Calculation/CalculationHandler.h"

#include "Calculation/CalcEuler.h"
#include "Calculation/CalcRK4.h"
#include "Calculation/CalcLeapFrog.h"

UI::UI(GLFWwindow* window) : window_(window) {
    InitImGui(window);
}

inline void SetupImGuiStyle(float alpha_)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        
        //  Style dougbinks from github: https://gist.github.com/dougbinks/8089b4bbaccaaf6fa204236978d165a9#file-imguiutils-h-L9-L93
        style.Alpha = 1.0f;
        style.FrameRounding = 3.0f;
        style.Colors[ImGuiCol_Text]                  = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
        style.Colors[ImGuiCol_PopupBg]               = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
        style.Colors[ImGuiCol_Border]                = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
        style.Colors[ImGuiCol_BorderShadow]          = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
        style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.26f, 0.59f, 0.98f, 0.15f);
        style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
        style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
        style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
        style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_Button]                = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_Header]                = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
        style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);


        for (int i = 0; i <= ImGuiCol_COUNT; i++)
        {
            ImVec4& col = style.Colors[i];
            float H, S, V;
            ImGui::ColorConvertRGBtoHSV( col.x, col.y, col.z, H, S, V );

            if( S < 0.1f )
            {
                V = 1.0f - V;
            }
            ImGui::ColorConvertHSVtoRGB( H, S, V, col.x, col.y, col.z );
            if( col.w < 1.00f )
            {
                col.w *= alpha_;
            }
        }
    }



void ShiftImGuiHue(float hueOffset)
{
    ImGuiStyle& style = ImGui::GetStyle();

    for (int i = 0; i < ImGuiCol_COUNT; i++)
    {
        ImVec4& c = style.Colors[i];

        float h, s, v;
        ImGui::ColorConvertRGBtoHSV(c.x, c.y, c.z, h, s, v);

        h += hueOffset;
        if (h > 1.0f) h -= 1.0f;
        if (h < 0.0f) h += 1.0f;

        ImGui::ColorConvertHSVtoRGB(h, s, v, c.x, c.y, c.z);
    }
}



void UI::InitImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    SetupImGuiStyle(0.8f);
    ShiftImGuiHue(0.45f);

}

void UI::startFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::defineStyleAndUi(UiState::Parameters& params) {
    ImGui::Begin("Optionen", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Checkbox("Spur", &params.showTrace);
    ImGui::SameLine();
    if (ImGui::InputInt("Spurlänge", &params.traceLength, 100, 1000) && params.traceLength > 0 && params.traceLength <= 99999) {
        params.traceLengthChanged = true;
    }

    ImGui::Separator();

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

    ImGui::Separator();

    if (ImGui::RadioButton("Euler", params.method == AlgorithmSelect::Method::Euler)) {
        params.method = AlgorithmSelect::Method::Euler;
        CalculationHandler::getInstance().setCalculator(std::make_unique<CalcEuler>());
    }

    ImGui::SameLine();

    if (ImGui::RadioButton("Runge Kutta 4", params.method == AlgorithmSelect::Method::RK4)) {
        params.method = AlgorithmSelect::Method::RK4;
        CalculationHandler::getInstance().setCalculator(std::make_unique<CalcRK4>());
    }

    ImGui::SameLine();

    if (ImGui::RadioButton("Leapfrog", params.method == AlgorithmSelect::Method::Leapfrog)) {
        params.method = AlgorithmSelect::Method::Leapfrog;
        CalculationHandler::getInstance().setCalculator(std::make_unique<CalcLeapFrog>());
    }

    ImGui::Separator();

    if (ImGui::Button("Start simulation")) {
        params.run = true;
    }
    ImGui::SameLine();

    if (ImGui::Button("Stop simulation")) {
        params.run = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset simulation")) {
        params.resetRequested = true;
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