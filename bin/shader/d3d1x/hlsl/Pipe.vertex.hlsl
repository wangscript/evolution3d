//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
matrix matWorld;
matrix matView;
matrix matProject;




//--------------------------------------------------------------------------------------
struct VS_INPUT
{
      float4 pos      : POSITION;
	  float4 norm     : NORMAL;
	  float4 Color    : COLOR;
      float4 dir      : TEXCOORD0;
};

struct GS_INPUT
{
    float4 pos      : SV_POSITION;
	float4 norm     : NORMAL;
    float4 dir      : TEXCOORD0;
    float4 Color    : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
GS_INPUT main( VS_INPUT input )
{
      GS_INPUT output = (GS_INPUT)0;
      input.pos.w = 1.0f;
      output.pos = mul( matWorld   , input.pos );
//      output.pos = mul( matView    , output.pos);
//      output.pos = mul( matProject , output.pos);


//	  output.pos = input.pos ;
      output.norm = input.norm;
      output.dir = input.dir;

      output.Color = input.Color;
      return output;
}