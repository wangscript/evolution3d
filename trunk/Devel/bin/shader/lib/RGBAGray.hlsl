/*
///{{Slot = CM_Gray
Input 
{
    float4 ColorIn  : COLOR0; // 类型 名字 : 语义
}

Output class = CM_GrayRet
{
   float4 color: COLOR0;
}
///}}
*/

///{{Declaration
struct CM_GrayRet
{
	float4 color;
};

CM_GrayRet CM_Gray(float4 ColorIn);
///}}

///{{SourceCode
CM_GrayRet CM_Gray(float4 ColorIn)
{
   CM_GrayRet ret;
   float gray = 0.3 * ColorIn.x + 0.59 * ColorIn.y + 0.11 * ColorIn.z;
   ret.color = float4(gray,gray,gray,ColorIn.w);
   return ret;
}
///}}