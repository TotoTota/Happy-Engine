#pragma once

#include "Hazel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Hazel/Renderer/EditorCamera.h"

#include <imgui/imgui.h>

namespace Hazel {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
		bool IsTitlebarHovered() override { return m_TitlebarHovered; }
	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

		void ShowAboutModal();
		void DrawAboutModal();
	private:
		Hazel::OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		Entity m_HoveredEntity;

		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;

		bool m_TitlebarHovered = false;
		bool m_AboutPopup = false;

		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_HappyLogo;
		Ref<Texture2D> m_CloseLogo;
		Ref<Texture2D> m_MaximizeLogo;
		Ref<Texture2D> m_MinimizeLogo;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int m_GizmoType = -1;

		ImVec4 m_GradientColor;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}