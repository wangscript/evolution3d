
struct xLightDesc
{
    float4 m_Ambient;
    float4 m_Diffuse;
    float4 m_Speculer;
    float4 m_Position;
    float4 m_dir;
};

struct cLightState
{   
    int4       nLight;  

    float4     m_Ambient [8];
    float4     m_Diffuse [8];
    float4     m_Speculer[8];
    float4     m_Position[8];
    float4     m_dir     [8];
};

cLightState xLightState;

float4 Phong_Lighting(float3 vNormal , float3 wPosition , float3 eyePosition)
{
   float4 vTotal  = float4(0.0f,0.0f,0.0f , 0.0f);
   for(int i = 0 ; i < xLightState.nLight.x ; i ++ )
   {
      //xLightDesc _desc = xLightState.m_Light[i];
      float3 LightDirection = xLightState.m_Position[i].xyz - wPosition;
      
      float3 fvLightDirection = normalize( LightDirection );
      float3 fvNormal         = normalize( vNormal );
      float  fNDotL           = dot( fvNormal, fvLightDirection ); 
   
      float3 fvReflection     = normalize( ( ( 2.0f * fvNormal ) * ( fNDotL ) ) - fvLightDirection ); 
      float3 fvViewDirection  = normalize( eyePosition -  wPosition);
      float  fRDotV           = max( 0.0f, dot( fvReflection, fvViewDirection ) );
      
      float4 fvTotalAmbient   = xLightState.m_Ambient[i]; 
      float4 fvTotalDiffuse   = fNDotL * xLightState.m_Diffuse[i]; 
      float4 fvTotalSpecular  = float4(xLightState.m_Speculer[i].xyz , 1.0f) * pow( fRDotV, xLightState.m_Speculer[i].w );
   
      vTotal += ( saturate( fvTotalDiffuse + fvTotalAmbient +  fvTotalSpecular ) );
   }
   //vTotal.x = xLightState.nLight.x;///3.0f;//abs(vTotal.x );
   //vTotal.y = 0.0f;//abs(vTotal.y );
   //vTotal.z = 0.0f;//abs(vTotal.z );
   vTotal.w = 1.0f;
   return vTotal;
} 