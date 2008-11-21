#include "xRenderer.h"
#include "xMaterial.h"
#include "BaseLib/xStringHash.h"
#include <map>
using namespace std;

BEGIN_NAMESPACE_XEVOL3D


typedef map<int , IRendererCreator*> CreatorLists;
static CreatorLists      g_CreatorList;

xRendererManager* xRendererManager::singleton()
{
     static xRendererManager g_RM;
	 return &g_RM;
}

IRenderer* xRendererManager::getRenderer()
{
	return m_pRenderer;
}

bool xRendererManager::registeRenderer(IRendererCreator* pRendererCreator)
{
	const wchar_t* name = pRendererCreator->name();
	int id = xStringHash(name);
	CreatorLists::iterator pos = g_CreatorList.find(id);
	if(g_CreatorList.end() != pos) return false;

	g_CreatorList.insert(CreatorLists::value_type(id , pRendererCreator));
	return true;
}

bool xRendererManager::unregisteRenderer(IRendererCreator* pRendererCreator)
{
	const wchar_t* name = pRendererCreator->name();
	int id = xStringHash(name);
	CreatorLists::iterator pos = g_CreatorList.find(id);
	if(g_CreatorList.end() == pos) return false;
	g_CreatorList.erase(pos);
	return true;
}

xRendererManager::xRendererManager()
{
	m_pRenderer = NULL;
}

IRenderer* xRendererManager::createRenderer(const wchar_t* name)
{
	int id = xStringHash(name);
	CreatorLists::iterator pos = g_CreatorList.find(id);
	if(g_CreatorList.end() == pos) return NULL;

	IRendererCreator* pCreator = pos->second;
	m_pRenderer =  pCreator->createRenderer();
	return m_pRenderer;
}


void IRenderer::deleteMaterial(xMaterial* pMaterial)
{
	pMaterial->KillObject();
}



END_NAMESPACE_XEVOL3D
