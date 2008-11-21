#ifndef __XMATH_ROTATOR_H__
#define __XMATH_ROTATOR_H__

#include "xmathbasedef.h"

BEGIN_NAMESPACE_XMATHLIB
class _XEVOL_BASE_API_ xtransformer
{
public:
	virtual xmat4* toMatrix() = 0;
	virtual void   toMatrix(xmat4* mat) = 0;
};

class _XEVOL_BASE_API_ xZNormalRotator : xtransformer
{
	xmat4   m_mMatrix;
	xvec4   m_vCenter;
	xvec4   m_vNormal;
	xvec3   m_vCurAngle;
	float   m_fZOffset;
	bool    m_bDirty;
	bool    m_bValChanged;
	void	_calcMatrix();
public:
	xZNormalRotator();
	~xZNormalRotator();
	// set normal
	void setNormal(const xvec4& v);
	// set angle on axis X
	void setPitch(float fAngle);
	// set angle on axis Y
	void setYaw(float fAngle);
	// set angle on axis Z
	void setRoll(float fAngle);
	// set the Z depth
	void setDepth(float zOffset);
	void setCenter(const xvec4& c);
	void pitch(float fStep);
	void yaw(float fStep);
	void roll(float fStep);
	void zoom(float fStep);
	void reset();
	void set(const xvec4& vNormal, float zOffset);
public:
	virtual xmat4* toMatrix() ;
	virtual void   toMatrix(xmat4* mat);
	void	copyFrom(const xZNormalRotator& roter);
	//xplane getPlane();
	inline xvec4 getCenter() const{
		return m_vCenter;
	}
	inline xvec4 getNormal() const{
		return m_vNormal;
	}
	inline float getPitch() const{
		return m_vCurAngle.x;
	}
	inline float getYaw() const{
		return m_vCurAngle.y;
	}
	inline float getRoll() const{
		return m_vCurAngle.z;
	}
	inline xvec3 getAngle() const{
		return m_vCurAngle;
	}
	inline float getDepth() const{
		return m_fZOffset;
	}
	inline bool pickChanging(){
		bool b = m_bValChanged;
		m_bValChanged = false;
		return b;
	}
};

END_NAMESPACE_XMATHLIB
#endif
