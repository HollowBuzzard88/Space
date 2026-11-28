#pragma once

namespace Space
{
    enum class Key : uint16_t
    {
        UNKNOWN = 0U,

        /* Printable keys */

        SPACE           = 1U,
        APOSTROPHE      = 2U,    /* ' */
        COMMA           = 3U,    /* , */
        MINUS           = 4U,    /*'-'*/
        PERIOD          = 5U,    /* . */
        SLASH           = 6U,    /* / */
        Char_0          = 7U,
        Char_1          = 8U,
        Char_2          = 9U,
        Char_3          = 10U,
        Char_4          = 11U,
        Char_5          = 12U,
        Char_6          = 13U,
        Char_7          = 14U,
        Char_8          = 15U,
        Char_9          = 16U,
        SEMICOLON       = 17U,   /* ; */
        EQUAL           = 18U,   /* = */
        A               = 19U,
        B               = 20U,
        C               = 21U,
        D               = 22U,
        E               = 23U,
        F               = 24U,
        G               = 25U,
        H               = 26U,
        I               = 27U,
        J               = 28U,
        K               = 29U,
        L               = 30U,
        M               = 31U,
        N               = 32U,
        O               = 33U,
        P               = 34U,
        Q               = 35U,
        R               = 36U,
        S               = 37U,
        T               = 38U,
        U               = 39U,
        V               = 40U,
        W               = 41U,
        X               = 42U,
        Y               = 43U,
        Z               = 44U,
        LEFT_BRACKET    = 45U,   /* [ */
        BACKSLASH       = 46U,   /* \ */
        RIGHT_BRACKET   = 47U,   /* ] */
        GRAVE_ACCENT    = 48U,   /* ` */
        CUBED           = 49U,   /* ² */
        SQAURED         = 50U,   /* ³ */

        /* Function keys */
        
        ESCAPE          = 51U,
        ENTER           = 52U,
        TAB             = 53U,
        BACKSPACE       = 54U,
        INSERT          = 55U,
        FN_DELETE       = 56U,
        RIGHT           = 57U,
        LEFT            = 58U,
        DOWN            = 59U,
        UP              = 60U,
        PAGE_UP         = 61U,
        PAGE_DOWN       = 62U,
        HOME            = 63U,
        END             = 64U,
        CAPS_LOCK       = 65U,
        SCROLL_LOCK     = 66U,
        NUM_LOCK        = 67U,
        PRINT_SCREEN    = 68U,
        PAUSE           = 69U,
        F1              = 70U,
        F2              = 71U,
        F3              = 72U,
        F4              = 73U,
        F5              = 74U,
        F6              = 75U,
        F7              = 76U,
        F8              = 77U,
        F9              = 78U,
        F10             = 79U,
        F11             = 80U,
        F12             = 81U,
        F13             = 82U,
        F14             = 83U,
        F15             = 84U,
        F16             = 85U,
        F17             = 86U,
        F18             = 87U,
        F19             = 88U,
        F20             = 89U,
        F21             = 90U,
        F22             = 91U,
        F23             = 92U,
        F24             = 93U,
        KP_0            = 94U,
        KP_1            = 95U,
        KP_2            = 96U,
        KP_3            = 97U,
        KP_4            = 98U,
        KP_5            = 99U,
        KP_6            = 100U,
        KP_7            = 101U,
        KP_8            = 102U,
        KP_9            = 103U,
        KP_DECIMAL      = 104U,
        KP_DIVIDE       = 105U,
        KP_MULTIPLY     = 106U,
        KP_SUBTRACT     = 107U,
        KP_ADD          = 108U,
        KP_ENTER        = 109U,
        KP_EQUAL        = 110U,
        LEFT_SHIFT      = 111U,
        LEFT_CONTROL    = 112U,
        LEFT_ALT        = 113U,
        LEFT_SUPER      = 114U,
        RIGHT_SHIFT     = 115U,
        RIGHT_CONTROL   = 116U,
        RIGHT_ALT       = 117U,
        RIGHT_SUPER     = 118U,
        MENU            = 119U,


        SHIFT           = 120U,
        CONTROL         = 121U,
        ALT             = 122U,
        SUPER           = 123U,

        COUNT,

        ANY_CHAR,                   /* A wild card for any renderable character*/
        FIRST_CHAR      = SPACE,    /* Utility for the Input script to be easily check for a renderable char*/
        LAST_CHAR       = SQAURED,  /* Utility for the Input script to be easily check for a renderable char*/
    };

    int KeyCodeToVK(Key keycode);
    Key VKToKeyCode(uint8_t keycode);
}
