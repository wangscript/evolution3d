#ifndef _TERRAIN_DATA_H_
#define _TERRAIN_DATA_H_

#include "xTerrainDefinations.h"
#include "BaseLib/xStringTable.h"
#include "RenderAPI/xTextureManager.h"
#include "XMathLib/xvolume.h"

using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
// 有这样几个概念：
// DataSet：		当前唯一的，指示当前所编辑的所有区域的总和，由相邻的区域构成
//
// Data：			某个区域的数据，指示正常世界中的某个区块，由采样点组成。
//					采样是根据精度来的。例如，一个1km x 1km的区域，用5m精度，得出的采样点是200x200个。
//					同样大小的区域，如果用1m x 1m的精度，采样点就是1000x1000个。
//					判断两个Data是否邻接的重要信息就是位置。一个Data的最终位置如果能够与另一个Data的最终位置在某条边缘上重合，则可以认为是处于邻接的状态。
//					由于表面纹理的因素，某个区域只能被若干个Patch分辨开。如果不能做到，则需要插值来确保这一点。
//
// TerrainPatch：	渲染的概念，一个Patch相当于是Data Set的一个View，View一块到多块Data。
//					Terrain Patch可以View一块到多块的Data，有如下情况需要处理：
//					Patch映射多块，这个是比较基本的，不说了。
//					Patch映射多块，但有些块没有数据，这时候就缺少了邻接块，那么在LOD处理时需要注意。
//
// 这里有两个方案：
//		当前的方案是，同时刻只能存在一组精度的Data，这样的情况下，我们可以认为Patch可以直接反映一组精度范围。
//		目前每个块是1000x800，因此映射到1024x1024，也就是8x8Patch。
//
//		还有一种方案是允许不同的Terrain块具备不同的精度，这样的情况下，考虑到拼接问题，Patch的组织必须按照LOD来。
//		例如，无论1m精度还是5m精度，Terrain块的数据都一样，只是1m精度下用LOD0来显示（最高不超过LOD0），5m精度下用LOD4来显示（最高不超过LOD4）。
//


// 区域数据区
class xTerrainData
{
public:
	xTerrainData();
	~xTerrainData();
	struct SurfaceData
	{
		HBaseTexture hTexture;
		float minU;
		float minV;
		float sizeU;
		float sizeV;
	};

private:

	// 高程表
	float*	m_pDepth;

	// 采样点数量（按照采样点来算，单位：点）
	unsigned int m_ResX;
	unsigned int m_ResY;

	// 当前Patch在真实经纬度下的起始和结束
	double m_realStartX;
	double m_realStartY;
	double m_realEndX;
	double m_realEndY;

	// 管理器为其分配的Tile ID
	int _TileX;
	int _TileY;

	// 数据精度（每个采样点所换算的真实长度，单位：米）
	xvec3 m_vScale;

	// 对应的TIFF
	std::ds_wstring m_ccImageName;

	// 以下数据加入到Set才会生成（_format方法）

	unsigned int m_DataIDXInLevel;
	unsigned int m_DataIDYInLevel;
	// 中心点（从数据集计算得到的起始坐标系）
	xvec3 m_vRelativeCenter;
	// 数据区起始（从数据集计算得到的起始坐标系）
	xvec3 m_relativeStart;

private:
	// 表面表
	// TODO 表面文件
	HBaseTexture m_hTexture;
	// 表面大小（按照采样点来算，单位：点）
	unsigned int m_surfResX;
	unsigned int m_surfResY;
	std::ds_wstring m_ccTextureName;
	//// 表面精度（每个表面点所换算的真实长度，单位：米）
	//float m_surfDeltaX;
	//float m_surfDeltaY;

public:

	inline 	int GetTileX() const { return _TileX; }
	inline 	int GetTileY() const { return _TileY; }

	inline double getRealStartX() {return m_realStartX;}
	inline double getRealStartY() {return m_realStartY;}
	inline double getRealEndX() {return m_realEndX;}
	inline double getRealEndY() {return m_realEndY;}
	inline unsigned int getXID() {return m_DataIDXInLevel;}
	inline unsigned int getYID() {return m_DataIDYInLevel;}

public:
	inline unsigned int getResX(){return m_ResX;}
	inline unsigned int getResY(){return m_ResY;}

	inline unsigned int getSurfaceResX(){ return m_surfResX; }
	inline unsigned int getSurfaceResY(){ return m_surfResY; }

	inline const xvec3& getScale() const { return m_vScale; }
	inline const xvec3& getStart() const { return m_relativeStart; }
	inline const xvec3& getCenter() const{ return m_vRelativeCenter; }

	inline float		getHeight(float X,float Y)
	{
		unsigned int x = X - m_relativeStart.x;
		unsigned int y = Y - m_relativeStart.y;
		if ( x < m_ResX && y < m_ResY )
			return m_pDepth[y*m_ResX + x];
		return 0.0f;
	}

	float	getAcceptableData(double x, double y);

public:

	void _format(unsigned int Xinlevel, unsigned int Yinlevel, double setStartX, double setStartY);
	void _clearupEdges(xTerrainData* px, xTerrainData* pX, xTerrainData* py, xTerrainData* pY);
	// 查询采样点
	// 查询表面纹理
	// 一个地形数据集由多块地形Patch渲染，因此每个地形Patch只是View了这个数据集中的一小块。
	void patchView(unsigned int startX, unsigned int startY, unsigned int sampleX, unsigned int sampleY, std::vector<float>& outDepth, SurfaceData& outSurface);

	bool load(xBaseTextureMgr* pTexMgr, xXmlNode* pNode);
	bool save(xXmlNode* pNode);

};

// xTerrainDataSet是TerrainData的集合，按照经纬度来进行管理
// 同时只能有一个xTerrainDataSet
class xTerrainDataSet
{
public:
	xTerrainDataSet();
	~xTerrainDataSet();
private:

	// Terrain Data组
	// 一个Scene可能包含多组Height Data，每组Height Data按照经纬度来区分，精度因素也需要考虑进去。
	// Terrain Data占位是按照实际占位来算的。
	std::vector< xTerrainData*,dllsafe_alloc<xTerrainData*> > m_TerrainHeightDatas;
	// Bound
	double			m_TerrainOriginX;
	double			m_TerrainOriginY;
	double			m_TerrainSizeX;
	double			m_TerrainSizeY;
	// Terrain Data横向覆盖（有些Data可能不存在）
	unsigned int	m_TerrainDataCountX;
	// Terrain Data纵向覆盖（有些Data可能不存在）
	unsigned int	m_TerrainDataCountY;
	xBaseTextureMgr* m_pTexMgr;

public:

	// TODO 更好的设计
	inline void _setTextureMgr(xBaseTextureMgr* pTexMgr) { m_pTexMgr = pTexMgr; }

	// 查询当前数据集坐标系原始顶点
	double getOriginX();
	double getOriginY();
	double getSizeX();
	double getSizeY();

	// 查询某点的高度（这里的X,Y是指从数据集局部坐标系的位置）
	float getHeight(float X, float Y);
	// 查询某点的高度（这里的X,Y是指真实位置）
	float getHeightReal( double x, double y);

	void GetSideTerrain(xTerrainData* pSource, xTerrainData** ppx, xTerrainData** ppX, xTerrainData** ppy, xTerrainData** ppY);
public:

	bool load(xXmlNode* pNode);
	bool save(xXmlNode* pNode);
	// 注意：TerData可能是空的（例如某个块是空的）
	struct PatchDataView
	{
		xTerrainData* pData;
		unsigned int xID;
		unsigned int yID;
	};
	void generatePatchView( unsigned int tileCountX, unsigned int tileCountY, unsigned int& patchXCount, unsigned int& patchYCount, std::vector<PatchDataView>& pTerData );

private:

	// 分析邻接、完整性，等等
	bool analysis();

};


END_NAMESPACE_XEVOL3D

#endif