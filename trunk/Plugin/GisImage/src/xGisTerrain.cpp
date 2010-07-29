#include "stdafx.h"
#include "xGisTerrain.h"
#include "BaseLib/xLogger.h"
#include "Renderer/xRenderer.h"
#include "xScene/xSceneVisitor.h"
#include "XMathLib/xvolume.h"
#include "XMathLib/xGeomLib.h"
#include "RenderAPI/xRenderApiImp.h"
#include <xcomdoc/xdocfstream.h>
BEGIN_NAMESPACE_XEVOL3D;


using namespace XGeomLib;
using namespace XMathLib;

IMPL_BASE_OBJECT_CLASSID(xGisTerrainModel , xBaseModel);
IMPL_BASE_OBJECT_CLASSID(xGisTerrainMesh  , xCoreMesh );
IMPL_OBJECT_FACTORY(xGisTerrainModel , xBaseModel , xGisTerrainModelCreator , xBaseModelCreator , xBaseModelCreatorMgr , L"terrain"    , L"gis terrain model" , xBaseTextureMgr* , bool);

IDepthStencilState* xGisTerrainMeshObject::m_pUnderGroundStencil = NULL;
xGisTerrainMesh::xGisTerrainMesh(xBaseTextureMgr* pTexMgr , bool bSysMemCopy) 
: xCoreMesh(pTexMgr , NULL)
{
    
}

xGisTerrainMesh::~xGisTerrainMesh()
{

}


IInputAssembler* xGisTerrainMesh::createInputAssembler(IRenderApi* pRenderApi)
{
    wchar_t _name[32] = L"xGisTerrainMesh";
    IInputAssembler* pAss = pRenderApi->getInputAssembler(_name);
    if(pAss != NULL) 
        return pAss;
    xInputLayoutDesc InputDesc;
    int BuffIdx = 0;
    InputDesc.addElement(SHADER_SEMANTIC_POSITION     , SHADERVARTYPE_FLOAT4 , 0 , BuffIdx);
    InputDesc.addBufferDesc(RESOURCE_USAGE_DEFAULT    , RESOURCE_ACCESS_NONE);
    return pRenderApi->createInputAssembler(_name, InputDesc);
}

bool xGisTerrainMesh::load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg)
{
    
    //读入文件头
    ds_wstring descName = ds_wstring(_dir) + L"desc.xml";
    xcomdocstream* pdescstream = doc.open_stream(descName.c_str() );
    if(pdescstream == NULL)
        return false;
    std::xcdstream  _in(pdescstream);
    xXmlDocument xml;
    xml.load(_in);
    _in.close();
    doc.close_stream(pdescstream);
    m_aabb.Init(10000000000.0f);
    xXmlNode* pRootNode = xml.root();
    m_name     = pRootNode->value(L"name");
    m_nFace    = pRootNode->int_value(L"nFace");
    m_nVertex  = pRootNode->int_value(L"nVertex");
    m_nSubMesh = 1;//GisTerrain不需要多个子材质
    m_nUVMaps  = 0;//GisTerrain的Mesh不需要UV通道
    m_nFrame   = 1;//目前只需要一帧
    m_bSkin       = false;
    //读入SubMesh信息
    m_SubMeshs.resize(m_nSubMesh);
    xXmlNode::XmlNodes subMeshNodes;
    pRootNode->findNode(L"submesh", subMeshNodes);
    assert(subMeshNodes.size() == m_nSubMesh);
    for(int i = 0 ; i < m_nSubMesh ; i ++)
    {
        xSubMesh& subMesh      = m_SubMeshs[i];
        xXmlNode* pSubMeshNode = subMeshNodes[i];
        subMesh.m_iMatIdx = pSubMeshNode->int_value(L"nMatIndex");
        subMesh.m_nFace   = pSubMeshNode->int_value(L"nFace");
    }


    ds_wstring positionName = ds_wstring(_dir) + L"position";
    ds_wstring faceName     = ds_wstring(_dir) + L"face";


    xcomdocstream* pPositionStream = doc.open_stream(positionName.c_str() );
    xcomdocstream* pFaceStream     = doc.open_stream(faceName.c_str() );

    m_VertexBuffers.resize(m_nFrame);

    //如果不支持Gpu的Skin。那么就创建一个静态的SkinMesh
    m_pInputAss = createInputAssembler(this->m_pTexMgr->renderApi() );
    m_pVertexStream = m_pInputAss->createVertexStream();

    void* pInitData = NULL; 
    uint32 nVertex = 0;
    //如果数据能一次性读入，将优化读取速度，此处因我偷懒没优化。
    {
        pPositionStream->read(nVertex);
        xvec4*  vertexs = new xvec4[  nVertex ];
        for(size_t i = 0 ; i < nVertex ; i ++)
        {
            xvec4&      vert_pos =  vertexs[i];
            xvec3       pos ;
            xvec3       nrm;
            xColor_4ub  cl;
            xvec3       tan;
            pPositionStream->read(pos);
            vert_pos.x = pos.x ;
            vert_pos.y = pos.y ;
            vert_pos.z = pos.z ;
            vert_pos.w = 1.0f;

            m_aabb.AddPoint(pos);
        }
        m_VertexBuffers[0] = m_pVertexStream->createInputBuffer(0 , nVertex , vertexs , NULL);
        delete [] vertexs;

    }
    doc.close_stream( pPositionStream );


    //读入所有的面
    for(int i = 0 ; i < m_nSubMesh ; i ++)
    {
        int32 nFace ;
        int32 iMatIdx;
        pFaceStream->read(nFace);
        pFaceStream->read(iMatIdx);
        xvec3ui* pData = new xvec3ui [nFace];
        pFaceStream->read(pData , nFace);

        xInputBufferDesc idxBufDesc;
        idxBufDesc.m_usage = RESOURCE_USAGE_DEFAULT;
        idxBufDesc.m_accessFlage = RESOURCE_ACCESS_NONE;
        idxBufDesc.m_bindtype = BIND_AS_INDEX_BUFFER;
        m_SubMeshs[i].m_pIdxBuffer = m_pRenderApi->createInputBuffer( nFace *3  , 4 , &idxBufDesc, pData);
        delete pData;		
    }
    doc.close_stream( pFaceStream);

	return true;
}

bool xGisTerrainMesh::draw(xCoreSkeleton* pSkeleton , xCoreActionFrame* pActionFrame)
{
    if(pSkeleton == NULL) pSkeleton = m_pSkeleton;

    m_pRenderApi->setVertexStream( m_pVertexStream );

    for(int i = 0 ; i < m_nSubMesh ; i ++)
    {
         m_pRenderApi->draw(m_SubMeshs[i].m_pIdxBuffer , m_SubMeshs[i].m_nFace * 3 );
    }
    return true;	
}

//------------------------
xGisTerrainModel::xGisTerrainModel(xBaseTextureMgr* pTexMgr , bool bSysMemCopy)
:xBaseModel(pTexMgr , bSysMemCopy)
{
	 m_aabb.Init(100000000.0f);
}

xGisTerrainModel::~xGisTerrainModel()
{

}


//====================
bool xGisTerrainModel::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
    xcomdoc _doc ;
    if(_doc.open(fileName , xcdm_read) == false )
    {
         return false;
    }
	return load(_doc , L"" , arg);
}

bool xGisTerrainModel::load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg)
{
    xcomdocstream* pDescXml = doc.open_stream(L"desc.xml");
    if(pDescXml == NULL)
        return false;
    std::xcdstream _stream(pDescXml);
    xXmlDocument xmlDoc;
    if(false == xmlDoc.load(_stream , true) )
    {
        doc.close_stream(pDescXml);
        return false;
    }
    _stream.close();
    doc.close_stream(pDescXml);

    xXmlNode* pRootNode = xmlDoc.root();

    m_name = pRootNode->value(L"name");
    xXmlNode* pAABBNode = pRootNode->findNode(L"BoundBox");
    m_aabb.m_Min = xvec3(0.0f , 0.0f , 0.0f);
    m_aabb.m_Max = xvec3(1.0f , 1.0f , 1.0f);
    if(pAABBNode)
    {
        m_aabb.m_Min.x = pAABBNode->float_value(L"min_x");
        m_aabb.m_Min.y = pAABBNode->float_value(L"min_y");
        m_aabb.m_Min.z = pAABBNode->float_value(L"min_z");

        m_aabb.m_Max.x = pAABBNode->float_value(L"max_x");
        m_aabb.m_Max.y = pAABBNode->float_value(L"max_y");
        m_aabb.m_Max.z = pAABBNode->float_value(L"max_z");
    }


    xXmlNode::XmlNodes meshNodes;
    pRootNode->findNode(L"mesh" , meshNodes);
    size_t _MeshNodes = meshNodes.size() ;
    for(size_t i = 0 ; i <  _MeshNodes ; i ++)
    {
        xXmlNode* pMeshNode = meshNodes[i];
        const wchar_t* name = pMeshNode->value(L"name");
        xGisTerrainMesh* pMesh = new xGisTerrainMesh(m_pTexMgr , false);
        ds_wstring meshName = ds_wstring(_dir) + L"/" + name + L"/";
        if( pMesh->load(doc , meshName.c_str(), 0) == false)
        {
            delete pMesh;
            continue;
        }
        _setupMesh(pMesh);

    }    
    return m_Meshs.size() > 0;
}

void xGisTerrainModel::_setupMesh( xCoreMesh* pMesh) 
{
	m_Meshs.push_back(pMesh);
}

bool  xGisTerrainModel::drawMeshs(xCoreActionFrame* pActionFrame)
{
    size_t _nMeshs = m_Meshs.size();
    for(size_t i = 0 ; i <  _nMeshs; i ++)
    {
        m_Meshs[i]->draw(NULL , NULL);
    }
    return true;
}

bool  xGisTerrainModel::draw(xCoreActionFrame* pActionFrame)
{
    return drawMeshs(pActionFrame);
}

IMPL_BASE_OBJECT_DLLSAFE(xGisTerrainScene  , xTerrainBase);
IMPL_SCENENODE_FACTORY(xGisTerrainScene);
IMPL_SCENE_FACTORY(xGisTerrainScene     ,  L"xGisTerrainScene" );

IMPL_BASE_OBJECT_DLLSAFE(xGisTerrainMeshObject  , xTerrainBase);
IMPL_SCENEOBJECT_FACTORY_2(xGisTerrainMeshObject);

xGisTerrainMeshObject::xGisTerrainMeshObject(ISceneGraph*  pScene , int arg)
:xSceneModel(pScene , arg)
{
  
}

xGisTerrainMeshObject::~xGisTerrainMeshObject()
{

}

bool xGisTerrainMeshObject::load(xXmlNode* pXml)
{
    return xSceneModel::load(pXml);
}

bool xGisTerrainMeshObject::save(xXmlNode* pXml)
{
    if(false == xSceneModel::save(pXml) )
        return false ;
    pXml->setValue(L"type" , L"xGisTerrainMeshObject");
    return true;
}

bool xGisTerrainMeshObject::unload()
{
    return xSceneModel::unload();
}


size_t xGisTerrainMeshObject::nDrawElement()
{ 
    return 1 ;
}

IDrawElement* xGisTerrainMeshObject::drawElement(size_t idx) 
{
     return m_pModelElement;
}

bool xGisTerrainMeshObject::setDrawElement(IDrawElement* pDrawElement , size_t idx)
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}
bool  xGisTerrainMeshObject::updateFrame(unsigned long passedTime, IRenderCamera* pCamera)
{
    return true;
}

bool xGisTerrainMeshObject::isVisible(xGeomLib::xCamera* pCamera)
{
	return true;
}

bool  xGisTerrainMeshObject::draw(IBaseRenderer* pRenderer , xGeomLib::xCamera* pCamera)
{
	if(m_pUnderGroundStencil)
		pRenderer->renderApi()->setDepthStencilState(m_pUnderGroundStencil);

    size_t nElement = nDrawElement();
    for(size_t i = 0 ; i < nElement ; i ++)
    {
        IDrawElement*   pDrawElement = drawElement(i);
        IRenderEffect*  pEffect      = effect(i);
        if(pEffect )
        {
            pEffect->draw(pDrawElement , this);
        }
    }
    return true;
}


bool     xGisTerrainMeshObject::drawImm(IBaseRenderer* pRenderer , unsigned int passedTime  , xGeomLib::xCamera* pCamera)
{
    size_t nElement = nDrawElement();
    for(size_t i = 0 ; i < nElement ; i ++)
    {
        IDrawElement*   pDrawElement = drawElement(i);
        IRenderEffect*  pEffect      = effect(i);
        if(pEffect )
        {
            pEffect->drawImm(pDrawElement  , this , passedTime);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////
void         xGisTerrainLoDCtrller::setTerrainScene(xGisTerrainScene* pScene)
{
	m_pAreaNode = pScene;
}

const void*  xGisTerrainLoDCtrller::getValuePtr()
{
	return &m_pAreaNode->m_LoDLevel;
}

size_t       xGisTerrainLoDCtrller::getValueDataLen()
{
	return sizeof(m_pAreaNode->m_LoDLevel);
}
bool         xGisTerrainLoDCtrller::setValulePtr(const void* pData , size_t dataLen , int _offset)
{
	if(dataLen != sizeof(m_pAreaNode->m_LoDLevel))
		return false;
	return m_pAreaNode->setLoDLevel( *(int*)pData );

}
bool         xGisTerrainLoDCtrller::reAlloc(size_t iData)
{
	return false;
}

bool xGisTerrainScene::setLoDLevel(int lodLevel)
{
	m_LoDLevel = lodLevel;
	return true;
}

static float                        g_fTerrainAlpha = 1.0f;
static TConstantValueBinder<float>  g_ConstTerrainAlpha(g_fTerrainAlpha);


xGisTerrainScene::xGisTerrainScene(ISceneGraph* pScene , ISceneNode* pParent  )
: xTerrainBase(pScene , pParent )
, m_PropertySet(L"GisTerrain System" , _TRANSLATE(L"GisTerrain System") )
{
	if ( pScene )
	{
		m_pRenderer   = pScene->renderer();
		m_pResPathMgr = pScene->getPathManager();
		m_pTextureMgr = pScene->getTextureManger();
		m_pModelMgr   = pScene->getModelManager();
	}
	m_LoDLevel = 3;
	m_LoDCtrller.setTerrainScene(this);
	m_TypeName = L"xGisTerrainScene";

    //创建给骨骼动画用的BoneFrame;//
    IRenderApi* pRenderApi = m_pRenderer->renderApi();
    m_pGisTerrainMeshArgBuffer = pRenderApi->findInputBuffer(L"xGisTerrainMeshArg");
    if(m_pGisTerrainMeshArgBuffer == NULL )
    {
         m_pGisTerrainMeshArgBuffer = pRenderApi->createConstBuffer( sizeof(xGisTerrainMeshArg) ); 
         pRenderApi->setInputBuffer(L"xGisTerrainMeshArg" , m_pGisTerrainMeshArgBuffer);
    }

    m_viewDist = 1000.0f;
    m_GisTerrainMeshArg.TerrainScale = float4(m_viewDist , m_viewDist , 1.0f , 1.0f);
	m_DEMPath = L"./DEMPackage/";


	//地形管理
	m_pScene->renderer()->renderApi()->registeShaderConstBinder(L"fGisTerrainAlpha"      , &g_ConstTerrainAlpha );

}

float   xGisTerrainScene::getAlpha()
{
	return g_fTerrainAlpha;
}

void    xGisTerrainScene::setAlpha(float _alpha)
{
	g_fTerrainAlpha = _alpha;
}

xGisTerrainScene::~xGisTerrainScene()
{
	detachAllObject();
}

float xGisTerrainScene::height(float x , float y)
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
	return 1.0f;
}
bool  xGisTerrainScene::region(float& x , float& y , float& w , float& h)
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
    return true ; 
}
bool  xGisTerrainScene::resolution(float& x  , float& y )
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
	x = 5.0f ;
	y = 4.0f ; 
	return true ; 
}
bool xGisTerrainScene::load(xXmlNode* pNode)
{
	//Load thing right here.

	if(!xTerrainBase::load(pNode))
	{
		return false;
	}

    const wchar_t* _hScale = pNode->value(L"hScale");
    if(_hScale)
    {
        float fHScale = pNode->float_value(L"hScale");
        m_viewDist = pNode->float_value(L"viewDist");
        m_GisTerrainMeshArg.TerrainScale.x = m_viewDist;
        m_GisTerrainMeshArg.TerrainScale.y = m_viewDist;

        m_GisTerrainMeshArg.TerrainScale.z = fHScale;
    }

	const wchar_t* _demPath = pNode->value(L"demPackage");
	if(_demPath)
	{
		m_DEMPath = _demPath;
	}


    //设置DEM和遥感影像的纹理
     m_pRenderer->renderApi()->setTexture(L"DEMTextures"   , &m_DEMsArray);
     m_pRenderer->renderApi()->setTexture(L"ImageTextures" , &m_TextureArray);

    
    xXmlNode* pDemPkgs = pNode->findNode(L"DEMPkgs");
    xXmlNode::XmlNodes _demPkgNodes;
    pDemPkgs->findNode(L"DEMPkg" , _demPkgNodes );


    IBaseTexture* pBlankTexture = m_pTextureMgr->add(L"white.jpg" , 0 , true ).getResource() ;

    //清除上次加载留下来的痕迹
    m_vDemPackages.clear();
    m_vImagePackags.clear();
    m_DEMsArray.setArraySize(9);
    m_TextureArray.setArraySize(9);
    for(int i = 0 ; i < m_TextureArray.nSubTexture() ; i ++)
    {
        m_TextureArray.setSubTexture(i ,  pBlankTexture );
        m_DEMsArray.setSubTexture   (i ,  pBlankTexture );
    }

    //
    int nImage = 0;
    for(size_t i = 0 ; i < _demPkgNodes.size() ; i ++)
    {
        xXmlNode* pPkgNode = _demPkgNodes[i];
        const wchar_t* demName = pPkgNode->value(L"dem");
        const wchar_t* texName   = pPkgNode->value(L"texture");

        IBaseTexture* pDEMTexture = m_pTextureMgr->add(demName , 0 , true).getResource() ;
        IBaseTexture* pTexture    = m_pTextureMgr->add(texName , 0 , true).getResource() ;
        if(pTexture == NULL) pTexture = pBlankTexture;

        if(pDEMTexture)
        {

            std::ds_wstring fullName = _RES_ABSPATH_(m_DEMPath.c_str() ) + std::ds_wstring(demName);
            xPDEMImage* pImage = new xPDEMImage();
            pImage->load(fullName.c_str() , NULL , 0 , PIXELFORMAT_R32F , IO_LEFT_LOWER , 0);

            xPDEMImage& _image = *pImage;
            m_DEMsArray.setSubTexture(nImage ,  pDEMTexture );
            m_TextureArray.setSubTexture(nImage ,  pTexture );
            
            if (nImage == 0)
            {
                m_vOffset.x = -_image.m_Region.m_MinX;
                m_vOffset.y = -_image.m_Region.m_MinY;
            }
            //设置区域 
            m_GisTerrainMeshArg.DEMRegion[nImage].x   = _image.m_Region.m_MinX    +  m_vOffset.x ;
            m_GisTerrainMeshArg.DEMRegion[nImage].y   = _image.m_Region.m_MinY    +  m_vOffset.y ;
            m_GisTerrainMeshArg.DEMRegion[nImage].z   = _image.m_Region.m_Width   + _image.m_Region.m_MinX  +  m_vOffset.x ;
            m_GisTerrainMeshArg.DEMRegion[nImage].w   = _image.m_Region.m_Length  + _image.m_Region.m_MinY  +  m_vOffset.y ;

            m_GisTerrainMeshArg.ImageRegion[nImage].x = _image.m_Region.m_MinX   +  m_vOffset.x;
            m_GisTerrainMeshArg.ImageRegion[nImage].y = _image.m_Region.m_MinY   +  m_vOffset.y;
            m_GisTerrainMeshArg.ImageRegion[nImage].z = _image.m_Region.m_Width   + _image.m_Region.m_MinX  +  m_vOffset.x ;
            m_GisTerrainMeshArg.ImageRegion[nImage].w = _image.m_Region.m_Length  + _image.m_Region.m_MinY  +  m_vOffset.y ;


            xTerrainImageInfo _demInfo;
            _demInfo.m_Region    = _image.m_Region;
            _demInfo.m_ImageFile = demName;
            _demInfo.m_pImage    = pImage;

			_demInfo.m_TextureFile = _RES_ABSPATH_(m_DEMPath.c_str() ) + std::ds_wstring(texName);

            xTerrainImageInfo _imgInfo;
            _imgInfo.m_Region = _image.m_Region;
			_imgInfo.m_ImageFile = texName;
			_imgInfo.m_TextureFile = _RES_ABSPATH_(m_DEMPath.c_str() ) + std::ds_wstring(texName);


            m_vDemPackages.push_back(_demInfo);
            m_vImagePackags.push_back(_imgInfo);
            nImage ++;
        }

        

    }

    for(int i = 0 ; i < m_TextureArray.nSubTexture() ; i ++)
    {
        //设置DEM纹理
        wchar_t _name[32] = {0};
        swprintf(_name , L"DEMTextures[%d]" , i);
        m_pRenderer->renderApi()->setTexture(_name   , m_DEMsArray.subTexture(i)    );
        //设置遥感影像的纹理
        swprintf(_name , L"ImageTextures[%d]" , i);
        m_pRenderer->renderApi()->setTexture(_name   , m_TextureArray.subTexture(i) );
    }

    m_GisTerrainMeshArg.nDEM = int4(nImage, nImage , 0 , 0 );

    this->getObjects(m_MeshObjects , true);
	return true;
}

size_t             xGisTerrainScene::nDem()
{
    return m_vDemPackages.size();
}
xGisImageRegion&   xGisTerrainScene::getDemRegion(size_t idx)
{
    return m_vDemPackages[idx].m_Region;
}

const wchar_t*      xGisTerrainScene::getDemPath()
{
	return m_DEMPath.c_str();
}
 const xTerrainImageInfo&  xGisTerrainScene::getImgInfo(size_t idx)
 {
	 return m_vImagePackags[idx];
 }

 const xTerrainImageInfo&  xGisTerrainScene::getDemInfo(size_t idx)
 {
	 return m_vDemPackages[idx];
 }

bool xGisTerrainScene::save(xXmlNode* pXml)
{
	if(pXml == NULL)
		return false;
    if(xTerrainBase::save(pXml) == false )
        return false;
	pXml->setValue(L"type"       , L"xGisTerrainScene");
	pXml->setValue(L"name"       , m_Name.c_str() );
    pXml->setValue(L"hScale"     , m_GisTerrainMeshArg.TerrainScale.z );
    pXml->setValue(L"viewDist"   , m_viewDist );
    pXml->setValue(L"demPackage" , m_DEMPath.c_str() );

    xXmlNode* pDemPkgs = pXml->insertNode(L"DEMPkgs");
    for(size_t i = 0; i < m_vDemPackages.size() ; i ++)
    {
        xXmlNode* pPkgNode = pDemPkgs->insertNode(L"DEMPkg");
        pPkgNode->setValue(L"dem" , m_vDemPackages[i].m_ImageFile.c_str() );
        pPkgNode->setValue(L"texture" , m_vImagePackags[i].m_ImageFile.c_str() );
    }
	return true;
}

bool xGisTerrainScene::visit(ISceneVisitor* pVisitor , eVisitOrder _order , IRenderCamera* pCamera )
{
   if(pVisitor)
   {
       return pVisitor->visit(this , m_pScene);
   }
   return false;
}

bool xGisTerrainScene::_visit(ISceneNode* pSceneNode , ISceneVisitor* pVisitor , IRenderCamera* pCamera)
{
	return true ;
}

bool xGisTerrainScene::updateFrame(unsigned long passedTime, IRenderCamera* pCamera , bool bRecursive )
{
    //更新摄像机位置；
    xvec4 _camera_dist =  pCamera->getEye().m_EyePos - m_GisTerrainMeshArg.CameraPos;
    _camera_dist.z = 0.0f;
    _camera_dist.w = 0.0f;
    if(_camera_dist.len() > m_viewDist/7 )
	    m_GisTerrainMeshArg.CameraPos = pCamera->getEye().m_EyePos;
	m_GisTerrainMeshArg.CameraPos.z = 0.0f;
 
	//m_pRenderer->renderApi()->setTexture();
	m_pGisTerrainMeshArgBuffer->update(eLock_WriteDiscard , &m_GisTerrainMeshArg , sizeof(m_GisTerrainMeshArg) );

    const wchar_t* _gisQueueName = L"gisground";
    if(getAlpha() < 0.8f)
    {
        _gisQueueName = L"gisground-transpency";
    }
    for(int i = 0 ; i < m_MeshObjects.size() ; i ++)
    {
        xGisTerrainMeshObject* pMeshObject = m_MeshObjects[i];
        if( pMeshObject->effect(0) )
        {
            pMeshObject->effect(0)->setArg(L"queue" , _gisQueueName);
        }
    }

	return ISceneNode::updateFrame(passedTime , pCamera , bRecursive);
}

bool xGisTerrainScene::proccessOverlap(xPDEMImage* pImage)
{
    //处理重叠的部分。注意处理的时候，可能会影响已有的Image.
    return  true;
}

void xGisTerrainScene::fitTopCamera(xCamera* pCamera)
{
     int iNearest = 0;
     float minLen = 100000000000000000000.0f;
     float2 CameraPosRel = pCamera->m_Eye.m_EyePos.xy();
     CameraPosRel.x -= m_vOffset.x;
     CameraPosRel.y -= m_vOffset.y;

     for(size_t i = 0 ; i < m_vDemPackages.size() ; i ++)
     {
         //如果摄像机就在这个区域内部。那就好说了
         if(m_vDemPackages[i].m_Region.PtInRect( CameraPosRel ) ) 
         {
             iNearest = i;
             break;
         }

         float2 _cen =  float2( m_vDemPackages[i].m_Region.m_MinX   + m_vDemPackages[i].m_Region.m_Width  / 2.0f ,
                                m_vDemPackages[i].m_Region.m_MinY   + m_vDemPackages[i].m_Region.m_Length / 2.0f );
         float2 _len = CameraPosRel - _cen;
         float  _fLen = _len.fabs();
         if( _fLen < minLen) 
         { 
             minLen = _fLen ; 
             iNearest = i;
         }
     }
     //找到离得最近那一个DEM。

     float2 cameraPos = m_vDemPackages[iNearest].m_Region.GetNearestPt( CameraPosRel ) ; 
     float  h =   m_vDemPackages[iNearest].m_Region.m_Height + m_vDemPackages[iNearest].m_Region.m_MinZ;

     pCamera->m_Eye.m_EyePos    = float4( cameraPos.x + m_vOffset.x  , cameraPos.y + m_vOffset.y , h + m_vDemPackages[iNearest].m_Region.m_Width / 5.0f , 1.0f );
     pCamera->m_Eye.m_EyeTarget = float4( cameraPos.x + m_vOffset.x  , cameraPos.y + m_vOffset.y , h                                                    , 1.0f );
     pCamera->m_Eye.m_Up        = float4( 0.0f , 1.0f , 0.0f , 1.0f);
     pCamera->dirty();
}

bool xGisTerrainScene::beginUndergroundEdit()
{

	xGisTerrainMeshObject::m_pUnderGroundStencil = m_pRenderer->renderApi()->createDepthStencilState(L"Overlay");
	setAlpha(0.5f);
	return true;
}

bool xGisTerrainScene::endUndergroundEdit()
{
	xGisTerrainMeshObject::m_pUnderGroundStencil = m_pRenderer->renderApi()->createDepthStencilState(L"Default");
	setAlpha(1.0f);
	return true;

}

//xTerrainImageInfo& xGisTerrainScene::getImageInfo(size_t idx)
//{
//	return m_vImagePackags[idx];
//}
//
//xTerrainImageInfo& xGisTerrainScene::getDemInfo(size_t idx)
//{
//	return m_vDemPackages[idx];
//}

size_t xGisTerrainScene::nTexture()
{
	return m_vImagePackags.size();
}

void xGisTerrainScene::removeDEM( size_t idx )
{
	vTerrainImages listTempDEMPackge;
	vTerrainImages listTempImagePackge;
	for ( size_t i=0; i<m_vDemPackages.size(); i++ )
	{
		if ( i != idx )
		{
			listTempDEMPackge.push_back( m_vDemPackages[i]);
		}
	}
	for ( size_t i=0; i<m_vImagePackags.size(); i++ )
	{
		if ( i != idx )
		{
			listTempImagePackge.push_back( m_vImagePackags[i]);
		}
	}

	listTempDEMPackge.swap( m_vDemPackages );
	listTempImagePackge.swap( m_vImagePackags );
}

float xGisTerrainScene::getOffsetX()
{
	return m_vOffset.x;
}

float xGisTerrainScene::getOffsetY()
{
	return m_vOffset.y;
}
END_NAMESPACE_XEVOL3D