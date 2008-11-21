//===============================================
//Begin import lib
//Finish import lib
//============================================


//Insert Node Declaration Node= p:\Projects\SVN\Evol3D\Devel\bin\shader\d3d10\simpleMesh.vertex.hlsl

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


//Insert Node Declaration Node= p:\Projects\SVN\Evol3D\Devel\bin\shader\d3d10\SkinMesh.hlsl

struct SKINMESHVS_INPUT
{
    float4 Pos      : POSITION;
    float4 Nor      : NORMAL;
    float4 Color    : COLOR;
    float4 Tan      : TANGENT;
	float4 Weights  : BLENDWEIGHT;
	int4   BoneIdxs : BLENDINDICES;
	float2 Tex      : TEXCOORD;  
};

struct VM_SkinRet
{
	float4 Pos;
	float4 Tan;
	float4 Nor;
};

VM_SkinRet VM_DoSkin(float4 Pos , float4 Nor , float4 Tan , float4 Weights , int4   BoneIdxs);

#define VS_INPUT SKINMESHVS_INPUT

//==================================================

//============================================

//Insert Node Code Node= p:\Projects\SVN\Evol3D\Devel\bin\shader\d3d10\SkinMesh.hlsl

VM_SkinRet VM_DoSkin(float4 Pos , float4 Nor , float4 Tan , float4 Weights , int4   BoneIdxs)
{
	VM_SkinRet ret;

	ret.Pos = mul( matWorld   ,     Pos );
	ret.Pos = mul( matView    , ret.Pos);
	ret.Pos = mul( matProject , ret.Pos);

	ret.Nor = Nor ;
	ret.Tan = Tan ;

	return ret;
}

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


   //Ó¦ÓÃÐÞ¸ÄÆ÷ name=SkinMesh 
   VM_SkinRet Node_0_Ret = VM_DoSkin(input.Pos , input.Nor , input.Color , input.Weights , input.BoneIdxs);

   output.Pos = Node_0_Ret.Pos; 
   output.Nor = Node_0_Ret.Nor; 
   output.Tan = Node_0_Ret.Tan; 
   output.Color = input.Color; 
   output.Tex = input.Tex; 

    return output;
}

