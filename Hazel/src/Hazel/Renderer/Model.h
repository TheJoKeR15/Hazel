#pragma once

#include <vector>

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Vertex.h"
#include "Hazel/Renderer/Mesh.h"
#include "Hazel/Renderer/Entity.h"



#include "assimp/Importer.hpp"			// C++ importer interface
#include "assimp/scene.h"          // Output data structure
#include "assimp/postprocess.h"    // Post processing flags

namespace Hazel {
    class Model : public Hazel::Entity
    {
    public:
        Model(char* path) 
            : m_path(path)
        {
            loadModel(path);
            transform = glm::mat4(1.f);
        }
        void Draw(Hazel::Ref<Hazel::Shader> shader);

        void Draw(Hazel::Ref<Hazel::Shader> shader,glm::mat4 Transform);

        char* GetPath() { return m_path; };

        // Should this model be rendered ?
        



    private:
        // model data
        std::vector<Mesh> meshes;
        

        std::string directory;


        char* m_path;
        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        //std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,std::string typeName);
    };
}


