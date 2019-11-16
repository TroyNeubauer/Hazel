#pragma once



namespace Hazel {
	
	class CameraStorage;
	

	class CameraController
	{
	public:
		virtual bool Update(CameraStorage& camera, bool paused) = 0;

		virtual ~CameraController();
	};

	class AdvancedCameraController : public CameraController
	{
	public:
		virtual bool Update(CameraStorage& camera, bool paused);
	};

	class FPSCameraController : public CameraController
	{
	public:
		virtual bool Update(CameraStorage& camera, bool paused);
	};

}
