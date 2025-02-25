#include "Visuals.hpp"
#include "Offsets.hpp"
#include <Windows.h>
#include <thread>
#include <chrono>

std::mutex DrawM;


// Adjusting the color of health based features like the Health bar:
ImColor HealthColor(float HealthPercentage) {
    float r = (1.0f - HealthPercentage) * 255.0f;
    float g = HealthPercentage * 255.0f;
    return ImColor(static_cast<int>(r), static_cast<int>(g), 0);
}

// Adding an rainbow color theme. Not being used rn tho :)
ImColor RainbowColor(float speed = 0.5f) {
    float time = static_cast<float>(ImGui::GetTime()) * speed;
    float r = sin(time) * 0.5f + 0.5f;
    float g = sin(time + 2.0f) * 0.5f + 0.5f;
    float b = sin(time + 4.0f) * 0.5f + 0.5f;

    return ImColor(static_cast<int>(r * 255), static_cast<int>(g * 255), static_cast<int>(b * 255));
}
// Nice white/pink fading animation for all Visuals
// U can just replace it with a normal colour or the RainbowColor method above!
ImColor GetFadingColor() {
    float time = static_cast<float>(ImGui::GetTime());
    float fadeFactor = (sin(time) + 1.0f) / 2.0f;

    int rWhite = static_cast<int>(255 * (1.0f - fadeFactor));
    int gWhite = static_cast<int>(255 * (1.0f - fadeFactor));
    int bWhite = static_cast<int>(255 * (1.0f - fadeFactor));

    int rPink = static_cast<int>(255 * fadeFactor);
    int gPink = static_cast<int>(105 * fadeFactor);
    int bPink = static_cast<int>(180 * fadeFactor);

    int finalR = rWhite + rPink;
    int finalG = gWhite + gPink;
    int finalB = bWhite + bPink;

    return ImColor(finalR, finalG, finalB);
}
// This is were the ESP features gets drawn into the game! (Screen)
void DrawEntity(const CEntities::Entity& Entity, ImDrawList* DrawList) {
    auto ViewMatrix = reinterpret_cast<float(*)[4][4]>(CEntities::Client + Offsets::ViewMatrix);

    Vector2 Feet, Head;
    if (Entity.FeetPos.WorldToScreen(Feet, ViewMatrix) && Entity.HeadPos.WorldToScreen(Head, ViewMatrix)) {
        float Height = Feet.Y - Head.Y;
        float Width = Height * 0.5f;

        float HealthWidth = 4.0f;
        float HealthPadding = 4.0f;
        float HealthHeight = Height * (Entity.Health / 100.0f);

        float HealthPercentage = Entity.Health / 100.0f;

        ImColor Bar = HealthColor(HealthPercentage);
        ImColor Background = ImColor(0x00, 0x00, 0x00, 76);
        ImColor Border = GetFadingColor();
        ImColor Text = GetFadingColor();

        ImVec2 MousePos = ImGui::GetMousePos();

        ImVec2 BoxMin = { Feet.X - Width / 2, Head.Y };
        ImVec2 BoxMax = { Feet.X + Width / 2, Head.Y + Height };

        if (MousePos.x >= BoxMin.x && MousePos.x <= BoxMax.x && MousePos.y >= BoxMin.y && MousePos.y <= BoxMax.y) {
            Border = ImColor(234, 93, 96);
            Text = ImColor(234, 93, 96);
        }
        //Box ESP
        if (Globals::ESP::Box) {
            DrawList->AddRectFilled(BoxMin, BoxMax, Background, 0.0f);
            DrawList->AddRect(BoxMin, BoxMax, Border, 0.0f);
        }
        //Name ESP
        if (Globals::ESP::Name) {
            ImVec2 TextPos = { Feet.X - Width / 2, Head.Y - 15.0f };
            DrawList->AddText(TextPos, Text, Entity.Name.c_str());
        }
        //Health ESP
        if (Globals::ESP::Health) {
            DrawList->AddRectFilled({ Feet.X - Width / 2 - HealthWidth - HealthPadding, Head.Y }, { Feet.X - Width / 2 - HealthPadding, Head.Y + Height }, Background, 10.0f);
            DrawList->AddRectFilled({ Feet.X - Width / 2 - HealthWidth - HealthPadding, Head.Y + Height - HealthHeight }, { Feet.X - Width / 2 - HealthPadding, Head.Y + Height }, Bar, 10.0f);
        }
        //HealthText ESP
        if (Globals::ESP::HealthText) {
            ImVec2 TextPos = { Feet.X - Width / 2 + 2.0f, Head.Y + Height - 15.0f };
            DrawList->AddText(ImVec2(TextPos.x, TextPos.y), Text, std::to_string(static_cast<int>(Entity.Health)).c_str());
        }
        //Arrow/Line/Snapline ESP
        // Tried making an option to use like arrows showing the direction but.. im retarded and its hard bugged!
        if (Globals::ESP::Arrows) {
            ImGuiViewport* Viewport = ImGui::GetMainViewport();
            ImVec2 screenCenter = ImVec2(Viewport->GetCenter().x, Viewport->GetCenter().y);

            ImVec2 BoxCenter = ImVec2((BoxMin.x + BoxMax.x) / 2, (BoxMin.y + BoxMax.y) / 2);

            ImColor lineColor = GetFadingColor();

            if (Globals::ESP::ArrowAsTriangle) {
                float deltaX = BoxCenter.x - screenCenter.x;
                float deltaY = BoxCenter.y - screenCenter.y;

                float length = sqrt(deltaX * deltaX + deltaY * deltaY);
                deltaX /= length;
                deltaY /= length;

                float arrowSize = 20.0f;
                float offsetX = deltaX * arrowSize;
                float offsetY = deltaY * arrowSize;

                float spaceBetween = 5.0f;
                ImVec2 offsetScreenCenter = ImVec2(screenCenter.x + deltaX * spaceBetween, screenCenter.y + deltaY * spaceBetween);

                ImVec2 p1 = offsetScreenCenter;
                ImVec2 p2 = ImVec2(offsetScreenCenter.x + offsetX, offsetScreenCenter.y + offsetY);

                ImVec2 p3 = ImVec2(p2.x - arrowSize / 2, p2.y - arrowSize);
                ImVec2 p4 = ImVec2(p2.x + arrowSize / 2, p2.y - arrowSize);

                float angle = atan2(deltaY, deltaX);

                float cosAngle = cos(angle);
                float sinAngle = sin(angle);

                auto RotatePoint = [&](ImVec2 point, ImVec2 center, float angle) {
                    float dx = point.x - center.x;
                    float dy = point.y - center.y;
                    return ImVec2(
                        center.x + dx * cos(angle) - dy * sin(angle),
                        center.y + dx * sin(angle) + dy * cos(angle)
                    );
                    };

                p1 = RotatePoint(p1, offsetScreenCenter, angle);
                p2 = RotatePoint(p2, offsetScreenCenter, angle);
                p3 = RotatePoint(p3, offsetScreenCenter, angle);
                p4 = RotatePoint(p4, offsetScreenCenter, angle);

                DrawList->AddTriangleFilled(p1, p3, p4, lineColor);
            }
            else {
                DrawList->AddLine(screenCenter, BoxCenter, lineColor, 1.0f);
            }
        }
    }
}

template <typename T>
T Clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}


// This isnt changing any world color its just overlaying an darker screen block u can adjust!
void WorldColor() {
    if (Globals::ESP::WorldColor) {
        Globals::ESP::DarknessLevel = Clamp(Globals::ESP::DarknessLevel, 0.0f, 1.0f);

        ImDrawList* DrawList = ImGui::GetBackgroundDrawList();
        ImGuiViewport* Viewport = ImGui::GetMainViewport();

        ImVec2 ScreenMin = Viewport->Pos;
        ImVec2 ScreenMax = { ScreenMin.x + Viewport->Size.x, ScreenMin.y + Viewport->Size.y };

        int darknessIntensity = static_cast<int>(Globals::ESP::DarknessLevel * 255);
        ImColor overlayColor = ImColor(0, 0, 0, darknessIntensity);

        DrawList->AddRectFilled(ScreenMin, ScreenMax, overlayColor);
    }
}

// Draws aimbot FOV
void DrawFOV(ImDrawList* DrawList) {
    if (Globals::Aimbot::DrawFOV) {
        ImGuiViewport* Viewport = ImGui::GetMainViewport();
        ImVec2 Center = ImVec2(Viewport->GetCenter().x, Viewport->GetCenter().y);

        ImColor circleColor = GetFadingColor();

        DrawList->AddCircle(Center, Globals::Aimbot::FOV, circleColor, 32, 1.0f);
    }
}
// Draws an X Crosshair
void Crosshair() {
    if (Globals::ESP::Crosshair) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 center = viewport->GetCenter();

        ImColor crosshairColor = GetFadingColor();

        float size = 6.9f;

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();

        drawList->AddLine(ImVec2(center.x - size, center.y - size), ImVec2(center.x + size, center.y + size), crosshairColor, 2.0f);
        drawList->AddLine(ImVec2(center.x - size, center.y + size), ImVec2(center.x + size, center.y - size), crosshairColor, 2.0f);

    }
}
// Add things which arent in the DrawEntity here by using "Visual-name();"
// if u dont add these here it wont work!
void Visuals::Players() {
    WorldColor();
    Crosshair();
    Entities->Update();

    ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

    for (const auto& Entity : Entities->GetEntities())
        DrawEntity(Entity, DrawList);

    DrawFOV(DrawList);
}
