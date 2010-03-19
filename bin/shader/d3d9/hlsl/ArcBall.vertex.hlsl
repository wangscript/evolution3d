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
};

struct PS_INPUT
{
      float4 Pos      : POSITION0;
      float4 vPos     : TEXCOORD0;
      float4 wPos     : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT main( VS_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
      input.Pos.w = 1.0f;
      output.Pos = mul( matWorld   , input.Pos );
      output.Pos = mul( matView    , output.Pos);
      output.Pos = mul( matProject , output.Pos);
      output.vPos = input.Pos;
      output.wPos = output.Pos;
      return output;
}