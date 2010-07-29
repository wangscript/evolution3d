//===============================================
//Begin import lib
//Import lib name=libDip

float4 dip_filter(float3x3 _filter ,  sampler2D _texture , float2 _xy, float2 texSize)
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
                  final_color += tex2D( _texture , _uv_new ) * _filter[i][j];
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


//Insert Node Declaration Node= D:\SVN\evol3d-google\Demo\RayTracerDemo\..\..\bin\shader\d3d9\hlsl\simple2D.pixel.hlsl

sampler2D    DiffuseTexture : register(s0);
struct PS_INPUT
{
      float4 Tex      : TEXCOORD0;
      float4 Tex1     : TEXCOORD1;
      float4 Color    : COLOR;
};
//==================================================

//============================================

//Insert Node Code Node= D:\SVN\evol3d-google\Demo\RayTracerDemo\..\..\bin\shader\d3d9\lib\simple.texture.hlsl

float4 COMMON_Texture(float4 _Color , float4 texCoord , sampler2D _texture )
{
      return tex2D(_texture, texCoord.xy) * _Color ;
}

//Insert Node Code Node= D:\SVN\evol3d-google\Demo\RayTracerDemo\..\..\bin\shader\d3d9\lib\simple.fakehdr.hlsl


float4 CM_FakeHDR(float4 vDiffuse , float4 texCoord , sampler2D _texture )
{
      float2    TexSize = float2(32 , 32);
      float2    intXY   = float2(texCoord.x * TexSize.x , texCoord.y * TexSize.y);

      float3x3 _smooth_fil = float3x3 (1/16.0 ,2/16.0,1/16.0 ,
                                       2/16.0 ,4/16.0,2/16.0 ,
                                       1/16.0 ,2/16.0,1/16.0 );

      float  _exposure  = 1.6;   
      float4 _dsColor   = dip_filter(_smooth_fil , _texture , intXY, TexSize);
      float  _lum       = 0.3 * _dsColor.x + 0.59 * _dsColor.y + 0.11 * _dsColor.z;
      return float4(xposure(vDiffuse , _lum , _exposure ).xyz , vDiffuse.w);    
}
//============================================


//============================================
//Begin Main Node'code 

float4 main( PS_INPUT input) : COLOR0
{
      float4 vDiffuse =  input.Color ;

   //应用修改器 name=simple.texture 
   float4 Ret_ColorModify_Node0 = COMMON_Texture(vDiffuse , input.Tex , DiffuseTexture);

   //应用修改器 name=simple.fakehdr 
   float4 Ret_ColorModify_Node1 = CM_FakeHDR(Ret_ColorModify_Node0 , input.Tex , DiffuseTexture);

   vDiffuse = Ret_ColorModify_Node1; 

      return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , clamp(vDiffuse.w , 0.0  , 1.0) );
}
