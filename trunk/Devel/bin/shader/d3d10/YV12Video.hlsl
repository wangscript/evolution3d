//一个用来演示的Shader基础类, PixelShader

///{{Declaration
//视频处理的PS
sampler2D YTextue; 
sampler2D UTextue;
sampler2D VTextue;
float     cTime;
struct PS_INPUT 
{
   float2 Texcoord :        TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 rgbColor : COLOR0;
};
///}}

/*
///{{ParamTable //Shader的参数描述表
   texture   YTexture : <file="" , manager="" , width="" , height="" /> ;
   float     cTime    : TIME ;
///}}
*/

///{{SourceCode
PS_OUTPUT main( PS_INPUT Input )
{      
   //===采样输入器=====================================
   float4 yuvColor;
   yuvColor.x       = tex2D( YTextue, Input.Texcoord ).x;
   yuvColor.y       = tex2D( UTextue, Input.Texcoord ).x;
   yuvColor.z       = tex2D( VTextue, Input.Texcoord ).x;
   yuvColor.w       = 1.0f;
   PS_OUTPUT RetVal  ;
   //================================================== 
///}}

/*
///{{Socket = ColorModify // Socket = 名字, 在这里定义一个插入点
   Input
   {
       float4 yuvColor  : YUVCOLOR;
   }
   Output var = RetVal
   {
       float4  rgbColor : COLOR0;
   }
///}}
*/

///{{DefaultCode  = ColorModify 
   //默认的代码 ，如果Name指定的插槽没有插入Node。则该代码才会被编译进去
   RetVal.rgbColor = float4(yuvColor,1.0);
///}}

///{{SourceCode   
   //==================================================
   return RetVal;
}
///}}