#include "hzpch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Hazel {

	// Once we have projects, change this
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/FileIcon.png");
		m_PNGIcon = Texture2D::Create("Resources/PNGIcon.png");
		m_HappySceneIcon = Texture2D::Create("Resources/HappyScene.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::Button("Back"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
			ImGui::PopStyleColor();
		}

		static float padding = 30.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		int i = 0;

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			float cursorPosX = ImGui::GetCursorPosX();
			float cursorPosY = ImGui::GetCursorPosY();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			const ImVec2 p = ImGui::GetCursorScreenPos();
			float x = p.x + 4.0f;
			float y = p.y + 4.0f;
			static ImVec4 colf = ImVec4(0.13f, 0.13f, 0.13f, 1.0f);
			const ImU32 col = ImColor(colf);
			ImGui::PushID(i++);

			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			std::string filenameString = relativePath.stem().string();

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			if (directoryEntry.is_directory())
				m_FileType = "Folder";
			else if (relativePath.extension() == ".png")
				m_FileType = "PNG Image";
			else if (relativePath.extension() == ".hazel")
				m_FileType = "Happy Scene";
			else if (relativePath.extension() == ".jpg")
				m_FileType = "JPG Image";
			else if (relativePath.extension() == ".bmp")
				m_FileType = "BMP Image";
			else if (relativePath.extension() == ".ttf")
				m_FileType = "True Type Font";
			else if (relativePath.extension() == ".txt")
				m_FileType = "Text";
			else
				m_FileType = "Unknown";

			if (relativePath.extension() == ".png")
				icon = m_PNGIcon;
			else if (relativePath.extension() == ".hazel")
				icon = m_HappySceneIcon;

			//ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			draw_list->AddRectFilled({ x, y }, { x + thumbnailSize + 90, y + 170 }, col);
			draw_list->AddImage((ImTextureID)icon->GetRendererID(), { x, y }, { x + thumbnailSize + 30, y + thumbnailSize + 30 }, { 0, 1 }, { 1, 0 });
			draw_list->AddRectFilled({ x, y + 170 }, { x + thumbnailSize + 90, y + 300 }, ImU32(ImColor(ImVec4(0.2f, 0.2f, 0.2f, 1.0f))));
			ImGui::SetCursorPosY(cursorPosY + 180);
			ImGui::SetCursorPosX(cursorPosX + 10);
			ImGui::TextWrapped(filenameString.c_str());
			ImGui::SetCursorPosY(cursorPosY + 280);
			ImGui::SetCursorPosX((cursorPosX + thumbnailSize + 25) - ImGui::CalcTextSize(m_FileType).x);
			ImGui::Text(m_FileType);
			ImGui::SetCursorPosY(cursorPosY + 4.0f);
			ImGui::SetCursorPosX(cursorPosX + 4.0f);
			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0, 0, 0, 0.2f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0, 0, 0, 0.0f });
			ImGui::Button("##Button", { thumbnailSize + 90, 300 });
			ImGui::PopStyleColor(3);
			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (directoryEntry.is_directory())
						m_CurrentDirectory /= path.filename();
				}
			}

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

/*			ImGui::TextWrapped(filenameString.c_str());*/

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		// ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		// ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar
		ImGui::End();
	}

	void ContentBrowserPanel::FileButton(Ref<Texture2D>& icon, const char* name, const char* type)
	{
		
	}

}