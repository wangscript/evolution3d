
struct xLightDesc
{
    float4 m_Ambient;
    float4 m_Diffuse;
    float4 m_Speculer;
    float4 m_Position;
    float4 m_dir;
};

cbuffer xLightState
{   
    int4       nLight;    
    xLightDesc m_Light[8];
}

float4 Phong_Lighting(float3 vNormal , float3 wPosition , float3 eyePosition)
{
   float4 vTotal  = float4(0.0f,0.0f,0.0f , 0.0f);
   for(int i = 0 ; i < nLight.x ; i ++ )
   {
      xLightDesc _desc = m_Light[i];
      float3 LightDirection = _desc.m_Position.xyz - wPosition;
      
      float3 fvLightDirection = normalize( LightDirection );
      float3 fvNormal         = normalize( vNormal );
      float  fNDotL           = dot( fvNormal, fvLightDirection ); 
   
      float3 fvReflection     = normalize( ( ( 2.0f * fvNormal ) * ( fNDotL ) ) - fvLightDirection ); 
      float3 fvViewDirection  = normalize( eyePosition -  wPosition);
      float  fRDotV           = max( 0.0f, dot( fvReflection, fvViewDirection ) );
      
      float4 fvTotalAmbient   = _desc.m_Ambient; 
      float4 fvTotalDiffuse   = fNDotL * _desc.m_Diffuse; 
      float4 fvTotalSpecular  = float4(_desc.m_Speculer.xyz , 1.0f) * pow( fRDotV, _desc.m_Speculer.w );
   
      vTotal += ( saturate( fvTotalDiffuse + fvTotalAmbient +  fvTotalSpecular ) );
   }
   vTotal.w = 1.0f;
   return vTotal;
} 