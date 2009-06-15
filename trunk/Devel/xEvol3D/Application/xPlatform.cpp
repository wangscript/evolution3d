/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "xPlatform.h"
#include <vector>
using namespace std;
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(IPlatform , IBaseObject)
IPlatform* g_pPlatformSingleTon = NULL;

bool IPlatform::dispatchMsg(xWindowMsg & msg)
{
	if(m_pEventTranslator) 
		m_pEventTranslator->translator(msg);

	if(m_pCurrentApplication)
	{
		if(true == m_pCurrentApplication->dispatchMsg(msg))
		   return true;
	}

	if(msg.m_pWndReciver != NULL)
		return msg.m_pWndReciver->dispatchMsg(msg);
	else
		return false;
	return false;

}


bool  IPlatform::setApplication(IApplication* pApp)
{
	m_pCurrentApplication = pApp;
	return true;
}

IApplication*  IPlatform::getApplication()
{

	return m_pCurrentApplication;
}

IPlatform::IPlatform()
{
	g_pPlatformSingleTon = this;
	m_pEventTranslator   = NULL;
}

IPlatform::~IPlatform()
{

}

IPlatform* IPlatform::singleton()
{
	return g_pPlatformSingleTon;
}

class CPlatformFactoryMgr
{
public:
	CPlatformFactoryMgr()
	{
		g_creaters.size();
	}
	vector<IPlatformFactory*> g_creaters;
};

static CPlatformFactoryMgr* g_PCMMgr = NULL;

int  IPlatformFactory::numOfPlatform()
{
	if(g_PCMMgr == NULL)
		g_PCMMgr = new CPlatformFactoryMgr;
	return (int)g_PCMMgr->g_creaters.size();
}

IPlatformFactory* IPlatformFactory::getByIndex(int i)
{
	if(g_PCMMgr == NULL)
		g_PCMMgr = new CPlatformFactoryMgr;
	if(i < 0 || i >= (int)g_PCMMgr->g_creaters.size())
		return NULL;
	return g_PCMMgr->g_creaters[i];
}


IPlatform* IPlatformFactory::create(const wchar_t*  name)
{
	if(g_PCMMgr == NULL)
		g_PCMMgr = new CPlatformFactoryMgr;

	for(size_t i = 0 ;  i <  g_PCMMgr->g_creaters.size() ; i ++)
	{
		if( std::wstring(g_PCMMgr->g_creaters[i]->name()) == name)
			return g_PCMMgr->g_creaters[i]->createInstance();
	}
	return NULL;
}

bool IPlatformFactory::registe(IPlatformFactory* creater)
{
	if(g_PCMMgr == NULL)
		g_PCMMgr = new CPlatformFactoryMgr;

	for(size_t i = 0 ;  i <  g_PCMMgr->g_creaters.size() ; i ++)
	{
		if( std::wstring( g_PCMMgr->g_creaters[i]->name() )== creater->name())
			return false;
	}
	g_PCMMgr->g_creaters.push_back(creater);
	return true;
}

END_NAMESPACE_XEVOL3D
