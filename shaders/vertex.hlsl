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
    float3 PosL : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 UV : UV;
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 UV : UV;
};

VertexOut main(VertexIn input)
{
    VertexOut vout;
    vout.Pos = mul(World, float4(input.PosL, 1.0f));
    vout.Pos = mul(CameraView, vout.Pos);
    vout.Pos = mul(CameraProjection, vout.Pos);
    vout.Normal = input.Normal;
    vout.UV = input.UV;

    return vout;
}