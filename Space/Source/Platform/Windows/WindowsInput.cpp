#include "SP_pch.h"
#include "Platform/Windows/WindowsInput.h"

namespace Space
{
    Input* Input::s_Instance = new WindowsInput();

    bool WindowsInput::IsKeyPressedImpl(int keycode)
    {
        short keyValue = GetAsyncKeyState(keycode);

        // If the value is 0, the check failed
        if (keyValue == 0)
        {
            //TODO: Add Error Logging
            return false;
        }

        // Since the Win32 API uses the sign bit to determine wether the key is down or not, we compare the value to 0
        // If the value is positive, the key is not pressed
        return (keyValue < 0);
    }

    bool WindowsInput::IsMouseButtonPressedImpl(int button)
    {
        return WindowsWindow::wndMouseButtons[button];
    }

    std::pair<float, float> WindowsInput::GetMousePosImpl()
    {
        return WindowsWindow::wndMousePos;
    }

    float WindowsInput::GetMouseXImpl() { return WindowsWindow::wndMousePos.first; }
    float WindowsInput::GetMouseYImpl() { return WindowsWindow::wndMousePos.second; }
}
