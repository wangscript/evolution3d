SamplerState FontPointSampler : register(s0);
Texture2D    DiffuseTexture : register(t0);

struct PS_INPUT
{
    float4 Pos      : SV_POSITION;
	float2 Tex      : TEXCOORD;
    float4 Color    : COLOR;
};

float4 main( PS_INPUT input) : SV_Target
{
    float a = DiffuseTexture.Sample(FontPointSampler, input.Tex ).w * 1.4 ;
    float4 vDiffuse  = float4(input.Color.xyz , a);
    return vDiffuse;//float4(input.Color.x,input.Tex.x , a , 1.0f);//vDiffuse;
}

 