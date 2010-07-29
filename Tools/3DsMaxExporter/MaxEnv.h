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
	 int _id;
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
	int compress_rate  ;

private:
    CMaxEnv()
    {
		compress_rate = 0;
		compress_rate = XCOMDOC_COMPRESS_BEST;
        m_bUseBeforeSkeletonPose = true; 
        m_bCalcInitPosMySelf     = true;
        m_bExpVertexNormal       = false;
	    m_bInvertTexCoordV       = false;
	    m_bInvertYZCoord         = false;
    };
    static bool      __init_node_id(INode* pNode,sBoneIDMap_t& useID);
    static bool      __confirm_node_id(INode* pNode);
public:
   Interface		* m_pInterace;
   HWND               m_hPannel;
   IGameScene*        m_pIGameScene;
   bool               m_bUseBeforeSkeletonPose;
   bool               m_bCalcInitPosMySelf;
   bool               m_bExpVertexNormal;
   bool               m_bInvertTexCoordV;
   bool               m_bInvertYZCoord;
   //stringstream           m_log;

   int    getCompressRate() {return compress_rate ; }
   void   setCompressRate(int v) { compress_rate = v;}
   static CMaxEnv& singleton()
   {
       static CMaxEnv ms_Env;
       return ms_Env;
   }
   static CMaxEnv& GetInstance()
   {
	   return singleton();
   }
   static CSetBoneGroupDlg m_BoneGroupDlg;
   static CSetSkinGroupDlg m_SkinGroupDlg;


   bool             get_saved_filename(wchar_t* file_name, const wchar_t* extern_string , const wchar_t* ext_name,const wchar_t* strTile, bool saved);
   float            TicksToSeconds(TimeValue ticks);
   TimeValue        SecondsToTicks(float sec);
   void             SetBipedUniform(INode *pNode, BOOL bUniform);
   void             UseBeforeSkeletonPose(bool bUsed = true);
   void             start(Interface* pInterface , HWND hPannel);
   void             shutdown();
   float            getFps();
   static bool      Init_scene_bone_id();
   static bool      Confirm_Scene_BoneID();

   static bool      IsMesh(INode* pNode);
   static bool      IsBone( INode* pNode );
   static bool      IsXEvol3DBone(INode* pNode);
   static void      SetXEvol3DBone(INode* pNode , BOOL bIsBone);
   static bool      IsPureDummy(INode* pNode);
   static bool      IsBoneDummy(INode* node);

   static Modifier* FindPhysiqueModifier(INode* nodePtr);
   static Modifier* FindSkinModifier(INode *pINode);
   static INode*    FindNodeByBoneID(int id);
   static void      FindNodeByBoneID(int id , INode* pRootNode,vector<INode*>& NodesFined);

   void             BeginLog(const wchar_t* fileName);
   void             BeginLog();
   void             FinishLog();
   void             SaveLogToXComdoc(xcomdoc& doc);
protected:
   std::wstring     m_strLogFileName;
};
const char* INodeNameA(INode* pNode);
wstring     INodeName(INode* pNode);
wstring     ToWCHAR(const char* pStr);
wstring     ToWCHAR(const string& pStr);
string      ToAnsi(const std::wstring& pStr);
void        WGetWndText(CWnd* pWnd , wchar_t* out , int nTex);


typedef std::vector<INode*>           sMaxNodes_t;
wstring tex_usage_string(int usage);
#define _XASSERT(exp) do{  if( ! (exp) ){ MessageBoxA(GetActiveWindow(),#exp,"´íÎó",MB_OK); assert(exp);} } while(0)


