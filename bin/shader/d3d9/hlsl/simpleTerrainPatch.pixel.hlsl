//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
///{{Declaration
sampler2D    DiffuseTexture : register(s0);
struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float4 Nor       : TEXCOORD0;
      float4 Color     : TEXCOORD1;
      float4 Tex       : TEXCOORD2; 

      float4 wPosition : TEXCOORD3;
      float4 wNormal   : TEXCOORD4; 
};
///}}

///>>Import = libDip

///{{SourceCode
float4 main( PS_INPUT input) : COLOR0
{
    float4 vDiffuse =  float4(1.0,1.0,1.0,1.0);//input.Color ;
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
		  float4    input.Color     : VERTEXCOLOR;
          float4    input.Tex       : TEXCOORD;
          float4    input.Tex       : TEXCOORD0;
          float4    input.Tex       : TEXCOORD1;
          float4    input.wPosition : WPOSITION;
          float4    input.wNormal   : WNORMAL;  
          float4    input.wTangent  : WTANGENT; 
          sampler2D DiffuseTexture  : TEXTURE0;
    }
    Output 
    {
          float4 vDiffuse : COLOR0;       
    }
///}}
*/

///{{SourceCode
   if(vDiffuse.w <0.01)
	   discard;
    return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , clamp(vDiffuse.w , 0.0  , 1.0) );
}
///}}
