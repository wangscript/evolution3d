//===============================================
//Begin import lib
//Finish import lib
//============================================


//Insert Node Declaration Node= D:\SVN\evol3d-google\Demo\RayTracerDemo\..\..\bin\shader\d3d9\hlsl\simple2D.pixel.hlsl

sampler2D    DiffuseTexture : register(s0);
struct PS_INPUT
{
      float4 Tex      : TEXCOORD0;
      float4 Tex1     : TEXCOORD1;
      float4 Color    : COLOR;
};
//==================================================

//============================================

//Insert Node Code Node= D:\SVN\evol3d-google\Demo\RayTracerDemo\..\..\bin\shader\d3d9\lib\simple.texture.hlsl

float4 COMMON_Texture(float4 _Color , float4 texCoord , sampler2D _texture )
{
      return tex2D(_texture, texCoord.xy) * _Color ;
}
//============================================


//============================================
//Begin Main Node'code 

float4 main( PS_INPUT input) : COLOR0
{
      float4 vDiffuse =  input.Color ;

   //Ó¦ÓÃÐÞ¸ÄÆ÷ name=simple.texture 
   float4 Ret_ColorModify_Node0 = COMMON_Texture(vDiffuse , input.Tex , DiffuseTexture);

   vDiffuse = Ret_ColorModify_Node0; 

      return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , clamp(vDiffuse.w , 0.0  , 1.0) );
}
