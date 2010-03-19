//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
///{{Declaration
struct cTransformBuffer
{
      matrix matWorld;
      matrix matView;
      matrix matProject;
	  float4 cameraUp;
	  float4 cameraPos;
	  float4 cameraDir;
	  float4 cameraArg;//[Near , Far , Fov , Aspect]
};

cTransformBuffer TransformBuffer;

sampler2D    DiffuseTexture : register(s0);
float        AlphaRef;

struct PS_INPUT
{      
     
      float4 Color     : COLOR;

      float2 Tex       : TEXCOORD0;  
      float4 Pos       : TEXCOORD1;
      float4 Nor       : TEXCOORD2;     
      float4 Tan       : TEXCOORD3;      

      float4 wPosition : TEXCOORD4;
      float4 wNormal   : TEXCOORD5;  
      float4 wTangent  : TEXCOORD6;  
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
          float4    input.Position  : POSITION;
          float4    input.Normal    : NORMAL;  
          float4    input.Tangent   : TANGENT; 
          
          float4    input.wPosition : WPOSITION;
          float4    input.wNormal   : WNORMAL;  
          float4    input.wTangent  : WTANGENT; 

          sampler2D DiffuseTexture  : TEXTURE0;
          
          //Camera Infomation
          float4 TransformBuffer.cameraUp       : CAMERAUP ;
          float4 TransformBuffer.cameraPos      : CAMERAPOSITION ;
          float4 TransformBuffer.cameraDir      : CAMERADIR ;
          float4 TransformBuffer.cameraArg      : CAMERAARG ;
    }
    Output 
    {
          float4 vDiffuse : COLOR0;       
    }
///}}
*/

///{{SourceCode
    vDiffuse.w = 1.0f;
    if(vDiffuse.w <= AlphaRef)
       discard;
    return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , 1.0f);// clamp(vDiffuse.w , 0.0  , 1.0) );
}
///}}
