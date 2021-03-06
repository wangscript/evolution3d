#include "../xStdPch.h"
#include "xMemBuffer.h"
#include "xShaderLexer.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(IInputAssembler    , IRenderApiObject );
IMPL_BASE_OBJECT_CLASSID(IVertexStream      , IRenderApiObject );
IMPL_BASE_OBJECT_CLASSID(IInputBuffer       , IRenderApiObject );
xInputElementTypeInfo::xInputElementTypeInfo(const eShaderVarType& vet)
{
	  xShaderVarInfo info;
	  SHADER_VAR_PARSER()->shaderVarInfo(vet , info);
      m_dataType         = info.m_dataType;
	  m_nComponent       = info.m_nCol * info.m_nRow;
	  m_bytePerComponent = info.m_bytePerComponent;
	  m_stride           = m_nComponent * m_bytePerComponent;
	  type = vet;
}

const size_t xInputLayoutDesc::nStrideOfBuffer(size_t bufIdx) const
{
	size_t bufSize = 0;
	size_t nInputElements =  m_vInputElemets.size();
	for(size_t i = 0 ; i < nInputElements; i ++)
	{
		const xInputElement& _el = m_vInputElemets[i];
		if(_el.m_BufferIdx == bufIdx )
		{
			xInputElementTypeInfo ieti(_el.m_Type);
			bufSize += ieti.m_stride;
		}
	}
	return bufSize;
}

const size_t xInputLayoutDesc::nBuffer()
{
	size_t nBuffer = 0;
	size_t nInputElements =  m_vInputElemets.size();
	for(size_t i = 0 ; i < nInputElements; i ++)
	{
		xInputElement& _el = m_vInputElemets[i];
		if(_el.m_BufferIdx >= nBuffer )
		{
			nBuffer = _el.m_BufferIdx;
		}
	}
	return nBuffer + 1;
}

xInputBufferDesc& xInputLayoutDesc::inputSlotDesc(size_t slotIdx)
{
	if(slotIdx >= m_vInputBufferDescs.size() )
	{
		static xInputBufferDesc defDesc;
		return defDesc;
	}
	return m_vInputBufferDescs[slotIdx];
}

bool xInputLayoutDesc::addBufferDesc(xInputBufferDesc& desc)
{
    m_vInputBufferDescs.push_back(desc);
	return true;
}

bool xInputLayoutDesc::addBufferDesc( eResourceUsage usage,eResourceAccessFlage accessFlage,eResourceBindType type,bool bShared , bool   bCreateBuffer)
{
	xInputBufferDesc desc;
	desc.m_accessFlage = accessFlage;
	desc.m_bShared = bShared;
	desc.m_bindtype = type;
	desc.m_usage = usage;
	desc.m_bCreateBuffer = bCreateBuffer;
    m_vInputBufferDescs.push_back(desc);
	return true;
}

bool xInputLayoutDesc::addElement(xInputElement& element)
{
	 if(element.m_offset <= 0 )
	 {
		 element.m_offset = nStrideOfBuffer(element.m_BufferIdx);
	 }
     m_vInputElemets.push_back(element);
	 return true;
}

bool xInputLayoutDesc::addElement(wchar_t* semantic ,eShaderVarType veType , size_t sematicIdx , size_t bufIdx, int  bufOffset , eInputPerVertUsage inputUsage ,size_t  instanceDataStepRate)
{
     xInputElement el;
	 if(bufOffset < 0)
	 { 
		 bufOffset = (int)nStrideOfBuffer(bufIdx);
	 }

	 el.m_BufferIdx  = bufIdx;
	 el.m_offset = bufOffset;
	 el.m_InputUsage = inputUsage;
	 el.m_InstanceDataStepRate = instanceDataStepRate;
	 wcsncpy(el.m_Semantic , semantic , 16);
	 el.m_SemanticIdx = sematicIdx;
	 el.m_Type = veType;
	 m_vInputElemets.push_back(el);
	 return true;
}
xInputLayoutDesc::xInputLayoutDesc()
{
	m_vInputElemets.reserve(16);
}
xInputLayoutDesc::~xInputLayoutDesc()
{

}

END_NAMESPACE_XEVOL3D
