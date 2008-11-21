#include "xCoreMesh.h"
#include "../xcomdoc/xdocfstream.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xCoreMesh  , IBaseResource);


xCoreMesh::xCoreMesh(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr , bool bSusMemCopy)
{
	m_pRenderApi = pRenderApi;
	m_pTexMgr    = pTexMgr;
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
	for(size_t  i = 0 ; i < m_SubMeshs.size() ; i ++)
	{
		m_SubMeshs[i].m_pIdxBuffer->ReleaseObject();
	}
	m_SubMeshs.clear();
	XSAFE_RELEASEOBJECT(m_pInputAss);
	XSAFE_RELEASEOBJECT(m_pVertexStream);
	return true;
}

unsigned long xCoreMesh::memUsage()
{
	return 1;
}

static IInputAssembler* CreateInputAssembler(int nUVChanel , IRenderApi* pRenderapi , bool bSkin)
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
	xCfgDocument xml;
	xml.load(_in);
	_in.close();
	doc.close_stream(pMatStream);

	xCfgNode* pRootNode = xml.root();
	if(pRootNode == NULL)
		return false;

	xCfgNode::CfgNodes nodes;
	pRootNode->findNode(L"material" , nodes);
	for(size_t i = 0 ; i < nodes.size() ; i ++)
	{
		xCfgNode* pMatNode = nodes[i];
		xMeshMaterial mat;
		xCfgNode::CfgNodes texNodes;
		pMatNode->findNode(L"texture" , texNodes);
		for(size_t iTex = 0; iTex < texNodes.size() ; iTex ++)
		{
			xMeshTexture tex;
			tex.m_iChannel = texNodes[iTex]->int_value(L"chanel");
			tex.m_TexName  = texNodes[iTex]->value(L"image");
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
	xCfgDocument xml;
	xml.load(_in);
	_in.close();
	doc.close_stream(pdescstream);

	xCfgNode* pRootNode = xml.root();
	m_name     =pRootNode->value(L"name");
	m_nFace    =pRootNode->int_value(L"nFace");
	m_nVertex  =pRootNode->int_value(L"nVertex");
	m_nSubMesh =pRootNode->int_value(L"nSubMesh");
	m_nUVMaps  =pRootNode->int_value(L"nUVMaps");
	
	//骨架ID
    xCfgNode* pSkelNode = pRootNode->findNode(L"skeleton");
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
	xCfgNode::CfgNodes subMeshNodes;
	pRootNode->findNode(L"submesh", subMeshNodes);
	assert(subMeshNodes.size() == m_nSubMesh);
	for(int i = 0 ; i < m_nSubMesh ; i ++)
	{
		xSubMesh& subMesh      = m_SubMeshs[i];
		xCfgNode* pSubMeshNode = subMeshNodes[i];
		subMesh.m_iMatIdx = pSubMeshNode->int_value(L"nMatIndex");
		subMesh.m_nFace   = pSubMeshNode->int_value(L"nFace");
 	}
	//加载材质
  
	loadMaterial(doc , _dir);
	
	
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

    m_bSkin = pWeightStream != NULL;


	m_pInputAss = CreateInputAssembler(m_nUVMaps , m_pRenderApi , m_bSkin);
    m_pVertexStream = m_pInputAss->createVertexStream();
	void* pInitData = NULL; 
	int nVertex = 0;
	pNormalStream->read(nVertex);
	pTangentStream->read(nVertex);
	pDiffuseStream->read(nVertex);

	//骨骼动画数据读取
	if(pWeightStream)
	{
       pWeightStream->read(nVertex);
	  
	   int  vertSize = sizeof(xSkinMeshVertex) + (m_nUVMaps - 1) * sizeof(xvec2);
	   char*  pData = new char[  vertSize * nVertex ];
	   for(int i = 0 ; i < nVertex ; i ++)
	   {
		   xSkinMeshVertex* pVertex =  (xSkinMeshVertex*)(pData + i * vertSize);
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
		   for(int iBone = 0 ; iBone < wv.m_nEffBone ; iBone ++)
		   {
			   pVertex->m_weight[iBone] = wv.m_Weight[iBone].m_fWeight;
			   pVertex->m_windex[iBone] = wv.m_Weight[iBone].m_BoneIndex ;
		   }

		   pVertex->m_Normal   = xvec4(nrm.x  , nrm.y , nrm.z  , 1.0f);
		   pVertex->m_Tangent  = xvec4(tan.x  , tan.y , tan.z  , 1.0f);
		   pVertex->m_Diffuse  = xvec4(cl.r/255.0f , cl.g /255.0f , cl.b/255.0f , cl.a /255.0f);
		   for(int iUV = 0 ; iUV < m_nUVMaps ; iUV ++)
		   {
			   xvec2 uv;
			   pUVStream[iUV]->read(uv);
			   pVertex->m_uv[iUV] = uv;
		   }
	   }
	   m_pVertexBuffer = m_pVertexStream->createInputBuffer(0 , nVertex , pData , NULL);
	   delete [] pData;

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
			 pVertex->m_Position = xvec4(pos.x  , pos.y , pos.z  , 1.0f);
			 pVertex->m_Normal   = xvec4(nrm.x  , nrm.y , nrm.z  , 1.0f);
			 pVertex->m_Tangent  = xvec4(tan.x  , tan.y , tan.z  , 1.0f);
			 pVertex->m_Diffuse  = xvec4(cl.r/255.0f , cl.g /255.0f , cl.b/255.0f , cl.a /255.0f);
			 for(int iUV = 0 ; iUV < m_nUVMaps ; iUV ++)
			 {
				 xvec2 uv;
				 pUVStream[iUV]->read(uv);
				 pVertex->m_uv[iUV] = uv;
			 }
		 }
		 m_pVertexBuffer = m_pVertexStream->createInputBuffer(0 , nVertex , pData , NULL);
		 delete [] pData;

	}


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

	doc.close_stream(pWeightStream    );
	doc.close_stream( pPositionStream );
	doc.close_stream( pNormalStream   );
	doc.close_stream( pTangentStream  );
	doc.close_stream( pDiffuseStream  );
	doc.close_stream( pFaceStream     );
	for(int i = 0 ; i < m_nUVMaps ; i ++)
	{
		doc.close_stream(pUVStream[i]);
	}

	delete [] pUVStream;
	return true;
}



bool xCoreMesh::draw()
{
	m_pRenderApi->setVertexStream( m_pVertexStream );
	for(int i = 0 ; i < m_nSubMesh ; i ++)
	{
		xMeshMaterial& mat = m_Materials[m_SubMeshs[i].m_iMatIdx ];
		for(size_t iTex = 0 ; iTex < mat.m_vTextures.size() ; iTex ++)
		{
			m_pRenderApi->setTexture( (int)iTex , mat.m_vTextures[iTex].m_hTexture.getResource() );
		}

		m_pRenderApi->draw(m_SubMeshs[i].m_pIdxBuffer , m_SubMeshs[i].m_nFace * 3 );
	}
	return true;	
}
END_NAMESPACE_XEVOL3D
