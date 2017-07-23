Texture2D TextureAmbient : register(t0);
Texture2D TextureDiffuse : register(t1);
Texture2D TextureEmissive : register(t2);
Texture2D TextureBump : register(t3);
Texture2D TextureNormal : register(t4);
Texture2D TextureSpecularPower : register(t5);
Texture2D TextureSpecular : register(t6);
Texture2D TextureOpacity : register(t7);

SamplerState Sampler : register(s0);

cbuffer ObjectBuffer : register(b0)
{
    float4x4 World;
    float4x4 CameraView;
    float4x4 CameraProjection;
}

cbuffer MaterialBuffer : register(b1)
{
    float3 MaterialColorDiffuse;
    uint MaterialUseAmbientTexture;
    float3 MaterialColorSpecular;
    uint MaterialUseDiffuseTexture;
    float3 MaterialColorAmbient;
    uint MaterialUseEmissiveTexture;
    float3 MaterialColorEmissive;
    uint MaterialUseBumpTexture;

    float MaterialOpacity;
    float MaterialSpecularScale;
    float MaterialSpecularPower;
    float MaterialBumpIntensity;
    
    uint MaterialUseNormalTexture;
    uint MaterialUseSpecularPowerTexture;
    uint MaterialUseSpecularTexture;
    uint MaterialUseOpacityTexture;
}

struct Material
{
    float3 Emissive;
    float3 Ambient;
    float3 Diffuse;
    float3 Specular;
    float SpecularPower;
};

cbuffer PassBuffer : register(b2)
{
    float3 EyePosition;
    float SpecularPower;
    bool ShowDiffuse;
    bool ShowSpecular;
    bool ShowAmbient;
    bool ShowEmissive;
    float4 PassLightColor;
    float4 PassLightPosition;
    float LightIntensity;
    float LightRange;
    uint NormalMapping;
}

struct VertexOut
{
    float4 PosLocal : POSITIONLOCAL;
    float4 PosWorld : POSITIONWORLD;
    float4 PosView : POSITIONVIEW;
    float4 PosHomogeneous : SV_POSITION;
    float3 NormalWorld : NORMAL;
    float3 TangentWorld : TANGENT;
    float3 BitangentWorld : BITANGENT;
    float2 UV : UV;
};

struct LightingResult
{
    float3 diffuse;
    float3 specular;
};

struct Light
{
    float3 position;
    float3 color;
    float range;
    float intensity;
};

float DoAttenuation(Light light, float3 P, float3 N, float3 E)
{
    return 1.0f - smoothstep(light.range * 0.50f, light.range, distance(P, light.position));
}

float3 DoDiffuse(Material mat, Light light, float3 P, float3 N, float3 E)
{
    float3 L = normalize(light.position - P);

    return mat.Diffuse * light.color * max(dot(N, L), 0.0f);
}

float3 DoSpecular(Material mat, Light light, float3 P, float3 N, float3 E)
{
    float3 L = normalize(light.position - P);
    float3 V = normalize(E - P);
    float3 H = normalize(L + V);

    return mat.Specular * pow(max(dot(N, H), 0), mat.SpecularPower);
}

LightingResult DoLighting(Material mat, Light light, float3 P, float3 N, float3 E)
{
    float3 L = light.position - P;
    float D = length(L);
    L = L / D; // normalizes L using D

    LightingResult result;

    float attenuation = DoAttenuation(light, P, N, E);
    result.diffuse =    DoDiffuse(mat, light, P, N, E) * light.intensity * attenuation;
    result.specular =   DoSpecular(mat, light, P, N, E) * light.intensity * attenuation;

    return result;
}

float3 ExpandNormal(float3 normal)
{
    return normal * 2.0f - 1.0f;
}

float3 CompressNormal(float3 normal)
{
    return normal / 2.0f + 0.5f;
}

float3 DoNormalMapping(float3x3 TBN, Texture2D tex, sampler s, float2 uv)
{
    float3 normal = ExpandNormal(tex.Sample(s, uv).xyz);
    return normalize(mul(normal, TBN));
}

float3 DoBumpMapping(float3x3 TBN, Texture2D tex, sampler s, float2 uv, float bumpScale)
{
    float height = tex.Sample(s, uv).r * bumpScale;
    float heightU = tex.Sample(s, uv, int2(1, 0)).r * bumpScale;
    float heightV = tex.Sample(s, uv, int2(0, 1)).r * bumpScale;
 
    float3 p = { 0, 0, height };
    float3 pU = { 1, 0, heightU };
    float3 pV = { 0, 1, heightV };
 
    float3 normal = cross(normalize(pU - p), normalize(pV - p));
 
    normal = mul(normal, TBN);
 
    return normal;
}

float4 main(VertexOut input) : SV_Target0
{
    float3 GlobalAmbient = float3(0.05f, 0.05f, 0.05f);

    Material material;
    material.Diffuse = MaterialColorDiffuse;
    material.Specular = MaterialColorSpecular;
    material.Ambient = MaterialColorAmbient;
    material.Emissive = MaterialColorEmissive;
    material.SpecularPower = MaterialSpecularPower;

    float3 P = input.PosWorld.xyz;
    float3 N = normalize(input.NormalWorld);
    float3 E = EyePosition;

    if (MaterialUseAmbientTexture)
    {
        material.Ambient = TextureAmbient.Sample(Sampler, input.UV).xyz;
    }

    if (MaterialUseEmissiveTexture)
    {
        material.Emissive = TextureEmissive.Sample(Sampler, input.UV).xyz;
    }

    if (MaterialUseDiffuseTexture)
    {
        material.Diffuse = TextureDiffuse.Sample(Sampler, input.UV).xyz;
    }

    if (MaterialUseSpecularPowerTexture)
    {
        material.SpecularPower = TextureSpecularPower.Sample(Sampler, input.UV).x;
    }

    if (MaterialUseSpecularTexture)
    {
        material.Specular = TextureDiffuse.Sample(Sampler, input.UV).xyz;
    }

    if (MaterialUseNormalTexture && NormalMapping != 0)
    {
        float3x3 TBN = float3x3(normalize(input.TangentWorld), normalize(input.BitangentWorld), normalize(input.NormalWorld));
        
        N = DoNormalMapping(TBN, TextureNormal, Sampler, input.UV);
    }

    if (MaterialUseBumpTexture && NormalMapping != 0)
    {
        float3x3 TBN = float3x3(normalize(input.TangentWorld), normalize(input.BitangentWorld), normalize(input.NormalWorld));
        
        N = DoBumpMapping(TBN, TextureBump, Sampler, input.UV, MaterialBumpIntensity);
    }

    if (MaterialUseOpacityTexture)
    {
        clip(TextureOpacity.Sample(Sampler, input.UV).x - 0.05f);
    }

    material.Ambient *= GlobalAmbient;
    material.Specular *= MaterialSpecularScale;
    material.SpecularPower = max(material.SpecularPower, 1.0f);

    Light light;
    light.color = PassLightColor.xyz;
    light.position = PassLightPosition.xyz;
    light.range = LightRange;
    light.intensity = LightIntensity;

    LightingResult result = DoLighting(material, light, P, N, E);

    float3 color = 0.0f;

    if (ShowAmbient)
        color += material.Ambient;

    if (ShowEmissive)
        color += material.Emissive;

    if (ShowDiffuse)
        color += result.diffuse;

    if (ShowSpecular)
        color += result.specular;

    return float4(color, 1.0f);
}