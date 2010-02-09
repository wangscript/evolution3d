#ifndef _DXFPARSER_H
#define _DXFPARSER_H
#include "ModelLoader.h"

// This class holds all the data and function for loading in a .Ase file.
class CLoadDxf
{
public:
	typedef vector<ds_wstring>     wstringArray;
public:
	CLoadDxf();
	~CLoadDxf();
public:

	// This is the only function the client needs to call to load the .OBJ file
	bool ImportDxf(t3DModel *pModel, const wchar_t*strFileName);

protected:
	// This is the main loop that parses the .OBJ file
	bool _ReadDxfFile(t3DModel *pModel);
private:
	// This is our file pointer to load the .OBJ file
	FILE *m_FilePointer;
};

#endif
