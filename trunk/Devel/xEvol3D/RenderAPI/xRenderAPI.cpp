#include  "xRenderApi.h"
#include  "xBaseShader.h"
#include  "xShaderName.h"
#include  "xShaderManager.h"
BEGIN_NAMESPACE_XEVOL3D

IRenderCamera::IRenderCamera()
{
	m_RefCount = 1;
}

IRenderCamera::~IRenderCamera()
{

}




IRenderApi::IRenderApi()
{
}

IRenderApi::~IRenderApi()
{
}


END_NAMESPACE_XEVOL3D
