#pragma once

namespace Space
{
    class Planet
    {
    public:
        inline static std::vector<float> GeneratePlanet()
        {
            float vertices[]
            {
               -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f,  0.5f, 0.0f
            };

            std::vector<float> output;
            output.append_range(vertices);
            return output;
        }
    };
}
