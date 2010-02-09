#ifndef __XEVOL_PARTICLE_EMITTER_H__
#define __XEVOL_PARTICLE_EMITTER_H__
#include "../BaseLib/stl_dllalloc.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xParticle
{
public:
	xvec4     m_size;      //粒子的大小
	xColor_4f m_color;     //粒子的颜色
	xvec4     m_pos;       //粒子的位置
	xquat     m_rotate;    //粒子的旋转
	xvec4     m_life;      //.x life ; .y total life.
public:

};

END_NAMESPACE_XEVOL3D
#endif
