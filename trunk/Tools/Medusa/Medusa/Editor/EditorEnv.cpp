#include "Stdafx.h"
#include "EditorEnv.h"
#include <windows.h>
#include "xMedusaEditorImpl.h"
CEditorEnv::~CEditorEnv()
{

}
CEditorEnv::CEditorEnv()
{
	m_bInited = false;
	m_hRenderWindow = NULL;
	m_Evol3DEnv = new CEvolEnviroment();
}

void CEditorEnv::AddDocument(CDocument* pDocument)
{
	m_vDocuments.push_back(pDocument);
}

const wchar_t* CEditorEnv::sysCfgfile()
{
	if(m_Evol3DEnv == NULL)
		return NULL;
	return m_Evol3DEnv->sysCfgfile();
}

bool CEditorEnv::onMessage(xWindowMsg& msg) 
{ 
	if(m_Evol3DEnv == NULL)
		return false;
	return m_Evol3DEnv->onMessage(msg) ; 
}

bool CEditorEnv::messageLoop()
{
    m_Evol3DEnv->run();
	return true;
}

bool CEditorEnv::init()
{
	return m_Evol3DEnv->init();
}

void CEditorEnv::initRenderer(HWND hRenderWindow)
{
	if(hRenderWindow ) 
		m_hRenderWindow = hRenderWindow;
	m_bInited = true; 
	m_Evol3DEnv->initRenderer(m_hRenderWindow);
}



void CEditorEnv::OnIdle()
{
	if(m_Evol3DEnv == NULL)
		return ;
	MSG msg;
	//long  thisTick;
	static long  lastTick = GetTickCount();
	long thisTickt = GetTickCount();
	long passedTime = thisTickt - lastTick;
	lastTick = thisTickt;
	m_Evol3DEnv->prevUpdateFrame(passedTime);
	m_Evol3DEnv->updateFrame(passedTime);
	m_Evol3DEnv->postUpdateFrame(passedTime);
	return ;
}

bool CEditorEnv::bInited()
{
	return m_bInited;
}

void CEditorEnv::Uninit()
{
	m_Evol3DEnv->exit();
	delete m_Evol3DEnv;
}


xXmlNode* CEditorEnv::sysConfig()
{
	return m_sysConfig.root();
}

xXmlNode* CEditorEnv::sysCfgNode(const wchar_t* sector , bool bCreated)
{
	xXmlNode* pNode = m_sysConfig.root();
	if(pNode == NULL)
	{
		pNode = m_sysConfig.insertNode(L"root");
	}
	if(sector == NULL)
		return pNode;
	pNode =  m_sysConfig.root()->findNode(sector);
	if(pNode == NULL && bCreated)
	{
		pNode = m_sysConfig.root()->insertNode(sector);
	}
	return pNode;

}

int CEditorEnv::sysValue(const wchar_t* sector , const wchar_t* _value, int defValue)
{
	xXmlNode* pNode = sysCfgNode(sector , true);

	xXmlValue* pValue = pNode->findValue(_value);
	if(pValue == NULL ) 
	{
		pValue = pNode->insertValue(_value);
		pValue->setValue(defValue);
		return defValue;
	}
	return pValue->int_value();
}

const wchar_t* CEditorEnv::sysValue(const wchar_t* sector , const wchar_t* _value, const wchar_t* defValue)
{
	xXmlNode* pNode = sysCfgNode(sector , true);

	xXmlValue* pValue = pNode->findValue(_value);
	if(pValue == NULL ) 
	{
		pValue = pNode->insertValue(_value);
		pValue->setValue(defValue);
		return defValue;
	}
	return pValue->value();
}

float CEditorEnv::sysValue(const wchar_t* sector , const wchar_t* _value, float defValue)
{
	xXmlNode* pNode = sysCfgNode(sector , true);

	xXmlValue* pValue = pNode->findValue(_value);
	if(pValue == NULL ) 
	{
		pValue = pNode->insertValue(_value);
		pValue->setValue(defValue);
		return defValue;
	}
	return pValue->float_value();
}

bool CEditorEnv::sysValue(const wchar_t* sector , const wchar_t* _value, bool defValue)
{
	xXmlNode* pNode = sysCfgNode(sector , true);

	xXmlValue* pValue = pNode->findValue(_value);
	if(pValue == NULL ) 
	{
		pValue = pNode->insertValue(_value);
		pValue->setValue(defValue);
		return defValue;
	}
	return pValue->bool_value();
}

void CEditorEnv::setSysValue(const wchar_t* sector , const wchar_t* _value, int defValue)
{
	xXmlNode* pNode = sysCfgNode(sector , true);

	xXmlValue* pValue = pNode->findValue(_value);
	if(pValue == NULL ) 
	{
		pValue = pNode->insertValue(_value);
		pValue->setValue(defValue);
		return ;
	}
	pValue->setValue(defValue);
	return ;
}

void CEditorEnv::setSysValue(const wchar_t* sector , const wchar_t* _value, const wchar_t* defValue)
{
	xXmlNode* pNode = sysCfgNode(sector , true);

	xXmlValue* pValue = pNode->findValue(_value);
	if(pValue == NULL ) 
	{
		pValue = pNode->insertValue(_value);
		pValue->setValue(defValue);
		return ;
	}
	pValue->setValue(defValue);
	return ;
}

void CEditorEnv::setSysValue(const wchar_t* sector , const wchar_t* _value, float defValue)
{
	xXmlNode* pNode = sysCfgNode(sector , true);

	xXmlValue* pValue = pNode->findValue(_value);
	if(pValue == NULL ) 
	{
		pValue = pNode->insertValue(_value);
		pValue->setValue(defValue);
		return ;
	}
	pValue->setValue(defValue);
	return ;
}

void CEditorEnv::setSysValue(const wchar_t* sector , const wchar_t* _value, bool defValue)
{
	xXmlNode* pNode = sysCfgNode(sector , true);

	xXmlValue* pValue = pNode->findValue(_value);
	if(pValue == NULL ) 
	{
		pValue = pNode->insertValue(_value);
		pValue->setValue(defValue);
		return ;
	}
	pValue->setValue(defValue);
	return ;
}


