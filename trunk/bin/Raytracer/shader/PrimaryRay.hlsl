//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
///{{Declaration
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
///}}

///{{SourceCode
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
    o.Color     = float4(0.0,0.0,0.0,0.0) ;
///}}

    /*
///{{Default = ColorModify
    vDiffuse = float4(vDiffuse.xxx , 1.0);
///}}
    */

    /*
///{{Socket = ColorModify 
    Input
    {
        float4    o.Color         : COLOR0;
        float4    input.Color     : VERTEXCOLOR;
        float4    input.Tex       : TEXCOORD;
        float4    input.Tex       : TEXCOORD0;
        float4    input.Tex       : TEXCOORD1;
        float4    input.wPosition : WPOSITION;
        float4    input.wNormal   : WNORMAL;  
        float4    input.wTangent  : WTANGENT; 

        Sampler   DefaultSampler  : SAMPLER;
        Texture2D DiffuseTexture  : TEXTURE0;
    }
    Output 
    {
        float4 o.Color : COLOR0;       
    }
///}}
    */

///{{SourceCode
    o.wPosition = input.wPosition;
    o.wNormal   = normalize(input.wNormal);
    o.wTangent  = normalize(input.wTangent);

    //计算反射向量
    float3 vDir = normalize(input.wPosition.xyz - cameraPos.xyz);
    float3 vNormal = o.wNormal.xyz;
    float3 vReflection = 2 * dot(-vDir , vNormal) * vNormal + vDir ;  

    //反射向量
    o.wReflect = float4(vReflection.x , vReflection.y , vReflection.z , 1.0f);
    return o;
}
///}}


