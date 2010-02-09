//===============================================
//Begin import lib
//Finish import lib
//============================================


//Insert Node Declaration Node= D:\SVN\Evol3D\xEvolEngine\bin\Raytracer\shader\PrimaryRay.hlsl

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
//==================================================

//============================================

//Insert Node Code Node= D:\SVN\Evol3D\xEvolEngine\bin\shader\d3d1x\lib\simple.texture.hlsl

float4 COMMON_Texture(float4 _Color , float2 texCoord , SamplerState _sampler , Texture2D _texture )
{
      return _texture.Sample(_sampler, texCoord.xy) * _Color ;
}
//============================================


//============================================
//Begin Main Node'code 

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

   //应用修改器 name=simple.texture 
   float4 Node_0_Ret = COMMON_Texture(o.Color , input.Tex , DefaultSampler , DiffuseTexture);

   o.Color = Node_0_Ret; 

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
