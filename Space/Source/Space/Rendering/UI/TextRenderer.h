#pragma once

#include <array>
#include <cstdint>

#include "Space/Rendering/VertexArray.h"
#include "Space/Rendering/Shader.h"

namespace Space
{
    class TextRenderer
    {
    public:
        TextRenderer() = default;
        ~TextRenderer();

        TextRenderer(const TextRenderer&) = delete;
        TextRenderer& operator=(const TextRenderer&) = delete;

        // Needs a current OpenGL context. Returns false if the font could not be loaded.
        bool Init(const std::string& fontPath, float pixelHeight = 18.0f);

        // @param baselinePos left end of the text's baseline, in pixels (y up).
        void Draw(const std::string& text, glm::vec2 baselinePos, const glm::vec4& color, const glm::vec2& viewportSize);

        // @returns The width of the given text based on the font and font size
        float MeasureText(const std::string& text) const;
        
        // Splits text on '\n' and word-wraps it so every line fits into maxWidth pixels.
        std::vector<std::string> WrapText(const std::string& text, float maxWidth) const;
    public:
        float GetLineHeight() const { return m_LineHeight; }
        float GetAscent() const { return m_Ascent; }    // positive, above baseline
        float GetDescent() const { return m_Descent; }  // negative, below baseline

        bool IsReady() const { return m_Shader && m_Shader->GetID() != 0 && m_Texture != 0; }

        static std::shared_ptr<TextRenderer> Create();
    private:
        struct Glyph
        {
            float x0, y0, x1, y1;   // position in the atlas (pixels)
            float xOff, yOff;       // offset from the open position (yOff is y-down, relative to baseline)
            float xadvance;
        };

        struct Vertex
        {
            float x, y, u, v;
        };
    private:
        static constexpr int FIRST_CHAR = 32;
        static constexpr int CHAR_COUNT = 95;
        static constexpr int ATLAS_SIZE = 512;
        static constexpr size_t MAX_GLYPHS = 4096;

        const Glyph& GetGlyph(unsigned char c) const;

        std::array<Glyph, CHAR_COUNT> m_Glyphs;

        float m_Ascent = 0.0f;
        float m_Descent = 0.0f;
        float m_LineHeight = 0.0f;

        uint32_t m_Texture = 0;
        std::shared_ptr<Shader> m_Shader = 0;
        std::shared_ptr<VertexArray> m_VA = 0;
        uint32_t m_VB = 0;
    };
}
