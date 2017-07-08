#pragma once

namespace blowbox
{
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 bitangent;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT4 color;
		uint8_t bone_ids[4];
		float bone_weights[4];

		//Has to be defined for fbx mesh importing to be able to take out duplicate vertices and index them
		bool operator==(const Vertex& other)
		{
			if (
				other.position.x == position.x &&
				other.position.y == position.y &&
				other.position.z == position.z &&

				other.normal.x == normal.x &&
				other.normal.y == normal.y &&
				other.normal.z == normal.z &&

				other.tangent.x == tangent.x &&
				other.tangent.y == tangent.y &&
				other.tangent.z == tangent.z &&

				other.bitangent.x == bitangent.x &&
				other.bitangent.y == bitangent.y &&
				other.bitangent.z == bitangent.z &&

				other.uv.x == uv.x &&
				other.uv.y == uv.y &&

				other.color.x == color.x &&
				other.color.y == color.y &&
				other.color.z == color.z &&
				other.color.w == color.w
				)
			{
				return true;
			}
			return false;
		}
	};

	const static D3D12_INPUT_ELEMENT_DESC input_element_vertex_desc[] = {
		{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BITANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 36,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "UV",				0, DXGI_FORMAT_R32G32_FLOAT,		0, 48,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",			0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 56,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}