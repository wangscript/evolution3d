#include "xBaseShader.h"

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(IBaseShader , IRenderResource)
IMPL_BASE_OBJECT_CLASSID(IGpuProgram , IRenderResource)

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

IBaseShader::IBaseShader(IRenderApi* pOwner) : IRenderResource(pOwner)
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

IGpuProgram::IGpuProgram(IRenderApi* pOwner) : IRenderResource(pOwner)
{

}

IGpuProgram::~IGpuProgram()
{

}

END_NAMESPACE_XEVOL3D
