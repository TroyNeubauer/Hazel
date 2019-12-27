#include "hzpch.h"

#ifdef HZ_ENABLE_OPEN_GL
	#define IMGUI_IMPL_OPENGL_LOADER_GLAD
	#include "examples/imgui_impl_opengl3.cpp"
#elif defined(HZ_ENABLE_OPEN_GLES)
	#define IMGUI_IMPL_OPENGL_ES3
	#include "examples/imgui_impl_opengl3.cpp"

#endif


#ifdef HZ_ENABLE_VULKAN
	#include "examples/imgui_impl_vulkan.cpp"
#endif


#ifdef HZ_SDL_WINDOW
	#include "examples/imgui_impl_sdl.cpp"

#elif HZ_GLFW3_WINDOW
	#include "examples/imgui_impl_glfw.cpp"
#else

#endif
