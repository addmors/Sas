#pragma once

#include "entt.hpp"
#include "Sas\Core\Timestep.h"
#include "Sas\Renderer\EditorCamera.h"
namespace Sas {
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera camera);
		void OnViewPortResize(uint32_t width, uint32_t height);

		Entity  GetPrimaryCameraEntity();

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		entt::registry m_Registry;
		uint32_t m_ViewPortWidth = 0, m_ViewPortHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};


}