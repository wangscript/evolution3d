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

#ifndef _X_MATH_BINOMIAL_H_
#define _X_MATH_BINOMIAL_H_
#include "../BaseLib/stl_dllalloc.h"
#include "xMathLib.h"
BEGIN_NAMESPACE_XGEOMLIB
template <typename T , typename _Alloc = std::allocator<T> > class xBinomial
{
public:
	xBinomial(int nPt)
	{
		init(nPt);
	}

	xBinomial()
	{
		m_nRank  = 0 ;
		m_Coeffs = NULL; 
	}

	void init(int nPt)
	{
		m_nRank  = nPt;
		m_Coeffs = m_Alloc.allocate(nPt + 1);
		m_Coeffs[0] = T(1.0);
		for(int i = 1 ; i <= nPt ; i ++)
		{
			m_Coeffs[i] = m_Coeffs[i-1] * (nPt - (i-1) ) / ( (i-1) + 1.0f);
		}
	}

	~xBinomial()
	{
		m_Alloc.deallocate(m_Coeffs , m_nRank);
	}


	T operator[](int t)
	{
        return m_Coeffs[t];
	}

	T operator() (const typename T& a , const typename T& b)
	{
		 T result = 0;
		 double _b = 1;
         for(int i = 0 ; i < m_nRank ; i ++)
		 {
			 result += pow(a  , (m_nRank - i) ) * _b * m_Coeffs[i];
			 _b *= b;              
		 }
		 return result;
	}

public:
	int    m_nRank;
	T*     m_Coeffs; 
	_Alloc m_Alloc;
};
END_NAMESPACE_XGEOMLIB
#endif
