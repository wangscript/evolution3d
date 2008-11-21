/*
///{{Slot = COMMON_Texture
Input 
{
       float4    Color          : COLOR0;
	   float4    Tex            : TEXCOORD;
	   Sampler   DefaultSampler : SAMPLER;
	   Texture2D DiffuseTexture : TEXTURE0;
}

Output 
{
   float4 color: COLOR0;
}
///}}
*/


///{{SourceCode
float4 COMMON_Texture(float4 _Color , float2 texCoord , SamplerState _sampler , Texture2D _texture )
{
     return _texture.Sample(_sampler, texCoord.xy) * _Color ;
}
///}}
