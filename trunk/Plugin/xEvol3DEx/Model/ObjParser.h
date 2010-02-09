#ifndef _OBJPARSER_H
#define _OBJPARSER_H
#include "ModelLoader.h"

// This class holds all the data and function for loading in a .Ase file.
class CLoadOBJ 
{
public:
	typedef vector<ds_wstring>     wstringArray;
public:
	CLoadOBJ();
	~CLoadOBJ();
public:

	// This is the only function the client needs to call to load the .OBJ file
	bool ImportOBJ(t3DModel *pModel, const wchar_t*strFileName);

protected:
	// This is the main loop that parses the .OBJ file
	bool _ReadOBJFile(t3DModel *pModel);

	bool _ParseOBJString(wchar_t *buffer,wstringArray &words,int *cnt);
	bool _HandleFace(wstringArray &words,tFace *face);

	int _GetCurMat(ds_wstring &name,t3DModel *pModel);
	void _LoadMaterialLib(ds_wstring file_name,t3DModel *pModel);

private:
	// This is our file pointer to load the .OBJ file
	FILE       *m_FilePointer;
	ds_wstring  m_strFilePath;
private:
	void _trimLeftString(ds_wstring &_str);
	void _trimRightString(ds_wstring &_str);
	ds_wstring _SpanExcludingString(ds_wstring &_str, const wchar_t& _charSet );
	ds_wstring _getFilePath( ds_wstring strFullName );

};

#endif
