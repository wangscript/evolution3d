#include "stdafx.h"
#include "xD10GPUProgram.h"
#include "xDirect3D10API.h"
#include <OperationSys/xOperationSys.h>
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD10GpuProgram   , IGpuProgram)
IMPL_BASE_OBJECT_CLASSID(xD10GpuProgParamTable , IGpuProgramParamTable);


xD10GpuProgParamTable::~xD10GpuProgParamTable()
{
    XSAFE_DELETE(m_pVSParamTable);
	XSAFE_DELETE(m_pPSParamTable);
	XSAFE_DELETE(m_pGSParamTable);
}

xD10GpuProgParamTable::xD10GpuProgParamTable(IGpuProgram* pGpuProgram)
{
	m_RefCount = 1;
	m_pVSParamTable = NULL;
	m_pPSParamTable = NULL;
	m_pGSParamTable = NULL;
	m_pGpuProgram = pGpuProgram;
}

IGpuProgram* xD10GpuProgParamTable::getGpuProgram()
{
	return m_pGpuProgram;
}

IShaderParamTable* xD10GpuProgParamTable::getShaderParamTab(eShaderType shType)
{
	if(shType == eShader_PixelShader) 
		return m_pPSParamTable;

	if(shType == eShader_VertexShader) 
		return m_pVSParamTable;

	if(shType == eShader_GeometryShader) 
		return m_pGSParamTable;
	return NULL;
}

bool xD10GpuProgParamTable::load(xXmlNode* pXMLNode)
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
	return false;
}


bool xD10GpuProgParamTable::updateRenderContext()
{
	if(m_pGSParamTable) m_pGSParamTable->updateRenderContext();
	if(m_pVSParamTable) m_pVSParamTable->updateRenderContext();
	if(m_pPSParamTable) m_pPSParamTable->updateRenderContext();
	return true;
}
bool  xD10GpuProgParamTable::commit()
{
	if(m_pGSParamTable) m_pGSParamTable->commit();
	if(m_pVSParamTable) m_pVSParamTable->commit();
	if(m_pPSParamTable) m_pPSParamTable->commit();
	return true;

}

bool xD10GpuProgParamTable::setTexture(const wchar_t* name    , IBaseTexture* pTexture , eShaderType _shader)
{
	int iTexName = xStringHash(name);
	return setTexture(iTexName , pTexture , _shader);
}

bool xD10GpuProgParamTable::setTexture(const int texName , IBaseTexture* pTexture , eShaderType _shader)
{
	switch(_shader)
	{
	case eShader_None:
		{
			if(m_pPSParamTable && m_pPSParamTable->setTexture(texName , pTexture) )
				return true;
			if(m_pVSParamTable && m_pVSParamTable->setTexture(texName , pTexture) )
				return true;
			if(m_pGSParamTable && m_pGSParamTable->setTexture(texName , pTexture) )
				return true;
			return false;
		}
		break;

	case eShader_PixelShader:
		{
			if(m_pPSParamTable && m_pPSParamTable->setTexture(texName , pTexture) )
				return true;
			return false;
		}
		break;

	case eShader_VertexShader:
		{
			if(m_pVSParamTable && m_pVSParamTable->setTexture(texName , pTexture) )
				return true;
			return false;
		}
		break;

	case eShader_GeometryShader:
		{
			if(m_pGSParamTable && m_pGSParamTable->setTexture(texName , pTexture) )
				return true;
			return false;
		}
		break;
	}
	return false;
}

xD10ShaderParamTable*  xD10GpuProgParamTable::getShaderParamTable(eShaderType shType)
{
	switch(shType)
	{
	case eShader_None       : return NULL;
    case eShader_PixelShader: return m_pPSParamTable;
	case eShader_VertexShader: return m_pVSParamTable;
	case eShader_GeometryShader: return m_pGSParamTable;
	default: return NULL;
	}
	return NULL;
}

bool xD10GpuProgParamTable::setTextureBySlot(const int iSlot      , IBaseTexture* pTexture , eShaderType _shader)
{
	if(_shader == eShader_None)
	{
			if(m_pPSParamTable && m_pPSParamTable->setTextureBySlot(iSlot , pTexture) )
				return true;
			if(m_pVSParamTable && m_pVSParamTable->setTextureBySlot(iSlot , pTexture) )
				return true;
			if(m_pGSParamTable && m_pGSParamTable->setTextureBySlot(iSlot , pTexture) )
				return true;
			return false;
	}
	else
	{
		xD10ShaderParamTable* pTable = getShaderParamTable(_shader);
		if(pTable == NULL) return false;
		pTable->setTextureBySlot(iSlot , pTexture);
		return true;
	}
}

bool xD10GpuProgParamTable::setParamater(const wchar_t* name , float  _value , eShaderType shType)
{
	return T_setParamater(name,_value,shType);
}

bool xD10GpuProgParamTable::setParamater(const wchar_t* name , int    _value , eShaderType shType)
{
	return T_setParamater(name,_value,shType);
}

bool xD10GpuProgParamTable::setParamater(const wchar_t* name , xvec4& _value , eShaderType shType)
{
	return T_setParamater(name,_value,shType);
}

bool xD10GpuProgParamTable::setParamater(const wchar_t* name , xmat4& _value , eShaderType shType )
{
	return T_setParamater(name,_value,shType);
}

bool xD10GpuProgParamTable::setParamater(const wchar_t* name , xvec4* _array , int n , int baseIndex, eShaderType shType)
{
	return T_setParamater(name,_array, n , baseIndex, shType);
}

bool xD10GpuProgParamTable::setParamater(const wchar_t* name , int*   _array , int n , int baseIndex, eShaderType shType)
{
	return T_setParamater(name,_array, n , baseIndex, shType);
}

bool xD10GpuProgParamTable::setParamater(const wchar_t* name , float* _array , int n , int baseIndex, eShaderType shType )
{
	return T_setParamater(name,_array, n , baseIndex, shType);
}

bool xD10GpuProgParamTable::setParamater(const wchar_t* name , xmat4* _array , int n , int baseIndex, eShaderType shType)
{
	return T_setParamater(name,_array, n , baseIndex, shType);
}


xD10GpuProgram::xD10GpuProgram(IRenderApi* pRenderApi):IGpuProgram(pRenderApi)
{
	m_RefCount = 1;
	m_defParamTable = NULL;
	m_pGeomShader   = NULL;
	m_pPixelShader  = NULL;
	m_pVertexShader = NULL;
	m_pInstalledTable = NULL;
}

xD10GpuProgram::~xD10GpuProgram()
{
	unload();

}
bool xD10GpuProgram::_afterLoad()
{
	XEvol_Convert(m_defParamTable ,  createParamTable(false) );
	//Load DefConst Table;
	return m_pVertexShader!= NULL || m_pPixelShader != NULL;
}


bool xD10GpuProgram::load(const wchar_t* shaderName[eShader_Max] , unsigned long arg) 
{
	const wchar_t* vsName = shaderName[eShader_VertexShader];
	const wchar_t* psName = shaderName[eShader_PixelShader];
	const wchar_t* gsName = shaderName[eShader_GeometryShader];
    if(vsName && wcslen(vsName) )
	{
		XEvol_Convert(m_pVertexShader ,  m_pRenderApi->createShader(vsName , eShader_VertexShader).getResource() );
		if(m_pVertexShader == NULL )
		{
			XEVOL_LOG(eXL_ERROR_FALT,L"Create Vertex shader failed: name=%s  \n", vsName );
		}
		else
		{
			XEVOL_LOG(eXL_DEBUG_TIPS,L"find vertex shader %s  \n", vsName );
		}
	}

	if(psName && wcslen(psName) )
	{
		XEvol_Convert(m_pPixelShader ,  m_pRenderApi->createShader(psName , eShader_PixelShader).getResource() );
		if(m_pPixelShader == NULL )
		{
			XEVOL_LOG(eXL_ERROR_FALT,L"Create Pixel shader failed: name=%s  \n", psName );
		}
		else
		{
			XEVOL_LOG(eXL_DEBUG_TIPS,L"find pixel shader %s  \n", psName );
		}
	}

	if(gsName && wcslen(gsName) )
	{
		XEvol_Convert(m_pGeomShader ,  m_pRenderApi->createShader(gsName , eShader_GeometryShader).getResource() );
		if(m_pGeomShader == NULL )
		{
			XEVOL_LOG(eXL_ERROR_FALT,L"Create geometry shader failed: name=%s  \n", gsName );
		}
		else
		{
			XEVOL_LOG(eXL_DEBUG_TIPS,L"find geometry shader %s  \n", gsName );
		}
	}
	return _afterLoad();
}

bool xD10GpuProgram::_load(xXmlNode* pNode )
{

	xXmlNode* pShaderNode =  pNode->findNode(RENDER_NAME,0);
	xXmlNode::XmlNodes nodes;
	pShaderNode->findNode(L"shader", nodes );
	xXmlNode* pShaderConstNode = pShaderNode->findNode(L"Constants");
	if(pShaderConstNode) 
		pShaderConstNode = pShaderNode->findNode(L"Paramaters");

	size_t _nNodes = nodes.size();
	for(size_t i = 0 ; i <  _nNodes; i ++)
	{
		xXmlNode* pShaderFileNode = nodes[i];
		const wchar_t* strFile = pShaderFileNode->value(L"file");
		XEVOL_ASSERT_POINTER(strFile , L"Shader node not find <file> value\n");
		const wchar_t* strType = pShaderFileNode->value(L"type");
		XEVOL_ASSERT_POINTER(strType , L"Shader node not find <type> value\n");

		ds_wstring type = strType;
		if(type == L"VertexShader")
		{
			XEVOL_LOG(eXL_DEBUG_TIPS,L"find vertex shader %s  \n",strFile );
			XEvol_Convert(m_pVertexShader ,  m_pRenderApi->createShader(strFile , eShader_VertexShader).getResource() );
		}
		else if(type == L"PixelShader")
		{
			XEVOL_LOG(eXL_DEBUG_TIPS,L"find pixel shader %s  \n",strFile );
			XEvol_Convert(m_pPixelShader  ,  m_pRenderApi->createShader(strFile , eShader_PixelShader).getResource() );

		}
		else if(type == L"GeomShader")
		{
			XEVOL_LOG(eXL_DEBUG_TIPS,L"find geometry shader %s  \n",strFile );
			XEvol_Convert(m_pGeomShader  ,   m_pRenderApi->createShader(strFile , eShader_GeometryShader).getResource() );
		}
	};

	bool ret =  _afterLoad();
	if(ret == true)  
		m_defParamTable->load(pShaderConstNode);
	return ret;
}


bool xD10GpuProgram::load(const wchar_t* fileName , unsigned long  arg)
{
    unload();
    xXmlDocument xmlDoc;
	if(false == xmlDoc.load(fileName,true) ) return false;
	return _load(xmlDoc.root());
}


bool xD10GpuProgram::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();
	xXmlDocument xmlDoc;

	if(false == xmlDoc.load( (const wchar_t*)buf, (int)bufLen , true) ) 
		return false;

	return _load(xmlDoc.root());
}

bool xD10GpuProgram::isLoaded()
{
	return m_pGeomShader!= NULL || m_pVertexShader != NULL || m_pPixelShader != NULL;
}

bool  xD10GpuProgram::unload()
{
	XSAFE_KILLOBJECT(m_defParamTable);
	return true;
}

IGpuProgramParamTable*  xD10GpuProgram::createParamTable(bool bLoadDef)
{
	xD10GpuProgParamTable* pGpuProgramTable = new xD10GpuProgParamTable(this); 

	if(m_pGeomShader  ) 
	{
		XEvol_Convert(pGpuProgramTable->m_pGSParamTable  ,   m_pGeomShader->createShaderParamTable() ); 
	}

	if(m_pPixelShader )
	{
		XEvol_Convert(pGpuProgramTable->m_pPSParamTable  ,  m_pPixelShader->createShaderParamTable() ); 
	}

	if(m_pVertexShader) 
	{
		XEvol_Convert(pGpuProgramTable->m_pVSParamTable  , m_pVertexShader->createShaderParamTable() ); 
	}

	if(bLoadDef)
	{
		//加载默认的数据
		assert(0);
		XEVOL_WARNNING_NOT_IMPLEMENT;
	}

	return pGpuProgramTable;
}

IGpuProgramParamTable*  xD10GpuProgram::getDefaultParamTable()
{
	return m_defParamTable;
}


void xD10GpuProgram::commit()
{
	xD10GpuProgParamTable* pD10GpuGonstTable = (xD10GpuProgParamTable*)getParamTable();
	pD10GpuGonstTable->updateRenderContext();
	pD10GpuGonstTable->commit();
}

bool xD10GpuProgram::setTexture(const wchar_t* texName , IBaseTexture* pTexture , eShaderType _shader)
{
	 return getParamTable()->setTexture(texName , pTexture , _shader);
}

bool xD10GpuProgram::setTexture(const int texName , IBaseTexture* pTexture , eShaderType _shader)
{
	return getParamTable()->setTexture(texName , pTexture , _shader);
}

bool xD10GpuProgram::setTextureBySlot(const int iSlot      , IBaseTexture* pTexture , eShaderType _shader)
{
	 return getParamTable()->setTextureBySlot(iSlot , pTexture , _shader);
}
END_NAMESPACE_XEVOL3D