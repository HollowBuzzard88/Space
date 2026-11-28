#pragma once

#include "Space/Core/Message.h"

#include <chrono>
#include <deque>

#define MAX_MESSAGES 1024

namespace Space
{
    class Renderer;
    class UIRenderer;
    class LogRenderer;

    class Log
    {
    public:
        static void Init(std::shared_ptr<UIRenderer> UIrenderer);
        static void RenderLogger();

        static void OnWindowResize(const glm::vec2& windowSize);

        static std::deque<Message> GetMessageHistory() { return m_MessageHistory; }
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
            msg.Message = std::move(formatted);
            msg.Source = source;
            msg.Time = secs.count();
            msg.Type = type;

            m_MessageHistory.push_back(msg);
            if (m_MessageHistory.size() > MAX_MESSAGES)
            {
                m_MessageHistory.pop_front();
            }
        }

        template <typename... Args>
        static std::string FormatMessage(std::string_view message, Args&&... args)
        {
            return std::vformat(message, std::make_format_args(args...));
        }
    private:
        static std::deque<Message> m_MessageHistory;
        static std::shared_ptr<LogRenderer> m_LogRenderer;
    };
}

#define SP_CORE_TRACE(...)  ::Space::Log::Trace(MessageSource::SERVER, __VA_ARGS__)
#define SP_CORE_INFO(...)   ::Space::Log::Info(MessageSource::SERVER, __VA_ARGS__)
#define SP_CORE_WARN(...)   ::Space::Log::Warn(MessageSource::SERVER, __VA_ARGS__)
#define SP_CORE_ERROR(...)  ::Space::Log::Error(MessageSource::SERVER, __VA_ARGS__)

#define CL_TRACE(...)       ::Space::Log::Trace(MessageSource::CLIENT, __VA_ARGS__)
#define CL_INFO(...)        ::Space::Log::Info(MessageSource::CLIENT, __VA_ARGS__)
#define CL_WARN(...)        ::Space::Log::Warn(MessageSource::CLIENT, __VA_ARGS__)
#define CL_ERROR(...)       ::Space::Log::Error(MessageSource::CLIENT, __VA_ARGS__)
