#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Hazel {

	class Hazelnut : public Application
	{
	public:
		Layer* editorLayer = new EditorLayer();

		Hazelnut(ApplicationCommandLineArgs args)
			: Application("Hazelnut", true, args)
		{
			PushLayer(editorLayer);
		}

		~Hazelnut()
		{
		}

		bool IsTitlebarHovered() override 
		{
			return editorLayer->IsTitlebarHovered();
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new Hazelnut(args);
	}

}