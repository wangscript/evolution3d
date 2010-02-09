
float4x4 normalMatrix(float4x4 mat)
{
      float4x4 matNormal = mat;//transpose(mat);
      matNormal[3] = float4(0.0f,0.0f,0.0f,1.0f);
      matNormal[0][3] = 0.0f; 
      matNormal[1][3] = 0.0f;  
      matNormal[2][3] = 0.0f;  
      return matNormal;
}