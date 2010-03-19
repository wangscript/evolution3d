sampler2D    DiffuseTexture : register(s0);

struct PS_INPUT
{
      float2 Tex      : TEXCOORD0;
      float4 Color    : COLOR;
};

float4 main( PS_INPUT input) : COLOR0
{
      float a =  sin(tex2D(DiffuseTexture, input.Tex ).w) * 1.4 ;
      float4 vDiffuse  =  float4(input.Color.xyz , a);
      return vDiffuse;
}

