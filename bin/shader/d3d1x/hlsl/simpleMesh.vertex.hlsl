///{{Declaration
cbuffer TransformBuffer
{
      matrix matWorld;
      matrix matView;
      matrix matProject;
	  float4 cameraUp;
	  float4 cameraPos;
	  float4 cameraDir;
	  float4 cameraArg;//[Near , Far , Fov , Aspect]
}

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

      float4 wPosition : TEXCOORD2;
      float4 wNormal   : TEXCOORD3;  
      float4 wTangent  : TEXCOORD4;
};

#define VS_INPUT STATICMESHVS_INPUT
///}}

///>>Import = libMatrix

///{{SourceCode
PS_INPUT main( VS_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
      output.Pos   = mul( matWorld   , input.Pos );
      output.Pos   = mul( matView    , output.Pos);
      output.Pos   = mul( matProject , output.Pos);
      output.Tex   = input.Tex;
      output.Color = input.Color.xyzw;


	  float4x4 matModelView = matView * matWorld;
	  float4x4 matNormal = normalMatrix(matModelView);

	  //�۲�����ϵ�е�
      output.Tan   = mul(matNormal , input.Tan);
      output.Nor   = mul(matNormal , input.Nor);

   	  //��������ϵ�е�
	  matNormal = normalMatrix(matWorld);
      output.wPosition = mul(matWorld  , input.Pos );
      output.wNormal   = mul(matNormal , input.Nor);
      output.wTangent  = mul(matNormal , input.Tan);
///}}

/*
///{{Socket = VertexModifier // Socket = ����, �����ﶨ��һ�������
      Input
      {
            //�����λ�ú��з�����
            float4 input.Pos      : POSITION;
            float4 input.Nor      : NORMAL;
            float4 input.Tan      : TANGENT;

            //��������ռ�
            float4 input.Pos      : WPOSITION;
            float4 input.Nor      : WNORMAL;
            float4 input.Tan      : WTANGENT;

            //����任���ֵ
            float4 output.Pos     : TPOSITION;
            float4 output.Nor     : TNORMAL;
            float4 output.Tan     : TTANGENT;

            float4 input.Color    : COLOR;
            float4 input.Weights  : BLENDWEIGHT;
            int4   input.BoneIdxs : BLENDINDICES;
            float2 input.Tex      : TEXCOORD;  
            
            //Camera Infomation
            float4 cameraUp       : CAMERAUP ;
            float4 cameraPos      : CAMERAPOSITION ;
            float4 cameraDir      : CAMERADIR ;
            float4 cameraArg      : CAMERAARG ;
	  
      }
      Output 
      {
            //����任���ֵ
            float4 output.Pos      : TPOSITION;
            float4 output.Nor      : TNORMAL;
            float4 output.Tan      : TTANGENT;

            //����任�������ռ��λ��,�з�����
            float4 output.wPosition: WPOSITION;
            float4 output.wNormal  : WNORMAL;
            float4 output.wTangent : WTANGENT;

            //�������꣬��ɫ��
            float4 output.Color    : COLOR;
            float2 output.Tex      : TEXCOORD;   
      }
///}}
*/


///{{SourceCode 
    return output;
}
///}}