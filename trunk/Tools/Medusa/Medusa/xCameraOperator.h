#pragma once
#include "xEvolEnviroment.h"
using namespace XEvol3D;

enum eCameraState 
{
    eCS_Top,
    eCS_2D ,
    eCS_3D ,
    eCS_Free,
};

class xCameraOperator : public ICameraOperator
{
	IRenderCamera*                m_pCamera;
    std::wstring                  m_Info;
	xvec2 m_ptLastMouse;
public:
	xCameraOperator();
	~xCameraOperator();

public:
    bool             onMessage(xWindowMsg& _msg);
    const wchar_t*   cameraInfo();
     bool            onActive();
     bool            onDeActive();
     void            setCamera( IRenderCamera* pCamera );
     const wchar_t*  name();
     void            updateTick(float _tPassedTime);
public:


	/// operator
	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();

	void moveUp();
	void moveDown();

	void rotLeft();
	void rotRight();
	void rotUp();
	void rotDown();

	void onLButtonDown( int x , int y );
	void onLButtonUp( int x , int y );
	void onMouseMove( int x , int y );
	void onMouseWheel( float fDelta );
private:
	void _moveForward( float fDis );
	void _shift(float fDis);

protected:
    eCameraState m_State;
};