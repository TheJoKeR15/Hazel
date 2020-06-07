#include "hzpch.h"
#include "Hazel/Renderer/Scene.h"
#include "Hazel/Renderer/Renderer.h"


namespace Hazel
{
	void Scene::ComposeFinalImage()
	{
		RenderCommand::SetViewport(0, 0, (uint32_t)m_ViewPortSizeX, (uint32_t)m_ViewPortSizeY);
		CompositionBuffer->Bind();
		ClearScene();
		m_CompositionShader->Bind();
		ExposedTexture2D->Bind(0);
		bloomGaussiansV[0]->Bind(1);
		bloomGaussiansV[1]->Bind(2);
		bloomGaussiansV[2]->Bind(3);
		bloomGaussiansV[3]->Bind(4);
		//bloomGaussiansV[4]->Bind(5);
		m_CompositionShader->SetInt("ExposedImage", *ExposedTexture2D->GetSlot());
		m_CompositionShader->SetInt("BloomImage1", *bloomGaussiansV[0]->GetSlot());
		m_CompositionShader->SetInt("BloomImage2", *bloomGaussiansV[1]->GetSlot());
		m_CompositionShader->SetInt("BloomImage3", *bloomGaussiansV[2]->GetSlot());
		m_CompositionShader->SetInt("BloomImage4", *bloomGaussiansV[3]->GetSlot());
		//m_CompositionShader->SetInt("BloomImage5", *bloomGaussiansV[4]->GetSlot());
		m_CompositionShader->SetFloat("BloomIntensity", bloom_Intensity);
		//CompositionBuffer->Bind();
		
		m_SquareVA->Bind();

		Renderer::Submit(m_CompositionShader, m_SquareVA);

		CompositionBuffer->Unbind();
	}

}