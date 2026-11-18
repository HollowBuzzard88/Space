#pragma once

#include "Space/Core/Application.h"
#include "Space/Core/AssetManager.h"

int main(int argc, char const *argv[])
{
    Space::AssetManager("C:/dev/Space/Resources/Assets/");

    Space::Application* app = new Space::Application();
    app->Run();

    delete app;

    return 0;
}
