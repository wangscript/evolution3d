#ifndef __MAX_NODE_MESH_BUILDER_H__
#define __MAX_NODE_MESH_BUILDER_H__
#include "MaxMesh.h"
typedef std::vector<CMaxMesh> MaxMeshs;
void    GetBonesFromNodes( sMaxNodes_t& out, sMaxNodes_t& in);

void    GetMeshNodesFromNodes( sMaxNodes_t& out, sMaxNodes_t& in);
void    GetMeshNodesFromScene( sMaxNodes_t& out );
void    GetNodesFromScene( sMaxNodes_t& out );
void    GetNodesFromSelectedNode( sMaxNodes_t& out );

void    BuildSkinMeshsFromNodes(MaxMeshs& out, sMaxNodes_t& in,CSkeletonExporter* pSkeleton);
void    BuildMeshsFromNodes(MaxMeshs& out, sMaxNodes_t& in);
void    ReleaseMaxMeshs(MaxMeshs& meshs);

#endif
