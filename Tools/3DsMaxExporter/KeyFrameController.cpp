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

#include "stdafx.h"
#include <Windows.h>
#include <string>
using namespace std;
#include "XEvolMaxExporter.h"
#include "Resource.h"
#include "XEvolMaxExporter.h"
#include "NodeViewDlg.h"
#include "LogTextDlg.h"
#include "SkeletonSetDlg.h"

void SampleController(INode *n, Control *c);

void KeyTest() 
{
	int i, numKeys;
	INode *         n;
	Control *       c;
	Quat            newQuat, prevQuat;
	IKeyControl*    ikeys;
	ITCBPoint3Key   tcbPosKey;
	ITCBRotKey      tcbRotKey;
	ITCBScaleKey    tcbScaleKey;
	IBezPoint3Key   bezPosKey;
	IBezQuatKey     bezRotKey;
	IBezScaleKey    bezScaleKey;
	ILinPoint3Key   linPosKey;
	ILinRotKey      linRotKey;
	ILinScaleKey    linScaleKey;

	// Get the first node in the selection set
	if (!CMaxEnv::singleton().m_pInterace->GetSelNodeCount()) 
		return;
	n = CMaxEnv::singleton().m_pInterace->GetSelNode(0); 

	// --- Process the position keys ---
	c = n->GetTMController()->GetPositionController();
	ikeys = GetKeyControlInterface(c);

	if (!ikeys) 
	{
		// No interface available to access the keys...
		// Just sample the controller to get the position
		// data at each key...
		SampleController(n, c);
		return; 
	}

	numKeys = ikeys->GetNumKeys();
	DebugPrint(_T("\nThere are %d position key(s)"), numKeys);
	if (c->ClassID() == Class_ID(TCBINTERP_POSITION_CLASS_ID, 0)) 
	{
		for (i = 0; i < numKeys; i++) 
		{
			ikeys->GetKey(i, &tcbPosKey);
			//DebugPrint(_T("\nTCB Position Key: %d=(%.1f, %.1f, %.1f)"), 
			//	i, tcbPosKey.val.x, tcbPosKey.val.y, tcbPosKey.val.z);
		}
	}
	else if (c->ClassID() == Class_ID(HYBRIDINTERP_POSITION_CLASS_ID, 0)) 
	{
		for (i = 0; i < numKeys; i++) 
		{
			ikeys->GetKey(i, &bezPosKey);
			//DebugPrint(_T("\nBezier Position Key: %d=(%.1f, %.1f, %.1f)"), 
			//	i, bezPosKey.val.x, bezPosKey.val.y, bezPosKey.val.z);

		}
	}
	else if (c->ClassID() == Class_ID(LININTERP_POSITION_CLASS_ID, 0)) 
	{
		for (i = 0; i < numKeys; i++) 
		{
			ikeys->GetKey(i, &linPosKey);
			//DebugPrint(_T("\nLinear Position Key: %d=(%.1f, %.1f, %.1f)"), 
			//	i, linPosKey.val.x, linPosKey.val.y, linPosKey.val.z);
		}
	}
	// --- Process the rotation keys ---

	c = n->GetTMController()->GetRotationController();
	ikeys = GetKeyControlInterface(c);
	if (!ikeys) return;
	numKeys = ikeys->GetNumKeys();
	//DebugPrint(_T("\nThere are %d rotation key(s)"), numKeys);
	if (c->ClassID() == Class_ID(TCBINTERP_ROTATION_CLASS_ID, 0)) 
	{
		for (i = 0; i < numKeys; i++) 
		{
			ikeys->GetKey(i, &tcbRotKey);
			newQuat = QFromAngAxis(tcbRotKey.val.angle, tcbRotKey.val.axis);
			if (i) newQuat = prevQuat * newQuat;
			prevQuat = newQuat;
			//DebugPrint(_T("\nTCB Rotation Key: %d=(%.1f, %.1f, %.1f, %.1f)"), 
			//	i, newQuat.x, newQuat.y, newQuat.z, newQuat.w);
		}
	}
	else if (c->ClassID() == Class_ID(HYBRIDINTERP_ROTATION_CLASS_ID, 0)) 
	{
		for (i = 0; i < numKeys; i++) 
		{
			ikeys->GetKey(i, &bezRotKey);
			newQuat = bezRotKey.val;
			if (i) 
				newQuat = prevQuat * newQuat;

			prevQuat = newQuat;
			//DebugPrint(_T("\nBezier Rotation Key: %d=(%.1f, %.1f, %.1f, %.1f)"), 
			//	i, newQuat.x, newQuat.y, newQuat.z, newQuat.w);
		}
	}
	else if (c->ClassID() == Class_ID(LININTERP_ROTATION_CLASS_ID, 0)) 
	{
		for (i = 0; i < numKeys; i++) 
		{
			ikeys->GetKey(i, &linRotKey);
			newQuat = linRotKey.val;
			if (i) 
				newQuat = prevQuat * newQuat;

			prevQuat = newQuat;
			//DebugPrint(_T("\nLinear Rotation Key: %d=(%.1f, %.1f, %.1f, %.1f)"), 
			//	i, newQuat.x, newQuat.y, newQuat.z, newQuat.w);
		}
	}

	// --- Process the scale keys ---
	c = n->GetTMController()->GetScaleController();
	ikeys = GetKeyControlInterface(c);

	if (!ikeys) return;

	numKeys = ikeys->GetNumKeys();
	
	//DebugPrint(_T("\nThere are %d scale key(s)"), numKeys);



	if (c->ClassID() == Class_ID(TCBINTERP_SCALE_CLASS_ID, 0)) 
	{

		for (i = 0; i < numKeys; i++) 
		{
			ikeys->GetKey(i, &tcbScaleKey);
			//DebugPrint(_T("\nTCB Scale Key: %2d=(%.1f, %.1f, %.1f)"), 
			//	i, tcbScaleKey.val.s.x, tcbScaleKey.val.s.y, 
			//	tcbScaleKey.val.s.z);
		}
	}
	else if (c->ClassID() == Class_ID(HYBRIDINTERP_SCALE_CLASS_ID, 0)) 
	{
		for (i = 0; i < numKeys; i++) 
		{
			ikeys->GetKey(i, &bezScaleKey);
			//DebugPrint(_T("\nBezier Scale Key: %2d=(%.1f, %.1f, %.1f)"), 
			//	i, bezScaleKey.val.s.x, bezScaleKey.val.s.y, 
			//	bezScaleKey.val.s.z);
		}
	}
	else if (c->ClassID() == Class_ID(LININTERP_SCALE_CLASS_ID, 0)) 
	{
		for (i = 0; i < numKeys; i++) 
		{
			ikeys->GetKey(i, &linScaleKey);
			DebugPrint(_T("\nLinear Scale Key: %2d=(%.1f, %.1f, %.1f)"), 
				i, linScaleKey.val.s.x, linScaleKey.val.s.y, 
				linScaleKey.val.s.z);
		}
	}
}
// Display the position data of controller in world coordinates for each

// frame in the animation range

void SampleController(INode *n, Control *c)
{
	TimeValue t;
	Point3 trans;
	Matrix3 pmat;
	Interval ivalid;
	int tpf = GetTicksPerFrame();
	int s = CMaxEnv::singleton().m_pInterace->GetAnimRange().Start()/tpf; 
	int e = CMaxEnv::singleton().m_pInterace->GetAnimRange().End()/tpf;
	// Sample the controller at every frame in the anim range
	for (int f = s; f <= e; f++) 
	{
		t = f*tpf;
		ivalid = FOREVER;
		pmat = n->GetParentTM(t);
		c->GetValue(t, &pmat, ivalid, CTRL_RELATIVE);
		trans = pmat.GetTrans();
		//DebugPrint(_T("\nPosition at frame: %d of %d=(%.1f, %.1f, %.1f)"), 
		//              f, e, trans.x, trans.y, trans.z);
	}
}

