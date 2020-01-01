#include "hzpch.h"
#include "ImGuiLayer.h"

#include <imgui.h>

namespace Hazel {
    void ImGuiLayer::OnEvent(Event* event)
    {
        if (event->GetEventType() == EventType::TextTyped)
        {
            ImGui::GetIO().AddInputCharactersUTF8(reinterpret_cast<TextTypedEvent*>(event)->GetText());
        }
    }


}