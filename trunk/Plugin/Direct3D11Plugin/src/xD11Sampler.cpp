#include "stdafx.h"
#include "xD11Sampler.h"
#include "xD11ConstLexer.h"
#include "xDirect3D11API.h"
#include <BaseLib/xXmlDocument.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD11Sampler  , ISamplerState);
bool xD11Sampler::_destory()
{
    XSAFE_RELEASE(m_pD10Sampler);
	m_pD10Sampler = NULL;
	m_RefCount=1;
	return true;
}


bool xD11Sampler::_load(xXmlNode* node)
{
	/*
	<?xml version ="1.0" encoding="unicode" ?>
	<Sampler name=Default>
		<filter  min = "linear" mag = "linear"  mip = "point" anisotropy = "16"  compare = "never" />
		<address u = "clamp" v = "clamp" w = "clamp"/>
		<border  r = "1.0" g = "0.0" b = "0.0" a = "0.0" />
		<lod     min = "0" max="50" bias = "0.0" />
	</Sampler>
	*/
	m_name = node->value(L"name");
	D3D11_SAMPLER_DESC desc;
	//Filter;
	xXmlNode* pNode = node->findNode(L"filter");
	if(pNode)
	{
		const wchar_t* min_val = pNode->value(L"min");
		const wchar_t* mag_val = pNode->value(L"mag");
		const wchar_t* mip_val = pNode->value(L"mip");
		bool  bComp = pNode->bool_value(L"compare");
		desc.Filter =  xD11ConstLexer::GetFilter(min_val,mag_val,mip_val,bComp);

		const wchar_t* anisotropy_val = pNode->value(L"anisotropy");
		if(anisotropy_val)
			desc.MaxAnisotropy = pNode->int_value(L"anisotropy");

		const wchar_t* func_val = pNode->value(L"compare");
		if( func_val )
			desc.ComparisonFunc = xD11ConstLexer::GetCompareFunc(func_val);
	}

	pNode = node->findNode(L"address");
	if(pNode)
	{
		const wchar_t* u_val = pNode->value(L"u");
		const wchar_t* v_val = pNode->value(L"v");
		const wchar_t* w_val = pNode->value(L"w");
		desc.AddressU = xD11ConstLexer::GetAdress(u_val);
		desc.AddressV = xD11ConstLexer::GetAdress(v_val);
		desc.AddressW = xD11ConstLexer::GetAdress(w_val);
	}


	pNode = node->findNode(L"border");
	if(pNode)
	{
		desc.BorderColor[0] = pNode->float_value(L"r");
		desc.BorderColor[1] = pNode->float_value(L"g");
		desc.BorderColor[2] = pNode->float_value(L"b");
		desc.BorderColor[3] = pNode->float_value(L"a");
	}

	pNode = node->findNode(L"lod");
	if(pNode)
	{
		desc.MipLODBias  = pNode->float_value(L"bias");
		desc.MaxAnisotropy;
		desc.MinLOD = pNode->float_value(L"min");
		desc.MaxLOD = pNode->float_value(L"max");
	}

	m_pD11RenderAPI->d11Device()->CreateSamplerState(&desc,&m_pD10Sampler);
	return m_pD10Sampler != NULL;
}

xD11Sampler::xD11Sampler(xD3D11RenderApi* pAPI  )
:ISamplerState(pAPI)
{
	m_pD11RenderAPI = pAPI;
	m_pD10Sampler = NULL;
}
xD11Sampler::~xD11Sampler()
{
	_destory();
}
bool          xD11Sampler::load(const wchar_t* fileName , unsigned long  arg)
{
	xXmlDocument doc;
	if(false == doc.load(fileName , true) )
		return false;
	return _load(doc.root() );
}
bool          xD11Sampler::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(buf == 0 || bufLen == 0)
		return load(fileName , arg);
	xXmlDocument doc;
	if(false == doc.load((const wchar_t* )buf, bufLen ,  true) )
		return false;
	return _load(doc.root() );
}
bool          xD11Sampler::isLoaded()
{
	return m_pD10Sampler != NULL;
}
bool          xD11Sampler::unload()
{
	return _destory();
}
unsigned long xD11Sampler::memUsage()
{
	return 1;
}


END_NAMESPACE_XEVOL3D
