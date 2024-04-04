#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Hazel {

	class Hazelnut : public Application
	{
	public:
		Layer* editorLayer = new EditorLayer();

		Hazelnut()
			: Application("Hazelnut", true)
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

	Application* CreateApplication()
	{
		return new Hazelnut();
	}

}