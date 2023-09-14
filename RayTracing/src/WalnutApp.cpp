#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include <iostream>

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		// Settings Panel
		ImGui::Begin("Settings");

		if (ImGui::Button("Render")) 
		{
			Render();
		};

		ImGui::End();

		// Viewport Panel
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollbar);

		m_viewportWidth = ImGui::GetContentRegionAvail().x;
		m_viewportHeight = ImGui::GetContentRegionAvail().y;

		if (m_Image)
			ImGui::Image(m_Image->GetDescriptorSet(), ImVec2((float)m_Image->GetWidth(), (float)m_Image->GetWidth()));

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Render()
	{
		if (!m_Image || isViewportResized())
		{
			m_Image = std::make_shared<Image>(m_viewportWidth, m_viewportHeight, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_viewportWidth * m_viewportHeight];
		}

		std::cout << "Width: " << m_viewportWidth << std::endl;
		std::cout << "Height: " << m_viewportHeight << std::endl;

		for (uint32_t i = 0; i < m_viewportWidth * m_viewportHeight; i++)
		{
			m_ImageData[i] = Random::UInt();
			m_ImageData[i] |= 0xff000000;
		}

		m_Image->SetData(m_ImageData);
	}
private:
	std::shared_ptr<Image> m_Image;
	uint32_t* m_ImageData = nullptr;
	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

	bool isViewportResized() {
		return m_viewportWidth != m_Image->GetWidth() || m_viewportHeight != m_Image->GetHeight();
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}