#include "stdafx.h"
#include "xMedusaEditorImpl.h"
USING_NS_XEVOL3D;

typedef TFactoryManager<IMEdUIElementFactory , IMEdUIElement , int , int > CMEdUIElManagerImpl;
CMEdUIElManagerImpl* g_CMEdUIElManagerImpl = NULL;
CMEdUIElFactoryMgr*  CMEdUIElFactoryMgr::singleton()
{
	static CMEdUIElFactoryMgr g_CMEdUIElManager;
	return &g_CMEdUIElManager;
}

void  CMEdUIElFactoryMgr::_enusureSingleton()
{
	if(g_CMEdUIElManagerImpl == NULL)
		g_CMEdUIElManagerImpl = new CMEdUIElManagerImpl;
}

size_t CMEdUIElFactoryMgr::nFactories()
{
	_enusureSingleton(); return g_CMEdUIElManagerImpl->nFactories();

}

IMEdUIElement* CMEdUIElFactoryMgr::createInstance(const wchar_t* name )
{
	_enusureSingleton(); return g_CMEdUIElManagerImpl->createInstance(name,0,0);
}

IMEdUIElementFactory* CMEdUIElFactoryMgr::find(size_t idx)
{
	_enusureSingleton(); return g_CMEdUIElManagerImpl->find(idx);
}

IMEdUIElementFactory* CMEdUIElFactoryMgr::find(const wchar_t* name)
{
	_enusureSingleton(); return g_CMEdUIElManagerImpl->find(name);
}

bool CMEdUIElFactoryMgr::registe(IMEdUIElementFactory* object)
{
	_enusureSingleton(); return g_CMEdUIElManagerImpl->registe(object);
}

bool CMEdUIElFactoryMgr::unregiste(IMEdUIElementFactory* object)
{
	_enusureSingleton(); return g_CMEdUIElManagerImpl->unregiste(object);
}

