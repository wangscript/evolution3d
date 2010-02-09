#include "../stdafx.h"
#include "HelpFunction.h"

bool operator < (const _VertexIndex& lhv , const _VertexIndex& rhv)
{
	if( lhv.vi < rhv.vi)
		return true;
	if( lhv.vi > rhv.vi)
		return false;

	if( lhv.ti < rhv.ti)
		return true;
	return false;
}

bool operator == (const _VertexIndex& lhv ,const _VertexIndex& rhv)
{
	if( lhv.vi == rhv.vi ||  lhv.ti == rhv.ti)
		return true;
	return false;
}

int find(std::vector<_VertexIndex>& VIArray , _VertexIndex& idx)
{
	size_t nVertex = VIArray.size();
	for(size_t i = 0 ; i < nVertex ; i ++)
	{
		if(VIArray[i] == idx)
			return (int)i;
	}
	return -1;
}
void assign(xvec3& point , const CModelVector3& v)
{
	point.x = v.x;
	point.y = v.y;
	point.z = v.z;
}
void assign(xvec2& point , const CModelVector2& v)
{
	point.x = v.x;
	point.y = v.y;
}

bool __cacl_tbn(xvec3* pTangents , t3DObject* pObject , tFace& face)
{
	xvec3  normal[3];
	xvec3  Tangent; 
	xvec3  p[3];

	assign(normal[0],pObject->pNormals[face.vertIndex[0]]);
	assign(normal[1],pObject->pNormals[face.vertIndex[1]]);
	assign(normal[2],pObject->pNormals[face.vertIndex[2]]);


	assign(p[0],pObject->pVerts[face.vertIndex[0]]);
	assign(p[1],pObject->pVerts[face.vertIndex[1]]);
	assign(p[2],pObject->pVerts[face.vertIndex[2]]);



	xvec2 uv[3];
	assign(uv[0] , pObject->pTexVerts[face.coordIndex[0]]);
	assign(uv[1] , pObject->pTexVerts[face.coordIndex[1]]);
	assign(uv[2] , pObject->pTexVerts[face.coordIndex[2]]);

	xvec3  e1 = p[1] - p[0];
	xvec3  e2 = p[2] - p[0];
	xvec2 u1 ( uv[1].s - uv[0].s , uv[1].t - uv[0].t );
	xvec2 u2 ( uv[2].s - uv[0].s , uv[2].t - uv[0].t );

	float det =  ( u1.s * u2.t - u2.s * u1.t);
	if(det == 0.0f)
	{
		Tangent = e1;
	}
	else
	{
		Tangent = u2.t * e1 - u1.t * e2;
	}

	//从Normal 和 Tangent里重新计算出Tangent,因为面的Tangent和顶点的Normal可能不垂直
	xvec3 final_tangent;
	for(int i = 0 ;i < 3 ; ++i)
	{
		xvec3 binormal  = normal[i].cp(Tangent);
		final_tangent   = binormal.cp(normal[i]);
		final_tangent.normalize();
		pTangents[face.vertIndex[i]].x += final_tangent.x;
		pTangents[face.vertIndex[i]].y += final_tangent.y;
		pTangents[face.vertIndex[i]].z += final_tangent.z;

	}
	return true;
}


int _unicodeToMultiByte( const wchar_t * wcsBuffer, char * mbsBuffer, int charCnt )
{
#ifdef _WIN32
	BOOL bUsedDefChar = FALSE;
	WideCharToMultiByte(CP_ACP , 0 , wcsBuffer , (int)wcslen(wcsBuffer) , mbsBuffer , charCnt , NULL, &bUsedDefChar);
#else

#endif
	return charCnt;
}


float ___atof( const wchar_t *ucsBuffer )
{
	float _f;
	char mbsBuffer[128]={0};
	_unicodeToMultiByte(ucsBuffer, mbsBuffer, 128 );
	_f = atof(mbsBuffer);

	return _f;

}

int ___atoi(const wchar_t *ucsBuffer)
{
	int _f;
	char mbsBuffer[128]={0};
	_unicodeToMultiByte(ucsBuffer, mbsBuffer, 128 );

	_f = atoi(mbsBuffer);

	return _f;
}