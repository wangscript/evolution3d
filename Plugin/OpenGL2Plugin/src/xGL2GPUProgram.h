#pragma once

#include "xGL2Shader.h"
#include "xGL2ParamBuffer.h"
BEGIN_NAMESPACE_XEVOL3D

class xGL2GpuProgParamTable : public IGpuProgramParamTable
{
public:
	xGL2ShaderParamTable* m_pVSParamTable;
	xGL2ShaderParamTable* m_pPSParamTable;
	xGL2ShaderParamTable* m_pGSParamTable;
	IGpuProgram*          m_pGpuProgram;
public:
	virtual ~xGL2GpuProgParamTable();
	xGL2GpuProgParamTable(IGpuProgram* pGpuProgram);
	IGpuProgram*           getGpuProgram();
	IShaderParamTable*     getShaderParamTab(eShaderType shType);
	bool                   updateRenderContext();
	bool                   commit();

	bool                   setTexture(const wchar_t* name        , IBaseTexture* pTexture , eShaderType _shader =  eShader_None);
	bool                   setTexture(const int texName          , IBaseTexture* pTexture , eShaderType _shader =  eShader_None);
	bool                   setTextureBySlot(const int iSlot      , IBaseTexture* pTexture , eShaderType _shader =  eShader_None);

	bool                   load(xXmlNode* pXMLNode);
	xGL2ShaderParamTable*  getShaderParamTable(eShaderType shType);
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
			if(m_pPSParamTable && m_pPSParamTable->setParamater(name , _value ) )
				return true;
			if(m_pVSParamTable && m_pVSParamTable->setParamater(name , _value) )
				return true;
			if(m_pGSParamTable && m_pGSParamTable->setParamater(name , _value) )
				return true;
			return false;
		}
		else
		{
			xGL2ShaderParamTable* pTable = getShaderParamTable(_shader);
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
			if(m_pPSParamTable && m_pPSParamTable->setParamater(name , _array , n , baseIndex) )
				return true;
			if(m_pVSParamTable && m_pVSParamTable->setParamater(name , _array , n , baseIndex) )
				return true;
			if(m_pGSParamTable && m_pGSParamTable->setParamater(name , _array , n , baseIndex) )
				return true;
			return false;
		}
		else
		{
			xGL2ShaderParamTable* pTable = getShaderParamTable(_shader);
			if(pTable == NULL) return false;
			pTable->setParamater(name , _array , n , baseIndex);
			return true;
		}
		return false;
	}


};

class xGL2GpuProgram : public IGpuProgram
{
	friend class xGL2RenderApi ;
	xGL2GeomShader*         m_pGeomShader;
	xGL2PixelShader*        m_pPixelShader;
	xGL2VertexShader*       m_pVertexShader;
	xGL2GpuProgParamTable*  m_defParamTable;
	IGpuProgramParamTable*  m_pInstalledTable;

	IMPL_BASE_OBJECT_INTERFACE(xGL2GpuProgram)
public:
    bool                    _load(xXmlNode* pNode );
	bool                    _afterLoad();
public:
	xGL2GpuProgram(IRenderApi* pRenderApi);
	~xGL2GpuProgram();
	bool                    load(const wchar_t* fileName , unsigned long  arg);
	bool                    load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool                    load(const wchar_t* vsName , const wchar_t* psName, const wchar_t* gsName , unsigned long arg);
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
};


END_NAMESPACE_XEVOL3D
