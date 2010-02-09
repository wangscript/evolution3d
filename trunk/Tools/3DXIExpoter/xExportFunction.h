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

#ifndef __MAX_MODEL_EXPORTER_H__
#define __MAX_MODEL_EXPORTER_H__
#include "MaxNodeTree.h"
#include "MaxMesh.h"
class CModelBaseExportor
{
protected:
	CModelBaseExportor(){   }
	bool _export_svectorsAsXML(sVectors_t& vec , xcomdoc& doc , std::wstring mesh_dir , std::wstring stream_name , std::wstring rootNode_name , int compress_rate);
	bool _export_svectors(sVectors_t& vec , xcomdoc& doc , std::wstring mesh_dir , std::wstring stream_name , int compress_rate);
protected:
	bool exportMeshDescToStream( std::wstring mesh_dir , xcomdoc& doc , xMeshData& meshData, int nFrame , sSkeletonID_t sid);
	bool exportFaceToStream( std::wstring mesh_dir , xcomdoc& doc , xMeshData& meshData);
	bool exportMaterialToStream(std::wstring mesh_dir , xcomdoc& doc , xMeshData& meshData);
	bool exportWeightToStream(std::wstring mesh_dir , xcomdoc& doc , xMeshData& meshData, int compress_rate);


	bool exportDiffuseToStream(std::wstring mesh_dir , xcomdoc& doc , xMeshData& meshData, int compress_rate);
	bool exportUVChannelToStream(std::wstring mesh_dir , xcomdoc& doc , xMeshData& meshData, int compress_rate);
	
	
	bool exportWeightToStreamAsXML(std::wstring mesh_dir , xcomdoc& doc , xMeshData& meshData, int compress_rate);
	bool exportDiffuseToStreamAsXML(std::wstring mesh_dir , xcomdoc& doc , xMeshData& meshData, int compress_rate);
	bool exportUVChannelToStreamAsXML(std::wstring mesh_dir , xcomdoc& doc , xMeshData& meshData, int compress_rate);
	bool exportFaceToStreamAsXML( std::wstring mesh_dir , xcomdoc& doc , xMeshData& meshData);
	bool exportMeshDescToStreamAsXML( std::wstring mesh_dir , xcomdoc& doc , xMeshData& meshData, int nFrame , sSkeletonID_t sid );

public:
	void  export_mesh_data(xMeshData& meshData , std::wstring& mesh_dir , xcomdoc& doc , sSkeletonID_t&  sid  , bool SkinMesh , bool bTextMode );
};

#endif
