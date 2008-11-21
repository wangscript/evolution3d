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
#include "MaxEnv.h"
#include "DataDefine.h"
#include "xcomdoc/xcomdoc.h"
#include "BaseLib/xStringHash.h"
#include "BaseLib/xI18N.h"
using namespace XEvol3DModel;
using namespace XEvol3D;
CSetBoneGroupDlg CMaxEnv::m_BoneGroupDlg;
CSetSkinGroupDlg CMaxEnv::m_SkinGroupDlg;
namespace XEvol3D
{
	void XEVOL_LOG(int logLevel , const char * fmt , ...)
	{

	}
}

string  ToAnsi(const wstring& pStr)
{
	char aStr[1024]={0};
	XEvol_UnicodeToLocale(pStr.c_str() , aStr , 1024);
	return aStr;
}

wstring INodeName(INode* pNode)
{
	wchar_t nName[512] = {0};
	XEvol_LocaleToUnicode(pNode->GetName() , nName , 512);
	return nName;
}

const char* INodeNameA(INode* pNode)
{
	return pNode->GetName();
}

wstring ToWCHAR(const char* pStr)
{
	wchar_t nName[8192] = {0};
	XEvol_LocaleToUnicode(pStr , nName , 8192);
	return nName;
}

wstring ToWCHAR(const string& pStr)
{
	wchar_t nName[8192] = {0};
	XEvol_LocaleToUnicode(pStr.c_str() , nName , 8192);
	return nName;
}

void    WGetWndText(CWnd* pWnd , wchar_t* out , int nTex)
{
      GetWindowTextW(pWnd->GetSafeHwnd() , out, nTex);
}

wstring tex_usage_string(int usage)
{
    switch(usage)
    {
    case tex_usage_none:
        return L"Unused";
    case tex_usage_ambient:
        return L"ambient";
    case tex_usage_diffuse:
        return L"diffuse";
    case tex_usage_specular:
        return L"specular";
    case tex_usage_shininesNs:
        return L"shininesNs";
    case tex_usage_shininess:
        return L"shininess strength";
    case tex_usage_self_illu:
        return L"self-illumination";
    case tex_usage_opacity:
        return L"opacity";
    case tex_usage_filter_color:
        return L"filter color";
    case tex_usage_bump:
        return L"bump";
    case tex_usage_reflection:
        return L"reflection";
    case tex_usage_refraction:
        return L"refraction";
    case tex_usage_displacement:
        return L"displacement"; 
    default :
        return L"Unused";
    }
}

void CMaxEnv::start(Interface* pInterface , HWND hPannel)
{
	m_pIGameScene = GetIGameInterface();
	m_pIGameScene->InitialiseIGame();
	m_pInterace   = pInterface;
	m_hPannel     = hPannel;
}

void CMaxEnv::shutdown()
{
    m_pIGameScene->ReleaseIGame();
}
Modifier *CMaxEnv::FindSkinModifier(INode *pINode)
{
#if MAX_RELEASE >= 4000
    // get the object reference of the node
    Object *pObject;
    pObject = pINode->GetObjectRef();
    if(pObject == 0) return 0;

    // loop through all derived objects
    while(pObject->SuperClassID() == GEN_DERIVOB_CLASS_ID)
    {
        IDerivedObject *pDerivedObject;
        pDerivedObject = static_cast<IDerivedObject *>(pObject);

        // loop through all modifiers
        int stackId;
        for(stackId = 0; stackId < pDerivedObject->NumModifiers(); stackId++)
        {
            // get the modifier
            Modifier *pModifier;
            pModifier = pDerivedObject->GetModifier(stackId);

            // check if we found the skin modifier
            if(pModifier->ClassID() == SKIN_CLASSID) return pModifier;
        }

        // continue with next derived object
        pObject = pDerivedObject->GetObjRef();
    }
#endif

    return 0;
}

bool CMaxEnv::IsXEvol3DBone(INode* pNode)
{
    BOOL bIsBone; 
	if(pNode->UserPropExists("IsBone") == FALSE)
		return false;
	pNode->GetUserPropBool("IsBone",bIsBone);
	return  bIsBone != FALSE;
}

void CMaxEnv::SetXEvol3DBone(INode* pNode , BOOL bIsBone)
{
    pNode->SetUserPropBool("IsBone",bIsBone);
}

void CMaxEnv::FindNodeByBoneID(int id , INode* pRootNode,vector<INode*>& NodesFined)
{
     for(int i = 0 ; i < pRootNode->NumberOfChildren() ; ++i )
     {
         INode* pNode = pRootNode->GetChildNode(i);
         int nodeId;
         if(pNode->GetUserPropInt("BoneID",nodeId) == FALSE)
         {
             XEVOL_LOG(eXL_DEBUG_HIGH , L" 场景中的有节点没有BoneID属性\n");
         }
         else
         {
             if(nodeId == id)
             {
                 int nNode = (int)NodesFined.size();
                 bool bFined = false;
                 for(int i = 0 ; i < nNode ; i++)
                 {
                     if(NodesFined[i] == pNode)
                     {
                         bFined = true;
                         break;
                     }
                 }
                 if(bFined == false)
                    NodesFined.push_back(pNode);
             }
         }
         FindNodeByBoneID(id,pNode,NodesFined);
     }
}

INode* CMaxEnv::FindNodeByBoneID(int id)
{
    vector<INode*> NodesFined;
    INode* pRootNode = singleton().m_pInterace->GetRootNode();
    assert(pRootNode);
    FindNodeByBoneID(id,pRootNode,NodesFined);
    if(NodesFined.size() == 1)
        return NodesFined[0];
    if(NodesFined.size() > 1)
    {
        MessageBoxA(GetActiveWindow(),"有相同的BoneID","错误",MB_OK);
        assert(0);
    }
    return NULL;
}

bool CMaxEnv::__init_node_id(INode* pNode,map<int,int>& useID)
{
    int ID;
    bool ret = true;
    if(pNode->GetUserPropInt("BoneID",ID) == FALSE)
    {
        wstringstream stream;
        stream<<INodeName(pNode);
        //stream<<"_"<<<<pNode->GetHandle();
        ID = xStringHash(stream.str().c_str());
        pNode->SetUserPropInt("BoneID",ID);
        
    }

    if(useID.find(ID) != useID.end())
    {
        wstringstream stream;
        stream<<INodeName(pNode);//<<"_";
        //stream<<pNode->GetHandle();
		wstring message = wstring(L"分配BoneID的时候出错: BoneID Hashstring : ")  + stream.str();
        MessageBoxW(GetActiveWindow(), message.c_str(),L"错误",MB_OK);
        return false;
    }

    useID.insert(map<int,int>::value_type(ID,ID));

    int nChildNode = pNode->NumberOfChildren();

    for(int i = 0 ;i < nChildNode ; ++i)
    {
         bool thisret = __init_node_id(pNode->GetChildNode(i),useID);
         ret = ret && thisret;
    }
    return ret;

}

bool CMaxEnv::Init_scene_bone_id()
{
    map<int,int> UsedID;
    return __init_node_id(singleton().m_pInterace->GetRootNode(),UsedID);
}

bool CMaxEnv::__confirm_node_id(INode* pNode)
{
    int ID;
    if(pNode->GetUserPropInt("BoneID",ID) == FALSE)
    {
        MessageBoxW(GetActiveWindow(), (wstring(L"没有分配BoneID BoneNode : ") +  INodeName(pNode) ).c_str(),L"错误",MB_OK);
        return false;
    }
    int nChildNode = pNode->NumberOfChildren();
    for(int i = 0 ;i < nChildNode ; ++i)
    {
        if( false == __confirm_node_id(pNode->GetChildNode(i)) )
            return false;
    }
    return true;

}

bool CMaxEnv::Confirm_Scene_BoneID()
{
   return __confirm_node_id(singleton().m_pInterace->GetRootNode());
}



bool CMaxEnv::IsBone( INode* pNode )
{

	if(IsXEvol3DBone(pNode))
	{
		return true;
	}

    ObjectState os = pNode->EvalWorldState(0); 
    if( os.obj )
    {
        if( os.obj->ClassID() == BONE_OBJ_CLASSID )
        {
            return true;
        }
        else 
        {
            Control *pControl;
            pControl = pNode->GetTMController();
            if((pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) || (pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID)) 
            {
                return true;
            }
            else
            {
                // 用户自己定义的骨骼
                wstring nodeName = INodeName(pNode);
                if( wstring::npos != nodeName.find(L"bone_") ||
                    wstring::npos != nodeName.find(L"Bone_") ||   
                    wstring::npos != nodeName.find(L"BONE_")    )
                         return true;
            }
        }
    }
    return false;
}

bool CMaxEnv::IsPureDummy(INode* node)
{
    char* p = node->GetName();
    const ObjectState& os = node->EvalWorldState(0); 
    if( os.obj )
    {
        if( os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0) )
        {
            return true;
        }
        return false;
    }
    return true;
}

Modifier* CMaxEnv::FindPhysiqueModifier(INode* nodePtr)
{
    // Get object from node. Abort if no object.
    Object* ObjectPtr = nodePtr->GetObjectRef();

    if (!ObjectPtr) 
        return NULL;

    // Is derived object ?
    while (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID && ObjectPtr)
    {
        // Yes -> Cast.
        IDerivedObject *DerivedObjectPtr = (IDerivedObject *)(ObjectPtr);

        // Iterate over all entries of the modifier stack.
        int ModStackIndex = 0;
        while (ModStackIndex < DerivedObjectPtr->NumModifiers())
        {
            // Get current modifier.
            Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);

            // Is this Physique ?
            if (ModifierPtr->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
            {
                // Yes -> Exit.
                return ModifierPtr;
            }

            // Next modifier stack entry.
            ModStackIndex++;
        }
        ObjectPtr = DerivedObjectPtr->GetObjRef();
    }

    // Not found.
    return NULL;
}

bool CMaxEnv::IsBoneDummy(INode* node)
{
    char* p = node->GetName();
    const ObjectState& os = node->EvalWorldState(0); 
    if( os.obj )
    {
        if( os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0) )
        {
            //	return true;
            Control *pControl;
            pControl = node->GetTMController();
            if((pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) || (pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID)) 
                return true;
        }
        return false;
    }
    return true;
}

bool CMaxEnv::IsMesh(INode* pNode)
{
    ULONG superClassID;

    Object* pObj;

    assert(pNode);
    pObj = pNode->GetObjectRef();

    if (pObj == NULL)
    {
        return false;
    }

    superClassID = pObj->SuperClassID();
    //find out if mesh is renderable
    if( !pNode->Renderable() || pNode->IsNodeHidden())
    {
        return false;
    }
    bool bFoundGeomObject = false;
    //find out if mesh is renderable (more)
    switch(superClassID)
    {
    case GEN_DERIVOB_CLASS_ID:
        {
            do{
                pObj = ((IDerivedObject*)pObj)->GetObjRef();
                superClassID = pObj->SuperClassID();
            }while( superClassID == GEN_DERIVOB_CLASS_ID );

            switch(superClassID)
            {
            case GEOMOBJECT_CLASS_ID:
                return true;
                break;
            }
        }
        break;

    case GEOMOBJECT_CLASS_ID:
        {
              return true;
        }
        break;
    default:
        break;
    }
    return bFoundGeomObject;
}

void CMaxEnv::UseBeforeSkeletonPose(bool bUsed)
{
	CheckDlgButton(m_hPannel , IDC_BUSE_BEFORESKELETON_BIND , bUsed?TRUE:FALSE);
}

#include <Userenv.h>
#pragma comment(lib,"Userenv.lib")

void CMaxEnv::BeginLog(const wchar_t* fileName)
{
	HANDLE hToken = 0;
	OpenProcessToken(GetCurrentProcess() , TOKEN_ALL_ACCESS , &hToken);
	wchar_t _usrDir [1024]={0};
	DWORD _size = 1024;
	GetUserProfileDirectoryW(hToken,_usrDir,&_size);
	wstring userDir = wstring(_usrDir) + L"\\" + fileName;
    m_strLogFileName = userDir;
    XEVOL_LOG_REDIR(userDir.c_str());
}

void CMaxEnv::BeginLog()
{
	BeginLog(L"xevol_export_log.txt");
}

void CMaxEnv::FinishLog()
{
	XEVOL_LOG_CLOSE();   
	ShellExecuteW(GetActiveWindow() , L"open" , L"notepad.exe" , m_strLogFileName.c_str() , NULL , SW_SHOW);
}

void CMaxEnv::SaveLogToXComdoc(xcomdoc& doc)
{
	XEVOL_LOG_FLUSH();
    doc.add_file(L"log",m_strLogFileName.c_str());
}
