#include "hzpch.h"
#include "Scene.h"

Hazel::Scene::Scene()
{
}

void Hazel::Scene::Initialize()
{
}

void Hazel::Scene::RenderScene()
{
}

void Hazel::Scene::BeginScene()
{
}

void Hazel::Scene::EndScene()
{
}

void Hazel::Scene::AddEnitity(Entity* newEntinity)
{
	Entities.push_back(newEntinity);
}
