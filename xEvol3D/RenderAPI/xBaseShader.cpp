#include "../xStdPch.h"
#include "xBaseShader.h"

BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(IBaseShader , IRenderApiResource)
IMPL_BASE_OBJECT_CLASSID(IGpuProgram , IRenderApiResource)
IMPL_BASE_OBJECT_CLASSID(IInputBufferReflection , IBaseObject)
IMPL_BASE_OBJECT_CLASSID(IShaderParamTable      , IBaseObject)
IMPL_BASE_OBJECT_CLASSID(IGpuProgramParamTable  , IBaseObject)

bool IShaderConstantReflection::updateData()
{
	if(m_pBinder) 
		return m_pBinder->updateConstant(this);
	return false;
};


IShaderParamTable::~IShaderParamTable()
{

}

IShaderParamTable::IShaderParamTable()
{
	this->m_RefCount = 1;
}

IBaseShader::IBaseShader(IRenderApi* pOwner) : IRenderApiResource(pOwner)
{

}

IBaseShader::~IBaseShader()
{

}

IGpuProgramParamTable::~IGpuProgramParamTable()
{

}

IGpuProgramParamTable::IGpuProgramParamTable()
{
	this->m_RefCount = 1;
}

IGpuProgram::IGpuProgram(IRenderApi* pOwner) : IRenderApiResource(pOwner)
{

}

IGpuProgram::~IGpuProgram()
{

}

END_NAMESPACE_XEVOL3D
