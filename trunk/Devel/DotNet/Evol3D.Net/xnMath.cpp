#include "stdafx.h"
#include <vcclr.h>

#include "xnMath.h"

namespace xEvol3DNet 
{

	//Vec3
	xVector3^ xVector3::cp(const xVector3^ v1) 
	{
		xVector3^ vN = gcnew xVector3();
		*vN->m_vec = m_vec->cp( *v1->m_vec );
		return vN;
	}

	//Vec4
	xVector^ xVector::cp(const xVector^ v1) 
	{
		xVector^ vN = gcnew xVector();
		*vN->m_vec = m_vec->cp( *v1->m_vec );
		return vN;
	}

	xVector^ xVector::getNormal() 
	{
		xVector^ vN = gcnew xVector();
		*vN->m_vec = m_vec->getNormal();
		return vN;
	}
}
