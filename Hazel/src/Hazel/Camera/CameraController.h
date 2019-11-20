#pragma once



namespace Hazel {
	
	class CameraStorage3D;
	

	class CameraController3D
	{
	public:
		virtual bool Update(CameraStorage3D& camera) = 0;

		virtual ~CameraController3D() {}
	};

	class CameraController2D
	{
	public:
		virtual bool Update(Camera2D& camera) = 0;
	};

	class AdvancedCameraController : public CameraController3D
	{
	public:
		virtual bool Update(CameraStorage3D& camera);
	};

	class FPSCameraController : public CameraController3D
	{
	public:
		virtual bool Update(CameraStorage3D& camera);
	};

}
