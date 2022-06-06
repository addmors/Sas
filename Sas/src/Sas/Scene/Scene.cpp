#include "ssph.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"
#include "Sas\Renderer\Renderer2D.h"
namespace Sas {
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}
	Entity Scene::CreateEntity(const std::string& name)
	{

		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}
	void Scene::OnUpdate(Timestep ts)
	{
		//Update script 
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{entity, this};
						nsc.Instance->OnCreate();
					}
					nsc.Instance->OnUpdate(ts);
				});
		}
		
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

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
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : group) {
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite.Color);
			}
			Renderer2D::EndScene();
		}
	}
	void Scene::OnViewPortResize(uint32_t width, uint32_t height)
	{
		m_ViewPortWidth = width;
		m_ViewPortHeight = height;

		//Resize our non-FixedAspectRatio
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = m_Registry.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewPortSize(width, height);
			}
		}
	}
}
