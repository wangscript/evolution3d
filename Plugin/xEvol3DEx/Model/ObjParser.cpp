//***********************************************************************//
//																		 //
//		-  -		 //
//                                                                       //
//		$Author:		Rock Mao		mhm15191@msn.com			 //
//																		 //
//		$Program:		OBJ Loader										 //
//																		 //
//		$Description:	Demonstrates how to load a .Obj file format		 //
//																		 //
//		$Date:			12/10/09										 //
//																		 //
//***********************************************************************//
#include "../stdafx.h"
#include "ObjParser.h"
#include "HelpFunction.h"
#define MAX_STRINGLENGTH 255

USING_NS_XEVOL3D;

///////////////////////////////// IMPORT OBJ \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function is called to load in an .obj file by the file name
/////
///////////////////////////////// IMPORT OBJ \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*


CLoadOBJ::CLoadOBJ()
{
	m_FilePointer = NULL;
	m_strFilePath = L"";
}


CLoadOBJ::~CLoadOBJ()
{

}


bool CLoadOBJ::ImportOBJ(t3DModel *pModel, const wchar_t* strFileName)
{

	// Make sure we have a valid model and file name
	if(!pModel || !strFileName) return false;

	m_strFilePath = _getFilePath( strFileName );
	// Here we open the desired file for read only and return the file pointer
	m_FilePointer = _wfopen(strFileName, L"r");

	// Check to make sure we have a valid file pointer
	if(!m_FilePointer)
	{
		// Create an error message for the attempted file
		XEVOL_LOG(eXL_DEBUG_HIGH, L"Unable to find or open the file: %s\n", strFileName);
		return false;
	}

	// Now that we have a valid file and it's open, let's read in the info!
	_ReadOBJFile(pModel);

	// Now that we have the file read in, let's compute the vertex normals for lighting
	//	ComputeNormals(pModel);

	// Close the .OBJ file that we opened
	fclose(m_FilePointer);

	// Return a success!
	return true;
}

bool CLoadOBJ::_ReadOBJFile(t3DModel *pModel)
{
	pModel->numOfObjects = 0;

	/// Local Variables ///////////////////////////////////////////////////////////
	int cnt;
	wchar_t buffer[MAX_STRINGLENGTH];

	wstringArray words;
	ds_wstring   temp;
	long vCnt = 0, nCnt = 0, tCnt = 0, fCnt = 0;
	long vPos = 0, nPos = 0, tPos = 0, fPos = 0;
	tFace *face = NULL;
	int	   curMat = 0;
	float vertexScale = 1.0;

	t3DObject pObject;


	pModel->numOfObjects += 1;
	//visual->glTex = 0;
	if (m_FilePointer != NULL)
	{
		// first pass sets up the number of objects in the file
		while (!feof(m_FilePointer))
		{
			fgetws(buffer,MAX_STRINGLENGTH,m_FilePointer);	// get a string from the file
			_ParseOBJString(buffer,words,&cnt);	// break the string into cnt words
			if (cnt > 0)						// make sure some words are there
			{
				temp = words[0];			// check the first work
				if (temp.length() > 0)
				{
					if (temp[0] == L'v')			// only look at words that start with v
					{
						if (temp.length() > 1 && temp[1] == L'n')			// vn is a normal
							nCnt++;
						else if (temp.length() > 1 && temp[1] == L't')	// vt is a texture 
							tCnt++;
						else
							vCnt++;											// v is a vertex
					}
					else if (temp[0] == L'f')
						fCnt++;												// f is a face
					else if (temp == L"mtllib")  // handle the material library
					{
						_LoadMaterialLib(words[1],pModel);
					}
				}
			}
			words.clear();		// clear word buffer
		}


		// Now that i know how many, allocate room for it
		if (vCnt > 0)
		{
			pObject.pVerts = (CModelVector3 *)malloc(vCnt * sizeof(CModelVector3));
			//visual->deformData = (tVector *)malloc(sizeof(tVector) * vCnt);
			if (nCnt > 0)
			{
				pObject.pNormals = (CModelVector3 *)malloc(nCnt * sizeof(CModelVector3));
			}
			if (tCnt > 0)
			{
				pObject.pTexVerts = (CModelVector2 *)malloc(tCnt * sizeof(CModelVector2));
			}
			if (fCnt > 0)
			{
				pObject.pFaces = (tFace *)malloc(fCnt * sizeof(tFace));
			}
			fseek(m_FilePointer,0,SEEK_SET);

			// now that it is all alloc'ed.  grab the real data
			while (!feof(m_FilePointer))
			{
				fgetws(buffer,MAX_STRINGLENGTH,m_FilePointer);
				_ParseOBJString(buffer,words,&cnt);
				if (cnt > 0)
				{
					temp = words[0];
					if (temp.length() > 0)
					{
						if (temp[0] == L'v')		// words starting with v
						{
							if (temp.length() > 1 && temp[1] == L'n')	// vn normals
							{
								pObject.pNormals[nPos].x = ___atof(words[1].c_str());
								pObject.pNormals[nPos].y = ___atof(words[2].c_str());
								pObject.pNormals[nPos].z = ___atof(words[3].c_str());
								nPos++;
							}
							else if (temp.length() > 1 && temp[1] == L't')	// vt textures
							{
								pObject.pTexVerts[tPos].x = ___atof(words[1].c_str());
								pObject.pTexVerts[tPos].y = ___atof(words[2].c_str());
								tPos++;
							}
							else											// vertices
							{
								pObject.pVerts[vPos].x = ___atof(words[1].c_str()) * vertexScale;
								pObject.pVerts[vPos].y = ___atof(words[2].c_str()) * vertexScale;
								pObject.pVerts[vPos].z = ___atof(words[3].c_str()) * vertexScale;
								vPos++;
							}
						}
						else if (temp[0] == L'f')			// f v/t/n v/t/n v/t/n	face line
						{
							if (words.size() > 4)
							{
								wsprintf(buffer,L"Face %d has more than 3 vertices",fPos);
								MessageBoxW(NULL,buffer,L"ERROR",MB_OK);
							}
							_HandleFace( words,&pObject.pFaces[fPos]);
							//pObject.pFaces[fPos].mat = curMat;
							fPos++;
						}
						else if (temp == L"usemtl")			// f v/t/n v/t/n v/t/n	FACE LINE
						{
							curMat = _GetCurMat(words[1],pModel);
						}
						else if (temp == L"scale")			// f v/t/n v/t/n v/t/n	FACE LINE
						{
							vertexScale = ___atof(words[1].c_str());
						}
					}
				}
				words.clear();		// clear word buffer
			}
			pObject.numOfVerts   = vCnt;// Set the vertex count
			pObject.numTexVertex = tCnt;// Set the texture coord count
			pObject.numOfFaces   = fCnt;// Set the face count
			pObject.materialID   = curMat;
		}

		fclose(m_FilePointer);

	}
	else
		return false;

	pModel->pObject.push_back(pObject);
	return true;



}


bool CLoadOBJ::_ParseOBJString(wchar_t *buffer,wstringArray &words,int *cnt)
{
	/// Local Variables ///////////////////////////////////////////////////////////
	ds_wstring _in = buffer;
	ds_wstring temp;
	///////////////////////////////////////////////////////////////////////////////

	_trimLeftString( _in );
	_trimRightString( _in );

	*cnt = 0;
	do 
	{
		temp = _SpanExcludingString( _in, L' ');// get up to the next space or tab
		//temp = in.SpanExcluding(" \t");
		words.push_back( temp );
		if (temp == _in ) 
			break;

		_in = _in.substr( temp.length(), _in.length() - temp.length() );
		//in = in.Right(in.GetLength() - temp.GetLength());
		_trimLeftString( _in );

		//in.TrimLeft();
		*cnt = *cnt + 1;			
	} while (1);
	*cnt = *cnt + 1;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Procedure:	HandleFace
// Purpose:		Handles the Face Line in an OBJ file.  Extracts index info to 
//				a face Structure
// Arguments:	Array of words from the face line, place to put the data
// Notes:		Not an Official OBJ loader as it doesn't handle anything other than
//				3-4 vertex polygons.
///////////////////////////////////////////////////////////////////////////////	
bool CLoadOBJ::_HandleFace(wstringArray &words,tFace *face)
{	
	/// Local Variables ///////////////////////////////////////////////////////////
	int loop,loopcnt;
	ds_wstring temp;
	ds_wstring vStr,nStr,tStr;		// HOLD POINTERS TO ELEMENT POINTERS
	int nPos,tPos;
	///////////////////////////////////////////////////////////////////////////////
	loopcnt = words.size();
	if (loopcnt > 4)
		loopcnt = 4;

	// loop through the 3 - 4 words of the facelist line, word 0 has 'f'
	for (loop = 1; loop < loopcnt; loop++)
	{
		temp = words[loop];			// grab the next word
		// face data is in the format vertex/texture/normal
		tPos = temp.find('/');				// find the '/' separating vertex and texture
		vStr = temp.substr(0, tPos);				// get the vertex number
		temp[tPos] = L' ';                                 // change the '/' to a space so i can try again
		//temp.SetAt(tPos,' ');				
		nPos = temp.find('/');				// find the '/' separating texture and normal
		tStr = temp.substr( tPos + 1, nPos - tPos - 1);
		//tStr = temp.Mid(tPos + 1, nPos - tPos - 1);		// get the texture number

		int __rightCnt = temp.length() - nPos - 1;
		int __rightPos = temp.length() - __rightCnt;
		nStr = temp.substr( __rightPos, __rightCnt );
		//nStr = temp.Right(temp.length() - nPos - 1);	// get the normal number
		//face->vertIndex
		face->vertIndex[loop - 1] = ___atoi(vStr.c_str()) - 1;		// store off the index for the vertex
		face->coordIndex[loop - 1] = ___atoi(tStr.c_str()) - 1;		// store off the index for the texture
		//face->n[loop - 1] = ___atoi(nStr.c_str()) - 1;		// store off the index for the normal
	}



	return true;
}


int CLoadOBJ::_GetCurMat(ds_wstring &name,t3DModel *pModel)
{
	int matCnt = pModel->pMaterials.size();
	for (int loop = 0; loop < matCnt; loop++)
	{
		tMaterialInfo &_matInfo = pModel->pMaterials[loop];

		char mbsBuffer[255]={0};
		_unicodeToMultiByte(name.c_str(), mbsBuffer, 255 );

		if ( 0 == strcmp( mbsBuffer,_matInfo.strName ))
			return loop;
	}
	return 0;
}


void CLoadOBJ::_LoadMaterialLib(ds_wstring file_name,t3DModel *pModel)
{
	/// Local Variables ///////////////////////////////////////////////////////////

	ds_wstring name = m_strFilePath;
	name += file_name;
	int cnt;
	wchar_t buffer[MAX_STRINGLENGTH];
	wstringArray words;
	ds_wstring   temp;
	FILE *fp;
	int matCnt = 0,curMat = -1;
	///////////////////////////////////////////////////////////////////////////////
	//strcpy(visual->map,"");
	fp = _wfopen(name.c_str(),L"r");
	if (fp != NULL)
	{
		// first pass sets up the number of objects in the file
		while (!feof(fp))
		{
			fgetws(buffer,MAX_STRINGLENGTH,fp);	// get a string from the file
			_ParseOBJString(buffer,words,&cnt);	// break the string into cnt words
			if (cnt > 0)						// make sure some words are there
			{
				temp = words[0];			// check the first work
				if (temp.length() > 0)
				{
					if (temp == L"newmtl")			// ambient
					{
						matCnt++;
					}
				}
			}
			words.clear();		// clear word buffer
		}

		tMaterialInfo _matInfo;
		pModel->pMaterials.push_back( _matInfo );
		fseek(fp,0,SEEK_SET);
		// Get Data
		while (!feof(fp))
		{
			fgetws(buffer,MAX_STRINGLENGTH,fp);	// get a string from the file
			_ParseOBJString(buffer,words,&cnt);	// break the string into cnt words
			if (cnt > 0)						// make sure some words are there
			{
				temp = words[0];			// check the first work
				if (temp.length() > 0)
				{
					if (temp == L"newmtl") // ambient
					{
						curMat++;
						char mbsBuffer[255]={0};
						_unicodeToMultiByte(name.c_str(), mbsBuffer, 255 );

						strncpy(_matInfo.strName,mbsBuffer,255);

					}
					else if (temp == L"Ka")			// ambient
					{

					}
					else if (temp == L"Kd")		// diffuse color
					{

					}
					else if (temp == L"Ks")		// specular color
					{

					}
					else if (temp == L"Ns")		// specular coefficient
					{

					}
					else if (temp == L"map_Kd")	// texture map name
					{
						char mbsBuffer[255]={0};
						_unicodeToMultiByte(name.c_str(), mbsBuffer, 255 );

						strncpy(_matInfo.strName,mbsBuffer,255);
					}
					else if (temp == L"map_Ks")	// texture map name
					{

					}
				}
			}
			words.clear();		// clear word buffer
		}
		fclose(fp);
	}

	pModel->numOfMaterials = matCnt;
}


void CLoadOBJ::_trimLeftString(ds_wstring &_str)
{
	int _nPos = _str.find_first_not_of(L' ');
	if ( _nPos > 0 )
	{
		_str = _str.substr( _nPos, _str.length() - _nPos + 1 );
	}

}


void CLoadOBJ::_trimRightString(ds_wstring &_str)
{
	//int _nPos = _str.find_last_not_of()
	if ( _str.length() < 2 )
		return;
	wchar_t __tail = _str.at( _str.length() - 2);
	while( L' ' == __tail )
	{
		_str = _str.substr( 0, _str.length() - 2);
		__tail = _str.at( _str.length() - 2);
	}

}

ds_wstring CLoadOBJ::_SpanExcludingString(ds_wstring &_str, const wchar_t& _charSet )
{
	ds_wstring ___strOut;
	int _nPos = _str.find_first_of( _charSet );
	___strOut = _str.substr( 0, _nPos );
	return ___strOut;
}


ds_wstring CLoadOBJ::_getFilePath( ds_wstring strFullName )
{
	ds_wstring strPath;
	int _nPos = strFullName.find_last_of( L'\\');

	if ( _nPos > 0 )
	{
		strPath = strFullName.substr(0, _nPos + 1 );
	}
	return strPath;
}
