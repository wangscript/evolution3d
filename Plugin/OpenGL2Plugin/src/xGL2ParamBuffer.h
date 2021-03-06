#ifndef __XEVOL_D10_SHADER_PARAM_BUFFER_H__
#define __XEVOL_D10_SHADER_PARAM_BUFFER_H__
#include <RenderAPI/xBaseShader.h>
#include  <d3d10.h>
#include  <d3d10shader.h>
#include <vector>
#include <map>
#include <string>
#include "xGL2InputBuffer.h"
BEGIN_NAMESPACE_XEVOL3D

enum eD10ResourceBindType
{
	eRBT_CBuffer,
	eRBT_TBuffer,
	eRBT_Buffer,
	eRBT_Texture,
	eRBT_Sampler,
};

struct xGL2TextureBinding
{
	//HBaseTexture  m_hTexture;
	int             m_iTexUsage;//In the renderer side.
	int             m_iShaderSlot; // 在Shader里的Slot
	wchar_t         m_Name[32];//在Shader里的名字
	int             m_iName;//Shader里的名字
	IBaseTexture*   m_pTexture;
};

struct xGL2SamplerBinding
{
	ISamplerState* m_pSampler;
	int            m_iShaderSlot; // 在Shader里的Slot
	wchar_t        m_Name[32];
};

struct xGL2BufferBinding
{
	IInputBuffer*           m_InputBuffer;
	xGL2InputBufReflection* m_pReflection;
	int                     m_bindPoint;
	std::wstring            m_Name;
	eD10ResourceBindType    m_bindType;
public:
	xGL2BufferBinding();
	~xGL2BufferBinding();
	void          unload();
	bool          validate();
};


typedef std::vector<xGL2SamplerBinding> vD10SamplerBindings;
typedef std::vector<xGL2TextureBinding> vD10TextureBindings;
typedef std::vector<xGL2BufferBinding>  vD10BufferBindings;


/****************************************
记录所有需要自动加载的Shader
****************************************/
class xGL2ShaderParamTable : public IShaderParamTable
{
	IBaseShader*         m_pD10Shader;

	//绑定Sampler
	vD10SamplerBindings              m_SamplerBindings;
	//绑定纹理
	vD10TextureBindings              m_TextureBindings;
	//绑定的ConstBuffer
	vD10BufferBindings               m_ConstBufferBindings;
	//绑定为ShaderResourceView的Buffer;
	vD10BufferBindings               m_BufferBindings;

	

	//匹配，一个便于顺序遍历，一个便于查找
	vecShaderConstantDescs          m_vecDesc;
	mapShaderConstantDescs          m_mapDesc;
	//
	bool     createResourceBindingTable(ID3D10ShaderReflection* pShaderReflection);
	
    bool     createConstBufferBinding(ID3D10ShaderReflection* pShaderReflection);

	bool     createBufferVarList(ID3D10ShaderReflectionConstantBuffer* pCBReflection , GL2_SHADER_BUFFER_DESC& cbDesc, xGL2BufferBinding& cbSlot, int bufIdex);

	bool     createTextureBuffer(GL2_SHADER_INPUT_BIND_DESC resDesc);
	bool     createConstBuffer(ID3D10ShaderReflection* pShaderReflection , GL2_SHADER_INPUT_BIND_DESC& resDesc);
	bool     createTBuffer(ID3D10ShaderReflection* pShaderReflection , GL2_SHADER_INPUT_BIND_DESC& resDesc);
    
	bool     installSamplerTextureBinding();
	bool     installConstBuffer();
    bool     installBuffer();

public:
	bool     load(ID3D10ShaderReflection* pShaderReflection );
	bool     unload();
public:
	xGL2ShaderParamTable(IBaseShader*   pD10Shader);
	~xGL2ShaderParamTable();
	bool                       commit();
	
//public;
public:
	size_t                     nConstant();
	IShaderConstantReflection*       getConstDesc(size_t idx);
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
   						       
						       
};

END_NAMESPACE_XEVOL3D

#endif
