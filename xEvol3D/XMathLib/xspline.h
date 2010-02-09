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

#ifndef _X_MATH_BEZIER_H_
#define _X_MATH_BEZIER_H_
#include "../BaseLib/stl_dllalloc.h"
#include "xbinomial.h"
#include "xMathLib.h"
BEGIN_NAMESPACE_XGEOMLIB
template <typename T , template <typename T> class _Alloc = std::allocator > class xBezier
{
public:
	typedef typename T::value_type data_type;
public:
	xBezier(T* pt , int nPt) : m_Binomial(nPt - 1)
	{
		m_nControlPts = nPt;
		m_invTCoffie = m_Alloc2.allocate(nPt);
		m_ControlPts = m_Alloc.allocate(nPt);
		for(int i = 0 ; i < nPt  ; i ++)
		{
			m_ControlPts[i] = pt[i];
		}
	}

	xBezier(int nPt) 
	{
        init(nPt);
	}

	xBezier()
	{
		m_nControlPts = 0;
		m_ControlPts  = NULL;
		m_invTCoffie  = NULL;
	}

	void init(int nPt)
	{
		m_Binomial.init(nPt - 1);
		m_nControlPts = nPt;
		m_invTCoffie = m_Alloc2.allocate(nPt);
		m_ControlPts = m_Alloc.allocate(nPt);
	}

	void setControlPoint(const T& pt , int i)
	{
		m_ControlPts[i] = pt;
	}

	~xBezier()
	{
		if(m_invTCoffie) m_Alloc2.deallocate(m_invTCoffie , m_nControlPts);
		if(m_ControlPts) m_Alloc.deallocate(m_ControlPts , m_nControlPts);
	}

	T& operator[] (int t)
	{
		//用贝塞尔曲线公式计算
		return m_ControlPts[t];
	}
    T calcControlPt(data_type t , data_type t1 , int iPt , int iRank)
	{
		if(iRank == 0) return m_ControlPts[iPt];

		return calcControlPt(t , t1 , iPt , iRank - 1 ) * t1 + calcControlPt(t , t1 , iPt + 1 , iRank - 1) * t;
	}

	T operator()(data_type t)
	{
        //用递归计算.
        data_type t1 = (data_type)1.0f - t;
		return calcControlPt(t , t1  , 0 , m_nControlPts - 1);

	}

	T value(data_type t)
	{
		//用贝塞尔曲线公式计算
        m_invTCoffie[m_nControlPts-1] = 1.0f;
		for(int i = m_nControlPts - 2 ; i >= 0 ;  i --)
			m_invTCoffie[i] = (1.0f - t) * m_invTCoffie[i + 1];

		data_type _t = 1.0f;
		T _ret(0.0f);
		for(int i = 0 ; i < m_nControlPts ; i ++)
		{
			T _item = m_ControlPts[i] * ( (data_type)m_Binomial[i] * _t * m_invTCoffie[i] ) ;
			_ret  = _ret +  _item;
			_t *= t ;
		}
		return _ret;
	}

public:
	xBinomial<data_type , _Alloc<data_type> > m_Binomial;
	int                     m_nControlPts;
	T*                      m_ControlPts;
    data_type*              m_invTCoffie;
	_Alloc<T>               m_Alloc;
	_Alloc<data_type>       m_Alloc2;
};


template <typename T , template <typename T> class _Alloc = std::allocator > class xBezierSpline
{
public:
	typedef T                              data_type;
	typedef xMathLib::TVec3<T>             point_type; 
	typedef xBezier<point_type , _Alloc>   bezier;

public:
	xBezierSpline()
	{
		  m_nPt = 0;
	}

	~xBezierSpline()
	{

	}

	void init(int nPt)
	{
		m_vPoints.resize(nPt); // 定位点,nPt个
		m_vNormals.resize(nPt); // 定位点上的

		m_vBiziers.resize(nPt - 1);
		for(int i  = 0 ; i < nPt - 1 ; i ++) 
		{
			m_vBiziers[i].init(3); //都是两阶的bezier曲线
		}

		m_nPt = nPt;
	}

	void setControlPoint(int i , const point_type& pt , const point_type& normal)
	{
		m_vPoints[i]  = pt;
		m_vNormals[i] = normal;
	}

	bool setup()
	{
		for(int i = 0 ; i < m_nPt - 1 ; i ++)
		{
			bezier& bz = m_vBiziers[i];
			point_type& pt0 = m_vPoints[i];
			point_type& pt1 = m_vPoints[i + 1];
			point_type& nr0 = m_vNormals[i];
			point_type& nr1 = m_vNormals[i + 1];

			bz[0] = pt0;
			bz[2] = pt1;
			//现在来计算中间那个bezier控制点.
			//这个控制点应该是由 跟两个端点垂直的平面相交的直线上 到 这两个点连成的线段最近的点。
			
			//先来求两个平面相交的直线 n1 . p = s1 ;  n0 . p = s0;
			data_type s0 = pt0.dp(nr0);
			data_type s1 = pt1.dp(nr1);
			data_type n0n1 = nr0.dp(nr1);
			data_type a  = (s1 * n0n1 - s0) / (n0n1 * n0n1 - 1);
			data_type b =  (s0 * n0n1 - s1) / (n0n1 * n0n1 - 1);

			//求得相交的直线了。
            point_type p0  = nr0 * a + nr1 * b;
			point_type d0 = nr0.cp(nr1);

			//控制点定义的直线
			point_type& p1 = pt0;
			point_type  d1 = pt1 - pt0;
			d1.normalize();

			point_type p ;
			{
				data_type b = - d0.dp( d1 );
				data_type d = d0.dp( p0 - p1);
				data_type e = d1.dp( p0 - p1);
				data_type s = ( b * e - d) / ( 1 - b * b );

                p = p0 + d0 * s ;
			}

			bz[1] = p;
		}
		return true;
	}



	point_type operator() (int idx , data_type t)
	{
		bezier& bz = m_vBiziers[idx];
		return bz(t);
	}

public:
	typedef std::vector<point_type > point_vector;
    typedef std::vector<bezier >         bezier_vector;
	bezier_vector m_vBiziers; // nPt - 1;
	point_vector  m_vPoints ; // 定位点,nPt个
	point_vector  m_vNormals; // 定位点上的
	int           m_nPt;
};


template <typename T , template <typename T> class _Alloc = std::allocator > class xCubicSpline
{
public:
	typedef T                         data_type;
    typedef xMathLib::TVec4<T>        point_type; 
public:
    struct xPointNode
	{
		point_type m_Position;
		point_type m_Tangent;
		data_type  m_startTime;
	};
	typedef std::vector<xPointNode , _Alloc<xPointNode> > node_vector;

public:
	xCubicSpline()
	{
		matH.m[0][0] = (data_type)2.0 ; matH.m[0][1] = (data_type)-2.0;  matH.m[0][2] = (data_type)1.0 ;  matH.m[0][3] = (data_type) 1.0; 
		matH.m[1][0] = (data_type)-3.0; matH.m[1][1] = (data_type) 3.0;  matH.m[1][2] = (data_type)-2.0;  matH.m[1][3] = (data_type)-1.0; 
		matH.m[2][0] = (data_type)0.0 ; matH.m[2][1] = (data_type) 0.0;  matH.m[2][2] = (data_type) 1.0;  matH.m[2][3] = (data_type) 0.0; 
		matH.m[3][0] = (data_type)1.0 ; matH.m[3][1] = (data_type) 0.0;  matH.m[3][2] = (data_type) 0.0;  matH.m[3][3] = (data_type) 0.0; 

	}
	void init(int nPt)
	{
        m_vNodes.resize(nPt);
		for(int i = 0 ; i < nPt ; i ++)
		{
			m_vNodes[i].m_startTime = (data_type)0.0;
		}
	}

	bool setup()
	{
		size_t nPt = m_vNodes.size() ;
		data_type    len = (data_type)0.0;
        for(size_t i = 0 ; i < nPt - 1  ; i ++)
		{
			xPointNode& ptNode = m_vNodes[i];
			xPointNode& ptNodeNext = m_vNodes[ i + 1];
			point_type  v = ptNodeNext.m_Position - ptNode.m_Position;
			len += v.fabs();			
		}

		data_type dist = (data_type)0.0;
		for(size_t i = 0 ; i < nPt - 1  ; i ++)
		{
			xPointNode& ptNode = m_vNodes[i];
			xPointNode& ptNodeNext = m_vNodes[ i + 1];
			point_type  v = ptNodeNext.m_Position - ptNode.m_Position;
			ptNode.m_startTime = dist/len;
			dist += v.fabs();			
		}

		m_vNodes[nPt-1].m_startTime = (data_type)1.0;
		return true;
	}
    
	void   evolute(const xPointNode& p1 , const xPointNode& p2 , data_type fTime , point_type& ret)
	{
         data_type _tTime = p2.m_startTime - p1.m_startTime;
		 data_type t = (fTime-p1.m_startTime)/_tTime;
		 data_type t2 = t*t;
		 data_type t3 = t*t2;
         point_type vt(t3 , t2 , t , (data_type)1.0 );
		 *(point_type*)&matG.m[0][0] = p1.m_Position;
		 *(point_type*)&matG.m[1][0] = p2.m_Position;
		 *(point_type*)&matG.m[2][0] = p1.m_Tangent;
		 *(point_type*)&matG.m[3][0] = p2.m_Tangent;

		 xMathLib::xmat4 mat ;
		 xMathLib::XM_Mul(matH , matG , mat);
		 xMathLib::XM_Mul(vt , mat , ret);
		 return ;
	}

	point_type   operator()(data_type fTime)
	{
		size_t nPt = m_vNodes.size() ;
		for(size_t i = 0 ; i < nPt  ; i ++)
		{
			xPointNode& ptNode = m_vNodes[i];
			xPointNode& ptNodeNext = m_vNodes[ i + 1];
			if(ptNode.m_startTime <= fTime && ptNodeNext.m_startTime >= fTime)
			{
				point_type _ret;
				evolute(ptNode , ptNodeNext , fTime , _ret);
				return _ret;
			}
		}
		return point_type();
	}
public:
	node_vector     m_vNodes;
	xMathLib::xmat4 matH;
	xMathLib::xmat4 matG;
};

END_NAMESPACE_XGEOMLIB
#endif
