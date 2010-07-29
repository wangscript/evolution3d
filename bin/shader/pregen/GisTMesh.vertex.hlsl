
SamplerState   DefaultClampSampler        ;
Texture2D      DEMTextures[9]             ;

cbuffer TransformBuffer
{
      float4x4 matWorld;
      float4x4 matView;
      float4x4 matProject;
      float4x4 matTexture;
	  float4   cameraUp;
	  float4   cameraPos;
	  float4   cameraDir;
	  float4   cameraArg;//[Near , Far , Fov , Aspect]
}; 

cbuffer xGisTerrainMeshArg
{
      float4  TerrainScale;
      float4  CameraPos;
	  float4  DEMRegion[9];
      float4  ImageRegion[9];
	  int4    nDEM;
};

struct VS_INPUT
{
      float3 Pos      : POSITION;
};

struct PS_INPUT
{
      float4 Pos       : SV_POSITION;
      float4 Nor       : NORMAL;
      float4 Color     : COLOR;
      float4 Tan       : TANGENT;
      float4 Tex       : TEXCOORD;  

      float4 wPosition : TEXCOORD2;
      float4 wNormal   : TEXCOORD3;  
      float4 wTangent  : TEXCOORD4;
};

float _LoDScale(float v)
{
     return v;
     float _ret = v;
     float len = abs(v);
     if(v > 0 )  return (( v + 1.0f ) * v *  1.0);
     if(v <= 0 ) return (( v - 1.0f ) * v * -1.0);
}

PS_INPUT main( VS_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
	  float3 _Pos = input.Pos.xyz;
      _Pos.x = _LoDScale(_Pos.x);//// * _Pos.x;
      _Pos.y = _LoDScale(_Pos.y);//// * _Pos.y;
      _Pos = float3(CameraPos.xy , 0.0) +  TerrainScale.xyz * _Pos;

      float4 _posInWorld = mul( matWorld   , float4(_Pos ,1) );
      _Pos = _posInWorld.xyz;
	  
      float _bitFlag = 1.02f;
	  float _h = 0.0f;
	  float div = 0.0f;
	  for(int i = 0 ; i < nDEM.x ; i ++ )
	  {
	        if( _Pos.x >= DEMRegion[i].x  && _Pos.x <= DEMRegion[i].z &&
			    _Pos.y >= DEMRegion[i].y  && _Pos.y <= DEMRegion[i].w  )
			{
				 float w = DEMRegion[i].z - DEMRegion[i].x;
				 float h = DEMRegion[i].w - DEMRegion[i].y;
				 float _x = _Pos.x - DEMRegion[i].x;
				 float _y = _Pos.y - DEMRegion[i].y;
				 float2 texCoord = float2( _x /w , _y / h ) ;

                 float _v = DEMTextures[i].SampleLevel(DefaultClampSampler, texCoord  , 0 ).x;
                 if(_v > 0 )//-3000000.0f)
                 {
                     div += 1.0f;
				     _h   += _v;
                 }
			}
	  }

      if(div > 0.0f)
	  {
          _Pos.z = _h *  TerrainScale.z / div ;
      }
      else
      {
          _Pos.z = 0.0f;
          _bitFlag = 0.0f;
      }


      
      output.Pos       = mul( matView    , float4(_Pos ,1) );
      output.Pos       = mul( matProject , output.Pos);
      output.wPosition = float4(_Pos, _bitFlag);
      output.wNormal   = float4(0,0,1,0);

      output.Nor      = float4(0  ,0  ,1.0,   0);
      output.Color    = float4(1.0,1.0,1.0, 1.0);
      output.Tan      = float4(0  ,0  ,1.0, 0  );
      output.Tex      = float4(0.5,0.5,1.0, 0  );
      output.wTangent = float4(0  ,0  ,1.0, 0  );
    return output;
}
