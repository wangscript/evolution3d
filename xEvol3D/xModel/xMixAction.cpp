#include "../xStdPch.h"
#include "xMixAction.h"
#include "xCoreSkeleton.h"
#include "../xcomdoc/xdocfstream.h"
#include "../BaseLib/xI18N.h"
#include "xCoreMesh.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D 

IMPL_BASE_OBJECT_CLASSID(xMixedAction         , xCoreAction);

xMixedAction::xMixedAction(xCoreSkeleton* pSkeleton)
{
	m_Info.m_eActType    = eActType_Mix;
	m_Info.m_iFirstFrame = 0;
	m_Info.m_iLastFrame  = 200;
	m_Info.m_lTime       = 5000; //5秒
	m_Info.m_nFrame      = 200;
	m_pSkeleton          = pSkeleton;
}

const wchar_t* xMixedAction::typeID() const
{
	return L"mix";
}

xMixedAction::~xMixedAction()
{

}

bool xMixedAction::setMainAction(xCoreAction* pAction)
{
	return true;
}

bool xMixedAction::setAction(const wchar_t* slotName , xCoreAction* pAction)
{
	return true;
}

bool xMixedAction::setAction(int slotName , xCoreAction* pAction)
{
	xActionSlot& slot = m_ActionSlots[slotName];
	slot.m_pAction = pAction;
	return true;
}


bool xMixedAction::load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir)
{
	std::ds_wstring fullName = std::ds_wstring(actionDir) + pCfgNode->name() + L".xra";
	xcomdocstream* stream =doc.open_stream(fullName.c_str() , true);
	if(stream == NULL)
	{
		//找没扩展名的
		fullName = std::ds_wstring(actionDir) + pCfgNode->name();
		stream = doc.open_stream(fullName.c_str() , true);
	}

	if(stream == NULL)
		return false;

	bool ret= load(pCfgNode->value(L"Name") , stream );
	doc.close_stream(stream);
	return ret;
}

bool xMixedAction::load(const wchar_t * _name , xcomdocstream* stream)
{
	char* buf = new char[stream->data_len()];
	stream->read(buf , stream->data_len() );
	xXmlDocument doc;
	if(false == doc.load(buf , stream->data_len() , true) )
	    return false;
	return load(doc.root());
}

bool xMixedAction::load(const wchar_t * _name , istream& stream)
{
	xXmlDocument doc;
	if( doc.load(stream , true) == false )
		return false;
	return load(doc.root());
}

bool xMixedAction::load(xXmlNode* pXmlNode )
{
	int _nBone = m_pSkeleton->nBone();
    m_BoneActionMap.resize(_nBone);
	for(int i = 0 ; i <  _nBone; i ++)
	{
		m_BoneActionMap[i] = -1;
	}

	if(pXmlNode->name() == std::ds_wstring(L"slots") )
	{
		xXmlNode::XmlNodes slotNodes;
		pXmlNode->findNode(L"slot" , slotNodes);
		size_t nSlot = slotNodes.size();
        m_ActionSlots.resize(nSlot);
		for(size_t iSlot = 0 ; iSlot < nSlot ; iSlot ++)
		{
			xXmlNode* pSlotNode = slotNodes[iSlot];
			xActionSlot& slot = m_ActionSlots[iSlot];
			slot.m_strName = pSlotNode->value(L"name");
			slot.m_pAction = NULL;
			slot.m_pAttribute = new xCoreActionAttr();
			slot.m_pAttribute->init(_nBone , xCoreActionAttr::eBoneExclude );
			slot.m_hashName = xStringHash( slot.m_strName.c_str() );
			slot.m_TimeShift = 0;
			//初始化骨骼;
			xXmlNode::XmlNodes boneNodes;
			pSlotNode->findNode(L"bone" , boneNodes);
			size_t nBoneInSlot = boneNodes.size();

            //如果没有bone列表。则表示这个Slot瓜分所有剩下的.
			if(boneNodes.size() == 0 && iSlot != 0)
			{
				//分配所有没有分配过的
				for(int i = 0 ; i <  _nBone; i ++)
				{
					if(m_BoneActionMap[i] == -1) 
					{
						m_BoneActionMap[i] = iSlot;
						(*slot.m_pAttribute)[i] = xCoreActionAttr::eBoneInclude ;
					}
				}
			}

			//分配所有的Bone;
			for(size_t iBone = 0 ; iBone < nBoneInSlot ; iBone ++)
			{
				xXmlNode* pBoneNode = boneNodes[iBone];
				bool bRecursive = pBoneNode->bool_value(L"recursive");
				bool onlyChild = pBoneNode->bool_value(L"onlyChild");

				int boneIdx = m_pSkeleton->findBoneIndex(pBoneNode->value(L"name"));
				setBoneToActionSlot(boneIdx , iSlot , bRecursive , onlyChild);
			}

		}

		//将所有没有分配过的骨骼，全部分配到第一个节点上。
		for(int i = 0 ; i <  _nBone; i ++)
		{
			if(m_BoneActionMap[i] == -1) 
			{
				m_BoneActionMap[i] = 0;
				xActionSlot& slot = m_ActionSlots[0];
				(*slot.m_pAttribute)[i] = xCoreActionAttr::eBoneInclude ;
			}
		}
	}
	return true;
}

bool xMixedAction::excludeBone(int boneIdx , int iSlotOwnner)
{
    int _nSlot = (int)m_ActionSlots.size();
    for(int i = 0 ; i < iSlotOwnner ; i ++)
    {
        if(iSlotOwnner == i) continue ;
        xActionSlot& slot = m_ActionSlots[i];
        (*slot.m_pAttribute)[boneIdx] = xCoreActionAttr::eBoneExclude;
    }
    return true;
}

bool xMixedAction::setBoneToActionSlot(int boneIdx , int iSlot , bool bRecursive , bool onlyChild)
{
	if(!onlyChild)
	{
		m_BoneActionMap[boneIdx] = iSlot;
		xActionSlot& slot = m_ActionSlots[iSlot];
		(*slot.m_pAttribute)[boneIdx] = xCoreActionAttr::eBoneInclude;
        excludeBone(boneIdx , iSlot);
	}

	if(bRecursive == false)
		return true;

	xBoneHiberarchy& boneH = m_pSkeleton->boneHiberarchy(boneIdx);

	for(int i = 0 ; i < boneH.nChildren(); ++i)
	{
		int     childIndex = boneH.childIndex(i);
		setBoneToActionSlot(childIndex , iSlot , bRecursive , false);
	}
	return true;
}


xBaseActionState* xMixedAction::getActionState(int boneIndex, xBaseActionState* pActionState)
{
    int iSlot = m_BoneActionMap[boneIndex];
    return (pActionState + iSlot);
}

xBoneData*  xMixedAction::getBoneData(int boneIndex, int frame )
{
	int iSlot = m_BoneActionMap[boneIndex];
	xMixedAction::xActionSlot& slot = m_ActionSlots[iSlot];
	return slot.m_pAction->getBoneData(boneIndex, frame );
}

xCoreActionAttr*  xMixedAction::attribute()
{
	return NULL;
}

bool xMixedAction::blend(xBaseActionState* pActionSlotState , xCoreActionFrame* _skeletonFrame, xCoreSkeleton* pSkeleton )
{
    xCoreActionFrame& skeletonFrame = *_skeletonFrame;
    size_t nSlot = m_ActionSlots.size();
    for(size_t i = 0 ; i < nSlot ; i ++)
    {
        xMixedAction::xActionSlot& slot = m_ActionSlots[i];
        xBaseActionState& SlotState = pActionSlotState[i];
        int& frame1   = SlotState.m_Frame1;
        int& frame2   = SlotState.m_Frame2;
        float& tfloat = SlotState.m_fTime;
        const xActionInfo* pActionInfo = slot.m_pAction->info();
        int realTime = slot.m_TimeShift + SlotState.m_lTime;
        if(slot.m_pAction->bLoop())
        {
            tfloat = _getBlendFrameLoop( pActionInfo ,  realTime , frame1 , frame2);
        }
        else
        {
            tfloat = _getBlendFrameClamp(pActionInfo ,  realTime , frame1 , frame2);
        }
    }

    return pSkeleton->blendSlerp(this , pActionSlotState , skeletonFrame );

}

bool  xMixedAction::blend(long _time_in_ms,xCoreActionFrame* _skeletonFrame, xCoreSkeleton* pSkeleton)
{
	xCoreActionFrame& skeletonFrame = *_skeletonFrame;
	size_t nSlot = m_ActionSlots.size();
	for(size_t i = 0 ; i < nSlot ; i ++)
	{
		xMixedAction::xActionSlot& slot = m_ActionSlots[i];
		int frame1;
		int frame2;
		float tfloat = 0.0f;
		const xActionInfo* pActionInfo = slot.m_pAction->info();
		int realTime = slot.m_TimeShift + _time_in_ms;
		if(m_bLoopAction)
		{
			tfloat = _getBlendFrameLoop( pActionInfo ,  realTime , frame1 , frame2);
		}
		else
		{
			tfloat = _getBlendFrameClamp(pActionInfo ,  realTime , frame1 , frame2);
		}
		pSkeleton->blendSlerp(slot.m_pAction , slot.m_pAttribute , skeletonFrame,tfloat,frame1,frame2);
	}
	return true;
}



bool  xMixedAction::blend(long _time_in_ms[], xCoreActionFrame* _skeletonFrame, xCoreSkeleton* pSkeleton)
{
    xCoreActionFrame& skeletonFrame = *_skeletonFrame;
    size_t nSlot = m_ActionSlots.size();
    for(size_t i = 0 ; i < nSlot ; i ++)
    {
        xMixedAction::xActionSlot& slot = m_ActionSlots[i];
        int frame1;
        int frame2;
        float tfloat = 0.0f;
        const xActionInfo* pActionInfo = slot.m_pAction->info();
        int realTime = _time_in_ms[i];
        if(slot.m_pAction->bLoop() )
        {
            tfloat = _getBlendFrameLoop( pActionInfo ,  realTime , frame1 , frame2);
        }
        else
        {
            tfloat = _getBlendFrameClamp(pActionInfo ,  realTime , frame1 , frame2);
        }
        pSkeleton->blendSlerp(slot.m_pAction , slot.m_pAttribute , skeletonFrame,tfloat,frame1,frame2);
    }
    return true;
}

void xMixedAction::unload()
{
	size_t nSlot = m_ActionSlots.size();
    for(size_t i = 0 ; i < nSlot ; i ++)
	{
		xMixedAction::xActionSlot& slot = m_ActionSlots[i];
		XSAFE_DELETE(slot.m_pAttribute);
	}
	m_ActionSlots.clear();
	m_BoneActionMap.clear();
}

int xMixedAction::memUsage()
{
    return 1;
}

bool xMixedAction::setTime(float _time)
{
    return false;
}

float xMixedAction::getDurTime()
{
    return 0.0f;
}

const xActionInfo* xMixedAction::info() const
{
	const xMixedAction::xActionSlot& slot = m_ActionSlots[0];
	return slot.m_pAction->info();
}

const xSkeletonID& xMixedAction::skeletonID() const
{
      return m_pSkeleton->id();
}

const wchar_t* xMixedAction::name() const
{
     return m_Name.c_str();
}

const int xMixedAction::nBone() const
{
	return m_pSkeleton->nBone();
}

END_NAMESPACE_XEVOL3D
