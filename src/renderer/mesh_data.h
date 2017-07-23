#pragma once

#include "util/vector.h"
#include "util/string.h"
#include "renderer/vertex.h"

namespace blowbox
{
    /**
    * This class should be used to create data for meshes. Every mesh is a
    * collection of vertices and indices with a given topology. You have to
    * specify those yourself. This class is used by the Mesh in order to
    * construct the necessary rendering buffers needed for rendering.
    *
    * @brief Describes all the data for a Mesh.
    */
    class MeshData
    {
    public:
        MeshData();

        /**
        * @brief Constructs a MeshData with a given set of vertices, indices and topology.
        * @param[in] name The name for this MeshData.
        * @param[in] vertices The vertices for this MeshData.
        * @param[in] indices The indices for this MeshData.
        * @param[in] topology The topology of the MeshData.
        */
        MeshData(const String& name, const Vector<Vertex>& vertices, const Vector<Index>& indices, D3D_PRIMITIVE_TOPOLOGY topology);
        ~MeshData();

        /** 
        * @brief Sets the name of this MeshData.
        * @param[in] name The name for this MeshData.
        */
        void SetName(const String& name);

        /**
        * @brief Sets the vertices for this MeshData.
        * @param[in] vertices The array of vertices.
        */
        void SetVertices(const Vector<Vertex>& vertices);

        /**
        * @brief Sets the indices for this MeshData.
        * @param[in] indices The array of indices.
        */
        void SetIndices(const Vector<Index>& indices);

        /**
        * @brief Sets the topology for this MeshData.
        * @param[in] topology The topology that should be set.
        */
        void SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);

        /** @returns The underlying vertices array. */
        Vector<Vertex>& GetVertices();
        /** @returns The underlying vertices array. */
        const Vector<Vertex>& GetVertices() const;
        
        /** @returns The underlying indices array. */
        Vector<Index>& GetIndices();
        /** @returns The underlying indices array. */
        const Vector<Index>& GetIndices() const;

        /** @returns The topology of this MeshData. */
        D3D_PRIMITIVE_TOPOLOGY GetTopology();
        /** @returns The topology of this MeshData. */
        D3D_PRIMITIVE_TOPOLOGY GetTopology() const;

        /** @returns The name of this MeshData. */
        const String& GetName() const;

    private:
        String name_;                       //!< The name of this MeshData.
        Vector<Vertex> vertices_;           //!< The vertices of this MeshData.
        Vector<Index> indices_;             //!< The indices of this MeshData.
        D3D_PRIMITIVE_TOPOLOGY topology_;   //!< The topology of this MeshData.
    };
}