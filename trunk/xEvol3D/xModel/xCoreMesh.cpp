#include "../xStdPch.h"
#include "xCoreMesh.h"
#include "xCoreAction.h"
#include "../xcomdoc/xdocfstream.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xCoreMesh  , IBaseResource);


xCoreMesh::xCoreMesh(xBaseTextureMgr* pTexMgr , bool bSusMemCopy)
:IRenderOtherRes(NULL)
{
    m_RefCount = 1;
	m_pRenderApi = pTexMgr->renderApi();
	m_pTexMgr    = pTexMgr;
	m_pInputAss = NULL;
	m_pVertexStream = NULL;
    //目前从DirectX 10开始支持GPU Skin
    m_bGpuSkin = m_pRenderApi->featureLevel() >= IRenderApi::eFeature_ShaderModel4;
    m_pSkinVertexData = NULL;
    m_pStaticVertexData = NULL;
}

bool xCoreMesh::load(const wchar_t* fileName , unsigned long  arg)
{
	return load(fileName , NULL , 0 , arg);
}

bool xCoreMesh::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	xcomdoc doc;
	bool bRet = true;
	if(buf == NULL || bufLen == 0)
	{
		bRet = doc.open(fileName,  xcdm_read);
	}
	else
	{
		bRet = doc.open( (const _xcd_int8*)buf, (size_t)bufLen,  xcdm_read);
	}

	if( bRet == false )
		return false;

	xcomdocstream* pDesc = doc.open_stream(L"desc.xml");
	if(pDesc != NULL)
	{
		bRet = load(doc , L"" , arg);
		doc.close_stream(pDesc);
		doc.close();
	}
	return bRet;
}

bool xCoreMesh::isLoaded()
{
	return m_nVertex > 0;
}

bool xCoreMesh::unload()
{
	m_nFace = 0 ;
	m_nVertex = 0 ;
	m_name = L"";
	m_nSubMesh = 0;
	m_nUVMaps = 0;
	m_SkeltonID.m_HiWord = 0;
	m_SkeltonID.m_LoWord = 0;
	m_bSkin = false;

	m_Materials.clear();
	size_t _nSubMesh = m_SubMeshs.size() ;
	for(size_t  i = 0 ; i <  _nSubMesh; i ++)
	{
		m_SubMeshs[i].m_pIdxBuffer->ReleaseObject();
	}
	m_SubMeshs.clear();
	XSAFE_RELEASEOBJECT(m_pInputAss);
	XSAFE_RELEASEOBJECT(m_pVertexStream);
    XSAFE_DELETE_ARRAY(m_pSkinVertexData);
    XSAFE_DELETE_ARRAY(m_pStaticVertexData);
	return true;
}

unsigned long xCoreMesh::memUsage()
{
	return 1;
}

IInputAssembler* xCoreMesh::CreateInputAssembler(int nUVChanel , IRenderApi* pRenderapi , bool bSkin)
{
	wchar_t _name[32] = {0};
	if(bSkin)
	{
	    swprintf(_name,L"SkinMeshIA_%0dUV" , nUVChanel);
		IInputAssembler* pAss = pRenderapi->getInputAssembler(_name);
		if(pAss != NULL) 
			return pAss;

		xInputLayoutDesc InputDesc;
		int BuffIdx = 0;
		InputDesc.addElement(SHADER_SEMANTIC_POSITION     , SHADERVARTYPE_FLOAT4 , 0 , BuffIdx);
		InputDesc.addElement(SHADER_SEMANTIC_NORMAL       , SHADERVARTYPE_FLOAT4 , 0 , BuffIdx);
		InputDesc.addElement(SHADER_SEMANTIC_COLOR        , SHADERVARTYPE_FLOAT4 , 0 , BuffIdx);
		InputDesc.addElement(SHADER_SEMANTIC_TANGENT      , SHADERVARTYPE_FLOAT4 , 0 , BuffIdx);
		InputDesc.addElement(SHADER_SEMANTIC_WEIGHT       , SHADERVARTYPE_FLOAT4 , 0 , BuffIdx);
		InputDesc.addElement(SHADER_SEMANTIC_BLENDINDICES , SHADERVARTYPE_INT4   , 0 , BuffIdx);		
		for(int j = 0 ; j < nUVChanel ; j ++)
		{
			InputDesc.addElement(SHADER_SEMANTIC_TEXCOORD , SHADERVARTYPE_FLOAT2 , j , BuffIdx);
		}
		InputDesc.addBufferDesc(RESOURCE_USAGE_DEFAULT , RESOURCE_ACCESS_NONE);
		return pRenderapi->createInputAssembler(_name, InputDesc);
	}
	else
	{
		swprintf(_name,L"StaticMeshIA_%0dUV" , nUVChanel);
		IInputAssembler* pAss = pRenderapi->getInputAssembler(_name);
		if(pAss != NULL) 
			return pAss;

		xInputLayoutDesc InputDesc;
		int BuffIdx = 0;
		InputDesc.addElement(SHADER_SEMANTIC_POSITION     , SHADERVARTYPE_FLOAT4 , 0 , BuffIdx);
		InputDesc.addElement(SHADER_SEMANTIC_NORMAL       , SHADERVARTYPE_FLOAT4 , 0 , BuffIdx);
		InputDesc.addElement(SHADER_SEMANTIC_COLOR        , SHADERVARTYPE_FLOAT4 , 0 , BuffIdx);
		InputDesc.addElement(SHADER_SEMANTIC_TANGENT      , SHADERVARTYPE_FLOAT4 , 0 , BuffIdx);
		for(int j = 0 ; j < nUVChanel ; j ++)
		{
			InputDesc.addElement(SHADER_SEMANTIC_TEXCOORD , SHADERVARTYPE_FLOAT2 , j , BuffIdx);
		}
		InputDesc.addBufferDesc(RESOURCE_USAGE_DEFAULT , RESOURCE_ACCESS_NONE);
		return pRenderapi->createInputAssembler(_name, InputDesc);
	}

}

bool xCoreMesh::loadMaterial(xcomdoc& doc , const wchar_t* _dir)
{
	ds_wstring matXMLName = ds_wstring(_dir) + L"material.xml";

	xcomdocstream* pMatStream = doc.open_stream(matXMLName.c_str() );
	xcdstream  _in(pMatStream);
	xXmlDocument xml;
	xml.load(_in);
	_in.close();
	doc.close_stream(pMatStream);

	xXmlNode* pRootNode = xml.root();
	if(pRootNode == NULL)
		return false;

	xXmlNode::XmlNodes nodes;
	pRootNode->findNode(L"material" , nodes);
	size_t nMatNodes = nodes.size() ;
	for(size_t i = 0 ; i < nMatNodes ; i ++)
	{
		xXmlNode* pMatNode = nodes[i];
		xMeshMaterial mat;
		xXmlNode::XmlNodes texNodes;
		pMatNode->findNode(L"texture" , texNodes);
		if(pMatNode->value(L"matscript") )
		{
			mat.m_Script = pMatNode->value(L"matscript") ;
		}

		xXmlNode* pAmbNode = pMatNode->findNode(L"Ambient");
		if(pAmbNode)
		{
			mat.m_Ambient.r=pAmbNode->float_value(L"r"); 
			mat.m_Ambient.g=pAmbNode->float_value(L"g"); 
			mat.m_Ambient.b=pAmbNode->float_value(L"b"); 
			mat.m_Ambient.a=pAmbNode->float_value(L"a"); 
		}
		xXmlNode* pDiffNode = pMatNode->findNode(L"Diffuse");
		if(pDiffNode)
		{
			mat.m_Diffuse.r=pDiffNode->float_value(L"r"); 
			mat.m_Diffuse.g=pDiffNode->float_value(L"g"); 
			mat.m_Diffuse.b=pDiffNode->float_value(L"b"); 
			mat.m_Diffuse.a=pDiffNode->float_value(L"a"); 
		}


		xXmlNode* pSpecNode = pMatNode->findNode(L"Speculer");
		if(pSpecNode)
		{
			mat.m_Speculer.r=pSpecNode->float_value(L"r"); 
			mat.m_Speculer.g=pSpecNode->float_value(L"g"); 
			mat.m_Speculer.b=pSpecNode->float_value(L"b"); 
			mat.m_Speculer.a=pSpecNode->float_value(L"a"); 
		}

		xXmlNode* pEmissNode = pMatNode->findNode(L"Emissive");
		if(pEmissNode)
		{
			mat.m_Emissive.r=pEmissNode->float_value(L"r"); 
			mat.m_Emissive.g=pEmissNode->float_value(L"g"); 
			mat.m_Emissive.b=pEmissNode->float_value(L"b"); 
			mat.m_Emissive.a=pEmissNode->float_value(L"a");
		}

		mat.m_fShiness = pMatNode->float_value(L"Shiness" );
		mat.m_fOpactiy = pMatNode->float_value(L"Opactiy" );
		size_t nTexNodes = texNodes.size() ;
		for(size_t iTex = 0; iTex < nTexNodes ; iTex ++)
		{
			xMeshTexture tex;
			tex.m_iChannel = texNodes[iTex]->int_value(L"chanel");
			tex.m_TexName  = texNodes[iTex]->value(L"image");
			tex.m_hTexture = m_pTexMgr->add(tex.m_TexName.c_str() , 0 , true);
			mat.m_vTextures.push_back(tex);
		}

		//如果读取出来的纹理个数为0.
		if(nTexNodes == 0)
		{
			xMeshTexture tex;
			tex.m_iChannel = 0;
			tex.m_TexName  = L"white.tga";
			tex.m_hTexture = m_pTexMgr->add(tex.m_TexName.c_str() , 0 , true);
			mat.m_vTextures.push_back(tex);
		}
		m_Materials.push_back(mat);
	}

	return true;
}

struct xModelWeightedVertex
{
	xvec3       m_InitPos;
	int         m_nEffBone; //影响到的骨头数目
	struct 
	{
		int    m_BoneIndex;
		int    m_BoneID;
		float  m_fWeight;
	}m_Weight[4];
};


bool xCoreMesh::load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg)
{
	//读入文件头
	ds_wstring descName = ds_wstring(_dir) + L"desc.xml";
	xcomdocstream* pdescstream = doc.open_stream(descName.c_str() );
	if(pdescstream == NULL)
		return false;
	xcdstream  _in(pdescstream);
	xXmlDocument xml;
	xml.load(_in);
	_in.close();
	doc.close_stream(pdescstream);
    m_aabb.Init(10000000000.0f);
	xXmlNode* pRootNode = xml.root();
	m_name     =pRootNode->value(L"name");
	m_nFace    =pRootNode->int_value(L"nFace");
	m_nVertex  =pRootNode->int_value(L"nVertex");
	m_nSubMesh =pRootNode->int_value(L"nSubMesh");
	m_nUVMaps  =pRootNode->int_value(L"nUVMaps");
    m_nFrame   = 1;
	if(pRootNode->value(L"nFrame") ) m_nFrame = pRootNode->int_value(L"nFrame");

	//纹理v坐标的属性。
	const wchar_t* pTexCoordStyle = pRootNode->value(L"TexCoordStyle");
	std::ds_wstring  strTexCoordStyle = L"OpenGL";
	if(pTexCoordStyle != NULL)
	{
		strTexCoordStyle = pTexCoordStyle;
	}
	ds_wstring rApiTexCoordStyle = m_pRenderApi->texCoordStyle();
	bool bInvertVCoord = rApiTexCoordStyle != strTexCoordStyle;

	
	//骨架ID
    xXmlNode* pSkelNode = pRootNode->findNode(L"skeleton");
	if(pSkelNode)
	{
		m_SkeltonID.m_HiWord = pSkelNode->int_value(L"HiID");
		m_SkeltonID.m_LoWord = pSkelNode->int_value(L"LoID");
	}
	else
	{
		m_SkeltonID.m_HiWord = 0;
		m_SkeltonID.m_LoWord = 0;
	}
    
	//读入SubMesh信息
	m_SubMeshs.resize(m_nSubMesh);
	xXmlNode::XmlNodes subMeshNodes;
	pRootNode->findNode(L"submesh", subMeshNodes);
	assert(subMeshNodes.size() == m_nSubMesh);
	for(int i = 0 ; i < m_nSubMesh ; i ++)
	{
		xSubMesh& subMesh      = m_SubMeshs[i];
		xXmlNode* pSubMeshNode = subMeshNodes[i];
		subMesh.m_iMatIdx = pSubMeshNode->int_value(L"nMatIndex");
		subMesh.m_nFace   = pSubMeshNode->int_value(L"nFace");
 	}
	//加载材质
  
	loadMaterial(doc , _dir);
	
	ds_wstring faceName     = ds_wstring(_dir) + L"face";
	xcomdocstream* pFaceStream     = doc.open_stream(faceName.c_str() );

	//读入所有的面
    for(int i = 0 ; i < m_nSubMesh ; i ++)
	{
		int nFace ;
		int iMatIdx;
		pFaceStream->read(nFace);
		pFaceStream->read(iMatIdx);
		xvec3ui* pData = new xvec3ui [nFace];
		pFaceStream->read(pData , nFace);

		xInputBufferDesc idxBufDesc;
		idxBufDesc.m_usage = RESOURCE_USAGE_DEFAULT;
		idxBufDesc.m_accessFlage = RESOURCE_ACCESS_NONE;
		idxBufDesc.m_bindtype = BIND_AS_INDEX_BUFFER;
		m_SubMeshs[i].m_pIdxBuffer = m_pRenderApi->createInputBuffer( nFace *3  , 4 , &idxBufDesc, pData);
		delete pData;		
	}
	doc.close_stream( pFaceStream);

	if(m_nFrame == 1)
		return readSingleFrame(doc , _dir , arg , bInvertVCoord);
	else
	{
		return readAnimationFrame(doc , _dir , arg , bInvertVCoord);
	}
	return true;
}

bool xCoreMesh::readSingleFrame(xcomdoc& doc , const wchar_t* _dir , unsigned int arg , bool bInvertVCoord)
{
	ds_wstring weightName   = ds_wstring(_dir) + L"weight";
	ds_wstring positionName = ds_wstring(_dir) + L"position";
	ds_wstring normalName   = ds_wstring(_dir) + L"normal";
	ds_wstring tangentName  = ds_wstring(_dir) + L"tangent";
	ds_wstring diffusenName = ds_wstring(_dir) + L"diffuse";
	ds_wstring faceName     = ds_wstring(_dir) + L"face";


	xcomdocstream* pWeightStream   = doc.open_stream(weightName.c_str() );
	xcomdocstream* pPositionStream = doc.open_stream(positionName.c_str() );
	xcomdocstream* pNormalStream   = doc.open_stream(normalName.c_str() );
	xcomdocstream* pTangentStream  = doc.open_stream(tangentName.c_str() );
	xcomdocstream* pDiffuseStream  = doc.open_stream(diffusenName.c_str() );
	xcomdocstream* pFaceStream     = doc.open_stream(faceName.c_str() );

	xcomdocstream** pUVStream = new xcomdocstream* [m_nUVMaps];
	for(int i = 0 ; i < m_nUVMaps ; i ++)
	{
		wchar_t uvName[9] = {0};
		swprintf(uvName , L"uv_%d" , i);
		ds_wstring faceName     = ds_wstring(_dir) + uvName;
		pUVStream[i] = doc.open_stream( faceName.c_str() );
		int nVertex = 0;
		pUVStream[i]->read(nVertex);
	}
    m_nFrame = 1;
	m_bSkin = pWeightStream != NULL;
    m_VertexBuffers.resize(m_nFrame);

    //如果不支持Gpu的Skin。那么就创建一个静态的SkinMesh
	m_pInputAss = CreateInputAssembler(m_nUVMaps , m_pRenderApi , m_bGpuSkin && m_bSkin);
	m_pVertexStream = m_pInputAss->createVertexStream();

	void* pInitData = NULL; 
	int nVertex = 0;
	pNormalStream->read(nVertex);
	pTangentStream->read(nVertex);
	pDiffuseStream->read(nVertex);

	xMeshMaterial* pMat = NULL;
	bool useMatColor = false;
	size_t nMat = m_Materials.size();
	//只要有一个材质的纹理层数为0，就用这个材质的颜色填充。
	for(size_t i = 0 ; i < nMat ; i ++)
	{
		if(m_Materials[i].m_vTextures.size() == 0) 
		{
			pMat = &m_Materials[i];
			useMatColor = true;
		}
	}

	//如果数据能一次性读入，将优化读取速度，此处因我偷懒没优化。

	//骨骼动画数据读取
	if(pWeightStream)
	{
		pWeightStream->read(nVertex);
		int  nSkinVertSize = sizeof(xSkinMeshVertex) + (m_nUVMaps - 1) * sizeof(xvec2);
		m_pSkinVertexData = new char[  nSkinVertSize * nVertex ];
		for(int i = 0 ; i < nVertex ; i ++)
		{
			xSkinMeshVertex* pVertex =  (xSkinMeshVertex*)(m_pSkinVertexData + i * nSkinVertSize);
			pVertex->m_weight[0] =  pVertex->m_weight[1] = pVertex->m_weight[2] = pVertex->m_weight[3] = 0.0f;
			pVertex->m_windex[0] =  pVertex->m_windex[1] = pVertex->m_windex[2] = pVertex->m_windex[3] = 0;

			xModelWeightedVertex wv ;
			xvec3       nrm;
			xColor_4ub  cl;
			xvec3       tan;
			pWeightStream->read(wv);
			pNormalStream->read(nrm);
			pTangentStream->read(tan);
			pDiffuseStream->read(cl);
			pVertex->m_Position = xvec4(wv.m_InitPos.x , wv.m_InitPos.y , wv.m_InitPos.z , 1.0f);
			m_aabb.AddPoint(wv.m_InitPos);
			for(int iBone = 0 ; iBone < wv.m_nEffBone ; iBone ++)
			{
				pVertex->m_weight[iBone] = wv.m_Weight[iBone].m_fWeight;
				pVertex->m_windex[iBone] = wv.m_Weight[iBone].m_BoneIndex ;
			}

			pVertex->m_Normal   = xvec4(nrm.x  , nrm.y , nrm.z  , 1.0f);
			pVertex->m_Tangent  = xvec4(tan.x  , tan.y , tan.z  , 1.0f);
			pVertex->m_Diffuse  = xvec4(cl.r/255.0f , cl.g /255.0f , cl.b/255.0f , cl.a /255.0f);

			//如果某个材质的
			if(useMatColor && pMat)
			{
				pVertex->m_Diffuse.x *= pMat->m_Diffuse.r ; 
				pVertex->m_Diffuse.y *= pMat->m_Diffuse.g ;
				pVertex->m_Diffuse.z *= pMat->m_Diffuse.b ;
				pVertex->m_Diffuse.w *= pMat->m_Diffuse.a ;

			}
			for(int iUV = 0 ; iUV < m_nUVMaps ; iUV ++)
			{
				xvec2 uv;
				pUVStream[iUV]->read(uv);
				if(bInvertVCoord)   { uv.y = 1.0f - uv.y; }//把纹理坐标逆过来
				pVertex->m_uv[iUV] = uv;
			}
		}


        if(m_bGpuSkin)
        {
             xInputBufferDesc InputBuffDesc;
             InputBuffDesc.m_usage = RESOURCE_USAGE_DEFAULT;
		     m_VertexBuffers[0] = m_pVertexStream->createInputBuffer(0 , nVertex , m_pSkinVertexData , NULL);
              XSAFE_DELETE_ARRAY(m_pSkinVertexData);
        }
        else
        {
            //如果不是GPU Skin则不填充数据
            PrepareNonGpuSkinData();
        }
	}
	else
	{
		pPositionStream->read(nVertex);
		int  vertSize = sizeof(xStaticMeshVertex) + (m_nUVMaps - 1) * sizeof(xvec2);
		char*  pData = new char[  (vertSize) * (nVertex )];
		for(int i = 0 ; i < nVertex ; i ++)
		{
			xStaticMeshVertex* pVertex =  (xStaticMeshVertex*)(pData + i * vertSize);
			xvec3       pos ;
			xvec3       nrm;
			xColor_4ub  cl;
			xvec3       tan;
			pPositionStream->read(pos);
			pNormalStream->read(nrm);
			pTangentStream->read(tan);
			pDiffuseStream->read(cl);
			m_aabb.AddPoint(pos);
			pVertex->m_Position = xvec4(pos.x  , pos.y , pos.z  , 1.0f);
			pVertex->m_Normal   = xvec4(nrm.x  , nrm.y , nrm.z  , 1.0f);
			pVertex->m_Tangent  = xvec4(tan.x  , tan.y , tan.z  , 1.0f);
			pVertex->m_Diffuse  = xvec4(cl.r/255.0f , cl.g /255.0f , cl.b/255.0f , cl.a /255.0f);
			for(int iUV = 0 ; iUV < m_nUVMaps ; iUV ++)
			{
				xvec2 uv;
				pUVStream[iUV]->read(uv);
				if(bInvertVCoord) { uv.y = 1.0f - uv.y; } //把纹理坐标逆过来
				pVertex->m_uv[iUV] = uv;
			}
		}
		m_VertexBuffers[0] = m_pVertexStream->createInputBuffer(0 , nVertex , pData , NULL);
		delete [] pData;

	}


	doc.close_stream( pWeightStream   );
	doc.close_stream( pPositionStream );
	doc.close_stream( pNormalStream   );
	doc.close_stream( pTangentStream  );
	doc.close_stream( pDiffuseStream  );
	for(int i = 0 ; i < m_nUVMaps ; i ++)
	{
		doc.close_stream(pUVStream[i]);
	}
	delete [] pUVStream;
	return true;
}

bool xCoreMesh::readAnimationFrame(xcomdoc& doc , const wchar_t* _dir , unsigned int arg , bool bInvertVCoord)
{
	ds_wstring positionName = ds_wstring(_dir) + L"position";
	ds_wstring normalName   = ds_wstring(_dir) + L"normal";
	ds_wstring tangentName  = ds_wstring(_dir) + L"tangent";
	ds_wstring diffusenName = ds_wstring(_dir) + L"diffuse";
	ds_wstring faceName     = ds_wstring(_dir) + L"face";

	xcomdocstream* pPositionStream = doc.open_stream(positionName.c_str() );
	xcomdocstream* pNormalStream   = doc.open_stream(normalName.c_str() );
	xcomdocstream* pTangentStream  = doc.open_stream(tangentName.c_str() );
	xcomdocstream* pDiffuseStream  = doc.open_stream(diffusenName.c_str() );
	xcomdocstream* pFaceStream     = doc.open_stream(faceName.c_str() );

	xcomdocstream** pUVStream = new xcomdocstream* [m_nUVMaps];
	for(int i = 0 ; i < m_nUVMaps ; i ++)
	{
		wchar_t uvName[9] = {0};
		swprintf(uvName , L"uv_%d" , i);
		ds_wstring faceName     = ds_wstring(_dir) + uvName;
		pUVStream[i] = doc.open_stream( faceName.c_str() );
		int nVertex = 0;
		pUVStream[i]->read(nVertex);
	}
	m_bSkin   = false;
	m_VertexBuffers.resize(m_nFrame);

	m_pInputAss = CreateInputAssembler(m_nUVMaps , m_pRenderApi , m_bSkin);
	m_pVertexStream = m_pInputAss->createVertexStream();

	void* pInitData = NULL; 
	int nVertex = 0;
	pNormalStream->read(nVertex);
	pTangentStream->read(nVertex);
	pDiffuseStream->read(nVertex);
	pPositionStream->read(nVertex);

	m_nVertex = nVertex;
	int  vertSize = sizeof(xStaticMeshVertex) + (m_nUVMaps - 1) * sizeof(xvec2);
    char*  pData = new char[  (vertSize) * (nVertex )];
	for(int iFrame = 0 ;iFrame < m_nFrame  ; iFrame ++)
	{
		for(int iVertex = 0 ; iVertex < nVertex ; iVertex ++)
		{
			xStaticMeshVertex* pVertex =  (xStaticMeshVertex*)(pData + iVertex * vertSize);
			xvec3       pos ;
			xvec3       nrm;
			xColor_4ub  cl;
			xvec3       tan;
			pPositionStream->read(pos);
			pNormalStream->read(nrm);
			pTangentStream->read(tan);
			pDiffuseStream->read(cl);
			pVertex->m_Position = xvec4(pos.x  , pos.y , pos.z  , 1.0f);
			pVertex->m_Normal   = xvec4(nrm.x  , nrm.y , nrm.z  , 1.0f);
			pVertex->m_Tangent  = xvec4(tan.x  , tan.y , tan.z  , 1.0f);
			pVertex->m_Diffuse  = xvec4(cl.r/255.0f , cl.g /255.0f , cl.b/255.0f , cl.a /255.0f);
			m_aabb.AddPoint(pos);
			for(int iUV = 0 ; iUV < m_nUVMaps ; iUV ++)
			{
				xvec2 uv;
				pUVStream[iUV]->read(uv);
				if(bInvertVCoord) { uv.y = 1.0f - uv.y; } //把纹理坐标逆过来
				pVertex->m_uv[iUV] = uv;
			}
		}
		m_VertexBuffers[iFrame] = m_pVertexStream->createInputBuffer(0 , nVertex , pData , NULL);
		

	}
	delete [] pData;

	doc.close_stream( pPositionStream );
	doc.close_stream( pNormalStream   );
	doc.close_stream( pTangentStream  );
	doc.close_stream( pDiffuseStream  );
	for(int i = 0 ; i < m_nUVMaps ; i ++)
	{
		doc.close_stream(pUVStream[i]);
	}
	delete [] pUVStream;
	return true;
}

bool xCoreMesh::draw(xCoreSkeleton* pSkeleton , xCoreActionFrame* pActionFrame)
{
    if(m_bGpuSkin == false) 
    {
        doSkin(pSkeleton , pActionFrame);
    }

	m_pRenderApi->setVertexStream( m_pVertexStream );
	for(int i = 0 ; i < m_nSubMesh ; i ++)
	{
        int matidx = m_SubMeshs[i].m_iMatIdx;
        if(matidx >= m_Materials.size() ) matidx = m_Materials.size() - 1;

		xMeshMaterial& mat = m_Materials[ matidx ];
		size_t nTex = mat.m_vTextures.size() ;
		for(size_t iTex = 0 ; iTex < nTex ; iTex ++)
		{
			IBaseTexture* pTexture = mat.m_vTextures[iTex].m_hTexture.getResource();
			if(pTexture == NULL)
			{
				XEVOL_LOG(eXL_DEBUG_HIGH , L"mesh texture not found %s\n" , mat.m_vTextures[iTex].m_TexName.c_str() );
				pTexture = m_pTexMgr->add(L"white.tga",0 , true).getResource();
			}
			m_pRenderApi->setTexture( (int)iTex ,  pTexture);
		}

		m_pRenderApi->draw(m_SubMeshs[i].m_pIdxBuffer , m_SubMeshs[i].m_nFace * 3 );
	}
	return true;	
}

bool xCoreMesh::doSkin(xCoreSkeleton* pSkeleton , xCoreActionFrame* pActionFrame)
{
    if(pSkeleton == NULL || pActionFrame == NULL)
        return false;

    xCoreActionFrame& actionFrame = *pActionFrame;
    int  nSkinVertSize   = sizeof(xSkinMeshVertex)   + (m_nUVMaps - 1) * sizeof(xvec2);
    int  nStaticVertSize = sizeof(xStaticMeshVertex) + (m_nUVMaps - 1) * sizeof(xvec2);
    for(int i = 0 ; i < m_nVertex ; ++i )
    {
        xStaticMeshVertex* pStaticVertex = (xStaticMeshVertex*)(m_pStaticVertexData + nStaticVertSize * i );
        xSkinMeshVertex*   pSkinedVertex = (xSkinMeshVertex*)  (m_pSkinVertexData   + nSkinVertSize   * i );


        xvec4&  v   = pStaticVertex->m_Position;
        xvec4&  nrm = pStaticVertex->m_Normal;
        xvec4&  tg  = pStaticVertex->m_Tangent;

        xvec4&  nrm_init  = pSkinedVertex->m_Normal;
        xvec4&  tg_init   = pSkinedVertex->m_Tangent;

        v   = xvec4(0.0f,0.0f,0.0f);
        nrm = xvec4(0.0f,0.0f,0.0f);
        tg  = xvec4(0.0f,0.0f,0.0f);
        xvec4& initPos = pSkinedVertex->m_Position;
        for(int iw = 0 ; iw < 4 ; ++iw)
        {
            xvec4   temp;
            int boneIndex = pSkinedVertex->m_windex[iw];
            float fWeight = pSkinedVertex->m_weight[iw];
            xmat4& m = actionFrame.operator[](boneIndex);

            //计算法向量
            m.transform_l_3x3(nrm_init,temp);
            temp *= fWeight;
            XM_Add(nrm,temp,nrm);
            //计算tangent
            m.transform_l_3x3(tg_init,temp);
            temp *= fWeight;
            XM_Add(tg,temp,tg);

            //计算顶点位置
            XM_Mul(initPos,m,temp);
            temp *= fWeight;
            XM_Add(v,temp,v);

        }
        //v = initPos;
        //nrm = nrm_init;
        nrm.normalize();
        tg.normalize();
        v.w = nrm.w = tg.w = 1.0f;
    }
    m_VertexBuffers[0]->update(eLock_WriteDiscard , m_pStaticVertexData , m_nVertex * nStaticVertSize);
    return true;
}

void xCoreMesh::PrepareNonGpuSkinData() 
{
    xInputBufferDesc InputBuffDesc;
    InputBuffDesc.m_usage = RESOURCE_USAGE_DYNAMIC;
    m_VertexBuffers[0] = m_pVertexStream->createInputBuffer(0 , m_nVertex , NULL , NULL);

    int  nSkinVertSize   = sizeof(xSkinMeshVertex)   + (m_nUVMaps - 1) * sizeof(xvec2);
    int  nStaticVertSize = sizeof(xStaticMeshVertex) + (m_nUVMaps - 1) * sizeof(xvec2);
    m_pStaticVertexData = new char[  (nStaticVertSize) * (m_nVertex )];
    for(int i = 0 ; i < m_nVertex ; ++i )
    {
        xStaticMeshVertex* pStaticVertex = (xStaticMeshVertex*)(m_pStaticVertexData + nStaticVertSize * i );
        xSkinMeshVertex*   pSkinedVertex = (xSkinMeshVertex*)  (m_pSkinVertexData   + nSkinVertSize   * i );
        for(int iUV = 0 ; iUV < m_nUVMaps ; iUV ++)
        {
            pStaticVertex->m_uv[iUV] = pSkinedVertex->m_uv[iUV];
        }
    }
}

bool xCoreMesh::setCurrentFrame(int iFrame)
{
	//只对帧动画有用
    if(iFrame >= (int)m_VertexBuffers.size() )
	{
		iFrame = (int)m_VertexBuffers.size() - 1;
	}
	m_pVertexStream->setInputBuffer(0,m_VertexBuffers[iFrame]);
	return true;
}

END_NAMESPACE_XEVOL3D
