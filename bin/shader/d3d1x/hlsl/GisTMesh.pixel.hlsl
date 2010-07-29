//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
///{{Declaration

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

///}}

///>>Import = libDip

///{{SourceCode
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



///}}


    /*
///{{Default = ColorModify
    vDiffuse = float4(vDiffuse.xxx , 1.0);
///}}
    */

/*
///{{Socket = ColorModify // Socket = 名字, 在这里定义一个插入点
    Input
    {
          float4    vDiffuse        : COLOR0;
          float4    input.wPosition : WPOSITION;
          float4    input.wNormal   : WNORMAL;  

          Sampler   DefaultSampler  : SAMPLER;
          Texture2D DiffuseTexture  : TEXTURE0;
    }
    Output 
    {
          float4 vDiffuse : COLOR0;       
    }
///}}
*/

///{{SourceCode
    if(vDiffuse.w <0.01)
	    discard;
    return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , fGisTerrainAlpha );//clamp(vDiffuse.w , 0.0  , 1.0) );
}
///}}
