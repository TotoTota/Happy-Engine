#include "EditorLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Scene/SceneSerializer.h"

namespace Hazel {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_HappyLogo = Texture2D::Create("assets/textures/Hazel2.png");
		m_CloseLogo = Texture2D::Create("assets/textures/close2.png");
		m_MaximizeLogo = Texture2D::Create("assets/textures/maximize2.png");
		m_MinimizeLogo = Texture2D::Create("assets/textures/minimize.png");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

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

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(ts);

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

			float cursorPosX = ImGui::GetCursorPosX();
			float cursorPosY = ImGui::GetCursorPosY();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			ImGui::Image((ImTextureID)m_HappyLogo->GetRendererID(), ImVec2{ 40.0f, 40.0f }, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SetCursorPosY(cursorPosY);

			float remainingWidth = titlebarWidth - 215.0f;
			float textPosX = remainingWidth / 2.0f - ImGui::CalcTextSize("Hazelnut").x / 2.0f + 110;

			if (ImGui::BeginMenu("File"))
			{

				if (ImGui::MenuItem("Serialize"))
				{
					SceneSerializer serializer(m_ActiveScene);
					serializer.Serialize("assets/scenes/Example.hazel");
				}

				if (ImGui::MenuItem("Deserialize"))
				{
					SceneSerializer serializer(m_ActiveScene);
					serializer.Deserialize("assets/scenes/Example.hazel");
				}

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::InvisibleButton("##titleBarDragZone", ImVec2(titlebarWidth - 215.0f, titlebarHeight));
			m_TitlebarHovered = ImGui::IsItemHovered();

			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];
			ImGui::SameLine(textPosX);
			ImGui::PushFont(boldFont);
			ImGui::Text("Hazelnut");
			ImGui::PopFont();

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

		ImGui::Begin("Stats");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

}
