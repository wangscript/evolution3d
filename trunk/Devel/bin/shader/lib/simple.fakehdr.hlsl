/*
///{{Slot = CM_FakeHDR
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

///>>Import = libDip


///{{SourceCode





float4 CM_FakeHDR(float4 vDiffuse , float2 texCoord , SamplerState _sampler , Texture2D _texture )
{
   float2    TexSize = float2(32 , 32);
   float2    intXY   = float2(texCoord.x * TexSize.x , texCoord.y * TexSize.y);
   
   float3x3 _smooth_fil = float3x3 (1/16.0 ,2/16.0,1/16.0 ,
                                    2/16.0 ,4/16.0,2/16.0 ,
                                    1/16.0 ,2/16.0,1/16.0 );
          
   float  _exposure  = 1.6;   
   float4 _dsColor   = dip_filter(_smooth_fil , _sampler , _texture , intXY, TexSize);
   float  _lum       = 0.3 * _dsColor.x + 0.59 * _dsColor.y + 0.11 * _dsColor.z;
   return float4(xposure(vDiffuse , _lum , _exposure ).xyz , vDiffuse.w);    
}

///}}}
