#pragma once

#include "Hazel/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Hazel {
	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void OnWindowResize(int width, int height) override;
		virtual void Destroy();

		virtual GraphicsAPIType GetAPIType();

	private:
		GLFWwindow* m_Handle;
	};
}