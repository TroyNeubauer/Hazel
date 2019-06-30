#pragma once
#ifdef HZ_PLATFORM_UNIX

#include "Hazel/Input.h"

namespace Hazel {

	class UnixInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}


#endif