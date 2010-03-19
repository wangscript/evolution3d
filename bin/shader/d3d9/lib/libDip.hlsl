
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

