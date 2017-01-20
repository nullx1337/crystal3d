#include "GLContext.h"

namespace Graphics
{
	namespace OpenGL {

		GLContext::GLContext() 
			: m_HDC(0), m_Context(0)
		{
		}

		GLContext::~GLContext()
		{
			wglMakeCurrent(m_HDC, nullptr);
			wglDeleteContext(m_Context);
		}

		bool GLContext::CreateContext(Window::CrWindowHandle a_WindowHandle)
		{
			PIXELFORMATDESCRIPTOR pfd{};
			pfd.nSize = sizeof(pfd);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 24;
			pfd.cDepthBits = 16;
			pfd.iLayerType = PFD_MAIN_PLANE;

			m_HDC = GetDC(static_cast<HWND>(a_WindowHandle));
			int32_t pxFormat = ChoosePixelFormat(m_HDC, &pfd);

			if (!SetPixelFormat(m_HDC, pxFormat, &pfd))
			{
				CrAssert(0, "SetPixelFormat failed");
				return false;
			}

			m_Context = wglCreateContext(m_HDC);
			if (!wglMakeCurrent(m_HDC, m_Context))
			{
				CrAssert(0, "wglMakeCurrent failed!");
				return false;
			}

			return true;
		}

		bool GLContext::SwapBuffer() const
		{
			return SwapBuffers(m_HDC) != 0;
		}
	}
}