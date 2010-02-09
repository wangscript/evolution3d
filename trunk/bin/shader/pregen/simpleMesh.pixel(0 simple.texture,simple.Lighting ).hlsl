//===============================================
//Begin import lib
//Import lib name=libDip

float4 dip_filter(float3x3 _filter , SamplerState _sampler , Texture2D _texture , float2 _xy, float2 texSize)
{
      float2 _filter_pos_delta[3][3] = 
      {
            { float2(-1.0 , -1.0) , float2(0,-1.0), float2(1.0 , -1.0) },
            { float2( 0.0 , -1.0) , float2(0, 0.0), float2(1.0 ,  0.0) },
            { float2( 1.0 , -1.0) , float2(0, 1.0), float2(1.0 ,  1.0) },
      };

      float4 final_color = float4(0.0,0.0,0.0,0.0); 
      for(int i = 0 ; i < 3 ; i ++ )
      {
            for(int j = 0 ; j < 3 ; j ++)
            {
                  float2 _xy_new = float2(_xy.x + _filter_pos_delta[i][j].x , _xy.y + _filter_pos_delta[i][j].y);
                  float2 _uv_new = float2(_xy_new.x/texSize.x , _xy_new.y/texSize.y);
                  final_color += _texture.Sample( _sampler , _uv_new ) * _filter[i][j];
            } 
      }
      return final_color;
}

//HDR Midle-Gray Exposure re-assign
float4  xposure(float4 _color , float gray , float ex)
{
      float b = ( 4 * ex -  1 );
      float a = 1 - b;
      float f = gray * ( a * gray + b );

      return   f * _color;
}

//Import lib name=libLighting

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
} //Finish import lib
//============================================


//Insert Node Declaration Node= D:\SVN\Evol3D\xEvolEngine\bin\shader\d3d1x\hlsl\simpleMesh.pixel.hlsl

cbuffer TransformBuffer
{
      matrix matWorld;
      matrix matView;
      matrix matProject;
	  float4 cameraUp;
	  float4 cameraPos;
	  float4 cameraDir;
	  float4 cameraArg;//[Near , Far , Fov , Aspect]
}

SamplerState DefaultSampler : register(s0);
Texture2D    DiffuseTexture : register(t0);
float        AlphaRef;
struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float4 Tan      : TANGENT;
      float2 Tex      : TEXCOORD; 

      float4 wPosition : TEXCOORD2;
      float4 wNormal   : TEXCOORD3;  
      float4 wTangent  : TEXCOORD4;   
};
//==================================================

//============================================

//Insert Node Code Node= D:\SVN\Evol3D\xEvolEngine\bin\shader\d3d1x\lib\simple.texture.hlsl

float4 COMMON_Texture(float4 _Color , float2 texCoord , SamplerState _sampler , Texture2D _texture )
{
      return _texture.Sample(_sampler, texCoord.xy) * _Color ;
}

//Insert Node Code Node= D:\SVN\Evol3D\xEvolEngine\bin\shader\d3d1x\lib\simple.Lighting.hlsl

float4 Simple_Lighting(float4 _Color , float4 wNormal , float4    wPosition , float4 eyePositon)
{
      return _Color * Phong_Lighting(wNormal.xyz , wPosition.xyz , eyePositon.xyz );
}
//============================================


//============================================
//Begin Main Node'code 

float4 main( PS_INPUT input) : SV_Target
{
    float4 vDiffuse =  float4(1.0,1.0,1.0,1.0);//input.Color ;

   //应用修改器 name=simple.texture 
   float4 Node_0_Ret = COMMON_Texture(vDiffuse , input.Tex , DefaultSampler , DiffuseTexture);

   //应用修改器 name=simple.Lighting 
   float4 Node_1_Ret = Simple_Lighting(Node_0_Ret , input.wNormal , input.wPosition , cameraPos);

   vDiffuse = Node_1_Ret; 

    if(vDiffuse.w <= AlphaRef)
       discard;
    return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , clamp(vDiffuse.w , 0.0  , 1.0) );
}
