sampler2D YUY2Textue;
float4    TexSize;
struct PS_INPUT 
{
   float2 Texcoord :        TEXCOORD0;
};
float Mod2(int x)
{
	int x2 = x/2;
	float ix2  = x2;
	float fx2   = x/2.0;
	if(ix2 == fx2)
		return 0;
	return 1;	
}
float4 main( PS_INPUT Input ) : COLOR0
{      
 
   float3 yuvColor;
   float3 delYuv    = float3(-16.0/255.0 , -128.0/255.0 , -128.0/255.0);
   float2 TexCoord = Input.Texcoord;
   int texCoordX = TexCoord.x*TexSize.x*2;
   if( Mod2(texCoordX) > 0.0)
       yuvColor.x       = tex2D( YUY2Textue, TexCoord ).x;
   else
       yuvColor.x       = tex2D( YUY2Textue, TexCoord ).z;
   yuvColor.y       = tex2D( YUY2Textue, TexCoord ).w;
   yuvColor.z       = tex2D( YUY2Textue, TexCoord ).y;

   yuvColor += delYuv;
   float3 matYUVRGB1 = float3(1.164,  2.018 ,   0.0   );
   float3 matYUVRGB2 = float3(1.164, -0.391 , -0.813  );
   float3 matYUVRGB3 = float3(1.164,    0.0 ,  1.596  );

   float4 rgbColor;
   rgbColor.x = dot(yuvColor,matYUVRGB1);//yuvColor.x;//
   rgbColor.y = dot(yuvColor,matYUVRGB2);//yuvColor.x;//
   rgbColor.z = dot(yuvColor,matYUVRGB3);//yuvColor.x;//
   rgbColor.w = 1.0f;

   return rgbColor;
}
