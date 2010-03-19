#include "geotiffio.h"
#include "xTerrainTool.h"
#include "geo_normalize.h"
#include "libxtiff/xtiffio.h"
#include "xTerrainData.h"
#include "OperationSys/xOperationSys.h"

BEGIN_NAMESPACE_XEVOL3D


#define _TEMP_SX 5000.0
#define _TEMP_SY 4000.0

//------------------------------------------------------------------------

xTerrainData::xTerrainData()
{
	m_pDepth;
	m_ResX;
	m_ResY;
	m_realStartX;
	m_realStartY;
	m_ccImageName;

	m_hTexture;
	m_surfResX;
	m_surfResY;
}

xTerrainData::~xTerrainData()
{
	if(m_pDepth)
	{
		delete[] m_pDepth;
		m_pDepth = NULL;
	}
}

void xTerrainData::_format(unsigned int Xinlevel, unsigned int Yinlevel, double setStartX, double setStartY)
{
	double rstartX = m_realStartX - setStartX;
	double rstartY = m_realStartY - setStartY;
	double rendX = m_realEndX - setStartX;
	double rendY = m_realEndY - setStartY;

	m_DataIDXInLevel	= Xinlevel;
	m_DataIDYInLevel	= Yinlevel;
	m_relativeStart.x	= (float)rstartX;
	m_relativeStart.y	= (float)rstartY;
	m_relativeStart.z	= 0.0f;
	m_vRelativeCenter.x	= ((float)rendX - rstartX) * 0.5f;
	m_vRelativeCenter.y	= ((float)rendY - rstartY) * 0.5f;
}

void xTerrainData::patchView(unsigned int startX, unsigned int startY, unsigned int sampleX, unsigned int sampleY, std::vector<float>& outDepth, xTerrainData::SurfaceData& outSurface)
{
	// Depth
	for ( unsigned int y = 0 ; y < sampleY ; ++y )
	{
		for ( unsigned int x = 0 ; x < sampleX ; ++x )
		{
			unsigned int realX = startX + x;
			unsigned int realY = startY + y;
			outDepth.push_back( m_pDepth[realY*m_ResX+realX]);
		}
	}

	// Surface
	outSurface.minU = (float)startX / (float)m_surfResX;
	outSurface.minV = (float)startY / (float)m_surfResY;
	outSurface.sizeU = (float)sampleX / (float)m_surfResX;
	outSurface.sizeV = (float)sampleY / (float)m_surfResY;
	outSurface.hTexture = m_hTexture;
}

bool xTerrainData::load(xBaseTextureMgr* pTexMgr, xXmlNode* pNode)
{
	xXmlValue* pVal = pNode->findValue(L"TempDEMData");
	if ( !pVal )
	{
		return false;
	}
	m_ccImageName = pVal->value();
	// 读取DEM数据
	TIFF* pTiff = XTIFFOpenW( _RES_ABSPATH_(m_ccImageName.c_str()), "r");
	if (!pTiff)
	{
		return false;
	}

	// 转化为Geo Tiff
	GTIF* gtif = GTIFNew(pTiff);
	if (!gtif)
	{
		if (pTiff) XTIFFClose(pTiff);
		return false;
	}

	// 获取Normalize信息
	GTIFDefn GTIFDefN;
	GTIFGetDefn( gtif, &GTIFDefN );

	// 分析所有Geo Tiff Tag
	uint32 imageWidth, imageLength;
	TIFFGetField(pTiff, TIFFTAG_IMAGEWIDTH, &imageWidth);
	TIFFGetField(pTiff, TIFFTAG_IMAGELENGTH, &imageLength);
	uint32 tileWidth, tileLength;
	TIFFGetField(pTiff, TIFFTAG_TILEWIDTH, &tileWidth);
	TIFFGetField(pTiff, TIFFTAG_TILELENGTH, &tileLength);
	//double tiepoints[6]={0,0,0,0,0,0};
	//double pixscale[3]={0,0,0};
	// Geospecific Tags
	short tiepointsize, pixscalesize;
	double* tiepoints;//[6];
	double* pixscale;//[3];
	TIFFGetField(pTiff, TIFFTAG_GEOTIEPOINTS,  &tiepointsize,&tiepoints);
	TIFFGetField(pTiff, TIFFTAG_GEOPIXELSCALE, &pixscalesize,&pixscale);
	m_vScale.x = pixscale[0];	m_vScale.y = pixscale[1];	m_vScale.z = pixscale[2];
	if ( pixscale[2] < 0.0001f )
		m_vScale.z = pixscale[0];

	// Corner
	double minX = 0.0;
	double minY = imageLength;
	double maxX = imageWidth;
	double maxY = 0.0;
	if( !GTIFImageToPCS( gtif, &minX, &minY ) )
		return false;
	if( !GTIFImageToPCS( gtif, &maxX, &maxY ) )
		return false;

	m_ResX = 1025;
	m_ResY = 1025;
	m_pDepth = new float[m_ResX*m_ResY];
	// 获取采样点对应的米数，将它们等分1024份（供数据区读取）
	float SizeX = imageWidth * m_vScale.x;
	float SizeY = imageLength * m_vScale.y;
	m_vScale.x = SizeX / m_ResX;
	m_vScale.y = SizeX / m_ResY;

	m_realStartX	= minX;
	m_realStartY	= minY;
	m_realEndX		= maxX;
	m_realEndY		= maxY;

	// Tile方式读取，读取完毕后放入到临时存储区中，以备修正和生成顶点。
	if ( TIFFIsTiled(pTiff) )
	{

		// 读出原始数据（可能是1000X800）
		float* OriginDepth = new float[imageWidth*imageLength];
		tdata_t buf;
		buf = _TIFFmalloc(TIFFTileSize(pTiff));
		for (uint32 y = 0; y < imageLength; y += tileLength)
		{
			for (uint32 x = 0; x < imageWidth; x += tileWidth)
			{
				TIFFReadTile(pTiff, buf, x, y, 0,0 );
				float * pBuf = (float*)buf;
				for ( uint32 ty = y; (ty<imageLength)&&(ty<(y+tileLength)) ; ++ty )		{
					for ( uint32 tx = x; (tx<imageWidth)&&(tx<(x+tileWidth)) ; ++tx )	{
						OriginDepth[ty*imageWidth + tx] = pBuf[(ty-y)*tileWidth + (tx-x)];
					}
				}
			}
		}
		_TIFFfree(buf);
		TIFFClose(pTiff);
		GTIFFree(gtif);

		// 原始（如：1000 X 800）算到1025 X 1025

        GeoTif_Scale(OriginDepth  , imageWidth , imageLength , m_pDepth , m_ResX , m_ResY);

		float xfactor = ((float) (imageWidth - 1)) / 1024.0f;
		float yfactor = ((float) (imageLength - 1)) / 1024.0f;
		// 生成高度信息
		uint32 px , py;
		for ( uint32 y = 0 ; y < m_ResY ; ++y )
		{
			for ( uint32 x = 0 ; x < m_ResX ; ++x )
			{
				// 找到这个顶点在像素图上的像素位置
				px = x * xfactor;
				py = y * yfactor;
				// 取此像素的高度
				m_pDepth[m_ResX*y + x] = OriginDepth[py*imageWidth + px];
			}
		}
	}

	// 加载Surface
	m_ccTextureName = pNode->findValue(L"TempSurfData")->value();
	m_hTexture  = pTexMgr->loadTexture(_RES_ABSPATH_( m_ccTextureName.c_str() ) , true , 0);
	m_surfResX	= pNode->findValue(L"TempSurfSizeX")->int_value();
	m_surfResY	= pNode->findValue(L"TempSurfSizeY")->int_value();

	_TileX = m_realStartX / _TEMP_SX;
	_TileY = m_realStartY / _TEMP_SY;

	double tilesx = _TEMP_SX;
	double tilesy = _TEMP_SY;
	double tsx = getRealEndX() - getRealStartX();
	double tsy = getRealEndY() - getRealStartY();
	double decx = tsx - tilesx;
	double decy = tsy - tilesy;
	// TODO 更好的设计
	if ( abs(decx) > 100 || abs(decy) > 100 )
	{
		// TODO log error
		return false;
	}

	return true;
}

bool xTerrainData::save(xXmlNode* pNode)
{
	pNode->insertValue(L"TempDEMData")->setValue(m_ccImageName.c_str());
	pNode->insertValue(L"TempSurfData")->setValue(m_ccTextureName.c_str());
	pNode->insertValue(L"TempSurfSizeX")->setValue(m_surfResX);
	pNode->insertValue(L"TempSurfSizeY")->setValue(m_surfResY);

	return true;
}

float	xTerrainData::getAcceptableData(double x, double y)
{
	double sizex = (m_realEndX - m_realStartX)/1024.0;
	double sizey = (m_realEndY - m_realStartY)/1024.0;
	// 将xy映射到ID空间：
	int xid = (x - m_realStartX)/sizex;
	int yid = (y - m_realStartY)/sizey;
	// 此ID的数据是否合法？
	if ( xid >= 0 && xid < m_ResX && yid >= 0 && yid < m_ResY && m_pDepth[yid*m_ResX+xid] > -20000.0f )
	{
		return m_pDepth[yid*m_ResX+xid];
	}
	// TODO如果不合法，寻找周边合法数据
	return -30000.0f;
}

void xTerrainData::_clearupEdges(xTerrainData* px, xTerrainData* pX, xTerrainData* py, xTerrainData* pY)
{
	double xsize_per_samp = (m_realEndX - m_realStartX)/1024.0;
	double ysize_per_samp = (m_realEndY - m_realStartY)/1024.0;
	// 
	for (uint32 y = 0; y < m_ResY; ++y )
	{
		for (uint32 x = 0; x < m_ResX; ++x)
		{
			if(m_pDepth[y*m_ResX+x] < -20000.0f)
			{
				double realx = x * xsize_per_samp + m_realStartX;
				double realy = y * ysize_per_samp + m_realStartY;
				// 从4个方向得到数值。
				float vx = -30000.0f;
				if(px) vx = px->getAcceptableData(realx, realy);
				float vX = -30000.0f;
				if(pX) vX = pX->getAcceptableData(realx, realy);
				float vy = -30000.0f;
				if(py) vy = py->getAcceptableData(realx, realy);
				float vY = -30000.0f;
				if(pY) vY = pY->getAcceptableData(realx, realy);
				if ( vx > -20000.0f )m_pDepth[y*m_ResX+x] = vx;
				if ( vX > -20000.0f )m_pDepth[y*m_ResX+x] = vX;
				if ( vy > -20000.0f )m_pDepth[y*m_ResX+x] = vy;
				if ( vY > -20000.0f )m_pDepth[y*m_ResX+x] = vY;
			}
		}
	}
}

//------------------------------------------------------------------------



//------------------------------------------------------------------------

xTerrainDataSet::xTerrainDataSet()
{
	m_TerrainHeightDatas.clear();
	m_TerrainOriginX = 100000000000000000000000000000.0;
	m_TerrainOriginY = 100000000000000000000000000000.0;
	m_TerrainSizeX = 0;
	m_TerrainSizeY = 0;
	m_TerrainDataCountX = 0;
	m_TerrainDataCountY = 0;
}

xTerrainDataSet::~xTerrainDataSet()
{
	for ( size_t i = 0 ; i < m_TerrainHeightDatas.size() ; ++i )
	{
		if ( m_TerrainHeightDatas[i] )
		{
			delete m_TerrainHeightDatas[i];
		}
	}
}

double xTerrainDataSet::getOriginX()
{
	return m_TerrainOriginX;
}

double xTerrainDataSet::getOriginY()
{
	return m_TerrainOriginY;
}

double xTerrainDataSet::getSizeX()
{
	return m_TerrainSizeX;
}

double xTerrainDataSet::getSizeY()
{
	return m_TerrainSizeY;
}

float xTerrainDataSet::getHeight(float X, float Y)
{
	double SvrSizeX = m_TerrainSizeX / m_TerrainDataCountX;
	unsigned int xid = X / SvrSizeX;
	double SvrSizeY = m_TerrainSizeY / m_TerrainDataCountY;
	unsigned int yid = Y / SvrSizeY;
	for (size_t i = 0 ; i < m_TerrainHeightDatas.size(); ++i)
	{
		if ( m_TerrainHeightDatas[i]->getXID() == xid && m_TerrainHeightDatas[i]->getYID() == yid )
		{
			return m_TerrainHeightDatas[i]->getHeight(X,Y);
		}
	}

	return 0.0f;
}

float xTerrainDataSet::getHeightReal(double X, double Y)
{
	return getHeight(X-m_TerrainOriginX, Y-m_TerrainOriginY);
}

void xTerrainDataSet::generatePatchView( unsigned int tileCountX, unsigned int tileCountY, unsigned int& patchXCount, unsigned int& patchYCount, std::vector<PatchDataView>& pTerData )
{
	unsigned int px = 1024 / tileCountX;
	unsigned int py = 1024 / tileCountY;
	patchXCount = m_TerrainDataCountX * px;
	patchYCount = m_TerrainDataCountY * py;
	for ( unsigned int y = 0 ; y < patchYCount ; ++y )
	{
		for ( unsigned int x = 0 ; x < patchXCount ; ++x )
		{
			xTerrainData* pData = NULL;
			for (size_t i = 0 ; i < m_TerrainHeightDatas.size(); ++i)
			{
				if ( m_TerrainHeightDatas[i]->getXID() == (x/px) && m_TerrainHeightDatas[i]->getYID() == (y/py) )
				{
					pData = m_TerrainHeightDatas[i];
					break;
				}
			}
			PatchDataView pdv;
			pdv.pData = pData;
			pdv.xID = x%px;
			pdv.yID = y%py;
			pTerData.push_back(pdv);
		}
	}
}

bool xTerrainDataSet::load(xXmlNode* pNode)
{

	int node_count = pNode->countNode();
	std::vector< xTerrainData*,dllsafe_alloc<xTerrainData*> > TerrainDatas;
	for ( int i = 0 ; i < node_count ; ++i )
	{
		xXmlNode* pChild = pNode->findNode(i);
		xTerrainData* pTerrainData = new xTerrainData();
		if( !pTerrainData->load(m_pTexMgr, pChild) )
		{
			delete pTerrainData;
			continue;
		}
		TerrainDatas.push_back(pTerrainData);
	}

	// 算出包围盒
	double sx = -100000000000000000000000000000.0;
	double sy = -100000000000000000000000000000.0;
	// 用以区分不同的Data之间是否有特别大（小）的块
	int origin_idx = INT_MAX;
	int origin_idy = INT_MAX;

	if ( TerrainDatas.empty() )
	{
		return false;
	}

	vector<size_t> killData;
	// TODO 更好的设计
	// 根据配置，预算格子大小（因为DEM是等大小的）
	// 读取某个节点，并适配某个格子，第一个格子决定所有适配的空间位置，Tile从0,0开始起算。
	// 第二个格子开始，适配空间，对于地形数据，我们检查X方向和Y方向均除以5000（TODO 当前版本）。
	// 适配需要修改数据区域，直到相邻数据区域完全缝合。
	for (size_t i = 0 ; i < TerrainDatas.size(); ++i)
	{
		double tilesx = _TEMP_SX;
		double tilesy = _TEMP_SY;
		double tsx = TerrainDatas[i]->getRealEndX() - TerrainDatas[i]->getRealStartX();
		double tsy = TerrainDatas[i]->getRealEndY() - TerrainDatas[i]->getRealStartY();

		m_TerrainOriginX = min(m_TerrainOriginX,TerrainDatas[i]->getRealStartX());
		m_TerrainOriginY = min(m_TerrainOriginY,TerrainDatas[i]->getRealStartY());
		origin_idx = min(origin_idx,TerrainDatas[i]->GetTileX());
		origin_idy = min(origin_idy,TerrainDatas[i]->GetTileY());
		m_TerrainSizeX	= max(m_TerrainSizeX,TerrainDatas[i]->getRealEndX());
		m_TerrainSizeY	= max(m_TerrainSizeY,TerrainDatas[i]->getRealEndY());

		double decx = tsx - tilesx;
		double decy = tsy - tilesy;
		// TODO 更好的设计
		if ( abs(decx) > 100 || abs(decy) > 100 )
		{
			// TODO log error
			return false;
		}
	}
	m_TerrainSizeX	-= m_TerrainOriginX;
	m_TerrainSizeY	-= m_TerrainOriginY;

	// 计算大致覆盖的地形数量
	m_TerrainDataCountX = m_TerrainSizeX / _TEMP_SX + 1;
	m_TerrainDataCountY = m_TerrainSizeY / _TEMP_SY + 1;

	bool* BoolArray = new bool[m_TerrainDataCountX*m_TerrainDataCountY];
	memset(BoolArray, 0, sizeof(BoolArray));

	// 算出每个地形的覆盖范围
	// 计算这个data是落在了哪个范围内
	for (size_t i = 0 ; i < TerrainDatas.size(); ++i)
	{
		unsigned int xid = TerrainDatas[i]->GetTileX() - origin_idx;
		unsigned int yid = TerrainDatas[i]->GetTileY() - origin_idy;
		if ( BoolArray[xid+yid*m_TerrainDataCountX] )
		{
			killData.push_back(i);
		}
		BoolArray[xid+yid*m_TerrainDataCountX] = true;
		TerrainDatas[i]->_format( xid, yid, m_TerrainOriginX, m_TerrainOriginY );
	}

	// 已经存在的应该被抹去
	for (size_t i = 0 ; i < killData.size(); ++i)
	{
		size_t k = killData[i];
		if (TerrainDatas[k])
		{
			delete TerrainDatas[k];
			TerrainDatas[k] = NULL;
		}
	}
	for (size_t i = 0 ; i < TerrainDatas.size(); ++i)
	{
		if ( TerrainDatas[i] != NULL )
		{
			m_TerrainHeightDatas.push_back(TerrainDatas[i]);
		}
	}
	delete[] BoolArray;

	// 平整地形高度，处理边缘地带。
	for (size_t i = 0 ; i < m_TerrainHeightDatas.size(); ++i)
	{
		xTerrainData* px = NULL;	xTerrainData* pX = NULL;	xTerrainData* py = NULL;	xTerrainData* pY = NULL;
		GetSideTerrain(m_TerrainHeightDatas[i], &px, &pX, &py, &pY);
		m_TerrainHeightDatas[i]->_clearupEdges(px, pX, py, pY);
	}

	return true;
}

void xTerrainDataSet::GetSideTerrain(xTerrainData* pSource, xTerrainData** ppx, xTerrainData** ppX, xTerrainData** ppy, xTerrainData** ppY)
{
	for (size_t i = 0 ; i < m_TerrainHeightDatas.size(); ++i)
	{
		if (m_TerrainHeightDatas[i]->GetTileX() == pSource->GetTileX() - 1
			&& m_TerrainHeightDatas[i]->GetTileY() == pSource->GetTileY())
		{
			*ppx = m_TerrainHeightDatas[i];
		}
		if (m_TerrainHeightDatas[i]->GetTileX() == pSource->GetTileX() + 1
			&& m_TerrainHeightDatas[i]->GetTileY() == pSource->GetTileY())
		{
			*ppX = m_TerrainHeightDatas[i];
		}
		if (m_TerrainHeightDatas[i]->GetTileY() == pSource->GetTileY() - 1
			&& m_TerrainHeightDatas[i]->GetTileX() == pSource->GetTileX())
		{
			*ppy = m_TerrainHeightDatas[i];
		}
		if (m_TerrainHeightDatas[i]->GetTileY() == pSource->GetTileY() + 1
			&& m_TerrainHeightDatas[i]->GetTileX() == pSource->GetTileX())
		{
			*ppY = m_TerrainHeightDatas[i];
		}
	}
}

bool xTerrainDataSet::save(xXmlNode* pNode)
{
	for (size_t i = 0 ; i < m_TerrainHeightDatas.size(); ++i)
	{
		xXmlNode* pDataNode = pNode->insertNode(L"TerrainData");
		if ( pDataNode )
		{
			m_TerrainHeightDatas[i]->save(pDataNode);
		}
	}
	return true;
}

bool xTerrainDataSet::analysis()
{
	return true;
}


//------------------------------------------------------------------------



END_NAMESPACE_XEVOL3D