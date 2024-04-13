#pragma once

#include "Hazel/Renderer/Texture.h"

namespace Hazel {

	class TestPanel 
	{
	public:
		TestPanel();

		void OnImGuiRender();
	private:
		Ref<Texture2D> m_Icon;
	};

}
