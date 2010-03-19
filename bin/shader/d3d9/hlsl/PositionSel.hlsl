//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
///{{Declaration
struct PS_INPUT
{
      
      float4 Color     : COLOR;
      float2 Tex       : TEXCOORD0;  
      float4 Nor       : TEXCOORD1;
      float4 wPosition : TEXCOORD2;
      float4 wNormal   : TEXCOORD3;  
};
///}}

///{{SourceCode
float4 main( PS_INPUT input) : COLOR0
{
    return input.wPosition ;
}
///}}
