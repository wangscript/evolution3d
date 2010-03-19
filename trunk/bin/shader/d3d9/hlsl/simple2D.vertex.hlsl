//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
matrix matWorld;
matrix matView;
matrix matProject;

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
      float4 Pos    : POSITION;
      float4 Color  : COLOR;
      float2 Tex    : TEXCOORD0;
      float2 Tex1   : TEXCOORD1;
};

struct PS_INPUT
{
      float4 Pos      : POSITION;
      float4 Tex      : TEXCOORD0;
      float4 Tex1     : TEXCOORD1;
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
      output.Pos = mul(output.Pos , matWorld   );
      output.Pos = mul(output.Pos , matView    );
      output.Pos = mul(output.Pos , matProject );
      output.Tex  = float4(input.Tex,1.0f,1.0f);
      output.Tex1 = float4(input.Tex1,1.0f,1.0f);
      output.Color = input.Color;

      return output;
}