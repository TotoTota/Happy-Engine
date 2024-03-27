#pragma once

#include "glm/glm.hpp"

namespace Hazel {

	struct TransformComponent 
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SptiteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SptiteRendererComponent() = default;
		SptiteRendererComponent(const SptiteRendererComponent&) = default;
		SptiteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

}
