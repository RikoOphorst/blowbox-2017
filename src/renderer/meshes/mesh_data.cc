#include "mesh_data.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    MeshData::MeshData()
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    MeshData::MeshData(const String& name, const Vector<Vertex>& vertices, const Vector<Index>& indices, D3D_PRIMITIVE_TOPOLOGY topology) :
        name_(name),
        vertices_(vertices),
        indices_(indices),
        topology_(topology)
    {

    }

    //------------------------------------------------------------------------------------------------------
    MeshData::~MeshData()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void MeshData::SetName(const String& name)
    {
        name_ = name;
    }

    //------------------------------------------------------------------------------------------------------
    void MeshData::SetVertices(const Vector<Vertex>& vertices)
    {
        vertices_ = vertices;
    }

    //------------------------------------------------------------------------------------------------------
    void MeshData::SetIndices(const Vector<Index>& indices)
    {
        indices_ = indices;
    }

    //------------------------------------------------------------------------------------------------------
    void MeshData::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)
    {
        topology_ = topology;
    }

    //------------------------------------------------------------------------------------------------------
    Vector<Vertex>& MeshData::GetVertices()
    {
        return vertices_;
    }

    //------------------------------------------------------------------------------------------------------
    const Vector<Vertex>& MeshData::GetVertices() const
    {
        return vertices_;
    }

    //------------------------------------------------------------------------------------------------------
    Vector<Index>& MeshData::GetIndices()
    {
        return indices_;
    }

    //------------------------------------------------------------------------------------------------------
    const Vector<Index>& MeshData::GetIndices() const
    {
        return indices_;
    }

    //------------------------------------------------------------------------------------------------------
    D3D_PRIMITIVE_TOPOLOGY MeshData::GetTopology()
    {
        return topology_;
    }

    //------------------------------------------------------------------------------------------------------
    D3D_PRIMITIVE_TOPOLOGY MeshData::GetTopology() const
    {
        return topology_;
    }
    
    //------------------------------------------------------------------------------------------------------
    const String& MeshData::GetName() const
    {
        return name_;
    }
}