#pragma once

namespace Space
{
    struct Rect
    {
        Rect(const glm::vec2& position, const glm::vec2& size) : m_Position(position), m_Size(size) {}

        bool IsInside(const glm::vec2& position) const
        {
            return position.x > m_Position.x && position.x < m_Position.x + m_Size.x &&
                   position.y >= m_Position.y && position.y <= m_Position.y + m_Size.y;
        };
    private:
        glm::vec2 m_Position, m_Size;
    };
}
