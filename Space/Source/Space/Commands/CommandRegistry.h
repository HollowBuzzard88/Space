#pragma once

namespace Space
{
    enum class CommandID
    {
        #define COMMAND(id, str, desc) id,
        #include "Space/Commands/Commands.def"
        #undef COMMAND
        COUNT
    };

    class CommandRegistry
    {
    public:
        using Callback = std::function<void(const std::string*)>;
    public:
        static bool Register(CommandID id, Callback fn)
        {
            GetCallbacks()[id] = fn;
            return true; 
        }

        static void Execute(const std::string& input, const std::string* args)
        {
            auto& nameMap = GetNameMap();
            if (nameMap.contains(input))
            {
                CommandID id = nameMap[input];
                if (GetCallbacks().contains(id))
                {
                    GetCallbacks()[id](args);
                }
            }
        }

        static void Execute(CommandID id, const std::string* args)
        {
            if (GetCallbacks().contains(id))
            {
                GetCallbacks()[id](args);
            }
        }
    private:
        static std::unordered_map<CommandID, Callback>& GetCallbacks()
        {
            static std::unordered_map<CommandID, Callback> callbacks;
            return callbacks;
        }

        static std::unordered_map<std::string, CommandID>& GetNameMap()
        {
            static std::unordered_map<std::string, CommandID> map = {
                #define COMMAND(id, str, desc) { str, CommandID::id },
                #include "Space/Commands/Commands.def"
                #undef COMMAND
            };
            return map;
        }
    };
}
