// TestBase.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
using namespace XEvol3D;
class CShrewGameLayer : public IAppLayer
{
public:
    virtual bool           onRenderTick(long passedTime, ISceneVisitor* pVisitor) = 0;
    virtual ISceneGraph*   scene() { return NULL ; }
};
