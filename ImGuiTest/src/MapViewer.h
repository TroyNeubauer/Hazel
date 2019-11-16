#pragma once
#include <imgui.h>
#include "lodepng.h"

#include <iostream>
#include <math.h>
#include <vector>

class MapViewer
{

public:
	MapViewer(float lat, float lng, float width) : m_Lat(lat), m_Lng(lng), m_Width(width)
	{

	}

	void DrawWindow(const char* title)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 550));

		if (ImGui::Begin(title, &m_Open, ImGuiWindowFlags_NoScrollbar))
		{
			if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGui::IsMouseClicked(1))
				ImGui::OpenPopup("context");
			DrawContents();
		}
		ImGui::End();
	}

	void DrawContents()
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImGui::Separator();
		ImGui::Text("Latitude %.3f, Longitude %.3f, Meters Per Pixel %.1f, zoom: %d", m_Lat, m_Lng, GetMetersPerPixel(ImGui::GetWindowWidth()), GetZoomLevel());

		if (ImGui::BeginPopup("context"))
		{
			ImGui::PushItemWidth(56);
			ImGui::EndPopup();
		}
	}


	inline float GetMetersPerPixel(int displayWidth) { return m_Width / (float) displayWidth; }

	int GetZoomLevel()
	{
		int zoom = 0;
		while (GetWidthForZoom(zoom) > m_Width)
			zoom++;
		return zoom;
	}

	inline float GetWidthForZoom(int zoom)
	{
		return EARTH_CIRC_M / powf(2.0f, (float) zoom);
	}



private:
	const float EARTH_CIRC_M = 40075.01f;//Earth's circumference in meters


	float m_Lat, m_Lng, m_Width;
	bool m_Open = true;

private:
	ImTextureID LoadTexture(int zoom, int x, int y)
	{
		char name[128];
		
		std::vector<unsigned char> image;
		unsigned int width, height;

		unsigned error = ObtainTexture(image, width, height, zoom, x, y);

		//if there's an error, display it
		if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

		//generate an OpenGL texture ID for this texture
		GLuint id;
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		//store the texture data for OpenGL use
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	int ObtainTexture(std::vector<unsigned char>& image, unsigned int& width, unsigned int& height, int zoom, int x, int y)
	{
		std::vector<unsigned char> png;

		char name[128];
		snprintf(name, sizeof(name), "%s/%d/%d/%d.png", "./map", zoom, x, y);
		lodepng::load_file(png, name); //load the image file with given filename
		lodepng::State state; //optionally customize this one

		LodePNGColorMode mode;
		mode.colortype = LodePNGColorType::LCT_RGB;
		mode.bitdepth = 24;

		state.info_raw = mode;

		return lodepng::decode(png, width, height, state, png);
	}
};
