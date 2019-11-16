
#include "Platform/System/FileTrackerOverride.h"
#include "Platform/System/AllocTracker.h"

#include "Hazel/glm.h"
#include "Hazel/TUtil.h"

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

#include "Hazel/Log.h"

#ifdef HZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
