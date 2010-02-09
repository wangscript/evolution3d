#ifndef _XEVOL_DIRECT3D11_INPUT_ASSEMBLER_H_
#define _XEVOL_DIRECT3D11_INPUT_ASSEMBLER_H_
#include <RenderApi/xRenderApi.h>
#include <string>
BEGIN_NAMESPACE_XEVOL3D
class xD3D11RenderApi;
class xD11InputAssembler : public IInputAssembler
{
	xD3D11RenderApi*          m_pRenderApi;
	std::wstring              m_Name;
	size_t                    m_HashName;
	xInputLayoutDesc          m_Desc;
	ID3D11InputLayout*        m_D11InputLayout;
	ID3D11VertexShader*       m_pDummyVertexShader;
	IMPL_BASE_OBJECT_INTERFACE(xD11InputAssembler);
public:
    xD11InputAssembler(xD3D11RenderApi* pRenderer , const wchar_t* name , xInputLayoutDesc& desc);
	~xD11InputAssembler();
	const xInputLayoutDesc& desc() const;
	const wchar_t*          name() const;
	const size_t            hash_name() const;
	bool                    create() ;
	IVertexStream*          createVertexStream();
	xInputLayoutDesc&       layoutDesc(){return m_Desc ; }

public:
	ID3D11InputLayout*      d11Layout(){ return m_D11InputLayout; }
protected:
	D3D11_INPUT_ELEMENT_DESC* _createInputElementDesc(xInputLayoutDesc  &   desc , std::wstring& inputDecl) ;
	void                      _releaseInputElementDesc(D3D11_INPUT_ELEMENT_DESC* pIED , int nElement);

};


END_NAMESPACE_XEVOL3D

#endif

