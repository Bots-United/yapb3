//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include <Core.hpp>

bool RecastWrapper::NavigationMeshLoader::load (const char *const /*filename = NULL*/)
{
	m_filename[0u] = '\0';

	using namespace HalfLifeEngine;

	// all valid model_s' stored in the engine start at 1 by this method, not 0,
	// and the world model is always the first model. Thus, it's at index 1.
	SDK::Structures::Model_t *const pModel (g_server->GetServer ().worldmodel);

	// make sure it's a valid Model_t pointer and the model type is ModelType_Brush
	InternalAssert (pModel != NULL && pModel->type == SDK::Constants::ModelType_Brush);

	int vcap (0);
	const WorldMap::ExtraModelInfo_t &info (g_server->GetNavigationMeshManager ().GetWorldMap ().GetWorldModelExtraInfo ());
	const Math::Vector3D *vertexes (info.surfmesh.data_vertex3f);

	for (int i (0); i < info.surfmesh.num_vertices; ++i, ++vertexes)
		addVertex (vertexes->x, vertexes->z, vertexes->y, vcap);	// convert vertex to OGL coordinates: swap y and z

	int tcap (0);
	const WorldMap::ExtraModelInfo_t::ExtraSurfaceInfo_t *surface (info.surfaces);

	for (int surfaceindex = 0; surfaceindex < info.surfacesNumber; ++surfaceindex, ++surface)
	{
		const int *trianglesIndexes (info.surfmesh.data_element3i + surface->num_firsttriangle * 3u);

		for (int i (0); i < surface->num_triangles; ++i, trianglesIndexes += 3u)
			addTriangle (trianglesIndexes[0u], trianglesIndexes[1u], trianglesIndexes[2u], tcap);
	}

	// Calculate normals.
	m_normals = new float[m_triCount * 3u];
	for (unsigned int i (0u); i < m_triCount * 3u; i += 3u)
	{
		const Math::Vector3D &v0 (m_verts + m_tris[i + 0u] * 3);
		const Math::Vector3D &v1 (m_verts + m_tris[i + 1u] * 3);
		const Math::Vector3D &v2 (m_verts + m_tris[i + 2u] * 3);
		Math::Vector3D &n (reinterpret_cast <Math::Vector3D &> (m_normals[i]));

		n = Math::Plane::CalculateAndGetNormal (v0, v1, v2);
	}

	return true;
}