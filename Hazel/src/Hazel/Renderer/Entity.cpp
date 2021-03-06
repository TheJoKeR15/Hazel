#include "hzpch.h"
#include "Entity.h"

namespace Hazel {
	Entity::Entity() : UniqueID(0)
	{
	}
	Entity::Entity(uint32_t ID) : displayName("UNAMED ENTITY") , UniqueID(ID)
	{
		OnConstruct();
	}
	Entity::Entity(std::string name , uint32_t ID) : displayName(name), UniqueID(ID)
	{
	}
	void Entity::OnConstruct()
	{
	}
	void Entity::OnInit()
	{

	}

	void Entity::OnBeginFrame()
	{
	}

	void Entity::OnUpdate(float dt)
	{

	}

	void Entity::DrawShawdowPass(Ref<Shader> ShadowPassShader)
	{

	}

	void Entity::DrawMainPass(Ref<Shader> MainPassShader)
	{
	}



	void Entity::OnEndFrame()
	{
	}

}