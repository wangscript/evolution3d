///{{Declaration
struct cTransformBuffer 
{
      float4x4 matWorld;
      float4x4 matView;
      float4x4 matProject;
      float4x4 matTexture;
	  float4 cameraUp;
	  float4 cameraPos;
	  float4 cameraDir;
	  float4 cameraArg;//[Near , Far , Fov , Aspect]
};

cTransformBuffer TransformBuffer;

struct STATICMESHVS_INPUT
{
      float3 Pos      : POSITION;
      float2 UV       : TEXCOORD;
};


struct PS_INPUT
{
      float4 Pos       : POSITION;
      float4 Nor       : TEXCOORD0;
      float4 Color     : TEXCOORD1;
      float4 Tex       : TEXCOORD2; 

      float4 wPosition : TEXCOORD3;
      float4 wNormal   : TEXCOORD4;  
};

#define VS_INPUT STATICMESHVS_INPUT
///}}



///{{SourceCode
PS_INPUT main( VS_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
      output.Pos   = mul( TransformBuffer.matWorld   , float4(input.Pos,1) );
      output.Pos   = mul( TransformBuffer.matView    , output.Pos);
      output.Pos   = mul( TransformBuffer.matProject , output.Pos);
      output.Nor   = float4(0,0,1,0);
      output.Color = float4(1,1,1,1);
      output.Tex   = float4(input.UV , 1.0f , 1.0f)

      output.wPosition = mul( TransformBuffer.matWorld   , float4(input.Pos,1) );
      output.wNormal  = float4(0,0,1,0);
///}}

/*
///{{Socket = VertexModifier // Socket = 名字, 在这里定义一个插入点
      Input
      {
            //输入的位置和切法向量
            float4 input.Pos      : POSITION;
            float4 input.Nor      : NORMAL;
            float4 input.Tan      : TANGENT;

            //定义世界空间
            float4 input.Pos      : WPOSITION;
            float4 input.Nor      : WNORMAL;
            float4 input.Tan      : WTANGENT;

            //定义变换后的值
            float4 output.Pos     : TPOSITION;
            float4 output.Nor     : TNORMAL;
            float4 output.Tan     : TTANGENT;

            float4 input.Color    : COLOR;
            float4 input.Weights  : BLENDWEIGHT;
            int4   input.BoneIdxs : BLENDINDICES;
            float4 output.Tex     : TEXCOORD;  
      }
      Output 
      {
            //定义变换后的值
            float4 output.Pos      : TPOSITION;
            float4 output.Nor      : TNORMAL;
            float4 output.Tan      : TTANGENT;

            //定义变换后的世界空间的位置,切法向量
            float4 output.wPosition: WPOSITION;
            float4 output.wNormal  : WNORMAL;
            float4 output.wTangent : WTANGENT;

            //纹理坐标，颜色。
            float4 output.Color    : COLOR;
            float4 output.Tex      : TEXCOORD;   
      }
///}}
*/


///{{SourceCode 
    return output;
}
///}}