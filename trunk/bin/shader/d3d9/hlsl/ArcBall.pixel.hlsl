struct PS_INPUT
{      
      float4 vPos     : TEXCOORD0;
      float4 Pos      : TEXCOORD1;
};

float4 main( PS_INPUT input) : COLOR0
{
      float4 cRet = float4(1.0f,1.0f,1.0f,1.0f);
      if(input.vPos.x == 0.0)
            cRet =  float4(1.0f,0.0f,0.0f,1.0f);
      if(input.vPos.y == 0.0)
            cRet =  float4(0.0f,1.0f,0.0f,1.0f);

      return cRet;
}

