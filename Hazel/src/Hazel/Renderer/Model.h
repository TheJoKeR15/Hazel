#pragma once

#include <vector>

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Vertex.h"
#include "Hazel/Renderer/Mesh.h"
#include "Hazel/Renderer/Entity.h"
#include "Material.h"



#include "assimp/Importer.hpp"	    // C++ importer interface
#include "assimp/scene.h"           // Output data structure
#include "assimp/postprocess.h"     // Post processing flags

namespace Hazel {
    class Model : public Hazel::Entity
    {
    public:

        Model(char* path, std::string Inname, Hazel::Ref<Hazel::Material> Material,Hazel::Ref<Hazel::Shader> shader)
            : m_path(path), m_Material(Material), m_Shader(shader),Entity(Inname)
        {
            loadModel(path);
            //transform = glm::mat4(1.f);
            //Shader = m_Material->GetShader();
        }

        virtual void OnInit() override;

        virtual void OnBeginFrame() override;

        virtual void OnUpdate(float dt) override;

        virtual void OnEndFrame() override;

        void Draw();

        void Draw(Hazel::Ref<Hazel::Shader> shader);

        void Draw(Hazel::Ref<Hazel::Shader> shader,glm::mat4 Transform);

        char* GetPath() { return m_path; };

        
        // @TODO : REPLACE this with an ECS or component system later
        // Because here an entity can only have one material
        // The material of the entity 
        Hazel::Ref<Hazel::Material> m_Material;

        // @TODO : REPLACE this with an ECS or component system later
        // Because here an entity can only have one material
        // The material of the entity 
        Hazel::Ref<Hazel::Shader> m_Shader;

        std::vector<Mesh> &Getmeshes() { return meshes; };


    private:
        // model data
        std::vector<Mesh> meshes;
        std::vector<Ref<Material>> Materials;
        std::vector<Ref<Texture2D>> textures_loaded;
       // std::vector<Ref<Texture2D>> textures;
        std::string directory;
        char* m_path;
        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        Ref<Texture2D> loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName);
        //std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,std::string typeName);
    };
}


