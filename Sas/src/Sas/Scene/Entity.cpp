#include "ssph.h"
#include "Entity.h"

namespace Sas {
	Entity::Entity(entt::entity handle, Scene* scene)
	: m_EntityHandle(handle), m_Scene(scene){}
}