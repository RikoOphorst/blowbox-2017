#include "model_factory.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla
#include <assimp/version.h>

#include "core/scene/entity_factory.h"
#include "util/assert.h"
#include "util/quaternion.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    SharedPtr<Entity> ModelFactory::LoadModel(const String& file_path_to_model)
    {
        SharedPtr<Entity> root_entity = EntityFactory::CreateEntity("model_root");

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(file_path_to_model.c_str(),
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_JoinIdenticalVertices |
            aiProcess_GenNormals
        );

        if (scene == nullptr)
        {
            return root_entity;
        }

        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
        const aiColor4D ZeroColor(0.0f, 0.0f, 0.0f, 1.0f);

        Vector<SharedPtr<Mesh>> meshes;
        ProcessMeshes(scene->mMeshes, scene->mNumMeshes, &meshes);

        ProcessNode(scene->mRootNode, root_entity, meshes);

        return root_entity;
    }
    
    //------------------------------------------------------------------------------------------------------
    void ModelFactory::ProcessMeshes(aiMesh** meshes, unsigned int num_meshes, Vector<SharedPtr<Mesh>>* out_meshes)
    {
        for (unsigned int i = 0; i < num_meshes; i++)
        {
            Vector<Vertex> vertices;
            Vector<Index> indices;

            ProcessVertices(meshes[i], &vertices);
            ProcessIndices(meshes[i], &indices);

            SharedPtr<Mesh> mesh = eastl::make_shared<Mesh>();
            mesh->Create(MeshData(vertices, indices, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
            (*out_meshes).push_back(mesh);
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

            for (unsigned int k = 0; k < face.mNumIndices; k++)
            {
                (*out_indices).push_back(static_cast<Index>(face.mIndices[k]));
            }
        }
    }
    
    //------------------------------------------------------------------------------------------------------
    Vector<SharedPtr<Entity>> ModelFactory::ProcessNode(aiNode* node, SharedPtr<Entity> parent, const Vector<SharedPtr<Mesh>>& available_meshes)
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
            ProcessNode(node->mChildren[i], entities[0], available_meshes);
        }

        return entities;
    }
}