#pragma once

#include "entt.hpp"
#include "Sas\Core\Timestep.h"

namespace Sas {
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void OnUpdate(Timestep ts);
		void OnViewPortResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewPortWidth = 0, m_ViewPortHeight = 0;
		friend class Entity;
		friend class SceneHierarchyPanel;
	};


}