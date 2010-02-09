#include "geotiffio.h"
#include "geo_normalize.h"
#include "libxtiff/xtiffio.h"
#include "xTerrainData.h"
#include "OperationSys/xOperationSys.h"

BEGIN_NAMESPACE_XEVOL3D


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
		float xfactor = ((float) (imageWidth - 1)) / 1024.0f;
		float yfactor = ((float) (imageLength - 1)) / 1024.0f;
		// 生成高度信息
		uint32 px , py;
		float ZAvr = 0.0f;
		int ZAvrCount = 0;
		for ( uint32 y = 0 ; y < m_ResY ; ++y )
		{
			for ( uint32 x = 0 ; x < m_ResX ; ++x )
			{
				// 找到这个顶点在像素图上的像素位置
				px = x * xfactor;
				py = y * yfactor;
				// 取此像素的高度
				m_pDepth[m_ResX*y + x] = OriginDepth[py*imageWidth + px];
				if ( m_pDepth[m_ResX*y + x] > -20000.0f )
				{
					ZAvr += m_pDepth[m_ResX*y + x];
					++ZAvrCount;
					m_vRelativeCenter.z	= max(m_vRelativeCenter.z, m_pDepth[m_ResX*y + x]);
				}
			}
		}
		ZAvr /= ZAvrCount;
		for ( uint32 y = 0 ; y < m_ResY ; ++y )
		{
			for ( uint32 x = 0 ; x < m_ResX ; ++x )
			{
				// 取此像素的高度
				if ( m_pDepth[m_ResX*y + x] < -20000.0f )
				{
					m_pDepth[m_ResX*y + x] = ZAvr;
				}
			}
		}
	}

	// 加载Surface
	m_ccTextureName = pNode->findValue(L"TempSurfData")->value();
	m_hTexture  = pTexMgr->loadTexture(_RES_ABSPATH_( m_ccTextureName.c_str() ) , true , 0);
	m_surfResX	= pNode->findValue(L"TempSurfSizeX")->int_value();
	m_surfResY	= pNode->findValue(L"TempSurfSizeY")->int_value();

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
	// TODO 更好的设计
	double tilesx = 0;
	double tilesy = 0;

	for (size_t i = 0 ; i < TerrainDatas.size(); ++i)
	{
		double tsx = TerrainDatas[i]->getRealEndX() - TerrainDatas[i]->getRealStartX();
		double tsy = TerrainDatas[i]->getRealEndY() - TerrainDatas[i]->getRealStartY();

		m_TerrainOriginX = min(m_TerrainOriginX,TerrainDatas[i]->getRealStartX());
		m_TerrainOriginY = min(m_TerrainOriginY,TerrainDatas[i]->getRealStartY());
		m_TerrainSizeX	= max(m_TerrainSizeX,TerrainDatas[i]->getRealEndX());
		m_TerrainSizeY	= max(m_TerrainSizeY,TerrainDatas[i]->getRealEndY());

		if ( tilesx < 1 && tilesy < 1 )
		{
			tilesx = TerrainDatas[i]->getRealEndX() - TerrainDatas[i]->getRealStartX();
			tilesy = TerrainDatas[i]->getRealEndY() - TerrainDatas[i]->getRealStartY();
			continue;
		}

		double decx = tsx - tilesx;
		double decy = tsy - tilesy;
		// TODO 更好的设计
		if ( decx > 50 || decy > 50 )
		{
			// TODO log error
			return false;
		}
		tilesx = TerrainDatas[i]->getRealEndX() - TerrainDatas[i]->getRealStartX();
		tilesy = TerrainDatas[i]->getRealEndY() - TerrainDatas[i]->getRealStartY();
	}
	m_TerrainSizeX	-= m_TerrainOriginX;
	m_TerrainSizeY	-= m_TerrainOriginY;

	// 计算大致覆盖的地形数量
	m_TerrainDataCountX = m_TerrainSizeX / tilesx + 1;
	m_TerrainDataCountY = m_TerrainSizeY / tilesy + 1;

	bool* BoolArray = new bool[m_TerrainDataCountX*m_TerrainDataCountY];
	memset(BoolArray, 0, sizeof(BoolArray));

	// 算出每个地形的覆盖范围
	// 计算这个data是落在了哪个范围内
	vector<size_t> killData;
	for (size_t i = 0 ; i < TerrainDatas.size(); ++i)
	{
		double tsx = TerrainDatas[i]->getRealEndX() - TerrainDatas[i]->getRealStartX();
		double tsy = TerrainDatas[i]->getRealEndY() - TerrainDatas[i]->getRealStartY();
		double tcx = tsx*0.5 + TerrainDatas[i]->getRealStartX();
		double tcy = tsy*0.5 + TerrainDatas[i]->getRealStartY();
		unsigned int xid = (tcx-m_TerrainOriginX) / tilesx;
		unsigned int yid = (tcy-m_TerrainOriginY) / tilesy;
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

	return true;
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