#include "hzpch.h"
#include "TestPanel.h"

#include "imgui/imgui.h"

namespace Hazel {

	TestPanel::TestPanel()
	{
		m_Icon = Texture2D::Create("Resources/DirectoryIcon.png");
	}

	void TestPanel::OnImGuiRender()
	{
		ImGui::Begin("Test Panel for content browser");

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		const ImVec2 p = ImGui::GetCursorScreenPos();
		float x = p.x + 4.0f;
		float y = p.y + 4.0f;
		static ImVec4 colf = ImVec4(0.13f, 0.13f, 0.13f, 1.0f);
		const ImU32 col = ImColor(colf);
		static ImVec4 outlineColfHovered = ImVec4(0.90f, 0.93f, 0.0f, 0.94f);
		static ImVec4 outlineColf = ImVec4(0.20f, 0.23f, 0.0f, 0.94f);
		const ImU32 outlineCol = ImColor(outlineColf);
		draw_list->AddRectFilled({ x, y }, { x + 150, y + 170 }, col);
		draw_list->AddImage((ImTextureID)m_Icon->GetRendererID(), { x, y }, { x + 150, y + 150 }, { 0, 1 }, { 1, 0 });
		draw_list->AddRectFilled({ x, y + 170 }, { x + 150, y + 300 }, ImU32(ImColor(ImVec4(0.2f, 0.2f, 0.2f, 1.0f))));
		draw_list->AddRect({ x, y }, { x + 150, y + 300 }, outlineCol);
		float cursorPosX = ImGui::GetCursorPosX();
		float cursorPosY = ImGui::GetCursorPosY();
		ImGui::SetCursorPosY(cursorPosY + 4.0f);
		ImGui::SetCursorPosX(cursorPosX + 4.0f);
		ImGui::InvisibleButton("##Button", { 150, 300 });
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsItemActive())
				outlineColf = ImVec4(0.20f, 0.23f, 0.0f, 0.94f);
			else
				outlineColf = outlineColfHovered;
		}
		else
			outlineColf = ImVec4(0.20f, 0.23f, 0.0f, 0.94f);

		ImGui::SetCursorPosY(cursorPosY + 180);
		ImGui::SetCursorPosX(cursorPosX + 10);
		ImGui::Text("Folder Name");
		ImGui::SetCursorPosY(cursorPosY + 280);
		ImGui::SetCursorPosX(cursorPosX + 90);
		ImGui::Text("A Folder");

		ImGui::End();
	}

}