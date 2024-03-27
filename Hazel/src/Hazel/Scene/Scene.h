#pragma once

#include "entt.hpp"

<<<<<<< HEAD
=======
#include "Components.h"
>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0

#include "Hazel/Core/Timestep.h"

namespace Hazel {

<<<<<<< HEAD
	class Entity;

	class Scene
=======
	class Scene 
>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0
	{
	public:
		Scene();
		~Scene();

<<<<<<< HEAD
		Entity CreateEntity(const std::string& name = std::string());
=======
		entt::entity CreateEntity();

		// Temp
		entt::registry& Reg() { return m_Registry; }
>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
<<<<<<< HEAD

		friend class Entity;
	};

}
=======
	};

}
>>>>>>> ba8db8c0353feb32340e45ebb5c67567a9f04da0
