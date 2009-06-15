#include "xD10ParamBuffer.h"
#include <BaseLib/xStringHash.h>
#include <BaseLib/xI18N.h>
#include <RenderAPI/xShaderSemantic.h>
#include "xD10ConstLexer.h"
#include "xD10Shader.h"
#include "xD10InputBuffer.h"
#include "xDirect3D10API.h"
BEGIN_NAMESPACE_XEVOL3D


xD10BufferBinding::xD10BufferBinding()
{
	m_bindPoint = 0;
	m_bindType  = eRBT_CBuffer;
    m_pReflection =  NULL;
	m_InputBuffer = NULL;
}

void xD10BufferBinding::unload()
{
	XSAFE_RELEASEOBJECT(m_InputBuffer);
}

xD10BufferBinding::~xD10BufferBinding()
{

}


bool xD10BufferBinding::validate()
{
   if(m_InputBuffer == NULL)
	   return false;
   xD10InputBufReflection* 	 pReflection =   m_pReflection == NULL ? (xD10InputBufReflection*)m_InputBuffer->reflection() : m_pReflection;
   if(pReflection == NULL) 
	   return false;
   return pReflection->flushToBuffer();
}


eShaderVarDataType toParamDataType(D3D10_SHADER_VARIABLE_TYPE svt)
{
     switch( svt )
	 {
	    case      D3D10_SVT_VOID:// = 0,
		 return SEDT_ANY;
		
		case	 D3D10_SVT_BOOL:// = 1,
			return SEDT_BOOL;
		
		case	 D3D10_SVT_INT:// = 2,
			return SEDT_INT32;

		case	 D3D10_SVT_FLOAT:// = 3,
			return SEDT_FLOAT32;

		case	 D3D10_SVT_STRING:// = 4,
			return SEDT_STRING;

		case	 D3D10_SVT_TEXTURE:// = 5,
			return SEDT_TEXTURE;

		case	 D3D10_SVT_TEXTURE1D:// = 6,
			return SEDT_TEXTURE1D;

		case	 D3D10_SVT_TEXTURE2D:// = 7,
			return SEDT_TEXTURE2D;

		case	 D3D10_SVT_TEXTURE3D:// = 8,
			return SEDT_TEXTURE3D;

		case	 D3D10_SVT_TEXTURECUBE:// = 9,
			return SEDT_TEXTURECUBE;

		case	 D3D10_SVT_SAMPLER:// = 10,
			return SEDT_SAMPLER;

		case	 D3D10_SVT_PIXELSHADER:// = 15,
			return SEDT_OBJECT;
		case	 D3D10_SVT_VERTEXSHADER:// = 16,
			return SEDT_OBJECT;

		case	 D3D10_SVT_UINT:// = 19,
			return SEDT_UINT32;
		case	 D3D10_SVT_UINT8:// = 20,
			return SEDT_UBYTE;

		case	 D3D10_SVT_GEOMETRYSHADER:// = 21,
		case	 D3D10_SVT_RASTERIZER:// = 22,
		case	 D3D10_SVT_DEPTHSTENCIL:// = 23,
		case	 D3D10_SVT_BLEND ://= 24,
			return SEDT_OBJECT;


			//buffers;
		case	 D3D10_SVT_BUFFER:// = 25,
			return SEDT_OBJECT;
		case	 D3D10_SVT_CBUFFER:// = 26,
			return SEDT_OBJECT;
		case	 D3D10_SVT_TBUFFER:// = 27,
			return SEDT_OBJECT;

			//Texture array;
		case	 D3D10_SVT_TEXTURE1DARRAY ://= 28,
			return SEDT_OBJECT;
		case	 D3D10_SVT_TEXTURE2DARRAY:// = 29,
			return SEDT_OBJECT;

		case	 D3D10_SVT_RENDERTARGETVIEW:// = 30,
			return SEDT_OBJECT;
		case	 D3D10_SVT_DEPTHSTENCILVIEW ://= 31,
			return SEDT_OBJECT;


		case	 D3D10_SVT_TEXTURE2DMS:// = 32,
			return SEDT_OBJECT;
		case	 D3D10_SVT_TEXTURE2DMSARRAY:// = 33,
			return SEDT_OBJECT;
		case	 D3D10_SVT_TEXTURECUBEARRAY:// = 34,
			return SEDT_OBJECT;
	 }
	 return SEDT_ANY;
}

eShaderVarType toParamClass (eShaderVarDataType dataType , D3D10_SHADER_VARIABLE_CLASS varClass , int row , int colum)
{
	switch(varClass)
	{
	case  D3D10_SVC_SCALAR:
	case  D3D10_SVC_VECTOR:
	case  D3D10_SVC_MATRIX_ROWS:
	case  D3D10_SVC_MATRIX_COLUMNS:
		return (eShaderVarType) ( MAKE_SHADER_VAR_TYPPE(dataType , row , colum) );

	case  D3D10_SVC_OBJECT:
		XEVOL_LOG(eXL_DEBUG_HIGH,L"Shader Class 为 D3D10_SVC_OBJECT , 不支持\n" );
		return SHADERVARTYPE_Unkown;
	case  D3D10_SVC_STRUCT:
		XEVOL_LOG(eXL_DEBUG_HIGH,L"Shader Class 为 D3D10_SVC_STRUCT , 不支持\n" );
		return SHADERVARTYPE_Unkown;
	}
	return SHADERVARTYPE_Unkown;
}

static void ClipName(wchar_t* name , wchar_t* clipStr)
{
	for(size_t i = 0 ; i < wcslen(name) ; i ++ )
	{
		if(wcscmp(name + i , clipStr ) == 0 )
		{
			name[i] = 0;
			break;
		}
	}
}

bool xD10ShaderParamTable::createResourceBindingTable(ID3D10ShaderReflection* pShaderReflection)
{
	xD3D10RenderApi* pD10Api = (xD3D10RenderApi*) m_pD10Shader->renderApi();
	D3D10_SHADER_DESC shader_desc;
	pShaderReflection->GetDesc(&shader_desc);
	for(size_t i  = 0 ; i < shader_desc.BoundResources ; i ++)
	{
		D3D10_SHADER_INPUT_BIND_DESC resDesc;
		pShaderReflection->GetResourceBindingDesc( (UINT)i , &resDesc);
		switch(resDesc.Type)
		{
		case D3D10_SIT_CBUFFER:
			{
				createConstBuffer(pShaderReflection,resDesc);
			}
			break;

		case D3D10_SIT_TBUFFER:
			{
				createTBuffer(pShaderReflection,resDesc);
			}
			break;

		case D3D10_SIT_TEXTURE:
			{
				if(resDesc.Dimension == D3D10_SRV_DIMENSION_BUFFER)
				{
					createTextureBuffer(resDesc);
				}
				else
				{
					xD10TextureBinding texer;
					texer.m_iShaderSlot = resDesc.BindPoint;
					memset(texer.m_Name,0, 64);
					XEvol_LocaleToUnicode(resDesc.Name , texer.m_Name, 32 );
					//找到绑定到哪个Texture上了
					wchar_t slotName[32];
					wcsncpy(slotName , texer.m_Name , 32);
					ClipName(slotName , L"Texture" );
					texer.m_iTexUsage = xD10ConstLexer::GetTextureSlotIdx(slotName);
					texer.m_iName     = xStringHash(texer.m_Name);
					texer.m_pTexture  = NULL;
					m_TextureBindings.push_back(texer);
				}
			}
			break;
		case D3D10_SIT_SAMPLER:
			{
				xD10SamplerBinding sampler;
				sampler.m_iShaderSlot = resDesc.BindPoint;
				memset(sampler.m_Name,0, 64);
				XEvol_LocaleToUnicode(resDesc.Name , sampler.m_Name , 32 );
				wchar_t slotName[32];
				wcsncpy(slotName , sampler.m_Name , 32);
				ClipName(slotName , L"Sampler" );
				sampler.m_pSampler = pD10Api->createSamplerState(slotName);
				m_SamplerBindings.push_back(sampler);
			}
			break;	
		}
	}
	return true;
}



bool xD10ShaderParamTable::createTextureBuffer(D3D10_SHADER_INPUT_BIND_DESC resDesc)
{
	//建立一个Buffer;
	wchar_t bufName[64] = {0};
	XEvol_LocaleToUnicode(resDesc.Name , bufName, 64);

	IInputBuffer* pConstBuffer = NULL;
	//首先查找这个名字的const buffer Name;，如果没有，表示环境里没有
	pConstBuffer = m_pD10Shader->renderApi()->findInputBuffer(bufName);
	if(pConstBuffer != NULL)
	{
		pConstBuffer->AddRef();
	}

	//Buffer已经建立，加到列表里
	m_BufferBindings.push_back( xD10BufferBinding() );

	xD10BufferBinding& cbSlot =  m_BufferBindings[ m_BufferBindings.size() - 1];
	cbSlot.m_InputBuffer = pConstBuffer;
	cbSlot.m_Name        = bufName;
	cbSlot.m_bindPoint   = resDesc.BindPoint;
	cbSlot.m_bindType    = eRBT_Buffer;
	return true;
}

ID3D10ShaderReflectionConstantBuffer* GetConstBufferDesc(ID3D10ShaderReflection* pShaderReflection , D3D10_SHADER_BUFFER_DESC& cbDesc , D3D10_SHADER_INPUT_BIND_DESC& resDesc)
{
	D3D10_SHADER_DESC ShaderDesc;
	pShaderReflection->GetDesc(&ShaderDesc);

	//获得ConstBuffer的Desc和Reflection
	ID3D10ShaderReflectionConstantBuffer* pCBReflection = NULL;
	for(UINT  idx = 0 ; idx < ShaderDesc.ConstantBuffers ; idx ++)
	{
		pCBReflection = pShaderReflection->GetConstantBufferByIndex( idx ) ;

		if(pCBReflection == NULL) 
			continue;

		pCBReflection->GetDesc(&cbDesc);
		if(std::string(cbDesc.Name) == resDesc.Name)
			return pCBReflection;
	}
	return NULL;
}

bool xD10ShaderParamTable::createTBuffer(ID3D10ShaderReflection* pShaderReflection , D3D10_SHADER_INPUT_BIND_DESC& resDesc)
{
	D3D10_SHADER_DESC    ShaderDesc;
	pShaderReflection->GetDesc(&ShaderDesc);

	//获得ConstBuffer的Desc和Reflection
	D3D10_SHADER_BUFFER_DESC cbDesc ; 
	ID3D10ShaderReflectionConstantBuffer* pCBReflection = GetConstBufferDesc(pShaderReflection,cbDesc,resDesc);

	if(pCBReflection == NULL) 
		return false;

	//建立一个Buffer;
	wchar_t bufName[64] = {0};
	XEvol_LocaleToUnicode(cbDesc.Name , bufName, 64);

	IInputBuffer* pConstBuffer = NULL;
	//首先查找这个名字的const buffer Name;，如果没有，表示环境里没有
	xD10InputBufReflection* pBufReflection = NULL;
	if(pConstBuffer == NULL)
	{
		pConstBuffer = m_pD10Shader->renderApi()->createInputBuffer(cbDesc.Size);
		xD10InputBuffer* pD10Buffer = (xD10InputBuffer*)pConstBuffer;
		pBufReflection = (xD10InputBufReflection*)pD10Buffer->createReflection(true);
	}
	else
	{
		pConstBuffer->AddRef();
	}

	//Buffer已经建立，加到列表里
	m_BufferBindings.push_back( xD10BufferBinding() );

	xD10BufferBinding& cbSlot =  m_BufferBindings[ m_BufferBindings.size() - 1];
	cbSlot.m_InputBuffer = pConstBuffer;
	cbSlot.m_Name        = bufName;
	cbSlot.m_bindPoint   = resDesc.BindPoint;
	cbSlot.m_pReflection = pBufReflection;
	cbSlot.m_bindType    = eRBT_TBuffer;
	createBufferVarList(pCBReflection,cbDesc,cbSlot , (int)m_BufferBindings.size() -  1 );
	return true;
}


bool xD10ShaderParamTable::createConstBuffer(ID3D10ShaderReflection* pShaderReflection , D3D10_SHADER_INPUT_BIND_DESC& resDesc)
{
	D3D10_SHADER_DESC    ShaderDesc;
	pShaderReflection->GetDesc(&ShaderDesc);

	//获得ConstBuffer的Desc和Reflection
	D3D10_SHADER_BUFFER_DESC cbDesc ; 
	ID3D10ShaderReflectionConstantBuffer* pCBReflection = GetConstBufferDesc(pShaderReflection,cbDesc,resDesc);

	if(pCBReflection == NULL) 
		return false;

	//建立一个Buffer;
	wchar_t bufName[64] = {0};
	XEvol_LocaleToUnicode(cbDesc.Name , bufName, 64);

	IInputBuffer* pConstBuffer = NULL;
	//首先查找这个名字的const buffer Name;，如果没有，表示环境里没有
	pConstBuffer = m_pD10Shader->renderApi()->findInputBuffer(bufName);
	xD10InputBufReflection* pBufReflection = NULL;
	if(pConstBuffer == NULL)
	{
		pConstBuffer = m_pD10Shader->renderApi()->createConstBuffer(cbDesc.Size);
		xD10InputBuffer* pD10Buffer = (xD10InputBuffer*)pConstBuffer;
		pBufReflection = (xD10InputBufReflection*)pD10Buffer->createReflection(true);
	}
	else
	{
		pConstBuffer->AddRef();
	}

	//Buffer已经建立，加到列表里
	m_ConstBufferBindings.push_back( xD10BufferBinding() );

	xD10BufferBinding& cbSlot =  m_ConstBufferBindings[ m_ConstBufferBindings.size() - 1];
	cbSlot.m_InputBuffer = pConstBuffer;
	cbSlot.m_Name        = bufName;
	cbSlot.m_bindPoint   = resDesc.BindPoint;
	cbSlot.m_bindType    = eRBT_CBuffer;
	cbSlot.m_pReflection = pBufReflection;
	createBufferVarList(pCBReflection,cbDesc,cbSlot , (int)m_ConstBufferBindings.size() - 1 );
	return true;
}

bool xD10ShaderParamTable::createBufferVarList(ID3D10ShaderReflectionConstantBuffer* pCBReflection , D3D10_SHADER_BUFFER_DESC& cbDesc, xD10BufferBinding& cbSlot, int bufIdex)
{
	if(cbSlot.m_pReflection == NULL)
		return false;
	//开始建立所有的变量
	for(size_t i = 0 ; i < cbDesc.Variables ; i ++ )
	{
		ID3D10ShaderReflectionVariable* pVarReflect     =  pCBReflection->GetVariableByIndex( (int)i );
		ID3D10ShaderReflectionType*     pVarTypeReflect =  pVarReflect->GetType();
		D3D10_SHADER_VARIABLE_DESC varDesc ; 
		D3D10_SHADER_TYPE_DESC     varTypeDesc;

		pVarTypeReflect->GetDesc(&varTypeDesc);
		pVarReflect->GetDesc(&varDesc);
		wchar_t varName[32]={0};
		XEvol_LocaleToUnicode(varDesc.Name , varName , 32);

		xD10ShaderConstReflection* constDesc = cbSlot.m_pReflection->addConstant();
		constDesc->m_bufOffset = varDesc.StartOffset;
		eShaderVarDataType dataType  = toParamDataType(varTypeDesc.Type ) ;

		constDesc->m_element   = varTypeDesc.Elements;
		constDesc->m_strName   = varName;
		constDesc->m_hashName  = xStringHash(varName);
		constDesc->m_varType   = toParamClass(dataType , varTypeDesc.Class , varTypeDesc.Rows , varTypeDesc.Columns );
        constDesc->m_bindType  = cbSlot.m_bindType;
		
		xShaderVarInfo info;
		SHADER_VAR_PARSER()->shaderVarInfo(constDesc->m_varType , info);
		constDesc->m_semantic     = xShaderSemanticTable::singleton().parser(  varName );
		constDesc->m_pBinder      = m_pD10Shader->renderApi()->getShaderConstBinder( varName );
		constDesc->m_pBufferReflection = cbSlot.m_pReflection;
		if(constDesc->m_semantic != eSPS_NotUsed )
		{
			//这里表示找到一个是需要自动设置变量数据的
		}	

		//将ConstDesc插入到两个查找队列里。一个顺序，一个hash
		m_mapDesc.insert( mapShaderConstantDescs::value_type(constDesc->m_hashName , constDesc) );
		m_vecDesc.push_back( constDesc );
		//m_iName = xStringHash(m_name);
	}
	return true;
}


bool xD10ShaderParamTable::load(ID3D10ShaderReflection* pShaderReflection )
{
	unload();	
	createResourceBindingTable(pShaderReflection);
	return true;
}

bool xD10ShaderParamTable::unload()
{
	//清除所有的标记

	for(size_t i = 0 ;  i < m_BufferBindings.size() ; i ++)
	{
		m_BufferBindings[i].unload();
	}

	for(size_t i = 0 ;  i < m_ConstBufferBindings.size() ; i ++)
	{
		m_ConstBufferBindings[i].unload();
	}

	m_BufferBindings.clear();
	m_BufferBindings.clear();

	m_mapDesc.clear();
	m_vecDesc.clear();
	return true;
}


bool xD10ShaderParamTable::setTexture(const wchar_t* textureName , IBaseTexture* pTexture)
{
	int texName = xStringHash(textureName);
	return setTexture(texName , pTexture);
}

bool xD10ShaderParamTable::setTexture(const int texName          , IBaseTexture* pTexture)
{
	for(size_t i  = 0 ; i < m_TextureBindings.size() ; i ++)
	{
		xD10TextureBinding& binder = m_TextureBindings[i];
		if(texName == binder.m_iName)
		{
			binder.m_pTexture = pTexture;
			return true;
		}
	}
	return false;
}

bool xD10ShaderParamTable::setTextureBySlot(const int iSlot      , IBaseTexture* pTexture)
{
	for(size_t i  = 0 ; i < m_TextureBindings.size() ; i ++)
	{
		xD10TextureBinding& binder = m_TextureBindings[i];
		if(iSlot == binder.m_iShaderSlot)
		{
			binder.m_pTexture = pTexture;
			return true;
		}
	}
	return false;
}

bool xD10ShaderParamTable::installSamplerTextureBinding()
{
	xD3D10RenderApi* pD10Api = (xD3D10RenderApi*) m_pD10Shader->renderApi();
	for(size_t i  = 0 ; i < m_SamplerBindings.size() ; i ++)
	{
		xD10SamplerBinding& binder = m_SamplerBindings[i];
		if(binder.m_pSampler)
		{
			pD10Api->setSamplerState(m_pD10Shader->type() , binder.m_iShaderSlot , binder.m_pSampler);
		}
	}

	for(size_t i  = 0 ; i < m_TextureBindings.size() ; i ++)
	{
		xD10TextureBinding& binder = m_TextureBindings[i];

		//首先要检查binder.m_pTexture
		IBaseTexture* pTexture = binder.m_pTexture ;

		//如果为空，就检查是不是有自动绑定的设置
		if(pTexture == NULL)
		{
			if(binder.m_iTexUsage != -1)
			{
				pTexture = pD10Api->getTexture(binder.m_iTexUsage);
			}
			else
			{
				pTexture = pD10Api->getTexture(binder.m_iName);
			}
		}

		//如果还为空，就从其它地方找
		if(pTexture != NULL)
		{
			pD10Api->setShaderResource(m_pD10Shader->type() , binder.m_iShaderSlot , pTexture);
		}
		else
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"Missing texture bing texture name=%s\n",binder.m_Name);
		}
	}
	return true;
}


bool xD10ShaderParamTable::installConstBuffer()
{
	for(size_t i = 0 ; i < m_ConstBufferBindings.size() ; i ++)
	{
		xD3D10RenderApi* pD10Api = (xD3D10RenderApi*) m_pD10Shader->renderApi();
		xD10BufferBinding& cbslot = m_ConstBufferBindings[i];
		cbslot.validate();

		xD10InputBuffer* InputBuffer = (xD10InputBuffer*)cbslot.m_InputBuffer;
		//Const Buffer的提交
		ID3D10Buffer*    pD10Buffer = (ID3D10Buffer*) cbslot.m_InputBuffer->handle();
		eShaderType shType = m_pD10Shader->type();
		//Commit the buffer to D3D 10 Device;
		if(shType == eShader_VertexShader )
		{
			pD10Api->d10Device()->VSSetConstantBuffers( cbslot.m_bindPoint , 1 , &pD10Buffer);
		}
		else if(shType == eShader_PixelShader)
		{
			pD10Api->d10Device()->PSSetConstantBuffers( cbslot.m_bindPoint , 1 , &pD10Buffer);
		}
		else if(shType == eShader_GeometryShader)
		{
			pD10Api->d10Device()->GSSetConstantBuffers( cbslot.m_bindPoint , 1 , &pD10Buffer);  
		}
	}
	return true;
}

bool xD10ShaderParamTable::installBuffer()
{
	for(size_t i = 0 ; i < m_BufferBindings.size() ; i ++)
	{
		xD3D10RenderApi* pD10Api = (xD3D10RenderApi*) m_pD10Shader->renderApi();
		xD10BufferBinding& cbslot = m_BufferBindings[i];
		cbslot.validate();

		xD10InputBuffer* InputBuffer = (xD10InputBuffer*)cbslot.m_InputBuffer;

		ID3D10ShaderResourceView*    pRSV = NULL; 

		//找一个绑定的Buffer;
		if(InputBuffer == NULL && cbslot.m_bindType == eRBT_Buffer)
		{
			InputBuffer = (xD10InputBuffer*)pD10Api->findInputBuffer( cbslot.m_Name.c_str() );
		}

		if(InputBuffer)
		{
			pRSV =  InputBuffer->toShaderResourceView(PIXELFORMAT_R32G32B32A32F);
		}

		//判断绑定的Resource View
		if(pRSV == NULL)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"从Buffer构造ResourceView失败\n");
			continue;
		}

		eShaderType shType = m_pD10Shader->type();
		//Commit the buffer to D3D 10 Device;
		if(shType == eShader_VertexShader )
		{
			pD10Api->d10Device()->VSSetShaderResources( cbslot.m_bindPoint , 1 , &pRSV);
		}
		else if(shType == eShader_PixelShader)
		{
			pD10Api->d10Device()->PSSetShaderResources( cbslot.m_bindPoint , 1 , &pRSV);
		}
		else if(shType == eShader_GeometryShader)
		{
			pD10Api->d10Device()->GSSetShaderResources( cbslot.m_bindPoint , 1 , &pRSV);  
		}
	}
	return true;
}

bool xD10ShaderParamTable::commit()
{
     installSamplerTextureBinding();
     installConstBuffer();
     installBuffer();
	return true;
}

xD10ShaderParamTable::xD10ShaderParamTable(IBaseShader*   pD10Shader)
{
	m_pD10Shader = pD10Shader;
}

xD10ShaderParamTable::~xD10ShaderParamTable()
{
    unload();
}

size_t xD10ShaderParamTable::nConstant()
{
	return m_mapDesc.size();
}

IShaderConstantReflection* xD10ShaderParamTable::getConstDesc(size_t idx)
{
	IShaderConstantReflection* pDesc = m_vecDesc[idx];
	return pDesc;
}

IBaseShader* xD10ShaderParamTable::getShader()
{
	m_pD10Shader;
	return NULL;
}

/*
自动设置渲染API的状态：
包括: 
变换矩阵:          World , View , WorldView , Normal , MVP , InvWorld , InvView , InvWorldView .
灯光: 
fog:               Arg , Color.
TextureStage Arg:  纹理大小
*/

bool xD10ShaderParamTable::updateRenderContext()
{
	for(size_t i = 0 ; i < m_vecDesc.size() ; i ++)
	{
		xD10ShaderConstReflection* pConstDesc = m_vecDesc[i];
		pConstDesc->updateData();
	}
	return false;
}

//设置paramater;
template<typename T> bool  T_setParamater(mapShaderConstantDescs& constMap , const wchar_t* name , const T& _value)
{
	int hashName = xStringHash(name);
	mapShaderConstantDescs::iterator pos = constMap.find(hashName);
	if(pos == constMap.end())
		return false;

	
	xD10ShaderConstReflection* pConst = pos->second;
	return pConst->setData( (void*)&_value, sizeof(T) );
}

template<typename T> bool  T_setParamater(mapShaderConstantDescs& constMap , const wchar_t* name , T* _array , int n , int baseIndex)
{
	int hashName = xStringHash(name);
	mapShaderConstantDescs::iterator pos = constMap.find(hashName);
	if(pos == constMap.end())
		return false;
   
	int _offset = baseIndex * sizeof(T); 

	xD10ShaderConstReflection* pConst = pos->second;
	return pConst->setData(_offset ,  (void*)&_array, sizeof(T) * n );
}

bool xD10ShaderParamTable::setParamater(const wchar_t* name , float  _value)
{
	return T_setParamater(m_mapDesc , name , _value);
}

bool xD10ShaderParamTable::setParamater(const wchar_t* name , int    _value)
{
	return T_setParamater(m_mapDesc , name , _value);
}

bool xD10ShaderParamTable::setParamater(const wchar_t* name , xvec4& _value)
{
	return T_setParamater(m_mapDesc , name , _value);
}

bool xD10ShaderParamTable::setParamater(const wchar_t* name , xmat4& _value)
{
	return T_setParamater(m_mapDesc , name , _value);
}

bool xD10ShaderParamTable::setParamater(const wchar_t* name , xvec4* _array , int n , int baseIndex)
{
	return T_setParamater(m_mapDesc ,name , _array , n , baseIndex);
}

bool xD10ShaderParamTable::setParamater(const wchar_t* name , int*   _array , int n , int baseIndex)
{
	return T_setParamater(m_mapDesc ,name , _array , n , baseIndex);
}

bool xD10ShaderParamTable::setParamater(const wchar_t* name , float* _array , int n , int baseIndex)
{
	return T_setParamater(m_mapDesc ,name , _array , n , baseIndex);
}

bool xD10ShaderParamTable::setParamater(const wchar_t* name , xmat4* _array , int n , int baseIndex)
{
	return T_setParamater(m_mapDesc ,name , _array , n , baseIndex);
}




END_NAMESPACE_XEVOL3D

