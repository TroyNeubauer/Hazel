
#include "Platform/System/FileTrackerOverride.h"
#include "Platform/System/AllocTracker.h"

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Hazel/Core/Core.h"
#include "Hazel/Core/glm.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Core/TUtil.h"
#include "Hazel/Debug/Instrumentor.h"

#ifdef HZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
