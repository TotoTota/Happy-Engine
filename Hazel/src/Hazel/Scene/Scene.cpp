#include "hzpch.h"
#include "Scene.h"

<<<<<<< HEAD
#include "Components.h"
#include "Hazel/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace Hazel {

	static void DoMath(const glm::mat4& transform)
	{

=======
#include <glm/glm.hpp>

#include "Hazel/Renderer/Renderer2D.h"

namespace Hazel {

	static void DoMaths(const glm::mat4& transform)
	{
		
>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0
	}

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{
<<<<<<< HEAD

=======
		
>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0
	}

	Scene::Scene()
	{
<<<<<<< HEAD
#if ENTT_EXAMPLE_CODE
=======
#if 0
>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

<<<<<<< HEAD

		if (m_Registry.has<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);


=======
		if(m_Registry.has<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0
		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
<<<<<<< HEAD
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
=======
			auto[transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0
		}
#endif
	}

	Scene::~Scene()
	{
<<<<<<< HEAD
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
=======

	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0
	}

	void Scene::OnUpdate(Timestep ts)
	{
<<<<<<< HEAD
		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
			
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
=======
		auto group = m_Registry.group<TransformComponent>(entt::get<SptiteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SptiteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0
		}
	}

}