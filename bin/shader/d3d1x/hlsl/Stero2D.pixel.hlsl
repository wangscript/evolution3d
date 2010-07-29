SamplerState DefaultSampler : register(s0);
Texture2D    DiffuseTexture : register(t0);
Texture2D    RightEyeTexture : register(t1);
struct PS_INPUT
{      
   float4 Pos      : SV_POSITION;      
   float4 Tex      : TEXCOORD0;     
   float4 Tex1     : TEXCOORD1;     
   float4 Color    : COLOR;
};

float4 COMMON_Texture(float4 _Color , float4 texCoord , SamplerState _sampler , Texture2D _texture )
{     
    return _texture.Sample(_sampler, texCoord.xy) * _Color ;
}
 
float4 CM_SteroLeft(float4 vDiffuse )
{     
    return float4(vDiffuse.x , 0 , 0 , vDiffuse.w);   
}
  
float4 CM_SteroRight(float4 vDiffuse )
{     
    return float4(0 , vDiffuse.y , vDiffuse.z , vDiffuse.w);    
}
 
float4 main( PS_INPUT input) : SV_Target
{   
   float4 vDiffuse =  input.Color ;
   float4 lColor = DiffuseTexture.Sample(DefaultSampler, input.Tex.xy);
   lColor = CM_SteroLeft(lColor);

   float4 rColor = RightEyeTexture.Sample(DefaultSampler, input.Tex.xy);
   rColor = CM_SteroRight(rColor);

   vDiffuse = lColor ; 
   vDiffuse.x += rColor.x;
   vDiffuse.y += rColor.y;
   vDiffuse.z += rColor.z;  

   return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , 1.0f);//clamp(vDiffuse.w , 0.0  , 1.0) );
   
}
