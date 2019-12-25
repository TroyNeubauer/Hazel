#include "hzpch.h"

#ifdef HZ_ENABLE_OPEN_GL
	#define IMGUI_IMPL_OPENGL_LOADER_GLAD
	#include "examples/imgui_impl_opengl3.cpp"
#elif defined(HZ_ENABLE_OPEN_GLES)
	#include "examples/imgui_impl_sdl.cpp"

#endif


#ifdef HZ_ENABLE_VULKAN
	#include "examples/imgui_impl_vulkan.cpp"
#endif


#ifdef HZ_GLFW2_WINDOW
	#include "examples/imgui_impl_glfw2.cpp"

#elif HZ_GLFW3_WINDOW
	#include "examples/imgui_impl_glfw.cpp"
#else

#endif
