#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public Hazel::Application
{
public:
	Hazel::Layer* sandbox2D = new Sandbox2D();

	Sandbox(Hazel::ApplicationCommandLineArgs args)
		: Hazel::Application("Sandbox", false, args)
	{
		// PushLayer(new ExampleLayer());
		PushLayer(sandbox2D);
	}

	~Sandbox()
	{
	}

	bool IsTitlebarHovered() override
	{
		return sandbox2D->IsTitlebarHovered();
	}
};

Hazel::Application* Hazel::CreateApplication(Hazel::ApplicationCommandLineArgs args)
{
	return new Sandbox(args);
}