#ifndef _XEVOL_DIRECT3D10_INPUT_ASSEMBLER_H_
#define _XEVOL_DIRECT3D10_INPUT_ASSEMBLER_H_
#include <RenderApi/xRenderApi.h>
#include <d3d10.h>
#include <string>
BEGIN_NAMESPACE_XEVOL3D
class xD3D10RenderApi;
class xD10InputAssembler : public IInputAssembler
{
	xD3D10RenderApi*          m_pRenderApi;
	std::wstring              m_Name;
	size_t                    m_HashName;
	xInputLayoutDesc          m_Desc;
	ID3D10InputLayout*        m_D10InputLayout;
	IMPL_BASE_OBJECT_INTERFACE(xD10InputAssembler);
public:
    xD10InputAssembler(xD3D10RenderApi* pRenderer , const wchar_t* name , xInputLayoutDesc& desc);
	~xD10InputAssembler();
	const xInputLayoutDesc& desc() const;
	const wchar_t*          name() const;
	const size_t            hash_name() const;
	bool                    create() ;
	IVertexStream*          createVertexStream();
	xInputLayoutDesc&       layoutDesc(){return m_Desc ; }

public:
	ID3D10InputLayout*      d10Layout(){ return m_D10InputLayout; }
protected:
	D3D10_INPUT_ELEMENT_DESC* _createInputElementDesc(xInputLayoutDesc  &   desc , std::wstring& inputDecl) ;
	void                      _releaseInputElementDesc(D3D10_INPUT_ELEMENT_DESC* pIED , int nElement);

};


END_NAMESPACE_XEVOL3D

#endif

