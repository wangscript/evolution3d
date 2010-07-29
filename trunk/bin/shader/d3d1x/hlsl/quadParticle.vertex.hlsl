///{{Declaration
cbuffer TransformBuffer
{
      float4x4 matWorld;
      float4x4 matView;
      float4x4 matProject;
      float4x4 matTexture;
	  float4 cameraUp;
	  float4 cameraPos;
	  float4 cameraDir;
	  float4 cameraArg;//[Near , Far , Fov , Aspect]
}

struct BILLBOARD_INPUT
{
      float4 Pos      : POSITION;
      float4 Color    : COLOR;
      float2 Tex      : TEXCOORD;  
};


struct PS_INPUT
{
      float4 Pos       : SV_POSITION;
      float4 Color     : COLOR;
      float4 Tex       : TEXCOORD;  
      float4 wPosition : TEXCOORD2;
};


///}}

///>>Import = libMatrix

///{{SourceCode
PS_INPUT main( BILLBOARD_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
      output.Pos   = mul( matWorld   , input.Pos );
      output.Pos   = mul( matView    , output.Pos);
      output.Pos   = mul( matProject , output.Pos);
      output.Tex   = float4(input.Tex , 1.0f , 1.0f);
      output.Color = input.Color.xyzw;

      //世界坐标系中的
      output.wPosition = mul(matWorld  , input.Pos );
///}}

/*
///{{Socket = VertexModifier // Socket = 名字, 在这里定义一个插入点
      Input
      {
            //输入的位置和切法向量
            float4 input.Pos      : POSITION;

            //定义世界空间
            float4 input.Pos      : WPOSITION;

            //定义变换后的值
            float4 output.Pos     : TPOSITION;

            float4 input.Color    : COLOR;
            float4 output.Tex     : TEXCOORD;  
            
            //Camera Infomation
            float4 cameraUp       : CAMERAUP ;
            float4 cameraPos      : CAMERAPOSITION ;
            float4 cameraDir      : CAMERADIR ;
            float4 cameraArg      : CAMERAARG ;
	  
      }
      Output 
      {
            //定义变换后的值
            float4 output.Pos      : TPOSITION;
            //定义变换后的世界空间的位置,切法向量
            float4 output.wPosition: WPOSITION;
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