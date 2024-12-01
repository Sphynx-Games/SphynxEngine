#pragma once

#include "Core/Core.h"


namespace Sphynx
{
    class GraphicsContext;
    typedef std::function<void(class Event&)> EventCallbackFunction;

    struct SPHYNX_API WindowParams
    {
        const char* Title;
        uint32_t Width;
        uint32_t Height;
        bool IsFullscreen;
        bool IsVSync;
    };

    class SPHYNX_API Window
    {
    public:
        Window() : m_GraphicsContext(nullptr) {};
        virtual ~Window() = default;

        virtual void Update() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        virtual void SetEventCallbackFunction(EventCallbackFunction function) = 0;

        inline GraphicsContext* GetGraphicsContext() const { return m_GraphicsContext; }

    public:
        static Window* Create(const WindowParams& params = WindowParams());

    protected:
        GraphicsContext* m_GraphicsContext;

    };

}