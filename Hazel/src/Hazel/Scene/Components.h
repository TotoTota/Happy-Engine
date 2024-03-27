#pragma once

<<<<<<< HEAD
#include <glm/glm.hpp>

#include "Hazel/Renderer/Camera.h"

namespace Hazel {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
=======
#include "glm/glm.hpp"

namespace Hazel {

	struct TransformComponent 
>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

<<<<<<< HEAD
	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		Hazel::Camera Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			: Camera(projection) {}
	};

}
=======
	struct SptiteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SptiteRendererComponent() = default;
		SptiteRendererComponent(const SptiteRendererComponent&) = default;
		SptiteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

}
>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0
