#include "hzpch.h"

#include "Core.h"


#ifdef HZ_PLATFORM_EMSCRIPTEN
void EmscriptenDebugBreak()
{
	EM_ASM({
		alert('Assertion failed!');
	});
}

#endif 
