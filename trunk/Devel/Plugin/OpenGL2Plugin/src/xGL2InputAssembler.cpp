#include "xOpenGL2API.h"
#include "xGL2InputAssembler.h"
#include "xGL2InputBuffer.h"

#include <string>
#include <d3dx10.h>
using namespace std;
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xGL2InputAssembler , IInputAssembler);

xGL2InputAssembler::xGL2InputAssembler(IRenderApi* pRenderer , const wchar_t* name ,xInputLayoutDesc& desc)
:m_pRenderApi(pRenderer) , IInputAssembler(pRenderer) , m_Name(name)
{
    m_RefCount = 1;
	m_HashName = xStringHash(name);
	m_Desc = desc;
	m_pRenderApi->objectManager().inputAssemblerMgr().add( name , this);
}

xGL2InputAssembler::~xGL2InputAssembler()
{
	m_pRenderApi->objectManager().inputAssemblerMgr().remove( this );
	
	if( m_D10InputLayout ) m_D10InputLayout->Release();
}

const xInputLayoutDesc& xGL2InputAssembler::desc() const
{
	return m_Desc;
}

const wchar_t* xGL2InputAssembler::name() const
{
	return m_Name.c_str();
}

const size_t xGL2InputAssembler::hash_name() const
{
	return m_HashName;
}

bool xGL2InputAssembler::create()
{
	 return true;
}


IVertexStream* xGL2InputAssembler::createVertexStream()
{
	return new xGL2VertexStream(m_pRenderApi , this);
}

END_NAMESPACE_XEVOL3D

