int4 ucolor_id;
float2 ToFloat2( uint v )
{
      uint h = v/256;
      return float2(frac(v/256.0f),  h/256.0);
}

float4 main() : SV_Target
{  
      float2 v1 = ToFloat2(ucolor_id.x);
      float2 v2 = ToFloat2(ucolor_id.y);
      return  float4(v1.x, v1.y , v2.x, v2.y);
}
