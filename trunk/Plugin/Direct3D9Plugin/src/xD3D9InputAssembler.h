#ifndef _XEVOL_DIRECT3D9_INPUT_ASSEMBLER_H_
#define _XEVOL_DIRECT3D9_INPUT_ASSEMBLER_H_
#include <RenderApi/xRenderApi.h>
#include <string>
BEGIN_NAMESPACE_XEVOL3D
class xD3D9RenderApi;
class xD3D9InputAssembler : public IInputAssembler
{
	xD3D9RenderApi*               m_pRenderApi;
	std::wstring                  m_Name;
	size_t                        m_HashName;
	xInputLayoutDesc              m_Desc;
	IDirect3DVertexDeclaration9*  m_D3DVertexDecl;
	IMPL_BASE_OBJECT_INTERFACE(xD3D9InputAssembler);
public:
    xD3D9InputAssembler(xD3D9RenderApi* pRenderer , const wchar_t* name , xInputLayoutDesc& desc);
	~xD3D9InputAssembler();
	const xInputLayoutDesc& desc() const;
	const wchar_t*          name() const;
	const size_t            hash_name() const;
	bool                    create() ;
	IVertexStream*          createVertexStream();
	xInputLayoutDesc&       layoutDesc(){return m_Desc ; }

public:
	IDirect3DVertexDeclaration9*      d3d9Layout(){ return m_D3DVertexDecl; }
protected:
	D3DVERTEXELEMENT9* _createInputElementDesc(xInputLayoutDesc  &   desc) ;
	void                _releaseInputElementDesc(D3DVERTEXELEMENT9* pIED , int nElement);

};


END_NAMESPACE_XEVOL3D

#endif

