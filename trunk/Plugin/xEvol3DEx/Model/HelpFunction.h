
#ifndef _HELPFUNCTION_H
#define _HELPFUNCTION_H

#include "ModelLoader.h"

struct _VertexIndex
{
	int vi;
	int ti;
	int idx;


};

bool operator < (const _VertexIndex& lhv , const _VertexIndex& rhv);
bool operator == (const _VertexIndex& lhv ,const _VertexIndex& rhv);
int find(std::vector<_VertexIndex>& VIArray , _VertexIndex& idx);

void assign(xvec3& point , const CModelVector3& v);
void assign(xvec2& point , const CModelVector2& v);
bool __cacl_tbn(xvec3* pTangents , t3DObject* pObject , tFace& face);

int _unicodeToMultiByte( const wchar_t * wcsBuffer, char * mbsBuffer, int charCnt );
float ___atof( const wchar_t *ucsBuffer );
int ___atoi(const wchar_t *ucsBuffer);

#endif
