#include "SP_pch.h"
#include "Space/Rendering/TextRenderer.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <cmath>
#include <fstream>

#include "Space/Core/AssetManager.h"

namespace Space
{
    std::shared_ptr<TextRenderer> TextRenderer::Create()
    {
        return std::make_shared<TextRenderer>();
    }

    TextRenderer::~TextRenderer()
    {
        if (m_Texture) glDeleteTextures(1, &m_Texture);
        if (m_VB) glDeleteBuffers(1, &m_VB);
    }

    bool TextRenderer::Init(const std::string &fontPath, float pixelHeight)
    {
        // -------------------- Load the .ttf file -------------------- //
        std::ifstream file(fontPath, std::ios::binary | std::ios::ate);
        if (!file)
        {
            SP_CORE_ERROR("TextRenderer: Could not open font-file '{0}'", fontPath);
            return false;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<unsigned char> fontData((size_t)size);
        if (!file.read((char*)fontData.data(), size))
        {
            SP_CORE_ERROR("TextRenderer: Could not read font-file '{0}'", fontPath);
            return false;
        }

        int fontOffset = stbtt_GetFontOffsetForIndex(fontData.data(), 0);

        stbtt_fontinfo info;
        if (!stbtt_InitFont(&info, fontData.data(), fontOffset))
        {
            SP_CORE_ERROR("TextRenderer: '{0}' is not a valid font", fontPath);
            return false;
        }

        float scale = stbtt_ScaleForPixelHeight(&info, pixelHeight);
        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

        m_Ascent = ascent * scale;
        m_Descent = descent * scale;
        m_LineHeight = (ascent - descent + lineGap) * scale;

        // -------------------- Bake the glyphs into an atlas -------------------- //
        std::vector<unsigned char> bitmap(ATLAS_SIZE * ATLAS_SIZE);
        stbtt_bakedchar baked[CHAR_COUNT];

        int result = stbtt_BakeFontBitmap(fontData.data(), fontOffset, pixelHeight,
                                            bitmap.data(), ATLAS_SIZE, ATLAS_SIZE,
                                            FIRST_CHAR, CHAR_COUNT, baked);
        if (result <= 0)
        {
            SP_CORE_ERROR("TextRenderer: Glyphs do not fit into the {0}x{0} atlas (reduce the font size)", ATLAS_SIZE);
            return false;
        }

        for (int i = 0; i < CHAR_COUNT; i++)
        {
            m_Glyphs[i] = { (float)baked[i].x0, (float)baked[i].y0, (float)baked[i].x1, (float)baked[i].y1,
                            (float)baked[i].xoff, (float)baked[i].yoff, (float)baked[i].xadvance };
        }

        // -------------------- Atlas texture -------------------- //
        GLint prevTexture = 0, prevAlignment = 4;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTexture);
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &prevAlignment);

        glGenTextures(1, &m_Texture);
        glBindTexture(GL_TEXTURE_2D, m_Texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, ATLAS_SIZE, ATLAS_SIZE, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glPixelStorei(GL_UNPACK_ALIGNMENT, prevAlignment);
        glBindTexture(GL_TEXTURE_2D, (GLuint)prevTexture);
 
        // -------------------- Shader program -------------------- //
        m_Shader = Shader::Create(AssetManager::GetAbsolutePath("Shaders/textured.glsl"));

        GLint previousProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &previousProgram);

        m_Shader->Bind();
        m_Shader->SetInt("uAtlas", 0);

        glUseProgram((GLuint)previousProgram);

        // -------------------- Dynamic vertex buffer --------------------
        GLint prevVAO = 0;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevVAO);

        m_VA = VertexArray::Create();
        glGenBuffers(1, &m_VB);

        m_VA->Bind();
        glBindBuffer(GL_ARRAY_BUFFER, m_VB);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, u));


        std::vector<uint32_t> indices(MAX_GLYPHS * 6);
        for (uint32_t i = 0; i < MAX_GLYPHS; i++)
        {
            uint32_t o = i * 4;
            uint32_t* p = &indices[i * 6];

            p[0] = o;     p[1] = o + 1; p[2] = o + 2;
            p[3] = o + 2; p[4] = o + 3; p[5] = o;
        }
        std::shared_ptr<IndexBuffer> IB = IndexBuffer::Create(indices.data(), (uint32_t)indices.size());
        m_VA->SetIndexBuffer(IB);

        glBindVertexArray((GLuint)prevVAO);

        return true;
    }

    const TextRenderer::Glyph &TextRenderer::GetGlyph(unsigned char c) const
    {
        if (c < FIRST_CHAR ||c >= FIRST_CHAR + CHAR_COUNT)
            c = '?';

        return m_Glyphs[c - FIRST_CHAR];
    }

    float TextRenderer::MeasureText(const std::string &text) const
    {
        float width = 0.0f;
        for (unsigned char c : text)
            width += GetGlyph(c).xadvance;

        return width;
    }

    std::vector<std::string> TextRenderer::WrapText(const std::string &text, float maxWidth) const
    {
        std::vector<std::string> lines;

        size_t start = 0;
        while (start <= text.size())
        {
            size_t end = text.find('\n', start);
            std::string paragraph = text.substr(start, end == std::string::npos ? std::string::npos : end - start);

            std::string line;
            size_t pos = 0;
            while (pos <= paragraph.size())
            {
                size_t space = paragraph.find(' ', pos);
                std::string word = paragraph.substr(pos, space == std::string::npos ? std::string::npos : space - pos);
                pos = (space == std::string::npos) ? paragraph.size() + 1 : space + 1;

                std::string candidate = line.empty() ? word : line + " " + word;
                if (MeasureText(candidate) <= maxWidth)
                {
                    line = candidate;
                    continue;
                }

                if (!line.empty())
                {
                    lines.push_back(line);
                    line.clear();
                }

                // A single word that is wider than the whole line: break it up by characters
                while (MeasureText(word) > maxWidth && word.size() > 1)
                {
                    size_t n = 1;
                    while (n < word.size() && MeasureText(word.substr(0, n + 1)) <= maxWidth) n++;

                    lines.push_back(word.substr(0, n));
                    word.erase(0, n);
                }
                line = word;
            }
            lines.push_back(line);

            if (end == std::string::npos) break;
            start = end + 1;
        }

        return lines;
    }

    void TextRenderer::Draw(const std::string &text, glm::vec2 baselinePos, const glm::vec4 &color, const glm::vec2 &viewportSize)
    {
        if (m_Shader->GetID() == 0 || text.empty())
            return;
        
        // -------------------- Build one quad (2 triangles) per glyph -------------------- //
        const size_t textSize = text.size();
        const size_t glyphCount = std::min(textSize, MAX_GLYPHS);

        std::vector<Vertex> vertices;
        vertices.reserve(glyphCount * 4);

        float penX = std::floor(baselinePos.x);
        float baseY = std::floor(baselinePos.y);
        const float inv = 1.0f / (float)ATLAS_SIZE;

        for (size_t i = 0; i < glyphCount; i++)
        {
            const Glyph& g = GetGlyph((unsigned char)text[i]);

            float x0 = penX + g.xOff;
            float x1 = x0 + (g.x1 - g.x0);
            float yTop = baseY - g.yOff;
            float yBottom = yTop - (g.y1 - g.y0);

            float u0 = g.x0 * inv, u1 = g.x1 * inv;
            float vTop = g.y0 * inv, vBottom = g.y1 * inv;

            vertices.push_back({ x0, yBottom, u0, vBottom });
            vertices.push_back({ x1, yBottom, u1, vBottom });
            vertices.push_back({ x1, yTop, u1, vTop });
            vertices.push_back({ x0, yTop, u0, vTop });

            penX += g.xadvance;
        }

        // -------------------- Save the current GL -------------------- //
        GLint prevProgram = 0, prevVAO = 0, prevTexture = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &prevProgram);
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevVAO);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTexture);

        GLboolean blendWasOn = glIsEnabled(GL_BLEND);
        GLboolean depthWasOn = glIsEnabled(GL_DEPTH_TEST);
        GLboolean cullWasOn = glIsEnabled(GL_CULL_FACE);

        // -------------------- Draw -------------------- //
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glm::mat4 proj = glm::ortho(0.0f, viewportSize.x, 0.0f, viewportSize.y);

        m_Shader->Bind();
        m_Shader->SetMat4("uProj", proj);
        m_Shader->SetVec4("uColor", color);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Texture);

        m_VA->Bind();
        glBindBuffer(GL_ARRAY_BUFFER, m_VB);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

        glDrawElements(GL_TRIANGLES, (GLsizei)(glyphCount * 6), GL_UNSIGNED_INT, nullptr);

        // -------------------- Restore -------------------- //
        glBindTexture(GL_TEXTURE_2D, (GLuint)prevTexture);
        glBindVertexArray((GLuint)prevVAO);
        glUseProgram((GLuint)prevProgram);

        if (!blendWasOn) glDisable(GL_BLEND);
        if (depthWasOn) glEnable(GL_DEPTH_TEST);
        if (cullWasOn) glEnable(GL_CULL_FACE);
    }
}
