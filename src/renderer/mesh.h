#pragma once

#include "renderer/mesh_data.h"
#include "renderer/buffers/structured_buffer.h"
#include "renderer/buffers/byte_address_buffer.h"

#include "util/shared_ptr.h"

namespace blowbox
{
    /**
    * This class creates the appropriate buffers for a MeshData.
    * The renderers in Blowbox expect this object. It is usually
    * stored by Entity objects, but you can also keep it seperately,
    * although there isn't much you can do with it by yourself.
    *
    * @brief A mesh that can be rendered.
    */
    class Mesh
    {
    public:
        Mesh();
        ~Mesh();

        /**
        * @brief Creates the buffers for a given MeshData.
        * @param[in] mesh The mesh data that should be used to construct this Mesh.
        * @remarks You can call Create() multiple times on a Mesh. It will destruct any buffers that might have previously been alive.
        */
        void Create(const MeshData& mesh);

        /** @brief Destroys all buffers held in the Mesh. */
        void Destroy();

        /** @returns The MeshData that was used to construct this Mesh. */
        const MeshData& GetMeshData() const;
        
        /** @returns A StructuredBuffer that has all vertex data in it for this Mesh. */
        const StructuredBuffer& GetVertexBuffer() const;

        /** @returns A ByteAddressBuffer that has all index data in it for this Mesh. */
        const ByteAddressBuffer& GetIndexBuffer() const;
    private:
        bool initialized_;                  //!< Whether the Mesh has been initialized yet.
        MeshData mesh_data_;                //!< The MeshData that was used to construct this Mesh.
        StructuredBuffer vertex_buffer_;    //!< A StructuredBuffer that has all vertex data in it for this Mesh.
        ByteAddressBuffer index_buffer_;    //!< A ByteAddressBuffer that has all index data in it for this Mesh.
    };
}