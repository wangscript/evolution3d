#include "xKidRenderer.h"
#include "xKidRTech.h"
#include "../xRendererQueue.h"
#include "../xDrawElement.h"
#include "xKidMaterail.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(xKidRenderer , IBaseRenderer);


xKidRenderer::xKidRenderer(IRenderApi* pRenderApi) : IBaseRenderer(pRenderApi), m_XMLMgr(L"MatXMLNanager" , 1)
{
      m_pTech = new xKidBasicRenderTech(pRenderApi);
	  m_pTech->setRenderer(this);
}

xKidRenderer::~xKidRenderer()
{

}

bool xKidRenderer::loadConfig(const wchar_t* xmlName , const int8* xmlData , int len)
{
	xXmlDocument xml;
	bool bLoaded = false;
	if(xmlData == NULL)
		bLoaded = xml.load(xmlName);
	else
		bLoaded = xml.load(xmlData , len);
	if(bLoaded == false)
		return false;

	xXmlNode* pRootNode = xml.root() ? xml.root()->findNode(this->name()) : NULL;
	if(pRootNode == NULL)
		return false;
	xXmlNode* pMatRootNode = pRootNode->findNode(L"material");
	XEvol_AddResmanagerPath(m_XMLMgr , pMatRootNode);
	return IBaseRenderer::loadConfig(xmlName,xmlData , len);
}

bool xKidRenderer::drawPass(int idQueue, xRenderPass* pObject)
{
     return m_pTech->drawPass(idQueue , pObject);
}

bool xKidRenderer::drawPass(xRenderPass* pObject , unsigned int passedTime ,   bool bDrawImm)
{
     return pObject->render(passedTime);
}

bool xKidRenderer::setRenderTech(const wchar_t* techName)
{
	xKidBasicRenderTech* pRenderTech = xKidRTechFactoryMgr::singleton()->createInstance(techName , m_pRenderApi , this);
	if(pRenderTech == NULL)
		return false;
	if(m_pTech) 
	{
		m_pTech->ReleaseObject();

	}
	m_pTech = pRenderTech;
    return true;
}

IRenderTech* xKidRenderer::getRenderTech()
{
      return m_pTech;
}     

bool xKidRenderer::begin(unsigned int passedTime)
{

	return true;
}
bool xKidRenderer::end(unsigned int passedTime)
{
	m_pTech->commitRenderer(passedTime);
	return true;
}


xRenderPass* xKidRenderer::createPass(const wchar_t* passName)
{
    return new xRenderPass(this);
}

xMaterial* xKidRenderer::createMaterial(const wchar_t* matName)
{
	std::wstring _matXML = matName;
	_matXML += L".material";
	HXMLDoc hXMLDoc = m_XMLMgr.add(_matXML.c_str() , true , true );
	if(hXMLDoc.getResource() == NULL )
		return NULL;

	xKidBasicMaterial* pKidMat = new xKidBasicMaterial(m_pRenderApi);

	bool bRet = pKidMat->load( hXMLDoc->root() );

	if(bRet == true)
	{
		return pKidMat;
	}
	delete pKidMat ;
	return NULL;
}

IRenderEffect* xKidRenderer::createRenderEffect(const wchar_t* effectName)
{
    xKidBasicRenderEffect* pEffect = new xKidBasicRenderEffect(m_pRenderApi , this);
	if(effectName == std::wstring(L"KidBlendEffect"))
	{
		pEffect->setQueueName(L"transpency");
	}
	else
	{
		pEffect->setQueueName(L"default");
	}
	return pEffect;
}

size_t xKidRenderer::nRenderEffect(int idx)
{
	return 2;
}

const wchar_t* xKidRenderer::renderEffectName(int idx)
{
   if(idx == 0)
	   return L"KidBasicEffect";
   else
	   return L"KidBlendEffect";
}

size_t xKidRenderer::nMaterial()
{
    xXmlDocument doc;
	std::wstring matListFile = _ABSPATH(L"./KidRenderer/materials.xml");
	if(doc.load(matListFile.c_str()) == false)
		return 0;
	xXmlNode* pRootNode = doc.root();
	if(pRootNode==NULL)
		return 0;
	xXmlNode::XmlNodes matNodes;
	pRootNode->findNode(L"material" , matNodes);
	size_t nMat = matNodes.size();
	doc.unload();
	return nMat;
}

const wchar_t* xKidRenderer::materialName(int idx)
{
	xXmlDocument doc;
	std::wstring matListFile = _ABSPATH(L"./KidRenderer/materials.xml");
	if(doc.load(matListFile.c_str()) == false)
		return NULL;
	xXmlNode* pRootNode = doc.root();
	if(pRootNode==NULL)
		return NULL;
	xXmlNode::XmlNodes matNodes;
	pRootNode->findNode(L"material" , matNodes);
	size_t nMat = matNodes.size();
	if(idx >= nMat)
		return NULL;
	static std::wstring _matName = matNodes[idx]->value(L"name");
	doc.unload();
	return _matName.c_str();
}



END_NAMESPACE_XEVOL3D
