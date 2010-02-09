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

#pragma  once
#include "XEvolMaxExporter.h"
#include "SetBoneGroupDlg.h"
#include "SetSkinGroupDlg.h"

#include "xcomdoc/xcomdoc.h"
using namespace XEvol3D;

struct sBoneID_t
{
	 int     _id;
	 wstring _nodeID;
	 INode*  _pNode;
};

typedef map<int,sBoneID_t> sBoneIDMap_t;

namespace XEvol3D
{
	void XEVOL_LOG(int logLevel , const char * fmt , ...);
}

using namespace std;

class CMaxEnv
{ 
private:
    CMaxEnv();
public:
   Interface		* m_pInterace;
   HWND               m_hPannel;
   IGameScene*        m_pIGameScene;
   bool               m_bUseBeforeSkeletonPose;
   bool               m_bCalcInitPosMySelf;
   bool               m_bExpVertexNormal;
   bool               m_bInvertTexCoordV;
   bool               m_bInvertYZCoord;
   int                m_compress_rate  ;
   //stringstream           m_log;

  
   static CMaxEnv& Inst()
   {
       static CMaxEnv ms_Env;
       return ms_Env;
   }

   int              getCompressRate() { return m_compress_rate ; }
   unsigned int     MaxTimeToFrame(unsigned int iMaxTime);
   bool             GetSaveFileName(wchar_t* file_name, const wchar_t* extern_string , const wchar_t* ext_name,const wchar_t* strTile);
   void             UseBeforeSkeletonPose(bool bUsed = true);
   void             start(Interface* pInterface , HWND hPannel);
   void             shutdown();
   bool             IsMesh(INode* pNode);
   bool             IsBone( INode* pNode );
   bool             IsXEvol3DBone(INode* pNode);
   void             SetXEvol3DBone(INode* pNode , BOOL bIsBone);
   bool             IsPureDummy(INode* pNode);
   bool             IsBoneDummy(INode* node);
   unsigned int     GetCurrentTime();
			 
   Modifier*        FindPhysiqueModifier(INode* nodePtr);
   Modifier*        FindSkinModifier(INode *pINode);
   INode*           FindNodeByBoneID(int id);
   void             FindNodeByBoneID(int id , INode* pRootNode,vector<INode*>& NodesFined);
   bool             InitNodeBoneID(INode* pNode,sBoneIDMap_t& useID);
   bool             ConfirmBoneID(INode* pNode);
   bool             InitSceneBoneID();
   bool             ConfirmBoneIDs();

   void             BeginLog(const wchar_t* fileName);
   void             BeginLog();
   void             FinishLog();
   void             SaveLogToXComdoc(xcomdoc& doc);
   wstring          GetTextureUsingName(int usage);
protected:
   std::wstring     m_strLogFileName;

public:
	CSetBoneGroupDlg m_BoneGroupDlg;
	CSetSkinGroupDlg m_SkinGroupDlg;
};
typedef std::vector<INode*> sMaxNodes_t;

IGameScene* G_GameScene();
CMaxEnv&    G_MaxEnv();
void        G_SetCurrentTime(unsigned int iMaxTime);
const char* INodeNameA(INode* pNode);
wstring     INodeName(INode* pNode);
wstring     ToWCHAR(const char* pStr);
wstring     ToWCHAR(const string& pStr);
string      ToAnsi(const std::wstring& pStr);
void        WGetWndText(CWnd* pWnd , wchar_t* out , int nTex);
void        GTranspose(GMatrix& Mat);
void        NoTrans(GMatrix& Mat);
#define _XASSERT(exp) do{  if( ! (exp) ){ MessageBoxA(GetActiveWindow(),#exp,"´íÎó",MB_OK); assert(exp);} } while(0)


