#ifndef _MODEL_LOADER_H_
#define _MODEL_LOADER_H_

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <vector>
using namespace std;


//////////// *** NEW *** ////////// *** NEW *** ///////////// *** NEW *** ////////////////////

// This file includes all of the model structures that are needed to load
// in a .3DS file.  If you intend to do animation you will need to add on
// to this.  These structures only support the information that is needed
// to load the objects in the scene and their associative materials.

#define MAX_TEXTURES 100								// The maximum amount of textures to load

// This is our 3D point class.  This will be used to store the vertices of our model.
class CModelVector3 
{
public:
	float x, y, z;
};

// This is our 2D point class.  This will be used to store the UV coordinates.
class CModelVector2 
{
public:
	float x, y;
};

// This is our face structure.  This is is used for indexing into the vertex 
// and texture coordinate arrays.  From this information we know which vertices
// from our vertex array go to which face, along with the correct texture coordinates.
struct tFace
{
	int vertIndex[3];			// indicies for the verts that make up this triangle
	int coordIndex[3];			// indicies for the tex coords to texture this face
};

// This holds the information for a material.  It may be a texture map of a color.
// Some of these are not used, but I left them because you will want to eventually
// read in the UV tile ratio and the UV tile offset for some models.
struct tMaterialInfo
{
	char  strName[255];			// The texture name
	char  strFile[255];			// The texture file name (If this is set it's a texture map)
	BYTE  color[3];				// The color of the object (R, G, B)
	float fColor[3];			// The color of the object (R, G, B) from 0 to 1
	int   texureId;				// the texture ID
	float uTile;				// u tiling of texture  (Currently not used)
	float vTile;				// v tiling of texture	(Currently not used)
	float uOffset;			    // u offset of texture	(Currently not used)
	float vOffset;				// v offset of texture	(Currently not used)
} ;

// This holds all the information for our model/scene. 
// You should eventually turn into a robust class that 
// has loading/drawing/querying functions like:
// LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
struct t3DObject 
{
	int  numOfVerts;			// The number of verts in the model
	int  numOfFaces;			// The number of faces in the model
	int  numTexVertex;			// The number of texture coordinates
	int  materialID;			// The texture ID to use, which is the index into our texture array
	bool bHasTexture;			// This is TRUE if there is a texture map for this object
	char strName[255];			// The name of the object
	CModelVector3  *pVerts;			// The object's vertices
	CModelVector3  *pNormals;		// The object's normals
	CModelVector2  *pTexVerts;		// The texture's UV coordinates
	tFace *pFaces;				// The faces information of the object
};

// This holds our model information.  This should also turn into a robust class.
// We use STL's (Standard Template Library) vector class to ease our link list burdens. :)
struct t3DModel 
{
	int numOfObjects;					// The number of objects in the model
	int numOfMaterials;					// The number of materials for the model
	vector<tMaterialInfo> pMaterials;	// The list of material information (Textures and colors)
	vector<t3DObject> pObject;			// The object list for our model
};


// This calculates a vector between 2 points and returns the result
CModelVector3 Vector(CModelVector3 vPoint1, CModelVector3 vPoint2);
CModelVector3 AddVector(CModelVector3 vVector1, CModelVector3 vVector2);
CModelVector3 DivideVectorByScaler(CModelVector3 vVector1, float Scaler);
CModelVector3 Cross(CModelVector3 vVector1, CModelVector3 vVector2);
CModelVector3 Normalize(CModelVector3 vNormal);
#endif 




