//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
///{{Declaration
sampler2D    DiffuseTexture : register(s0);
struct PS_INPUT
{
      float4 Tex      : TEXCOORD0;
      float4 Color    : COLOR;
};
///}}

///>>Import = libDip

///{{SourceCode
float4 main( PS_INPUT input) : COLOR0
{
      float4 vDiffuse =  input.Color ;
      ///}}


    /*
    ///{{Default = ColorModify
    vDiffuse = float4(vDiffuse.xxx , 1.0);
    ///}}
    */

/*
///{{Socket = ColorModify // Socket = 名字, 在这里定义一个插入点
      Input
      {
            float4    vDiffuse        : COLOR0;
            float4    input.Tex       : TEXCOORD;
            float4    input.Tex       : TEXCOORD0;
            float4    input.Tex       : TEXCOORD1;
            sampler2D DiffuseTexture  : TEXTURE0;
      }
      Output 
      {
            float4 vDiffuse : COLOR0;       
      }
///}}
*/


      ///{{SourceCode
       return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , clamp(vDiffuse.w , 0.0  , 1.0) );
}
///}}
