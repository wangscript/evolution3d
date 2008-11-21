#include "xRenderApiManager.h"
#include <vector>
#include <map>
#include <string>
using namespace std;

BEGIN_NAMESPACE_XEVOL3D
vector<IRenderApiCreator*>& getRenderApiCreators()
{
	static vector<IRenderApiCreator*>* m_RenderApiCreators = NULL;
	if(m_RenderApiCreators == NULL)
		m_RenderApiCreators = new vector<IRenderApiCreator*>;
	return *m_RenderApiCreators;
}

xRendererAPIManager::xRendererAPIManager()
{

}

xRendererAPIManager::~xRendererAPIManager()
{

}

bool xRendererAPIManager::unregisteAPICreator(IRenderApiCreator* pRenderApiCreator)
{
	vector<IRenderApiCreator*>::iterator pos = getRenderApiCreators().begin();
	for(; pos != getRenderApiCreators().end() ; pos ++)
	{
		IRenderApiCreator* pCreator = *pos;
		if(  pRenderApiCreator == pCreator )
		{
			getRenderApiCreators().erase( pos );
			return true;
		}

	}
	return false;
}

bool xRendererAPIManager::registeAPICreator(IRenderApiCreator* pRenderApiCreator)
{
	IRenderApiCreator* pCreator = findAPICreator( pRenderApiCreator->renderName());
	if(pCreator == NULL)
	{
		XEVOL_LOG(eXL_DEBUG_NORMAL, L"  Register a Renderer creator : renderer name = %s\n", pRenderApiCreator->renderName() ) ;
		getRenderApiCreators().push_back(pRenderApiCreator);
		return true;
	}
	return false;
}

IRenderApiCreator*   xRendererAPIManager::findAPICreator(const wchar_t* rendererName)
{
	for(int i  = 0 ; i < (int)getRenderApiCreators().size() ; i ++)
	{
		IRenderApiCreator* pRC = getRenderApiCreators()[i];
		if(    wstring(rendererName) == wstring( pRC->renderName()   )  )
			return getRenderApiCreators()[i];
	}
	return NULL;
}

int xRendererAPIManager::numOfAPICreator()
{
	return (int)getRenderApiCreators().size();
}

IRenderApiCreator*   xRendererAPIManager::findAPICreator(int index)
{
	return getRenderApiCreators()[index];
}

END_NAMESPACE_XEVOL3D
