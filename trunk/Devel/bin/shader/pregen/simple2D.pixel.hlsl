
SamplerState DefaultSampler : register(s0);
Texture2D    DiffuseTexture : register(t0);
struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float4 Tex      : TEXCOORD0;
      float4 Tex1     : TEXCOORD1;
      float4 Color    : COLOR;
};


float4 main( PS_INPUT input) : SV_Target
{
      float4 vDiffuse =  input.Color ;


      return vDiffuse;
}

