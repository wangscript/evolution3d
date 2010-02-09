SamplerState DefaultSampler : register(s0);

Texture2D YVideoTexture;
Texture2D UVideoTexture;
Texture2D VVideoTexture;

struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float2 Tex      : TEXCOORD;
      float4 Color    : COLOR;
};

float4 main( PS_INPUT Input )  : SV_Target
{      

      float3 yuvColor;
      float3 delYuv    = float3(-16.0/255.0 , -128.0/255.0 , -128.0/255.0);
      yuvColor.x       = YVideoTexture.Sample(DefaultSampler , Input.Tex ).x;
      yuvColor.y       = UVideoTexture.Sample(DefaultSampler , Input.Tex ).x;
      yuvColor.z       = VVideoTexture.Sample(DefaultSampler , Input.Tex ).x;
      yuvColor += delYuv;
      float3 matYUVRGB1 = float3(1.164,  2.018 ,   0.0   );
      float3 matYUVRGB2 = float3(1.164, -0.391 , -0.813  );
      float3 matYUVRGB3 = float3(1.164,    0.0 ,  1.596  );

      float4 rgbColor;
      rgbColor.x = dot(yuvColor,matYUVRGB1);
      rgbColor.y = dot(yuvColor,matYUVRGB2);
      rgbColor.z = dot(yuvColor,matYUVRGB3);
      rgbColor.w = 1.0f;

      return rgbColor;
}
