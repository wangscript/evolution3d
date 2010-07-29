//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
matrix matWorld;
matrix matView;
matrix matProject;

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
      float4 Pos      : POSITION;
      float4 Color    : COLOR;
};

struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float4 Color    : COLOR;
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

//	  output.Pos = input.Pos ;

      output.Color = input.Color;
      return output;
}