
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
///{{Declaration
cbuffer TransformBuffer
{
      float4x4 matWorld;
      float4x4 matView;
      float4x4 matProject;
      float4x4 matTexture;
	  float4   cameraUp;
	  float4   cameraPos;
	  float4   cameraDir;
	  float4   cameraArg;//[Near , Far , Fov , Aspect]
}

SamplerState DefaultSampler : register(s0);
Texture2D    DiffuseTexture : register(t0);
float        AlphaRef;
struct PS_INPUT
{
      float4 pos      : SV_POSITION;
//      float4 norm     : NORMAL;
      float4 Color    : COLOR;
      float4 tex      : TEXCOORD; 

      float4 wPosition : TEXCOORD2;
      float4 wNormal   : TEXCOORD3;  
};
///}}

///>>Import = libDip

///{{SourceCode
float4 main( PS_INPUT input) : SV_Target
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
          float4    input.tex       : TEXCOORD;
          float4    input.tex       : TEXCOORD0;
          float4    input.tex       : TEXCOORD1;
          float4    input.pos       : POSITION;
          float4    input.norm      : NORMAL;  
          float4    input.tan       : TANGENT; 
          
          float4    input.wPosition : WPOSITION;
          float4    input.wNormal   : WNORMAL;  
          float4    input.wTangent  : WTANGENT; 

          Sampler   DefaultSampler  : SAMPLER;
          Texture2D DiffuseTexture  : TEXTURE0;
          
          //Camera Infomation
          float4 cameraUp       : CAMERAUP ;
          float4 cameraPos      : CAMERAPOSITION ;
          float4 cameraDir      : CAMERADIR ;
          float4 cameraArg      : CAMERAARG ;
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
