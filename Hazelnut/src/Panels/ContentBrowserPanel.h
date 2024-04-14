#pragma once

#include <filesystem>

#include "Hazel/Renderer/Texture.h"

namespace Hazel {

	class ContentBrowserPanel 
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		void FileButton(Ref<Texture2D>& icon, const char* name, const char* type);
	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_PNGIcon;
		Ref<Texture2D> m_HappySceneIcon;

		const char* m_FileType = "Unknown";
	};

}
