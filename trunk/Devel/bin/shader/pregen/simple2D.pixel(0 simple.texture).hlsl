//===============================================
//Begin import lib
//Finish import lib
//============================================


//Insert Node Declaration Node= r:\Projects\SVN\Evol3D\Devel\bin\shader\d3d10\simple2D.pixel.hlsl

SamplerState DefaultSampler : register(s0);
Texture2D    DiffuseTexture : register(t0);
struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float4 Tex      : TEXCOORD0;
      float4 Tex1     : TEXCOORD1;
      float4 Color    : COLOR;
};

//==================================================

//============================================

//Insert Node Code Node= r:\Projects\SVN\Evol3D\Devel\bin\shader\lib\simple.texture.hlsl

float4 COMMON_Texture(float4 _Color , float2 texCoord , SamplerState _sampler , Texture2D _texture )
{
      return _texture.Sample(_sampler, texCoord.xy) * _Color ;
}

//============================================


//============================================
//Begin Main Node'code 

float4 main( PS_INPUT input) : SV_Target
{
      float4 vDiffuse =  input.Color ;


   //Ó¦ÓÃÐÞ¸ÄÆ÷ name=simple.texture 
   float4 Node_0_Ret = COMMON_Texture(vDiffuse , input.Tex , DefaultSampler , DiffuseTexture);

   vDiffuse = Node_0_Ret; 

      return vDiffuse;
}

