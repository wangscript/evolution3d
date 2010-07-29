#ifndef __XMIX_ACTION_H__
#define __XMIX_ACTION_H__

#include "xBaseAction.h"

BEGIN_NAMESPACE_XEVOL3D
//混合的动作。可以有上下身混合的动作，表情混合动作等等
/***********************************************
动作混合的原理：
1. 混合的动作，需要加载一个动作配置文件。扩展名为.mix
   这个文件一般跟其它的动作文件放在一起。收动作管理器
   的管理，加载是需要给xActionName指定Type为"Mix";

2. 动作配置文件中指定分组，分组称为xActionSlot。每个
   Slot有名字和属性表。

3.xActionSlot中有一个Attribute数组。读入.mix文件，其实
  就是设置这些Slot中的Attribute数组，同时设置xMixedAction
  中的m_BoneActionMap。Map将会指出某块骨骼属于哪个Slot控制。
  在Slot的Attribute中，只有属于该Slot的Bone.其Attribute值
  才会是0.

5. 在.mix文件中，如果某个slot没指定哪些骨骼，则会使用所有
   前面分配剩下的骨骼。 如果指定了骨骼，则可以选择是不是
   递归分配所有的子骨骼（一般情况下是true)。也可以设置是
   只分配其子骨骼。

6.混合动作的时候，getBoneData会根据Bone属于哪个Slot。而将
  getBoneData的调用转到该Slot上挂接的xCoreAction上去。

7.混合动作在MixedAction中有个专用函数。可以为每个Slot上的
  动作指定一个时间轴。注意time_in_ms是个数组。


***********************************************/
class _XEVOL_BASE_API_ xMixedAction : public xBaseAction
{
	IMPL_BASE_OBJECT_INTERFACE(xMixedAction);
public:

	struct xActionSlot
	{
		std::ds_wstring       m_strName;
		int                   m_hashName;
		xCoreActionAttr*      m_pAttribute;
    public:
        xActionSlot()
        {
            m_hAction.setNULL();
        }
        xBaseAction*          getAction() const
        {
            return m_hAction;
        }
        void                  setAction(HCoreAction hAction)
        {
            m_hAction = hAction;
        }
        void                  setAction(xBaseAction* pAction)
        {
            m_hAction = HCoreAction(pAction);
        }

    protected:
        HCoreAction           m_hAction;
	};

public:
	xMixedAction(xCoreSkeleton* pSkeleton , int param);
	~xMixedAction();
public:
    bool                 setMainAction(xBaseAction* pAction);
	bool                 setAction(const wchar_t* slotName , HCoreAction pAction);
	bool                 setAction(int iSlot , HCoreAction pAction);
    bool                 setAction(const wchar_t* slotName , xBaseAction* pAction);
    bool                 setAction(int iSlot , xBaseAction* pAction);

	xActionSlot&         actionSlot(int iSlot){ return m_ActionSlots[iSlot] ; }
    int                  nActionSlot() const { return (int)m_ActionSlots.size() ; }
    bool                 getBoneTrans(int boneIndex, xBaseActionState* _pActionState , xBoneTrans& boneTrans);
public:
	bool                 load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir);
	bool                 load(const wchar_t * _name , xcomdocstream* stream);
	bool                 load(const wchar_t * _name , istream& stream);
	bool                 load(xXmlNode* pCfgNode );

    xBaseActionState*    getActionState(int boneIndex, xBaseActionState* pActionState);
	xCoreActionAttr*     attribute();
	
	bool                 blend(long time_in_ms  , xCoreActionFrame* skeletonFrame, xCoreSkeleton* pSkeleton);
    bool                 blend(long time_in_ms[], xCoreActionFrame* skeletonFrame, xCoreSkeleton* pSkeleton);
    bool                 blend(xBaseActionState* pActionSlotState , xCoreActionFrame* skeletonFrame, xCoreSkeleton* pSkeleton );
	void                 unload();
	int                  memUsage();
	bool                 setTime(float _time);
    float                getDurTime();
	const xActionInfo*   info() const;
	const xSkeletonID&   skeletonID() const;
	const wchar_t*       name() const;
	const int            nBone() const;
	

protected:
	bool    setBoneToActionSlot(int boneIdx , int iSlot , bool bRecursive , bool onlyChild);
    bool    excludeBone(int boneIdx , int iSlotOwnner);
protected:
	typedef ds_vector(xActionSlot)  xActionSlots;
	typedef std::vector<int>        xBoneActionMap;

	xActionSlots      m_ActionSlots;
	xBoneActionMap    m_BoneActionMap;
	xCoreSkeleton*    m_pSkeleton;
	xBaseActionState* m_pActionSlotState;
};

END_NAMESPACE_XEVOL3D
#endif
