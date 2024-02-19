#pragma once

#include <cstdint>
#include <functional>

namespace Sphynx
{
    typedef std::function<void(class Event&)> EventCallbackFunction;

    struct WindowParams
    {
        const char* Title;
        uint32_t Width;
        uint32_t Height;
        bool IsFullscreen;
        bool IsVSync;
    };

    class Window
    {
    public:
        virtual ~Window() = default;

        virtual void Update() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        virtual void SetEventCallbackFunction(EventCallbackFunction function) = 0;

    public:
        static Window* Create(const WindowParams& params = WindowParams());

    };

}