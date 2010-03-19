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


struct STATICMESHVS_INPUT
{
      float4 Pos      : POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float4 Tan      : TANGENT;
      float2 Tex      : TEXCOORD;  
};


struct PS_INPUT
{
      float4 oPos      : POSITION0;
      float4 Color     : COLOR;
      float2 Tex       : TEXCOORD0;  
      float4 Pos       : TEXCOORD1;
      float4 Nor       : TEXCOORD2;     
      float4 Tan       : TEXCOORD3;      

      float4 wPosition : TEXCOORD4;
      float4 wNormal   : TEXCOORD5;  
      float4 wTangent  : TEXCOORD6;   
};

#define VS_INPUT STATICMESHVS_INPUT
///}}

///>>Import = libMatrix

///{{SourceCode
PS_INPUT main( VS_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
      output.Pos   = mul( input.Pos  , TransformBuffer.matWorld  );
      output.Pos   = mul( output.Pos , TransformBuffer.matView   );
      output.Pos   = mul( output.Pos , TransformBuffer.matProject);
      output.Tex   = input.Tex;
      output.Color = input.Color.xyzw;


	  float4x4 matModelView = TransformBuffer.matWorld * TransformBuffer.matView;
	  float4x4 matNormal    = normalMatrix(matModelView);

	  //观察坐标系中的
      output.Tan   = mul(matNormal , input.Tan);
      output.Nor   = mul(matNormal , input.Nor);

   	  //世界坐标系中的
	  matNormal = normalMatrix(TransformBuffer.matWorld);
      output.wPosition = mul( input.Pos  , TransformBuffer.matWorld );
      output.wNormal   = mul( input.Nor  , matNormal                );
      output.wTangent  = mul( input.Tan  , matNormal                );
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
            float2 input.Tex      : TEXCOORD;  
            
            //Camera Infomation
            float4 TransformBuffer.cameraUp       : CAMERAUP ;
            float4 TransformBuffer.cameraPos      : CAMERAPOSITION ;
            float4 TransformBuffer.cameraDir      : CAMERADIR ;
            float4 TransformBuffer.cameraArg      : CAMERAARG ;
	  
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
            float2 output.Tex      : TEXCOORD;   
      }
///}}
*/


///{{SourceCode 
    output.oPos = output.Pos;
    return output;
}
///}}