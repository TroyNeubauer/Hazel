#pragma once
// For use by Hazel applications

//Override fopen and fclose
#include "Platform/System/FileTrackerOverride.h"
#include "Platform/System/AllocTracker.h"

#include "Hazel/Application.h"
#include "Hazel/Layer/Layer.h"
#include "Hazel/Log.h"
#include "Hazel/Engine.h"
#include "Hazel/Input.h"
#include "Hazel/KeyCodes.h"
#include "Hazel/MouseButtonCodes.h"

#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Layer/DebugLayer.h"

#include "Hazel/System/File.h"
#include "Hazel/System/Path.h"
#include "Hazel/System/System.h"
#include "Hazel/System/Timer.h"

#include "Hazel/Util/Utils.h"

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/GraphicsAPI.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/VertexArray.h"

// ---Entry Point---------------------
#include "Hazel/EntryPoint.h"
// -----------------------------------