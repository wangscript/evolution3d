//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
//matrix matWorld;
//matrix matView;
//matrix matProject;

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

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
      float4 Pos    : POSITION;
      float4 Color  : COLOR;
      float2 Tex    : TEXCOORD;
};

struct PS_INPUT
{
      float4 Pos      : POSITION0;
      float4 Tex      : TEXCOORD0;
      float4 Color    : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT main( VS_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
      output.Pos   = input.Pos;
      output.Pos.x -= 0.5f;
      output.Pos.y -= 0.5f;

      output.Pos   = mul( output.Pos , TransformBuffer.matWorld   );
      output.Pos   = mul( output.Pos , TransformBuffer.matView    );
      output.Pos   = mul( output.Pos , TransformBuffer.matProject );
      output.Tex   = float4(input.Tex , 1.0f , 1.0f);
      output.Color = input.Color;

      return output;

}