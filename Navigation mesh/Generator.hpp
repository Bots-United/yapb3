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
// Generator.hpp
//
// Class: Generator
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_MESH_GENERATOR_INCLUDED
#define NAVIGATION_MESH_GENERATOR_INCLUDED

class Generator
{
	//
	// Group: Private constants.
	//
	private:
		static const unsigned char  MaximumNavigationNodesPerCell = 5u;
		static const unsigned short GridCellSize;	///< the width/height of a grid cell for spatially partitioning navigation nodes for fast access

	//
	// Group: Private members.
	//
	private:
		NavNodeStack              m_nodes;										///< the master stack of all nodes for this map
		NavigationNodesGridArray *m_nodesGrid;									///< the master grid of all nodes for this map
		const struct NodesGridSize_t
		{
			const unsigned char x, y, z;

			inline NodesGridSize_t (void);	// Note: This function declared in Server.hpp.

			//
			// Group: Private operators.
			//
			private:
				inline NodesGridSize_t &operator = (const NodesGridSize_t &/*right*/);	// Avoid "warning C4512: 'NodesGridSize_t' : assignment operator could not be generated".
		}               m_nodesGridSize;

		NavigationNode *m_currentNode;									///< the current node we are sampling from
		NavDirType_t    m_generationDirection;
		enum GenerationStateType
		{
			SAMPLE_WALKABLE_SPACE,
			CREATE_AREAS_FROM_SAMPLES,
			FIND_HIDING_SPOTS,
			FIND_APPROACH_AREAS,
			FIND_ENCOUNTER_SPOTS,
			FIND_SNIPER_SPOTS,
			FIND_EARLIEST_OCCUPY_TIMES,
			SAVE_NAV_MESH,

			NUM_GENERATION_STATES
		} m_generationState;											///< the state of the generation process
		enum GenerationModeType
		{
			GENERATE_NONE,
			GENERATE_FULL,
			GENERATE_INCREMENTAL,
			GENERATE_ANALYSIS_ONLY
		} m_generationMode;						///< true while a Navigation Mesh is being generated
		NavAreaArray::IndexType_t m_generationIndex;										///< used for iterating navigation areas during generation process
		unsigned int m_maximumSampleTicks;
		unsigned int m_sampleTick;											///< counter for displaying pseudo-progress while sampling walkable space

		HalfLifeEngine::SDK::Constants::TeamID_t m_spawnTeam;	///< team of player spawn points, used to initiate sampling

		struct WalkableSeedSpot_t
		{
			Math::Vector3D pos;
			Math::Vector3D normal;
		};
		DynamicArray <WalkableSeedSpot_t> m_walkableSeeds;	///< array of walkable seed spots for sampling

		unsigned short m_seedIndex;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Generator (void) :
			m_nodesGridSize (),
			m_nodesGrid (new NavigationNodesGridArray[m_nodesGridSize.x * m_nodesGridSize.y * m_nodesGridSize.z]),

			m_currentNode (NULL),
			m_generationMode (GENERATE_NONE),
			m_spawnTeam (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist)	// set the default value
		{
			#if defined _DEBUG
				// Reliability check.
				if (m_nodesGrid == NULL)
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation nodes grid!", sizeof (NavigationNodesGridArray) * m_nodesGridSize.x * m_nodesGridSize.y * m_nodesGridSize.z);
			#endif	// if defined _DEBUG
		}
		inline ~Generator (void)
		{
			// destroy navigation nodes created during map generation
			m_nodes.DeleteAndPurgeAll ();	// destroy all nodes
			delete [] m_nodesGrid;	// destroy all nodes
		}

	//
	// Group: Private functions.
	//
	private:
		NavigationNode *const GetNextWalkableSeedNode (void);					///< return the next walkable seed as a node
		void                  BuildTransientAreaList  (void);

		// Note: The following 3 functions are declared in Server.hpp.
		inline const unsigned char WorldToGridX (const float worldX) const;	///< given X component, return grid index
		inline const unsigned char WorldToGridY (const float worldY) const;	///< given Y component, return grid index
		inline const unsigned char WorldToGridZ (const float worldZ) const;	///< given Z component, return grid index

	//
	// Group: Functions.
	//
	public:
		void BeginGeneration (const bool incremental = false);					///< initiate the generation process
		void BeginAnalysis (void);											///< re-analyze an existing Mesh. Determine Hiding Spots, Encounter Spots, etc.

		inline const bool IsGenerating (void) const { return m_generationMode != GENERATE_NONE; }	///< return true while a Navigation Mesh is being generated

		inline const HalfLifeEngine::SDK::Constants::TeamID_t GetPlayersSpawnPointsTeam (void)                                                const { return m_spawnTeam; }	///< Return team of player spawn points
		inline void                                           SetPlayersSpawnPointsTeam (const HalfLifeEngine::SDK::Constants::TeamID_t team)       { m_spawnTeam = team; }	///< Define the team of player spawn points

		inline void AddWalkableSeed (const Math::Vector3D &pos, const Math::Vector3D &normal)	///< Add given walkable position to list of seed positions for map sampling.
		{
			const WalkableSeedSpot_t seed = {pos, normal};

			m_walkableSeeds += seed;
		}
		inline void ClearWalkableSeeds (void) { m_walkableSeeds.RemoveAll (); }	///< erase all walkable seed positions
};

#endif	// ifndef NAVIGATION_MESH_GENERATOR_INCLUDED