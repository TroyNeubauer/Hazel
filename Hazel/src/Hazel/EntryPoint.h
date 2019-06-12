#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	Hazel::Log::Init();

	auto app = Hazel::CreateApplication(argc, argv);
	app->Run();
	delete app;
	Hazel::Log::DisableLogging();
}

#endif
