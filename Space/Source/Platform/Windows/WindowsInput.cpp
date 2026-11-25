#include "SP_pch.h"
#include "Platform/Windows/WindowsInput.h"

namespace Space
{
    Input* Input::s_Instance = new WindowsInput();

    int KeyCodeToVK(Key keycode)
    {
        switch (keycode)
        {
            case Key::SPACE:            return VK_SPACE;
            case Key::COMMA:            return VK_OEM_COMMA;
            case Key::MINUS:            return VK_OEM_MINUS;
            case Key::PERIOD:           return VK_OEM_PERIOD;
            case Key::Char_0:           return 0x30;
            case Key::Char_1:           return 0x31;
            case Key::Char_2:           return 0x32;
            case Key::Char_3:           return 0x33;
            case Key::Char_4:           return 0x34;
            case Key::Char_5:           return 0x35;
            case Key::Char_6:           return 0x36;
            case Key::Char_7:           return 0x37;
            case Key::Char_8:           return 0x38;
            case Key::Char_9:           return 0x39;
            case Key::A:                return 0x41;
            case Key::B:                return 0x42;
            case Key::C:                return 0x43;
            case Key::D:                return 0x44;
            case Key::E:                return 0x45;
            case Key::F:                return 0x46;
            case Key::G:                return 0x47;
            case Key::H:                return 0x48;
            case Key::I:                return 0x49;
            case Key::J:                return 0x4A;
            case Key::K:                return 0x4B;
            case Key::L:                return 0x4C;
            case Key::M:                return 0x4D;
            case Key::N:                return 0x4E;
            case Key::O:                return 0x4F;
            case Key::P:                return 0x50;
            case Key::Q:                return 0x51;
            case Key::R:                return 0x52;
            case Key::S:                return 0x53;
            case Key::T:                return 0x54;
            case Key::U:                return 0x55;
            case Key::V:                return 0x56;
            case Key::W:                return 0x57;
            case Key::X:                return 0x58;
            case Key::Y:                return 0x59;
            case Key::Z:                return 0x5A;
            case Key::ESCAPE:           return VK_ESCAPE;
            case Key::ENTER:            return VK_RETURN;
            case Key::TAB:              return VK_TAB;
            case Key::BACKSPACE:        return VK_BACK;
            case Key::INSERT:           return VK_INSERT;
            case Key::FN_DELETE:        return VK_DELETE;
            case Key::RIGHT:            return VK_RIGHT;
            case Key::LEFT:             return VK_LEFT;
            case Key::DOWN:             return VK_DOWN;
            case Key::UP:               return VK_UP;
            case Key::PAGE_UP:          return VK_PRIOR;
            case Key::PAGE_DOWN:        return VK_NEXT;
            case Key::HOME:             return VK_HOME;
            case Key::END:              return VK_END;
            case Key::CAPS_LOCK:        return VK_CAPITAL;
            case Key::SCROLL_LOCK:      return VK_SCROLL;
            case Key::NUM_LOCK:         return VK_NUMLOCK;
            case Key::PRINT_SCREEN:     return VK_PRINT;
            case Key::PAUSE:            return VK_PAUSE;
            case Key::F1:               return VK_F1;
            case Key::F2:               return VK_F2;
            case Key::F3:               return VK_F3;
            case Key::F4:               return VK_F4;
            case Key::F5:               return VK_F5;
            case Key::F6:               return VK_F6;
            case Key::F7:               return VK_F7;
            case Key::F8:               return VK_F8;
            case Key::F9:               return VK_F9;
            case Key::F10:              return VK_F10;
            case Key::F11:              return VK_F11;
            case Key::F12:              return VK_F12;
            case Key::F13:              return VK_F13;
            case Key::F14:              return VK_F14;
            case Key::F15:              return VK_F15;
            case Key::F16:              return VK_F16;
            case Key::F17:              return VK_F17;
            case Key::F18:              return VK_F18;
            case Key::F19:              return VK_F19;
            case Key::F20:              return VK_F20;
            case Key::F21:              return VK_F21;
            case Key::F22:              return VK_F22;
            case Key::F23:              return VK_F23;
            case Key::F24:              return VK_F24;
            case Key::KP_0:             return VK_NUMPAD0;
            case Key::KP_1:             return VK_NUMPAD1;
            case Key::KP_2:             return VK_NUMPAD2;
            case Key::KP_3:             return VK_NUMPAD3;
            case Key::KP_4:             return VK_NUMPAD4;
            case Key::KP_5:             return VK_NUMPAD5;
            case Key::KP_6:             return VK_NUMPAD6;
            case Key::KP_7:             return VK_NUMPAD7;
            case Key::KP_8:             return VK_NUMPAD8;
            case Key::KP_9:             return VK_NUMPAD9;
            case Key::KP_DECIMAL:       return VK_DECIMAL;
            case Key::KP_DIVIDE:        return VK_DIVIDE;
            case Key::KP_MULTIPLY:      return VK_MULTIPLY;
            case Key::KP_SUBTRACT:      return VK_SUBTRACT;
            case Key::KP_ADD:           return VK_ADD;
            case Key::KP_ENTER:         return VK_RETURN;
            case Key::LEFT_SHIFT:       return VK_LSHIFT;
            case Key::LEFT_CONTROL:     return VK_LCONTROL;
            case Key::LEFT_ALT:         return VK_LMENU;
            case Key::LEFT_SUPER:       return VK_LWIN;
            case Key::RIGHT_SHIFT:      return VK_RSHIFT;
            case Key::RIGHT_CONTROL:    return VK_RCONTROL;
            case Key::RIGHT_ALT:        return VK_RMENU;
            case Key::RIGHT_SUPER:      return VK_RWIN;
            case Key::SHIFT:            return VK_SHIFT;
            case Key::CONTROL:          return VK_CONTROL;
            case Key::ALT:              return VK_MENU;
        }

        SP_CORE_ERROR("Invalid Keycode: {0}", (uint16_t)keycode);
        return -1;
    }
    Key VKToKeyCode(uint8_t keycode)
    {
        switch (keycode)
        {
            case VK_SPACE:              return Key::SPACE;
            case VK_OEM_COMMA:          return Key::COMMA;
            case VK_OEM_MINUS:          return Key::MINUS;
            case VK_SUBTRACT:           return Key::KP_SUBTRACT;
            case VK_OEM_PERIOD:         return Key::PERIOD;
            case 0x30:                  return Key::Char_0;
            case 0x31:                  return Key::Char_1;
            case 0x32:                  return Key::Char_2;
            case 0x33:                  return Key::Char_3;
            case 0x34:                  return Key::Char_4;
            case 0x35:                  return Key::Char_5;
            case 0x36:                  return Key::Char_6;
            case 0x37:                  return Key::Char_7;
            case 0x38:                  return Key::Char_8;
            case 0x39:                  return Key::Char_9;
            case 0x41:                  return Key::A;
            case 0x42:                  return Key::B;
            case 0x43:                  return Key::C;
            case 0x44:                  return Key::D;
            case 0x45:                  return Key::E;
            case 0x46:                  return Key::F;
            case 0x47:                  return Key::G;
            case 0x48:                  return Key::H;
            case 0x49:                  return Key::I;
            case 0x4A:                  return Key::J;
            case 0x4B:                  return Key::K;
            case 0x4C:                  return Key::L;
            case 0x4D:                  return Key::M;
            case 0x4E:                  return Key::N;
            case 0x4F:                  return Key::O;
            case 0x50:                  return Key::P;
            case 0x51:                  return Key::Q;
            case 0x52:                  return Key::R;
            case 0x53:                  return Key::S;
            case 0x54:                  return Key::T;
            case 0x55:                  return Key::U;
            case 0x56:                  return Key::V;
            case 0x57:                  return Key::W;
            case 0x58:                  return Key::X;
            case 0x59:                  return Key::Y;
            case 0x5A:                  return Key::Z;
            case VK_ESCAPE:             return Key::ESCAPE;
            case VK_RETURN:             return Key::ENTER;
            case VK_TAB:                return Key::TAB;
            case VK_BACK:               return Key::BACKSPACE;
            case VK_INSERT:             return Key::INSERT;
            case VK_DELETE:             return Key::FN_DELETE;
            case VK_RIGHT:              return Key::RIGHT;
            case VK_LEFT:               return Key::LEFT;
            case VK_DOWN:               return Key::DOWN;
            case VK_UP:                 return Key::UP;
            case VK_PRIOR:              return Key::PAGE_UP;
            case VK_NEXT:               return Key::PAGE_DOWN;
            case VK_HOME:               return Key::HOME;
            case VK_END:                return Key::END;
            case VK_CAPITAL:            return Key::CAPS_LOCK;
            case VK_SCROLL:             return Key::SCROLL_LOCK;
            case VK_NUMLOCK:            return Key::NUM_LOCK;
            case VK_PRINT:              return Key::PRINT_SCREEN;
            case VK_PAUSE:              return Key::PAUSE;
            case VK_F1:                 return Key::F1;
            case VK_F2:                 return Key::F2;
            case VK_F3:                 return Key::F3;
            case VK_F4:                 return Key::F4;
            case VK_F5:                 return Key::F5;
            case VK_F6:                 return Key::F6;
            case VK_F7:                 return Key::F7;
            case VK_F8:                 return Key::F8;
            case VK_F9:                 return Key::F9;
            case VK_F10:                return Key::F10;
            case VK_F11:                return Key::F11;
            case VK_F12:                return Key::F12;
            case VK_F13:                return Key::F13;
            case VK_F14:                return Key::F14;
            case VK_F15:                return Key::F15;
            case VK_F16:                return Key::F16;
            case VK_F17:                return Key::F17;
            case VK_F18:                return Key::F18;
            case VK_F19:                return Key::F19;
            case VK_F20:                return Key::F20;
            case VK_F21:                return Key::F21;
            case VK_F22:                return Key::F22;
            case VK_F23:                return Key::F23;
            case VK_F24:                return Key::F24;
            case VK_NUMPAD0:            return Key::KP_0;
            case VK_NUMPAD1:            return Key::KP_1;
            case VK_NUMPAD2:            return Key::KP_2;
            case VK_NUMPAD3:            return Key::KP_3;
            case VK_NUMPAD4:            return Key::KP_4;
            case VK_NUMPAD5:            return Key::KP_5;
            case VK_NUMPAD6:            return Key::KP_6;
            case VK_NUMPAD7:            return Key::KP_7;
            case VK_NUMPAD8:            return Key::KP_8;
            case VK_NUMPAD9:            return Key::KP_9;
            case VK_DECIMAL:            return Key::KP_DECIMAL;
            case VK_DIVIDE:             return Key::KP_DIVIDE;
            case VK_MULTIPLY:           return Key::KP_MULTIPLY;
            case VK_ADD:                return Key::KP_ADD;
            case VK_LSHIFT:             return Key::LEFT_SHIFT;
            case VK_LCONTROL:           return Key::LEFT_CONTROL;
            case VK_LMENU:              return Key::LEFT_ALT;
            case VK_LWIN:               return Key::LEFT_SUPER;
            case VK_RSHIFT:             return Key::RIGHT_SHIFT;
            case VK_RCONTROL:           return Key::RIGHT_CONTROL;
            case VK_RMENU:              return Key::RIGHT_ALT;
            case VK_RWIN:               return Key::RIGHT_SUPER;
            case VK_SHIFT:              return Key::SHIFT;
            case VK_CONTROL:            return Key::CONTROL;
            case VK_MENU:               return Key::ALT;
        }

        SP_CORE_ERROR("Invalid Keycode: {0}", (uint16_t)keycode);
        return Key::UNKNOWN;
    }

    bool WindowsInput::IsKeyDownImpl(Key keycode)
    {
        if (keycode == Key::ANY_CHAR)
        {
            for (uint16_t i = (uint16_t)Key::FIRST_CHAR; i < (uint16_t)Key::LAST_CHAR; i++)
            {
                if (WindowsWindow::keyboard[i] && !WindowsWindow::keyboardLast[i]) return true;
            }

            return false;
        }

        return WindowsWindow::keyboard[(uint16_t)keycode] && !WindowsWindow::keyboardLast[(uint16_t)keycode];
    }
    bool WindowsInput::IsKeyHeldImpl(Key keycode)
    {
        if (keycode == Key::ANY_CHAR)
        {
            for (uint16_t i = (uint16_t)Key::FIRST_CHAR; i < (uint16_t)Key::LAST_CHAR; i++)
            {
                if (WindowsWindow::keyboard[i] && WindowsWindow::keyboardLast[i]) return true;
            }

            return false;
        }

        return WindowsWindow::keyboard[(uint16_t)keycode] && WindowsWindow::keyboardLast[(uint16_t)keycode] ;
    }
    bool WindowsInput::IsKeyUpImpl(Key keycode)
    {
        if (keycode == Key::ANY_CHAR)
        {
            for (uint16_t i = (uint16_t)Key::FIRST_CHAR; i < (uint16_t)Key::LAST_CHAR; i++)
            {
                if (!WindowsWindow::keyboard[i]) return true;
            }

            return false;
        }

        return !WindowsWindow::keyboard[(uint16_t)keycode];
    }

    bool WindowsInput::IsMouseButtonPressedImpl(MouseButtons button)
    {
        return WindowsWindow::mouse[(uint8_t)button];
    }

    glm::vec2 WindowsInput::GetMousePosImpl()
    {
        return WindowsWindow::wndMousePos;
    }

    float WindowsInput::GetMouseXImpl() { return WindowsWindow::wndMousePos.x; }
    float WindowsInput::GetMouseYImpl() { return WindowsWindow::wndMousePos.y; }
    char WindowsInput::GetLastCharImpl()
    {
        std::queue<char>& queue = WindowsWindow::keyboardCharQueue;
        if (queue.empty()) return 0;

        char c = queue.front();
        queue.pop();
        return c;
    }
}
