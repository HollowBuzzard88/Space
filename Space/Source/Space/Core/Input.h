#pragma once

#include "Space/Core/MouseCodes.h"
#include "Space/Core/KeyCodes.h"

namespace Space
{
    class Input
    {
    public:
        inline static bool IsKeyDown(Key keycode) { return s_Instance->IsKeyDownImpl(keycode); }
        inline static bool IsKeyHeld(Key keycode) { return s_Instance->IsKeyHeldImpl(keycode); }
        inline static bool IsKeyUp(Key keycode) { return s_Instance->IsKeyUpImpl(keycode); }

        inline static bool IsMouseButtonPressed(MouseButtons button) { return s_Instance->IsMouseButtonPressedImpl(button); }
        inline static glm::vec2 GetMousePos() { return s_Instance->GetMousePosImpl(); }
        inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
        inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

        inline static char GetLastChar() { return s_Instance->GetLastCharImpl(); }

    protected:
        virtual bool IsKeyDownImpl(Key keycode) = 0;
        virtual bool IsKeyHeldImpl(Key keycode) = 0;
        virtual bool IsKeyUpImpl(Key keycode) = 0;

        virtual bool IsMouseButtonPressedImpl(MouseButtons button) = 0;
        virtual glm::vec2 GetMousePosImpl() = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;

        virtual char GetLastCharImpl() = 0;
    private:
        static Input* s_Instance;
    };
}
