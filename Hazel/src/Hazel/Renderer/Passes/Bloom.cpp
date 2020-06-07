#include "hzpch.h"
#include "Hazel/Renderer/Scene.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel
{


	void Scene::BloomBlur()
	{
        m_GaussianBlur->Bind();



        for (int i = 0; i < bloomHFBOs.size(); i++)
            {

                RenderCommand::SetViewport(0, 0, bloomDownSampled[i]->GetWidth(), bloomDownSampled[i]->GetHeight());
                bloomDFBOs[i]->Bind();
                ClearScene();
                m_GaussianBlur->SetBool("Enable", false);
                BloomTexture2D->Bind(0);
                m_GaussianBlur->SetInt("BloomImage", *BloomTexture2D->GetSlot());
                m_SquareVA->Bind();
                Renderer::Submit(m_GaussianBlur, m_SquareVA);
                bloomDFBOs[i]->Unbind();

                RenderCommand::SetViewport(0, 0, bloomGaussiansH[i]->GetWidth(), bloomGaussiansH[i]->GetHeight());
                bloomHFBOs[i]->Bind();
                ClearScene();
                m_GaussianBlur->SetBool("bHorizontal", true);
                bloomDownSampled[i]->Bind(i + 1);
                m_GaussianBlur->SetInt("BloomImage", *bloomDownSampled[i]->GetSlot());
                m_SquareVA->Bind();
                Renderer::Submit(m_GaussianBlur, m_SquareVA);
                bloomHFBOs[i]->Unbind();


                RenderCommand::SetViewport(0, 0, bloomGaussiansV[i]->GetWidth(), bloomGaussiansV[i]->GetHeight());
                bloomVFBOs[i]->Bind();
                ClearScene();
                m_GaussianBlur->SetBool("bHorizontal", false);
                bloomGaussiansH[i]->Bind(i+ bloomHFBOs.size()+1);
                m_GaussianBlur->SetInt("BloomImage", *bloomGaussiansH[i]->GetSlot());
                m_SquareVA->Bind();
                Renderer::Submit(m_GaussianBlur, m_SquareVA);
                bloomVFBOs[i]->Unbind();
            }  
	}

}

