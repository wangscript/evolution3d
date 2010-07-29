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


//Insert Node Declaration Node= D:\SVN\Evol3D\CityDesignner\CityDesignner\..\..\xEvolEngine\bin\shader\d3d1x\hlsl\simple.pixel.hlsl

SamplerState DefaultSampler : register(s0);
Texture2D    DiffuseTexture : register(t0);
struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float4 Tex      : TEXCOORD;
      float4 Color    : COLOR;
};
//==================================================

//============================================

//Insert Node Code Node= D:\SVN\Evol3D\CityDesignner\CityDesignner\..\..\xEvolEngine\bin\shader\d3d1x\lib\simple.texture.hlsl

float4 COMMON_Texture(float4 _Color , float4 texCoord , SamplerState _sampler , Texture2D _texture )
{
      return _texture.Sample(_sampler, texCoord.xy) * _Color ;
}
//============================================


//============================================
//Begin Main Node'code 

float4 main( PS_INPUT input) : SV_Target
{
      float4 vDiffuse =  input.Color ;

   //Ó¦ÓÃÐÞ¸ÄÆ÷ name=simple.texture 
   float4 Ret_ColorModify_Node0 = COMMON_Texture(vDiffuse , input.Tex , DefaultSampler , DiffuseTexture);

   vDiffuse = Ret_ColorModify_Node0; 

       return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , clamp(vDiffuse.w , 0.0  , 1.0) );
}
