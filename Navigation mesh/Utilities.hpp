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
// Utilities.hpp
//
// Name space: Utilities
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef UTILITIES_INCLUDED
#define UTILITIES_INCLUDED

//--------------------------------------------------------------------------------------------------------------
DECLARE_TYPED_ENUMERATION (unsigned char, IgnoreEntityFlag_t)
{
	IgnoreEntityFlag_None,

	WALK_THRU_DOORS          = BIT (0u),
	WALK_THRU_BREAKABLES     = BIT (1u),
	WALK_THRU_PUSHABLES      = BIT (2u),
//	WALK_THRU_TOGGLE_BRUSHES = BIT (3u),	// MADE BY EDWARD!!! - START

	WALK_THRU_EVERYTHING     = WALK_THRU_DOORS | WALK_THRU_BREAKABLES | WALK_THRU_PUSHABLES// | WALK_THRU_TOGGLE_BRUSHES	// MADE BY EDWARD!!! - START
};

inline const bool IsCanWalkThruEntity (const HalfLifeEngine::SDK::Classes::Edict *const entity, const IgnoreEntityFlag_t flags)
{
	// Return true if given entity can be ignored when moving
	// MADE BY EDWARD!!! - START
	InternalAssert (entity != NULL);
/*
	if (entity->GetClassName () == "worldspawn" || entity->GetClassName () == "player")
		return false;

	// if we hit a clip brush, ignore it if it is not BRUSHSOLID_ALWAYS
	if (entity->GetClassName () == "func_brush")
	{
		switch (static_cast <CFuncBrush *> (entity)->m_iSolidity)
		{
			case CFuncBrush::BRUSHSOLID_ALWAYS:
				return false;

			case CFuncBrush::BRUSHSOLID_NEVER:
				return true;

			case CFuncBrush::BRUSHSOLID_TOGGLE:
				return (flags & WALK_THRU_TOGGLE_BRUSHES) != 0;
		}

		return true;
	}
*/	// MADE BY EDWARD!!! - END
	// if we hit a door, assume its walkable because it will open when we touch it
	if (entity->GetClassName ().CompareWithCaseCheck ("func_door", GET_STATIC_STRING_LENGTH ("func_door")) == 0)
		return (flags & WALK_THRU_DOORS) != 0;

	// if we hit a breakable object, assume its walkable because we will shoot it when we touch it
	if (entity->IsBreakable () && entity->IsBreakableShootable ())
		return (flags & WALK_THRU_BREAKABLES) != 0;

	if (entity->GetClassName () == "func_pushable")
		return (flags & WALK_THRU_PUSHABLES) != 0;

	return false;
}

inline const bool TraceLineThruWalkableEntities (const Math::Vector3D &source, const Math::Vector3D &destination, const HalfLifeEngine::SDK::Constants::TraceIgnore_t/* traceIgnore*/, HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult)
{
/*	const Math::Vector3D &direction ((destination - source).Normalize ());	// one unit long

//	source.x += direction.x * 16.0f;	// 16 units long
//	source.y += direction.y * 16.0f;	// 16 units long
//	source.z += direction.z * 18.0f;	// 18 units long

//	destination.x -= direction.x * 16.0f;	// 16 units long
//	destination.y -= direction.y * 16.0f;	// 16 units long
//	destination.z -= direction.z * 18.0f;	// 18 units long

	UTIL_TraceHull (source, destination, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);

//	traceResult.endPosition.x += direction.x * 16.0f;	// 16 units long
//	traceResult.endPosition.y += direction.y * 16.0f;	// 16 units long
	traceResult.endPosition.z += direction.z * 18.0f;	// 18 units long
*/
//	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, destination, traceIgnore, NULL, traceResult);
{
	HalfLifeEngine::Engine::TraceFilter_WalkableEntities traceFilter (NULL, 0, HalfLifeEngine::Engine::TraceFilter_WalkableEntities::WALK_THRU_EVERYTHING);
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHullThroughSolidEntities (source, destination, HalfLifeEngine::SDK::Constants::Hull_Point, traceFilter, traceResult);
}
	return traceResult.fraction == 1.0f && traceResult.isStartSolid == false;
#if 0
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, destination, traceIgnore, NULL, traceResult);

//	if (/*traceResult.isStartSolid || */traceResult.fraction == 1.0f || !IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
//		return;
	if (traceResult.fraction == 1.0f)
	{
		if (traceResult.isStartSolid == false)
			return true;
		else if (IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
		{
			traceResult.isStartSolid = traceResult.isAllSolid = false;

			return true;
		}
		else
			return false;
//		return traceResult.isStartSolid == false || IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING);
	}
	else if (!IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
		return false;
/*/
	do
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (traceResult.endPosition, destination, traceIgnore, traceResult.hitEntity, traceResult);
	} while (/*!traceResult.isStartSolid && *//*traceResult.fraction < 1.0f && IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING));*/
	for (/* Empty */; /* Empty */; /* Empty */)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (traceResult.endPosition, destination, traceIgnore, traceResult.hitEntity, traceResult);

		if (traceResult.fraction == 1.0f)
		{
			if (traceResult.isStartSolid == false)
				return true;
			else if (IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
			{
				traceResult.isStartSolid = traceResult.isAllSolid = false;

				return true;
			}
			else
				return false;
//			return traceResult.isStartSolid == false || IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING);
		}
		else if (!IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
			return false;
	}
#endif	// if 0
}

inline const bool TraceHullThruWalkableEntities (const Math::Vector3D &source, const Math::Vector3D &destination, const bool/* ignoreMonsters*/, const HalfLifeEngine::SDK::Constants::Hull_t/* hullNumber*/, HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult)
{
	UTIL_TraceHull (source, destination, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);

	return traceResult.fraction == 1.0f && traceResult.isStartSolid == false;
#if 0
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, ignoreMonsters, hullNumber, NULL, traceResult);

//	if (/*traceResult.isStartSolid || */traceResult.fraction == 1.0f || !IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
//		return;
	if (traceResult.fraction == 1.0f)
	{
		if (traceResult.isStartSolid == false)
			return true;
		else if (IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
		{
			traceResult.isStartSolid = traceResult.isAllSolid = false;

			return true;
		}
		else
			return false;
//		return traceResult.isStartSolid == false || IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING);
	}
	else if (!IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
		return false;
/*
	do
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (traceResult.endPosition, destination, ignoreMonsters, hullNumber, traceResult.hitEntity, traceResult);
	} while (/*!traceResult.isStartSolid && *//*traceResult.fraction < 1.0f && IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING));*/
	for (/* Empty */; /* Empty */; /* Empty */)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (traceResult.endPosition, destination, ignoreMonsters, hullNumber, traceResult.hitEntity, traceResult);

		if (traceResult.fraction == 1.0f)
		{
			if (traceResult.isStartSolid == false)
				return true;
			else if (IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
			{
				traceResult.isStartSolid = traceResult.isAllSolid = false;

				return true;
			}
			else
				return false;
//			return traceResult.isStartSolid == false || IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING);
		}
		else if (!IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
			return false;
	}
#endif	// if 0
}
/*
void FindHullIntersection (const Math::Vector3D &source, HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult, const Math::Vector3D &mins, const Math::Vector3D &maxs, const HalfLifeEngine::SDK::Constants::TraceIgnore_t traceIgnore, HalfLifeEngine::SDK::Classes::Edict *const entityToSkip)
{
	HalfLifeEngine::SDK::Structures::TraceResult_t tempTraceResult;
	const Math::Vector3D &hullEnd (source.GetInterpolated (traceResult.endPosition, 2.0f));

	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, hullEnd, traceIgnore, entityToSkip, tempTraceResult);

	if (tempTraceResult.fraction < 1.0f)
	{
		traceResult = tempTraceResult;

		return;
	}

	float distanceSquared (MATH_GET_SQUARED (1e6f));
	const Math::Vector3D *const hull[2u] = {&mins, &maxs};
	Math::Vector3D destination;

	for (unsigned char i (0u), j, k; i < 2u; ++i)
		for (j = 0u; j < 2u; ++j)
			for (k = 0u; k < 2u; ++k)
			{
				destination.x = hullEnd.x + hull[i]->x;
				destination.y = hullEnd.y + hull[j]->y;
				destination.z = hullEnd.z + hull[k]->z;

				HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, destination, traceIgnore, entityToSkip, tempTraceResult);

				if (tempTraceResult.fraction == 1.0f)
					continue;

				const float thisDistanceSquared (tempTraceResult.endPosition.GetDistanceSquared (source));

				if (thisDistanceSquared < distanceSquared)
				{
					traceResult = tempTraceResult;
					distanceSquared = thisDistanceSquared;
				}
			}
}
*/
inline void TraceHull (const Math::Vector3D &source, const Math::Vector3D &destination, const Math::Vector2D &mins, const Math::Vector2D &maxs, const HalfLifeEngine::SDK::Constants::TraceIgnore_t traceIgnore, HalfLifeEngine::SDK::Classes::Edict *const/* entityToSkip*/, HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult)
{
	HalfLifeEngine::SDK::Structures::TraceResult_t tempTraceResult;
	float fraction (1.0f);
/*	const Math::Vector2D *const hull[2u] = {&mins, &maxs};
	const Math::Vector3D hullEnd (source.GetInterpolated (destination, 2.0f));
	Math::Vector3D end;

	for (unsigned char i (0u), j; i < 2u; ++i)
		for (j = 0u; j < 2u; ++j)
		{
			end.x = hullEnd.x + hull[i]->x;
			end.y = hullEnd.y + hull[j]->y;
			end.z = hullEnd.z;

			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, end, traceIgnore, entityToSkip, tempTraceResult);

			if (tempTraceResult.fraction == 1.0f)
				continue;

			if (tempTraceResult.fraction < fraction)
			{
				traceResult = tempTraceResult;
				fraction = tempTraceResult.fraction;
			}
		}*/
	Math::Vector2D offset;
	bool isStartSolid (false), isAllSolid (false);

	for (offset.x = mins.x; offset.x < maxs.x; ++offset.x/*/offset.x += maxs.x * 0.5f*/)
		for (offset.y = mins.y; offset.y < maxs.y; ++offset.y/*/offset.y += maxs.y * 0.5f*/)
		{
//			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source + offset, destination + offset, traceIgnore, entityToSkip, tempTraceResult);
			TraceLineThruWalkableEntities (source + offset, destination + offset, traceIgnore, tempTraceResult);

			if (tempTraceResult.isStartSolid)
				isStartSolid = true;

			if (tempTraceResult.isAllSolid)
				isAllSolid = true;

			if (tempTraceResult.fraction < fraction)
			{
				traceResult = tempTraceResult;
				fraction = tempTraceResult.fraction;
			}
		}

	traceResult.isStartSolid = isStartSolid;
	traceResult.isAllSolid = isAllSolid;
}
/*
inline void TraceHull (const Math::Vector3D &source, const Math::Vector3D &destination, const Math::Vector3D &mins, const Math::Vector3D &maxs, const HalfLifeEngine::SDK::Constants::TraceIgnore_t traceIgnore, HalfLifeEngine::SDK::Classes::Edict *const entityToSkip, HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult)
{
	HalfLifeEngine::SDK::Structures::TraceResult_t tempTraceResult;
	float fraction (1.0f);
	Math::Vector3D offset;
	bool isStartSolid (false), isAllSolid (false);

	for (offset.x = mins.x; offset.x < maxs.x; ++offset.x)
		for (offset.y = mins.y; offset.y < maxs.y; ++offset.y)
			for (offset.z = mins.z; offset.z < maxs.z; ++offset.z)
			{
				HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source + offset, destination + offset, traceIgnore, entityToSkip, tempTraceResult);

				if (tempTraceResult.isStartSolid)
					isStartSolid = true;

				if (tempTraceResult.isAllSolid)
					isAllSolid = true;

				if (tempTraceResult.fraction < fraction)
				{
					traceResult = tempTraceResult;
					fraction = tempTraceResult.fraction;
				}
			}

	traceResult.isStartSolid = isStartSolid;
	traceResult.isAllSolid = isAllSolid;
}
*/
inline const bool TraceHullThruWalkableEntities (const Math::Vector3D &source, const Math::Vector3D &destination, const Math::Vector2D &mins, const Math::Vector2D &maxs, const HalfLifeEngine::SDK::Constants::TraceIgnore_t traceIgnore, HalfLifeEngine::SDK::Classes::Edict *const/* entityToSkip*/, HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult)
{
	TraceHull (source, destination, mins, maxs, traceIgnore, NULL, traceResult);

	if (traceResult.fraction == 1.0f)
	{
		if (traceResult.isStartSolid == false)
			return true;
		else if (IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
		{
			traceResult.isStartSolid = traceResult.isAllSolid = false;

			return true;
		}
		else
			return false;
	}
	else if (!IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
		return false;

	for (/* Empty */; /* Empty */; /* Empty */)
	{
		TraceHull (traceResult.endPosition, destination, mins, maxs, traceIgnore, traceResult.hitEntity, traceResult);

		if (traceResult.fraction == 1.0f)
		{
			if (traceResult.isStartSolid == false)
				return true;
			else if (IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
			{
				traceResult.isStartSolid = traceResult.isAllSolid = false;

				return true;
			}
			else
				return false;
		}
		else if (!IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
			return false;
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
*  Trace filter that ignores players, NPCs, and objects that can be walked through
*/
/*class CTraceFilterWalkableEntities : public CTraceFilterNoNPCsOrPlayer	// MADE BY EDWARD!!! - START
{
	private:
		unsigned char m_flags;

	public:
		CTraceFilterWalkableEntities (const IHandleEntity *passentity, int collisionGroup, unsigned char flags) :
			CTraceFilterNoNPCsOrPlayer (passentity, collisionGroup),
			m_flags (flags)
		{ /*VOID*//* }

		bool ShouldHitEntity (IHandleEntity *pServerEntity, int contentsMask)
		{
			if (CTraceFilterNoNPCsOrPlayer::ShouldHitEntity (pServerEntity, contentsMask))
				return (!IsCanWalkThruEntity (EntityFromEntityHandle (pServerEntity), m_flags));

			return false;
		}
};*/	// MADE BY EDWARD!!! - END


extern const bool IsWalkableTraceLineClear (const Math::Vector3D &from, const Math::Vector3D &to, const unsigned char flags = IgnoreEntityFlag_None);
#if 0
namespace NEW
{
//-----------------------------------------------------------------------------
// Interface needed by tree data to do its job
//
// Note that anything that has convex spatial regions with leaves identified
// by indices can implement the ISpatialQuery. All you have to do is to implement
// a class that can answer the 5 questions in the Query interface about the
// spatial subdivision. For example, a K-D tree or a BSP tree could implement
// this interface
//
//-----------------------------------------------------------------------------
abstract_class ISpatialLeafEnumerator
{
public:
	// call back with a leaf and a context
	// The context is completely user defined; it's passed into the enumeration
	// function of ISpatialQuery.
	// This gets called	by the enumeration methods with each leaf
	// that passes the test; return true to continue enumerating,
	// false to stop

	virtual bool EnumerateLeaf( int leaf, int context) = 0;
};

//-----------------------------------------------------------------------------
// Data associated with leaves.
//
// This is a parasitic class that attaches data to the leaves specified by the
// ISpatialQuery sent in to the initialization function. It can't exist without
// a spatial partition of some sort to hold onto.
//-----------------------------------------------------------------------------
abstract_class IBSPTreeDataEnumerator
{
public:
	// call back with a userId and a context
	virtual bool __fastcall EnumerateElement( int userId, int context) = 0;
};

//-----------------------------------------------------------------------------
// What's the max size of the smallest spatial partition?
//-----------------------------------------------------------------------------
#define SPATIAL_SIZE            (256)
#define NODE_HEIGHT_RATIO       (4)
#define SPATIAL_HEIGHT          (SPATIAL_SIZE * NODE_HEIGHT_RATIO)
#define TEST_EPSILON            (0.03125f)

#define INV_SPATIAL_SIZE        ((float)(1.0 / SPATIAL_SIZE))
#define INV_SPATIAL_HEIGHT  ((float)(1.0 / SPATIAL_HEIGHT))

//-----------------------------------------------------------------------------
// Enumerates the leaves in a box
//-----------------------------------------------------------------------------
static bool EnumerateLeavesInBox_R( int node, Math::Vector3D const& mins, Math::Vector3D const& maxs, ISpatialLeafEnumerator* pEnum, int context)
{
         Math::Vector3D cornermin, cornermax;

         while( node >= 0 )
         {
                 dnode_t* pNode = &dnodes[node];
                 dplane_t* pPlane = &dplanes[pNode->planenum];

                 // Arbitrary split plane here
                 for (int i = 0; i < 3; ++i)
                 {
                         if (pPlane->normal[i] >= 0)
                         {
                                 cornermin[i] = mins[i];
                                 cornermax[i] = maxs[i];
                         }
                         else
                         {
                                 cornermin[i] = maxs[i];
                                 cornermax[i] = mins[i];
                         }
                 }

                 if ( (DotProduct( pPlane->normal, cornermax ) - pPlane->dist) <= -TEST_EPSILON )
                 {
                         node = pNode->children[1];
                 }
                 else if ( (DotProduct( pPlane->normal, cornermin ) - pPlane->dist) >= TEST_EPSILON )
                 {
                         node = pNode->children[0];
                 }
                 else
                 {
                         if (!EnumerateLeavesInBox_R( pNode->children[0], mins, maxs, pEnum, context))
                         {
                                 return false;
                         }

                         return EnumerateLeavesInBox_R( pNode->children[1], mins, maxs, pEnum, context);
                 }
         }

         return pEnum->EnumerateLeaf( - node - 1, context);
}

//-----------------------------------------------------------------------------
// Enumerate leaves along a ray
//-----------------------------------------------------------------------------
static bool EnumerateLeavesAlongRay_R( int node, Math::Ray const& ray, Math::Vector3D const& start, Math::Vector3D const& end, ISpatialLeafEnumerator* pEnum, int context)
{
         float front,back;

         while (node >= 0)
         {
                 dnode_t* pNode = &dnodes[node];
                 dplane_t* pPlane = &dplanes[pNode->planenum];

                 if ( pPlane->type <= PLANE_Z )
                 {
                         front = start[pPlane->type] - pPlane->dist;
                         back = end[pPlane->type] - pPlane->dist;
                 }
                 else
                 {
                         front = DotProduct(start, pPlane->normal) - pPlane->dist;
                         back = DotProduct(end, pPlane->normal) - pPlane->dist;
                 }

                 if (front <= -TEST_EPSILON && back <= -TEST_EPSILON)
                 {
                         node = pNode->children[1];
                 }
                 else if (front >= TEST_EPSILON && back >= TEST_EPSILON)
                 {
                         node = pNode->children[0];
                 }
                 else
                 {
                         // test the front side first
                         bool side = front < 0;

                         // Compute intersection point based on the original ray
                         float splitfrac;
                         float denom = DotProduct( ray.m_Delta, pPlane->normal );
                         if ( denom == 0.0f )
                         {
                                 splitfrac = 1.0f;
                         }
                         else
                         {
                                 splitfrac = (   pPlane->dist - DotProduct( ray.m_Start, pPlane->normal ) ) / denom;
                                 if (splitfrac < 0)
                                         splitfrac = 0;
                                 else if (splitfrac > 1)
                                         splitfrac = 1;
                         }

                         // Compute the split point
                         Math::Vector3D split;
                         VectorMA( ray.m_Start, splitfrac, ray.m_Delta, split );

                         bool r = EnumerateLeavesAlongRay_R (pNode->children[side], ray, start, split, pEnum, context);
                         if (!r)
                                 return r;
                         return EnumerateLeavesAlongRay_R (pNode->children[!side], ray, split, end, pEnum, context);
                 }
         }

         return pEnum->EnumerateLeaf( - node - 1, context);
}

bool EnumerateLeavesAlongRay( Math::Ray const& ray, ISpatialLeafEnumerator* pEnum, int context)
{
         if (!ray.m_IsSwept)
         {
                 Math::Vector3D mins, maxs;
                 VectorAdd( ray.m_Start, ray.m_Extents, maxs );
                 VectorSubtract( ray.m_Start, ray.m_Extents, mins );

                 return EnumerateLeavesInBox_R( 0, mins, maxs, pEnum, context);
         }

         // FIXME: Extruded ray not implemented yet
         InternalAssert ( ray.m_IsRay );

         Math::Vector3D end;
         VectorAdd( ray.m_Start, ray.m_Delta, end );
         return EnumerateLeavesAlongRay_R( 0, ray, ray.m_Start, end, pEnum, context);
}

//-----------------------------------------------------------------------------
// Enumerate elements in a particular leaf
//-----------------------------------------------------------------------------
bool CBSPTreeData::EnumerateElementsInLeaf( int leaf, IBSPTreeDataEnumerator* pEnum, int context)
{
/*	#ifdef _DEBUG
	// The enumeration method better damn well not change this list...
         int nCount = CountElementsInLeaf(leaf);
	#endif
*/
         unsigned short idx = m_Leaf[leaf].m_FirstElement;
         while (idx != m_LeafElements.InvalidIndex())
         {
                 BSPTreeDataHandle_t handle = m_LeafElements[idx];
                 if (!pEnum->EnumerateElement( m_Handles[handle].m_UserId, context))
                 {
//                         Assert( CountElementsInLeaf(leaf) == nCount );
                         return false;
                 }
                 idx = m_LeafElements.Next(idx);
         }

//         Assert( CountElementsInLeaf(leaf) == nCount );

         return true;
}

typedef unsigned short SpatialPartitionHandle_t;
//-----------------------------------------------------------------------------
// Gets handle info (for enumerations)
//-----------------------------------------------------------------------------
inline HandleInfo_t& CSpatialPartition::HandleInfo( SpatialPartitionHandle_t handle )
{
	return m_Handle[handle];
}

//-----------------------------------------------------------------------------
// Simple integer hash where the int is equal to the key
static bool Int_CompareFunc( const int &src1, const int &src2 )
 {
         return ( src1 == src2 );
 }


 static unsigned int Int_KeyFunc( const int &src )
 {
         return (unsigned)src;
 }

//-----------------------------------------------------------------------------
// Handle to an renderable in the client leaf system
//-----------------------------------------------------------------------------
DECLARE_TYPED_ENUMERATION (unsigned short, BSPTreeDataHandle_t)
{
	TREEDATA_INVALID_HANDLE = (BSPTreeDataHandle_t)~0
};

// A combination of the PARTITION_ flags above.
//-----------------------------------------------------------------------------
// These are the various partition lists. Note some are server only, some
// are client only
//-----------------------------------------------------------------------------
DECLARE_TYPED_ENUMERATION (int, SpatialPartitionListMask_t)
{
	PARTITION_ENGINE_SOLID_EDICTS		= (1 << 0),		// every edict_t that isn't SOLID_TRIGGER or SOLID_NOT (and static props)
	PARTITION_ENGINE_TRIGGER_EDICTS		= (1 << 1),		// every edict_t that IS SOLID_TRIGGER
	PARTITION_CLIENT_SOLID_EDICTS		= (1 << 2),
	PARTITION_CLIENT_RESPONSIVE_EDICTS	= (1 << 3),		// these are client-side only objects that respond to being forces, etc.
	PARTITION_ENGINE_NON_STATIC_EDICTS	= (1 << 4),		// everything in solid & trigger except the static props, includes SOLID_NOTs
	PARTITION_CLIENT_STATIC_PROPS		= (1 << 5),
	PARTITION_ENGINE_STATIC_PROPS		= (1 << 6),
	PARTITION_CLIENT_NON_STATIC_EDICTS	= (1 << 7),		// everything except the static props

	// Use this to look for all client edicts.
	PARTITION_ALL_CLIENT_EDICTS	= PARTITION_CLIENT_NON_STATIC_EDICTS | PARTITION_CLIENT_STATIC_PROPS | PARTITION_CLIENT_RESPONSIVE_EDICTS | PARTITION_CLIENT_SOLID_EDICTS,

	// These are the only handles in the spatial partition that the game is controlling (everything but static props)
	// These masks are used to handle updating the dirty spatial partition list in each game DLL
	PARTITION_CLIENT_GAME_EDICTS = PARTITION_ALL_CLIENT_EDICTS & ~PARTITION_CLIENT_STATIC_PROPS,
	PARTITION_SERVER_GAME_EDICTS = PARTITION_ENGINE_SOLID_EDICTS | PARTITION_ENGINE_TRIGGER_EDICTS | PARTITION_ENGINE_NON_STATIC_EDICTS
};


//-----------------------------------------------------------------------------
// All the information associated with a particular handle
//-----------------------------------------------------------------------------
struct HandleInfo_t
{
	int                             m_EnumId;               // Prevents objects being sent multiple times in an enumeration
	IHandleEntity   *m_pHandleEntity;
	unsigned int    m_ListFlags;    // which lists is it in?
	BSPTreeDataHandle_t     m_TreeHandle;
	Math::Vector3D                  m_Min;
	Math::Vector3D                  m_Max;
};

 //-----------------------------------------------------------------------------
 // Base class for performing element enumeration
 //-----------------------------------------------------------------------------
 class CEnumBase : public ISpatialLeafEnumerator, public IBSPTreeDataEnumerator
 {
 public:

         // UNDONE: Allow construction based on a set of listIds or a mask?
         CEnumBase( SpatialPartitionListMask_t listMask, bool coarseTest, IPartitionEnumerator* pIterator )
         {
                 m_pIterator = pIterator;
                 m_ListMask = listMask;
                 m_CoarseTest = coarseTest;
                 m_pHash = NULL;
                 // The enumeration context written into each handle is not reentrant.
                 // s_NestLevel is a simple counter that checks for reentrant calls and
                 // uses a hash table to store the visit status for nested calls
                 if ( s_NestLevel > 0 )
                 {
                         m_pHash = new CUtlHash<int>( 64, 0, 0, Int_CompareFunc, Int_KeyFunc );
                 }
                 ++s_NestLevel;
         }
         ~CEnumBase()
         {
                 delete m_pHash;
                 m_pHash = NULL;
                 --s_NestLevel;
         }

         bool EnumerateLeaf( int leaf, int context)
         {
                 return EnumerateElementsInLeaf( leaf, this, context);
         }

         bool ShouldVisit( int partitionHandle, HandleInfo_t &handleInfo, int enumId )
         {
                 // Keep going if this dude isn't in the list
                 if ( !( m_ListMask & handleInfo.m_ListFlags ) )
                         return false;

                 if ( m_pHash )
                 {
                         if ( m_pHash->InvalidHandle() != m_pHash->Find( partitionHandle )  )
                                 return false;
                         m_pHash->Insert( partitionHandle );
                 }
                 else
                 {
                         // Don't bother if we hit this one already...
                         if ( handleInfo.m_EnumId == enumId )
                                 return false;

                         // Mark this so we don't test it again
                         handleInfo.m_EnumId = enumId;
                 }

                 return true;
         }

         bool __fastcall EnumerateElement( int partitionHandle, int enumId)
         {
                 // Get at the handle
                 HandleInfo_t& handleInfo = g_SpatialPartition.HandleInfo( partitionHandle );

                 if ( !ShouldVisit( partitionHandle, handleInfo, enumId))
                         return true;

                 // If it's not a coarse test, actually do the intersection
                 if ( !m_CoarseTest )
                 {
                         if (!Intersect( handleInfo ))
                                 return true;
                 }

                 // Okay, this one is good...
                 IterationRetval_t retVal = m_pIterator->EnumElement( handleInfo.m_pHandleEntity );

                 return (retVal != ITERATION_STOP);
         }
         // Various versions must implement this
         virtual bool Intersect( HandleInfo_t& handleInfo ) = 0;
 private:
         static int s_NestLevel;
 protected:
         IPartitionEnumerator* m_pIterator;
         int             m_ListMask;
         bool    m_CoarseTest;
         CUtlHash<int> *m_pHash;
};

int CEnumBase::s_NestLevel = 0;

//-----------------------------------------------------------------------------
// Gets all entities along a ray...
//-----------------------------------------------------------------------------
class CEnumRay : public CEnumBase
{
public:
         CEnumRay( SpatialPartitionListMask_t listMask, bool coarseTest, IPartitionEnumerator* pIterator, const Math::Ray& ray ) :
                 CEnumBase( listMask, coarseTest, pIterator )
         {
                 m_pRay = &ray;
         }

         bool Intersect( HandleInfo_t& handleInfo )
         {
                 if (m_pRay->m_IsRay)
                 {
                         return IsBoxIntersectingRay( handleInfo.m_Min, handleInfo.m_Max, m_pRay->m_Start, m_pRay->m_Delta );
                 }
                 else
                 {
                         // Thicken the box by the ray extents
                         Math::Vector3D bmin, bmax;
                         VectorAdd( handleInfo.m_Max, m_pRay->m_Extents, bmax );
                         VectorSubtract( handleInfo.m_Min, m_pRay->m_Extents, bmin );

                         return IsBoxIntersectingRay( bmin, bmax, m_pRay->m_Start, m_pRay->m_Delta );
                 }
         }

 private:
         const Math::Ray* m_pRay;
 };

 void EnumerateElementsAlongRay( SpatialPartitionListMask_t listMask,
         const Math::Ray& ray, bool coarseTest, IPartitionEnumerator* pIterator )
 {
         // If this assertion fails, you're using a list
         // at a point where the spatial partition elements aren't set up!
         InternalAssert( (listMask & m_nSuppressedListMask) == 0);

         // Early-out.
         if ( listMask == 0 )
                 return;

         // No ray? Just do a simpler box test
         if (!ray.m_IsSwept)
         {
                 Math::Vector3D mins, maxs;
                 VectorSubtract( ray.m_Start, ray.m_Extents, mins );
                 VectorAdd( ray.m_Start, ray.m_Extents, maxs );
                 EnumerateElementsInBox( listMask, mins, maxs, coarseTest, pIterator );
                 return;
         }

         CEnumRay enumRay( listMask, coarseTest, pIterator, ray );
         ++m_EnumId;
         EnumerateLeavesAlongRay( ray, &enumRay, m_EnumId);
}
}
#endif	// if 0
#endif	// ifndef UTILITIES_INCLUDED