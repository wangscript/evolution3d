///{{Slot = Simple_Lighting
Input 
{
     float4    Color          : COLOR0;
     float4    wNormal        : WNORMAL;
     float4    wPosition      : WPOSITION;
     float4    eyePositon     : CAMERAPOSITION;
}

Output 
{
     float4 color: COLOR0;
}
///}}


///>>Import = libLighting

///{{SourceCode
float4 Simple_Lighting(float4 _Color , float4 wNormal , float4    wPosition , float4 eyePositon)
{
      return _Color * Phong_Lighting(wNormal.xyz , wPosition.xyz , eyePositon.xyz );
}
///}}
