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
};
///}}

///>>Import = libDip

///{{SourceCode
float4 main( PS_INPUT input) : SV_Target
{
    float4 vDiffuse =  float4(1.0,1.0,1.0,1.0);//input.Color ;
///}}


///{{Default = ColorModify
    //vDiffuse = float4(vDiffuse.xxx , 1.0);
///}}

/*
///{{Socket = ColorModify // Socket = 名字, 在这里定义一个插入点
   Input
   {
       float4    vDiffuse        : COLOR0;
       float4    input.Tex       : TEXCOORD;
       float4    input.Tex       : TEXCOORD0;
	   float4    input.Tex       : TEXCOORD1;
	   Sampler   DefaultSampler  : SAMPLER;
	   Texture2D DiffuseTexture  : TEXTURE0;
   }
   Output 
   {
      float4 vDiffuse : COLOR0;       
   }
///}}
*/

///{{SourceCode
   return vDiffuse;
}
///}}
