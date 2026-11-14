#include "Space/Core/Window.h"
#include "Space/Core/Core.h"

#include "Space/Core/Input.h"

#ifdef SP_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsInput.h"
#elifdef SP_PLATFORM_MAC
    #error No iOS implementation yet!
#elifdef SP_PLATFORM_LINUX
    #error No Linux implementation yet!
#endif

int main(int argc, char const *argv[])
{
    Space::WindowProps props;

    std::unique_ptr<Space::Window> window = std::unique_ptr<Space::Window>(Space::Window::Create(props));

    while (!window->ShouldClose())
    {
        window->OnUpdate();
    }
    
    return 0;
}
