#pragma once

namespace Space
{
    enum class Key : uint8_t
    {
        UNKNOWN = 0,

        /* Printable keys */

        SPACE           = 1,
        APOSTROPHE      = 2,    /* ' */
        COMMA           = 3,    /* , */
        MINUS           = 4,    /*'-'*/
        PERIOD          = 5,    /* . */
        SLASH           = 6,    /* / */
        Char_0          = 7,
        Char_1          = 8,
        Char_2          = 9,
        Char_3          = 10,
        Char_4          = 11,
        Char_5          = 12,
        Char_6          = 13,
        Char_7          = 14,
        Char_8          = 15,
        Char_9          = 16,
        SEMICOLON       = 17,   /* ; */
        EQUAL           = 18,   /* = */
        A               = 19,
        B               = 20,
        C               = 21,
        D               = 22,
        E               = 23,
        F               = 24,
        G               = 25,
        H               = 26,
        I               = 27,
        J               = 28,
        K               = 29,
        L               = 30,
        M               = 31,
        N               = 32,
        O               = 33,
        P               = 34,
        Q               = 35,
        R               = 36,
        S               = 37,
        T               = 38,
        U               = 39,
        V               = 40,
        W               = 41,
        X               = 42,
        Y               = 43,
        Z               = 44,
        LEFT_BRACKET    = 45,   /* [ */
        BACKSLASH       = 46,   /* \ */
        RIGHT_BRACKET   = 47,   /* ] */
        GRAVE_ACCENT    = 48,   /* ` */
        CUBED           = 49,   /* ² */
        SQAURED         = 50,   /* ³ */

        /* Function keys */
        ESCAPE          = 61,
        ENTER           = 62,
        TAB             = 63,
        BACKSPACE       = 64,
        INSERT          = 65,
        FN_DELETE          = 66,
        RIGHT           = 67,
        LEFT            = 68,
        DOWN            = 69,
        UP              = 70,
        PAGE_UP         = 71,
        PAGE_DOWN       = 72,
        HOME            = 73,
        END             = 74,
        CAPS_LOCK       = 75,
        SCROLL_LOCK     = 76,
        NUM_LOCK        = 77,
        PRINT_SCREEN    = 78,
        PAUSE           = 79,
        F1              = 80,
        F2              = 81,
        F3              = 82,
        F4              = 83,
        F5              = 84,
        F6              = 85,
        F7              = 86,
        F8              = 87,
        F9              = 88,
        F10             = 89,
        F11             = 90,
        F12             = 91,
        F13             = 92,
        F14             = 93,
        F15             = 94,
        F16             = 95,
        F17             = 96,
        F18             = 97,
        F19             = 98,
        F20             = 99,
        F21             = 100,
        F22             = 101,
        F23             = 102,
        F24             = 103,
        KP_0            = 104,
        KP_1            = 105,
        KP_2            = 106,
        KP_3            = 107,
        KP_4            = 108,
        KP_5            = 109,
        KP_6            = 110,
        KP_7            = 111,
        KP_8            = 112,
        KP_9            = 113,
        KP_DECIMAL      = 114,
        KP_DIVIDE       = 115,
        KP_MULTIPLY     = 116,
        KP_SUBTRACT     = 117,
        KP_ADD          = 118,
        KP_ENTER        = 129,
        KP_EQUAL        = 120,
        LEFT_SHIFT      = 121,
        LEFT_CONTROL    = 122,
        LEFT_ALT        = 123,
        LEFT_SUPER      = 124,
        RIGHT_SHIFT     = 125,
        RIGHT_CONTROL   = 126,
        RIGHT_ALT       = 127,
        RIGHT_SUPER     = 128,
        MENU            = 129,


        SHIFT           = 130,
        CONTROL         = 131,
        ALT             = 132,
        SUPER           = 133,

        ANY_CHAR        = 200,      /* A wild card for any renderable character*/
        FIRST_CHAR      = CUBED,    /* Utility for the Input script to be easily check for a renderable char*/
        LAST_CHAR       = SQAURED,  /* Utility for the Input script to be easily check for a renderable char*/

        COUNT
    };

    int KeyCodeToVK(Key keycode);
    Key VKToKeyCode(uint8_t keycode);
}
