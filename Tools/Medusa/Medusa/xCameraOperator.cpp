#include "Stdafx.h"
#include "xCameraOperator.h"
#include "xMedusaEditor.h"
#include "./Editor/EditorEnv.h"
USING_NS_MDED

xCameraOperator::xCameraOperator()
{
	m_pCamera = NULL;
    m_State   = eCS_Free;
    m_Info    = L"";
}
const wchar_t*  xCameraOperator::name()
{
    return L"default";
}
void xCameraOperator::updateTick(float _tPassedTime)
{
    return ;
}

bool xCameraOperator::onMessage(xWindowMsg& msg)
{
    if(msg.MsgID == WIN_MOUSE_WHEEL)
    {
        onMouseWheel( msg.Mouse.nValue );
        return true;
    }
    else if(msg.MsgID == WIN_MOUSE_MOVE)
    {
        onMouseMove( msg.Mouse.x, msg.Mouse.y );
        return true;
    }
    else if(msg.MsgID == WIN_KEYDOWN)
    {
        float fCameraStep = m_pCamera->step();
        if(msg.Keyboard.cKeyChar == 'a' )
        {
            moveLeft();
        }
        else if(msg.Keyboard.cKeyChar == 'd')
        {
            moveRight();

        }
        else if(msg.Keyboard.cKeyChar == 'w')
        {
            moveForward();
        }
        else if(msg.Keyboard.cKeyChar == 's')
        {
            moveBack();
        }
        else if(msg.Keyboard.nKey == VKEY_UP)
        {
            rotUp();
        }
        else if(msg.Keyboard.nKey == VKEY_DOWN)
        {
            rotDown();
        }
        if(msg.Keyboard.nKey == VKEY_LEFT)
        {
            rotLeft();
        }
        else if(msg.Keyboard.nKey == VKEY_RIGHT)
        {
            rotRight();
        }
        else if(msg.Keyboard.nKey == VKEY_PRIOR)
        {
            moveUp();
        }
        else if(msg.Keyboard.nKey == VKEY_NEXT)
        {
            moveDown();
        }
        else 
        {
            return false;
        }
        return true;
    }
    return false;
}

xCameraOperator::~xCameraOperator()
{

}
void xCameraOperator::setCamera( IRenderCamera* pCamera)
{
    m_pCamera = pCamera; 
}

const wchar_t*  xCameraOperator::cameraInfo()
{
    if(m_State == eCS_2D || m_State == eCS_Top )
        m_Info = L"2D顶视图";
    if(m_State == eCS_3D )
        m_Info = L"漫游视角";
    return m_Info.c_str();
}


bool xCameraOperator::onActive()
{
    return true;
}

bool xCameraOperator::onDeActive()
{
    return true;
}

void xCameraOperator::_shift(float fDis)
{
	xGeomLib::xCamera::XEye eye = m_pCamera->getEye();
	xvec4 vDir  = eye.m_EyeTarget - eye.m_EyePos;
	xvec4 vLeft = vDir.cp(eye.m_Up);
	vLeft.normalize();
	vLeft *= fDis;

	eye.m_EyePos.x += vLeft.x;
	eye.m_EyePos.y += vLeft.y;
	eye.m_EyePos.z += vLeft.z;

	eye.m_EyeTarget.x += vLeft.x;
	eye.m_EyeTarget.y += vLeft.y;
	eye.m_EyeTarget.z += vLeft.z;	
	m_pCamera->setEye( eye );

	GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

}


void xCameraOperator::_moveForward( float fDis )
{

	xGeomLib::xCamera::XEye eye = m_pCamera->getEye();
	xvec4 pos = eye.m_EyePos;
	xvec4 vDir = eye.m_EyeTarget - eye.m_EyePos;

	float fCameraStep = m_pCamera->step();


	//if ( fDelta > 65000 )
	//	fCameraStep *= -1;
	//else
	//	fCameraStep *= fDelta/120;

	vDir.normalize();
	vDir *= fDis;

	eye.m_EyePos.x += vDir.x;
	eye.m_EyePos.y += vDir.y;
	eye.m_EyePos.z += vDir.z;

	eye.m_EyeTarget.x += vDir.x;
	eye.m_EyeTarget.y += vDir.y;
	eye.m_EyeTarget.z += vDir.z;
	m_pCamera->setEye( eye );

	GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

}

void xCameraOperator::moveForward()
{
	float fCameraStep = m_pCamera->step();
	_moveForward(fCameraStep);
}
void xCameraOperator::moveBack()
{
	float fCameraStep = m_pCamera->step();
	_moveForward(fCameraStep*-1);
}

void xCameraOperator::moveLeft()
{
	float fCameraStep = m_pCamera->step();
	_shift(-fCameraStep);
}

void xCameraOperator::moveRight()
{
	float fCameraStep = m_pCamera->step();
	_shift(fCameraStep);
}


void xCameraOperator::moveUp()
{
		float fCameraStep = m_pCamera->step();
	m_pCamera->upDown(fCameraStep);
	GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

}

void xCameraOperator::moveDown()
{
		float fCameraStep = m_pCamera->step();
	m_pCamera->upDown(-fCameraStep);
	GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

}



void xCameraOperator::rotLeft()
{
	m_pCamera->yaw(2);
	GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

}


void xCameraOperator::rotRight()
{
	m_pCamera->yaw(-2);
	GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

}

void xCameraOperator::rotUp()
{
	m_pCamera->pitch(2);
	GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

}

void xCameraOperator::rotDown()
{
	m_pCamera->pitch(-2);
	GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

}


void xCameraOperator::onLButtonDown( int x , int y )
{

}

void xCameraOperator::onLButtonUp( int x , int y )
{

}


void xCameraOperator::onMouseMove( int x , int y )
{
	if(GetKeyState(VK_RBUTTON)   &   0x80)   
	{   
		//   此时鼠标左键被按下   
		if ( x < m_ptLastMouse.x )
			m_pCamera->circle( -0.02);
		else
			m_pCamera->circle( 0.02);

	}  
	GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_CameraChange , (int)m_pCamera);

	m_ptLastMouse.x = x;
	m_ptLastMouse.y = y;
}

void xCameraOperator::onMouseWheel( float fDelta )
{
	xGeomLib::xCamera::XEye eye = m_pCamera->getEye();
	xvec4 pos = eye.m_EyePos;
	xvec4 vDir = eye.m_EyeTarget - eye.m_EyePos;

	float fCameraStep = m_pCamera->step();

	
	if ( fDelta > 65000 )
		fCameraStep *= -1;
	else
		fCameraStep *= fDelta/120;
	
	vDir.normalize();
	vDir *= fCameraStep;

	eye.m_EyePos.x += vDir.x;
	eye.m_EyePos.y += vDir.y;
	eye.m_EyePos.z += vDir.z;

	//eye.m_EyeTarget.x += vDir.x;
	//eye.m_EyeTarget.y += vDir.y;
	//eye.m_EyeTarget.z += vDir.z;
	m_pCamera->setEye( eye );

}