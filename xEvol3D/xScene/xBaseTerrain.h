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
};
END_NAMESPACE_XEVOL3D

#endif
