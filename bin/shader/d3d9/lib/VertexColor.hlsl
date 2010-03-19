///{{Slot = COMMON_VertexColor
Input 
{
      float4    Color          : COLOR0;
	  float4    VertexColor    : VERTEXCOLOR;
}

Output 
{
      float4 color: COLOR0;
}
///}}


///{{SourceCode
float4 COMMON_VertexColor(float4 _Color , float4 _vColor)
{
      return _vColor * _Color ;
}
///}}
