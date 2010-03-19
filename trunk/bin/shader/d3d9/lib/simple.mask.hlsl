///{{Slot = CM_Mask
Input 
{
     float4     Color          : COLOR0;
     float4     Tex            : TEXCOORD1;
}

Output 
{
     float4 color: COLOR0;
}
///}}


///{{Declaration
sampler2D    Texture1       : register(s1);;
///}}

///{{SourceCode
float4 CM_Mask(float4 vDiffuse , float2 texCoord )
{
      float4 maskColor = tex2D(Texture1, texCoord.xy);
      float4 ret = vDiffuse * maskColor;
      return ret;
}
///}}

