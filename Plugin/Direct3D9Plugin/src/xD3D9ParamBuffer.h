#ifndef __XEVOL_D10_SHADER_PARAM_BUFFER_H__
#define __XEVOL_D10_SHADER_PARAM_BUFFER_H__
#include <RenderAPI/xBaseShader.h>
#include <vector>
#include <map>
#include <string>
#include "xD3DInputBuffer.h"
#include "xD3D9InputBuffer.h"
#include "xD3D9ConstantBuffer.h"
BEGIN_NAMESPACE_XEVOL3D

struct xD3D9TextureBinding
{
	//HBaseTexture  m_hTexture;
	int             m_iTexUsage;//In the renderer side.
	int             m_iShaderSlot; // 在Shader里的Slot
	wchar_t         m_Name[32];//在Shader里的名字
	int             m_iName;//Shader里的名字
	IBaseTexture*   m_pTexture;
};

struct xD3D9SamplerBinding
{
	ISamplerState* m_pSampler;
	int            m_iShaderSlot; // 在Shader里的Slot
	wchar_t        m_Name[32];
};

struct xD3D9BufferBinding
{
	IInputBuffer*               m_InputBuffer;
	xD3D9ConstBufferReflection* m_pReflection;
	std::wstring                m_Name;
	eD3DResourceBindType        m_bindType;
    D3DXHANDLE                  m_hConstHandle;
    ID3DXConstantTable*         m_pConstTable;
    int                         m_Bytes;
public:
	xD3D9BufferBinding();
	~xD3D9BufferBinding();
	void          unload();
	bool          validate(IDirect3DDevice9* pDevice);
};


typedef std::vector<xD3D9SamplerBinding> vD3D9SamplerBindings;
typedef std::vector<xD3D9TextureBinding> vD3D9TextureBindings;
typedef std::vector<xD3D9BufferBinding>  vD3D9BufferBindings;


/****************************************
记录所有需要自动加载的Shader
****************************************/
class xD3D9ShaderParamTable : public IShaderParamTable
{
    IMPL_BASE_OBJECT_INTERFACE(xD3D9ShaderParamTable);
public:
	xD3D9ShaderParamTable(IBaseShader*   pD10Shader);
	~xD3D9ShaderParamTable();		
//public;
public:
    bool                       load(ID3DXConstantTable* pShaderReflection );
    bool                       unload();
    bool                       commit();
	size_t                     nConstant();
	IShaderConstantReflection* getConstDesc(size_t idx);
	IBaseShader*               getShader();
	bool                       updateRenderContext();

	bool                       setTexture(const wchar_t* textureName , IBaseTexture* pTexture);
	bool                       setTexture(const int texName          , IBaseTexture* pTexture);
	bool                       setTextureBySlot(const int iSlot      , IBaseTexture* pTexture);

	bool                       setParamater(const wchar_t* name , float  _value);
	bool                       setParamater(const wchar_t* name , int    _value);
	bool                       setParamater(const wchar_t* name , xvec4& _value);
	bool                       setParamater(const wchar_t* name , xmat4& _value);



	bool                       setParamater(const wchar_t* name , xvec4* _array , int n , int baseIndex = 0);
	bool                       setParamater(const wchar_t* name , int*   _array , int n , int baseIndex = 0);
	bool                       setParamater(const wchar_t* name , float* _array , int n , int baseIndex = 0);
	bool                       setParamater(const wchar_t* name , xmat4* _array , int n , int baseIndex = 0);
   		
protected:
           
         
    bool                       createBufferVarList(ID3DXConstantTable* pShaderReflection , D3DXHANDLE hConstHandle , D3DXCONSTANT_DESC& ConstDesc, xD3D9BufferBinding& cbSlot, int bufIdex);             

    void                       AddConstReflection( xD3D9ShaderConstReflection* constDesc, int VarOffset, D3DXHANDLE hConstHandle, D3DXCONSTANT_DESC &_desc, wchar_t* varName, xD3D9BufferBinding &cbSlot ) ;
    bool                       installSamplerTextureBinding();
    bool                       installConstBuffer();

protected:
    IBaseShader*                m_pBaseShader;
    //绑定Sampler
    vD3D9SamplerBindings        m_SamplerBindings;
    //绑定纹理
    vD3D9TextureBindings        m_TextureBindings;
    //绑定的ConstBuffer
    vD3D9BufferBindings         m_ConstBufferBindings;
    //绑定为ShaderResourceView的Buffer;
    vD3D9BufferBindings         m_BufferBindings;
    //匹配，一个便于顺序遍历，一个便于查找
    vecD3D9ShaderConstantDescs  m_vecDesc;
    mapD3D9ShaderConstantDescs  m_mapDesc;
};

END_NAMESPACE_XEVOL3D

#endif
