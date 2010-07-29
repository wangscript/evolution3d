
sampler2D    DiffuseTexture : register(s0);
struct PS_INPUT
{
      float4 Tex      : TEXCOORD0;
      float4 Tex1     : TEXCOORD1;
      float4 Color    : COLOR;
};

float4 main( PS_INPUT input) : COLOR0
{
      float4 vDiffuse =  input.Color ;

      return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , clamp(vDiffuse.w , 0.0  , 1.0) );
}
