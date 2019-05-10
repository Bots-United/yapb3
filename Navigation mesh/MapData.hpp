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
// MapData.hpp
//
// Class: WorldMap
//
// Description: The virtual world itself.
//				Defines data structures for loading a BSP-30 (Quake 1 / Half-Life 1) file.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef MAP_DATA_INCLUDED
#define MAP_DATA_INCLUDED

struct NavigationNode_t;

static const unsigned short MAX_SAFE_FALL_SPEED (580u);	// maximum player safe fall speed

// walkable faces structure definition
struct WalkFace_t
{
	Math::Vector3D *corners;		// pointer to array of face edges vector locations (mallocated)
	unsigned short  cornerCount;	// number of edges this face has
#if 0
	inline WalkFace_t (void) : corners (NULL), cornerCount (0u) { /* VOID */ }
	inline ~WalkFace_t (void)
	{
		// free the walkable face corners...
		delete [] corners;
	}
#endif	// if 0
};

// navigation link structure definition
struct NavigationLink_t
{
	// internode special reachabilities
	DECLARE_TYPED_ENUMERATION (unsigned char, ReachabilityFlag_t)
	{
		REACHABILITY_NORMAL,

		REACHABILITY_LADDER     = BIT (0u),
		REACHABILITY_FALLEDGE   = BIT (1u),
		REACHABILITY_ELEVATOR   = BIT (2u),
		REACHABILITY_PLATFORM   = BIT (3u),
		REACHABILITY_CONVEYOR   = BIT (4u),
		REACHABILITY_TRAIN      = BIT (5u),
		REACHABILITY_LONGJUMP   = BIT (6u),
		REACHABILITY_SWIM       = BIT (7u),
		REACHABILITY_TELEPORTER = BIT (8u),
		REACHABILITY_JUMP       = BIT (9u),
		REACHABILITY_CROUCH     = BIT (10u)
	};

	NavigationNode_t   *nodeFrom;		// pointer to the navigation node beyond this entryPoint
	ReachabilityFlag_t  reachability;	// type of reachability it is (normal, ladder, edge fall, elevator, train, etc.)
	Math::Vector3D      origin;			// vector origin of this link (passage between 2 nodes) in the virtual world
};

typedef DynamicArray <WalkFace_t *> Sector_t;	// array of pointers to walkable faces located in this sector

class WorldMap
{
	//
	// Group: Private constants.
	//
	private:
		static const DynamicString  FileExtention;
		static const unsigned short GridCellSize = 300u;	///< the width/height of a grid cell for spatially partitioning sectors for fast access

	public:
		// Constant maximums of various kinds of data
		enum Constant_t
		{
			// map topology granularity
			MAX_MAP_PARALLELS     = HalfLifeEngine::SDK::Constants::MapSize / GridCellSize + 1u,
			MAX_MAP_MERIDIANS     = HalfLifeEngine::SDK::Constants::MapSize / GridCellSize + 1u
		};

		struct Entity_t
		{
			DynamicString                                 className;
			DynamicString                                 targetName;

			Math::Vector3D                                origin;		// Model's origin in world coordinates.
			Math::Angles3D                                angles;		// model angles

			// render information
			HalfLifeEngine::SDK::Constants::RenderMode_t  renderMode;	// entity render mode values
			float                                         renderAmount;
			Color <float>                                 renderColor;
			HalfLifeEngine::SDK::Constants::RenderFx_t    renderFx;		// entity render fx values

			int                                           health;

			unsigned short                                brushModelIndex;

			unsigned short                                spawnFlags;
//			class StudioModel                                  *studioModel;

			// Manually added members....
			public:
				bool isStaticSolid;

			public:
				inline void Reset (void)
				{
					className.MakeEmpty ();
					targetName.MakeEmpty ();

					origin = Math::Vector3D::ZeroValue;
					angles = Math::Angles3D::ZeroValue;

					renderMode = HalfLifeEngine::SDK::Constants::kRenderNormal;
					renderAmount = 1.0f;
					renderColor = Color <float>::ZeroValue;
					renderFx = HalfLifeEngine::SDK::Constants::kRenderFxNone;

					health = 0;

					brushModelIndex = 0u;
					spawnFlags = 0u;

					isStaticSolid = false;
				}
		};

		struct BSPMapFile_t
		{
			typedef DynamicArray <Entity_t>                                                        EntitiesArray_t;			// map's entity data array
			typedef DynamicArray <HalfLifeEngine::SDK::Structures::DLeaf_t>                        LeafsArray_t;				// map's leafs data array
			typedef DynamicArray <HalfLifeEngine::SDK::Structures::DPlane_t>                       PlanesArray_t;				// map's plane data array
			typedef DynamicArray <HalfLifeEngine::SDK::Structures::DVertex_t>                      VertexesArray_t;			// map's vertice data array
			typedef DynamicArray <HalfLifeEngine::SDK::Structures::DNode_t>                        NodesArray_t;				// map's nodes data array
			typedef DynamicArray <HalfLifeEngine::SDK::Structures::DClipNode_t>                    ClipNodesArray_t;			// map's clip nodes data array
			typedef DynamicArray <HalfLifeEngine::SDK::Structures::DFace_t>                        FacesArray_t;				// map's face data array
			typedef DynamicArray <HalfLifeEngine::SDK::Structures::DEdge_t, unsigned int>          EdgesArray_t;				// map's edge data array
			typedef DynamicArray <HalfLifeEngine::SDK::TypeDefinitions::DSurfEdge_t, unsigned int> SurfEdgesArray_t;			// map's surf edge data array
			typedef DynamicArray <HalfLifeEngine::SDK::Structures::DModel_t>                       ModelsArray_t;				// map's model data array (models[0] is a world map itself)
			typedef DynamicArray <HalfLifeEngine::SDK::TypeDefinitions::DMarkSurface_t>            MarkSurfacesArray_t;		// map's mark surfaces data array
			typedef DynamicArray <unsigned char/*/HalfLifeEngine::SDK::Structures::DTexture_t*/, unsigned int>                                     TexturesDataArray_t;		// map's textures data array (HalfLifeEngine::SDK::Structures::DTextureLump_t, HalfLifeEngine::SDK::Structures::DTexture_t)
			typedef DynamicArray <HalfLifeEngine::SDK::Structures::DTextureInformation_t>          TexturesInformationArray_t;	// map's textures information data array

			EntitiesArray_t            entities/*[HalfLifeEngine::SDK::Constants::MAX_MAP_ENTITIES]*/;					// map's entity data array
			LeafsArray_t               leafs/*[HalfLifeEngine::SDK::Constants::MAX_MAP_LEAFS]*/;						// map's leafs data array
			PlanesArray_t              planes/*[HalfLifeEngine::SDK::Constants::MAX_MAP_PLANES]*/;						// map's plane data array
			VertexesArray_t            vertexes/*[HalfLifeEngine::SDK::Constants::MAX_MAP_VERTS]*/;						// map's vertice data array
			NodesArray_t               nodes/*[HalfLifeEngine::SDK::Constants::MAX_MAP_NODES]*/;						// map's nodes data array
			ClipNodesArray_t           clipNodes/*[HalfLifeEngine::SDK::Constants::MAX_MAP_CLIP_NODES]*/;				// map's clip nodes data array
			FacesArray_t               faces/*[HalfLifeEngine::SDK::Constants::MAX_MAP_FACES]*/;						// map's face data array
			EdgesArray_t               edges/*[HalfLifeEngine::SDK::Constants::MAX_MAP_EDGES]*/;						// map's edge data array (edge 0 is not used, because 0 can't be negated)
			SurfEdgesArray_t           surfEdges/*[HalfLifeEngine::SDK::Constants::MAX_MAP_SURF_EDGES]*/;				// map's surf edge data array
			ModelsArray_t              models/*[HalfLifeEngine::SDK::Constants::MAX_MAP_MODELS]*/;						// map's model data array (models[0] is a world map itself)
			MarkSurfacesArray_t        markSurfaces/*[HalfLifeEngine::SDK::Constants::MAX_MAP_MARK_SURFACES]*/;			// map's mark surfaces data array
//			HalfLifeEngine::SDK::Structures::DTextureLump_t              textureHeader;									// map's textures header
			TexturesDataArray_t        texturesData/*[HalfLifeEngine::SDK::Constants::MAX_MAP_TEXTURE_LUMP]*/;			// map's textures data array (HalfLifeEngine::SDK::Structures::DTextureLump_t, HalfLifeEngine::SDK::Structures::DTexture_t)
			TexturesInformationArray_t texturesInformation/*[HalfLifeEngine::SDK::Constants::MAX_MAP_TextureInfo]*/;	// map's textures information data array

			inline void RemoveAll (void)
			{
				entities.RemoveAll ();
				leafs.RemoveAll ();
				planes.RemoveAll ();
				vertexes.RemoveAll ();
				nodes.RemoveAll ();
				clipNodes.RemoveAll ();
				faces.RemoveAll ();
				edges.RemoveAll ();
				surfEdges.RemoveAll ();
				models.RemoveAll ();
				markSurfaces.RemoveAll ();

//				delete [] textureHeader.dataOffsets;

				texturesData.RemoveAll ();
				texturesInformation.RemoveAll ();
			}
		};

		struct ExtraModelInfo_t
		{
			struct surfmesh_t
			{
				// triangle data in system memory
				int num_triangles; // number of triangles in the mesh
				int *data_element3i; // int[tris*3] triangles of the mesh, 3 indices into vertex arrays for each

				// vertex data in system memory
				int num_vertices; // number of vertices in the mesh
				Math::Vector3D *data_vertex3f; // Math::Vector3D[verts] vertex locations

				inline surfmesh_t (void) : data_element3i (NULL), data_vertex3f (NULL) {}
				inline ~surfmesh_t (void)
				{
					delete [] data_element3i;
					delete [] data_vertex3f;
				}
			} surfmesh;

			struct ExtraSurfaceInfo_t
			{
				// surfaces own ranges of vertices and triangles in the model->surfmesh
				int num_firsttriangle;	// first triangle
				int num_triangles;		// number of triangles
				int num_firstvertex;	// first vertex
				int num_vertices;		// number of vertices
			} *surfaces;
			unsigned short surfacesNumber;

			inline ExtraModelInfo_t (void) : surfaces (NULL) {}
			inline ~ExtraModelInfo_t (void) { delete [] surfaces; }
		};

		typedef StaticArray <Math::Winding *, unsigned short, 2000u> WindingArray_t;
		WindingArray_t m_windings;
		void MakeHullFaces (void);

		typedef StaticArray <WalkFace_t, unsigned short, HalfLifeEngine::SDK::Constants::MAX_MAP_FACES> WalkFacesArray_t;

	private:
		BSPMapFile_t m_bspFile;

		ExtraModelInfo_t m_worldModelExtraInfo;

		WalkFacesArray_t   m_walkFaces;	// map's walkable faces memory space (mallocated)
		unsigned char      m_parallelsCount;									// number of parallels dividing this map into sectors
		unsigned char      m_meridiansCount;									// number of meridians dividing this map into sectors
		Sector_t           m_topology[MAX_MAP_PARALLELS][MAX_MAP_MERIDIANS];	// map spatial topology

		typedef DynamicArray <BSPMapFile_t::ClipNodesArray_t::IndexType_t> ClipNodesIndexesArray_t;
		ClipNodesIndexesArray_t m_clipNodesIndexesArrays[HalfLifeEngine::SDK::Constants::Hull_Total];

	public:
		inline WorldMap (void) : m_parallelsCount (0u), m_meridiansCount (0u)
		{
			LookDownOnTheWorld ();	// Look down on the world and sort the faces and delimiters....
		}
		inline ~WorldMap (void)
		{
			// free the map data
			FreeMapData ();
		}

	private:
		struct EntityPair_t
		{
			DynamicString key;
			DynamicString value;
		};

		const bool ParseEntityPair (char *&data, EntityPair_t &epair);
		char *ParseEntity (char *data, Entity_t &entity);
		void MakeCollisionPointHull (void);

		void LoadWorldModelExtraInfo (void);

		template <typename nodeArrayType> void RecursiveCountClipNodesForOneHull (const nodeArrayType &nodes, const short clipNodeIndex, ClipNodesIndexesArray_t &clipNodesIndexesArray, BSPMapFile_t::ClipNodesArray_t::IndexType_t &count) const;
		void                                   CountClipNodesForOneHull          (void);

		void                  ReadEntitiesFromStringData (const LongDynamicString &entitiesData);

	public:
		void                  LookDownOnTheWorld         (void);
		void                  FreeMapData                (void);
		const bool            Load                       (const unsigned int bspFileSize);
		const bool            WalkFaceBelongsToSector    (const WalkFace_t &face, const unsigned char sector_i, const unsigned char sector_j) const;
		const unsigned int    Save                       (const unsigned int bspFileSize) const;
		const unsigned short  WalkFaceIndexOf            (const WalkFace_t &walkFace) const;
		WalkFace_t *const     GetWalkFaceUnder           (HalfLifeEngine::SDK::Classes::Edict *const player);
		WalkFace_t *const     GetWalkFaceUnder           (const Math::Vector3D &location);
		const Sector_t       &GetSectorUnder             (HalfLifeEngine::SDK::Classes::Edict *const entity) const;
		const Sector_t       &GetSectorUnder             (const Math::Vector2D &location) const;
		const Math::Vector3D  DropToFloor                (const Math::Vector3D &location) const;
		void                  DrawSector                 (const Human *const client, const Sector_t &sector, const Color <> &color, const unsigned char duration) const;
		void                  DrawWalkFace               (const Human *const client, const WalkFace_t &face, const Color <> &color, const unsigned char duration) const;
		void                  DrawNavigationLink         (const Human *const client, const NavigationLink_t *const link, const unsigned char duration) const;
		void                  WriteDebugBitmap           (void) const;
		const short           FindEntityByClassName      (short index, const char *const className) const;

		inline const BSPMapFile_t &GetBSPFile (void) const { return m_bspFile; }
		inline ExtraModelInfo_t &GetWorldModelExtraInfo (void) { return m_worldModelExtraInfo; }

		void Mod_Decompile_OBJ (HalfLifeEngine::SDK::Structures::Model_t *const model, const char *filename, const char *mtlfilename, const char *originalfilename) const;
		void Mod_Decompile_f (void) const;

		// find the contents of a coordinate in 3D space...
		inline const HalfLifeEngine::SDK::Constants::PointContentType_t GetPointContents (const Math::Vector3D &point, const HalfLifeEngine::SDK::Constants::Hull_t hullNumber = HalfLifeEngine::SDK::Constants::Hull_Point) const
		{
			// return contents
			return m_bspFile.leafs[FindLeaf (point, hullNumber)].contents;
		}

		inline const HalfLifeEngine::SDK::Constants::PointContentType_t GetHullPointContents (short startClipNodeIndex, const Math::Vector3D &point) const
		{
			// walk through the tree to find the content for the point....
			while (startClipNodeIndex >= 0)
			{
				// Reliability check.
				InternalAssert (startClipNodeIndex < m_bspFile.clipNodes.GetElementNumber ());

				const HalfLifeEngine::SDK::Structures::DClipNode_t &node (m_bspFile.clipNodes[static_cast <unsigned short> (startClipNodeIndex)]);
				const HalfLifeEngine::SDK::Structures::DPlane_t &plane (m_bspFile.planes[static_cast <unsigned short> (node.planeNum)]);
				const float distance (plane.type <= HalfLifeEngine::SDK::Structures::DPlane_t::Type_Z ? point[plane.type] - plane.distance : (plane.normal | point) - plane.distance);	// easier for axial planes, else - f(x,y,z) = Ax+Ay+Az-D

				// behind the plane OR in front or on the plane
				startClipNodeIndex = node.children[distance < 0.0f];
			}

			return static_cast <HalfLifeEngine::SDK::Constants::PointContentType_t> (startClipNodeIndex);
		}

		inline const unsigned short FindLeaf (short startNodeIndex, const Math::Vector3D &point) const
		{
			// This function returns the leaf node for a point in 3D space.

			// Reliability check.
			InternalAssert (startNodeIndex >= 0);

			// not a leaf
			// walk through the tree to find the leaf for the point....
			do
			{
				const HalfLifeEngine::SDK::Structures::DNode_t &node (m_bspFile.nodes[static_cast <unsigned short> (startNodeIndex)]);
				const HalfLifeEngine::SDK::Structures::DPlane_t &plane (m_bspFile.planes[static_cast <unsigned short> (node.planeNum)]);
				const float distance ((point | plane.normal) - plane.distance);	// f(x,y,z) = Ax+Ay+Az-D

				// behind the plane OR in front or on the plane
				startNodeIndex = node.children[distance < 0.0f];
			} while (startNodeIndex >= 0);

			// leaf index in leafs array
			return -(startNodeIndex + 1);
		}
		inline const unsigned short FindLeaf (const Math::Vector3D &point, const HalfLifeEngine::SDK::Constants::Hull_t/* hullNumber*/ = HalfLifeEngine::SDK::Constants::Hull_Point) const
		{
			// m_source at the root of the BSP tree....
			return FindLeaf (/*m_bspFile.models[0u].headNode[hullNumber]*/0, point);
		}

		inline const unsigned short GetFirstVertexID (const HalfLifeEngine::SDK::Structures::DFace_t &face, const unsigned short cornerIndex) const
		{
			// Reliability check.
			InternalAssert (cornerIndex < face.surfEdgesNum);

			// get the coordinates of the vertex of this edge...
			const int surfEdge (m_bspFile.surfEdges[static_cast <unsigned int> (face.firstSurfEdgeIndex + cornerIndex)]);

			// note: the q1bsp format does not allow a 0 surfedge (it would have no negative counterpart)
			// if its index in the BSP tree is negative... - revert it, consider the other side of the segment, else consider the first side of the segment
			// locate and return the first vertice of this edge
			return surfEdge < 0 ? m_bspFile.edges[static_cast <unsigned int> (-surfEdge)].vertices[1u] : m_bspFile.edges[static_cast <unsigned int> (surfEdge)].vertices[0u];
		}
		inline const Math::Vector3D &GetFaceCornerByIndex (const HalfLifeEngine::SDK::Structures::DFace_t &face, const unsigned short cornerIndex) const
		{
			// this function finds, translates and returns the vector location of the specified corner
			// on the BSP face pointed to by 'face'.

			// locate and return the first vertice of this edge
			return m_bspFile.vertexes[GetFirstVertexID (face, cornerIndex)].point;
		}

		inline const char *const GetTextureName (const HalfLifeEngine::SDK::Structures::DFace_t &face) const
		{
			const HalfLifeEngine::SDK::Structures::DTextureInformation_t &textureInformation (m_bspFile.texturesInformation[static_cast <unsigned short> (face.textureInfo)]);
			const HalfLifeEngine::SDK::Structures::DTextureLump_t &textureLump (reinterpret_cast <const HalfLifeEngine::SDK::Structures::DTextureLump_t &> (m_bspFile.texturesData[0u])/*/m_bspFile.textureHeader*/);

			// Reliability check.
			InternalAssert (textureInformation.textureIndex < textureLump.texturesNum);

			const BSPMapFile_t::TexturesDataArray_t::IndexType_t offset (textureLump.dataOffsets[textureInformation.textureIndex]);

			// not unused mip lump
			if (offset >= m_bspFile.texturesData.GetElementNumber ()/*== -1 - ORIGINAL*/)
				return NULL;

			const HalfLifeEngine::SDK::Structures::DTexture_t &texture (reinterpret_cast <const HalfLifeEngine::SDK::Structures::DTexture_t &> (m_bspFile.texturesData[offset]));

			return texture.name;
//*/			return m_bspFile.texturesData[offset].name;
		}

		inline const Math::Vector3D FindPointInPlanes (void) const
		{
			Math::Vector3D point (Math::Vector3D::ZeroValue);

			for (BSPMapFile_t::PlanesArray_t::IndexType_t planeIndex (0u); planeIndex < m_bspFile.planes.GetElementNumber (); ++planeIndex)
			{
				const HalfLifeEngine::SDK::Structures::DPlane_t &plane (m_bspFile.planes[planeIndex]);
				const float distance ((point | plane.normal) - plane.distance);

				if (distance < 0.0f)
					point -= plane.normal * distance;
			}

			return point;
		}

/*
		struct LadderFaceInfo_t
		{
			unsigned short planeIndex;
			struct MidSide_t
			{
				Math::Vector3D segment[2u];
				float          lengthSquared;
			} midSides[2u];
		};
		typedef DynamicArray <LadderFaceInfo_t, unsigned char> LadderFaceInfoArray;
		typedef DynamicArray <LadderFaceInfoArray, unsigned char> LadderFaceInfoArrays;
		void GetLadderPossibleClimbableMidSides (const HalfLifeEngine::SDK::Structures::DModel_t &model, DynamicArray <DynamicArray <LadderFaceInfo_t, unsigned char>, unsigned char> &faceInfoArrays) const;
*/
		void PrintEntities (void) const;
};

extern void UTIL_TraceHull (const Math::Vector3D &source, const Math::Vector3D &destination, const HalfLifeEngine::SDK::Constants::Hull_t hullNumber, HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult);

class RayTrace
{
	public:
		enum Type_t
		{
			Type_Point,
			Type_AxisAlignedBox,
			Type_Sphere,
			Type_AxisAlignedCylinder,
			Type_Ellipsoid,
			Type_Player,

			Type_Traces
		};	// Trace type
		enum
		{
			Flag_None,

			Flag_Face,
			Flag_Texture
		};	// Flags for data request

		struct TraceParameters_t
		{
			//HalfLifeEngine::SDK::Structures::DNode_t  *node;
			//int                Side;

			int                State;
			int                NodeNum;
			float              StartFrac;
			float              EndFrac;
			Math::Vector3D     StartPoint;
			Math::Vector3D     EndPoint;
			int                StartPlane;
			int                EndPlane;

			TraceParameters_t *Next;

			inline TraceParameters_t (void) : Next (NULL), State (0)/*, node (NULL)*/ { /*VOID*/ }

			inline TraceParameters_t &operator = (const TraceParameters_t &right)
			{
				State = right.State;
				NodeNum = right.NodeNum;
				StartFrac = right.StartFrac;
				EndFrac = right.EndFrac;
				StartPoint = right.StartPoint;
				EndPoint = right.EndPoint;
				StartPlane = right.StartPlane;
				EndPlane = right.EndPlane;

				return *this;
			}
		} *m_parameters;

		struct Result_t
		{
			bool                                                isAllSolid;					// if true, plane is not valid
			bool                                                isStartSolid;				// if true, the initial point is in a solid leaf
			bool                                                isInOpen;
			bool                                                isInWater;
			float                                               fraction;					// 1.0 = didn't hit anything
			Math::Vector3D                                      endPosition;				// final position (in blocking leaf)
			HalfLifeEngine::SDK::Structures::DPlane_t           plane;
//			const HalfLifeEngine::SDK::Classes::Edict          *hitEntity;
//			int                                                 hitGroup;

			HalfLifeEngine::SDK::Constants::PointContentType_t  contents;					// contents of endPosition
			float                                               hitDistance;
			int                                                 faceNum;					// hit face if fraction < 1.0
			const HalfLifeEngine::SDK::Structures::DPlane_t    *planePointer;
			int                                                 startContents;				// contents of start pos
			int                                                 startLeafNum;				// start leaf
			int                                                 blockingLeafNum;			// last non-blocking leaf
			int                                                 furthestNonBlockingLeafNum;	// blocking leaf
			const char                                         *textureName;				// texture of hit face if fraction < 1.0
			int                                                 planeNum;
		};

	private:
		Result_t       m_result;
		Result_t       m_tempResult;

		Math::Vector3D m_source;
		Math::Vector3D m_destination;

		unsigned char  m_flags;

		Type_t         m_type;

		Math::Vector3D m_hullSize;

		Math::Vector3D::AxisID_t m_axisID;
		Math::Vector3D::AxisID_t m_B1;
		Math::Vector3D::AxisID_t m_B2;

	public:
		inline RayTrace (const Math::Vector3D &source, const Math::Vector3D &destination, const unsigned char flags = Flag_Face | Flag_Texture, const Type_t type = Type_Point, const Math::Vector3D &hullSize = HalfLifeEngine::SDK::Constants::VEC_HULL_MAX, const Math::Vector3D::AxisID_t axisID = Math::Vector3D::AxisID_Z) :
			m_parameters (NULL),

			m_source (source),
			m_destination (destination),

			m_flags (flags),

			m_type (type),

			m_axisID (axisID),
			m_B1 (static_cast <Math::Vector3D::AxisID_t> (BIT (axisID) & Math::Vector3D::AxisID_Total)),
			m_B2 (static_cast <Math::Vector3D::AxisID_t> (BIT (m_B1) & Math::Vector3D::AxisID_Total))
		{
			switch (type)
			{
				case Type_AxisAlignedBox:
				case Type_Player:
						SetAxisAlignedBoxCenterSize (source, hullSize);

					break;

				case Type_Sphere:
						SetSphere (source, hullSize.x);

					break;

				case Type_AxisAlignedCylinder:
						SetAxisAlignedBoxHalfSizeCylinder (source, hullSize.x, hullSize.z, axisID);

					break;

				case Type_Ellipsoid:
						SetEllipsoid (source, hullSize);

					break;
			}

			Execute ();
		}

	private:
		void              GetHullOffsets    (const Math::Vector3D &planeNormal, const Math::Plane::PointSide_t side, float &offsetN, float &offsetF) const;
		inline const bool TraceToLeaf       (const unsigned short leafNodeIndex, Result_t &result) const;
		const int         TraceMoveFindFace (const Math::Vector3D &start, Result_t &result) const;
		void              TraceMove         (Result_t &result);
		const char        TraceMovePointR   (const int nodeIndex, const TraceParameters_t &traceParameters, Result_t &result);

	public:
		inline void                  SetSourcePosition      (const Math::Vector3D &source)      { m_source = source; }
		inline const Math::Vector3D &GetSourcePosition      (void) const                        { return m_source; }
		inline void                  SetDestinationPosition (const Math::Vector3D &destination) { m_destination = destination; }
		inline const Math::Vector3D &GetDestinationPosition (void) const                        { return m_destination; }
		inline void                  SetFlags               (const unsigned char flags)         { m_flags = flags; }
		inline const unsigned char   GetFlags               (void) const                        { return m_flags; }
		inline const Result_t       &GetResult              (void) const                        { return m_result; }

		void                         Execute                (void);

		inline void SetAxisAlignedBoxMinMax (const Math::Vector3D &mins, const Math::Vector3D &maxs)
		{
			m_source = mins.GetInterpolated (maxs, 0.5f);
			m_hullSize = maxs - mins;

			m_hullSize *= 0.5f;
		}
		inline void SetAxisAlignedBoxCenterSize (const Math::Vector3D &center, const Math::Vector3D &axisAlignedBoxHalfSize)
		{
			m_source = center;
			m_hullSize = axisAlignedBoxHalfSize;
		}
		inline void SetEllipsoid (const Math::Vector3D &center, const Math::Vector3D &axisAlignedBoxHalfSize)
		{
			m_source = center;
			m_hullSize = axisAlignedBoxHalfSize;
		}
		inline void SetAxisAlignedBoxHalfSizeCylinder (const Math::Vector3D &center, const float radius, const float halfLength, const Math::Vector3D::AxisID_t axisID)
		{
			m_source = center;
			m_hullSize.x = radius;
			m_hullSize.y = halfLength;
			m_axisID = axisID;
			m_B1 = static_cast <Math::Vector3D::AxisID_t> (BIT (axisID) & Math::Vector3D::AxisID_Total);
			m_B2 = static_cast <Math::Vector3D::AxisID_t> (BIT (m_B1) & Math::Vector3D::AxisID_Total);
		}
		inline void SetSphere (const Math::Vector3D &center, const float radius)
		{
			m_source = center;
			m_hullSize.x = radius;
		}

		inline const float GetXSize      (void) const { return m_hullSize.x * 2.0f; }
		inline const float GetYSize      (void) const { return m_hullSize.y * 2.0f; }
		inline const float GetHalfHeight (void) const { return m_hullSize.z; }
		inline const float GetHeight     (void) const { return m_hullSize.z * 2.0f; }

		inline const float GetRadius     (void) const { return m_hullSize.x; }
//		inline const float GetDiameter   (void) const { return m_hullSize.x * 2.0f; }
};
#if 0
class AxisAlignedBoxTrace : public RayTrace
{
	private:
		Math::Vector3D m_hullSize;

	public:
		inline AxisAlignedBoxTrace (const Math::Vector3D &source, const Math::Vector3D &destination, const unsigned char flags = Flag_None, const Math::Vector3D &hullSize = HalfLifeEngine::SDK::Constants::VEC_HULL_MAX) :
			RayTrace (source, destination, flags),

			m_hullSize (hullSize)
		{ /*VOID*/ }

	public:
		void               Execute       (void);

		inline const float GetXSize      (void) const { return m_hullSize.x * 2.0f; }
		inline const float GetYSize      (void) const { return m_hullSize.y * 2.0f; }
		inline const float GetHalfHeight (void) const { return m_hullSize.z; }
		inline const float GetHeight     (void) const { return m_hullSize.z * 2.0f; }
};

class AxisAlignedCylinderTrace : public RayTrace
{
	private:
		float                    m_radius;
		float                    m_length;
		Math::Vector3D::AxisID_t m_axisID;
		Math::Vector3D::AxisID_t m_B1;
		Math::Vector3D::AxisID_t m_B2;

	public:
		inline AxisAlignedCylinderTrace (const Math::Vector3D &source, const Math::Vector3D &destination, const unsigned char flags = Flag_None, const float radius = 18.0f, const float length = 36.0f, const Math::Vector3D::AxisID_t axisID = Math::Vector3D::AxisID_Z) :
			RayTrace (source, destination, flags),

			m_radius (radius),
			m_length (length),
			m_axisID (axisID),
			m_B1 (static_cast <Math::Vector3D::AxisID_t> (BIT (axisID) & Math::Vector3D::AxisID_Total)),
			m_B2 (static_cast <Math::Vector3D::AxisID_t> (BIT (m_B1) & Math::Vector3D::AxisID_Total))
		{ /*VOID*/ }

	public:
		void               Execute       (void);

		inline const float GetRadius     (void) const { return m_radius; }
		inline const float GetDiameter   (void) const { return m_radius * 2.0f; }
		inline const float GetHalfHeight (void) const { return m_length; }
		inline const float GetHeight     (void) const { return m_length * 2.0f; }
};

class SphereTrace : public RayTrace
{
	private:
		float m_radius;

	public:
		inline SphereTrace (const Math::Vector3D &source, const Math::Vector3D &destination, const unsigned char flags = Flag_None, const float radius = 18.0f) :
			RayTrace (source, destination, flags),

			m_radius (radius)
		{ /*VOID*/ }

	public:
		void               Execute     (void);

		inline const float GetRadius   (void) const { return m_radius; }
		inline const float GetDiameter (void) const { return m_radius * 2.0f; }
};
#endif	// if 0
// navigation node structure definition
struct NavigationNode_t
{
	WalkFace_t        *walkFace;		// the walkable face this node concerns
	NavigationLink_t   links[8u];		// array of navigation links to walk faces reachable from this one
	char               linksCount;		// number of navigation links in this navigation node

	// dynamic data used by the bot's path machine
	NavigationNode_t  *parent;			// pointer to this element's parent in the queue during path search
	NavigationLink_t  *entryPoint;		// pointer to this element's parent navlink that led to this node
	bool               isInOpenList;	// set to true if this node is in the path machine's OPEN list
	bool               isInClosedList;	// set to true if this node is in the path machine's CLOSED list
	float              travelCost;		// minimal cost of travel to this node during a path search
	float              remainingCost;	// estimated cost of travel from this node to the goal (heuristic)
	float              totalCost;		// weight of this element in the priority queue (sum of the 2 above)
};

// pathfinding machine
struct PathMachine_t
{
	bool               busy;				// set to true if the pathfinding machine is busy computing a path
	bool               finished;			// set to true if the pathfinding machine has finished computing its path.
	NavigationNode_t  *goalNode;			// pointer to the current goal node (node FROM since reverse pathfinding)
	bool               shouldUpdateGoal;	// set to true if the goal node should be kept updated after bot's position
	NavigationNode_t **open;				// pathfinding machine's OPEN list (priority queue of nodes to search)
	int                openCount;			// number of elements in the OPEN list (fixed-size heap of the queue)
	NavigationLink_t **path;				// resulting PATH list (array of pointers to navlinks, so far)
	int                pathCount;			// number of elements in the PATH list (i.e., number of navlinks in the path)
	float              pathCost;			// cost of the path (variated distance to goal, according to bot's likelevels)
};

#endif	// ifndef MAP_DATA_INCLUDED