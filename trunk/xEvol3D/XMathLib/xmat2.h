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

#ifndef _XMAT2_H_
#define _XMAT2_H_
#include "xmathbasedef.h"

BEGIN_NAMESPACE_XMATHLIB
class _XEVOL_BASE_API_ xmat2
{
public:
    //-----------------------------------------------------
    //
    //-----------------------------------------------------
    xmat2()
    {
        memset((void*) data,0,sizeof(float)*4);
    }

    //-----------------------------------------------------
    //
    //-----------------------------------------------------
    xmat2( float  i )
    {
        memset((void*) data,0,sizeof(float)*4);
        m[0][0] = i;
        m[1][1] = i;
    }

    //-----------------------------------------------------
    //
    //-----------------------------------------------------
    void zero()
    {
        memset((void*) data,0,sizeof(float)*4);
    }

    //-----------------------------------------------------
    //
    //-----------------------------------------------------
    xmat2( float _data[4] )
    {
        for(int  c = 0 ; c < 4 ; c++)
        {
            data[c] = _data[c];
        }
    }

    //-----------------------------------------------------
    //
    //-----------------------------------------------------
    xmat2(xvec2 vec[3],bool isVertical)
    {
        if(isVertical)
        {
            m[0][0] = vec[0].x;
            m[1][0] = vec[0].y;


            m[0][1] = vec[1].x;
            m[1][1] = vec[1].y;
        }
        else
        {
            m[0][0] = vec[0].x;
            m[0][1] = vec[0].y;

            m[1][0] = vec[1].x;
            m[1][1] = vec[1].y;
        }
    }


    //-----------------------------------------------------
    //
    //-----------------------------------------------------
    void identity()
    {
        memset((void*) data,0,sizeof(float)*4);
        m[0][0] = 1;
        m[1][1] = 1;
    }


    //-----------------------------------------------------
    //
    //-----------------------------------------------------
    void scale(float sx,float sy,float sz)
    {
        m[0][0] *= sx ; m[0][1] *= sx ; 
        m[1][0] *= sy ; m[1][1] *= sy ; 
    }

    operator float* (){ return data ; }
    //-----------------------------------------------------
    //
    //-----------------------------------------------------
    bool  inverse()
    {
        float _det = (m00*m11 - m01*m10);
        if(  numeric_equal(_det ,  0.0f ) == true ) 
            return false;
        m00  =  m11/_det; m01 = -m01/_det;
        m10  = -m10/_det; m11 =  m00/_det;
        return true;
    }
public:
    union
    {
        struct{
            float m00,m01;
            float m10,m11;
        };
        float data[4];
        float m[2][2];
    };
};

END_NAMESPACE_XMATHLIB

#endif

