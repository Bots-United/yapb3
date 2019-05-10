//
// Copyright © 2003-2010, by YaPB Development Team. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Recast definitions.hpp
//
// Current revision: 327
//
// Version: $ID:$
//

#ifndef RECAST_DEFINITIONS_INCLUDED
#define RECAST_DEFINITIONS_INCLUDED

#include <Navigation mesh/Recast navigation/Recast/Include/Recast.h>
#include <Navigation mesh/Recast navigation/Recast/Include/RecastAssert.h>
#include <Navigation mesh/Recast navigation/Recast/Include/RecastAlloc.h>

#include <Navigation mesh/Recast navigation/DebugUtils/Include/DebugDraw.h>
#include <Navigation mesh/Recast navigation/DebugUtils/Include/RecastDump.h>
#include <Navigation mesh/Recast navigation/DebugUtils/Include/RecastDebugDraw.h>

#include <Navigation mesh/Recast navigation/Detour/Include/DetourStatus.h>
#include <Navigation mesh/Recast navigation/Detour/Include/DetourNavMesh.h>
#include <Navigation mesh/Recast navigation/Detour/Include/DetourNavMeshBuilder.h>
#include <Navigation mesh/Recast navigation/Detour/Include/DetourAssert.h>
#include <Navigation mesh/Recast navigation/Detour/Include/DetourNode.h>
#include <Navigation mesh/Recast navigation/Detour/Include/DetourNavMeshQuery.h>
#include <Navigation mesh/Recast navigation/Detour/Include/DetourAlloc.h>
#include <Navigation mesh/Recast navigation/Detour/Include/DetourCommon.h>

#include <Navigation mesh/Recast navigation/DetourTileCache/Include/DetourTileCache.h>
#include <Navigation mesh/Recast navigation/DetourTileCache/Include/DetourTileCacheBuilder.h>

#include <Navigation mesh/Recast navigation/DebugUtils/Include/DetourDebugDraw.h>

#include <Navigation mesh/Recast navigation/DetourCrowd/Include/DetourLocalBoundary.h>
#include <Navigation mesh/Recast navigation/DetourCrowd/Include/DetourObstacleAvoidance.h>
#include <Navigation mesh/Recast navigation/DetourCrowd/Include/DetourPathCorridor.h>
#include <Navigation mesh/Recast navigation/DetourCrowd/Include/DetourPathQueue.h>
#include <Navigation mesh/Recast navigation/DetourCrowd/Include/DetourProximityGrid.h>
#include <Navigation mesh/Recast navigation/DetourCrowd/Include/DetourCrowd.h>

#include <Navigation mesh/Recast navigation/RecastDemo/Contrib/fastlz/fastlz.h>

#include <Navigation mesh/Recast navigation/RecastDemo/Include/PerfTimer.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/ChunkyTriMesh.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/MeshLoaderObj.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/InputGeom.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/imgui.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/imguiRenderGL.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/SlideShow.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/SampleInterfaces.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/MeshLoaderObj.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/Sample.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/NavMeshTesterTool.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/Sample_SoloMesh.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/Sample_TileMesh.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/Sample_TempObstacles.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/Sample_Debug.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/TestCase.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/ValueHistory.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/CrowdTool.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/ConvexVolumeTool.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/OffMeshConnectionTool.h>
#include <Navigation mesh/Recast navigation/RecastDemo/Include/NavMeshPruneTool.h>

#include <Navigation mesh/Recast navigation/RecastDemo/Include/AnnotationBuilder.h>

#include <Navigation mesh/Recast navigation/RecastWrapper.hpp>

#endif	// RECAST_DEFINITIONS_INCLUDED