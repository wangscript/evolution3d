/*
///{{Slot = CM_SteroLeft
Input 
{
float4    Color          : COLOR0;
}

Output 
{
     float4 color: COLOR0;
}
///}}
*/


///{{SourceCode

float4 CM_SteroLeft(float4 vDiffuse )
{
      return float4(vDiffuse.x , 0 , 0 , vDiffuse.w);    
}

///}}}
