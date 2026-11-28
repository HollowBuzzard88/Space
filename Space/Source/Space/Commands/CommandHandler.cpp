#include "SP_pch.h"
#include "Space/Commands/CommandHandler.h"

#include "Space/Commands/CommandRegistry.h"

#include "Space/Core/Application.h"

#include <Windows.h>
#include <tlhelp32.h>

namespace Space
{
    void CommandHandler::Init()
    {
        CommandRegistry::Register(CommandID::Kill, [](const std::string* args)
        {
            if (args != nullptr)
            {
                if (!args->empty())
                {
                    SP_CORE_ERROR("Command: 'Kill' doesn't take any arguments!");
                    return;
                }
            }

            SP_CORE_TRACE("Shutting down Process...");

            Application::Get().Shutdown();
        });
    }

    void CommandHandler::Execute(const std::string typedText)
    {
        std::istringstream stream(typedText.substr(0));
        std::string name;
        stream >> name; // the first word is used as the name

        std::vector<std::string> args;

        for (std::string arg; stream >> arg;) args.push_back(arg.c_str());

        CommandRegistry::Execute(name, args.data());
    }
}
