//===============================================
//Begin import lib
//Finish import lib
//============================================


//Insert Node Declaration Node= k:\Projects\SVN\Evol3D\Devel\bin\shader\d3d10\simpleMesh.vertex.hlsl

matrix matWorld;
matrix matView;
matrix matProject;
struct STATICMESHVS_INPUT
{
    float4 Pos      : POSITION;
    float4 Nor      : NORMAL;
    float4 Color    : COLOR;
    float4 Tan      : TANGENT;
	float2 Tex      : TEXCOORD;  
};


struct PS_INPUT
{
    float4 Pos      : SV_POSITION;
    float4 Nor      : NORMAL;
    float4 Color    : COLOR;
    float4 Tan      : TANGENT;
	float2 Tex      : TEXCOORD;    
};

#define VS_INPUT STATICMESHVS_INPUT

//==================================================

//============================================
//============================================


//============================================
//Begin Main Node'code 

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( matWorld   , input.Pos );
    output.Pos = mul( matView    , output.Pos);
    output.Pos = mul( matProject , output.Pos);
	output.Tex   = input.Tex;
    output.Color = input.Color.xyzw;
    output.Tan   = input.Tan;
    output.Nor   = input.Nor;


   //Ó¦ÓÃÐÞ¸ÄÆ÷ name=SkinAni0 

   output.Pos = output.Pos; 
   output.Nor = output.Nor; 
   output.Tan = output.Tan; 
   output.Color = input.Color; 
   output.Tex = input.Tex; 

    return output;
}

