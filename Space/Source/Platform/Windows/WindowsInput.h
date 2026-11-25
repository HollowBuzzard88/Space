#pragma once

#include "Space/Core/Input.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Space
{
    class WindowsInput : public Input
    {
    protected:
        virtual bool IsKeyDownImpl(Key keycode) override;
        virtual bool IsKeyHeldImpl(Key keycode) override;
        virtual bool IsKeyUpImpl(Key keycode) override;

        virtual bool IsMouseButtonPressedImpl(MouseButtons button) override;
        virtual glm::vec2 GetMousePosImpl() override;
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;

        virtual char GetLastCharImpl() override;
    };
}
