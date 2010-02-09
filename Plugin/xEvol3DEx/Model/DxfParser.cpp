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
#include "DxfParser.h"
#include "HelpFunction.h"
#define MAX_STRINGLENGTH 255

USING_NS_XEVOL3D;

///////////////////////////////// IMPORT OBJ \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function is called to load in an .obj file by the file name
/////
///////////////////////////////// IMPORT OBJ \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CLoadDxf::CLoadDxf()
{
	m_FilePointer = NULL;
}


CLoadDxf::~CLoadDxf()
{
	
}


bool CLoadDxf::ImportDxf(t3DModel *pModel, const wchar_t* strFileName)
{

	// Make sure we have a valid model and file name
	if(!pModel || !strFileName) return false;

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
	_ReadDxfFile(pModel);

	// Now that we have the file read in, let's compute the vertex normals for lighting
//	ComputeNormals(pModel);

	// Close the .OBJ file that we opened
	fclose(m_FilePointer);

	// Return a success!
	return true;
}


bool CLoadDxf::_ReadDxfFile(t3DModel *pModel)
{
	
	return true;
}