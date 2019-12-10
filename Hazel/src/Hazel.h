#pragma once
// For use by Hazel applications

//Override fopen and fclose
#include "Platform/System/FileTrackerOverride.h"

#include "Hazel/Camera/Camera.h"
#include "Hazel/Camera/CameraController.h"
#include "Platform/System/AllocTracker.h"

#include "Hazel/Layer/Layer.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Layer/DebugLayer.h"

#include "Hazel/Util/Utils.h"

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/GraphicsAPI.h"
#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/VertexArray.h"

#include "Hazel/Enviorment/Light.h"

#include "Hazel/Mesh/Mesh.h"
#include "Hazel/Mesh/IcoashedronMesh.h"

#include "Hazel/Core/Application.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Engine.h"
#include "Hazel/Core/glm.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Core/MouseButtonCodes.h"
#include "Hazel/Core/TUtil.h"
#include "Hazel/Core/TUtil.h"
#include "Hazel/Renderer2D/Renderer2D.h"

#include <FreeImage.h>

//std
#include <memory>
#include <vector>
#include <map>
#include <utility>


