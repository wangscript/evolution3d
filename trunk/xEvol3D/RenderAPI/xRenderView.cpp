#include "../xStdPch.h"
#include "xRenderView.h"

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(IRenderView   , IRenderApiObject);
IMPL_BASE_OBJECT_CLASSID(IRenderTarget , IRenderApiObject);
xRTSampleDesc xRTSampleDesc::Defualt = xRTSampleDesc(-1 , 0);
xRTSampleDesc xRTSampleDesc::None = xRTSampleDesc( 1 , 0 );
xRTSampleDesc::xRTSampleDesc(int Count , int Qulity)
{
	m_SampleCount = Count;
	m_SampleQulity = Qulity;
}


IRenderTarget::IRenderTarget(IRenderApi* pRenderApi)
:IRenderApiObject(pRenderApi)
{

}

IRenderTarget::~IRenderTarget()
{

}


IRenderView::IRenderView(IRenderApi* pRenderApi,const xRTSampleDesc& sampleDesc)
:IRenderApiObject(pRenderApi)
{
    m_RTSampleDesc = sampleDesc;
}

IRenderView::~IRenderView()
{

}

END_NAMESPACE_XEVOL3D
