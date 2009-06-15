struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float2 Tex      : TEXCOORD;
      float4 Color    : COLOR;
};
struct VS_INPUT
{
      float4 Pos    : POSITION;
      float4 Color  : COLOR;
      float2 Tex    : TEXCOORD;
};

PS_INPUT main( VS_INPUT input )
{
      PS_INPUT outPut = (PS_INPUT) 0;
      float4 inPos = input.Pos;
      inPos.xy = sign( inPos.xy);
      outPut.Pos    = float4( inPos.xy, 0.0f, 1.0f);

      outPut.Tex    = float2(input.Tex.x , 1.0f-input.Tex.y) ;//(float2(outPut.Pos.x, - outPut.Pos.y) + 1.0f)/2.0f;
      outPut.Color  = input.Color;
      return outPut;
}
