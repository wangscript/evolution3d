
cbuffer cbImmutable
{

    float3 g_vLightDir = {-0.3,0.9056,-0.3};
};

matrix matWorld;
matrix matView;
matrix matProject;

struct GS_INPUT
{
    float4 pos      : SV_POSITION;
	float4 norm     : NORMAL;
    float4 dir      : TEXCOORD0;
    float4 Color    : COLOR;
};

struct PS_INPUT
{
      float4 pos       : SV_POSITION;
//      float4 norm      : NORMAL;
      float4 Color     : COLOR;
      float2 tex       : TEXCOORD; 

      float4 wPosition : TEXCOORD2;
      float4 wNormal   : TEXCOORD3;  
};

void GSOutputPoint( float4 inputPos, float4 wPos, float3 deltaPos, int sideNum, int i, float ty, inout TriangleStream<PS_INPUT> TriangleOutputStream )
{
    PS_INPUT output = (PS_INPUT)0;
	output.pos  = wPos;
//	output.norm = inputNormal;

	float fRad = 3.0f;

	output.pos.x += deltaPos.x * fRad;
	output.pos.y += deltaPos.y * fRad;
	output.wPosition = output.pos;

	output.pos = mul( matView    , output.pos);
    output.pos = mul( matProject , output.pos);


	output.tex.x = float(i)/float(sideNum);
    output.tex.y = ty;


	output.wNormal.xyz   = deltaPos;
	output.wNormal.w     = 1.0f;

    TriangleOutputStream.Append(output);
}

//
// GS for rendering the pipes.
//
[maxvertexcount(56)]
void main(line GS_INPUT input[2], inout TriangleStream<PS_INPUT> TriangleOutputStream)
{
        //
        // setup the coordinate systems for each point
        //
		int nSideNum = 8;
		int nPointNum = nSideNum + 1;

		float pi = 3.14159265358979323846f;
		float fAngle = (pi * 2.0f) / float(nSideNum);

		float4 pos0 = mul( matView    , input[0].pos);
        pos0 = mul( matProject , pos0);

		float4 pos1 = mul( matView    , input[1].pos);
        pos1 = mul( matProject , pos1);

		float3 deltaPos;
		float fCurrentAngle;

		//// º∆À„÷ÿ∏¥Ã˘Õº
		float dx = pos0.x - pos1.x;
		float dy = pos0.y - pos1.y;
		float dz = pos0.z - pos1.z;
		float fDis = dx * dx + dy * dy + dz * dz;
		fDis = sqrt(fDis);

		fAngle = pi*2.0f /(float)nSideNum;
		for(int i=0; i< nPointNum; i++)
        {
			deltaPos.x = cos(fAngle * i);
			deltaPos.y = sin(fAngle * i);
			deltaPos.z = 0.0f;
			if( (nPointNum - 1) == i )
			{
			   deltaPos.x = cos(pi*2.0f);
			   deltaPos.y = sin(pi*2.0f);
			}

            GSOutputPoint( pos0,  input[0].pos, deltaPos, nSideNum, i, 0.0, TriangleOutputStream );
            GSOutputPoint( pos1,  input[1].pos, deltaPos, nSideNum, i, fDis/50.0, TriangleOutputStream );
        }
//		TriangleOutputStream.RestartStrip();


}