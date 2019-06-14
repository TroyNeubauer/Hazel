#pragma once

namespace Hazel {
	typedef const char* GraphicsAPIType;

	class GraphicsAPI {
	public:
		static const GraphicsAPIType NONE, NOT_CHOSEN, OPEN_GL, VULKAN, DIRECTX_12, METAL, SOFTWARE;
		
	public:
		static bool IsAvilable(GraphicsAPIType type);

		static void AddListener(void(*listener)(GraphicsAPIType, GraphicsAPIType));

		static void RemoveListener(void(*listener)(GraphicsAPIType, GraphicsAPIType));

		//Picks a graphics API to use if one wasn't already selected and then returns the current API
		static GraphicsAPIType Select();
		static void AddWantedAPI(GraphicsAPIType type);

		static inline GraphicsAPIType Get() { return s_API; }
		static bool Set(GraphicsAPIType type);

		static const char* ToString(GraphicsAPIType);
	private:
		static GraphicsAPIType s_API;
		static std::vector<void(*)(GraphicsAPIType, GraphicsAPIType)> s_APISwitchListeners;
		static std::vector<GraphicsAPIType> s_APIPriority;
	};



}


