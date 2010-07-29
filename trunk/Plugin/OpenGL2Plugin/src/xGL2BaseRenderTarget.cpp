#include "xOpenGL2API.h"
#include "xGL2BaseRenderTarget.h"
#include "xGL2BaseTexture.h"
BEGIN_NAMESPACE_XEVOL3D

xGL2BaseRenderTarget::xGL2BaseRenderTarget(xGL2RenderApi* pD10Api):IRenderTarget(pD10Api) 
{
	m_pGL2Api = pD10Api ; 
	m_pBaseTexture = NULL;
	m_RefCount  = 1;

}

bool xGL2BaseRenderTarget::grabRenderTagetData(int x , int y , int w , int h , void* pData)
{
	xGL2BaseTexture* pD10Texture = (xGL2BaseTexture*)m_pBaseTexture;
	return pD10Texture->grabRenderTagetData(x , y , w , h , pData);
}

void xGL2BaseRenderTarget::setTexture(IBaseTexture* pTexture)
{
	m_pBaseTexture = pTexture;
}

xGL2BaseRenderTarget::~xGL2BaseRenderTarget()
{

}


END_NAMESPACE_XEVOL3D


