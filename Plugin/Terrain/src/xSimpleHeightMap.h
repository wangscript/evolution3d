#ifndef _SIMPLE_HEIGHTMAP_H_
#define _SIMPLE_HEIGHTMAP_H_
#include "Application/xPluginMgr.h"
#include "BaseLib/xStringTable.h"
#include "xScene/xSceneGraph.h"
#include "xScene/xSceneNode.h"
#include "xScene/xSceneObject.h"
#include "xScene/xSceneDrawable.h"
#include "xScene/xScenePropertyTool.h"
#include "xScene/xBaseTerrain.h"
#include "xTerrainDefinations.h"
#include "xTerrainData.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D


class xSceneTerrainPatch : public xSceneEffectObject
{
	DECL_BASE_OBJECT_DLLSAFE(xSceneTerrainPatch);
public:

	virtual void           setData(xTerrainData* pData, unsigned int px, unsigned int py, unsigned int pxindata, unsigned int pyindata);
	// 需要显示时，显示
	virtual void           flush();
	// 不需要显示时，卸载
	virtual bool           unload();

	inline void	           setVisible(bool vis){m_bVisible=vis;}
	virtual void	       setDrawLod(int level, int xlevel, int Xlevel, int ylevel, int YLevel );

	virtual bool           load(xXmlNode* pXml);
	virtual bool           save(xXmlNode* pXml);
	virtual size_t         nDrawElement(){return 1;}
	virtual IDrawElement*  drawElement(size_t idx) ;
	virtual bool           setDrawElement(IDrawElement* pDrawElement,size_t idx);
	virtual bool           updateFrame(unsigned long passedTime);
	xSceneTerrainPatch(ISceneGraph*  pScene , int arg);
	virtual ~xSceneTerrainPatch();

	const xvec3& getCenter();
public:
	IDrawElement*  m_pElement;
	xTerrainData* m_pData;

	int m_iLevel;
	bool m_bVisible;

	unsigned int m_iPatchXInLevel;
	unsigned int m_iPatchYInLevel;
	unsigned int m_iPatchXInData;
	unsigned int m_iPatchYInData;

};
class xTerrainScene;
class xTerrainSceneLoDLevel : public IPropertyValue
{
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xTerrainSceneLoDLevel)
public:
	void setTerrainScene(xTerrainScene* pScene);
public:
	const void*  getValuePtr();
	size_t       getValueDataLen();
	bool         setValulePtr(const void* pData , size_t dataLen , int _offset);
	bool         reAlloc(size_t iData);
public:
	xTerrainScene* m_pScene;
};
class xTerrainScene : public xTerrainBase
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xTerrainScene);
public:
	xTerrainScene(ISceneGraph*  pScene , ISceneNode* pParent = NULL);
	virtual ~xTerrainScene();
protected:
	xTerrainDataSet	m_TerrainDataSet;
	std::vector<xSceneTerrainPatch*,dllsafe_alloc<xSceneTerrainPatch*> > m_pTerrainPatches;
	unsigned int m_PatchX;
	unsigned int m_PatchY;
public:
	virtual bool               attachObject(ISceneObject* pObject) { return xSceneBasicNode::attachObject(pObject) ; }
	virtual bool               detachObject(ISceneObject* pObject) { return xSceneBasicNode::detachObject(pObject); }
	virtual bool               detachAllObject() { return xSceneBasicNode::detachAllObject() ; }
	virtual size_t             nObjects() { return xSceneBasicNode::nObjects() ; }
	virtual ISceneObject*      getObject(size_t idx ) { return xSceneBasicNode::getObject(idx) ; }
public:
	virtual bool       load(xXmlNode* pNode);
	virtual bool       save(xXmlNode* pNode);
	virtual bool       visit(ISceneVisitor* pVisitor , eVisitOrder _order ,  IRenderCamera* pCamera = NULL);
	virtual bool       updateFrame(unsigned long passedTime, bool bRecursive);
protected:
	bool               _visit(ISceneNode* pSceneNode , ISceneVisitor* pVisitor , IRenderCamera* pCamera = NULL);

protected:
	friend class          xTerrainSceneLoDLevel;
	int                   m_LoDLevel; //野猪加的，用来由界面控制视野的精细程度
	xTerrainSceneLoDLevel m_LoDLevelValue;
	bool                  setLoDLevel(int lodLevel);
protected:
	BEGIN_SCENE_NODE_PROPERTY_SET(ISceneNodePropertySet , xSceneBasicNode);
	   DECL_SCENE_PROPERTY_VALUE( _TRANSLATE(L"LoD View Detail") , &m_LoDLevelValue );
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
};




struct TerrainPatchVertex
{
	xvec3   m_pos;
	xvec2   m_uv;
};
struct xTerrainPatchDesc
{
	int         m_xRes;
	int         m_yRes;
	xvec3       m_bbMin;
	xvec3       m_bbMax;
	float       m_deltaX;
	float       m_deltaY;
	float		m_startX;
	float       m_startY;
	float       m_zScale;
};


class xTerrainPatch : public IDrawElement
{
	// vertex buffers
	IVertexStream *         m_pVB;
	// Description
	xTerrainPatchDesc       m_Desc;
	// Input Assembly
	IInputAssembler*        m_pInputAss;
	IRenderApi*				m_pRenderApi;
	int                     m_iLevel[5];
	// 
	HBaseTexture			m_hBaseTexture;
	// 非正常Index Buffer（固有）
	DECL_BASE_OBJECT_DLLSAFE(xTerrainPatch);
public:
	xTerrainPatch(IBaseRenderer* pBaseRenderer);
	virtual ~xTerrainPatch();
	bool   buildTerrain(float* pDepthData, const xTerrainData::SurfaceData& surfData, int resX, int resY, float startX, float startY, float deltaX, float deltaY, float scaleZ);
	void   flush();
	void   unload();
	void   setLOD(int mylod, int xlod, int Xlod, int ylod, int Ylod)
	{
		m_iLevel[0] = mylod;
		m_iLevel[1] = xlod; if ( m_iLevel[1] < mylod ) m_iLevel[1] = mylod;
		m_iLevel[2] = Xlod; if ( m_iLevel[2] < mylod ) m_iLevel[2] = mylod;
		m_iLevel[3] = ylod; if ( m_iLevel[3] < mylod ) m_iLevel[3] = mylod;
		m_iLevel[4] = Ylod; if ( m_iLevel[4] < mylod ) m_iLevel[4] = mylod;
	}
	void           center(XMathLib::xvec3&   _center);
	void           aabb(xGeomLib::xaabb&     _aabb);
	void           shpere(xGeomLib::xshpere& _shpere);
	bool           render(unsigned long passedTime);
	bool           update(unsigned long passedTime);
};




END_NAMESPACE_XEVOL3D

#endif