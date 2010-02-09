#ifndef _XEVOL_OPENGL20_INPUT_ASSEMBLER_H_
#define _XEVOL_OPENGL20_INPUT_ASSEMBLER_H_
#include <RenderApi/xRenderApi.h>
#include "../GL2.h"
#include <string>
BEGIN_NAMESPACE_XEVOL3D

class xGL2InputAssembler : public IInputAssembler
{
	IRenderApi*               m_pRenderApi;
	std::wstring              m_Name;
	size_t                    m_HashName;
	xInputLayoutDesc          m_Desc;
	IMPL_BASE_OBJECT_INTERFACE(xGL2InputAssembler);
public:
    xGL2InputAssembler(IRenderApi* pRenderer , const wchar_t* name , xInputLayoutDesc& desc);
	~xGL2InputAssembler();
	const xInputLayoutDesc& desc() const;
	const wchar_t*          name() const;
	const size_t            hash_name() const;
	bool                    create() ;
	IVertexStream*          createVertexStream();
	xInputLayoutDesc&       layoutDesc(){return m_Desc ; }
};


END_NAMESPACE_XEVOL3D

#endif

