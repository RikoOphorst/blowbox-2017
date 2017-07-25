#pragma once

#include "renderer/d3d12_includes.h"
#include "util/vector.h"

#define BLOWBOX_USE_32BIT_INDICES

namespace blowbox
{
    /**
    * This data type is typedef-ed because it allows the entire
    * application to easily switch from 16 bit indices to 32 bit
    * indices, if desired.
    *
    * @typedef blowbox::Index
    * @brief Descibes an index.
    */
#ifdef BLOWBOX_USE_32BIT_INDICES
    typedef uint32_t Index;
#else
    typedef uint16_t Index;
#endif

    /**
    * Meshes consist of vertices. Every vertex has a bunch of information
    * attached to it. This class stores the data for every vertex.
    *
    * @brief Describes a Vertex.
    * @remarks Notably, bitangent is missing from the vertex layout. You 
    *          are supposed to generate the bitangent yourself in the vertex
    *          shader that you have bound to the pipeline.
    */
    class Vertex
    {
    public:
        Vertex() :
            position(0.0f, 0.0f, 0.0f),
            normal(0.0f, 1.0f, 0.0f),
            tangent(1.0f, 0.0f, 0.0f),
            uv(0.0f, 0.0f),
            color(1.0f, 0.0f, 1.0f, 0.0f)
        {
        
        }

        DirectX::XMFLOAT3 position;     //!< The position of the vertex.
        DirectX::XMFLOAT3 normal;       //!< The normal of the vertex.
        DirectX::XMFLOAT3 tangent;      //!< The tangent of the vertex.
        DirectX::XMFLOAT2 uv;           //!< The UV coordinates of the vertex.
        DirectX::XMFLOAT4 color;        //!< The color of the vertex.

        /** @returns A list of D3D12_INPUT_ELEMENT_DESCs that explain the entire data layout of a vertex. */
        static Vector<D3D12_INPUT_ELEMENT_DESC> GetInputElements() 
        {
            Vector<D3D12_INPUT_ELEMENT_DESC> input_elements(5);

            input_elements[0] = { "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,     0, 0,   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
            input_elements[1] = { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,     0, 12,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
            input_elements[2] = { "TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,     0, 24,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
            input_elements[3] = { "UV",          0, DXGI_FORMAT_R32G32_FLOAT,        0, 36,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
            input_elements[4] = { "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 44,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

            return input_elements;
        }
    };
}