
cbuffer TransformBuffer
{
      matrix matWorld;
      matrix matView;
      matrix matProject;
	  float4 cameraUp;
	  float4 cameraPos;
	  float4 cameraDir;
	  float4 cameraArg;//[Near , Far , Fov , Aspect]
}

struct STATICMESHVS_INPUT
{
      float3 Pos      : POSITION;
      float2 UV       : TEXCOORD;
};


struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float2 Tex      : TEXCOORD;  

      float4 wPosition : TEXCOORD2;
      float4 wNormal   : TEXCOORD3;  
};

#define VS_INPUT STATICMESHVS_INPUT

PS_INPUT main( VS_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
      output.Pos   = mul( matWorld   , float4(input.Pos,1) );
      output.Pos   = mul( matView    , output.Pos);
      output.Pos   = mul( matProject , output.Pos);
      output.Nor   = float4(0,0,1,0);
      output.Color = float4(1,1,1,1);
      output.Tex   = input.UV;

      output.wPosition = mul( matWorld   , float4(input.Pos,1) );
      output.wNormal  = float4(0,0,1,0);

    return output;
}
