/*
///{{Slot = CM_SteroRight
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

float4 CM_SteroRight(float4 vDiffuse )
{
      return float4(0 , vDiffuse.y , vDiffuse.z , vDiffuse.w);    
}

///}}}
