#pragma once

namespace Space
{
    class CommandHandler
    {
    public:
        static void Init();

        static void Execute(const std::string typedText);
    };
}
