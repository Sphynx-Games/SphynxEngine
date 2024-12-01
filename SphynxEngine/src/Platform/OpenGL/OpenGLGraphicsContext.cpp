#include "spxpch.h"
#include "OpenGLGraphicsContext.h"

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "Renderer/Window.h"


namespace Sphynx
{
	OpenGLGraphicsContext::OpenGLGraphicsContext(Window* window) :
		GraphicsContext(window),
		m_Context(nullptr)
	{
	}

	OpenGLGraphicsContext::~OpenGLGraphicsContext()
	{
	}

	void OpenGLGraphicsContext::Init()
	{
		SDL_Window* window = reinterpret_cast<SDL_Window*>(m_Window->GetNativeWindow());
		m_Context = SDL_GL_CreateContext(window);
		SPX_CORE_ASSERT(m_Context != nullptr, "Failed to create OpenGL context!");
		
		if(!SDL_GL_MakeCurrent(window, m_Context))
		{
			SPX_CORE_LOG_FATAL("Cannot set OpenGL context!");
		}

		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) 
		{
			SPX_CORE_LOG_FATAL("Cannot initialize glad proc address!");
		}
	}

	void OpenGLGraphicsContext::Shutdown()
	{
		SPX_CORE_ASSERT(m_Context != nullptr, "Context should be valid!");

		if (!SDL_GL_DestroyContext(m_Context))
		{
			SPX_CORE_LOG_FATAL("Cannot destroy OpenGL context!");
		}
		m_Context = nullptr;
	}

	void OpenGLGraphicsContext::OnBeforeWindowCreation()
	{
		SDL_GL_LoadLibrary(nullptr);

		int context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#ifdef SPX_DEBUG
		context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	void OpenGLGraphicsContext::SwapBuffers()
	{
		SDL_Window* window = reinterpret_cast<SDL_Window*>(m_Window->GetNativeWindow());
		SDL_GL_SwapWindow(window);
	}
}
