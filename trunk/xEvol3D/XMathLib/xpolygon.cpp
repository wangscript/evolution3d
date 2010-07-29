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

#include "../xStdPch.h"
#include "xMathLib.h"
#include "xGeomLib.h"
using namespace XEvol3D::XMathLib;
BEGIN_NAMESPACE_XGEOMLIB

class CPolygonToTriangles
{
	struct _Pt
	{
		xMathLib::xvec2     m_Pt;
		int                 m_idx;
	};
	typedef std::list<_Pt>    vPtList; 
	typedef vPtList::iterator vPtListIterator;
	vPtList                 m_PtList;
	const xMathLib::xvec2*  m_ptList;
	int                     m_nPt;
public:
	int GetNextPt(vPtListIterator curPos , xMathLib::xvec2& _ret)
	{
		if(m_PtList.size() == 0)
			return -1;


		vPtList::iterator _pos  = NextIterator(curPos);
		_ret = _pos->m_Pt;
		return _pos->m_idx;
	}

	int GetNext2Pt(vPtListIterator curPos , xMathLib::xvec2& _ret)
	{
		if(m_PtList.size() == 0)
			return -1;
		vPtList::iterator _pos  = Next2Iterator(curPos);
		_ret = _pos->m_Pt;
		return _pos->m_idx;
	}

	vPtListIterator NextIterator(vPtListIterator curPos )
	{
		if(m_PtList.size() == 0)
			return m_PtList.end();
		curPos ++;
		if(curPos == m_PtList.end() )
			curPos = m_PtList.begin();
		return curPos;
	}

	vPtListIterator Next2Iterator(vPtListIterator curPos)
	{
		if(m_PtList.size() == 0)
			return m_PtList.end();
		curPos ++;
		if(curPos == m_PtList.end() )
			curPos = m_PtList.begin();

		curPos ++;
		if(curPos == m_PtList.end() )
			curPos = m_PtList.begin();

		return curPos;
	}

	bool InterSection(const xMathLib::xvec2& l1_pt1 , const xMathLib::xvec2& l1_pt2  , vPtListIterator& _Pos , vPtListIterator& _next)
	{
		xMathLib::xvec2 l2_pt1 = _Pos->m_Pt;
		xMathLib::xvec2 l2_pt2 = _next->m_Pt;

		{
			xvec2 v1 = l2_pt2 - l1_pt1;
			xvec2 v2 = l2_pt1 - l1_pt1;
			xvec2 v  = l1_pt2 - l1_pt1;
			float _v1 = v.cp(v1);
			float _v2 = v.cp(v2);

			if( _v1 * _v2 >= 0 ) //同侧
				return false;
		}

		//换一个位置        
		{
			xvec2 v1 = l1_pt2 - l2_pt1;
			xvec2 v2 = l1_pt1 - l2_pt1;
			xvec2 v  = l2_pt2 - l2_pt1;
			float _v1 = v.cp(v1);
			float _v2 = v.cp(v2);

			if( _v1 * _v2 >= 0 ) //同侧
				return false;
		}
		return true;

	}

	bool InterSection(const xMathLib::xvec2& pt1 , const xMathLib::xvec2& pt2)
	{
		vPtListIterator i = m_PtList.begin();
		vPtListIterator j = m_PtList.end();
		j--;

		for( ; i != m_PtList.end() ; j = i , i ++)
		{
			if( true == InterSection(pt1 , pt2 , i , j) )
				return true;
		}
		return false;

	}

	bool   Inside(const xMathLib::xvec2& P)
	{
		XEVOL_WARNNING_NOT_IMPLEMENT_INFO("XM_Inside is not optimized!\n");
		bool bInside = false;
		vPtListIterator i = m_PtList.begin();
		vPtListIterator j = m_PtList.end();
		j--;

		for( ; i != m_PtList.end() ; j = i , i ++)
		{
			const xMathLib::xvec2& U0 = i->m_Pt;
			const xMathLib::xvec2& U1 = j->m_Pt;
			//y刚好处在U
			if( (U0.y <= P.y && P.y < U1.y )
				||
				(U1.y <= P.y && P.y < U0.y) )
			{
				//看x是不是穿过了 [U0 - U1]
				float x = U0.x + (P.y - U0.y) * (U1.x - U0.x) / (U1.y - U0.y);
				if(x > P.x ) bInside = !bInside;
			}

		}
     	return bInside;
	}

	bool   ToTriangles(const xMathLib::xvec2* ptList , int nPt , ds_vector(xMathLib::xvec3i)& _ret)
	{
		if(ptList == NULL && nPt < 3)
			return false;

		m_ptList = ptList;
		m_nPt = nPt;
		for(int i = 0 ; i < nPt ; i ++)
		{
			_Pt p = {ptList[i] , i };
			m_PtList.push_back( p );
		}
		return ToTriangles(m_PtList.begin() , _ret , 0);
	}

	bool   ToTriangles(vPtListIterator curPos , ds_vector(xMathLib::xvec3i)& _ret , int iRetryTime)
	{
		if(iRetryTime > (int)m_PtList.size() )
		{
			XEVOL_LOG(eXL_ERROR_FALT , "Polygon can not divided to triangles\n");
			return false;
		}
		if(m_PtList.size() == 3)
		{

			vPtListIterator nextPos = NextIterator(curPos);
			vPtListIterator next2Pos = Next2Iterator(curPos);
			xvec3i _tri(curPos->m_idx , nextPos->m_idx , next2Pos->m_idx);
			_ret.push_back(_tri);
			return true;
		}

		xMathLib::xvec2 PtNext ; 
		xMathLib::xvec2 PtThis = curPos->m_Pt;
		xMathLib::xvec2 PtNext2 ;
		int    idxNext  = GetNextPt (curPos , PtNext );
		int    idxNext2 = GetNext2Pt(curPos , PtNext2);
		int    idxCur   = curPos->m_idx;

		xMathLib::xvec2 lnext    =  PtNext2 - PtThis;
		xMathLib::xvec2 _center  = PtNext2 + PtThis;
		_center.x/=2.0f; _center.y/=2.0f;

		//首先，这个点应该在内部。
		if(Inside( _center) )
		{
			//其次，这条线，不能和其它直线有相交
			if(InterSection(PtThis , PtNext2 ) == false)
			{
				iRetryTime = 0;
				xvec3i _tri(idxCur , idxNext , idxNext2);
				_ret.push_back(_tri);
				vPtListIterator nextPos = NextIterator(curPos);
				vPtListIterator next2Pos = Next2Iterator(curPos);
				m_PtList.erase(nextPos);
				return ToTriangles(next2Pos , _ret , iRetryTime);
			}

		}
		//下一个顶点
		vPtListIterator nextPos = NextIterator(curPos);
		iRetryTime ++;
		return ToTriangles(nextPos , _ret , iRetryTime);
	}
};

//////////////////////////////////////////////////////////////////////////
class xTriangulate
{
public:  // xTriangulate a contour/polygon, places results in STL vector
    // as series of triangles.
    //static bool Process(const xvec2* contour, int nPt , ds_vector(xvec2) &result);  
    bool  ToTriangles(const xvec2* contour, int nPt ,  ds_vector(xvec3i) &result);  // compute area of a contour/polygon
    float Area(const xvec2* contour , int nPt);  // decide if point Px/Py is inside triangle defined by
    // (Ax,Ay) (Bx,By) (Cx,Cy)
    bool InsideTriangle(float Ax, float Ay,
        float Bx, float By,
        float Cx, float Cy,
        float Px, float Py);
private:
    bool Snip(const xvec2* contour , int nPt ,int u,int v,int w,int n,int *V);
};



static const float EPSILON=0.0000000001f;

float xTriangulate::Area(const xvec2* contour, int nPt)
{ 
    int n = nPt;  
    float A=0.0f;  
    for(int p=n-1,q=0; q<n; p=q++)
    {
        A+= contour[p].x*contour[q].y - contour[q].x*contour[p].y;
    }
    return A*0.5f;
}  

/*
InsideTriangle decides if a point P is Inside of the triangle
defined by A, B, C.
*/

bool xTriangulate::InsideTriangle(float Ax, float Ay,
                                 float Bx, float By,
                                 float Cx, float Cy,
                                 float Px, float Py)
{
    float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
    float cCROSSap, bCROSScp, aCROSSbp; 
    ax = Cx - Bx;  ay = Cy - By;
    bx = Ax - Cx;  by = Ay - Cy;
    cx = Bx - Ax;  cy = By - Ay;
    apx= Px - Ax;  apy= Py - Ay;
    bpx= Px - Bx;  bpy= Py - By;
    cpx= Px - Cx;  cpy= Py - Cy; 
    aCROSSbp = ax*bpy - ay*bpx;
    cCROSSap = cx*apy - cy*apx;
    bCROSScp = bx*cpy - by*cpx;  
    return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
};

bool xTriangulate::Snip(const xvec2* contour , int nPt,int u,int v,int w,int n,int *V)
{
    int p;
    float Ax, Ay, Bx, By, Cx, Cy, Px, Py; 
    Ax = contour[V[u]].x;
    Ay = contour[V[u]].y;  
    Bx = contour[V[v]].x;
    By = contour[V[v]].y;  
    Cx = contour[V[w]].x;
    Cy = contour[V[w]].y;  
    if ( EPSILON > (((Bx-Ax)*(Cy-Ay)) - ((By-Ay)*(Cx-Ax))) )
        return false; 
    
    for (p=0;p<n;p++)
    {
        if( (p == u) || (p == v) || (p == w) ) 
            continue;
        Px = contour[V[p]].x;
        Py = contour[V[p]].y;
        if (InsideTriangle(Ax,Ay,Bx,By,Cx,Cy,Px,Py))
            return false;
    }  
    return true;
}
//bool xTriangulate::Process(const ds_vector(xvec2) &contour,ds_vector(xvec2) &result)
bool xTriangulate::ToTriangles(const xvec2* contour, int nPt , ds_vector(xvec3i) &result)
{
    /* allocate and initialize list of Vertices in polygon */  
    int n = nPt;
    if ( n < 3 ) 
        return false;  
    int *V = new int[n];  /* we want a counter-clockwise polygon in V */  
    if ( 0.0f < Area(contour , nPt) )
        for (int v=0; v<n; v++) 
            V[v] = v;
    else
        for(int v=0; v<n; v++) 
            V[v] = (n-1)-v;  

    int nv = n;  /*  remove nv-2 Vertices, creating 1 triangle every time */
    int count = 2*nv;   /* error detection */ 
    for(int m=0, v=nv-1; nv>2; )
    {
        /* if we loop, it is probably a non-simple polygon */
        if (0 >= (count--))
        {
            //** xTriangulate: ERROR - probable bad polygon!
            return false;
        }    /* three consecutive vertices in current polygon, <u,v,w> */

        int u = v  ; 
        if (nv <= u)
            u = 0;     /* previous */

        v = u+1; 
        if (nv <= v) 
            v = 0;     /* new v    */

        int w = v+1; 
        if (nv <= w)
            w = 0;     /* next     */   

        if ( Snip(contour , nPt ,u,v,w,nv,V) )
        {
            int a,b,c,s,t;      /* true names of the vertices */
            a = V[u];
            b = V[v];
            c = V[w];      /* output Triangle */
            xvec3i _tri;
            _tri.v[0] = a ;
            _tri.v[1] = b ;
            _tri.v[2] = c ;
            result.push_back(_tri);
            //result.push_back( contour[a] );
            //result.push_back( contour[b] );
            //result.push_back( contour[c] );   
            m++;      /* remove v from remaining polygon */
            for(s=v,t=v+1;t<nv;s++,t++)
                V[s] = V[t]; 
            nv--;      /* resest error detection counter */
            count = 2*nv;
        }
    }  
    delete V;  
    return true;
}

bool   XM_ToTriangles(xMathLib::xvec2* ptList , int nPt , ds_vector(xMathLib::xvec3i)& _ret)
{
	if(nPt < 3  || ptList == NULL)
		return false;

	//初始化列表
    CPolygonToTriangles _cl2Tri ;
	//xTriangulate _cl2Tri ;
	return _cl2Tri.ToTriangles(ptList , nPt , _ret);
}


//////////////////////////////////////////////////////////////////////////
//镶嵌

bool   XM_Tesselation(const XGeomLib::xtriangle* ptList , int nTriangle , int iLevel , ds_vector(XGeomLib::xtriangle)& _ret)
{
	for(int i = 0 ; i < nTriangle ; i ++ ) XM_Tesselation(ptList[i] , iLevel , _ret);
	return true;
}

bool   XM_Tesselation(const XGeomLib::xtriangle& tri , int iLevel , ds_vector(XGeomLib::xtriangle)& _ret)
{
	xvec3 pt1 = tri.m_points[0];
    xvec3 pt2 = tri.m_points[1];
	xvec3 pt3 = tri.m_points[2];

	xvec3 pt4 = (pt1 + pt2) * 0.5f;
	xvec3 pt5 = (pt2 + pt3) * 0.5f;
	xvec3 pt6 = (pt3 + pt1) * 0.5f;
    if(iLevel == 1)
	{
		_ret.push_back( xtriangle(pt1, pt4 , pt6) );
		_ret.push_back( xtriangle(pt4, pt2 , pt5) );
		_ret.push_back( xtriangle(pt5, pt3 , pt6) );
		_ret.push_back( xtriangle(pt4, pt5 , pt6) );
	}
	else
	{
		iLevel --;
        XM_Tesselation(  xtriangle(pt1, pt4 , pt6) , iLevel , _ret);
		XM_Tesselation(  xtriangle(pt4, pt2 , pt5) , iLevel , _ret);
		XM_Tesselation(  xtriangle(pt5, pt3 , pt6) , iLevel , _ret);
		XM_Tesselation(  xtriangle(pt4, pt5 , pt6) , iLevel , _ret);
	}
	return true;
}


//测试顶点是否在内部
_XEVOL_BASE_API_ bool   XM_Inside(const xMathLib::xvec2* ptList , int nPt , const xMathLib::xvec2& P)
{
	XEVOL_WARNNING_NOT_IMPLEMENT_INFO("XM_Inside is not optimized!\n");
	bool bInside = false;
	for(int i = 0 , j = nPt - 1 ; i < nPt ; j = i , i ++)
	{
		const xMathLib::xvec2& U0 = ptList[i];
		const xMathLib::xvec2& U1 = ptList[j];

		//y刚好处在U
		if( (U0.y <= P.y && P.y < U1.y )
			||
			(U1.y <= P.y && P.y < U0.y) )
		{
			//看x是不是穿过了 [U0 - U1]
			float x = U0.x + (P.y - U0.y) * (U1.x - U0.x) / (U1.y - U0.y);
			if(x > P.x ) bInside = !bInside;
		}
	}
	return bInside;
}

END_NAMESPACE_XGEOMLIB

