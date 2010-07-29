
float4x4 normalMatrix(float4x4 mat)
{
      float4x4 matNormal = mat;//transpose(mat);
      matNormal[3] = float4(0.0f,0.0f,0.0f,1.0f);
      matNormal[0][3] = 0.0f; 
      matNormal[1][3] = 0.0f;  
      matNormal[2][3] = 0.0f;  
      return matNormal;
}

float4 transNormal(float4x4 mat , float4 nr)
{
      float4 _nr = float4(nr.xyz , 0.0f);
      float4 _ret = mul(_nr  , mat );
      _ret.w = nr.w;
      return _ret;
}