#include "hzpch.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "examples/imgui_impl_opengl3.cpp"

#ifdef HZ_ENABLE_VULKAN
#include "examples/imgui_impl_vulkan.cpp"
#endif

#include "examples/imgui_impl_glfw.cpp"
