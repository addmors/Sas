#pragma once

#include "Sas/Core/Core.h"
#include "Sas/Scene/Scene.h"
#include "Sas/Scene/Entity.h"

namespace Sas {
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);
		
		void SetContext(const Ref<Scene>& scene);
		
		void OnImGuiRender();

		Entity GetSelectedEntity() const {return m_SelectionContext;};

		void SetSelectedEntity(Entity entity);
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}
