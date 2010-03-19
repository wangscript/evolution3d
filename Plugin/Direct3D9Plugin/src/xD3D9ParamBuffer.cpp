#include "stdafx.h"
#include "xD3D9ParamBuffer.h"
#include <BaseLib/xStringHash.h>
#include <BaseLib/xI18N.h>
#include <RenderAPI/xShaderSemantic.h>
#include "xD3D9ConstLexer.h"
#include "xD3D9Shader.h"
#include "xD3D9InputBuffer.h"
#include "xD3D9ConstantBuffer.h"
#include "xDirect3D9API.h"
#include "xD3DTLEnviroment.h"
BEGIN_NAMESPACE_XEVOL3D
#define MAX_BONE 72

static bool isAStruct(D3DXCONSTANT_DESC& ConstDesc)
{
    if(ConstDesc.Class == D3DXPC_STRUCT ) return true;
    return false;
}
static bool isAConstant(D3DXCONSTANT_DESC& ConstDesc)
{
    if(ConstDesc.Class == D3DXPC_SCALAR ) return true;
    if(ConstDesc.Class == D3DXPC_VECTOR ) return true;
    if(ConstDesc.Class == D3DXPC_MATRIX_ROWS ) return true;
    if(ConstDesc.Class == D3DXPC_MATRIX_COLUMNS ) return true;
    return false;
}
static bool isATexture(D3DXCONSTANT_DESC& ConstDesc)
{
    if(ConstDesc.Type  == D3DXPT_SAMPLER ) return true;
    if(ConstDesc.Type == D3DXPT_SAMPLER1D ) return true;
    if(ConstDesc.Type == D3DXPT_SAMPLER2D ) return true;
    if(ConstDesc.Type == D3DXPT_SAMPLER3D ) return true;
    if(ConstDesc.Type == D3DXPT_SAMPLERCUBE ) return true;
    return false;
}


xD3D9BufferBinding::xD3D9BufferBinding()
{
	m_bindType  = eRBT_CBuffer;
    m_pReflection =  NULL;
	m_InputBuffer = NULL;
}

void xD3D9BufferBinding::unload()
{
	XSAFE_RELEASEOBJECT(m_InputBuffer);
}

xD3D9BufferBinding::~xD3D9BufferBinding()
{

}

void copyMatrix(xmat4& Source , xvec4* pDest)
{
     xvec4& v1 = pDest[0];
     Source.transpose();
     xvec4*    pSource = (xvec4*)&Source;
     pDest[0] = pSource[0];
     pDest[1] = pSource[1];
     pDest[2] = pSource[2];
}

struct cLightState
{   
    int4       nLight;  

    float4     m_Ambient [8];
    float4     m_Diffuse [8];
    float4     m_Speculer[8];
    float4     m_Position[8];
    float4     m_dir     [8];
};

bool xD3D9BufferBinding::validate(IDirect3DDevice9* pDevice)
{
   if(m_InputBuffer == NULL)
	   return false;
   xD3D9ConstBufferReflection* 	 pReflection =   m_pReflection == NULL ? (xD3D9ConstBufferReflection*)m_InputBuffer->reflection() : m_pReflection;
   if(pReflection == NULL) 
	   return false;

   if(this->m_Name == L"BoneFrame" && pReflection->dirty() )
   {
        int nBone = m_Bytes/sizeof(xmat4x3);
        xmat4*   pSource = (xmat4*)m_InputBuffer->handle();
        xvec4*   pDest   = (xvec4*)m_InputBuffer->handle();
        for(int i = 0 ; i < nBone ; i ++)
        {
            xmat4&    Source = pSource[i];
            xvec4&    Dest   = pDest[i * 3];
            copyMatrix(Source , &Dest);
        }
        pReflection->setDirty(false);
   }


   if(this->m_Name == L"xLightState"  )
   {
       cLightState LightStat ;
       xD1xLightEnvBuffer* pLightEnv = (xD1xLightEnvBuffer*)m_InputBuffer->handle();

       LightStat.nLight.x = pLightEnv->nLight[0];
       for(int i = 0 ; i < 8 ; i ++)
       {
           LightStat.m_Ambient [i] = pLightEnv->m_Light[i].m_Ambient ;
           LightStat.m_Diffuse [i] = pLightEnv->m_Light[i].m_Diffuse ;
           LightStat.m_Speculer[i] = pLightEnv->m_Light[i].m_Speculer;
           LightStat.m_Position[i] = pLightEnv->m_Light[i].m_position;
           LightStat.m_dir     [i] = pLightEnv->m_Light[i].m_dir     ;
       }

       xD3D9ConstantBuffer* InputBuffer = (xD3D9ConstantBuffer*)m_InputBuffer;
       int len = min(InputBuffer->bufLen() , m_Bytes );
       //如果这是一个单一的结构体，没有嵌套结构体;。直接搞定它
       m_pConstTable->SetValue(pDevice , m_hConstHandle , &LightStat , len );
       
       return false;
   }

   return pReflection->flushToBuffer();
}


static eShaderVarDataType toParamDataType(D3DXPARAMETER_TYPE _type)
{
     switch( _type )
	 {
     case D3DXPT_VOID:
         return SEDT_STRUCT;
     break;
     case D3DXPT_BOOL:
         return SEDT_BOOL;
         break;
     case D3DXPT_INT:
         return SEDT_INT32;
         break;
     case D3DXPT_FLOAT:
         return SEDT_FLOAT32;
         break;
     case D3DXPT_STRING:
         return SEDT_STRING;
         break;
     case D3DXPT_TEXTURE:
         return SEDT_TEXTURE;
         break;
     case D3DXPT_TEXTURE1D:
         return SEDT_TEXTURE1D;
         break;
     case D3DXPT_TEXTURE2D: 
         return SEDT_TEXTURE2D;
         break;
     case D3DXPT_TEXTURE3D:
         return SEDT_TEXTURE3D;
         break;
     case D3DXPT_TEXTURECUBE:
         return SEDT_TEXTURECUBE;
         break;
     case D3DXPT_SAMPLER:
         return SEDT_SAMPLER;
         break;
     case D3DXPT_SAMPLER1D:
         return SEDT_SAMPLER;
         break;
     case D3DXPT_SAMPLER2D:
         return SEDT_SAMPLER;
         break;
     case D3DXPT_SAMPLER3D:
         return SEDT_SAMPLER;
         break;
     case D3DXPT_SAMPLERCUBE:
         return SEDT_SAMPLER;
         break;
     case D3DXPT_PIXELSHADER:
     case D3DXPT_VERTEXSHADER:
     case D3DXPT_PIXELFRAGMENT:
     case D3DXPT_VERTEXFRAGMENT:
     default:
         return SEDT_ANY;
         break;
	 }
	 return SEDT_ANY;
}


static eShaderVarType toParamClass (eShaderVarDataType dataType , D3DXPARAMETER_CLASS varClass , int row , int colum)
{
	switch(varClass)
	{
	case  D3DXPC_SCALAR:
	case  D3DXPC_VECTOR:
	case  D3DXPC_MATRIX_ROWS:
	case  D3DXPC_MATRIX_COLUMNS:
		return (eShaderVarType) ( MAKE_SHADER_VAR_TYPPE(dataType , row , colum) );

	case  D3DXPC_OBJECT:
		XEVOL_LOG(eXL_DEBUG_HIGH,L"Shader Class 为 D3D10_SVC_OBJECT , 不支持\n" );
		return SHADERVARTYPE_Unkown;
	case  D3DXPC_STRUCT:
		return SHADERVARTYPE_STRUCT;
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

IMPL_BASE_OBJECT_CLASSID(xD3D9ShaderParamTable , IShaderParamTable);


void  xD3D9ShaderParamTable::AddConstReflection( xD3D9ShaderConstReflection* constDesc, int VarOffset, D3DXHANDLE hConstHandle, D3DXCONSTANT_DESC &_desc, wchar_t* varName, xD3D9BufferBinding &cbSlot ) 
{
    constDesc->m_Bytes     = _desc.Bytes;
    constDesc->m_bufOffset = VarOffset;
    constDesc->m_hConstHandle = hConstHandle;
    constDesc->m_pD3D9Api = dynamic_cast<xD3D9RenderApi*>(m_pBaseShader->renderApi());


    eShaderVarDataType dataType  = toParamDataType(_desc.Type) ;

    constDesc->m_element   = _desc.Elements;
    constDesc->m_strName   = varName;
    constDesc->m_hashName  = xStringHash(varName);
    constDesc->m_varType   = toParamClass(dataType , _desc.Class, _desc.Rows , _desc.Columns );
    constDesc->m_bindType  = cbSlot.m_bindType;

    xShaderVarInfo info;
    SHADER_VAR_PARSER()->shaderVarInfo(constDesc->m_varType , info);
    constDesc->m_semantic     = xShaderSemanticTable::singleton().parser(  varName );
    constDesc->m_pBinder      = m_pBaseShader->renderApi()->getShaderConstBinder( varName );
    constDesc->m_pBufferReflection = cbSlot.m_pReflection;
    if(constDesc->m_semantic != eSPS_NotUsed )
    {
        //这里表示找到一个是需要自动设置变量数据的
    }	

}

bool xD3D9ShaderParamTable::createBufferVarList(ID3DXConstantTable* pShaderReflection , D3DXHANDLE hParentHandle ,D3DXCONSTANT_DESC& ConstDesc , xD3D9BufferBinding& cbSlot, int bufIdex)
{
	if(cbSlot.m_pReflection == NULL)
		return false;
    int VarOffset = 0;
    if(cbSlot.m_pReflection && cbSlot.m_pReflection->nConstant() > 0)
        return false;
	//开始建立所有的变量
	for(size_t i = 0 ; i < ConstDesc.StructMembers ; i ++ )
	{
		D3DXHANDLE hConstHandle = pShaderReflection->GetConstant(hParentHandle , i);
        D3DXCONSTANT_DESC _desc;
        UINT _nConstant = 1;
        pShaderReflection->GetConstantDesc(hConstHandle , &_desc , &_nConstant);

		wchar_t varName[32]={0};
		XEvol_LocaleToUnicode(_desc.Name , varName , 32);

		xD3D9ShaderConstReflection* constDesc = cbSlot.m_pReflection->addConstant(varName);
        constDesc->m_pConstTable = pShaderReflection;
        AddConstReflection(constDesc, VarOffset, hConstHandle, _desc, varName, cbSlot);
        m_mapDesc.insert( mapD3D9ShaderConstantDescs::value_type(constDesc->m_hashName , constDesc) );
        if(isAStruct(_desc))
        {
            cbSlot.m_hConstHandle = NULL;
        }
        VarOffset += _desc.Bytes;
		//m_iName = xStringHash(m_name);
	}
	return true;
}


bool xD3D9ShaderParamTable::load(ID3DXConstantTable* pShaderReflection )
{
    if(pShaderReflection == NULL)
        return false;
    
    D3DXCONSTANTTABLE_DESC _TableDesc;
    pShaderReflection->GetDesc(&_TableDesc);
    for(int i = 0 ;i < (int)_TableDesc.Constants ; i++)
    {
        //第一层
        D3DXCONSTANT_DESC ConstDesc;
        D3DXHANDLE hConstHandle =  pShaderReflection->GetConstant(NULL , i );
        UINT nCount  = 1; 
        pShaderReflection->GetConstantDesc(hConstHandle , &ConstDesc , &nCount);

        //如果是结构体,那么表示这可能是一个ConstBuffer的结构
        if( isAStruct(ConstDesc))
        {
            wchar_t bufName[64] = {0};
            XEvol_LocaleToUnicode(ConstDesc.Name , bufName, 64);
            IInputBuffer* pConstBuffer = m_pBaseShader->renderApi()->findInputBuffer(bufName);
            xD3D9ConstBufferReflection* pBufReflection = NULL;
            if(pConstBuffer == NULL)
            {
                pConstBuffer                 = m_pBaseShader->renderApi()->createConstBuffer( ConstDesc.Bytes );
                pBufReflection = (xD3D9ConstBufferReflection*)pConstBuffer->createReflection(true);
            }
            else
            {
                pConstBuffer->AddRef();
                pBufReflection = (xD3D9ConstBufferReflection*)pConstBuffer->reflection();
            }

            //Buffer已经建立，加到列表里
            m_ConstBufferBindings.push_back( xD3D9BufferBinding() );

            xD3D9BufferBinding& cbSlot =  m_ConstBufferBindings[ m_ConstBufferBindings.size() - 1];
            cbSlot.m_InputBuffer = pConstBuffer;
            cbSlot.m_Name        = bufName;
            cbSlot.m_bindType    = eRBT_CBuffer;
            cbSlot.m_pReflection = pBufReflection;
            cbSlot.m_hConstHandle = hConstHandle;
            cbSlot.m_pConstTable  = pShaderReflection;
            cbSlot.m_Bytes        = ConstDesc.Bytes;
            createBufferVarList(pShaderReflection , hConstHandle, ConstDesc ,  cbSlot , (int)m_ConstBufferBindings.size() - 1 );
            continue;
        }
        else if(isAConstant(ConstDesc) )
        {
            xD3D9BufferBinding cbSlot;
            cbSlot.m_hConstHandle = NULL;
            cbSlot.m_InputBuffer = NULL;
            cbSlot.m_bindType = eRBT_CBuffer;
            cbSlot.m_Name = L"$Global";
            cbSlot.m_pReflection = NULL;

            wchar_t varName[64] = {0};
            XEvol_LocaleToUnicode(ConstDesc.Name , varName, 64);
            xD3D9ShaderConstReflection* constDesc = new xD3D9ShaderConstReflection(varName);
            constDesc->m_pConstTable = pShaderReflection;
            AddConstReflection(constDesc, 0, hConstHandle, ConstDesc, varName, cbSlot);
            //将ConstDesc插入到两个查找队列里。一个顺序，一个hash
            m_vecDesc.push_back( constDesc );
        }
        else if(isATexture(ConstDesc) )
        {
            xD3D9TextureBinding texer;
            texer.m_iShaderSlot = ConstDesc.RegisterIndex;
            memset(texer.m_Name,0, 64);
            XEvol_LocaleToUnicode(ConstDesc.Name , texer.m_Name, 32 );
            //找到绑定到哪个Texture上了
            wchar_t slotName[32];
            wcsncpy(slotName , texer.m_Name , 32);
            ClipName(slotName , L"Texture" );
            texer.m_iTexUsage = xD3D9ConstLexer::GetTextureSlotIdx(slotName);
            texer.m_iName     = xStringHash(texer.m_Name);
            texer.m_pTexture  = NULL;
            m_TextureBindings.push_back(texer);
        }


        continue;
    }
    return true;
}

bool xD3D9ShaderParamTable::unload()
{
	//清除所有的标记
    size_t _nBufBinds = m_BufferBindings.size();
	for(size_t i = 0 ;  i <  _nBufBinds; i ++)
	{
		m_BufferBindings[i].unload();
	}

	_nBufBinds = m_ConstBufferBindings.size();
	for(size_t i = 0 ;  i < _nBufBinds ; i ++)
	{
		m_ConstBufferBindings[i].unload();
	}

	m_BufferBindings.clear();
	m_ConstBufferBindings.clear();

	m_mapDesc.clear();
	m_vecDesc.clear();
	return true;
}


bool xD3D9ShaderParamTable::setTexture(const wchar_t* textureName , IBaseTexture* pTexture)
{
	int texName = xStringHash(textureName);
	return setTexture(texName , pTexture);
}

bool xD3D9ShaderParamTable::setTexture(const int texName          , IBaseTexture* pTexture)
{
	size_t _nTexBinding = m_TextureBindings.size();
	for(size_t i  = 0 ; i <  _nTexBinding; i ++)
	{
		xD3D9TextureBinding& binder = m_TextureBindings[i];
		if(texName == binder.m_iName)
		{
			binder.m_pTexture = pTexture;
			return true;
		}
	}
	return false;
}

bool xD3D9ShaderParamTable::setTextureBySlot(const int iSlot      , IBaseTexture* pTexture)
{
	size_t _nTexBinding = m_TextureBindings.size();
	for(size_t i  = 0 ; i <  _nTexBinding; i ++)
	{
		xD3D9TextureBinding& binder = m_TextureBindings[i];
		if(iSlot == binder.m_iShaderSlot)
		{
			binder.m_pTexture = pTexture;
			return true;
		}
	}
	return false;
}

bool xD3D9ShaderParamTable::installSamplerTextureBinding()
{
	xD3D9RenderApi* pD3D9Api = (xD3D9RenderApi*) m_pBaseShader->renderApi();
	size_t _nSamplerBinding = m_SamplerBindings.size() ;
	for(size_t i  = 0 ; i < _nSamplerBinding ; i ++)
	{
		xD3D9SamplerBinding& binder = m_SamplerBindings[i];
		if(binder.m_pSampler)
		{
			pD3D9Api->setSamplerState(m_pBaseShader->type() , binder.m_iShaderSlot , binder.m_pSampler);
		}
	}

	size_t _nTexBinding = m_TextureBindings.size();
	for(size_t i  = 0 ; i <  _nTexBinding; i ++)
	{
		xD3D9TextureBinding& binder = m_TextureBindings[i];

		//首先要检查binder.m_pTexture
		IBaseTexture* pTexture = binder.m_pTexture ;

		//如果为空，就检查是不是有自动绑定的设置
		if(pTexture == NULL)
		{
			if(binder.m_iTexUsage != -1)
			{
				pTexture = pD3D9Api->getTexture(binder.m_iTexUsage);
			}
			else
			{
				pTexture = pD3D9Api->getTexture(binder.m_iName);
			}
		}

		//如果还为空，就从其它地方找
		if(pTexture != NULL)
		{
			pD3D9Api->getDevice()->SetTexture(binder.m_iShaderSlot , (IDirect3DBaseTexture9*)pTexture->handle() );
		}
		else
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"Missing texture bing texture name=%s\n",binder.m_Name);
            pD3D9Api->getDevice()->SetTexture(binder.m_iShaderSlot , NULL);
		}
	}
	return true;
}


bool xD3D9ShaderParamTable::installConstBuffer()
{
	size_t _nBufBinding = m_ConstBufferBindings.size() ;
	for(size_t i = 0 ; i < _nBufBinding ;  i ++)
	{
		xD3D9RenderApi* pD3D9Api = (xD3D9RenderApi*) m_pBaseShader->renderApi();
		xD3D9BufferBinding& cbslot = m_ConstBufferBindings[i];
		if(false == cbslot.validate( pD3D9Api->getDevice() ) )
            continue;

		xD3D9ConstantBuffer* InputBuffer = (xD3D9ConstantBuffer*)cbslot.m_InputBuffer;
        int len = min(InputBuffer->bufLen() , cbslot.m_Bytes );
        HRESULT hr = E_FAIL;

        //如果这是一个单一的结构体，没有嵌套结构体;。直接搞定它
        if(cbslot.m_hConstHandle)
        {
            cbslot.m_pConstTable->SetValue(pD3D9Api->getDevice() , cbslot.m_hConstHandle , InputBuffer->handle() , len );
        }
        else
        {
            //有嵌套结构体，郁闷一点。我们只支持一层嵌套。基本也够用了。不够用的找自己的老妈去。
            int _nConstant = cbslot.m_pReflection->nConstant();
            char* pSource = (char*)InputBuffer->handle();
            for(int i = 0 ; i < _nConstant ; i ++ )
            {
                 xD3D9ShaderConstReflection* constDesc = (xD3D9ShaderConstReflection*)cbslot.m_pReflection->constantDesc(i);
                 char* pData = pSource + constDesc->m_bufOffset;
                 cbslot.m_pConstTable->SetValue(pD3D9Api->getDevice() , constDesc->m_hConstHandle , pData , constDesc->m_Bytes );
            }
           
        }

        continue;
	}
	return true;
}


bool xD3D9ShaderParamTable::commit()
{
     installSamplerTextureBinding();
     installConstBuffer();
	return true;
}

xD3D9ShaderParamTable::xD3D9ShaderParamTable(IBaseShader*   pShader)
{
	m_pBaseShader = pShader;
}

xD3D9ShaderParamTable::~xD3D9ShaderParamTable()
{
    unload();
}

size_t xD3D9ShaderParamTable::nConstant()
{
	return m_mapDesc.size();
}

IShaderConstantReflection* xD3D9ShaderParamTable::getConstDesc(size_t idx)
{
	IShaderConstantReflection* pDesc = m_vecDesc[idx];
	return pDesc;
}

IBaseShader* xD3D9ShaderParamTable::getShader()
{
	m_pBaseShader;
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

bool xD3D9ShaderParamTable::updateRenderContext()
{
	size_t _nVecDesc = m_vecDesc.size();
	for(size_t i = 0 ; i <  _nVecDesc; i ++)
	{
		xD3D9ShaderConstReflection* pConstDesc = m_vecDesc[i];
		pConstDesc->updateData();
	}
	return false;
}

//设置paramater;
template<typename T> bool  T_setParamater(mapD3D9ShaderConstantDescs& constMap , const wchar_t* name , const T& _value)
{
	int hashName = xStringHash(name);
	mapD3D9ShaderConstantDescs::iterator pos = constMap.find(hashName);
	if(pos == constMap.end())
		return false;

	
	xD3D9ShaderConstReflection* pConst = pos->second;
	return pConst->setData( (void*)&_value, sizeof(T) );
}

template<typename T> bool  T_setParamater(mapD3D9ShaderConstantDescs& constMap , const wchar_t* name , T* _array , int n , int baseIndex)
{
	int hashName = xStringHash(name);
	mapD3D9ShaderConstantDescs::iterator pos = constMap.find(hashName);
	if(pos == constMap.end())
		return false;
   
	int _offset = baseIndex * sizeof(T); 

	xD3D9ShaderConstReflection* pConst = pos->second;
	return pConst->setData(_offset ,  (void*)&_array, sizeof(T) * n );
}

bool xD3D9ShaderParamTable::setParamater(const wchar_t* name , float  _value)
{
	return T_setParamater(m_mapDesc , name , _value);
}

bool xD3D9ShaderParamTable::setParamater(const wchar_t* name , int    _value)
{
	return T_setParamater(m_mapDesc , name , _value);
}

bool xD3D9ShaderParamTable::setParamater(const wchar_t* name , xvec4& _value)
{
	return T_setParamater(m_mapDesc , name , _value);
}

bool xD3D9ShaderParamTable::setParamater(const wchar_t* name , xmat4& _value)
{
	return T_setParamater(m_mapDesc , name , _value);
}

bool xD3D9ShaderParamTable::setParamater(const wchar_t* name , xvec4* _array , int n , int baseIndex)
{
	return T_setParamater(m_mapDesc ,name , _array , n , baseIndex);
}

bool xD3D9ShaderParamTable::setParamater(const wchar_t* name , int*   _array , int n , int baseIndex)
{
	return T_setParamater(m_mapDesc ,name , _array , n , baseIndex);
}

bool xD3D9ShaderParamTable::setParamater(const wchar_t* name , float* _array , int n , int baseIndex)
{
	return T_setParamater(m_mapDesc ,name , _array , n , baseIndex);
}

bool xD3D9ShaderParamTable::setParamater(const wchar_t* name , xmat4* _array , int n , int baseIndex)
{
	return T_setParamater(m_mapDesc ,name , _array , n , baseIndex);
}




END_NAMESPACE_XEVOL3D

