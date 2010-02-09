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
#include "MaxNodeTree.h"


bool CMaxNodeTree::IsNodeInSet(vector<INode*>& Nodes ,INode* pNode)
{

	for(size_t i = 0 ; i < Nodes.size() ; i ++)
	{
		if( pNode == Nodes[i])
			return true;
	}
	return false;
}

bool CMaxNodeTree::IsChildNodeOfSet(vector<INode*>& Nodes ,INode* pNode)
{
	for(size_t i = 0 ; i < Nodes.size() ; i ++)
	{
		for(size_t j = 0 ; j< (size_t)Nodes[i]->NumberOfChildren(); j ++)
		{
			if( pNode == Nodes[i]->GetChildNode((int)j))
				return true;
		}
	}
	return false;
}



void CMaxNodeTreeItem::InsertAllChildNode(vector<INode*>& Nodes)
{
	for(int i = 0 ; i < m_pMaxNode->NumberOfChildren(); i++ )
	{
		INode* pNode = m_pMaxNode->GetChildNode(i);
		//不是在选择的列表里。
		if(CMaxNodeTree::IsNodeInSet(Nodes,pNode) == false)
			continue;

		size_t nSize = this->m_ChildNodes.size();
		CMaxNodeTreeItem TreeNode(this);
		m_ChildNodes.push_back(TreeNode);
		m_ChildNodes[nSize].m_pMaxNode = pNode;
		m_ChildNodes[nSize].InsertAllChildNode(Nodes);
	}
	return ;
}

void CMaxNodeTreeItem::InsertAllChildNode()
{
  	 for(int i = 0 ; i < m_pMaxNode->NumberOfChildren(); i++ )
	 {
		   INode* pNode = m_pMaxNode->GetChildNode(i);
		   size_t nSize = this->m_ChildNodes.size();
		   CMaxNodeTreeItem TreeNode(this);
		   m_ChildNodes.push_back(TreeNode);
		   m_ChildNodes[nSize].m_pMaxNode = pNode;
           m_ChildNodes[nSize].InsertAllChildNode();
	 }
	 return ;
}

void CMaxNodeTreeItem::Clear()
{
	for(size_t i = 0 ; i < m_ChildNodes.size() ; i++)
	{
		m_ChildNodes[i].Clear();
	}
	m_ChildNodes.clear();
}



void CMaxNodeTree::InsertRootNode(INode* pNode)
{
   
}
void CMaxNodeTree::BuildScene(sMaxNodes_t nodes)
{
    int n = (int)nodes.size();
    bool * IsChildNodes = new bool[n];
    vector<INode*> Nodes;
    for(int i = 0 ; i < n ; i ++)
    {
        IsChildNodes[i] = false;
        INode* pNode = nodes[i];
        Nodes.push_back(pNode);
    }
    //------------------------------------------------------------------
    for(int i = 0 ; i < n ; i ++)
    {
        INode* pNode = nodes[i];
        if( IsChildNodeOfSet(Nodes,pNode)  == true)
        {
            IsChildNodes[i] = true; 
        }
    }

    for(int i = 0 ; i < n ; i ++)
    {
        if(IsChildNodes[i] == true)
            continue;

        INode* pNode =  nodes[i];
        CMaxNodeTreeItem TreeNode(NULL);
        size_t nSize =  m_MaxNodeTreeRoot.size();
        m_MaxNodeTreeRoot.push_back(TreeNode);
        m_MaxNodeTreeRoot[nSize].m_pMaxNode = pNode;
        m_MaxNodeTreeRoot[nSize].InsertAllChildNode(Nodes);
    }

    delete [] IsChildNodes;
}

void CMaxNodeTree::BuildSelectTree()
{
	int n = G_MaxEnv().m_pInterace->GetSelNodeCount();
	bool * IsChildNodes = new bool[n];
    vector<INode*> Nodes;
	for(int i = 0 ; i < n ; i ++)
	{
		IsChildNodes[i] = false;
		char* pName = G_MaxEnv().m_pInterace->GetSelNode(i)->GetName();
		Nodes.push_back(G_MaxEnv().m_pInterace->GetSelNode(i));
	}
	//------------------------------------------------------------------
	for(int i = 0 ; i < n ; i ++)
	{
		INode* pNode =  G_MaxEnv().m_pInterace->GetSelNode(i);
	    if( IsChildNodeOfSet(Nodes,pNode)  == true)
		{
            IsChildNodes[i] = true; 
		}
	}

    for(int i = 0 ; i < n ; i ++)
	{
		if(IsChildNodes[i] == true)
			continue;

		INode* pNode =  G_MaxEnv().m_pInterace->GetSelNode(i);
		CMaxNodeTreeItem TreeNode(NULL);
		size_t nSize =  m_MaxNodeTreeRoot.size();
		m_MaxNodeTreeRoot.push_back(TreeNode);
		m_MaxNodeTreeRoot[nSize].m_pMaxNode = pNode;
	    m_MaxNodeTreeRoot[nSize].InsertAllChildNode(Nodes);
	}

	delete [] IsChildNodes;
}

void CMaxNodeTree::BuildRootScene()
{
	INode* pNode =  G_MaxEnv().m_pInterace->GetRootNode();
	CMaxNodeTreeItem TreeNode(NULL);
	size_t nSize =  m_MaxNodeTreeRoot.size();
	m_MaxNodeTreeRoot.push_back(TreeNode);
	m_MaxNodeTreeRoot[nSize].m_pMaxNode = pNode;
	m_MaxNodeTreeRoot[nSize].InsertAllChildNode();  
}

void CMaxNodeTree::Clear()
{
   for(size_t i = 0 ; i< m_MaxNodeTreeRoot.size() ; i ++)
   {
	   m_MaxNodeTreeRoot[i].Clear();
   }
   m_MaxNodeTreeRoot.clear();
   m_FinalSelNodes.clear();
}
void CMaxNodeTree::extractNodes(sMaxNodes_t& NodeList,CMaxNodeTreeItem& Items)
{
    if(Items.m_isSeleted != false)
    {
        NodeList.push_back(Items.m_pMaxNode);
    }
    for(size_t i = 0 ; i < Items.m_ChildNodes.size(); ++i)
    {
        extractNodes(NodeList,Items.m_ChildNodes[i]);
    }
}

void CMaxNodeTree::confirmSel(CMaxNodeTreeItem& Items)
{
    if(m_pNodeTreeCtrl->GetCheck(Items.m_TreeItem))
    {
        Items.m_isSeleted = true;
    }
    else
    {
        Items.m_isSeleted = false;
    }

    for(size_t i = 0 ; i < Items.m_ChildNodes.size(); ++i)
    {
        confirmSel(Items.m_ChildNodes[i]);
    } 
}
void CMaxNodeTree::confirmSel()
{
    for(size_t i = 0 ; i < m_MaxNodeTreeRoot.size() ; ++i)
    {
        confirmSel(m_MaxNodeTreeRoot[i]);
    }
}
sMaxNodes_t& CMaxNodeTree::buildFinalSelNodes()
{
    m_FinalSelNodes.clear();
    for(size_t i = 0 ; i < m_MaxNodeTreeRoot.size() ; ++i)
    {
        extractNodes(m_FinalSelNodes,m_MaxNodeTreeRoot[i]);
    }
    return m_FinalSelNodes;
}
void CMaxNodeTree::buildFinalSelNodes(sMaxNodes_t& Nodes)
{
    Nodes.clear();
    for(size_t i = 0 ; i < m_MaxNodeTreeRoot.size() ; ++i)
    {
        extractNodes(Nodes,m_MaxNodeTreeRoot[i]);
    }
}

