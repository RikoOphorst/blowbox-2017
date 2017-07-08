#pragma once

#include "d3d12_includes.h"

namespace blowbox
{
	class Texture;

	struct Material
	{
	public:
		bool use_emissive_map = false;
		DirectX::XMFLOAT3 emissive = DirectX::XMFLOAT3(0.0f, 0.2f, 0.0f);
		Texture* emissive_map = nullptr;

		bool use_diffuse_map = false;
		DirectX::XMFLOAT3 diffuse = DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f);
		Texture* diffuse_map = nullptr;

		bool use_specular_map = false;
		DirectX::XMFLOAT3 specular = DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f);
		Texture* specular_map;

		bool use_normal_map = false;
		Texture* normal_map = nullptr;

		bool use_shininess_map = false;
		float shininess = 200.0f;
		Texture* shininess_map = nullptr;

		bool use_ambient_map = false;
		DirectX::XMFLOAT3 ambient_reflectance = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
		Texture* ambient_map = nullptr;

		UINT renderer_material_cb_id = 0;
		UINT renderer_material_descriptor_id = 0;
	};
}