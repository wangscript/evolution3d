#include "stdafx.h"
#include "xDIRECT3D9API.h"
#include "xD3D9InputAssembler.h"
#include "xD3D9InputBuffer.h"
#include "xD3DInputBuffer.h"
#include <string>
using namespace std;

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD3D9InputAssembler , IInputAssembler);


static wchar_t * dummyShader = L"\nfloat4 main( VS_INPUT input ) : SV_POSITION  \n{\n    return input.Pos; \n}\n ";

xD3D9InputAssembler::xD3D9InputAssembler(xD3D9RenderApi* pRenderer , const wchar_t* name ,xInputLayoutDesc& desc)
:m_pRenderApi(pRenderer) , IInputAssembler(pRenderer) , m_Name(name)
{
    m_RefCount = 1;
	m_HashName = xStringHash(name);
	m_Desc = desc;
	m_pRenderApi->objectManager().inputAssemblerMgr().add( name , this);
    m_D3DVertexDecl = NULL;
}

xD3D9InputAssembler::~xD3D9InputAssembler()
{
	m_pRenderApi->objectManager().inputAssemblerMgr().remove( this );	
	XSAFE_RELEASE(m_D3DVertexDecl);
}

const xInputLayoutDesc& xD3D9InputAssembler::desc() const
{
	return m_Desc;
}

const wchar_t* xD3D9InputAssembler::name() const
{
	return m_Name.c_str();
}

const size_t xD3D9InputAssembler::hash_name() const
{
	return m_HashName;
}
#define SHADER_SEMANTIC_POSITION      (wchar_t*)L"POSITION"
#define SHADER_SEMANTIC_NORMAL        (wchar_t*)L"NORMAL"
#define SHADER_SEMANTIC_WEIGHT        (wchar_t*)L"BLENDWEIGHT"
#define SHADER_SEMANTIC_BLENDINDICES  (wchar_t*)L"BLENDINDICES"
#define SHADER_SEMANTIC_TANGENT       (wchar_t*)L"TANGENT"
#define SHADER_SEMANTIC_BINORMAL      (wchar_t*)L"BINORMAL"
#define SHADER_SEMANTIC_TEXCOORD      (wchar_t*)L"TEXCOORD"
#define SHADER_SEMANTIC_COLOR         (wchar_t*)L"COLOR"
#define SHADER_SEMANTIC_ANY           (wchar_t*)L"ANY"
BYTE GetD9Usage(const wchar_t* semantic)
{
    std::wstring strSemantic = semantic;
    if(strSemantic == SHADER_SEMANTIC_POSITION    ) return D3DDECLUSAGE_POSITION  ;
    if(strSemantic == SHADER_SEMANTIC_WEIGHT      ) return D3DDECLUSAGE_BLENDWEIGHT    ;
    if(strSemantic == SHADER_SEMANTIC_BLENDINDICES) return D3DDECLUSAGE_BLENDINDICES   ;
    if(strSemantic == SHADER_SEMANTIC_NORMAL      ) return D3DDECLUSAGE_NORMAL         ; 
    if(strSemantic == SHADER_SEMANTIC_POINTSIZE   ) return D3DDECLUSAGE_PSIZE          ;
    if(strSemantic == SHADER_SEMANTIC_TEXCOORD    ) return D3DDECLUSAGE_TEXCOORD       ;
    if(strSemantic == SHADER_SEMANTIC_TANGENT     ) return D3DDECLUSAGE_TANGENT        ;
    if(strSemantic == SHADER_SEMANTIC_BINORMAL    ) return D3DDECLUSAGE_BINORMAL       ;
    if(strSemantic == SHADER_SEMANTIC_TESSFACTOR  ) return D3DDECLUSAGE_TESSFACTOR     ;
    if(strSemantic == SHADER_SEMANTIC_COLOR       ) return D3DDECLUSAGE_COLOR          ;
    return D3DDECLUSAGE_POSITION  ;
}

static void  conv(const xInputElement& from , D3DVERTEXELEMENT9& to)
{
	wstring decl = L"float4";
    to.Offset     = (UINT)from.m_offset;
    to.Stream     = (WORD)from.m_BufferIdx;
    to.UsageIndex = (BYTE)from.m_SemanticIdx;
    to.Usage      = GetD9Usage(from.m_Semantic);
    to.Method     = D3DDECLMETHOD_DEFAULT;//如果要使用Tesslator ,则可以用这个参数
	switch(from.m_Type)
	{
	case SHADERVARTYPE_FLOAT:
		to.Type = D3DDECLTYPE_FLOAT1 ;
		decl = L"float";
		break;
	case SHADERVARTYPE_FLOAT2:
		to.Type = D3DDECLTYPE_FLOAT2 ;
		decl = L"float2";
		break;
	case SHADERVARTYPE_FLOAT3:
		to.Type = D3DDECLTYPE_FLOAT3  ;
		decl = L"float4";
		break;
	case SHADERVARTYPE_FLOAT4:
		to.Type = D3DDECLTYPE_FLOAT4   ;
        decl = L"float4";
		break;

	case SHADERVARTYPE_HALF2:
		to.Type = D3DDECLTYPE_FLOAT16_2;
		decl = L"half2";
		break;

	case SHADERVARTYPE_HALF4:
		to.Type = D3DDECLTYPE_FLOAT16_4;
		decl = L"half4";
		break;

	//case SHADERVARTYPE_INT:
	//	to.Type = DXGI_FORMAT_R32_SINT ;
	//	decl = L"int";
	//	break;		
	//case SHADERVARTYPE_INT2:	
	//	to.Type = DXGI_FORMAT_R32G32_SINT ;
	//	decl = L"int2";
	//	break;		
	//case SHADERVARTYPE_INT3:	
	//	to.Type = DXGI_FORMAT_R32G32B32_SINT ;
	//	decl = L"int3";
	//	break;		
	//case SHADERVARTYPE_INT4:	
	//	to.Type = DXGI_FORMAT_R32G32B32A32_SINT  ;
	//	decl = L"int4";
	//	break;

	//case SHADERVARTYPE_UINT:
	//	to.Type = DXGI_FORMAT_R32_UINT ;
	//	decl = L"int";
	//	break;		
	//case SHADERVARTYPE_UINT2:	
	//	to.Type = DXGI_FORMAT_R32G32_UINT ;
	//	decl = L"int2";
	//	break;		
	//case SHADERVARTYPE_UINT3:	
	//	to.Type = DXGI_FORMAT_R32G32B32_UINT ;
	//	decl = L"int3";
	//	break;		
	//case SHADERVARTYPE_UINT4:	
	//	to.Type = D3DDECLTYPE_INT  ;
	//	decl = L"int4";
	//	break;

	case SHADERVARTYPE_SHORT2:
		to.Type = D3DDECLTYPE_SHORT2;
		decl = L"int2";
		break;
	case SHADERVARTYPE_SHORT4:
		to.Type = D3DDECLTYPE_SHORT4;
		decl = L"int4";
		break;

		
	case SHADERVARTYPE_USHORT2:
		to.Type = D3DDECLTYPE_USHORT2N ;
		decl = L"int2";
		break;		
	case SHADERVARTYPE_USHORT4:
		to.Type = D3DDECLTYPE_USHORT4N ;
		decl = L"int4";
		break;

	case SHADERVARTYPE_UBYTE4:
		to.Type = D3DDECLTYPE_UBYTE4 ;
		decl = L"int4";
		break;
	}
}

D3DVERTEXELEMENT9* xD3D9InputAssembler::_createInputElementDesc(xInputLayoutDesc  &   desc )
{
	const xInputLayoutDesc::xInputElements_t& elementDesc = desc.inputElements();
    D3DVERTEXELEMENT9* D3DElementDesc = new D3DVERTEXELEMENT9[ elementDesc.size() + 1];
    D3DVERTEXELEMENT9 _end = { 0xFF,0,D3DDECLTYPE_UNUSED,0,0,0} ;
    D3DElementDesc[ elementDesc.size() ] = _end;
	int numElements = (int)elementDesc.size();
	for(int i = 0 ; i < numElements ; i ++)
	{
		conv(elementDesc[i], D3DElementDesc[i]) ;
	}
	return D3DElementDesc;
}

void xD3D9InputAssembler::_releaseInputElementDesc(D3DVERTEXELEMENT9* pIED , int nElement)
{
	delete [] pIED;
}

bool xD3D9InputAssembler::create()
{
	 const xInputLayoutDesc::xInputElements_t& elementDesc = m_Desc.inputElements();
	 int numElements = (int)elementDesc.size();
	 D3DVERTEXELEMENT9* pVertexElements = NULL;//new D3D11_INPUT_ELEMENT_DESC[ elementDesc.size() ];
	 pVertexElements = _createInputElementDesc( m_Desc );

     IDirect3DDevice9* pD3DDevice = m_pRenderApi->getDevice();
     HRESULT hr = pD3DDevice->CreateVertexDeclaration(pVertexElements , &m_D3DVertexDecl);
	 _releaseInputElementDesc(pVertexElements , numElements);
	 if( FAILED( hr ) )
		 return false;
	 return true;
}


IVertexStream* xD3D9InputAssembler::createVertexStream()
{
     return new xD3DVertexStream(m_pRenderApi , this);
}

END_NAMESPACE_XEVOL3D

