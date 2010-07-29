//===============================================
//Begin import lib
//Finish import lib
//============================================


//Insert Node Declaration Node= D:\SVN\Evol3D\CityDesignner\CityDesignner\..\..\xEvolEngine\bin\shader\d3d1x\hlsl\simple2D.pixel.hlsl

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

//Insert Node Code Node= D:\SVN\Evol3D\CityDesignner\CityDesignner\..\..\xEvolEngine\bin\shader\d3d1x\lib\simple.texture.hlsl

float4 COMMON_Texture(float4 _Color , float4 texCoord , SamplerState _sampler , Texture2D _texture )
{
      return _texture.Sample(_sampler, texCoord.xy) * _Color ;
}

//Insert Node Code Node= D:\SVN\Evol3D\CityDesignner\CityDesignner\..\..\xEvolEngine\bin\shader\d3d1x\lib\SteroL.hlsl


float4 CM_SteroLeft(float4 vDiffuse )
{
      return float4(vDiffuse.x , 0 , 0 , vDiffuse.w);    
}
//============================================


//============================================
//Begin Main Node'code 

float4 main( PS_INPUT input) : SV_Target
{
      float4 vDiffuse =  input.Color ;

   //应用修改器 name=simple.texture 
   float4 Ret_ColorModify_Node0 = COMMON_Texture(vDiffuse , input.Tex , DefaultSampler , DiffuseTexture);

   //应用修改器 name=SteroL 
   float4 Ret_ColorModify_Node1 = CM_SteroLeft(Ret_ColorModify_Node0);

   vDiffuse = Ret_ColorModify_Node1; 

      return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , clamp(vDiffuse.w , 0.0  , 1.0) );
}
