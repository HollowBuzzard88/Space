#pragma once

namespace Space
{
    enum class MessageType : uint8_t
    {
        TRACE = 0,
        INFO = 1,
        WARN = 2,
        ERROR = 3
    };

    enum class MessageSource : uint8_t
    {
        CLIENT = 0,
        SERVER = 0
    };

    struct Message
    {
        MessageType Type : 2;
        MessageSource Source : 1;
        uint32_t Time;

        const char* Message;
    };
}
