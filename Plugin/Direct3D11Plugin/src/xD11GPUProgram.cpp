#include "stdafx.h"
#include "xD11GPUProgram.h"
#include "xDirect3D11API.h"
#include <OperationSys/xOperationSys.h>
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD11GpuProgram   , IGpuProgram)
IMPL_BASE_OBJECT_CLASSID(xD11GpuProgParamTable , IGpuProgramParamTable);


xD11GpuProgParamTable::~xD11GpuProgParamTable()
{
	for(int i  = 0 ; i < eShader_Max ; i ++)
       XSAFE_DELETE(m_pParamTable[i]);
}

xD11GpuProgParamTable::xD11GpuProgParamTable(IGpuProgram* pGpuProgram)
{
	m_RefCount = 1;
	for(int i  = 0 ; i < eShader_Max ; i ++)
	   m_pParamTable[i] = NULL;
	m_pGpuProgram = pGpuProgram;
}

IGpuProgram* xD11GpuProgParamTable::getGpuProgram()
{
	return m_pGpuProgram;
}

IShaderParamTable* xD11GpuProgParamTable::getShaderParamTab(eShaderType shType)
{
	return m_pParamTable[shType];
	return NULL;
}

bool xD11GpuProgParamTable::load(xXmlNode* pXMLNode)
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
	return false;
}


bool xD11GpuProgParamTable::updateRenderContext()
{
	for(int i  = 0 ; i < eShader_Max ; i ++)
		if(m_pParamTable[i])  m_pParamTable[i]->updateRenderContext();
	return true;
}
bool  xD11GpuProgParamTable::commit()
{
	for(int i  = 0 ; i < eShader_Max ; i ++)
		if(m_pParamTable[i])  m_pParamTable[i]->commit();
	return true;

}

bool xD11GpuProgParamTable::setTexture(const wchar_t* name    , IBaseTexture* pTexture , eShaderType _shader)
{
	int iTexName = xStringHash(name);
	return setTexture(iTexName , pTexture , _shader);
}

bool xD11GpuProgParamTable::setTexture(const int texName , IBaseTexture* pTexture , eShaderType _shader)
{
	if( _shader == eShader_None )
	{
		{
			for(int i  = 0 ; i < eShader_Max ; i ++)
			{
				if(m_pParamTable[i] && m_pParamTable[i]->setTexture(texName , pTexture) )
				return true;
			}
			return false;
		}
	}

	if(m_pParamTable[_shader] && m_pParamTable[_shader]->setTexture(texName , pTexture) )
		return true;
	return false;


}

xD11ShaderParamTable*  xD11GpuProgParamTable::getShaderParamTable(eShaderType shType)
{
	if(shType == eShader_None) return NULL;
	return m_pParamTable[shType];
}

bool xD11GpuProgParamTable::setTextureBySlot(const int iSlot      , IBaseTexture* pTexture , eShaderType _shader)
{
	if(_shader == eShader_None)
	{
		for(int i = 0 ; i < eShader_Max ; i ++)
		{
			if(m_pParamTable[i] && m_pParamTable[i]->setTextureBySlot(iSlot , pTexture) )
				return true;
		}
	    return false;
	}
	else
	{
		xD11ShaderParamTable* pTable = getShaderParamTable(_shader);
		if(pTable == NULL) return false;
		pTable->setTextureBySlot(iSlot , pTexture);
		return true;
	}
}

bool xD11GpuProgParamTable::setParamater(const wchar_t* name , float  _value , eShaderType shType)
{
	return T_setParamater(name,_value,shType);
}

bool xD11GpuProgParamTable::setParamater(const wchar_t* name , int    _value , eShaderType shType)
{
	return T_setParamater(name,_value,shType);
}

bool xD11GpuProgParamTable::setParamater(const wchar_t* name , xvec4& _value , eShaderType shType)
{
	return T_setParamater(name,_value,shType);
}

bool xD11GpuProgParamTable::setParamater(const wchar_t* name , xmat4& _value , eShaderType shType )
{
	return T_setParamater(name,_value,shType);
}

bool xD11GpuProgParamTable::setParamater(const wchar_t* name , xvec4* _array , int n , int baseIndex, eShaderType shType)
{
	return T_setParamater(name,_array, n , baseIndex, shType);
}

bool xD11GpuProgParamTable::setParamater(const wchar_t* name , int*   _array , int n , int baseIndex, eShaderType shType)
{
	return T_setParamater(name,_array, n , baseIndex, shType);
}

bool xD11GpuProgParamTable::setParamater(const wchar_t* name , float* _array , int n , int baseIndex, eShaderType shType )
{
	return T_setParamater(name,_array, n , baseIndex, shType);
}

bool xD11GpuProgParamTable::setParamater(const wchar_t* name , xmat4* _array , int n , int baseIndex, eShaderType shType)
{
	return T_setParamater(name,_array, n , baseIndex, shType);
}


xD11GpuProgram::xD11GpuProgram(IRenderApi* pRenderApi):IGpuProgram(pRenderApi)
{
	m_RefCount = 1;
	m_defParamTable = NULL;
	for(int i = 0 ; i < eShader_Max ; i ++)
		m_pShader[i] = NULL;
	m_pInstalledTable = NULL;
}

xD11GpuProgram::~xD11GpuProgram()
{
	unload();

}
bool xD11GpuProgram::_afterLoad()
{
	XEvol_Convert(m_defParamTable ,  createParamTable(false) );
	//Load DefConst Table;
	return m_pShader[eShader_VertexShader] != NULL || m_pShader[eShader_PixelShader] != NULL;
}


bool xD11GpuProgram::load(const wchar_t* shaderNames[eShader_Max] , unsigned long arg) 
{
	for(int i = 0 ; i < eShader_Max ; i ++)
	{
		const wchar_t* shaderName = shaderNames[i];
		eShaderType _type = (eShaderType)i;
		if(shaderName)
		{
			XEvol_Convert(m_pShader[i] , m_pRenderApi->createShader(shaderName , _type).getResource() );
		}
	}
 	return _afterLoad();
}

eShaderType _GetShaderType(const ds_wstring& _shaderType)
{
	if(_shaderType == L"VertexShader")
	{
		return eShader_VertexShader ;
	}
	else if(_shaderType == L"PixelShader")
	{
		return eShader_PixelShader ;

	}
	else if(_shaderType == L"GeomShader")
	{
		return   eShader_GeometryShader ;
	}
	else if(_shaderType == L"HullShader")
	{
		return   eShader_HullShader ;
	}
	else if(_shaderType == L"DomainShader")
	{
		return   eShader_DomainShader ;
	}
	else if(_shaderType == L"ComputeShader")
	{
		return   eShader_ComputeShader ;
	}
	return eShader_None;
}
bool xD11GpuProgram::_load(xXmlNode* pNode )
{

	xXmlNode* pShaderNode =  pNode->findNode(RENDER_NAME,0);
	if(pShaderNode == NULL)
		return false;

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
		eShaderType shaderType = _GetShaderType(type);
		if(shaderType != eShader_None)
		{
			XEvol_Convert(m_pShader[i] , m_pRenderApi->createShader(strFile , shaderType).getResource() );
		}
	};

	bool ret =  _afterLoad();
	if(ret == true)  
		m_defParamTable->load(pShaderConstNode);
	return ret;
}


bool xD11GpuProgram::load(const wchar_t* fileName , unsigned long  arg)
{
    unload();
    xXmlDocument xmlDoc;
	if(false == xmlDoc.load(fileName,true) ) return false;
	return _load(xmlDoc.root());
}


bool xD11GpuProgram::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();
	xXmlDocument xmlDoc;

	if(false == xmlDoc.load( (const wchar_t*)buf, (int)bufLen , true) ) 
		return false;

	return _load(xmlDoc.root());
}

bool xD11GpuProgram::isLoaded()
{
	for(int i = 0 ; i < eShader_Max ; i++ )
		if(m_pShader[i] != NULL) return true;
	return false; 
}

bool  xD11GpuProgram::unload()
{
	XSAFE_KILLOBJECT(m_defParamTable);
	return true;
}

IGpuProgramParamTable*  xD11GpuProgram::createParamTable(bool bLoadDef)
{
	xD11GpuProgParamTable* pGpuProgramTable = new xD11GpuProgParamTable(this); 
    for(int i = 0 ; i < eShader_Max ; i ++)
	{
		if(m_pShader[i] != NULL)
		{
			XEvol_Convert(pGpuProgramTable->m_pParamTable[i]  ,   m_pShader[i]->createShaderParamTable() ); 
		}
	}

	if(bLoadDef)
	{
		//加载默认的数据
		assert(0);
		XEVOL_WARNNING_NOT_IMPLEMENT;
	}

	return pGpuProgramTable;
}

IGpuProgramParamTable*  xD11GpuProgram::getDefaultParamTable()
{
	return m_defParamTable;
}


void xD11GpuProgram::commit()
{
	xD11GpuProgParamTable* pD10GpuGonstTable = (xD11GpuProgParamTable*)getParamTable();
	pD10GpuGonstTable->updateRenderContext();
	pD10GpuGonstTable->commit();
}

bool xD11GpuProgram::setTexture(const wchar_t* texName , IBaseTexture* pTexture , eShaderType _shader)
{
	 return getParamTable()->setTexture(texName , pTexture , _shader);
}

bool xD11GpuProgram::setTexture(const int texName , IBaseTexture* pTexture , eShaderType _shader)
{
	return getParamTable()->setTexture(texName , pTexture , _shader);
}

bool xD11GpuProgram::setTextureBySlot(const int iSlot      , IBaseTexture* pTexture , eShaderType _shader)
{
	 return getParamTable()->setTextureBySlot(iSlot , pTexture , _shader);
}
END_NAMESPACE_XEVOL3D