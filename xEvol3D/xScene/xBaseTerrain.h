#ifndef _XTERRAIN_SCENE_BASE_H_
#define _XTERRAIN_SCENE_BASE_H_
#include "xSceneGraph.h"

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xTerrainBase : public ISceneGraph
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xTerrainBase);
public:
	xTerrainBase(ISceneGraph*  pScene , ISceneNode* pParent = NULL);
    virtual ~xTerrainBase();

public:
	//可读的属性
	virtual float height(float x , float y) = 0;
	virtual bool  region(float& x , float& y , float& w , float& h) = 0; //这个地形表示的区域
    virtual bool  resolution(float& x  , float& y ) = 0 ; // x , y 方向的分辨率
	
	//可写的属性
    virtual bool  setHeight(float x , float y , float Height) = 0;

    /// 给入一个Polygon，返回一组Line Strip线（Triangle List面）
    /// 这组返回的Line和Triangle将完全与地面的起伏吻合。
    virtual void gainDecalLines(xMathLib::xvec3* pVertices, int VertexCount, xMathLib::xvec3*& pOutDecalVertices, int& OutDecalVertCount) { return ; }
    virtual void gainDecalTriangles(xMathLib::xvec3* pVertices, int VertexCount, xMathLib::xvec3*& pOutDecalVertices, unsigned short*& pOutDecalTriangles) { return ; }

    /// 压平一组地形
    /// 提供：顺时针的凸多边形数据
    /// 应用：将地形在此凸多边形内的所有顶点全部压平到平均高度。
    /// 应用后可获取：此次压平后的平均高度（OutHeight）、此次压平总影响面积（OutArea）和体积（OutVolume）
    virtual void setHeightsInPoly(xMathLib::xvec3* pVertices, int VertexCount, float& OutHeight, float& OutArea, float& OutVolume) { return ; }
    /// 此函数是指定高度的版本
    virtual void setSpecHeightsInPoly(xMathLib::xvec3* pVertices, int VertexCount, float SpecHeight, float& OutArea, float& OutVolume) { return ; }
    bool load(xXmlNode* pXml);
    bool save(xXmlNode* pXml);
};
END_NAMESPACE_XEVOL3D

#endif
