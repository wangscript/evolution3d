#ifndef _GIS_TERRAIN_H_
#define _GIS_TERRAIN_H_
#include "xGisImgBaseInclude.h"
#include "Renderer/xModelElement.h"
#include "xModel/xBaseModel.h"
#include "xModel/xCoreMesh.h"
#include "xScene/xSceneModel.h"

#include "xPDEMImage.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D


class _GISIMAGE_API_ xGisTerrainMesh  : public xCoreMesh
{
public:
    IMPL_BASE_OBJECT_INTERFACE(xGisTerrainModel);
    xGisTerrainMesh(xBaseTextureMgr* pTexMgr , bool bSysMemCopy);
    ~xGisTerrainMesh();
public:
    bool                    load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg);
    IInputAssembler*        createInputAssembler(IRenderApi* pRenderApi);
    bool                    draw(xCoreSkeleton* pSkeleton , xCoreActionFrame* pActionFrame);
};

class _GISIMAGE_API_ xGisTerrainModel : public xBaseModel
{
public:
    IMPL_BASE_OBJECT_INTERFACE(xGisTerrainModel);
    xGisTerrainModel(xBaseTextureMgr* pTexMgr , bool bSysMemCopy);
    ~xGisTerrainModel();
public:
    virtual bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
    virtual bool           load(xcomdoc& doc            , const wchar_t* _dir      , unsigned int arg);
    void                   aabb(xGeomLib::xaabb& _aabb){ _aabb = m_aabb ; }
    const xGeomLib::xaabb& aabb(){return m_aabb ;}
    bool                   draw(xCoreActionFrame* pActionFrame = NULL); 
    bool                   drawMeshs(xCoreActionFrame* pActionFrame = NULL); 
protected:
    void              _setupMesh( xCoreMesh* pMesh);
};


class _GISIMAGE_API_ xGisTerrainLoDCtrller : public IPropertyValue
{
    IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xGisTerrainLoDCtrller)
public:
    void          setTerrainScene(xGisTerrainScene* pScene);
public:
    const void*   getValuePtr();
    size_t        getValueDataLen();
    bool          setValulePtr(const void* pData , size_t dataLen , int _offset);
    bool          reAlloc(size_t iData);
public:
    xGisTerrainScene* m_pAreaNode;
};

struct _GISIMAGE_API_ xGisTerrainMeshArg
{
    float4  TerrainScale;
    float4  CameraPos;
    float4  DEMRegion[9];
    float4  ImageRegion[9];
    int4    nDEM;
};

class _GISIMAGE_API_ xGisTerrainMeshObject : public xSceneModel
{
public:
    DECL_BASE_OBJECT_DLLSAFE(xGisTerrainMeshObject);
    xGisTerrainMeshObject(ISceneGraph*  pScene , int arg);
    virtual ~xGisTerrainMeshObject();
    virtual bool           load(xXmlNode* pXml);
    virtual bool           save(xXmlNode* pXml);
    virtual bool           unload();
    virtual size_t         nDrawElement();
    virtual IDrawElement*  drawElement(size_t idx) ;
    virtual bool           setDrawElement(IDrawElement* pDrawElement , size_t idx);
    virtual bool           updateFrame(unsigned long passedTime, IRenderCamera* pCamera);
    virtual bool           draw(IBaseRenderer* pRenderer , xGeomLib::xCamera* pCamera);
    virtual bool           drawImm(IBaseRenderer* pRenderer , unsigned int passedTime  , xGeomLib::xCamera* pCamera);
	virtual bool           isVisible(xGeomLib::xCamera* pCamera);
public:
    xvec4                  m_Position;
	xvec4                  m_Size;
public:
	static IDepthStencilState * m_pUnderGroundStencil;
};

struct _GISIMAGE_API_ xTerrainImageInfo
{
    xGisImageRegion m_Region;
    std::ds_wstring m_ImageFile;
	std::ds_wstring m_textureFile;
    xImage*         m_pImage;
	std::ds_wstring m_TextureFile;


    xTerrainImageInfo()
    {
        m_ImageFile = L"";
        m_pImage    = NULL;
		m_TextureFile = L"";

    }
};

class _GISIMAGE_API_ xGisTerrainScene : public xTerrainBase
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xGisTerrainScene);
    friend class xGisTerrainLoDCtrller;
public:
	xGisTerrainScene(ISceneGraph*  pScene , ISceneNode* pParent = NULL);
	virtual ~xGisTerrainScene();
	
public:
    //Use by editor.
    bool                       proccessOverlap(xPDEMImage* pImage);
public:
	virtual bool               attachObject(ISceneObject* pObject) { return xSceneBasicNode::attachObject(pObject) ; }
	virtual bool               detachObject(ISceneObject* pObject) { return xSceneBasicNode::detachObject(pObject); }
	virtual bool               detachAllObject() { return xSceneBasicNode::detachAllObject() ; }
	virtual size_t             nObjects() { return xSceneBasicNode::nObjects() ; }
	virtual ISceneObject*      getObject(size_t idx ) { return xSceneBasicNode::getObject(idx) ; }


    //用于摄像机的控制，和2D面板的绘制
    virtual size_t             nDem();
	virtual size_t             nTexture();
	//virtual xTerrainImageInfo& getImageInfo(size_t idx);
	//virtual xTerrainImageInfo& getDemInfo(size_t idx);

    virtual xGisImageRegion&   getDemRegion(size_t idx);
    virtual const wchar_t*     getDemPath() ;
	virtual const xTerrainImageInfo& getImgInfo(size_t idx);
	virtual const xTerrainImageInfo& getDemInfo(size_t idx);
    virtual void               fitTopCamera(xCamera* pCamera);
	virtual void               removeDEM( size_t idx );
	virtual float              getOffsetX();
	virtual float              getOffsetY();
public:
	virtual bool               load(xXmlNode* pNode);
	virtual bool               save(xXmlNode* pNode);
	virtual bool               visit(ISceneVisitor* pVisitor , eVisitOrder _order ,  IRenderCamera* pCamera = NULL);
	virtual bool               updateFrame(unsigned long passedTime , IRenderCamera* pCamera , bool bRecursive);
	virtual float              getAlpha();
	virtual void               setAlpha(float _alpha);
	virtual bool               beginUndergroundEdit();
	virtual bool               endUndergroundEdit();
	xvec4r                     getOffset(){ return m_vOffset; }


protected:
	bool                       _visit(ISceneNode* pSceneNode , ISceneVisitor* pVisitor , IRenderCamera* pCamera = NULL);

protected:
	int                        m_LoDLevel; //野猪加的，用来由界面控制视野的精细程度
	xGisTerrainLoDCtrller      m_LoDCtrller;
	bool                       setLoDLevel(int lodLevel);
protected:
	BEGIN_SCENE_NODE_PROPERTY_SET(ISceneNodePropertySet , xSceneBasicNode);
	   DECL_SCENE_PROPERTY_VALUE( _TRANSLATE(L"LoD View Detail") , &m_LoDLevel );
	     ADD_DESCRIPTOR(L"data_type" , L"int");
	     ADD_DESCRIPTOR(L"control" , L"slider");
		 ADD_DESCRIPTOR(L"range_min" , 1);
		 ADD_DESCRIPTOR(L"range_max" , 5);
	  END_SCENE_PROPERTY(_TRANSLATE(L"LoD View Detail"));

	END_SCENE_NODE_PROPERTY_SET(ISceneNodePropertySet);

protected:
	//可读的属性
	virtual float height(float x , float y);
	//这个地形表示的区域
	virtual bool  region(float& x , float& y , float& w , float& h);
	// x , y 方向的分辨率
	virtual bool  resolution(float& x  , float& y );
	

	//可写的属性
	virtual bool  setHeight(float x , float y , float Height)
	{
		XEVOL_WARNNING_NOT_IMPLEMENT;
		return true; 
	}

protected:
    typedef std::vector<IBaseTexture*> vDemPkgTextures;
    typedef std::vector<xTerrainImageInfo> vTerrainImages;

    vTerrainImages                      m_vDemPackages;
    vTerrainImages                      m_vImagePackags;
                                        
    std::ds_wstring                     m_DEMPath;
    xGisTerrainMeshObject*              m_pMeshObject;
    IInputBuffer*                       m_pGisTerrainMeshArgBuffer;
    xGisTerrainMeshArg                  m_GisTerrainMeshArg;
    xBaseTextureArray                   m_TextureArray;
    xBaseTextureArray                   m_DEMsArray;
    float                               m_viewDist;
    xvec4r                              m_vOffset;
    ds_vector(xGisTerrainMeshObject*)   m_MeshObjects;


};





END_NAMESPACE_XEVOL3D

#endif