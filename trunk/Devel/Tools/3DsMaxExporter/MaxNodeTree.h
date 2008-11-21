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

#ifndef __MAX_NODE_TREE_H__
#define __MAX_NODE_TREE_H__

#include "stdafx.h"
#include "MaxEnv.h"
#include <vector>
using namespace std;
class CMaxNodeTreeItem
{
public:
    typedef vector<CMaxNodeTreeItem> MaxNodeTreeItems;
    MaxNodeTreeItems                 m_ChildNodes;
    INode*                           m_pMaxNode;
    HTREEITEM                        m_TreeItem;
    CMaxNodeTreeItem*                m_pParentItem;
    bool                             m_isSeleted;
public:
    CMaxNodeTreeItem(CMaxNodeTreeItem* pParent)
    {
        m_isSeleted = true;
        m_pParentItem =pParent;
    }
    void InsertAllChildNode(vector<INode*>& Nodes);
    void InsertAllChildNode();
    void Clear();
};

class CMaxNodeTree
{
public:
    typedef vector<CMaxNodeTreeItem> MaxNodeTreeItems;
    MaxNodeTreeItems                 m_MaxNodeTreeRoot;
    sMaxNodes_t                      m_FinalSelNodes;
    CTreeCtrl*                       m_pNodeTreeCtrl;
public:
    CMaxNodeTree(CTreeCtrl* pCtrl){m_pNodeTreeCtrl = pCtrl;}
    void InsertRootNode(INode* pNode);
    void BuildSelectTree();
    void BuildRootScene();
    void BuildScene(sMaxNodes_t nodes);
    void Clear();

    sMaxNodes_t& buildFinalSelNodes();
    void         buildFinalSelNodes(sMaxNodes_t& Nodes);

    void confirmSel();

public:
    static bool IsNodeInSet(vector<INode*>& Nodes ,INode* pNode);
    static bool IsChildNodeOfSet(vector<INode*>& Nodes ,INode* pNode);

private:
    void extractNodes(sMaxNodes_t& NodeList,CMaxNodeTreeItem& Items);
    void confirmSel(CMaxNodeTreeItem& Items);

};

#endif

