#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Scene/SceneSerializer.h"

#include "Hazel/Utils/PlatformUtils.h"

#include "ImGuizmo.h"

#include "Hazel/Math/Math.h"

namespace Hazel {

	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::ShowAboutModal()
	{
		m_AboutPopup = true;
	}


	void EditorLayer::DrawAboutModal()
	{
		if (!m_AboutPopup)
			return;

		ImGui::OpenPopup("About");
		m_AboutPopup = ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		if (m_AboutPopup)
		{
			auto image = m_HappyLogo;
			ImGui::Image((ImTextureID)m_HappyLogo->GetRendererID(), { 48, 48 }, { 0, 1 }, { 1, 0 });

			ImGui::SameLine();

			ImGui::BeginGroup();
			ImGui::Text("Happy Engine");
			ImGui::Text("by Happiness Studio");
			ImGui::EndGroup();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
			ImGuiStyle& style = ImGui::GetStyle();

			float actualSize = ImGui::CalcTextSize("Close").x + style.FramePadding.x * 2.0f;
			float avail = ImGui::GetContentRegionAvail().x;

			float off = (avail - actualSize) * 0.5f;
			if (off > 0.0f)
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

			if (ImGui::Button("Close"))
			{
				m_AboutPopup = false;
				ImGui::CloseCurrentPopup();
			}

		}

		ImGui::EndPopup();
	}

	void EditorLayer::OnAttach()
	{

		HZ_PROFILE_FUNCTION();

		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		m_GradientColor = ImVec4((static_cast<float>(std::rand()) / RAND_MAX) * 0.75f,
			(static_cast<float>(std::rand()) / RAND_MAX) * 0.75f,
			(static_cast<float>(std::rand()) / RAND_MAX) * 0.75f,
			1.0f);

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_HappyLogo = Texture2D::Create("assets/textures/Hazel2.png");
		m_CloseLogo = Texture2D::Create("assets/textures/close2.png");
		m_MaximizeLogo = Texture2D::Create("assets/textures/maximize2.png");
		m_MinimizeLogo = Texture2D::Create("assets/textures/minimize.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0
		// Entity
		auto square = m_ActiveScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});

		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5.0f;
			}

			void OnDestroy()
			{
				
			}

			void OnUpdate(Timestep ts)
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(KeyCode::A))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(KeyCode::D))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(KeyCode::W))
					translation.y -= speed * ts;
				if (Input::IsKeyPressed(KeyCode::S))
					translation.y += speed * ts;
			}
		};
		
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Update
		if (m_ViewportFocused) 
			m_CameraController.OnUpdate(ts);
		
		m_EditorCamera.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Clear our entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);

		// Update Scene
		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		HZ_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		float framePaddingY = (float)style.FramePadding.y;
		style.FramePadding = ImVec2(4.0f, 15.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();
		style.FramePadding = ImVec2(4.0f, framePaddingY);

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		style.FramePadding = ImVec2(4.0f, 15.0f);
		if (ImGui::BeginMenuBar())
		{
			float titlebarWidth = ImGui::GetContentRegionAvail().x;
			float titlebarHeight = ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().FramePadding.y * 2.0f;

			// std::srand(static_cast<unsigned int>(std::time(nullptr)));

			ImVec2 windowPos = ImGui::GetWindowPos();
			float gradientWidth = ImGui::GetWindowSize().x * 0.18f;
			int numSegments = static_cast<int>(gradientWidth) * 2;
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			for (int i = 0; i < numSegments; ++i) {
				float alpha = 1.0f - (static_cast<float>(i) / numSegments);
				ImVec4 color = ImVec4(m_GradientColor.x, m_GradientColor.y, m_GradientColor.z, alpha * alpha);

				float posX = windowPos.x + (i * gradientWidth / numSegments);
				float posY = windowPos.y;
				float rectWidth = gradientWidth / numSegments;

				drawList->AddRectFilled(ImVec2(posX, posY), ImVec2(posX + rectWidth, posY + ImGui::GetWindowSize().y), ImColor(color));
			}

			float cursorPosX = ImGui::GetCursorPosX();
			float cursorPosY = ImGui::GetCursorPosY();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			ImGui::Image((ImTextureID)m_HappyLogo->GetRendererID(), ImVec2{ 40.0f, 40.0f }, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SetCursorPosY(cursorPosY);

			float remainingWidth = titlebarWidth - 215.0f;
			float textPosX = remainingWidth / 2.0f - ImGui::CalcTextSize("Hazelnut 2023.2 [Release]").x / 2.0f + 110;

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl-N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl-O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl-Shift-S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About"))
				{
					ShowAboutModal();
				}
				ImGui::EndMenu();
			}
			ImGui::SetCursorPosY(cursorPosY);

			ImGui::InvisibleButton("##titleBarDragZone", ImVec2(titlebarWidth - 250.0f, titlebarHeight));
			m_TitlebarHovered = ImGui::IsItemHovered();

			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];
			ImGui::SameLine(textPosX);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);
			ImGui::PushFont(boldFont);
			ImGui::Text("Hazelnut 2023.2 [Release]");
			ImGui::PopFont();
			ImGui::SetCursorPosY(cursorPosY);

			float buttonOffsetX = remainingWidth + cursorPosX + 95;
			float buttonOffsetY = cursorPosY + 5;

			ImGui::SameLine();
			ImGui::SetCursorPos(ImVec2(buttonOffsetX, buttonOffsetY));
			if (ImGui::InvisibleButton("##minimizeButton", { 30.0f, 30.0f })) Application::Get().Minimize();
			ImVec2 buttonPos3 = ImGui::GetItemRectMin();
			ImGui::GetWindowDrawList()->AddImage((ImTextureID)m_MinimizeLogo->GetRendererID(), ImVec2(buttonPos3.x, buttonPos3.y), ImVec2(buttonPos3.x + 40.0f, buttonPos3.y + 40.0f));

			if (ImGui::InvisibleButton("##maximizeButton", { 30.0f, 30.0f })) Application::Get().Maximize();
			ImVec2 buttonPos2 = ImGui::GetItemRectMin();
			ImGui::GetWindowDrawList()->AddImage((ImTextureID)m_MaximizeLogo->GetRendererID(), ImVec2(buttonPos2.x, buttonPos2.y), ImVec2(buttonPos2.x + 40.0f, buttonPos2.y + 40.0f), ImVec2(0, 1), ImVec2(1, 0));

			if (ImGui::InvisibleButton("##closeButton", { 30.0f, 30.0f })) Application::Get().Close();
			ImVec2 buttonPos = ImGui::GetItemRectMin();
			ImGui::GetWindowDrawList()->AddImage((ImTextureID)m_CloseLogo->GetRendererID(), ImVec2(buttonPos.x, buttonPos.y), ImVec2(buttonPos.x + 40.0f, buttonPos.y + 40.0f));

			ImGui::EndMenuBar();
		}
		style.FramePadding = ImVec2(4.0f, framePaddingY);
		
		// ImGui::ShowDemoWindow();

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		if (m_AboutPopup)
			DrawAboutModal();

		ImGui::Begin("Stats");

		std::string name = "None";
		if (m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_AssetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Camera
			// Runtime Camera
			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor Camera
			const glm::mat4 cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity Transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4& transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();

				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();

				break;
			}
			case Key::S:
			{
				if (control && shift)
					SaveSceneAs();

				break;
			}

			// Gizmos
			case Key::Q:
				m_GizmoType = -1;
				break;
			case Key::W:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(KeyCode::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
			OpenScene(filepath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize(path.string());
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}

}
