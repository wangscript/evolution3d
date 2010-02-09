#include "xGL2GPUProgram.h"
#include "xOpenGL2API.h"
#include <OperationSys/xOperationSys.h>
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xGL2GpuProgram   , IGpuProgram)



xGL2GpuProgParamTable::~xGL2GpuProgParamTable()
{
    XSAFE_DELETE(m_pVSParamTable);
	XSAFE_DELETE(m_pPSParamTable);
	XSAFE_DELETE(m_pGSParamTable);
}

xGL2GpuProgParamTable::xGL2GpuProgParamTable(IGpuProgram* pGpuProgram)
{
	m_RefCount = 1;
	m_pVSParamTable = NULL;
	m_pPSParamTable = NULL;
	m_pGSParamTable = NULL;
	m_pGpuProgram = pGpuProgram;
}

IGpuProgram* xGL2GpuProgParamTable::getGpuProgram()
{
	return m_pGpuProgram;
}

IShaderParamTable* xGL2GpuProgParamTable::getShaderParamTab(eShaderType shType)
{
	if(shType == eShader_PixelShader) 
		return m_pPSParamTable;

	if(shType == eShader_VertexShader) 
		return m_pVSParamTable;

	if(shType == eShader_GeometryShader) 
		return m_pGSParamTable;
	return NULL;
}

bool xGL2GpuProgParamTable::load(xXmlNode* pXMLNode)
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
	return false;
}


bool xGL2GpuProgParamTable::updateRenderContext()
{
	if(m_pGSParamTable) m_pGSParamTable->updateRenderContext();
	if(m_pVSParamTable) m_pVSParamTable->updateRenderContext();
	if(m_pPSParamTable) m_pPSParamTable->updateRenderContext();
	return true;
}
bool  xGL2GpuProgParamTable::commit()
{
	if(m_pGSParamTable) m_pGSParamTable->commit();
	if(m_pVSParamTable) m_pVSParamTable->commit();
	if(m_pPSParamTable) m_pPSParamTable->commit();
	return true;

}

bool xGL2GpuProgParamTable::setTexture(const wchar_t* name    , IBaseTexture* pTexture , eShaderType _shader)
{
	int iTexName = xStringHash(name);
	return setTexture(iTexName , pTexture , _shader);
}

bool xGL2GpuProgParamTable::setTexture(const int texName , IBaseTexture* pTexture , eShaderType _shader)
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

xGL2ShaderParamTable*  xGL2GpuProgParamTable::getShaderParamTable(eShaderType shType)
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

bool xGL2GpuProgParamTable::setTextureBySlot(const int iSlot      , IBaseTexture* pTexture , eShaderType _shader)
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
		xGL2ShaderParamTable* pTable = getShaderParamTable(_shader);
		if(pTable == NULL) return false;
		pTable->setTextureBySlot(iSlot , pTexture);
		return true;
	}
}

bool xGL2GpuProgParamTable::setParamater(const wchar_t* name , float  _value , eShaderType shType)
{
	return T_setParamater(name,_value,shType);
}

bool xGL2GpuProgParamTable::setParamater(const wchar_t* name , int    _value , eShaderType shType)
{
	return T_setParamater(name,_value,shType);
}

bool xGL2GpuProgParamTable::setParamater(const wchar_t* name , xvec4& _value , eShaderType shType)
{
	return T_setParamater(name,_value,shType);
}

bool xGL2GpuProgParamTable::setParamater(const wchar_t* name , xmat4& _value , eShaderType shType )
{
	return T_setParamater(name,_value,shType);
}

bool xGL2GpuProgParamTable::setParamater(const wchar_t* name , xvec4* _array , int n , int baseIndex, eShaderType shType)
{
	return T_setParamater(name,_array, n , baseIndex, shType);
}

bool xGL2GpuProgParamTable::setParamater(const wchar_t* name , int*   _array , int n , int baseIndex, eShaderType shType)
{
	return T_setParamater(name,_array, n , baseIndex, shType);
}

bool xGL2GpuProgParamTable::setParamater(const wchar_t* name , float* _array , int n , int baseIndex, eShaderType shType )
{
	return T_setParamater(name,_array, n , baseIndex, shType);
}

bool xGL2GpuProgParamTable::setParamater(const wchar_t* name , xmat4* _array , int n , int baseIndex, eShaderType shType)
{
	return T_setParamater(name,_array, n , baseIndex, shType);
}


xGL2GpuProgram::xGL2GpuProgram(IRenderApi* pRenderApi):IGpuProgram(pRenderApi)
{
	m_RefCount = 1;
	m_defParamTable = NULL;
	m_pGeomShader   = NULL;
	m_pPixelShader  = NULL;
	m_pVertexShader = NULL;
	m_pInstalledTable = NULL;
}

xGL2GpuProgram::~xGL2GpuProgram()
{
	unload();

}
bool xGL2GpuProgram::_afterLoad()
{
	XEvol_Convert(m_defParamTable ,  createParamTable(false) );
	//Load DefConst Table;
	return m_pVertexShader!= NULL || m_pPixelShader != NULL;
}

bool xGL2GpuProgram::_load(xXmlNode* pNode )
{
	
    xXmlNode* pShaderNode =  pNode->findNode(RENDER_NAME,0);
	xXmlNode::XmlNodes nodes;
	pShaderNode->findNode(L"shader", nodes );
	xXmlNode* pShaderConstNode = pShaderNode->findNode(L"Constants");
	if(pShaderConstNode) 
		pShaderConstNode = pShaderNode->findNode(L"Paramaters");

	for(size_t i = 0 ; i < nodes.size() ; i ++)
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
			XEvol_Convert(m_pVertexShader ,  m_pRenderApi->createShader(strFile , eShader_VertexShader) );
		}
		else if(type == L"PixelShader")
		{
			XEVOL_LOG(eXL_DEBUG_TIPS,L"find pixel shader %s  \n",strFile );
			XEvol_Convert(m_pPixelShader  ,  m_pRenderApi->createShader(strFile , eShader_PixelShader) );

		}
		else if(type == L"GeomShader")
		{
			XEVOL_LOG(eXL_DEBUG_TIPS,L"find geometry shader %s  \n",strFile );
			XEvol_Convert(m_pGeomShader  ,   m_pRenderApi->createShader(strFile , eShader_GeometryShader) );
		}
	};
 
    bool ret =  _afterLoad();
	if(ret == true)  
		m_defParamTable->load(pShaderConstNode);
	return ret;
}
bool xGL2GpuProgram::load(const wchar_t* vsName , const wchar_t* psName, const wchar_t* gsName , unsigned long arg) 
{
    if(vsName && wcslen(vsName) )
	{
		XEvol_Convert(m_pVertexShader ,  m_pRenderApi->createShader(vsName , eShader_VertexShader) );
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
		XEvol_Convert(m_pPixelShader ,  m_pRenderApi->createShader(psName , eShader_PixelShader) );
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
		XEvol_Convert(m_pGeomShader ,  m_pRenderApi->createShader(gsName , eShader_GeometryShader) );
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

bool xGL2GpuProgram::load(const wchar_t* fileName , unsigned long  arg)
{
    unload();
    xXmlDocument xmlDoc;
	if(false == xmlDoc.load(fileName,true) ) return false;
	return _load(xmlDoc.root());
}

bool xGL2GpuProgram::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();
	xXmlDocument xmlDoc;

	if(false == xmlDoc.load( (const wchar_t*)buf, (int)bufLen , true) ) 
		return false;

	return _load(xmlDoc.root());
}

bool xGL2GpuProgram::isLoaded()
{
	return m_pGeomShader!= NULL || m_pVertexShader != NULL || m_pPixelShader != NULL;
}

bool  xGL2GpuProgram::unload()
{
	XSAFE_KILLOBJECT(m_defParamTable);
	return true;
}

IGpuProgramParamTable*  xGL2GpuProgram::createParamTable(bool bLoadDef)
{
	xGL2GpuProgParamTable* pGpuProgramTable = new xGL2GpuProgParamTable(this); 

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

IGpuProgramParamTable*  xGL2GpuProgram::getDefaultParamTable()
{
	return m_defParamTable;
}


void xGL2GpuProgram::commit()
{
	xGL2GpuProgParamTable* pD10GpuGonstTable = (xGL2GpuProgParamTable*)getParamTable();
	pD10GpuGonstTable->updateRenderContext();
	pD10GpuGonstTable->commit();
}

bool xGL2GpuProgram::setTexture(const wchar_t* texName , IBaseTexture* pTexture , eShaderType _shader)
{
	 return getParamTable()->setTexture(texName , pTexture , _shader);
}

bool xGL2GpuProgram::setTexture(const int texName , IBaseTexture* pTexture , eShaderType _shader)
{
	return getParamTable()->setTexture(texName , pTexture , _shader);
}

bool xGL2GpuProgram::setTextureBySlot(const int iSlot      , IBaseTexture* pTexture , eShaderType _shader)
{
	 return getParamTable()->setTextureBySlot(iSlot , pTexture , _shader);
}
END_NAMESPACE_XEVOL3D