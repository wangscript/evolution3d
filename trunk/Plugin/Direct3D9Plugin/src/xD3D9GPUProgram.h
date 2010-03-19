#pragma once

#include "xD3D9Shader.h"
#include "xD3D9ParamBuffer.h"
BEGIN_NAMESPACE_XEVOL3D

class xD3D9GpuProgParamTable : public IGpuProgramParamTable
{
    IMPL_BASE_OBJECT_INTERFACE(xD3D9GpuProgParamTable);
public:
	xD3D9ShaderParamTable* m_pParamTable[eShader_Max];
	IGpuProgram*          m_pGpuProgram;
public:
	virtual ~xD3D9GpuProgParamTable();
	xD3D9GpuProgParamTable(IGpuProgram* pGpuProgram);
	IGpuProgram*           getGpuProgram();
	IShaderParamTable*     getShaderParamTab(eShaderType shType);
	bool                   updateRenderContext();
	bool                   commit();

	bool                   setTexture(const wchar_t* name        , IBaseTexture* pTexture , eShaderType _shader =  eShader_None);
	bool                   setTexture(const int texName          , IBaseTexture* pTexture , eShaderType _shader =  eShader_None);
	bool                   setTextureBySlot(const int iSlot      , IBaseTexture* pTexture , eShaderType _shader =  eShader_None);

	bool                   load(xXmlNode* pXMLNode);
	xD3D9ShaderParamTable*  getShaderParamTable(eShaderType shType);
	bool                   setParamater(const wchar_t* name , float  _value , eShaderType shType = eShader_None);
	bool                   setParamater(const wchar_t* name , int    _value , eShaderType shType = eShader_None);
	bool                   setParamater(const wchar_t* name , xvec4& _value , eShaderType shType = eShader_None);
	bool                   setParamater(const wchar_t* name , xmat4& _value , eShaderType shType = eShader_None);

	bool                   setParamater(const wchar_t* name , xvec4* _array , int n , int baseIndex = 0 , eShaderType shType = eShader_None);
	bool                   setParamater(const wchar_t* name , int*   _array , int n , int baseIndex = 0 , eShaderType shType = eShader_None);
	bool                   setParamater(const wchar_t* name , float* _array , int n , int baseIndex = 0 , eShaderType shType = eShader_None);
	bool                   setParamater(const wchar_t* name , xmat4* _array , int n , int baseIndex = 0 , eShaderType shType = eShader_None);

public:

	template <typename T> bool T_setParamater(const wchar_t* name , T  _value , eShaderType _shader)
	{
		if(_shader == eShader_None)
		{
			for( int i = 0 ; i < eShader_Max ; i ++)
			if(m_pParamTable[i] && m_pParamTable[i]->setParamater(name , _value ) )
				return true;
			return false;
		}
		else
		{
			xD3D9ShaderParamTable* pTable = getShaderParamTable(_shader);
			if(pTable == NULL) return false;
			pTable->setParamater(name , _value);
			return true;
		}
		return false;
	}

	template <typename T> bool T_setParamater(const wchar_t* name , T* _array , int n , int baseIndex , eShaderType _shader)
	{
		if(_shader == eShader_None)
		{
			for( int i = 0 ; i < eShader_Max ; i ++)
				if(m_pParamTable[i] && m_pParamTable[i]->setParamater( name , _array , n , baseIndex) )
					return true;
			return false;
		}
		else
		{
			xD3D9ShaderParamTable* pTable = getShaderParamTable(_shader);
			if(pTable == NULL) return false;
			pTable->setParamater(name , _array , n , baseIndex);
			return true;
		}
		return false;
	}


};

class xD3D9GpuProgram : public IGpuProgram
{
	friend class xD3D9RenderApi ;
	xD3D9Shader*             m_pShader[eShader_Max];
	xD3D9GpuProgParamTable*  m_defParamTable;
	IGpuProgramParamTable*   m_pInstalledTable;
	IMPL_BASE_OBJECT_INTERFACE(xD3D9GpuProgram)
public:
    bool                    _load(xXmlNode* pNode );
	bool                    _afterLoad();
public:
	xD3D9GpuProgram(IRenderApi* pRenderApi);
	~xD3D9GpuProgram();
	bool                    load(const wchar_t* fileName , unsigned long  arg);
	bool                    load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool                    load(const wchar_t* shaderNames[eShader_Max] , unsigned long arg);
	bool                    isLoaded();
	bool                    unload();
	unsigned long           memUsage(){return 1 ; };

	bool                    setTexture(const wchar_t* textureName , IBaseTexture* pTexture , eShaderType _shader =  eShader_None);
	bool                    setTexture(const int texName          , IBaseTexture* pTexture , eShaderType _shader =  eShader_None);
	bool                    setTextureBySlot(const int iSlot      , IBaseTexture* pTexture , eShaderType _shader =  eShader_None);

	IGpuProgramParamTable*  getParamTable(){ return m_pInstalledTable ? m_pInstalledTable : m_defParamTable ; }
	void                    setParamTable(IGpuProgramParamTable* pTable) {m_pInstalledTable = pTable ; }
	IGpuProgramParamTable*  createParamTable(bool bLoadDef);
	IGpuProgramParamTable*  getDefaultParamTable();
	void                    commit();
    IBaseShader*            getShader(eShaderType _type);
    void                    getName(xGpuProgNameParser& _name);
};


END_NAMESPACE_XEVOL3D
