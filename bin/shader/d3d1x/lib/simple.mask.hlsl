///{{Slot = CM_Mask
Input 
{
     float4   Color          : COLOR0;
     float4   Tex            : TEXCOORD1;
     Sampler  DefaultSampler : SAMPLER;
}

Output 
{
     float4 color: COLOR0;
}
///}}


///{{Declaration
Texture2D    Texture1       : register(t1);;
///}}

///{{SourceCode
float4 CM_Mask(float4 vDiffuse , float4 texCoord , SamplerState _sampler)
{
      float4 maskColor = Texture1.Sample(_sampler, texCoord.xy);
      float4 ret = vDiffuse * maskColor;
      return ret;
}
///}}

