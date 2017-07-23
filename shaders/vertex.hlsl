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
    float MaterialShininess;
    float MaterialShininessStrength;
    float MaterialBumpIntensity;
    
    uint MaterialUseNormalTexture;
    uint MaterialUseShininessTexture;
    uint MaterialUseSpecularTexture;
    uint MaterialUseOpacityTexture;
}

struct VertexIn
{
    float3 PosLocal : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 UV : UV;
    float4 Color : COLOR;
};

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

Texture2D TextureAmbient : register(t0);
Texture2D TextureDiffuse : register(t1);
Texture2D TextureEmissive : register(t2);
Texture2D TextureBump : register(t3);
Texture2D TextureNormal : register(t4);
Texture2D TextureShininess : register(t5);
Texture2D TextureSpecular : register(t6);
Texture2D TextureOpacity : register(t7);

VertexOut main(VertexIn input)
{
    VertexOut vout;
    vout.PosLocal = float4(input.PosLocal, 1.0f);
    vout.PosWorld = mul(World, vout.PosLocal);
    vout.PosView = mul(CameraView, vout.PosWorld);
    vout.PosHomogeneous = mul(CameraProjection, vout.PosView);

    vout.NormalWorld = mul(World, float4(input.Normal, 0.0f)).xyz;
    vout.TangentWorld = mul(World, float4(input.Tangent, 0.0f)).xyz;
    vout.BitangentWorld = cross(vout.NormalWorld, vout.TangentWorld);

    vout.UV = input.UV;

    return vout;
}