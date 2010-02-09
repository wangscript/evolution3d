
SamplerState DefaultSampler : register(s0);
Texture2D    DiffuseTexture : register(t0);
struct PS_INPUT
{
    float4 Pos      : SV_POSITION;
    float4 Nor      : NORMAL;
    float4 Color    : COLOR;
    float4 Tan      : TANGENT;
    float2 Tex      : TEXCOORD;

    float4 wPosition : TEXCOORD2;
    float4 wNormal   : TEXCOORD3;  
    float4 wTangent  : TEXCOORD4;

};

cbuffer TransformBuffer
{
    matrix matWorld;
    matrix matView;
    matrix matProject;
    float4 cameraUp;
    float4 cameraPos;
    float4 cameraDir;
    float4 cameraArg;//[Near , Far , Fov , Aspect]
}

struct PS_OUTPUT
{
    float4 Color     : SV_Target0;
    float4 wPosition : SV_Target1;
    float4 wNormal   : SV_Target2;
    float4 wReflect  : SV_Target3;
    float4 wTangent  : SV_Target4;
};

PS_OUTPUT main( PS_INPUT input)
{
    PS_OUTPUT o;
    o.Color     = float4(1.0,1.0,1.0,1.0) ;
    o.wPosition = float4(0.0,0.0,0.0,-100000.0);
    o.wNormal   = float4(0.0,0.0,0.0,-100000.0);
    o.wTangent  = float4(0.0,0.0,0.0,-100000.0);
    o.wReflect  = float4(0.0,0.0,0.0,-100000.0);
    return o;
}
