#include "../xStdPch.h"
#include "xBaseModelMgr.h"
#include "xCoreActionMgr.h"
#include "xMixAction.h"
#include "xCoreSkeleton.h"
#include "../xcomdoc/xdocfstream.h"
#include "../BaseLib/xI18N.h"
#include "xCoreMesh.h"

using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D 

IMPL_BASE_OBJECT_CLASSID(xMixedAction         , xBaseAction);
IMPL_ACTION_FACTORY(xMixedAction);
xMixedAction::xMixedAction(xCoreSkeleton* pSkeleton , int param) : xBaseAction(pSkeleton , param)
{
	m_Info.m_eActType    = eActType_Mix;
	m_Info.m_iFirstFrame = 0;
	m_Info.m_iLastFrame  = 200;
	m_Info.m_lTime       = 5000; //5秒
	m_Info.m_nFrame      = 200;
	m_pSkeleton          = pSkeleton;
	m_pActionSlotState   = NULL;
}


xMixedAction::~xMixedAction()
{

}

bool xMixedAction::setMainAction(xBaseAction* pAction)
{
	return true;
}

bool xMixedAction::setAction(const wchar_t* slotName , HCoreAction pAction)
{
	return true;
}

bool xMixedAction::setAction(int slotName , HCoreAction pAction)
{
	xActionSlot& slot = m_ActionSlots[slotName];
	slot.setAction(pAction);
	return true;
}

bool xMixedAction::setAction(const wchar_t* slotName , xBaseAction* pAction)
{
   return true;
}

bool xMixedAction::setAction(int slotName , xBaseAction* pAction)
{
    xActionSlot& slot = m_ActionSlots[slotName];
    slot.setAction(pAction);
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
	unload();
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
			slot.m_pAttribute = new xCoreActionAttr();
			slot.m_pAttribute->init(_nBone , xCoreActionAttr::eBoneExclude );
			slot.m_hashName = xStringHash( slot.m_strName.c_str() );
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
						m_BoneActionMap[i] = (int)iSlot;
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
				setBoneToActionSlot(boneIdx , (int)iSlot , bRecursive , onlyChild);
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

bool xMixedAction::getBoneTrans(int boneIndex, xBaseActionState* _pActionState , xBoneTrans& boneTrans)
{
    int iSlot = m_BoneActionMap[boneIndex];
    xBaseActionState* pActionState = _pActionState + iSlot;
    xBaseAction* pAction = m_ActionSlots[iSlot].getAction();
    return pAction->getBoneTrans(boneIndex , pActionState , boneTrans);
}

xBaseActionState* xMixedAction::getActionState(int boneIndex, xBaseActionState* pActionState)
{
    int iSlot = m_BoneActionMap[boneIndex];
    return (pActionState + iSlot);
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
        const xActionInfo* pActionInfo = slot.getAction()->info();
		slot.getAction()->updateState( SlotState.m_lTime , SlotState );
    }

    return pSkeleton->blendSlerp(this , pActionSlotState , skeletonFrame );

}

bool  xMixedAction::blend(long _time_in_ms,xCoreActionFrame* _skeletonFrame, xCoreSkeleton* pSkeleton)
{
	xCoreActionFrame& skeletonFrame = *_skeletonFrame;	
	size_t nSlot = m_ActionSlots.size();
	if(m_pActionSlotState == NULL)
		m_pActionSlotState = new xBaseActionState[nSlot];

	for(size_t i = 0 ; i < nSlot ; i ++)
	{
		xBaseActionState& SlotState = m_pActionSlotState[i];
		SlotState.m_lTime = _time_in_ms;
	}
	return blend(m_pActionSlotState, _skeletonFrame , pSkeleton);
}



bool  xMixedAction::blend(long _time_in_ms[], xCoreActionFrame* _skeletonFrame, xCoreSkeleton* pSkeleton)
{
	xCoreActionFrame& skeletonFrame = *_skeletonFrame;	
	size_t nSlot = m_ActionSlots.size();
	if(m_pActionSlotState == NULL)
		m_pActionSlotState = new xBaseActionState[nSlot];

	for(size_t i = 0 ; i < nSlot ; i ++)
	{
		xBaseActionState& SlotState = m_pActionSlotState[i];
		SlotState.m_lTime = _time_in_ms[i];
	}
	return blend(m_pActionSlotState, _skeletonFrame , pSkeleton);
}

void xMixedAction::unload()
{
	size_t nSlot = m_ActionSlots.size();
    for(size_t i = 0 ; i < nSlot ; i ++)
	{
		xMixedAction::xActionSlot& slot = m_ActionSlots[i];
		XSAFE_DELETE(slot.m_pAttribute);
	}
	XSAFE_DELETE_ARRAY(m_pActionSlotState);
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
    const xBaseAction* pAction = slot.getAction();
    if(pAction == NULL) return NULL;
	return pAction->info();
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
