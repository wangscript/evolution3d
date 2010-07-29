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
      output.Pos = mul(input.Pos  ,  matWorld  );
      output.Pos = mul(output.Pos ,  matView   );
      output.Pos = mul(output.Pos ,  matProject);
      output.Tex = float4(input.Tex , 1.0f , 1.0f);
      output.Color = input.Color;

      return output;
}