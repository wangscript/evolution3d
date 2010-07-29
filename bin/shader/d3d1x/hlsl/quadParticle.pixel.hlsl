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
      float4 Pos       : SV_POSITION;
      float4 Color     : COLOR;
      float4 Tex       : TEXCOORD;  
      float4 wPosition : TEXCOORD2;
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
          float4    input.Tex       : TEXCOORD;
          float4    input.Tex       : TEXCOORD0;
          float4    input.Tex       : TEXCOORD1;
          float4    input.Position : POSITION;
          float4    input.Normal   : NORMAL;  
          float4    input.Tangent  : TANGENT; 
          
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
    if(vDiffuse.w <= AlphaRef || vDiffuse.w < 0.0001)
       discard;
    return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , clamp(vDiffuse.w , 0.0  , 1.0) );
}
///}}
