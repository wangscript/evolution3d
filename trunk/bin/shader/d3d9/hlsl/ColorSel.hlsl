float4 fcolor_id;
float4 main() : COLOR0
{   
      float x = (float)(fcolor_id.x);// / 65536.0f;
      float y = (float)(fcolor_id.y);// / 65536.0f;
      float z = (float)(fcolor_id.z);// / 65536.0f;
      float w = (float)(fcolor_id.w);// / 65536.0f;

      return  float4(x, y , z, w);
}