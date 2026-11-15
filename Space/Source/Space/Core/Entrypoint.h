#pragma once

#include "Application.h"

int main(int argc, char const *argv[])
{
    Space::Application* app = new Space::Application();
    app->Run();

    delete app;

    return 0;
}
