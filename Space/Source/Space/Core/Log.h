#pragma once

#include "Space/Core/Message.h"

#include <chrono>

#define MAX_MESSAGES 1024

namespace Space
{
    class Renderer;
    class LogRenderer;

    class Log
    {
    public:
        static void Init(std::shared_ptr<Renderer> renderer);
        static void RenderLogger();
    public:
        template <typename... Args>
        static void Trace(MessageSource source, const char* message, Args... args) { ProcessMessage(source, MessageType::TRACE, message, args...); }
        template <typename... Args>
        static void Info(MessageSource source, const char* message, Args... args) { ProcessMessage(source, MessageType::INFO, message, args...); }
        template <typename... Args>
        static void Warn(MessageSource source, const char* message, Args... args) { ProcessMessage(source, MessageType::WARN, message, args...); }
        template <typename... Args>
        static void Error(MessageSource source, const char* message, Args... args) { ProcessMessage(source, MessageType::ERROR, message, args...); }
    private:
        template <typename... Args>
        static void ProcessMessage(MessageSource source, MessageType type, const char* message, Args... args)
        {
            Message msg;

            auto secs = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
            std::string formatted = FormatMessage(message, args...);
            msg.Message = formatted.c_str();
            msg.Source = source;
            msg.Time = (uint32_t)secs.count();
            msg.Type = type;

            m_MessageHistory.push_back(msg);
            if (m_MessageHistory.size() > MAX_MESSAGES)
            {
                m_MessageHistory.pop_back();
            }
        }

        template <typename... Args>
        static std::string FormatMessage(std::string_view message, Args&&... args)
        {
            return std::vformat(message, std::make_format_args(args...));
        }
    private:
        static std::vector<Message> m_MessageHistory;
        static std::shared_ptr<LogRenderer> m_LogRenderer;

        static std::shared_ptr<Renderer> m_Renderer;
    };
}

#define SP_CORE_TRACE(...)  ::Space::Log::Trace(MessageSource::SERVER, __VA_ARGS__)
#define SP_CORE_INFO(...)   ::Space::Log::Info(MessageSource::SERVER, __VA_ARGS__)
#define SP_CORE_WARN(...)   ::Space::Log::Warn(MessageSource::SERVER, __VA_ARGS__)
#define SP_CORE_ERROR(...)  ::Space::Log::Error(MessageSource::SERVER, __VA_ARGS__)

#define SP_TRACE(...)       ::Space::Log::Trace(MessageSource::CLIENT, __VA_ARGS__)
#define SP_INFO(...)        ::Space::Log::Info(MessageSource::CLIENT, __VA_ARGS__)
#define SP_WARN(...)        ::Space::Log::Warn(MessageSource::CLIENT, __VA_ARGS__)
#define SP_ERROR(...)       ::Space::Log::Error(MessageSource::CLIENT, __VA_ARGS__)
