#include "../xStdPch.h"
#include "xfrustum.h"
#include "xcamera.h"
#include "xvolume.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XGEOMLIB

bool xFrustum::from(const xMathLib::xmat4& _viewMat , const xMathLib::xmat4& _projMat)
{
	xMathLib::xmat4 vpmat ;
	XM_Mul(_viewMat , _projMat , vpmat);
	xvec4* p = NULL;
    
	/*
	M =   0,  1,  2,  3
	      4,  5,  6,  7
		  8,  9, 10, 11
		 12, 13, 14, 15

   [RIGHT    [-1 , 0 , 0 , 1]
   [LEFT     [1  , 0 , 0 , 1]
   [BOTTOM   [0  , 1 , 0 , 1]
   [TOP      [0  ,-1 , 0 , 1]
   [NEAR     [0  , 0 , 1 , 1]                     (-1,1)
   [FAR      [0  , 0 ,-1 , 1]


      [-1 , 0 , 0 , 1]    [  0,  1,  2,  3   ]    
      [1  , 0 , 0 , 1]	  [  4,  5,  6,  7   ]
      [0  , 1 , 0 , 1]	  [  8,  9, 10, 11   ]
      [0  ,-1 , 0 , 1]	  [ 12, 13, 14, 15   ]
      [0  , 0 , 1 , 1]
      [0  , 0 ,-1 , 1]
	*/
    
	xvec4 vRIGHT    (-1 , 0 , 0 , 1);
	xvec4 vLEFT     (1  , 0 , 0 , 1);
	xvec4 vBOTTOM   (0  , 1 , 0 , 1);
	xvec4 vTOP      (0  ,-1 , 0 , 1);
	xvec4 vNEAR     (0  , 0 , 1 , 1);
	xvec4 vFAR      (0  , 0 ,-1 , 1);

	p= (xvec4*)&m_plane[PL_RIGHT];
    XM_Mul(vpmat ,vRIGHT ,  *p);

	p= (xvec4*)&m_plane[PL_LEFT];
	XM_Mul( vpmat ,vLEFT , *p);

	p= (xvec4*)&m_plane[PL_BOTTOM];
	XM_Mul(vpmat ,vBOTTOM , *p);

	p= (xvec4*)&m_plane[PL_TOP];
	XM_Mul(vpmat ,vTOP ,  *p);

	p= (xvec4*)&m_plane[PL_FAR];
	XM_Mul(vpmat ,vFAR ,  *p);

	p= (xvec4*)&m_plane[PL_NEAR];
	XM_Mul( vpmat ,vNEAR , *p);

	// Normalize all plane normals
	for(size_t i=0;i<PL_MAX;i++)
	 	m_plane[i].normalize();

	return false;
}

bool xFrustum::from(xCamera* pCamera)
{
	return false;
}

bool xFrustum::visible(const xMathLib::xvec3& pt)
{
	return true;
}

bool xFrustum::visible(const xaabb& aabb)
{
	return true;
}

bool xFrustum::visible(const xaabb& aabb , const xMathLib::xmat4& aabbMat)
{
	return true;
}

bool xFrustum::visible(const xMathLib::xvec3& _cent , float radius)
{
    for(size_t i = 0 ; i < PL_MAX ; i ++)
	{
		float dist = _cent.x * m_plane[i].A + _cent.y * m_plane[i].B + _cent.z * m_plane[i].C + m_plane[i].D ;
        if(dist < -radius)
			return false;
	}
	return true;
}
END_NAMESPACE_XGEOMLIB

