

matrix matWorld;
matrix matView;
matrix matProject;

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


