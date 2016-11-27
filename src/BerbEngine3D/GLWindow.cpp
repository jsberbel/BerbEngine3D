#include "GLWindow.h"
#include "Assert.h"
#include <GL\glew.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

namespace brb {

#define EnableGLHint() \
		/*Indicates the accuracy of fog calculation.If per - pixel fog calculation is not efficiently supported by the GL implementation, 
	hinting GL_DONT_CARE or GL_FASTEST can result in per - vertex calculation of fog effects. */ \
	glHint(GL_FOG_HINT, GL_NICEST); \
	/*Indicates the accuracy of the derivative calculation for the GL shading language fragment processing built - in functions : dFdx, dFdy, and fwidth.*/ \
	glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST); \
	/*Indicates the quality of filtering when generating mipmap images.*/ \
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST); \
	/*Indicates the sampling quality of antialiased lines.If a larger filter function is applied, hinting GL_NICEST can result in more pixel fragments being generated during rasterization.*/ \
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); \
	/*Indicates the quality of color, texture coordinate, and fog coordinate interpolation.If perspective - corrected parameter interpolation is not efficiently supported by the GL implementation,
	hinting GL_DONT_CARE or GL_FASTEST can result in simple linear interpolation of colors and / or texture coordinates.*/ \
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); \
	/*Indicates the sampling quality of antialiased points.If a larger filter function is applied, hinting GL_NICEST can result in more pixel fragments being generated during rasterization.*/ \
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); \
	/*Indicates the sampling quality of antialiased polygons.Hinting GL_NICEST can result in more pixel fragments being generated during rasterization, if a larger filter function is applied.*/ \
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); \
	/*Indicates the quality and performance of the compressing texture images. Hinting GL_FASTEST indicates that texture images should be compressed as quickly as possible,
	while GL_NICEST indicates that texture images should be compressed with as little image quality loss as possible.
	GL_NICEST should be selected if the texture is to be retrieved by glGetCompressedTexImage for reuse.*/ \
	glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST); \

#define EnableStencilTest() glEnable(GL_DEPTH_TEST); \
							glDepthFunc(GL_LESS); \
							glEnable(GL_STENCIL_TEST); \
							glStencilFunc(GL_NOTEQUAL, 1, 0xFF); \
							glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); \

	GLWindow::GLWindow(const std::string &name, Uint32 screenWidth, Uint32 screenHeight, Uint32 curFlags) :
		m_name(name),
		m_screenWidth(screenWidth),
		m_screenHeight(screenHeight) 
	{
		SDL_Init(SDL_INIT_EVERYTHING); // Initialize everything in SDL (VIDEO, AUDIO, EVENTS,...)
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Tell we want a double buffered windows to avoid flickering
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		m_SDLWindow = SDL_CreateWindow(m_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL | curFlags);
		ASSERT(m_SDLWindow != nullptr);
		m_GLContext = SDL_GL_CreateContext(m_SDLWindow);
		ASSERT(m_GLContext != nullptr);
		glewExperimental = true;
		ASSERT (glewInit() == GLEW_OK);
		glClearDepth(1.0); // Set the base depth when depth buffer
		glEnable(GL_DEPTH_TEST); // Activate the Z-buffer
		glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Enable alpha blending
		glEnable(GL_CULL_FACE); glCullFace(GL_BACK); //glFrontFace(GL_CW);
		glLineWidth(2.0f);
		EnableGLHint();
		//EnableStencilTest(); TODO
		glDepthFunc(GL_LESS);
		SDL_GL_SetSwapInterval(0); // Set V-Sync
		printf("***  OpenGL Version: %s  ***\n", reinterpret_cast<char const*>(glGetString(GL_VERSION)));
	}

	GLWindow::~GLWindow() {
		if (m_SDLWindow != nullptr) SDL_DestroyWindow(m_SDLWindow);
		if (m_GLContext != nullptr) SDL_GL_DeleteContext(m_GLContext);
		SDL_Quit();
	}

	void GLWindow::ChangeSize(int screenWidth, int screenHeight) const { 
		SDL_SetWindowSize(m_SDLWindow, screenWidth, screenHeight); 
	};

	void GLWindow::ChangeName(const std::string &name) const { 
		SDL_SetWindowTitle(m_SDLWindow, name.c_str()); 
	};

	void GLWindow::SwapBuffer() const { 
		SDL_GL_SwapWindow(m_SDLWindow); 
	};

}
