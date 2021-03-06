//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
///{{Declaration
struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float4 Tan      : TANGENT;
      float4 Tex      : TEXCOORD;  

      float4 wPosition : TEXCOORD2;
      float4 wNormal   : TEXCOORD3;  
      float4 wTangent  : TEXCOORD4;
};

///}}

///{{SourceCode
float4 main( PS_INPUT input) : SV_Target
{
    return input.wPosition ;
}
///}}
