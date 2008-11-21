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
    float4 Pos      : SV_POSITION;
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
    output.Pos = mul( matWorld   , input.Pos );
    output.Pos = mul( matView    , output.Pos);
    output.Pos = mul( matProject , output.Pos);
	output.Tex  = float4(input.Tex,1.0f,1.0f);
	output.Tex1 = float4(input.Tex1,1.0f,1.0f);
    output.Color = input.Color;
    
    return output;
}