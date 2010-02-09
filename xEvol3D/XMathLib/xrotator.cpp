#include "../xStdPch.h"
#include "xMathLib.h"

BEGIN_NAMESPACE_XMATHLIB
const xvec4 vRt(1, 0, 0, 1);
const xvec4 vUp(0, 1, 0, 1);
const xvec4 vIn(0, 0, 1, 1);
const xvec4 vOt(0, 0,-1, 1);

xZNormalRotator::xZNormalRotator()
{
    reset();
	m_vCenter = xvec4(0,0,0,1);
	m_bDirty = false;
	m_bValChanged = false;
}
xZNormalRotator::~xZNormalRotator()
{
}

void xZNormalRotator::_calcMatrix()
{
	if(m_bDirty)
	{
		xmat4 mRotX, mRotY, mRotZ, mTran, mTranI, mTmp;
		XM_Rotate(mRotX, vRt, m_vCurAngle.x);
		XM_Rotate(mRotY, vUp, m_vCurAngle.y);
		XM_Rotate(mRotZ, vOt, m_vCurAngle.z);
		m_mMatrix = mRotX * mRotY * mRotZ;
		m_vNormal = vOt * m_mMatrix;
		XM_Transform(mTran, -m_vCenter.x , -m_vCenter.y , -m_vCenter.z);
		XM_Transform(mTranI, m_vCenter.x ,  m_vCenter.y ,  m_vCenter.z);
		//mTmp = mTran * m_mMatrix * mTranI;
		XM_Mul(mTran, m_mMatrix, mTmp);
		XM_Mul(mTmp, mTranI, m_mMatrix);
		m_mMatrix.m[3][2] += m_fZOffset;
		m_bDirty = false;
	}
}

void xZNormalRotator::copyFrom(const xZNormalRotator& roter)
{
	setCenter(roter.getCenter());
	setNormal(roter.getNormal());
	setDepth(roter.getDepth());
	_calcMatrix();
}

xmat4* xZNormalRotator::toMatrix()
{
    _calcMatrix();
	return &m_mMatrix;
}

void   xZNormalRotator::toMatrix(xmat4* mat)
{
	_calcMatrix();
	*mat = m_mMatrix;
}

//xplane xZNormalRotator::getPlane()
//{
//	xvec3 v(m_vCenter.x,m_vCenter.y,m_vCenter.z);
//	return xplane(v, getNormal());
//}

void xZNormalRotator::setNormal(const xvec4& v)
{
	if ( !(m_vNormal == v) )
	{
		if ( numeric_equal(v.len() , 0)  )
		{
			m_vNormal = xvec4(0,0,-1,1);
			m_vCurAngle = xvec3(0,0,0);
		}
		else
		{
			m_vNormal = v;
			m_vNormal.normalize();
			// calc pitch
			if ( numeric_equal(v.x , 0 ) && numeric_equal(v.z , 0 ) )
				m_vCurAngle.x = (m_vNormal.y > 0 ? 90.0f : 270.0f);
			else
			{
				xvec4 vv(0, m_vNormal.y, m_vNormal.z, 1);
				vv.normalize();
				m_vCurAngle.x = XM_ACosD(vUp.dp3(vv));
			}
			// calc yaw
			if ( numeric_equal(v.x , 0) && numeric_equal(v.y  , 0) )
				m_vCurAngle.y = (m_vNormal.z > 0 ? 180.0f : 0.0f);
			else
			{
				xvec4 vv(m_vNormal.x, 0, m_vNormal.z, 1);
				vv.normalize();
				m_vCurAngle.y = XM_ACosD(vOt.dp3(vv));
			}
			
			// calc roll
			if ( numeric_equal(v.y  , 0) && numeric_equal(v.z , 0) )
				m_vCurAngle.z = (m_vNormal.x > 0 ? 90.0f : 270.0f);
			else
			{
				xvec4 vv(m_vNormal.x, m_vNormal.y, 0.0f, 1.0f);
				vv.normalize();
				m_vCurAngle.z = XM_ACosD(vRt.dp3(vv));
			}
		}

		m_bDirty = true;
		m_bValChanged = true;
	}
}

void xZNormalRotator::setDepth(float zOffset)
{
	//if ( m_vCenter.z != zOffset )
	if ( m_fZOffset != zOffset )
	{
		m_fZOffset = zOffset;
		//m_vCenter.z = zOffset;
		m_bDirty = true;
		m_bValChanged = true;
	}
}

void xZNormalRotator::reset()
{
	m_vNormal = xvec4(0,0,-1,1);
	m_vCurAngle = xvec3(0,0,0);
	m_mMatrix.identity();
	m_fZOffset = 0.0f;
	m_bDirty = true;
	m_bValChanged = true;
}

void xZNormalRotator::setCenter(const xvec4& c)
{
	if ( !(m_vCenter == c) )
	{
		m_vCenter = c;
		m_bDirty = true;
		m_bValChanged = true;
	}
}

void xZNormalRotator::set(const xvec4& vNormal, float zOffset)
{
	setNormal(vNormal);
	m_fZOffset = zOffset;
	m_bDirty = true;
	m_bValChanged = true;
}

void xZNormalRotator::setPitch(float fAngle)
{
	while(fAngle >= 360.f) fAngle -= 360.f;
	while(fAngle < 0.f)	fAngle += 360.f;
	if ( m_vCurAngle.x != fAngle )
	{
		m_vCurAngle.x = fAngle;
		m_bDirty = true;
		m_bValChanged = true;
	}
}

void xZNormalRotator::setYaw(float fAngle)
{
	while(fAngle >= 360.f) fAngle -= 360.f;
	while(fAngle < 0.f)	fAngle += 360.f;
	if ( m_vCurAngle.y != fAngle )
	{
		m_vCurAngle.y = fAngle;
		m_bDirty = true;
		m_bValChanged = true;
	}
}

void xZNormalRotator::setRoll(float fAngle)
{
	while(fAngle >= 360.f) fAngle -= 360.f;
	while(fAngle < 0.f)	fAngle += 360.f;
	if ( m_vCurAngle.z != fAngle )
	{
		m_vCurAngle.z = fAngle;
		m_bDirty = true;
		m_bValChanged = true;
	}
}

void xZNormalRotator::pitch(float fStep)
{
	setPitch(getPitch()+fStep);
}

void xZNormalRotator::yaw(float fStep)
{
	setYaw(getYaw()+fStep);
}

void xZNormalRotator::roll(float fStep)
{
	setRoll(getRoll()+fStep);
}

void xZNormalRotator::zoom(float fStep)
{
	setDepth(m_fZOffset+fStep);
}

END_NAMESPACE_XMATHLIB
