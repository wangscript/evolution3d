#include "xSimpleHeightMap.h"
#include "BaseLib/xLogger.h"
#include "Renderer/xRenderer.h"
#include "xScene/xSceneVisitor.h"
#include "XMathLib/xvolume.h"
#include "XMathLib/xGeomLib.h"
#include "xTerrainIndexUtil.h"



BEGIN_NAMESPACE_XEVOL3D

using namespace XGeomLib;
using namespace XMathLib;

IMPL_BASE_OBJECT_DLLSAFE(xTerrainScene   , xTerrainBase);
IMPL_SCENEOBJECT_FACTORY(xSceneTerrainPatch ,L"xTerrainPatch");
IMPL_BASE_OBJECT_DLLSAFE(xSceneTerrainPatch, xSceneEffectObject);


typedef	std::vector< xplane, dllsafe_alloc<xplane> > PlaneVector;

float	AreaOfTriangle(const xvec3& v0, const xvec3& v1, const xvec3& v2)
{
	float l0,l1,l2;
	l0 = (v1-v0).len();
	l1 = (v2-v1).len();
	l2 = (v0-v2).len();
	float   s,a;
	s=(l0+l1+l2)/2;
	return sqrt(s*(s-l0)*(s-l1)*(s-l2));
}
float	AreaOfPolygon(const xvec3* vstart, int vcount)
{
	float area = 0.0f;
	for (int i = 1 ; i < vcount-1 ; ++i)
	{
		area += AreaOfTriangle(vstart[0], vstart[i], vstart[i+1]);
	}
	return area;
}

void	ZPolyCWInnerPlanes(xvec3* pPoly, int Length, PlaneVector& OutResult)
{
	if (Length < 3)
	{
		// 小于三不能叫做Polygon
		return;
	}
	// 上方点
	const float BIG_NUMBER = 100000000.0f;
	float MaxZ = -BIG_NUMBER;
	xvec3* pVerticesUp = new xvec3[Length];
	for (int i = 0 ; i < Length ; ++i)
	{
		pVerticesUp[i] = pPoly[i];
		MaxZ = max(pVerticesUp[i].z, MaxZ);
	}
	MaxZ += 100.0f;
	// 下方点
	xvec3* pVerticesDown = new xvec3[Length];
	for (int i = 0 ; i < Length ; ++i)
	{
		pVerticesDown[i] = pPoly[i];
		pVerticesDown[i].z = MaxZ - BIG_NUMBER;
	}

	// 总数有Polygon顶点数个（不带上下）
	OutResult.reserve(Length);
	OutResult.resize(Length);

	for (int i = 0 ; i < Length-1 ; ++i)
	{
		OutResult[i] = xplane(pVerticesUp[i+1], pVerticesUp[i], pVerticesDown[i]);
	}
	// 缝合
	OutResult[Length-1] = xplane(pVerticesUp[0], pVerticesUp[Length-1], pVerticesDown[Length-1]);

	delete[] pVerticesUp;
	delete[] pVerticesDown;
}

void	ZPolyCWInnerPlanes(xvec3* pPoly, int Length, float MaxY, float MinY, PlaneVector& OutResult)
{
	if (Length < 3)
	{
		// 小于三不能叫做Polygon
		return;
	}
	// 上方点
	xvec3* pVerticesUp = new xvec3[Length];
	for (int i = 0 ; i < Length ; ++i)
	{
		pVerticesUp[i] = pPoly[i];
		pVerticesUp[i].y = MaxY;
	}
	// 下方点
	xvec3* pVerticesDown = new xvec3[Length];
	for (int i = 0 ; i < Length ; ++i)
	{
		pVerticesUp[i] = pPoly[i];
		pVerticesUp[i].y = MinY;
	}

	// 总数有Polygon顶点数个
	OutResult.reserve(Length);
	OutResult.resize(Length);

	for (int i = 0 ; i < Length-1 ; ++i)
	{
		OutResult[i] = xplane(pVerticesUp[i+1], pVerticesUp[i], pVerticesDown[i]);
	}
	// 缝合
	OutResult[Length-1] = xplane(pVerticesUp[0], pVerticesUp[Length-1], pVerticesDown[Length-1]);

	delete[] pVerticesUp;
	delete[] pVerticesDown;
}

//-----------------------------------------------------------------------
void	AABBVertex( const xvec3& aabb_min, const xvec3& aabb_max, xvec3 v[8])
{
	// 上4
	v[0] = xvec3( aabb_min.x, aabb_max.y, aabb_min.z);
	v[1] = xvec3( aabb_max.x, aabb_max.y, aabb_min.z);
	v[2] = xvec3( aabb_max.x, aabb_max.y, aabb_max.z);
	v[3] = xvec3( aabb_min.x, aabb_max.y, aabb_max.z);
	// 下4，对应的
	v[4] = xvec3( aabb_min.x, aabb_min.y, aabb_min.z);
	v[5] = xvec3( aabb_max.x, aabb_min.y, aabb_min.z);
	v[6] = xvec3( aabb_max.x, aabb_min.y, aabb_max.z);
	v[7] = xvec3( aabb_min.x, aabb_min.y, aabb_max.z);
}



void xTerrainScene::gainDecalLines(xvec3* pVertices, int VertexCount, xvec3*& pOutDecalVertices, int& OutDecalVertCount)
{
	// Coming Soon
}
void xTerrainScene::gainDecalTriangles(xvec3* pVertices, int VertexCount, xvec3*& pOutDecalVertices, unsigned short*& pOutDecalTriangles)
{
	// Coming Soon
}
void	xTerrainScene::setHeightsInPoly(xvec3* pVertices, int VertexCount, float& OutHeight, float& OutArea, float& OutVolume)
{
	PlaneVector planes;
	ZPolyCWInnerPlanes(pVertices, VertexCount, planes);
	// 计算所有落在内部的点
	std::vector<xSceneTerrainPatch*,dllsafe_alloc<xSceneTerrainPatch*> > pOpingPatches;
	for(size_t i = 0 ; i < m_pTerrainPatches.size() ; i ++)
	{
		xSceneTerrainPatch* pTerrainPatch = m_pTerrainPatches[i];
		if ( pTerrainPatch )
		{
			// 是否被包含？
			float sx = pTerrainPatch->m_pData->getStart().x + pTerrainPatch->m_pData->getScale().x * pTerrainPatch->m_iPatchXInData*PATCH_SIDETILECOUNT;
			float sy = pTerrainPatch->m_pData->getStart().y + pTerrainPatch->m_pData->getScale().y * pTerrainPatch->m_iPatchYInData*PATCH_SIDETILECOUNT;
			float ex = sx + PATCH_SIDEVERTCOUNT*pTerrainPatch->m_pData->getScale().x;
			float ey = sy + PATCH_SIDEVERTCOUNT*pTerrainPatch->m_pData->getScale().y;
			sx += m_Placement.m_Position.x;
			sy += m_Placement.m_Position.y;
			ex += m_Placement.m_Position.x;
			ey += m_Placement.m_Position.y;
			bool ok = false;
			// 被包含？
			for (int v = 0 ; v < VertexCount - 2 ; ++v)
			{
				if (
					(pointInTri2D(sx,sy, pVertices[0].x, pVertices[0].y, pVertices[v+1].x, pVertices[v+1].y, pVertices[v+2].x, pVertices[v+2].y))
					|| (pointInTri2D(sx,ey, pVertices[0].x, pVertices[0].y, pVertices[v+1].x, pVertices[v+1].y, pVertices[v+2].x, pVertices[v+2].y))
					|| (pointInTri2D(ex,sy, pVertices[0].x, pVertices[0].y, pVertices[v+1].x, pVertices[v+1].y, pVertices[v+2].x, pVertices[v+2].y))
					|| (pointInTri2D(ex,ey, pVertices[0].x, pVertices[0].y, pVertices[v+1].x, pVertices[v+1].y, pVertices[v+2].x, pVertices[v+2].y))
					)
				{
					ok = true;
					break;
				}
			}
			// 包含？
			if (!ok)
			{
				for (int v = 0 ; v < VertexCount ; ++v)
				{
					if ( pVertices[v].x > sx && pVertices[v].x < ex
						&&  pVertices[v].y > sy && pVertices[v].y < ey
						)
					{
						ok = true;
						break;
					}
					
				}
			}
			if (ok) {
				pOpingPatches.push_back(pTerrainPatch);
			}
		}
	}
	// 对这些Patch计算
	// 计算平均高度差
	float avr_height = 0.0f;
	int avr_height_count = 0;
	for(size_t i = 0 ; i < pOpingPatches.size() ; i ++)
	{
		xSceneTerrainPatch* pPatch = pOpingPatches[i];
		float sx = pPatch->m_pData->getStart().x + pPatch->m_pData->getScale().x * pPatch->m_iPatchXInData*PATCH_SIDETILECOUNT;
		float sy = pPatch->m_pData->getStart().y + pPatch->m_pData->getScale().y * pPatch->m_iPatchYInData*PATCH_SIDETILECOUNT;

		// Patch里的哪些顶点是被包含的？
		for (int y = 0 ; y < PATCH_SIDEVERTCOUNT ; ++y)
		{
			for (int x = 0 ; x < PATCH_SIDEVERTCOUNT ; ++x)
			{
				float height = pPatch->m_pData->getHeight(pPatch->m_iPatchXInData*PATCH_SIDETILECOUNT+x, pPatch->m_iPatchYInData*PATCH_SIDETILECOUNT+y);
				xvec3 point = xvec3(
					sx + pPatch->m_pData->getScale().x*x,
					sy + pPatch->m_pData->getScale().y*y,
					height
					);
				point += m_Placement.m_Position;
				// 全在里面
				bool allinplanes = true;
				for(size_t p = 0 ; p < planes.size() ; ++p)
				{
					float d = planes[p] * point;
					if (d < 0.0f) {
						allinplanes = false;
						break;
					}
				}
				if (allinplanes)
				{
					avr_height += height;
					++avr_height_count;
				}
			}
		}
	}
	avr_height /= (float)avr_height_count;


	for(size_t i = 0 ; i < pOpingPatches.size() ; i ++)
	{
		xSceneTerrainPatch* pPatch = pOpingPatches[i];
		float sx = pPatch->m_pData->getStart().x + pPatch->m_pData->getScale().x * pPatch->m_iPatchXInData*PATCH_SIDETILECOUNT;
		float sy = pPatch->m_pData->getStart().y + pPatch->m_pData->getScale().y * pPatch->m_iPatchYInData*PATCH_SIDETILECOUNT;

		// Patch里的哪些顶点是被包含的？
		bool patch_handled = false;
		for (int y = 0 ; y < PATCH_SIDEVERTCOUNT ; ++y)
		{
			for (int x = 0 ; x < PATCH_SIDEVERTCOUNT ; ++x)
			{
				float height = pPatch->m_pData->getHeight(pPatch->m_iPatchXInData*PATCH_SIDETILECOUNT+x, pPatch->m_iPatchYInData*PATCH_SIDETILECOUNT+y);
				xvec3 point = xvec3(
					sx + pPatch->m_pData->getScale().x*x,
					sy + pPatch->m_pData->getScale().y*y,
					height
					);
				point += m_Placement.m_Position;
				// 全在里面
				bool allinplanes = true;
				for(size_t p = 0 ; p < planes.size() ; ++p)
				{
					float d = planes[p] * point;
					if (d < 0.0f) {
						allinplanes = false;
						break;
					}
				}
				if (allinplanes)
				{
					pPatch->m_pData->setHeight(pPatch->m_iPatchXInData*PATCH_SIDETILECOUNT+x, pPatch->m_iPatchYInData*PATCH_SIDETILECOUNT+y, avr_height);
					patch_handled = true;
				}
			}
		}
		if (patch_handled)
		{
			pPatch->flush();
		}
	}
	OutArea = AreaOfPolygon(pVertices, VertexCount);
	OutVolume = OutArea * 0;

}
void	xTerrainScene::setSpecHeightsInPoly(xvec3* pVertices, int VertexCount, float SpecHeight, float& OutArea, float& OutVolume)
{
	PlaneVector planes;
	ZPolyCWInnerPlanes(pVertices, VertexCount, planes);
	// 计算所有落在内部的点
	std::vector<xSceneTerrainPatch*,dllsafe_alloc<xSceneTerrainPatch*> > pOpingPatches;
	for(size_t i = 0 ; i < m_pTerrainPatches.size() ; i ++)
	{
		xSceneTerrainPatch* pTerrainPatch = m_pTerrainPatches[i];
		if ( pTerrainPatch )
		{
			// 是否被包含？
			float sx = pTerrainPatch->m_pData->getStart().x + pTerrainPatch->m_pData->getScale().x * pTerrainPatch->m_iPatchXInData*PATCH_SIDETILECOUNT;
			float sy = pTerrainPatch->m_pData->getStart().y + pTerrainPatch->m_pData->getScale().y * pTerrainPatch->m_iPatchYInData*PATCH_SIDETILECOUNT;
			float ex = sx + PATCH_SIDEVERTCOUNT*pTerrainPatch->m_pData->getScale().x;
			float ey = sy + PATCH_SIDEVERTCOUNT*pTerrainPatch->m_pData->getScale().y;
			sx += m_Placement.m_Position.x;
			sy += m_Placement.m_Position.y;
			ex += m_Placement.m_Position.x;
			ey += m_Placement.m_Position.y;
			bool ok = false;
			// 被包含？
			for (int v = 0 ; v < VertexCount - 2 ; ++v)
			{
				if (
					(pointInTri2D(sx,sy, pVertices[0].x, pVertices[0].y, pVertices[v+1].x, pVertices[v+1].y, pVertices[v+2].x, pVertices[v+2].y))
					|| (pointInTri2D(sx,ey, pVertices[0].x, pVertices[0].y, pVertices[v+1].x, pVertices[v+1].y, pVertices[v+2].x, pVertices[v+2].y))
					|| (pointInTri2D(ex,sy, pVertices[0].x, pVertices[0].y, pVertices[v+1].x, pVertices[v+1].y, pVertices[v+2].x, pVertices[v+2].y))
					|| (pointInTri2D(ex,ey, pVertices[0].x, pVertices[0].y, pVertices[v+1].x, pVertices[v+1].y, pVertices[v+2].x, pVertices[v+2].y))
					)
				{
					ok = true;
					break;
				}
			}
			// 包含？
			if (!ok)
			{
				for (int v = 0 ; v < VertexCount ; ++v)
				{
					if ( pVertices[v].x > sx && pVertices[v].x < ex
						&&  pVertices[v].y > sy && pVertices[v].y < ey
						)
					{
						ok = true;
						break;
					}

				}
			}
			if (ok) {
				pOpingPatches.push_back(pTerrainPatch);
			}
		}
	}
	// 对这些Patch计算
	// 计算平均高度差
	float avr_height_delta = 0.0f;
	int avr_height_count = 0;
	for(size_t i = 0 ; i < pOpingPatches.size() ; i ++)
	{
		xSceneTerrainPatch* pPatch = pOpingPatches[i];
		float sx = pPatch->m_pData->getStart().x + pPatch->m_pData->getScale().x * pPatch->m_iPatchXInData*PATCH_SIDETILECOUNT;
		float sy = pPatch->m_pData->getStart().y + pPatch->m_pData->getScale().y * pPatch->m_iPatchYInData*PATCH_SIDETILECOUNT;
		
		// Patch里的哪些顶点是被包含的？
		bool patch_handled = false;
		for (int y = 0 ; y < PATCH_SIDEVERTCOUNT ; ++y)
		{
			for (int x = 0 ; x < PATCH_SIDEVERTCOUNT ; ++x)
			{
				float height = pPatch->m_pData->getHeight(pPatch->m_iPatchXInData*PATCH_SIDETILECOUNT+x, pPatch->m_iPatchYInData*PATCH_SIDETILECOUNT+y);
				xvec3 point = xvec3(
					sx + pPatch->m_pData->getScale().x*x,
					sy + pPatch->m_pData->getScale().y*y,
					height
				);
				point += m_Placement.m_Position;
				// 全在里面
				bool allinplanes = true;
				for(size_t p = 0 ; p < planes.size() ; ++p)
				{
					float d = planes[p] * point;
					if (d < 0.0f) {
						allinplanes = false;
						break;
					}
				}
				if (allinplanes)
				{
					avr_height_delta += height - SpecHeight;
					++avr_height_count;
					pPatch->m_pData->setHeight(pPatch->m_iPatchXInData*PATCH_SIDETILECOUNT+x, pPatch->m_iPatchYInData*PATCH_SIDETILECOUNT+y, SpecHeight);
					patch_handled = true;
				}
			}
		}
		if (patch_handled)
		{
			pPatch->flush();
		}
	}

	avr_height_delta /= (float)avr_height_count;
	OutArea = AreaOfPolygon(pVertices, VertexCount);
	OutVolume = OutArea * avr_height_delta;
}
bool xTerrainScene::pointInTri2D(float px, float py, float ax, float ay, float bx, float by, float cx, float cy)
{
	float v1x, v2x, v1y, v2y;
	bool bClockwise;

	v1x = bx - ax;
	v1y = by - ay;

	v2x = px - bx;
	v2y = py - by;

	// For the sake of readability
#define Clockwise ( v1x * v2y - v1y * v2x >= 0.0 )

	bClockwise = Clockwise;

	v1x = cx - bx;
	v1y = cy - by;

	v2x = px - cx;
	v2y = py - cy;

	if( Clockwise != bClockwise )
		return false;

	v1x = ax - cx;
	v1y = ay - cy;

	v2x = px - ax;
	v2y = py - ay;

	if( Clockwise != bClockwise )
		return false;

	return true;

	// Clean up the #defines
#undef Clockwise
}








//------------------------------------------------------------------------
void           xSceneTerrainPatch::setData(xTerrainData* pData, unsigned int px, unsigned int py, unsigned int pxindata, unsigned int pyindata)
{
	m_pData = pData;
	m_iPatchXInLevel= px;
	m_iPatchYInLevel= py;
	m_iPatchXInData	= pxindata;
	m_iPatchYInData	= pyindata;
}

const xvec3&	xSceneTerrainPatch::getCenter()
{
	return m_pData->getCenter();
}

void           xSceneTerrainPatch::flush()
{
	if (m_pElement)
	{
		m_pElement->ReleaseObject();
		m_pElement = NULL;
	}
	// 创建渲染数据
	IBaseRenderer* pRenderer = m_pSceneGraph->renderer();

	std::vector<float> _depth_out;
	xTerrainData::SurfaceData _surface_out;
	m_pData->patchView(m_iPatchXInData*PATCH_SIDETILECOUNT, m_iPatchYInData*PATCH_SIDETILECOUNT, PATCH_SIDEVERTCOUNT, PATCH_SIDEVERTCOUNT, _depth_out, _surface_out);
	if ( _depth_out.empty() )
	{
		return;
	}
	xTerrainPatch* pTerrainPatch = new xTerrainPatch(pRenderer);
	float sx = m_pData->getStart().x + m_pData->getScale().x * m_iPatchXInData*PATCH_SIDETILECOUNT;
	float sy = m_pData->getStart().y + m_pData->getScale().y * m_iPatchYInData*PATCH_SIDETILECOUNT;
	if ( !pTerrainPatch->buildTerrain( &_depth_out[0], _surface_out, PATCH_SIDEVERTCOUNT, PATCH_SIDEVERTCOUNT,
		sx, sy, m_pData->getScale().x, m_pData->getScale().y, m_pData->getScale().z) )
	//if ( !pTerrainPatch->buildTerrain( &_depth_out[0], m_sizeX, m_sizeY, 0.0f, 0.0f, m_pData->getDeltaX(), m_pData->getDeltaY(), 20.0f) )
	{
		delete pTerrainPatch;
		return;
	}
	m_pElement = pTerrainPatch;

	m_pEffect = pRenderer->createRenderEffect( L"KidBasicEffect" );
	xMaterial* pMaterail = NULL;
	pMaterail = pRenderer->createMaterial( L"SimpleTerrainPatch" );
	if ( pMaterail )
	{
		m_pEffect->setMaterial(pMaterail);
	}
}

bool           xSceneTerrainPatch::unload()
{
	// 不显示，则可以取消渲染数据
	XSAFE_RELEASEOBJECT ( m_pElement ) ;
	return xSceneEffectObject::unload();
}

void			xSceneTerrainPatch::setDrawLod(int level, int xlevel, int Xlevel, int ylevel, int YLevel )
{
	if ( m_pElement )
	{
		xTerrainPatch* pTP = dynamic_cast<xTerrainPatch*>(m_pElement);
		pTP->setLOD(level,xlevel, Xlevel, ylevel, YLevel);
	}
}

void		   xSceneTerrainPatch::setSurfLevel(int level)
{
	if ( m_pElement )
	{
		xTerrainPatch* pTP = dynamic_cast<xTerrainPatch*>(m_pElement);
		pTP->setSurfLod(level);
	}
}

bool           xSceneTerrainPatch::load(xXmlNode* pXml)
{
	// need to do nothing here
	return true;
}

bool           xSceneTerrainPatch::save(xXmlNode* pXml)
{
	// need to do nothing here
	return true;
}

IDrawElement*  xSceneTerrainPatch::drawElement(size_t idx) 
{
	if ( m_bVisible && idx == 0)
	{
		return m_pElement;
	}
	return NULL;
}

bool           xSceneTerrainPatch::setDrawElement(IDrawElement* pDrawElement,size_t idx)
{
	return false;
}

bool           xSceneTerrainPatch::updateFrame(unsigned long passedTime , IRenderCamera* pCamera)
{
	m_pElement->update(passedTime);
	return true ;
}

xSceneTerrainPatch::xSceneTerrainPatch(ISceneGraph*  pScene , int arg )
: xSceneEffectObject(pScene, arg)
{
	m_pElement = NULL;
}

xSceneTerrainPatch::~xSceneTerrainPatch()
{
	m_pData = NULL;
	if ( m_pElement )
	{
		m_pElement->ReleaseObject();
		m_pElement = NULL;
	}
}

void         xTerrainSceneLoDLevel::setTerrainScene(xTerrainScene* pScene)
{
	m_pScene = pScene;
}

const void*  xTerrainSceneLoDLevel::getValuePtr()
{
	return &m_pScene->m_LoDLevel;
}
size_t       xTerrainSceneLoDLevel::getValueDataLen()
{
	return sizeof(m_pScene->m_LoDLevel);
}
bool         xTerrainSceneLoDLevel::setValulePtr(const void* pData , size_t dataLen , int _offset)
{
	if(dataLen != sizeof(m_pScene->m_LoDLevel))
		return false;
	return m_pScene->setLoDLevel( *(int*)pData );

}
bool         xTerrainSceneLoDLevel::reAlloc(size_t iData)
{
	return false;
}

bool xTerrainScene::setLoDLevel(int lodLevel)
{
	m_LoDLevel = lodLevel;
	return true;
}

xTerrainScene::xTerrainScene(ISceneGraph* pScene , ISceneNode* pParent  )
: xTerrainBase(pScene , pParent )
, m_PropertySet(L"Terrain System" , _TRANSLATE(L"Terrain System") )
{
	if ( pScene )
	{
		m_pRenderer = pScene->renderer();
		m_pResPathMgr = pScene->getPathManager();
		m_pTextureMgr = pScene->getTextureManger();
		m_pModelMgr = pScene->getModelManager();
	}
	m_PatchX = 0;
	m_PatchY = 0;

	m_TerrainDataSet._setTextureMgr(m_pTextureMgr);

	m_LoDLevel = 3;
	m_LoDLevelValue.setTerrainScene(this);
	m_TypeName = L"xTerrainScene";
}

xTerrainScene::~xTerrainScene()
{
	detachAllObject();
	for(size_t i = 0 ; i < m_pTerrainPatches.size() ; i ++)
	{
		xSceneTerrainPatch* pTerrainPatch = m_pTerrainPatches[i];
		if ( pTerrainPatch )
		{
			pTerrainPatch->ReleaseObject();
		}
	}
	m_pTerrainPatches.clear();
}

float xTerrainScene::height(float x , float y)
{
	return m_TerrainDataSet.getHeightReal(x,y);
}
bool  xTerrainScene::region(float& x , float& y , float& w , float& h)
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
	x = m_TerrainDataSet.getOriginX() ; y = m_TerrainDataSet.getOriginY() ; w = 1000.0f ; h = 10000.0f ; 
	return true ; 
}
bool  xTerrainScene::resolution(float& x  , float& y )
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
	x = 5.0f ;
	y = 4.0f ; 
	return true ; 
}
bool xTerrainScene::load(xXmlNode* pNode)
{
	//Load thing right here.
	if(!ISceneGraph::load(pNode))
	{
		return false;
	}

	//// TEST
	//m_TerrainData.test();
	m_TerrainDataSet.load(pNode);

	// TODO 此时高程和表面应该有效了，载入地形Patch
	std::vector<xTerrainDataSet::PatchDataView> terrainDataArray;
	m_TerrainDataSet.generatePatchView( PATCH_SIDETILECOUNT, PATCH_SIDETILECOUNT, m_PatchX, m_PatchY, terrainDataArray );
	m_pTerrainPatches.reserve(m_PatchX*m_PatchY);
	m_pTerrainPatches.resize(m_PatchX*m_PatchY);
	for ( unsigned int x = 0 ; x < m_PatchX ; ++x ){
		for ( unsigned int y = 0 ; y < m_PatchY ; ++y )
		{
			const xTerrainDataSet::PatchDataView& terrData = terrainDataArray[y*m_PatchX + x];

			if ( terrData.pData != NULL )
			{
				xSceneTerrainPatch* pTerrainPatch = new xSceneTerrainPatch(this,0);
				pTerrainPatch->setData(terrData.pData, x, y, terrData.xID, terrData.yID);
				pTerrainPatch->flush();

				m_pTerrainPatches[y*m_PatchX+x] = pTerrainPatch;

				attachObject(pTerrainPatch);
			}
			else
			{
				m_pTerrainPatches[y*m_PatchX+x] = NULL;
			}

			//// TEST
			//xSceneBasicNode* pNode = new xSceneBasicNode(this);
			//pNode->placement()->setPosition( xvec3(startX*m_TerrainData.getDeltaX(),startY*m_TerrainData.getDeltaY(),0) );
			//pNode->attachObject(pTerrainPatch);
			//pTerrainPatch->ReleaseObject();
			//insertChild(pNode);
		}
	}

	return true;
}

bool xTerrainScene::save(xXmlNode* pXml)
{
	if(pXml == NULL)
		return false;
	savePlacement(pXml);
	pXml->setValue(L"type" , L"xTerrainScene");
	pXml->setValue(L"name" , m_Name.c_str() );

	m_TerrainDataSet.save(pXml);

	// Terrain Patch不需要存储
	//beginEnumChildren();
	//ISceneNode* pChildNode = NULL;
	//while(pChildNode = nextChild() )
	//{
	//	xXmlNode* pChildXml = pXml->insertNode(L"child");
	//	pChildNode->save(pChildXml);
	//}
	//endEnumChildren();

	return true;
}

bool xTerrainScene::visit(ISceneVisitor* pVisitor , eVisitOrder _order , IRenderCamera* pCamera )
{
	// 计算LOD
	float _farplane = m_pRenderer->renderApi()->getCamera()->getFarPlan();
	xvec3 _campos = m_pRenderer->renderApi()->getCamera()->getEye().m_EyePos;
	for ( size_t i = 0 ; i < m_pTerrainPatches.size() ; ++ i )
	{
		// 判别可见范围，选择必要的Patch
		if ( !m_pTerrainPatches[i] || !m_pTerrainPatches[i]->m_pElement )
		{
			continue;
		}
		// TODO aabb检测
		xvec3 center ;
		m_pTerrainPatches[i]->m_pElement->center(center);
		center += this->placement()->m_Position;
		_campos.z = 0;
		center.z = 0;
		float _len = (_campos-center).len();
		if ( _len > (_farplane*1.1f) )
		{
			m_pTerrainPatches[i]->setVisible(false);
			m_pTerrainPatches[i]->m_iLevel = 0;
		}
		else
		{
			m_pTerrainPatches[i]->setVisible(true);
			// TEST
			m_pTerrainPatches[i]->m_iLevel = 4;
			if ( _len < (0.5f*_farplane) )
				m_pTerrainPatches[i]->m_iLevel = 3;
			if ( _len < (0.3f*_farplane) )
				m_pTerrainPatches[i]->m_iLevel = 2;
			if ( _len < (0.2f*_farplane) )
				m_pTerrainPatches[i]->m_iLevel = 1;
			if ( _len < (0.1f*_farplane) )
				m_pTerrainPatches[i]->m_iLevel = 0;

			if ( _len < (0.5f*_farplane) )
				m_pTerrainPatches[i]->setSurfLevel(3);
			if ( _len < (0.3f*_farplane) )
				m_pTerrainPatches[i]->setSurfLevel(2);
			if ( _len < (0.2f*_farplane) )
				m_pTerrainPatches[i]->setSurfLevel(1);
			if ( _len < (0.1f*_farplane) )
				m_pTerrainPatches[i]->setSurfLevel(0);

		}
	}
	for ( size_t i = 0 ; i < m_pTerrainPatches.size() ; ++ i )
	{
		if ( !m_pTerrainPatches[i] || !m_pTerrainPatches[i]->m_pElement )
		{
			continue;
		}
		int xid = m_pTerrainPatches[i]->m_iPatchXInLevel;
		int yid = m_pTerrainPatches[i]->m_iPatchYInLevel;
		int lod[4] = {0,0,0,0};
		if ( xid == 0  )
			lod[0] = 0;
		else if (m_pTerrainPatches[yid*m_PatchX+xid-1] == NULL)
			lod[0] = 0;
		else
			lod[0] = m_pTerrainPatches[yid*m_PatchX+xid-1]->m_iLevel;

		if ( xid == m_PatchX-1 )
			lod[1] = 0;
		else if (m_pTerrainPatches[yid*m_PatchX+xid+1] == NULL)
			lod[1] = 0;
		else
			lod[1] = m_pTerrainPatches[yid*m_PatchX+xid+1]->m_iLevel;

		if ( yid == 0 )
			lod[2] = 0;
		else if (m_pTerrainPatches[(yid-1)*m_PatchX+xid] == NULL)
			lod[2] = 0;
		else
			lod[2] = m_pTerrainPatches[(yid-1)*m_PatchX+xid]->m_iLevel;

		if ( yid == m_PatchY-1 )
			lod[3] = 0;
		else if (m_pTerrainPatches[(yid+1)*m_PatchX+xid] == NULL)
			lod[2] = 0;
		else
			lod[3] = m_pTerrainPatches[(yid+1)*m_PatchX+xid]->m_iLevel;

		m_pTerrainPatches[i]->setDrawLod(m_pTerrainPatches[i]->m_iLevel, lod[0], lod[1], lod[2], lod[3] );
	}
	return _visit(this , pVisitor , pCamera);
}

bool xTerrainScene::_visit(ISceneNode* pSceneNode , ISceneVisitor* pVisitor , IRenderCamera* pCamera)
{
	if(pCamera != NULL)
	{
		XEVOL_WARNNING_NOT_IMPLEMENT_INFO(L"Please do frustum culling here\n");
	}


	//后续遍历
	pSceneNode->beginEnumChildren();
	ISceneNode* pChild = NULL;
	while(pChild = pSceneNode->nextChild() )
	{
		_visit(pChild , pVisitor , pCamera);
	}
	pSceneNode->endEnumChildren();

	pVisitor->visit(pSceneNode , this);
	return true ;
}

bool xTerrainScene::updateFrame(unsigned long passedTime , IRenderCamera* pCamera, bool bRecursive)
{
	return ISceneNode::updateFrame(passedTime , pCamera ,  bRecursive);
}

IMPL_SCENENODE_FACTORY(xTerrainScene);
IMPL_SCENE_FACTORY(xTerrainScene     ,  L"xTerrainScene" );












IMPL_BASE_OBJECT_DLLSAFE(xTerrainPatch, xSceneEffectObject);

IDepthStencilState* xTerrainPatch::m_pUnderGroundStencil = NULL;
xTerrainPatch::xTerrainPatch(IBaseRenderer* pBaseRenderer)
: IDrawElement(pBaseRenderer)
{
	m_pVB = NULL;
	m_pInputAss = NULL;
	m_pRenderApi = pBaseRenderer->renderApi();
	if ( m_pRenderApi )
	{
		m_pRenderApi->AddRef();
	}
	m_iLevel[0] = 0;m_iLevel[1] = 0;m_iLevel[2] = 0;m_iLevel[3] = 0;m_iLevel[4] = 0;
}

xTerrainPatch::~xTerrainPatch()
{
	if ( m_pVB )
	{
		m_pVB->ReleaseObject();
		m_pVB = NULL;
	}
	if ( m_pInputAss )
	{
		m_pInputAss->ReleaseObject();
		m_pInputAss = NULL;
	}
	if ( m_pRenderApi )
	{
		m_pRenderApi->ReleaseObject();
		m_pRenderApi = NULL;
	}
}

bool   xTerrainPatch::buildTerrain(float* pDepthData, const xTerrainData::SurfaceData& surfData, int resX, int resY, float startX, float startY, float deltaX, float deltaY, float scaleZ)
{
	// descriptions
	m_Desc.m_zScale = scaleZ;
	m_Desc.m_xRes = resX;
	m_Desc.m_yRes = resY;
	m_Desc.m_deltaX = deltaX;
	m_Desc.m_deltaY = deltaY;
	m_Desc.m_startX = startX;
	m_Desc.m_startY = startY;
	m_hBaseTexture[0] = surfData.hTexture[0];
	m_hBaseTexture[1] = surfData.hTexture[1];
	m_hBaseTexture[2] = surfData.hTexture[2];
	m_hBaseTexture[3] = surfData.hTexture[3];
	m_iSurfLevel = 0;

	m_Desc.m_bbMin = xvec3(100000000.0f,100000000.0f,100000000.0f);
	m_Desc.m_bbMax = xvec3(-100000000.0f,-100000000.0f,-100000000.0f);

	// fill vertices
	TerrainPatchVertex* pVertices = new TerrainPatchVertex[ m_Desc.m_xRes * m_Desc.m_yRes ];

	float minZ  = 100000.0f;
	float maxZ  = -100000.0f;
	float _one_div_128 = 1.0f / 128.0f;
	for(int y = 0 ; y < m_Desc.m_yRes ; y ++)
	{
		for(int x = 0 ; x < m_Desc.m_xRes ; x ++)
		{
			int id = m_Desc.m_xRes * y + x;

			pVertices[id].m_pos.x = startX + deltaX*x;
			pVertices[id].m_pos.y = startY + deltaY*y;
			pVertices[id].m_pos.z = pDepthData[id]*scaleZ;
			//pVertices[id].m_pos.x = startX + deltaX*x;
			//pVertices[id].m_pos.y = pDepthData[id]*scaleZ;
			//pVertices[id].m_pos.z = startY + deltaY*y;

			pVertices[id].m_uv.x = surfData.minU + x*_one_div_128*surfData.sizeU;
			pVertices[id].m_uv.y = surfData.minV + y*_one_div_128*surfData.sizeV;
		}
	}
	for(int i = 0 ; i < m_Desc.m_xRes * m_Desc.m_yRes ; i ++)
	{
		//if(pVertices[i].m_pos.z < 0) 
		//{
		//	pVertices[i].m_pos.z =0.0 ; 
		//	continue;
		//}

		m_Desc.m_bbMin.x = min( m_Desc.m_bbMin.x, pVertices[i].m_pos.x );
		m_Desc.m_bbMin.y = min( m_Desc.m_bbMin.y, pVertices[i].m_pos.y );
		m_Desc.m_bbMin.z = min( m_Desc.m_bbMin.z, pVertices[i].m_pos.z );
		m_Desc.m_bbMax.x = max( m_Desc.m_bbMax.x, pVertices[i].m_pos.x );
		m_Desc.m_bbMax.y = max( m_Desc.m_bbMax.y, pVertices[i].m_pos.y );
		m_Desc.m_bbMax.z = max( m_Desc.m_bbMax.z, pVertices[i].m_pos.z );
	}

	// layout and description
	xInputLayoutDesc vd;
	vd.addElement(SHADER_SEMANTIC_POSITION,SHADERVARTYPE_FLOAT3,0,0);
	vd.addElement(SHADER_SEMANTIC_TEXCOORD,SHADERVARTYPE_FLOAT2,0,0);

	m_pInputAss = m_pRenderApi->createInputAssembler(L"xSimpleTerrainPatchIA" , vd);
	m_pVB = m_pInputAss->createVertexStream();

	//创建VertexBuffer;
	void* InitData[1] = {pVertices};
	m_pVB->createBuffers(m_Desc.m_xRes * m_Desc.m_yRes , InitData);

	delete[]pVertices;

	return true;
}

void   xTerrainPatch::flush()
{
}

void   xTerrainPatch::unload()
{
}

void   xTerrainPatch::center(XMathLib::xvec3&   _center)
{
	_center = (m_Desc.m_bbMin + m_Desc.m_bbMax)*0.5f;
}
void   xTerrainPatch::aabb(xGeomLib::xaabb&     _aabb)
{
	_aabb.m_Min = m_Desc.m_bbMin;
	_aabb.m_Min = m_Desc.m_bbMax;
}
void   xTerrainPatch::shpere(xGeomLib::xshpere& _shpere)
{
	_shpere.m_Orign = (m_Desc.m_bbMin + m_Desc.m_bbMax)*0.5f;
	_shpere.m_R = (_shpere.m_Orign-m_Desc.m_bbMin).len();
}

static xStaticTerrainIndices GStaticTerrainIndices;
bool   xTerrainPatch::render(unsigned long passedTime)
{
	int GW = m_Desc.m_xRes - 1;
	int GH = m_Desc.m_yRes - 1;
	if(m_pUnderGroundStencil)
		m_pRenderApi->setDepthStencilState(m_pUnderGroundStencil);

	m_pRenderApi->setVertexStream(m_pVB);

	m_pRenderApi->setTexture( 0, m_hBaseTexture[m_iSurfLevel].getResource());

	// assign index buffer
	IInputBuffer* pIB = GStaticTerrainIndices.getIB( m_pRenderApi, m_iLevel[0], m_iLevel[1], m_iLevel[2], m_iLevel[3], m_iLevel[4]);
	m_pRenderApi->draw(pIB,pIB->nElement());

	return true;
}

bool   xTerrainPatch::update(unsigned long passedTime)
{
	return true;
}

bool xTerrainScene::beginUndergroundEdit()
{
	
	xTerrainPatch::m_pUnderGroundStencil = m_pRenderer->renderApi()->createDepthStencilState(L"Overlay");
	return true;
}

bool xTerrainScene::endUndergroundEdit()
{
	xTerrainPatch::m_pUnderGroundStencil = m_pRenderer->renderApi()->createDepthStencilState(L"Default");
	return true;

}
END_NAMESPACE_XEVOL3D