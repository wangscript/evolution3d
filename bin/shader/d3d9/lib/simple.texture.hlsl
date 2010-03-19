///{{Slot = COMMON_Texture
Input 
{
      float4    Color          : COLOR0;
      float4    Tex            : TEXCOORD;
      sampler2D DiffuseTexture : TEXTURE0;
}

Output 
{
      float4 color: COLOR0;
}
///}}


///{{SourceCode
float4 COMMON_Texture(float4 _Color , float2 texCoord , sampler2D _texture )
{
      return tex2D(_texture, texCoord.xy) * _Color ;
}
///}}
