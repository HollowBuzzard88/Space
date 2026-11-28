#pragma once

namespace Space
{
    class Color
    {
    public:
        float r, g, b, a;

        constexpr Color(const float red, const float green, const float blue, const float alpha)
            : r(red / 255), g(green / 255), b(blue / 255), a(alpha / 255) {}
        constexpr Color(const glm::vec4& color)
            : r(color.r / 255), g(color.g / 255), b(color.b / 255), a(color.a / 255) {}
        
        operator glm::vec4() const
        {
            return glm::vec4(r, g, b, a);
        }
    public:
        static const Color& UI_DARK_MAIN;
        static const Color& UI_DARK_SECONDARY;
        static const Color& UI_DARK_TERTIARY;
        static const Color& UI_DARK_QUATERNARY;

        static const Color& UI_DARK_TEXT;
        static const Color& UI_DARK_TEXT_DIM;

        static const Color& UI_DARK_TEXT_INFO;
        static const Color& UI_DARK_TEXT_WARN;
        static const Color& UI_DARK_TEXT_ERROR;
    };

    inline constexpr const Color& Color::UI_DARK_MAIN       = Color(51.0f, 47.0f, 65.0f, 255.0f);
    inline constexpr const Color& Color::UI_DARK_SECONDARY  = Color(74.0f, 84.0f, 98.0f, 255.0f);
    inline constexpr const Color& Color::UI_DARK_TERTIARY   = Color(109.0f, 117.0f, 141.0f, 255.0f);
    inline constexpr const Color& Color::UI_DARK_QUATERNARY = Color(120.0f, 128.0f, 154.0f, 255.0f);

    inline constexpr const Color& Color::UI_DARK_TEXT       = Color(179.0f, 185.0f, 209.0f, 255.0f);
    inline constexpr const Color& Color::UI_DARK_TEXT_DIM   = Color(139.0f, 147.0f, 175.0f, 255.0f);

    inline constexpr const Color& Color::UI_DARK_TEXT_INFO  = Color(89.0f, 193.0f, 53.0f, 255.0f);
    inline constexpr const Color& Color::UI_DARK_TEXT_WARN  = Color(249.0f, 163.0f, 27.0f, 255.0f);
    inline constexpr const Color& Color::UI_DARK_TEXT_ERROR = Color(180.0f, 32.0f, 42.0f, 255.0f);
}
