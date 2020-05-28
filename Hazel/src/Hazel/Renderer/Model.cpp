#include "hzpch.h"
#include "Model.h"

namespace Hazel {

    void Model::Draw(Hazel::Ref<Hazel::Shader> shader, glm::mat4 Transform)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(shader, Transform);
        }

    }

	void Model::OnInit()
	{

        m_Material->Initialization();
        std::cout << "INITIALIZING MODEL" << std::endl;
	}

	void Model::OnBeginFrame()
	{
       RecalculateTransforms();
	}

	void Model::OnUpdate(float dt)
	{

        if (bVisible)
        {

            Draw();
        }
	}

	void Model::OnEndFrame()
	{
	}

	void Model::Draw()
	{
        

        Draw(m_Shader, transform);
	}

	void Model::Draw(Hazel::Ref<Hazel::Shader> shader)
    {
        //shader->Setfloat("SpecularStrenght" , )
        //RecalculateTransforms();
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader,transform);

    }

    void Model::loadModel(std::string path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            HZ_ERROR("ASSIMP::");
            std::cout << import.GetErrorString() << std::endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);

        //std::cout << "Loaded model with " << meshes.size() << " meshes \n" ;
    }


    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
            
        }

        //std::cout << "Loaded node with " << node->mNumMeshes << " meshes \n";
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
        //std::cout << "Loaded node with " << node->mNumChildren << " childrens \n";
    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            vertices.push_back(vertex);
            /*
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
            
            */
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        if (material->mNumProperties)
        {
             Hazel::Ref<Hazel::Material> Mat = std::make_shared<Hazel::Material>(m_Shader, material->GetName().C_Str());
             
             Mat->m_Albedo = loadMaterialTexture(material, aiTextureType_DIFFUSE, "Diffuse");
             Mat->m_Specular = loadMaterialTexture(material, aiTextureType_SPECULAR, "Specular");
             if (Mat->m_Albedo || Mat->m_Specular)
             {
                 return Mesh(vertices, indices, Mat);
             }
        }

        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN


        /*
        // 1. diffuse maps
        std::vector<Ref<Texture2D>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Ref<Texture2D>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        /*
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        */
        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices,m_Material,mesh->mName.C_Str());
    }

    Ref<Texture2D> Model::loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        Ref<Texture2D> texture;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j]->m_path.data(), str.C_Str()) == 0)
                {
                    texture=textures_loaded[j];
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it

                std::string path = directory +'/'+ str.C_Str()  ;
                texture = Texture2D::Create(path);
                
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return texture;
    }
}