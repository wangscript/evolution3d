//===============================================
//Begin import lib
//Finish import lib
//============================================


//Insert Node Declaration Node= D:\SVN\Evol3D\xEvolEngine\bin\shader\d3d1x\hlsl\simple2D.pixel.hlsl

SamplerState DefaultSampler : register(s0);
Texture2D    DiffuseTexture : register(t0);
struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float4 Tex      : TEXCOORD0;
      float4 Tex1     : TEXCOORD1;
      float4 Color    : COLOR;
};

//Insert Node Declaration Node= D:\SVN\Evol3D\xEvolEngine\bin\shader\d3d1x\lib\simple.mask.hlsl

Texture2D    Texture1       : register(t1);;
//==================================================

//============================================

//Insert Node Code Node= D:\SVN\Evol3D\xEvolEngine\bin\shader\d3d1x\lib\simple.texture.hlsl

float4 COMMON_Texture(float4 _Color , float2 texCoord , SamplerState _sampler , Texture2D _texture )
{
      return _texture.Sample(_sampler, texCoord.xy) * _Color ;
}

//Insert Node Code Node= D:\SVN\Evol3D\xEvolEngine\bin\shader\d3d1x\lib\simple.mask.hlsl

float4 CM_Mask(float4 vDiffuse , float2 texCoord , SamplerState _sampler)
{
      float4 maskColor = Texture1.Sample(_sampler, texCoord.xy);
      float4 ret = vDiffuse * maskColor;
      return ret;
}
//============================================


//============================================
//Begin Main Node'code 

float4 main( PS_INPUT input) : SV_Target
{
      float4 vDiffuse =  input.Color ;

   //应用修改器 name=simple.texture 
   float4 Node_0_Ret = COMMON_Texture(vDiffuse , input.Tex , DefaultSampler , DiffuseTexture);

   //应用修改器 name=simple.mask 
   float4 Node_1_Ret = CM_Mask(Node_0_Ret , input.Tex1 , DefaultSampler);

   vDiffuse = Node_1_Ret; 

      return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , clamp(vDiffuse.w , 0.0  , 1.0) );
}
