#include "mesh.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    Mesh::Mesh() :
        initialized_(false)
    {

    }

    //------------------------------------------------------------------------------------------------------
    Mesh::~Mesh()
    {
        Destroy();
    }

    //------------------------------------------------------------------------------------------------------
    void Mesh::Create(const MeshData& mesh_data)
    {
        if (initialized_)
        {
            Destroy();
        }

        initialized_ = true;
        mesh_data_ = mesh_data;

        vertex_buffer_.Create(
            L"VertexBuffer",
            static_cast<UINT>(mesh_data_.GetVertices().size()),
            static_cast<UINT>(sizeof(Vertex)),
            &(mesh_data_.GetVertices()[0])
        );

        index_buffer_.Create(
            L"IndexBuffer",
            static_cast<UINT>(mesh_data_.GetIndices().size()),
            static_cast<UINT>(sizeof(Index)),
            &(mesh_data_.GetIndices()[0])
        );
    }
    
    //------------------------------------------------------------------------------------------------------
    void Mesh::Destroy()
    {
        vertex_buffer_.Destroy();
        index_buffer_.Destroy();
    }
    
    //------------------------------------------------------------------------------------------------------
    const MeshData& Mesh::GetMeshData() const
    {
        return mesh_data_;
    }

    //------------------------------------------------------------------------------------------------------
    const StructuredBuffer& Mesh::GetVertexBuffer() const
    {
        return vertex_buffer_;
    }
    
    //------------------------------------------------------------------------------------------------------
    const ByteAddressBuffer& Mesh::GetIndexBuffer() const
    {
        return index_buffer_;
    }
}