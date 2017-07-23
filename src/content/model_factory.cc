#include "model_factory.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla
#include <assimp/version.h>

#include "core/scene/entity_factory.h"
#include "util/assert.h"
#include "util/quaternion.h"
#include "core/get.h"
#include "core/debug/console.h"
#include "renderer/texture_manager.h"
#include "renderer/material_manager.h"
#include "content/image_manager.h"

#include "core/debug/performance_profiler.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    SharedPtr<Entity> ModelFactory::LoadModel(const String& file_path_to_model)
    {
        char buf[512];
        sprintf(buf, "ModelFactoryLoad: %s", file_path_to_model.c_str());

        PerformanceProfiler::ProfilerBlock block(buf, ProfilerBlockType_CONTENT);
        SharedPtr<Entity> root_entity = EntityFactory::CreateEntity("model_root");

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(file_path_to_model.c_str(),
            aiProcess_GenNormals |
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_FlipUVs
        );
        
        String model_directory_path = file_path_to_model;

        while (model_directory_path[model_directory_path.size() - 1] != '/' && model_directory_path[model_directory_path.size() - 1] != '\\')
        {
            model_directory_path.pop_back();
        }

        if (scene == nullptr)
        {
            return root_entity;
        }

        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
        const aiColor4D ZeroColor(0.0f, 0.0f, 0.0f, 1.0f);

        Vector<SharedPtr<Mesh>> meshes;
        Vector<int> material_indices;
        ProcessMeshes(scene->mMeshes, scene->mNumMeshes, &meshes, &material_indices);

        Vector<WeakPtr<Material>> materials;
        ProcessMaterials(scene->mMaterials, scene->mNumMaterials, model_directory_path, &materials);

        ProcessNode(scene->mRootNode, root_entity, meshes, material_indices, materials);

        int num_indices = 0, num_vertices = 0;
        for (int i = 0; i < meshes.size(); i++)
        {
            num_indices += meshes[i]->GetMeshData().GetIndices().size();
            num_vertices += meshes[i]->GetMeshData().GetVertices().size();
        }

        sprintf(buf, "A model (%s) has been loaded.\nMeshes: %i\nVertices: %i\nIndices: %i", file_path_to_model.c_str(), meshes.size(), num_vertices, num_indices);
        Get::Console()->LogStatus(buf);

        return root_entity;
    }
    
    //------------------------------------------------------------------------------------------------------
    void ModelFactory::ProcessMeshes(aiMesh** meshes, unsigned int num_meshes, Vector<SharedPtr<Mesh>>* out_meshes, Vector<int>* out_material_indices)
    {
        for (unsigned int i = 0; i < num_meshes; i++)
        {
            Vector<Vertex> vertices;
            Vector<Index> indices;

            ProcessVertices(meshes[i], &vertices);
            ProcessIndices(meshes[i], &indices);

            Index max = 0;
            max = ~max;
            if (vertices.size() > max)
            {
                Get::Console()->LogWarning(String("A mesh with more than ") + eastl::to_string(max) + " vertices is being loaded while Blowbox is configured to use " + eastl::to_string(sizeof(Index) * 8) + " bit indices which don't support any more vertices than that. This will result in artifacts when rendering the model.");
            }

            SharedPtr<Mesh> mesh = eastl::make_shared<Mesh>();
            mesh->Create(MeshData(meshes[i]->mName.data, vertices, indices, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

            (*out_meshes).push_back(mesh);
            (*out_material_indices).push_back(meshes[i]->mMaterialIndex);
        }
    }

    //------------------------------------------------------------------------------------------------------
    void ModelFactory::ProcessVertices(aiMesh* mesh, Vector<Vertex>* out_vertices)
    {
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vert;

            vert.position = DirectX::XMFLOAT3(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            );

            if (mesh->HasNormals())
            {
                vert.normal = DirectX::XMFLOAT3(
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
                );
            }

            if (mesh->HasTangentsAndBitangents())
            {
                vert.tangent = DirectX::XMFLOAT3(
                    mesh->mTangents[i].x,
                    mesh->mTangents[i].y,
                    mesh->mTangents[i].z
                );
            }

            if (mesh->HasTextureCoords(0))
            {
                vert.uv = DirectX::XMFLOAT2(
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                );
            }

            if (mesh->HasVertexColors(0))
            {
                vert.color = DirectX::XMFLOAT4(
                    mesh->mColors[0][i].r,
                    mesh->mColors[0][i].g,
                    mesh->mColors[0][i].b,
                    mesh->mColors[0][i].a
                );
            }

            (*out_vertices).push_back(vert);
        }
    }

    //------------------------------------------------------------------------------------------------------
    void ModelFactory::ProcessIndices(aiMesh* mesh, Vector<Index>* out_indices)
    {
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace& face = mesh->mFaces[i];

            BLOWBOX_ASSERT(face.mNumIndices == 3);

            for (int k = 0; k < face.mNumIndices; k++)
            {
                (*out_indices).push_back(static_cast<Index>(face.mIndices[k]));
            }
        }
    }

    String ConvertTexToString(int i)
    {
        switch (static_cast<aiTextureType>(i))
        {
        case aiTextureType_AMBIENT: return "Ambient";
        case aiTextureType_DIFFUSE: return "Diffuse";
        case aiTextureType_DISPLACEMENT: return "Displacement";
        case aiTextureType_EMISSIVE: return "Emissive";
        case aiTextureType_HEIGHT: return "Height";
        case aiTextureType_LIGHTMAP: return "Lightmap";
        case aiTextureType_NORMALS: return "Normals";
        case aiTextureType_OPACITY: return "Opacity";
        case aiTextureType_REFLECTION: return "Reflection";
        case aiTextureType_SHININESS: return "Shininess";
        case aiTextureType_SPECULAR: return "Specular";
        }

        return "unknown";
    }

    bool IsSupportedTextureType(int i)
    {
        switch (static_cast<aiTextureType>(i))
        {
        case aiTextureType_AMBIENT: return true;
        case aiTextureType_DIFFUSE: return true;
        case aiTextureType_EMISSIVE: return true;
        case aiTextureType_HEIGHT: return true;
        case aiTextureType_NORMALS: return true;
        case aiTextureType_OPACITY: return true;
        case aiTextureType_SHININESS: return true;
        case aiTextureType_SPECULAR: return true;
        }

        return false;
    }

    //------------------------------------------------------------------------------------------------------
    void ModelFactory::ProcessMaterials(aiMaterial** materials, unsigned int num_materials, const String& model_directory_path, Vector<WeakPtr<Material>>* out_materials)
    {
        char buf[512];

        for (unsigned int i = 0; i < num_materials; i++)
        {
            aiMaterial* current = materials[i];
            aiString material_name;
            aiColor3D color_diffuse, color_specular, color_ambient, color_emissive;
            float opacity = 1.0f, specular_scale = 1.5f, specular_power = 1.0f, bump_intensity = 5.0f;

            current->Get(AI_MATKEY_NAME, material_name);
            current->Get(AI_MATKEY_COLOR_DIFFUSE, color_diffuse);
            current->Get(AI_MATKEY_COLOR_SPECULAR, color_specular);
            current->Get(AI_MATKEY_COLOR_AMBIENT, color_ambient);
            current->Get(AI_MATKEY_COLOR_EMISSIVE, color_emissive);
            current->Get(AI_MATKEY_OPACITY, opacity);
            current->Get(AI_MATKEY_SHININESS, specular_scale);
            current->Get(AI_MATKEY_SHININESS_STRENGTH, specular_power);
            current->Get(AI_MATKEY_BUMPSCALING, bump_intensity);

            SharedPtr<Material> processed_material = eastl::make_shared<Material>();
            processed_material->SetName(material_name.data);
            processed_material->SetColorDiffuse(DirectX::XMFLOAT3(color_diffuse.r, color_diffuse.g, color_diffuse.b));
            processed_material->SetColorSpecular(DirectX::XMFLOAT3(color_specular.r, color_specular.g, color_specular.b));
            processed_material->SetColorAmbient(DirectX::XMFLOAT3(color_ambient.r, color_ambient.g, color_ambient.b));
            processed_material->SetColorEmissive(DirectX::XMFLOAT3(color_emissive.r, color_emissive.g, color_emissive.b));
            processed_material->SetOpacity(opacity);
            processed_material->SetSpecularScale(specular_scale);
            processed_material->SetSpecularPower(specular_power);
            processed_material->SetBumpIntensity(bump_intensity);

            for (int i = 0; i < aiTextureType_UNKNOWN; i++)
            {
                if (IsSupportedTextureType(i))
                {
                    int texture_count = current->GetTextureCount(static_cast<aiTextureType>(i));

                    if (texture_count > 0)
                    {
                        if (texture_count > 1)
                        {
                            sprintf(buf, "A material that is being loaded (%s) specifies more than one textures for one texture type (%s). This is unsupported, instead Blowbox will use only the first texture.", material_name.data, ConvertTexToString(i));
                            Get::Console()->LogWarning(buf);
                        }

                        aiString path;
                        current->GetTexture(static_cast<aiTextureType>(i), 0, &path);

                        String full_path = model_directory_path + path.data;

                        WeakPtr<Image> image;
                        image = Get::ImageManager()->GetImage(full_path);

                        SharedPtr<Texture> texture(nullptr);

                        if (!Get::TextureManager()->HasBeenLoaded(full_path))
                        {
                            texture = eastl::make_shared<Texture>(image);
                            Get::TextureManager()->AddTexture(full_path, texture);
                        }
                        else
                        {
                            texture = Get::TextureManager()->GetTexture(full_path).lock();
                        }

                        switch (static_cast<aiTextureType>(i))
                        {
                        case aiTextureType_AMBIENT:
                            processed_material->SetTextureAmbient(texture);
                            break;
                        case aiTextureType_DIFFUSE:
                            processed_material->SetTextureDiffuse(texture);
                            break;
                        case aiTextureType_EMISSIVE:
                            processed_material->SetTextureEmissive(texture);
                            break;
                        case aiTextureType_HEIGHT:
                            processed_material->SetTextureBump(texture);
                            break;
                        case aiTextureType_NORMALS:
                            processed_material->SetTextureNormal(texture);
                            break;
                        case aiTextureType_OPACITY:
                            processed_material->SetTextureOpacity(texture);
                            break;
                        case aiTextureType_SHININESS:
                            processed_material->SetTextureSpecularPower(texture);
                            break;
                        case aiTextureType_SPECULAR:
                            processed_material->SetTextureSpecular(texture);
                            break;
                        }
                    }
                }
            }
            
            Get::MaterialManager()->AddMaterial(material_name.data, processed_material);
            out_materials->push_back(processed_material);
        }
    }
    
    //------------------------------------------------------------------------------------------------------
    Vector<SharedPtr<Entity>> ModelFactory::ProcessNode(aiNode* node, SharedPtr<Entity> parent, const Vector<SharedPtr<Mesh>>& available_meshes, const Vector<int>& material_indices, const Vector<WeakPtr<Material>>& available_materials)
    {
        DirectX::XMMATRIX node_local_transform;
        node_local_transform.r[0] = DirectX::XMVectorSet(node->mTransformation.a1, node->mTransformation.b1, node->mTransformation.c1, node->mTransformation.d1);
        node_local_transform.r[1] = DirectX::XMVectorSet(node->mTransformation.a2, node->mTransformation.b2, node->mTransformation.c2, node->mTransformation.d2);
        node_local_transform.r[2] = DirectX::XMVectorSet(node->mTransformation.a3, node->mTransformation.b3, node->mTransformation.c3, node->mTransformation.d3);
        node_local_transform.r[3] = DirectX::XMVectorSet(node->mTransformation.a4, node->mTransformation.b4, node->mTransformation.c4, node->mTransformation.d4);

        DirectX::XMVECTOR scaling_vec, quaternion_vec, translation_vec;
        BLOWBOX_ASSERT(DirectX::XMMatrixDecompose(&scaling_vec, &quaternion_vec, &translation_vec, node_local_transform) == true);

        DirectX::XMFLOAT3 scaling, rotation, translation;
        DirectX::XMStoreFloat3(&scaling, scaling_vec);
        DirectX::XMStoreFloat3(&translation, translation_vec);
        rotation = DirectX::XMQuaternionToEuler(quaternion_vec);

        Vector<SharedPtr<Entity>> entities;
        
        if (node->mNumMeshes >= 1)
        {
            for (int i = 0; i < node->mNumMeshes; i++)
            {
                SharedPtr<Entity> new_entity = EntityFactory::CreateEntity(node->mName.C_Str());
                new_entity->SetLocalPosition(translation);
                new_entity->SetLocalRotation(rotation);
                new_entity->SetLocalScaling(scaling);
                new_entity->SetMesh(available_meshes[node->mMeshes[i]]);
                new_entity->SetMaterial(available_materials[material_indices[node->mMeshes[i]]]);

                EntityFactory::AddChildToEntity(parent, new_entity);

                entities.push_back(new_entity);
            }
        }
        else
        {
            SharedPtr<Entity> new_entity = EntityFactory::CreateEntity(node->mName.C_Str());
            new_entity->SetLocalPosition(translation);
            new_entity->SetLocalRotation(rotation);
            new_entity->SetLocalScaling(scaling);
            new_entity->SetMesh(nullptr);

            EntityFactory::AddChildToEntity(parent, new_entity);

            entities.push_back(new_entity);
        }

        for (int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], entities[0], available_meshes, material_indices, available_materials);
        }

        return entities;
    }
}