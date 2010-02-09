#ifndef _TERRAIN_INDEXUTIL_H_
#define _TERRAIN_INDEXUTIL_H_

#include "Application/xPluginMgr.h"
#include "xScene/xSceneGraph.h"
#include "xScene/xSceneNode.h"
#include "xScene/xSceneObject.h"
#include "xScene/xSceneDrawable.h"
#include "xTerrainDefinations.h"

BEGIN_NAMESPACE_XEVOL3D

enum 
{
	MAX_LOD_LEVEL = 5,
};

enum
{
	SIDE_MINX	= 0	,
	SIDE_MAXX	= 1	,
	SIDE_MINY	= 2	,
	SIDE_MAXY	= 3	,
	SIDE_COUNT	= 4 ,
};


class xStaticTerrainIndices
{
public:
	xStaticTerrainIndices();
	~xStaticTerrainIndices();

private:
	typedef unsigned short ushort;
	typedef std::vector< ushort , dllsafe_alloc<ushort> > ushort_vector;
	ushort_vector KernalFaces[MAX_LOD_LEVEL];
	ushort_vector xLinkFaces[4][MAX_LOD_LEVEL][MAX_LOD_LEVEL];
	// Kernal Indices
	IInputBuffer  *         m_pIB[MAX_LOD_LEVEL][MAX_LOD_LEVEL][MAX_LOD_LEVEL][MAX_LOD_LEVEL][MAX_LOD_LEVEL];

	void buildDatabase();
	void genIB( IRenderApi* pRenderApi, int mylod, int minxLOD, int maxxLOD, int minyLOD, int maxYLOD );

public:
	// this lod: this patch's LOD
	// other lod: the other patch's LOD
	// otherside: SIDE_MINX or SIDE_MINY or SIDE_MAXX or SIDE_MAXY
	inline IInputBuffer*	getIB( IRenderApi* pRenderApi, int mylod, int minxLOD, int maxxLOD, int minyLOD, int maxYLOD )
	{
		if ( m_pIB[mylod][minxLOD][maxxLOD][minyLOD][maxYLOD] == NULL )
		{
			genIB( pRenderApi, mylod, minxLOD, maxxLOD, minyLOD, maxYLOD );
		}
		return m_pIB[mylod][minxLOD][maxxLOD][minyLOD][maxYLOD];
	}
};



END_NAMESPACE_XEVOL3D

#endif