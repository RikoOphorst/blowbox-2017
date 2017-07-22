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

struct VertexOut
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 UV : UV;
};

Texture2D DiffuseTexture : register(t0);
SamplerState SamplerAnisotropicWrap : register(s0);

float4 main(VertexOut input) : SV_Target0
{
    float3 color = DiffuseTexture.Sample(SamplerAnisotropicWrap, input.UV).xyz;

    color *= dot(input.Normal, normalize(-float3(0.5f, -1.0f, 0.5f)));

    return float4(color, 1.0f);
}