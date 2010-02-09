/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "stdafx.h"
#include "MaxMeshBuilder.h"
#include "SkeletonExp.h"
//#include "wm3.h"
void    GetBonesFromNodes( sMaxNodes_t& out, sMaxNodes_t& in)
{
	size_t nBone = in.size(); 
	out.clear();
	out.reserve(nBone);
	for(size_t i = 0;  i < nBone ; ++i)
	{
		INode* pNode = in[i];
		if(  G_MaxEnv().IsBone(pNode)  )
		{
			out.push_back(pNode);
		}

	}
}
void    GetMeshNodesFromNodes( sMaxNodes_t& out, sMaxNodes_t& in)
{
	size_t nMesh = in.size(); 
	out.clear();
	out.reserve(nMesh);
	for(size_t i = 0;  i < nMesh ; ++i)
	{
		INode* pNode = in[i];
		CMaxMesh mesh;
		if(mesh.Init(pNode , NULL) == false)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]不是一个Mesh，取消导出\n" , INodeName(pNode).c_str() );
			continue;
		}

		if(mesh.IsExportable() == false)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]不是一个Mesh，取消导出", INodeName(pNode).c_str() );
			continue;
		}
		mesh.Free();
		out.push_back(pNode);         
	}
}

void    ExtracAllNodes(INode* pNode, sMaxNodes_t& out)
{
	out.push_back(pNode);
    for(int i = 0 ; i  < pNode->NumberOfChildren() ; i++)
	{		
		ExtracAllNodes(pNode->GetChildNode(i) , out);
	}
}


void    GetNodesFromSelectedNode( sMaxNodes_t& out )
{
	for(int i = 0 ; i < G_MaxEnv().m_pInterace->GetSelNodeCount() ; ++i)
	   out.push_back(G_MaxEnv().m_pInterace->GetSelNode(i) );
}

void    GetNodesFromScene( sMaxNodes_t& out )
{
	 ExtracAllNodes(G_MaxEnv().m_pInterace->GetRootNode(),out);
}

void    GetMeshNodesFromScene( sMaxNodes_t& out )
{
	sMaxNodes_t nodes;
	GetNodesFromScene(nodes);
	GetMeshNodesFromNodes(out,nodes);
	nodes.clear();
}

void    BuildSkinMeshsFromNodes(MaxMeshs& out, sMaxNodes_t& in,CSkeletonExporter* pSkeleton)
{
    size_t nMesh = in.size(); 
    out.clear();
    out.reserve(nMesh);
	int iMesh = 0;
	for(size_t i = 0;  i < nMesh ; ++i)
	{
		INode* pNode = in[i];
		out.resize( iMesh + 1);
		CMaxMesh& mesh = out[iMesh];
        if(mesh.Init(pNode , pSkeleton) == false)
        {
           XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]不是一个Mesh，取消导出\n" , INodeName(pNode).c_str() );
            continue;
        }

		if(mesh.IsExportable() == false)
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]不是一个Mesh，取消导出\n" , INodeName(pNode).c_str() );
            continue;
        }
		if(pSkeleton)
		{
			mesh.InitSkeleton(pSkeleton);
		}
		iMesh ++ ;  
                
    }
}
void    BuildMeshsFromNodes(MaxMeshs& out, sMaxNodes_t& in)
{
    size_t nMesh = in.size(); 
    out.clear();
    out.reserve(nMesh);
	int iMesh = 0;
    for(size_t i = 0;  i < nMesh ; ++i)
    {
        INode* pNode = in[i];
		out.resize( iMesh + 1);
        CMaxMesh& mesh = out[iMesh];
        if(mesh.Init(pNode , NULL) == false)
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]不是一个Mesh，取消导出\n" , INodeName(pNode).c_str() );
            continue;
        }
        if(mesh.IsExportable() == false)
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]不是一个Mesh，取消导出\n" , INodeName(pNode).c_str() );
            continue;
        }
        iMesh ++ ;        
    }
}

void    ReleaseMaxMeshs(MaxMeshs& meshs)
{
    size_t nMesh = meshs.size();
    for(size_t i  = 0 ; i < nMesh ; ++i)
    {
        meshs[i].Free();
    }
    meshs.clear();
}



