SamplerState FontLinear     : register(s0);
Texture2D    DiffuseTexture : register(t0);

struct PS_INPUT
{
    float4 Pos      : SV_POSITION;
	float2 Tex      : TEXCOORD;
    float4 Color    : COLOR;
};

float4 main( PS_INPUT input) : SV_Target
{
    float l = DiffuseTexture.Sample(FontLinear, input.Tex ).r ;
    float4 vDiffuse = float4(input.Color.xyz , 1.0f) * l * 9.4f;
    return vDiffuse;
}

 