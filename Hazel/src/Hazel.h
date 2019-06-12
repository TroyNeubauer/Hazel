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

// ---Entry Point---------------------
#include "Hazel/EntryPoint.h"
// -----------------------------------