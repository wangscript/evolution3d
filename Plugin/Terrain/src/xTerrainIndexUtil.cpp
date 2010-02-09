#include "xTerrainIndexUtil.h"


BEGIN_NAMESPACE_XEVOL3D



xStaticTerrainIndices::xStaticTerrainIndices(  )
{
	buildDatabase();
}

xStaticTerrainIndices::~xStaticTerrainIndices()
{
}

void xStaticTerrainIndices::genIB( IRenderApi* pRenderApi, int mylod, int minxLOD, int maxxLOD, int minyLOD, int maxYLOD )
{
	if ( m_pIB[mylod][minxLOD][maxxLOD][minyLOD][maxYLOD] )
	{
		return;
	}
	ushort_vector Data;
	Data = KernalFaces[mylod];
	Data.insert(Data.end(), xLinkFaces[0][mylod][minxLOD].begin(), xLinkFaces[0][mylod][minxLOD].end() );
	Data.insert(Data.end(), xLinkFaces[1][mylod][maxxLOD].begin(), xLinkFaces[1][mylod][maxxLOD].end() );
	Data.insert(Data.end(), xLinkFaces[2][mylod][minyLOD].begin(), xLinkFaces[2][mylod][minyLOD].end() );
	Data.insert(Data.end(), xLinkFaces[3][mylod][maxYLOD].begin(), xLinkFaces[3][mylod][maxYLOD].end() );
	xInputBufferDesc idxBufDesc;
	idxBufDesc.m_usage = RESOURCE_USAGE_DEFAULT;
	idxBufDesc.m_accessFlage = RESOURCE_ACCESS_NONE;
	idxBufDesc.m_bindtype = BIND_AS_INDEX_BUFFER;
	m_pIB[mylod][minxLOD][maxxLOD][minyLOD][maxYLOD] = pRenderApi->createInputBuffer( Data.size(), sizeof(ushort), &idxBufDesc, &Data[0] );
}

void xStaticTerrainIndices::buildDatabase()
{
	// lod
	for ( int lod = 0 ; lod < MAX_LOD_LEVEL ; ++lod )
	{
		int _lod_step = 1 << lod;
		int side_sam_count = PATCH_SIDETILECOUNT/_lod_step-2;
		KernalFaces[lod].reserve(side_sam_count*side_sam_count*2*3);
		// 核心
		int i = 0;
		for (int y = _lod_step ; y < PATCH_SIDETILECOUNT-_lod_step ; y += _lod_step )
		{
			for ( int x = _lod_step ; x < PATCH_SIDETILECOUNT-_lod_step ; x += _lod_step )
			{
				//y*PATCH_SIDEVERTCOUNT + x
				//y*PATCH_SIDEVERTCOUNT + x + _lod_step
				//(y+_lod_step)*PATCH_SIDEVERTCOUNT + x
				//(y+_lod_step)*PATCH_SIDEVERTCOUNT + x + _lod_step
				//  2 3
				//  0 1

				// 0 2 3
				KernalFaces[lod].push_back(y*PATCH_SIDEVERTCOUNT + x);
				KernalFaces[lod].push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + x);
				KernalFaces[lod].push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + x + _lod_step);
				// 0 3 1
				KernalFaces[lod].push_back(y*PATCH_SIDEVERTCOUNT + x);
				KernalFaces[lod].push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + x + _lod_step);
				KernalFaces[lod].push_back(y*PATCH_SIDEVERTCOUNT + x + _lod_step);
			}
		}
		//// 同级
		ushort_vector xFaces;
		ushort_vector XFaces;
		ushort_vector yFaces;
		ushort_vector YFaces;
		ushort_vector xEdge;
		ushort_vector XEdge;
		ushort_vector yEdge;
		ushort_vector YEdge;
		// 小x边缘(x = 0)
		{
			for ( int y = 0 ; y < PATCH_SIDETILECOUNT ; y += _lod_step )
				xEdge.push_back(y*PATCH_SIDEVERTCOUNT);
			for (int y = _lod_step ; y < PATCH_SIDETILECOUNT-_lod_step ; y += _lod_step )
			{
				//y*PATCH_SIDEVERTCOUNT + x
				//y*PATCH_SIDEVERTCOUNT + x + _lod_step
				//(y+_lod_step)*PATCH_SIDEVERTCOUNT + x
				//(y+_lod_step)*PATCH_SIDEVERTCOUNT + x + _lod_step
				//  2 3
				//  0 1
				// 0 2 3
				int x = 0;
				xFaces.push_back(y*PATCH_SIDEVERTCOUNT + x);
				xFaces.push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + x);
				xFaces.push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + x + _lod_step);
				// 0 3 1
				xFaces.push_back(y*PATCH_SIDEVERTCOUNT + x);
				xFaces.push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT  + x+ _lod_step);
				xFaces.push_back(y*PATCH_SIDEVERTCOUNT  + x+ _lod_step);
			}
			// 两头(y = 0, y = PATCH_SIDETILECOUNT-_lod_step)
			// b c
			// a
			xFaces.push_back(0);
			xFaces.push_back(_lod_step*PATCH_SIDEVERTCOUNT);
			xFaces.push_back(_lod_step*PATCH_SIDEVERTCOUNT+_lod_step);
			// b
			// a c
			xFaces.push_back((PATCH_SIDETILECOUNT-_lod_step)*PATCH_SIDEVERTCOUNT);
			xFaces.push_back(PATCH_SIDETILECOUNT*PATCH_SIDEVERTCOUNT);
			xFaces.push_back((PATCH_SIDETILECOUNT-_lod_step)*PATCH_SIDEVERTCOUNT+_lod_step);
		}
		// 大X边缘(x = PATCH_SIDETILECOUNT-_lod_step)
		{
			for ( int y = 0 ; y < PATCH_SIDETILECOUNT ; y += _lod_step )
				XEdge.push_back(y*PATCH_SIDEVERTCOUNT+PATCH_SIDETILECOUNT);
			for (int y = _lod_step ; y < PATCH_SIDETILECOUNT-_lod_step ; y += _lod_step )
			{
				//y*PATCH_SIDEVERTCOUNT + x
				//y*PATCH_SIDEVERTCOUNT + x + _lod_step
				//(y+_lod_step)*PATCH_SIDEVERTCOUNT + x
				//(y+_lod_step)*PATCH_SIDEVERTCOUNT + x + _lod_step
				//  2 3
				//  0 1
				int x = PATCH_SIDETILECOUNT-_lod_step;
				// 0 2 3
				XFaces.push_back(y*PATCH_SIDEVERTCOUNT + x );
				XFaces.push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + x);
				XFaces.push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + _lod_step + x);
				// 0 3 1
				XFaces.push_back(y*PATCH_SIDEVERTCOUNT + x);
				XFaces.push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + _lod_step + x);
				XFaces.push_back(y*PATCH_SIDEVERTCOUNT + _lod_step + x);
			}
			// 两头(y = 0, y = PATCH_SIDETILECOUNT-_lod_step)
			// a b
			//   c
			XFaces.push_back(_lod_step*PATCH_SIDEVERTCOUNT + PATCH_SIDETILECOUNT-_lod_step);
			XFaces.push_back(_lod_step*PATCH_SIDEVERTCOUNT + PATCH_SIDETILECOUNT);
			XFaces.push_back(PATCH_SIDETILECOUNT);
			//   b
			// a c
			int y = (PATCH_SIDETILECOUNT-_lod_step);
			XFaces.push_back((y*PATCH_SIDEVERTCOUNT) + PATCH_SIDETILECOUNT-_lod_step);
			XFaces.push_back(PATCH_SIDEVERTCOUNT*PATCH_SIDEVERTCOUNT-1);
			XFaces.push_back(y*PATCH_SIDEVERTCOUNT + PATCH_SIDETILECOUNT);
		}
		// 小y边缘(y = 0)
		{
			for ( int x = 0 ; x < PATCH_SIDETILECOUNT ; x += _lod_step )
				yEdge.push_back(x);
			for (int x = _lod_step ; x < PATCH_SIDETILECOUNT-_lod_step ; x += _lod_step )
			{
				//y*PATCH_SIDEVERTCOUNT + x
				//y*PATCH_SIDEVERTCOUNT + x + _lod_step
				//(y+_lod_step)*PATCH_SIDEVERTCOUNT + x
				//(y+_lod_step)*PATCH_SIDEVERTCOUNT + x + _lod_step
				//  2 3
				//  0 1
				// 0 2 3
				int y = 0;
				yFaces.push_back(y*PATCH_SIDEVERTCOUNT + x);
				yFaces.push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + x);
				yFaces.push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + x + _lod_step);
				// 0 3 1
				yFaces.push_back(y*PATCH_SIDEVERTCOUNT + x);
				yFaces.push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT  + x+ _lod_step);
				yFaces.push_back(y*PATCH_SIDEVERTCOUNT  + x+ _lod_step);
			}
			// 两头(x = 0, x = PATCH_SIDETILECOUNT-_lod_step)
			//   b
			// a c
			yFaces.push_back(0);
			yFaces.push_back(_lod_step*PATCH_SIDEVERTCOUNT+_lod_step);
			yFaces.push_back(_lod_step);
			// b
			// a c
			yFaces.push_back(PATCH_SIDETILECOUNT-_lod_step);
			yFaces.push_back(_lod_step*PATCH_SIDEVERTCOUNT + PATCH_SIDETILECOUNT-_lod_step);
			yFaces.push_back(PATCH_SIDETILECOUNT);
		}
		// 大Y边缘(y = PATCH_SIDETILECOUNT-_lod_step)
		{
			for ( int x = 0 ; x < PATCH_SIDETILECOUNT ; x += _lod_step )
			{
				YEdge.push_back(PATCH_SIDETILECOUNT*PATCH_SIDEVERTCOUNT + x);
			}
			for (int x = _lod_step ; x < PATCH_SIDETILECOUNT-_lod_step ; x += _lod_step )
			{
				//y*PATCH_SIDEVERTCOUNT + x
				//y*PATCH_SIDEVERTCOUNT + x + _lod_step
				//(y+_lod_step)*PATCH_SIDEVERTCOUNT + x
				//(y+_lod_step)*PATCH_SIDEVERTCOUNT + x + _lod_step
				//  2 3
				//  0 1
				int y = PATCH_SIDETILECOUNT-_lod_step;
				// 0 2 3
				YFaces.push_back(y*PATCH_SIDEVERTCOUNT + x );
				YFaces.push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + x);
				YFaces.push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + _lod_step + x);
				// 0 3 1
				YFaces.push_back(y*PATCH_SIDEVERTCOUNT + x);
				YFaces.push_back((y+_lod_step)*PATCH_SIDEVERTCOUNT + _lod_step + x);
				YFaces.push_back(y*PATCH_SIDEVERTCOUNT + _lod_step + x);
			}
			// 两头(y = 0, y = PATCH_SIDETILECOUNT-_lod_step)
			// a b
			//   c
			YFaces.push_back(PATCH_SIDETILECOUNT*PATCH_SIDEVERTCOUNT);
			YFaces.push_back(PATCH_SIDETILECOUNT*PATCH_SIDEVERTCOUNT + _lod_step);
			YFaces.push_back((PATCH_SIDETILECOUNT-_lod_step)*PATCH_SIDEVERTCOUNT+_lod_step);
			// a b
			// c
			int y = (PATCH_SIDETILECOUNT-_lod_step);
			YFaces.push_back(PATCH_SIDEVERTCOUNT*PATCH_SIDEVERTCOUNT-1 -_lod_step);
			YFaces.push_back(PATCH_SIDEVERTCOUNT*PATCH_SIDEVERTCOUNT-1);
			YFaces.push_back((y*PATCH_SIDEVERTCOUNT) + PATCH_SIDETILECOUNT-_lod_step);
		}

		/// 临街
		xLinkFaces[0][lod][lod] = xFaces;
		xLinkFaces[1][lod][lod] = XFaces;
		xLinkFaces[2][lod][lod] = yFaces;
		xLinkFaces[3][lod][lod] = YFaces;
		// 只向高级别输送
		for ( int otherlod = lod+1 ; otherlod < MAX_LOD_LEVEL ; ++otherlod )
		{
			// 隔几个点删除？
			int _kill_step = 1 << (otherlod-lod);
			{
				xLinkFaces[0][lod][otherlod] = xFaces;
				// 此次被删点
				std::vector<ushort> _killpoints;
				// 替换点
				std::vector<ushort> _mergepoints;
				ushort _mergeid = 0;
				for ( int i = 0 ; i < xEdge.size() ; ++i )
				{
					if ( i % _kill_step != 0 )
					{
						_killpoints.push_back(xEdge[i]);
						_mergepoints.push_back(xEdge[_mergeid]);
					}
					else
					{
						_mergeid = i;
					}
				}
				// 搜索，凡是属于killpoints的，则替换为上一个点
				for ( size_t i = 0 ; i < xLinkFaces[0][lod][otherlod].size() ; ++i )
					for ( size_t j = 0 ; j < _killpoints.size() ; ++j )
						if ( xLinkFaces[0][lod][otherlod][i] == _killpoints[j] )
							xLinkFaces[0][lod][otherlod][i] = _mergepoints[j];
			}
			{
				// 此次被删点
				std::vector<ushort> _killpoints;
				// 替换点
				std::vector<ushort> _mergepoints;
				xLinkFaces[1][lod][otherlod] = XFaces;
				ushort _mergeid = 0;
				for ( int i = 0 ; i < XEdge.size() ; ++i )
				{
					if ( i % _kill_step != 0 )
					{
						_killpoints.push_back(XEdge[i]);
						_mergepoints.push_back(XEdge[_mergeid]);
					}
					else
					{
						_mergeid = i;
					}
				}
				// 搜索，凡是属于killpoints的，则替换为上一个点
				for ( size_t i = 0 ; i < xLinkFaces[1][lod][otherlod].size() ; ++i )
					for ( size_t j = 0 ; j < _killpoints.size() ; ++j )
						if ( xLinkFaces[1][lod][otherlod][i] == _killpoints[j] )
							xLinkFaces[1][lod][otherlod][i] = _mergepoints[j];
			}
			{
				// 此次被删点
				std::vector<ushort> _killpoints;
				// 替换点
				std::vector<ushort> _mergepoints;
				xLinkFaces[2][lod][otherlod] = yFaces;
				ushort _mergeid = 0;
				for ( int i = 0 ; i < yEdge.size() ; ++i )
				{
					if ( i % _kill_step != 0 )
					{
						_killpoints.push_back(yEdge[i]);
						_mergepoints.push_back(yEdge[_mergeid]);
					}
					else
					{
						_mergeid = i;
					}
				}
				// 搜索，凡是属于killpoints的，则替换为上一个点
				for ( size_t i = 0 ; i < xLinkFaces[2][lod][otherlod].size() ; ++i )
					for ( size_t j = 0 ; j < _killpoints.size() ; ++j )
						if ( xLinkFaces[2][lod][otherlod][i] == _killpoints[j] )
							xLinkFaces[2][lod][otherlod][i] = _mergepoints[j];
			}
			{
				// 此次被删点
				std::vector<ushort> _killpoints;
				// 替换点
				std::vector<ushort> _mergepoints;
				xLinkFaces[3][lod][otherlod] = YFaces;
				ushort _mergeid = 0;
				for ( int i = 0 ; i < YEdge.size() ; ++i )
				{
					if ( i % _kill_step != 0 )
					{
						_killpoints.push_back(YEdge[i]);
						_mergepoints.push_back(YEdge[_mergeid]);
					}
					else
					{
						_mergeid = i;
					}
				}
				// 搜索，凡是属于killpoints的，则替换为上一个点
				for ( size_t i = 0 ; i < xLinkFaces[3][lod][otherlod].size() ; ++i )
					for ( size_t j = 0 ; j < _killpoints.size() ; ++j )
						if ( xLinkFaces[3][lod][otherlod][i] == _killpoints[j] )
							xLinkFaces[3][lod][otherlod][i] = _mergepoints[j];
			}
		}
	}
}


END_NAMESPACE_XEVOL3D