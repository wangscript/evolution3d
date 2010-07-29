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

//Finish import lib
//============================================



SamplerState   DefaultClampSampler             ;
Texture2D      ImageTextures[9]           ;
float          fGisTerrainAlpha;
struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float4 Tan      : TANGENT;
      float4 Tex      : TEXCOORD;  

      float4 wPosition : TEXCOORD2;
      float4 wNormal   : TEXCOORD3;  
      float4 wTangent  : TEXCOORD4;
};

cbuffer xGisTerrainMeshArg
{
      float4  TerrainScale;
      float4  CameraPos;
	  float4  DEMRegion[9];
      float4  ImageRegion[9];
	  int4    nDEM;
};

float4 main( PS_INPUT input) : SV_Target
{
    if(input.wPosition.w < 1.0f)
        discard;

    float3 _Pos = input.wPosition.xyz;
    float4 vDiffuse =  float4(0.0,0.0,0.0,1.0);//input.Color ;
	//在这里计算地形的纹理贴图
    float div = 0.0f;
    for(int i = 0 ; i < nDEM.y ; i ++ )
	{
	        if( _Pos.x >= ImageRegion[i].x  && _Pos.x < ImageRegion[i].z &&
			    _Pos.y >= ImageRegion[i].y  && _Pos.y < ImageRegion[i].w  )
			{
			     div += 1.0f;

				 float w = ImageRegion[i].z - ImageRegion[i].x;
				 float h = ImageRegion[i].w - ImageRegion[i].y;
				 float _x = _Pos.x - ImageRegion[i].x;
				 float _y = _Pos.y - ImageRegion[i].y;
				 float2 texCoord = float2( _x /w , _y / h ) ;

				 vDiffuse   += ImageTextures[i].Sample(DefaultClampSampler, texCoord);
			}
	}
    if(div > 0.0f)
       vDiffuse /= div;
    else
    {
        vDiffuse.w = 0.0f;
        discard;
    }

    if(vDiffuse.w <0.01)
	    discard;
    return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , fGisTerrainAlpha );//clamp(vDiffuse.w , 0.0  , 1.0) );
}
