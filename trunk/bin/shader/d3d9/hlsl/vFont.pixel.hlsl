sampler2D    DiffuseTexture : register(s0);
struct PS_INPUT
{
      float4 Tex      : TEXCOORD;
      float4 Color    : COLOR;
};

float4 main( PS_INPUT input) : COLOR0
{
      float l = tex2D(DiffuseTexture, input.Tex ).r ;
      float4 vDiffuse = float4(input.Color.xyz , 1.0f) * l * 9.4f;
      return float4(1.0,1.0f,1.0f,1.0f);//vDiffuse;
}

