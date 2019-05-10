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
//
// Version: $ID:$
//

#include <Core.hpp>

const DynamicString WorldMap::FileExtention ("MAP");

void WorldMap::MakeHullFaces (/*const brush_t* const b, brushhull_t *const h*/void)
{
//	bface_t *f;
//	bface_t *f2;
	unsigned short clipNodeIndex, clipNodeIndex2;
//	unsigned int i;
	const HalfLifeEngine::SDK::Constants::Hull_t hullIndex (HalfLifeEngine::SDK::Constants::Hull_Human);

	m_windings.DeleteAndPurgeAll ();

//Label_Restart:
//	h->bounds.reset ();

//	for (unsigned short modelIndex (0u); modelIndex < m_bspFile.models.GetElementNumber (); ++modelIndex)
	{
		const HalfLifeEngine::SDK::Structures::DModel_t &model (m_bspFile.models[0u/*modelIndex*/]);
		const unsigned short clipNodesNumber (static_cast <unsigned short> (model.headNode[hullIndex + 1u]));

		// for each face in this brushes hull
		for (clipNodeIndex = static_cast <unsigned short> (model.headNode[hullIndex]); clipNodeIndex < clipNodesNumber; ++clipNodeIndex)
//	for (clipNodeIndex = 0u; clipNodeIndex < m_bspFile.clipNodes.GetElementNumber (); ++clipNodeIndex)
	{
		const unsigned short planeIndex (static_cast <unsigned short> (m_bspFile.clipNodes[clipNodeIndex].planeNum));
		Math::Winding *const w (new Math::Winding (m_bspFile.planes[planeIndex]));

		for (clipNodeIndex2 = static_cast <unsigned short> (model.headNode[hullIndex]); clipNodeIndex2 < clipNodesNumber; ++clipNodeIndex2)
//		for (clipNodeIndex2 = 0u; clipNodeIndex2 < m_bspFile.clipNodes.GetElementNumber (); ++clipNodeIndex2)
		{
			const unsigned short planeIndex2 (static_cast <unsigned short> (m_bspFile.clipNodes[clipNodeIndex2].planeNum));

			if (planeIndex2 == planeIndex)
				continue;

			if (!w->Chop (m_bspFile.planes[planeIndex2]))	// Nothing left to chop (Math::Winding::GetArea() will return 0 for us in this case for below)
				break;
		}

//		InternalAssert (w->GetArea () >= 0.1f);	// there's a bad brush (face not contributing) (OCCURS!!!!!!!!!!!!!)
		if (w->GetArea () < 0.1f)	// there's a bad brush (face not contributing) (OCCURS!!!!!!!!!!!!!)
		{
			delete w;
			continue;
		}
/*
		if (w->GetArea () < 0.1f)
		{
			InternalAssert (w->GetArea () != 0.0f);	// there's a bad brush (face not contributing)

			delete w;

			if (h->faces == f)
				h->faces = f->next;
			else
			{
				for (f2 = h->faces; f2->next != f; f2 = f2->next);

				f2->next = f->next;
			}

			goto Label_Restart;
		}
		else*/
		{
			m_windings += w;
/*			f->w = w;
			f->contents = CONTENTS_EMPTY;

			for (i = 0u; i < w->m_NumPoints; ++i)
				h->bounds.add (w->m_Points[i]);
*/		}
	}
	}
/*
	for (i = 0u; i < 3u; ++i)
	{
		if (h->bounds.m_Mins[i] < -BOGUS_RANGE / 2 || h->bounds.m_Maxs[i] > BOGUS_RANGE / 2)
		{
			Fatal (assume_BRUSH_OUTSIDE_WORLD, "Entity %i, Brush %i: outside world(+/-%d): (%.0f,%.0f,%.0f)-(%.0f,%.0f,%.0f)",
					b->entitynum, b->brushnum,
					BOGUS_RANGE / 2,
					h->bounds.m_Mins[0], h->bounds.m_Mins[1], h->bounds.m_Mins[2],
					h->bounds.m_Maxs[0], h->bounds.m_Maxs[1], h->bounds.m_Maxs[2]);
		}
	}*/

	AddLogEntry (true, LogLevel_Default, false, "WorldMap::MakeHullFaces(): Done, m_windings.GetElementNumber()=%u, clipNodesNumber=%u.", m_windings.GetElementNumber (), m_bspFile.models[0u].headNode[hullIndex + 1u] - m_bspFile.models[0u].headNode[hullIndex]);
}
/*
====================
ParseEntity

Parses an edict out of the given string, returning the new position
ed should be a properly initialized empty edict.
Used for initial level load and for savegames.
====================
*/
char *WorldMap::ParseEntity (char *data, Entity_t &entity)
{
	EntityPair_t e;

	// initialize the default entity variables fields...
	entity.Reset ();

	for (;;)
	{
		if (!ParseEntityPair (data, e))
			break;

//		AddLogEntry (false, LogLevel_Default, true, "ParseEntity(): Parsing entity, key: \"%s\", value: \"%s\".", e.key.GetData (), e.value.GetData ());

		if (e.key == "classname")
			entity.className = e.value;
		else if (e.key == "targetname")
			entity.targetName = e.value;
		else if (e.key == "origin")
		{
			if (sscanf (e.value, "%f %f %f", &entity.origin.x, &entity.origin.y, &entity.origin.z) != Math::Vector3D::AxisID_Total)
			{
				entity.origin = Math::Vector3D::ZeroValue;

				AddLogEntry (true, LogLevel_Warning, false, "Can't read \"origin\" vector \"%s\" for entity \"%s\" (%u)!", e.value.GetData (), entity.className.GetData (), m_bspFile.entities.GetElementNumber ());
			}
		}
		else if (e.key == "angle")
		{
			if (sscanf (e.value, "%f", &entity.angles.yaw) != 1u)	// set the yaw angle...
			{
				entity.angles.yaw = 0.0f;

				AddLogEntry (true, LogLevel_Warning, false, "Can't read \"angle\" value \"%s\" for entity \"%s\" (%u)!", e.value.GetData (), entity.className.GetData (), m_bspFile.entities.GetElementNumber ());
			}
		}
		else if (e.key == "angles")
		{
			if (sscanf (e.value, "%f %f %f", &entity.angles.pitch, &entity.angles.yaw, &entity.angles.roll) != Math::Angles3D::AngleID_Total)	// set the angles...
			{
				entity.angles = Math::Angles3D::ZeroValue;

				AddLogEntry (true, LogLevel_Warning, false, "Can't read \"angles\" value \"%s\" for entity \"%s\" (%u)!", e.value.GetData (), entity.className.GetData (), m_bspFile.entities.GetElementNumber ());
			}
		}
		else if (e.key == "renderamt")
		{
			unsigned short renderAmount (0u);

			if (sscanf (e.value, "%hu", &renderAmount) == 1u && renderAmount <= 255u)
				entity.renderAmount = renderAmount / 255.0f;
			else
				AddLogEntry (true, LogLevel_Warning, false, "Can't read \"renderamt\" value \"%s\" for entity \"%s\" (%u)!", e.value.GetData (), entity.className.GetData (), m_bspFile.entities.GetElementNumber ());
		}
		else if (e.key == "rendercolor")
		{
			unsigned short colorRed (0u), colorGreen (0u), colorBlue (0u);

			if (sscanf (e.value, "%hu %hu %hu", &colorRed, &colorGreen, &colorBlue) == Color <float>::ID_Total && colorRed <= 255u && colorGreen <= 255u && colorBlue <= 255u)
			{
				entity.renderColor.red   = colorRed / 255.0f;
				entity.renderColor.green = colorGreen / 255.0f;
				entity.renderColor.blue  = colorBlue / 255.0f;
			}
			else
				AddLogEntry (true, LogLevel_Warning, false, "Can't read \"rendercolor\" pair for entity \"%s\" (%u)!", entity.className.GetData (), m_bspFile.entities.GetElementNumber ());
		}
		else if (e.key == "model")
		{
			if (sscanf (e.value, "*%hu", &entity.brushModelIndex) != 1u || entity.brushModelIndex >= m_bspFile.models.GetElementNumber ())
			{
				AddLogEntry (true, LogLevel_Warning, false, "Can't read \"model\" index value \"%s\" for entity \"%s\" (%u)!", e.value.GetData (), entity.className.GetData (), m_bspFile.entities.GetElementNumber ());

				continue;
			}

			const HalfLifeEngine::SDK::Structures::DModel_t &model (m_bspFile.models[entity.brushModelIndex]);

			// calculate the origin for this brush model...
			entity.origin = model.boundingBox.GetCenter ();
		}
		else if (e.key == "spawnflags")
		{
			if (sscanf (e.value, "%hu", &entity.spawnFlags) != 1u)
			{
				entity.spawnFlags = 0u;

				AddLogEntry (true, LogLevel_Warning, false, "Can't read \"spawnflags\" value \"%s\" for entity \"%s\" (%u)!", e.value.GetData (), entity.className.GetData (), m_bspFile.entities.GetElementNumber ());
			}
		}
	}

	// Post initialization....
	/// @todo MAKE MORE CHECKS FOR BREAKABLES, PUSHABLES, ETC.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (entity.className == "worldspawn" || entity.className == "func_wall" || (entity.className == "func_breakable" && entity.spawnFlags == 1u))
		entity.isStaticSolid = true;

	return data;
}

const bool WorldMap::ParseEntityPair (char *&data, EntityPair_t &epair)
{
	// parse key
	data = HalfLifeEngine::Utilities::COM_Parse (data);

	if (HalfLifeEngine::Utilities::com_token[0u] == '}')
		return false;

	if (data == NULL)
		AddLogEntry (false, LogLevel_Critical, true, "WorldMap::ParseEntityPair(): EOF without closing brace!");

	epair.key = HalfLifeEngine::Utilities::com_token;

	// parse value
	data = HalfLifeEngine::Utilities::COM_Parse (data);

	if (HalfLifeEngine::Utilities::com_token[0u] == '}')
	{
		AddLogEntry (false, LogLevel_Critical, true, "WorldMap::ParseEntityPair(): closing brace without data!");

		return true;
	}

	if (data == NULL)
		AddLogEntry (false, LogLevel_Critical, true, "WorldMap::ParseEntityPair(): EOF without closing brace!");

	epair.value = HalfLifeEngine::Utilities::com_token;

	return true;
}

void WorldMap::ReadEntitiesFromStringData (const LongDynamicString &entitiesData)
{
	// Parses the dentdata string into entities

	AddLogEntry (false, LogLevel_Default, true, "WorldMap::ReadEntitiesFromStringData(): called!");

	InternalAssert (!entitiesData.IsEmpty ());

	m_bspFile.entities.RemoveAll ();
/*
================
ED_LoadFromFile

The entities are directly placed in the array, rather than allocated with
ED_Alloc, because otherwise an error loading the map would have entity
number references out of order.

Creates a server's entity / program execution context by
parsing textual entity definitions out of an ent file.

Used for both fresh maps and savegame loads.  A fresh map would also need
to call ED_CallSpawnFunctions () to let the objects initialize themselves.
================
*/
	Entity_t entity;
	char *data (entitiesData);

	for (;;)
	{
		// parse the opening brace
		data = HalfLifeEngine::Utilities::COM_Parse (data);

		if (data == NULL)
			break;

		if (HalfLifeEngine::Utilities::com_token[0u] != '{')
			AddLogEntry (false, LogLevel_Critical, true, "ReadEntitiesFromStringData(): found %s when expecting {", HalfLifeEngine::Utilities::com_token);

		data = ParseEntity (data, entity);

		m_bspFile.entities += entity;
	}

	AddLogEntry (true, LogLevel_Default, false, "%u entities succesfully loaded from BSP map file!", m_bspFile.entities.GetElementNumber ());
}
#if 0
void WorldMap::GetLadderPossibleClimbableMidSides (const HalfLifeEngine::SDK::Structures::DModel_t &model, LadderFaceInfoArrays &faceInfoArrays) const
{
	/// @note This works only for trivial ladders!

	unsigned short face_index;
	int face_index, edges, edge_index;
	const unsigned char vertexesNumber (4u);
	Math::Vector3D vertexes[vertexesNumber];	// hold up to four vertexes for each face
	typedef DynamicArray <float, unsigned char> VertexesLengthSquaredArray;
	VertexesLengthSquaredArray vertexesLengthSquaredArray;
	float tempVertexesLengthSquared;
	LadderFaceInfo_t tempFaceInfo;
	LadderFaceInfoArray faceInfoArray;

	for (face_index = 0u; face_index < model.facesNum; ++face_index)
	{
		const HalfLifeEngine::SDK::Structures::DFace_t &face (m_bspFile.faces[static_cast <unsigned short> (model.firstFaceIndex + face_index)]);

		// Reliability check.
		InternalAssert (face.surfEdgesNum == vertexesNumber);

		tempFaceInfo.planeIndex = face.planeNum;
		tempFaceInfo.midSides[0u].segment[0u] = Math::Vector3D::ZeroValue;
		tempFaceInfo.midSides[0u].segment[1u] = Math::Vector3D::ZeroValue;
		tempFaceInfo.midSides[0u].lengthSquared = 0.0f;
		tempFaceInfo.midSides[1u].segment[0u] = Math::Vector3D::ZeroValue;
		tempFaceInfo.midSides[1u].segment[1u] = Math::Vector3D::ZeroValue;
		tempFaceInfo.midSides[1u].lengthSquared = 0.0f;

		tempVertexesLengthSquared = 0.0f;

		for (edges = 0u; edges < face.surfEdgesNum; ++edges)
		{
			// get the coordinates of the vertex of this edge...
			edge_index = m_bspFile.surfEdges[static_cast <unsigned int> (face.firstSurfEdgeIndex + edges)];

			if (edge_index < 0)
			{
				const HalfLifeEngine::SDK::Structures::DEdge_t &e (m_bspFile.edges[static_cast <unsigned int> (-edge_index)]);

				vertexes[edges] = m_bspFile.vertexes[e.vertices[1]].point;
			}
			else
			{
				const HalfLifeEngine::SDK::Structures::DEdge_t &e (m_bspFile.edges[static_cast <unsigned int> (edge_index)]);

				vertexes[edges] = m_bspFile.vertexes[e.vertices[0]].point;
			}

			if (edges >= 1u)
			{
				// S = a * b
				tempVertexesLengthSquared += vertexes[edges].GetDistanceSquared (vertexes[edges - 1u]);
			}
		}

		// S = a * b
		tempVertexesLengthSquared += vertexes[face.surfEdgesNum - 1u].GetDistanceSquared (vertexes[0u]);

		tempFaceInfo.midSides[0u].segment[0u] = vertexes[0u].GetMidPoint (vertexes[1u]);
		tempFaceInfo.midSides[0u].segment[1u] = vertexes[2u].GetMidPoint (vertexes[3u]);

		tempFaceInfo.midSides[0u].lengthSquared = tempFaceInfo.midSides[0u].segment[0u].GetDistanceSquared (tempFaceInfo.midSides[0u].segment[1u]);

		tempFaceInfo.midSides[1u].segment[0u] = vertexes[1u].GetMidPoint (vertexes[2u]);
		tempFaceInfo.midSides[1u].segment[1u] = vertexes[3u].GetMidPoint (vertexes[0u]);

		tempFaceInfo.midSides[1u].lengthSquared = tempFaceInfo.midSides[1u].segment[0u].GetDistanceSquared (tempFaceInfo.midSides[1u].segment[1u]);

		if (tempFaceInfo.midSides[0u].segment[0u].z > tempFaceInfo.midSides[0u].segment[1u].z)
			SwapElements (tempFaceInfo.midSides[0u].segment[0u], tempFaceInfo.midSides[0u].segment[1u]);

		if (tempFaceInfo.midSides[1u].segment[0u].z > tempFaceInfo.midSides[1u].segment[1u].z)
			SwapElements (tempFaceInfo.midSides[1u].segment[0u], tempFaceInfo.midSides[1u].segment[1u]);

		if (tempFaceInfo.midSides[0u].lengthSquared < tempFaceInfo.midSides[1u].lengthSquared || tempFaceInfo.midSides[0u].segment[1u].z - tempFaceInfo.midSides[0u].segment[0u].z < tempFaceInfo.midSides[1u].segment[1u].z - tempFaceInfo.midSides[1u].segment[0u].z)
			SwapElements (tempFaceInfo.midSides[0u], tempFaceInfo.midSides[1u]);

		LadderFaceInfoArray::IndexType_t index (0u);

		for (/* Empty */; index < faceInfoArray.GetElementNumber (); ++index)
			if (vertexesLengthSquaredArray[index] > tempVertexesLengthSquared)
			{
				faceInfoArray.Insert (index, tempFaceInfo);
				vertexesLengthSquaredArray.Insert (index, tempVertexesLengthSquared);

				break;
			}

		if (index == faceInfoArray.GetElementNumber ())
		{
			faceInfoArray += tempFaceInfo;
			vertexesLengthSquaredArray += tempVertexesLengthSquared;
		}
	}

	InternalAssert (faceInfoArray.GetElementNumber () == model.facesNum);
	InternalAssert (faceInfoArray.GetElementNumber () % 2u == 0u);

	faceInfoArrays.RemoveAll ();

	LadderFaceInfoArray tempFaceInfoArray (faceInfoArray.GetFirstElement ());

	for (LadderFaceInfoArray::IndexType_t index (1u/* Skip smallest element */); index < faceInfoArray.GetElementNumber (); ++index)
	{
		if (vertexesLengthSquaredArray[index - 1u] != vertexesLengthSquaredArray[index])
		{
			InternalAssert (tempFaceInfoArray.GetElementNumber () % 2u == 0u);

			faceInfoArrays += tempFaceInfoArray;

			tempFaceInfoArray.RemoveAll ();
		}

		FaceInfoArray::IndexType_t index1 (0u);

		for (/* Empty */; index1 < tempFaceInfoArray.GetElementNumber (); ++index1)
			if (tempFaceInfoArray[index1].midSides[1u].lengthSquared <= faceInfoArray[index].midSides[1u].lengthSquared)
			{
				tempFaceInfoArray.Insert (index1, faceInfoArray[index]);

				break;
			}

		if (index1 == tempFaceInfoArray.GetElementNumber ())
			tempFaceInfoArray += faceInfoArray[index];
	}

	InternalAssert (!tempFaceInfoArray.IsEmpty ());
	InternalAssert (tempFaceInfoArray.GetElementNumber () % 2u == 0u);

	faceInfoArrays += tempFaceInfoArray;

	InternalAssert (faceInfoArrays.GetElementNumber () == faceInfoArray.GetElementNumber () / 2u);

	faceInfoArrays.PopFront ();	// Remove smallest element blindly.

	// Special case for ladders with faces more than 6 (these ladders are divided on two with the same planes for each!)
	if (faceInfoArrays.GetElementNumber () > 3u)
	{
		for (LadderFaceInfoArrays::IndexType_t index0 (faceInfoArrays.GetElementNumber ()); index0 > 0u/* Skip smallest element */; /* Empty */)
		{
			--index0;

			LadderFaceInfoArray &faceInfoArrayWithBiggestLength (faceInfoArrays[index0]);

			for (LadderFaceInfoArray::IndexType_t index (0u); index < faceInfoArrayWithBiggestLength.GetElementNumber (); ++index)
			{
				for (LadderFaceInfoArrays::IndexType_t index1 (0u/* Skip smallest element */); index1 < index0/* Skip biggest element */; ++index1)
				{
					const LadderFaceInfoArray &facesToSplit (faceInfoArrays[index1]);

					if (faceInfoArrayWithBiggestLength[index].planeIndex != facesToSplit[index].planeIndex)
						continue;

					for (unsigned char midSideIndex (0u); midSideIndex < 2u; ++midSideIndex)
					{
						if (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u] == facesToSplit[index].midSides[midSideIndex].segment[1u])
						{
							InternalAssert (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u].IsPointBelongsToSegment (facesToSplit[index].midSides[midSideIndex].segment[0u], facesToSplit[index].midSides[midSideIndex].segment[1u]));

							const Math::Vector3D &otherSegmentOldMid (faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[0u].GetMidPoint (faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[1u]));

							InternalAssert (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u].GetMidPoint (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u]) == otherSegmentOldMid);

							faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u] = facesToSplit[index].midSides[midSideIndex].segment[0u];

							const Math::Vector3D &segmentMid (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u].GetMidPoint (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u]));
							const Math::Vector3D &otherSegmentOldHalf ((otherSegmentOldMid - faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[1u]));

							// Update other mid side....
							faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[0u] = segmentMid + otherSegmentOldHalf;
							faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[1u] = segmentMid - otherSegmentOldHalf;

							// Recalculate length....
							faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].lengthSquared = tempFaceInfo.midSides[midSideIndex].segment[0u].GetDistanceSquared (tempFaceInfo.midSides[midSideIndex].segment[1u]);

							// Is both faces checked?
							if (index + 1u == faceInfoArrayWithBiggestLength.GetElementNumber ())
							{
								faceInfoArrays.Remove (index1, 1u);

								index1 = index0;	// Stop (this will break loop 'index1 < index0')
							}

							break;
						}
						else if (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u] == facesToSplit[index].midSides[midSideIndex].segment[0u])
						{
							InternalAssert (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u].IsPointBelongsToSegment (facesToSplit[index].midSides[midSideIndex].segment[1u], facesToSplit[index].midSides[midSideIndex].segment[0u]));

							const Math::Vector3D &otherSegmentOldMid (faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[0u].GetMidPoint (faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[1u]));

							InternalAssert (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u].GetMidPoint (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u]) == otherSegmentOldMid);

							faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u] = facesToSplit[index].midSides[midSideIndex].segment[1u];

							const Math::Vector3D &segmentMid (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u].GetMidPoint (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u]));
							const Math::Vector3D &otherSegmentOldHalf ((otherSegmentOldMid - faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[1u]));

							// Update other mid side....
							faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[0u] = segmentMid + otherSegmentOldHalf;
							faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[1u] = segmentMid - otherSegmentOldHalf;

							// Recalculate length....
							faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].lengthSquared = tempFaceInfo.midSides[midSideIndex].segment[0u].GetDistanceSquared (tempFaceInfo.midSides[midSideIndex].segment[1u]);

							// Is both faces checked?
							if (index + 1u == faceInfoArrayWithBiggestLength.GetElementNumber ())
							{
								faceInfoArrays.Remove (index1, 1u);

								index1 = index0;	// Stop (this will break loop 'index1 < index0')
							}

							break;
						}
					}
				}
			}
		}
	}

	InternalAssert (faceInfoArrays.GetElementNumber () == 2u);
}
#endif	// if 0
void WorldMap::PrintEntities (void) const
{
	AddLogEntry (true, LogLevel_Default, true, "Printing %u entities from map BSP file....", m_bspFile.entities.GetElementNumber ());

	for (unsigned short index (0u); index < m_bspFile.entities.GetElementNumber (); ++index)
	{
		const Entity_t &entity (m_bspFile.entities[index]);

		AddLogEntry (true, LogLevel_Default, true, "Entity %u:\n{\n\tclass name: \"%s\"\n\ttarget name: \"%s\"\n\torigin=[%.2f, %.2f, %.2f]\n\tangles=[%.2f, %.2f, %.2f]\n\trender mode=%u\n\trender amount=%.2f\n\trender color=[%.2f, %.2f, %.2f]\n\trender fx=%u\n\thealth=%i\n\tbrush model index=%u\n\tspawn flags=%u\n}\n", index + 1u, entity.className.GetData (), entity.targetName.GetData (), entity.origin.x, entity.origin.y, entity.origin.z, entity.angles.pitch, entity.angles.yaw, entity.angles.roll, entity.renderMode, entity.renderAmount, entity.renderColor.red, entity.renderColor.green, entity.renderColor.blue, entity.renderFx, entity.health, entity.brushModelIndex, entity.spawnFlags);
	}
}

const short WorldMap::FindEntityByClassName (short index, const char *const className) const
{
	// index should be -1 to start at first entity...
	++index;

	for (/* Empty */; index < m_bspFile.entities.GetElementNumber (); ++index)
		if (m_bspFile.entities[static_cast <unsigned short> (index)].className == className)
			return index;

	return -1;	// entity not found
}

/*
=================
MakeCollisionPointHull (MakeHull0)

Deplicate the drawing hull structure as a clipping hull
=================
*/
void WorldMap::MakeCollisionPointHull (void)
{
/*
////////FROM ZHLT///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (normal.x != 0.0f)
			origin.x += normal.x * (normal.x < 0.0f ? -hullSizes[hullIndex].mins.x : hullSizes[hullIndex].maxs.x);

		if (normal.y != 0.0f)
			origin.y += normal.y * (normal.y < 0.0f ? -hullSizes[hullIndex].mins.y : hullSizes[hullIndex].maxs.y);

		if (normal.z != 0.0f)
			origin.z += normal.z * (normal.z < 0.0f ? -hullSizes[hullIndex].mins.z : hullSizes[hullIndex].maxs.z);
////////OTHER METHOD////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		origin.x += normal.x <= 0.0f ? hullSizes[hullIndex].mins.x : hullSizes[hullIndex].maxs.x;
		origin.y += normal.y <= 0.0f ? hullSizes[hullIndex].mins.y : hullSizes[hullIndex].maxs.y;
		origin.z += normal.z <= 0.0f ? hullSizes[hullIndex].mins.z : hullSizes[hullIndex].maxs.z;
////////FROM HL2 SOURCE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		const Math::Vector3D offset
		(
			plane.normal.x >= 0.0f ? hullSizes[hullIndex].mins.x : hullSizes[hullIndex].maxs.x,
			plane.normal.y >= 0.0f ? hullSizes[hullIndex].mins.y : hullSizes[hullIndex].maxs.y,
			plane.normal.z >= 0.0f ? hullSizes[hullIndex].mins.z : hullSizes[hullIndex].maxs.z
		);

		plane.distance -= offset | plane.normal;
*/
	DynamicArray <BSPMapFile_t::PlanesArray_t::IndexType_t> modifiedPlanesIndexes;
	const HalfLifeEngine::SDK::Constants::Hull_t hullIndex (HalfLifeEngine::SDK::Constants::Hull_Head/*Human*//*! @todo FIX IT !!!*/);

//	for (unsigned short modelIndex (0u); modelIndex < m_bspFile.models.GetElementNumber (); ++modelIndex)
	{
//		const HalfLifeEngine::SDK::Structures::DModel_t &model (m_bspFile.models[modelIndex]);

//		for (unsigned short clipNodeIndex (static_cast <unsigned short> (model.headNode[hullIndex])); clipNodeIndex < model.headNode[HalfLifeEngine::SDK::Constants::Hull_Large]; ++clipNodeIndex)
		for (unsigned short clipNodeIndex (0u); clipNodeIndex < m_bspFile.clipNodes.GetElementNumber (); ++clipNodeIndex)
		{
			const unsigned short planeIndex (static_cast <unsigned short> (m_bspFile.clipNodes[clipNodeIndex].planeNum));

			if (modifiedPlanesIndexes.IsMember (planeIndex))
				continue;

			HalfLifeEngine::SDK::Structures::DPlane_t &plane (m_bspFile.planes[planeIndex]);

			modifiedPlanesIndexes += planeIndex;

			/// @todo MAKE IT WORK CORRECTLY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			// push the plane out appropriately for hull mins/maxs

			const Math::Vector3D offset
			(
				plane.normal.x >= 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].mins.x : HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].maxs.x,
				plane.normal.y >= 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].mins.y : HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].maxs.y,
				plane.normal.z >= 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].mins.z : HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].maxs.z
			);

			plane.distance -= offset | plane.normal;
/*
			const float oldDistance (plane.distance), totalOffset (offset | plane.normal);

			if (totalOffset > 0.0f && plane.distance > 0.0f)
				plane.distance -= totalOffset;
			else if (totalOffset < 0.0f && plane.distance < 0.0f)
				plane.distance += totalOffset;
			else
				plane.distance += totalOffset;

			AddLogEntry (true, LogLevel_Default, false, "WorldMap::MakeCollisionPointHull(): Shifting plane %u: [normal=[%.2f, %.2f, %.2f], distance: old=%.2f, new=%.2f (totalOffset=%.2f)].", planeIndex, plane.normal.x, plane.normal.y, plane.normal.z, oldDistance, plane.distance, totalOffset);
*/		}
	}
}
#if 0
/*
=================
Mod_BoundsFromClipNode -- johnfitz

update the model's clipmins and clipmaxs based on each node's plane.

This works because of the way brushes are expanded in hull generation.
Each brush will include all six axial planes, which bound that brush.
Therefore, the bounding box of the hull can be constructed entirely
from axial planes found in the clipnodes for that hull.
=================
*/
void WorldMap::Mod_BoundsFromClipNode (model_t *const mod, const HalfLifeEngine::SDK::Constants::Hull_t hull, const int nodenum)
{
	// process hull1 (we don't need to process hull2 becuase there's no such thing as a brush that appears in hull2 but not hull1)

	// (disabled for now becuase it fucks up on rotating models)

	if (nodenum < 0)
		return;	// hit a leafnode

	mclipnode_t *node = &mod->clipnodes[nodenum];
	mplane_t *plane = mod->hulls[hull].planes + node->planenum;

	if (plane->type <= HalfLifeEngine::SDK::Structures::DPlane_t::Type_Z)
	{
		if (plane->normal[plane->type] < 0.0f)
			mod->clipmins[plane->type] = Math::GetMinimumValueBetween (mod->clipmins[plane->type], -plane->distance - mod->hulls[hull].clip_mins[plane->type]);
		else
			mod->clipmaxs[plane->type] = Math::GetMaximumValueBetween (mod->clipmaxs[plane->type], plane->distance - mod->hulls[hull].clip_maxs[plane->type]);
	}
	else
	{
		// skip nonaxial planes; don't need them
	}

	Mod_BoundsFromClipNode (mod, hull, node->children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Front]);
	Mod_BoundsFromClipNode (mod, hull, node->children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Back]);
}
#endif	// if 0
template <typename nodeArrayType> void WorldMap::RecursiveCountClipNodesForOneHull (const nodeArrayType &nodes, const short clipNodeIndex, ClipNodesIndexesArray_t &clipNodesIndexesArray, BSPMapFile_t::ClipNodesArray_t::IndexType_t &count) const
{
	// Can also used for drawing nodes....

	if (clipNodeIndex < 0)
		return;	// Done

	InternalAssert (!clipNodesIndexesArray.IsMember (static_cast <const BSPMapFile_t::ClipNodesArray_t::IndexType_t> (clipNodeIndex)));

	clipNodesIndexesArray += static_cast <const BSPMapFile_t::ClipNodesArray_t::IndexType_t> (clipNodeIndex);

	// Increment count....
	++count;

	const HalfLifeEngine::SDK::Structures::DClipNode_t &clipNode (nodes[static_cast <const BSPMapFile_t::ClipNodesArray_t::IndexType_t> (clipNodeIndex)]);
//	const HalfLifeEngine::SDK::Structures::DPlane_t &plane (m_bspFile.planes[static_cast <const BSPMapFile_t::PlanesArray_t::IndexType_t> (clipNode.planeNum)]);

	RecursiveCountClipNodesForOneHull (nodes, clipNode.children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Front], clipNodesIndexesArray, count);
	RecursiveCountClipNodesForOneHull (nodes, clipNode.children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Back], clipNodesIndexesArray, count);
}
void WorldMap::CountClipNodesForOneHull (void)
{
	BSPMapFile_t::ClipNodesArray_t::IndexType_t counts[HalfLifeEngine::SDK::Constants::Hull_Total];
	BSPMapFile_t::ClipNodesArray_t::IndexType_t counts1[HalfLifeEngine::SDK::Constants::Hull_Total];

/*	for (*/BSPMapFile_t::ModelsArray_t::IndexType_t modelIndex (0u);// modelIndex < m_bspFile.models.GetElementNumber (); ++modelIndex)
	{
		const HalfLifeEngine::SDK::Structures::DModel_t &model (m_bspFile.models[modelIndex]);
		const bool isLastModelIndex (modelIndex + 1u == m_bspFile.models.GetElementNumber ());

		counts[HalfLifeEngine::SDK::Constants::Hull_Point] = 0u;
		counts1[HalfLifeEngine::SDK::Constants::Hull_Point] = static_cast <BSPMapFile_t::ClipNodesArray_t::IndexType_t> (model.headNode[HalfLifeEngine::SDK::Constants::Hull_Human] - model.headNode[HalfLifeEngine::SDK::Constants::Hull_Point]);

		m_clipNodesIndexesArrays[HalfLifeEngine::SDK::Constants::Hull_Point].RemoveAll ();

//		RecursiveCountClipNodesForOneHull (m_bspFile.nodes, static_cast <short> (model.headNode[HalfLifeEngine::SDK::Constants::Hull_Point]), m_clipNodesIndexesArrays[HalfLifeEngine::SDK::Constants::Hull_Point], counts[HalfLifeEngine::SDK::Constants::Hull_Point]);

		counts[HalfLifeEngine::SDK::Constants::Hull_Human] = 0u;
		counts1[HalfLifeEngine::SDK::Constants::Hull_Human] = static_cast <BSPMapFile_t::ClipNodesArray_t::IndexType_t> (model.headNode[HalfLifeEngine::SDK::Constants::Hull_Large] - model.headNode[HalfLifeEngine::SDK::Constants::Hull_Human]);

		m_clipNodesIndexesArrays[HalfLifeEngine::SDK::Constants::Hull_Human].RemoveAll ();

		RecursiveCountClipNodesForOneHull (m_bspFile.clipNodes, static_cast <short> (model.headNode[HalfLifeEngine::SDK::Constants::Hull_Human]), m_clipNodesIndexesArrays[HalfLifeEngine::SDK::Constants::Hull_Human], counts[HalfLifeEngine::SDK::Constants::Hull_Human]);

		counts[HalfLifeEngine::SDK::Constants::Hull_Large] = 0u;
		counts1[HalfLifeEngine::SDK::Constants::Hull_Large] = static_cast <BSPMapFile_t::ClipNodesArray_t::IndexType_t> (model.headNode[HalfLifeEngine::SDK::Constants::Hull_Head] - model.headNode[HalfLifeEngine::SDK::Constants::Hull_Large]);

		m_clipNodesIndexesArrays[HalfLifeEngine::SDK::Constants::Hull_Large].RemoveAll ();

		RecursiveCountClipNodesForOneHull (m_bspFile.clipNodes, static_cast <short> (model.headNode[HalfLifeEngine::SDK::Constants::Hull_Large]), m_clipNodesIndexesArrays[HalfLifeEngine::SDK::Constants::Hull_Large], counts[HalfLifeEngine::SDK::Constants::Hull_Large]);

		counts[HalfLifeEngine::SDK::Constants::Hull_Head] = 0u;
		counts1[HalfLifeEngine::SDK::Constants::Hull_Head] = static_cast <BSPMapFile_t::ClipNodesArray_t::IndexType_t> ((isLastModelIndex ? m_bspFile.clipNodes.GetElementNumber () : m_bspFile.models[modelIndex + 1u].headNode[HalfLifeEngine::SDK::Constants::Hull_Human]) - model.headNode[HalfLifeEngine::SDK::Constants::Hull_Head]);

		m_clipNodesIndexesArrays[HalfLifeEngine::SDK::Constants::Hull_Head].RemoveAll ();

		RecursiveCountClipNodesForOneHull (m_bspFile.clipNodes, static_cast <short> (model.headNode[HalfLifeEngine::SDK::Constants::Hull_Head]), m_clipNodesIndexesArrays[HalfLifeEngine::SDK::Constants::Hull_Head], counts[HalfLifeEngine::SDK::Constants::Hull_Head]);

		AddLogEntry (true, LogLevel_Default, true, "Model[%u] clip nodes counts: [point=%u(%u), human=%u(%u), large=%u(%u), head=%u(%u)]", modelIndex, counts[HalfLifeEngine::SDK::Constants::Hull_Point], counts1[HalfLifeEngine::SDK::Constants::Hull_Point], counts[HalfLifeEngine::SDK::Constants::Hull_Human], counts1[HalfLifeEngine::SDK::Constants::Hull_Human], counts[HalfLifeEngine::SDK::Constants::Hull_Large], counts1[HalfLifeEngine::SDK::Constants::Hull_Large], counts[HalfLifeEngine::SDK::Constants::Hull_Head], counts1[HalfLifeEngine::SDK::Constants::Hull_Head]);
	}
{
	const unsigned char hullIndex (HalfLifeEngine::SDK::Constants::Hull_Head);
	BSPMapFile_t::ClipNodesArray_t::IndexType_t changedClipNodesNumber (0u);
	DynamicArray <BSPMapFile_t::PlanesArray_t::IndexType_t> changedPlanes;

	for (ClipNodesIndexesArray_t::IndexType_t index (0u); index < m_clipNodesIndexesArrays[hullIndex].GetElementNumber (); ++index)
	{
		const HalfLifeEngine::SDK::Constants::Hull_t nextHullIndex (HalfLifeEngine::SDK::Constants::Hull_Large);//static_cast <HalfLifeEngine::SDK::Constants::Hull_t> ((hullIndex + 1u) & (HalfLifeEngine::SDK::Constants::Hull_Total - 1u)));
		const HalfLifeEngine::SDK::Structures::DClipNode_t &clipNode (m_bspFile.clipNodes[m_clipNodesIndexesArrays[hullIndex][index]]);
		const HalfLifeEngine::SDK::Structures::DPlane_t &plane (m_bspFile.planes[static_cast <const BSPMapFile_t::PlanesArray_t::IndexType_t> (clipNode.planeNum)]);

		if (plane.type <= HalfLifeEngine::SDK::Structures::DPlane_t::Type_Z)
		{
			// Axial plane....

			// Find same plane for next hull....
			for (ClipNodesIndexesArray_t::IndexType_t index2 (0u); index2 < m_clipNodesIndexesArrays[nextHullIndex].GetElementNumber (); ++index2)
			{
				const HalfLifeEngine::SDK::Structures::DClipNode_t &clipNode2 (m_bspFile.clipNodes[m_clipNodesIndexesArrays[nextHullIndex][index2]]);
				const HalfLifeEngine::SDK::Structures::DPlane_t &plane2 (m_bspFile.planes[static_cast <const BSPMapFile_t::PlanesArray_t::IndexType_t> (clipNode2.planeNum)]);

				if (plane.normal != plane2.normal)
					continue;

				// Reliability check.
				InternalAssert (plane.type == plane2.type);

				/// @warning DISTANCE IS WRONG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//				const float deltaDistance (Math::sqrtf (MATH_GET_SQUARED (plane2.distance)) - Math::sqrtf (MATH_GET_SQUARED (plane.distance)));
				const float deltaDistance (Math::fabsf (plane2.distance) - Math::fabsf (plane.distance));

//				if (deltaDistance <= 0.0f || deltaDistance > HalfLifeEngine::SDK::Constants::hullSizes[nextHullIndex].maxs[plane.type] - HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].maxs[plane.type])
				if (deltaDistance != HalfLifeEngine::SDK::Constants::hullSizes[nextHullIndex].maxs[plane.type] - HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].maxs[plane.type])
					continue;

				++changedClipNodesNumber;
				changedPlanes += static_cast <const BSPMapFile_t::PlanesArray_t::IndexType_t> (clipNode.planeNum);

				// HELLO BITCH!
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("FIND HULL %u PLANE=%u[nrm=[%.2f, %.2f, %.2f], dist=%.2f] FOR HULL %u PLANE=%u[nrm=[%.2f, %.2f, %.2f], dist=%.2f], delta=%.2f.\n", hullIndex, clipNode.planeNum, plane.normal.x, plane.normal.y, plane.normal.z, plane.distance, nextHullIndex, clipNode2.planeNum, plane2.normal.x, plane2.normal.y, plane2.normal.z, plane2.distance, deltaDistance);

				break;
			}
		}
		else
		{
#if 0
			// Find same plane for next hull....
			for (ClipNodesIndexesArray_t::IndexType_t index2 (0u); index2 < m_clipNodesIndexesArrays[nextHullIndex].GetElementNumber (); ++index2)
			{
				const HalfLifeEngine::SDK::Structures::DClipNode_t &clipNode2 (m_bspFile.clipNodes[m_clipNodesIndexesArrays[nextHullIndex][index2]]);
				const HalfLifeEngine::SDK::Structures::DPlane_t &plane2 (m_bspFile.planes[static_cast <const BSPMapFile_t::PlanesArray_t::IndexType_t> (clipNode2.planeNum)]);

				if (plane.normal != plane2.normal)
					continue;

				// Reliability check.
				InternalAssert (plane.type == plane2.type);

				/// @warning DISTANCE IS WRONG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				const float deltaDistance (Math::sqrtf (MATH_GET_SQUARED (plane2.distance)) - Math::sqrtf (MATH_GET_SQUARED (plane.distance)));

			if (plane.normal.x != 0.0f)
				origin.x += plane.normal.x * (plane.normal.x > 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[hullnum].maxs.x : -HalfLifeEngine::SDK::Constants::hullSizes[hullnum].mins.x);
			if (plane.normal.y != 0.0f)
				origin.y += plane.normal.y * (plane.normal.y > 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[hullnum].maxs.y : -HalfLifeEngine::SDK::Constants::hullSizes[hullnum].mins.y);
			if (plane.normal.z != 0.0f)
				origin.z += plane.normal.z * (plane.normal.z > 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[hullnum].maxs.z : -HalfLifeEngine::SDK::Constants::hullSizes[hullnum].mins.z);

//				if (deltaDistance <= 0.0f || deltaDistance > HalfLifeEngine::SDK::Constants::hullSizes[nextHullIndex].maxs[plane.type] - HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].maxs[plane.type])
				if (deltaDistance != HalfLifeEngine::SDK::Constants::hullSizes[nextHullIndex].maxs[plane.type] - HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].maxs[plane.type])
					continue;

				// BARADA!!!!

				++changedClipNodesNumber;
				changedPlanes += static_cast <const BSPMapFile_t::PlanesArray_t::IndexType_t> (clipNode.planeNum);

				// HELLO BITCH!
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("FIND HULL %u PLANE=%u[nrm=[%.2f, %.2f, %.2f], dist=%.2f] FOR HULL %u PLANE=%u[nrm=[%.2f, %.2f, %.2f], dist=%.2f], delta=%.2f.\n", hullIndex, clipNode.planeNum, plane.normal.x, plane.normal.y, plane.normal.z, plane.distance, nextHullIndex, clipNode2.planeNum, plane2.normal.x, plane2.normal.y, plane2.normal.z, plane2.distance, deltaDistance);

				break;
			}
#endif	// if 0
		}
	}
}
}

void WorldMap::LookDownOnTheWorld (void)
{
	// this function loads and interprets the map BSP file at server boot start. It opens the map
	// file named filename, reads its contents, parses through the different BSP lumps and fills
	// the map BSP data structure "map", so that we can access to much more geometric info about
	// the virtual world than the engine would lets us know normally. The BSP loading code comes
	// with heavy simplification and major rewriting from Zoner's Half-Life tools source code.
	// Once this process has been done once, a .MAP file is created which holds the relevant map
	// data the bots make use of, and this file is loaded instead of redoing the BSP analysis
	// process from the start the next time the server is booted. In extenso, this function
	// sort the BSP data and the walkable faces in a handy hash table). It checks whether a world
	// map file already exists for the map currently booted, and if so, opens it and use the data
	// therein. Else if it is a new map that has never been used with YaPB which is booted for
	// the first time, its purpose is to retrieve, compute and sort various map data used by the
	// bots such as the walkable faces, the location of the face delimiters, and finally a world
	// map is "drawn", separating each walkable face to the parallel and meridian it refers to.
	// Such a world map is very useful for the bots not to have to cycle through all the huge map
	// data when they want to bind just one face or just one face delimiter. It speeds up the
	// search by square the resolution of the world map, i.e., by parallels * meridians times.

	// do some cleanup first
	FreeMapData ();

	m_bspFile.RemoveAll ();

	unsigned int bspFileSize;
	unsigned char *const bspFile (HalfLifeEngine::Globals::g_halfLifeEngine->LoadFile ("maps/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".BSP", &bspFileSize));	// load the BSP file and get its actual size (can't fail to do this, the map already booted)

	// Reliability check.
	if (bspFile == NULL)
	{
		AddLogEntry (false, LogLevel_Critical, true, "LookDownOnTheWorld(): Couldn't open 'maps/%s.BSP' file.", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ());

		return;
	}
/*
	// if a world map already exists...
	if (Load (bspFileSize))
	{
		// close BSP file
		HalfLifeEngine::Globals::g_halfLifeEngine->FreeFile (bspFile);

		// return if the loading process completed successfully
		return;
	}
*/
	const HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t *const BSPHeader (reinterpret_cast <HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t *const> (bspFile));

	// world map not found, out of date or failed loading, we have to build a new one
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Looking down on the world.\n");	// tell people what we are doing

	// Reliability check.
	if (BSPHeader->version != HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::CurrentVersion)
		AddLogEntry (false, LogLevel_Critical, true, "WorldMap::LookDownOnTheWorld(): Current map has wrong header version number (%u), should be %u!", BSPHeader->version, HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::CurrentVersion);

	// read the entities, planes, vertexes, nodes, faces, edges, surf edges and models lumps of the BSP file...
	const LongDynamicString entitiesData (reinterpret_cast <char *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Entities].fileStartOffset), Math::GetMinimumValueBetween (static_cast <unsigned int> (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Entities].length), GetStringLength <unsigned int> (reinterpret_cast <char *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Entities].fileStartOffset))));

	// Reliability check.
	if (entitiesData.IsEmpty ())
		AddLogEntry (false, LogLevel_Critical, true, "WorldMap::LookDownOnTheWorld(): Current map has no entities, should be more than zero!");

	// Reliability check.
	InternalAssert (entitiesData.GetElementNumber () == BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Entities].length - 1u);
/*	if (entitiesData.GetElementNumber () != BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Entities].length - 1u)
		AddLogEntry (false, LogLevel_Critical, true, "entitiesData.GetElementNumber(%u) != (%u)BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Entities].length - 1u, LongStr::MaximumCap=%u, GetStringLength(entlump)=%u.", entitiesData.GetElementNumber (), BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Entities].length - 1u, LongDynamicString::MaximumPossibleSize, GetStringLength <unsigned int> (reinterpret_cast <char *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Entities].fileStartOffset)));
*/
	#if defined _DEBUG
		if (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Leafs].length % sizeof (HalfLifeEngine::SDK::Structures::DLeaf_t) > 0u)
			AddLogEntry (false, LogLevel_Critical, false, "Odd leafs lump size!");
	#endif	// if defined _DEBUG

	m_bspFile.leafs.Assign (reinterpret_cast <HalfLifeEngine::SDK::Structures::DLeaf_t *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Leafs].fileStartOffset), static_cast <unsigned short> (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Leafs].length / sizeof (HalfLifeEngine::SDK::Structures::DLeaf_t)));

	#if defined _DEBUG
		if (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Planes].length % sizeof (HalfLifeEngine::SDK::Structures::DPlane_t) > 0u)
			AddLogEntry (false, LogLevel_Critical, false, "Odd planes lump size!");
	#endif	// if defined _DEBUG

	m_bspFile.planes.Assign (reinterpret_cast <HalfLifeEngine::SDK::Structures::DPlane_t *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Planes].fileStartOffset), static_cast <unsigned short> (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Planes].length / sizeof (HalfLifeEngine::SDK::Structures::DPlane_t)));

	#if defined _DEBUG
		if (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Vertexes].length % sizeof (HalfLifeEngine::SDK::Structures::DVertex_t) > 0u)
			AddLogEntry (false, LogLevel_Critical, false, "Odd vertexes lump size!");
	#endif	// if defined _DEBUG

	m_bspFile.vertexes.Assign (reinterpret_cast <HalfLifeEngine::SDK::Structures::DVertex_t *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Vertexes].fileStartOffset), static_cast <unsigned short> (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Vertexes].length / sizeof (HalfLifeEngine::SDK::Structures::DVertex_t)));

	#if defined _DEBUG
		if (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Nodes].length % sizeof (HalfLifeEngine::SDK::Structures::DNode_t) > 0u)
			AddLogEntry (false, LogLevel_Critical, false, "Odd nodes lump size!");
	#endif	// if defined _DEBUG

	m_bspFile.nodes.Assign (reinterpret_cast <HalfLifeEngine::SDK::Structures::DNode_t *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Nodes].fileStartOffset), static_cast <unsigned short> (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Nodes].length / sizeof (HalfLifeEngine::SDK::Structures::DNode_t)));

	#if defined _DEBUG
		if (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_ClipNodes].length % sizeof (HalfLifeEngine::SDK::Structures::DClipNode_t) > 0u)
			AddLogEntry (false, LogLevel_Critical, false, "Odd clip nodes lump size!");
	#endif	// if defined _DEBUG

	m_bspFile.clipNodes.Assign (reinterpret_cast <HalfLifeEngine::SDK::Structures::DClipNode_t *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_ClipNodes].fileStartOffset), static_cast <unsigned short> (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_ClipNodes].length / sizeof (HalfLifeEngine::SDK::Structures::DClipNode_t)));

	// Reliability check.
	InternalAssert (m_bspFile.clipNodes.GetElementNumber () >= 6u * HalfLifeEngine::SDK::Constants::CollisionHull_Total);
//	InternalAssert (m_bspFile.clipNodes.GetElementNumber () % HalfLifeEngine::SDK::Constants::CollisionHull_Total == 0u);	// OCCURS!!! (3d_aim_nuke.bsp)

	#if defined _DEBUG
		if (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Faces].length % sizeof (HalfLifeEngine::SDK::Structures::DFace_t) > 0u)
			AddLogEntry (false, LogLevel_Critical, false, "Odd faces lump size!");
	#endif	// if defined _DEBUG

	m_bspFile.faces.Assign (reinterpret_cast <HalfLifeEngine::SDK::Structures::DFace_t *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Faces].fileStartOffset), static_cast <unsigned short> (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Faces].length / sizeof (HalfLifeEngine::SDK::Structures::DFace_t)));

	#if defined _DEBUG
		if (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Edges].length % sizeof (HalfLifeEngine::SDK::Structures::DEdge_t) > 0u)
			AddLogEntry (false, LogLevel_Critical, false, "Odd edges lump size!");
	#endif	// if defined _DEBUG

	m_bspFile.edges.Assign (reinterpret_cast <HalfLifeEngine::SDK::Structures::DEdge_t *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Edges].fileStartOffset), BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Edges].length / sizeof (HalfLifeEngine::SDK::Structures::DEdge_t));

	#if defined _DEBUG
		if (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_SurfEdges].length % sizeof (HalfLifeEngine::SDK::TypeDefinitions::DSurfEdge_t) > 0u)
			AddLogEntry (false, LogLevel_Critical, false, "Odd surf edges lump size!");
	#endif	// if defined _DEBUG

	m_bspFile.surfEdges.Assign (reinterpret_cast <HalfLifeEngine::SDK::TypeDefinitions::DSurfEdge_t *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_SurfEdges].fileStartOffset), BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_SurfEdges].length / sizeof (HalfLifeEngine::SDK::TypeDefinitions::DSurfEdge_t));

	#if defined _DEBUG
		if (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Models].length % sizeof (HalfLifeEngine::SDK::Structures::DModel_t) > 0u)
			AddLogEntry (false, LogLevel_Critical, false, "Odd models lump size!");
	#endif	// if defined _DEBUG

	m_bspFile.models.Assign (reinterpret_cast <HalfLifeEngine::SDK::Structures::DModel_t *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Models].fileStartOffset), static_cast <unsigned short> (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Models].length / sizeof (HalfLifeEngine::SDK::Structures::DModel_t)));

	// Reliability check.
	InternalAssert (!m_bspFile.models.IsEmpty ());
#if 0
	for (unsigned short modelIndex (0u); modelIndex < m_bspFile.models.GetElementNumber (); ++modelIndex)
		AddLogEntry (true, LogLevel_Default, true, "models[%u].headNode: [human=%i, large=%i, head=%i].", modelIndex, m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Human], m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Large], m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Head]);

{
	for (unsigned short index (static_cast <unsigned short> (m_bspFile.models[0u].headNode[HalfLifeEngine::SDK::Constants::Hull_Human])); index < m_bspFile.models[0u].headNode[HalfLifeEngine::SDK::Constants::Hull_Large]; ++index)
	{
		BSPMapFile_t::ClipNodesArray_t::IndexType_t indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Total] =
		{
			index,
			static_cast <unsigned short> (m_bspFile.models[0u].headNode[HalfLifeEngine::SDK::Constants::Hull_Large]) + index,
			static_cast <unsigned short> (m_bspFile.models[0u].headNode[HalfLifeEngine::SDK::Constants::Hull_Head]) + index
		};
		BSPMapFile_t::ClipNodesArray_t::IndexType_t maximumNodeIndex (static_cast <BSPMapFile_t::ClipNodesArray_t::IndexType_t> (m_bspFile.models[0u].headNode[HalfLifeEngine::SDK::Constants::Hull_Head]));

		if (indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Large] > maximumNodeIndex)
			indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Large] = maximumNodeIndex;

		maximumNodeIndex = static_cast <BSPMapFile_t::ClipNodesArray_t::IndexType_t> (m_bspFile.models.GetElementNumber () == 1u ? m_bspFile.clipNodes.GetElementNumber () : m_bspFile.models[0u + 1u].headNode[HalfLifeEngine::SDK::Constants::Hull_Human]);

		if (indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Head] > maximumNodeIndex)
			indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Head] = maximumNodeIndex;

		const HalfLifeEngine::SDK::Structures::DClipNode_t clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Total] =
		{
			m_bspFile.clipNodes[indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Human]],
			m_bspFile.clipNodes[indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Large]],
			m_bspFile.clipNodes[indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Head]]
		};
		const HalfLifeEngine::SDK::Structures::DPlane_t planes[HalfLifeEngine::SDK::Constants::CollisionHull_Total] =
		{
			m_bspFile.planes[clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].planeNum],
			m_bspFile.planes[clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].planeNum],
			m_bspFile.planes[clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].planeNum]
		};
/*
		HalfLifeEngine::SDK::Structures::DPlane_t *const planesPointers[HalfLifeEngine::SDK::Constants::CollisionHull_Total] =
		{
			&m_bspFile.planes[clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].planeNum],
			&m_bspFile.planes[clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].planeNum],
			&m_bspFile.planes[clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].planeNum]
		};

		if (planesPointers[HalfLifeEngine::SDK::Constants::CollisionHull_Human]->distance < 0.0f)
			planesPointers[HalfLifeEngine::SDK::Constants::CollisionHull_Human]->distance -= planesPointers[HalfLifeEngine::SDK::Constants::CollisionHull_Large]->distance ? HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Human].mins.x : HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Human].maxs.x;
		else
			//
/*
		// Reliability checks. OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//		InternalAssert (planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal == planes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].normal);
		if (planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal != planes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].normal)
			AddLogEntry (false, LogLevel_Critical, true, "Human: [clipNode=%u, planes[%u].normal[%.2f, %.2f, %.2f]] != Large: [clipNode=%u, planes[%u].normal[%.2f, %.2f, %.2f]].", indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Human], clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].planeNum, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal.x, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal.y, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal.z, indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Large], clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].planeNum, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].normal.x, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].normal.y, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].normal.z);
//		InternalAssert (planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal == planes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].normal);
		if (planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal != planes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].normal)
			AddLogEntry (false, LogLevel_Critical, true, "Human: [clipNode=%u, planes[%u].normal[%.2f, %.2f, %.2f]] != Head: [clipNode=%u, planes[%u].normal[%.2f, %.2f, %.2f]].", indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Human], clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].planeNum, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal.x, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal.y, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal.z, indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Head], clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].planeNum, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].normal.x, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].normal.y, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].normal.z);
*/
		AddLogEntry (true, LogLevel_Default, true, "--%u--\n\t\t\t\t\tHuman: [clipNode=%u[planeNum=%u[nrm=[%.2f, %.2f, %.2f], dist=%.2f], childs={%i, %i}]]", index, indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Human], clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].planeNum, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal.x, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal.y, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].normal.z, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].distance, clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Front], clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Human].children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Back]);
		AddLogEntry (true, LogLevel_Default, true, "\tLarge: [clipNode=%u[planeNum=%u[nrm=[%.2f, %.2f, %.2f], dist=%.2f], childs={%i, %i}]]", indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Large], clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].planeNum, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].normal.x, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].normal.y, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].normal.z, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].distance, clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Front], clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Large].children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Back]);
		AddLogEntry (true, LogLevel_Default, true, "\tHead : [clipNode=%u[planeNum=%u[nrm=[%.2f, %.2f, %.2f], dist=%.2f], childs={%i, %i}]]", indexes[HalfLifeEngine::SDK::Constants::CollisionHull_Head], clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].planeNum, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].normal.x, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].normal.y, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].normal.z, planes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].distance, clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Front], clipNodes[HalfLifeEngine::SDK::Constants::CollisionHull_Head].children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Back]);
	}
}
#endif	// if 0
//	CountClipNodesForOneHull ();

	#if defined _DEBUG
		if (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_MarkSurfaces].length % sizeof (HalfLifeEngine::SDK::TypeDefinitions::DMarkSurface_t) > 0u)
			AddLogEntry (false, LogLevel_Critical, false, "Odd mark surfaces lump size!");
	#endif	// if defined _DEBUG

	m_bspFile.markSurfaces.Assign (reinterpret_cast <HalfLifeEngine::SDK::TypeDefinitions::DMarkSurface_t *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_MarkSurfaces].fileStartOffset), static_cast <unsigned short> (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_MarkSurfaces].length / sizeof (HalfLifeEngine::SDK::TypeDefinitions::DMarkSurface_t)));
/*	m_bspFile.textureHeader.texturesNum = reinterpret_cast <unsigned int *> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Textures].fileStartOffset)[0u];

	#if defined _DEBUG
		if (sizeof (m_bspFile.textureHeader.texturesNum) + m_bspFile.textureHeader.texturesNum * sizeof (m_bspFile.textureHeader.dataOffsets[0u]) + m_bspFile.textureHeader.texturesNum * sizeof (HalfLifeEngine::SDK::Structures::DTexture_t) != BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Textures].length)
			AddLogEntry (false, LogLevel_Critical, false, "Odd mark texture data offset lump size %u! (Should be %u!)", sizeof (m_bspFile.textureHeader.texturesNum) + m_bspFile.textureHeader.texturesNum * sizeof (m_bspFile.textureHeader.dataOffsets[0u]) + m_bspFile.textureHeader.texturesNum * sizeof (HalfLifeEngine::SDK::Structures::DTexture_t), BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Textures].length);
	#endif	// if defined _DEBUG

	m_bspFile.textureHeader.dataOffsets = new unsigned int[m_bspFile.textureHeader.texturesNum];
	MemoryCopy (reinterpret_cast <unsigned int *> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Textures].fileStartOffset + sizeof (m_bspFile.textureHeader.texturesNum)), m_bspFile.textureHeader.dataOffsets, m_bspFile.textureHeader.texturesNum);
	for (unsigned int index (0u); index < m_bspFile.textureHeader.texturesNum; ++index)
	{
		if (m_bspFile.textureHeader.dataOffsets[index] == static_cast <unsigned int> (-1))
			continue;

		m_bspFile.textureHeader.dataOffsets[index] -= m_bspFile.textureHeader.GetSize ();
		m_bspFile.textureHeader.dataOffsets[index] /= sizeof (HalfLifeEngine::SDK::Structures::DTexture_t);
	}
	m_bspFile.texturesData.Assign (reinterpret_cast <HalfLifeEngine::SDK::Structures::DTexture_t *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Textures].fileStartOffset + m_bspFile.textureHeader.GetSize ()), m_bspFile.textureHeader.texturesNum);
//*/	m_bspFile.texturesData.Assign (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Textures].fileStartOffset, BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_Textures].length);

	#if defined _DEBUG
		if (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_TextureInfo].length % sizeof (HalfLifeEngine::SDK::Structures::DTextureInformation_t) > 0u)
			AddLogEntry (false, LogLevel_Critical, false, "Odd mark surfaces lump size!");
	#endif	// if defined _DEBUG

	m_bspFile.texturesInformation.Assign (reinterpret_cast <HalfLifeEngine::SDK::Structures::DTextureInformation_t *const> (bspFile + BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_TextureInfo].fileStartOffset), static_cast <unsigned short> (BSPHeader->lumps[HalfLifeEngine::SDK::Structures::BSPMapFileHeader_t::Lump_t::ID_TextureInfo].length / sizeof (HalfLifeEngine::SDK::Structures::DTextureInformation_t)));

	AddLogEntry (true, LogLevel_Default, false, "World size: [X=%.2f, Y=%.2f, Z=%.2f].", m_bspFile.models[0u].boundingBox.GetSizeX (), m_bspFile.models[0u].boundingBox.GetSizeY (), m_bspFile.models[0u].boundingBox.GetSizeZ ());

	ReadEntitiesFromStringData (entitiesData);

	LoadWorldModelExtraInfo ();
//	MakeHullFaces ();

//	MakeCollisionPointHull ();

	// everything is loaded, free the BSP file
	HalfLifeEngine::Globals::g_halfLifeEngine->FreeFile (bspFile);
/*
	AddLogEntry (true, LogLevel_Default, true, "Map has %u clip nodes.", m_bspFile.clipNodes.GetElementNumber ());

	for (unsigned short modelIndex (0u); modelIndex < m_bspFile.models.GetElementNumber (); ++modelIndex)
//		for (unsigned char hullIndex (HalfLifeEngine::SDK::Constants::Hull_Point); hullIndex < HalfLifeEngine::SDK::Constants::Hull_Total; ++hullIndex)
		{
			const HalfLifeEngine::SDK::Structures::DClipNode_t &clipNode0 (m_bspFile.clipNodes[m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Point]]);
			const HalfLifeEngine::SDK::Structures::DClipNode_t &clipNode1 (m_bspFile.clipNodes[m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Human]]);
			const HalfLifeEngine::SDK::Structures::DClipNode_t &clipNode2 (m_bspFile.clipNodes[m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Large]]);
			const HalfLifeEngine::SDK::Structures::DClipNode_t &clipNode3 (m_bspFile.clipNodes[m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Head]]);
			const HalfLifeEngine::SDK::Structures::DPlane_t &plane0 (m_bspFile.planes[clipNode0.planeNum]);
			const HalfLifeEngine::SDK::Structures::DPlane_t &plane1 (m_bspFile.planes[clipNode1.planeNum]);
			const HalfLifeEngine::SDK::Structures::DPlane_t &plane2 (m_bspFile.planes[clipNode2.planeNum]);
			const HalfLifeEngine::SDK::Structures::DPlane_t &plane3 (m_bspFile.planes[clipNode3.planeNum]);

			AddLogEntry (true, LogLevel_Default, true, "Model %u clip nodes indexes: [point=[%i, plane: %u [nrm: [%.2f, %.2f, %.2f], dist=%.2f]], [human=%i, plane: %u [nrm: [%.2f, %.2f, %.2f], dist=%.2f]], [large=%i, plane: %u [nrm: [%.2f, %.2f, %.2f], dist=%.2f]], [head=%i, plane: %u [nrm: [%.2f, %.2f, %.2f], dist=%.2f]]].\n", modelIndex, m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Point], clipNode0.planeNum, plane0.normal.x, plane0.normal.y, plane0.normal.z, plane0.distance, m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Human], clipNode1.planeNum, plane1.normal.x, plane1.normal.y, plane1.normal.z, plane1.distance, m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Large], clipNode2.planeNum, plane2.normal.x, plane2.normal.y, plane2.normal.z, plane2.distance, m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Head], clipNode3.planeNum, plane3.normal.x, plane3.normal.y, plane3.normal.z, plane3.distance);
		}
/*
	for (unsigned short modelIndex (0u); modelIndex < m_bspFile.models.GetElementNumber (); ++modelIndex)
		AddLogEntry (true, LogLevel_Default, true, "Model %u clip nodes indexes: [point=%i, human=%i, large=%i, head=%i].", modelIndex, m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Point], m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Human], m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Large], m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Head]);
*/
/*	AddLogEntry (true, LogLevel_Default, true, "Map has %u planes.", m_bspFile.planes.GetElementNumber ());

	for (unsigned short clipNodeIndex (0u); clipNodeIndex < m_bspFile.clipNodes.GetElementNumber (); ++clipNodeIndex)
	{
		const unsigned short planeIndex (static_cast <unsigned short> (m_bspFile.clipNodes[clipNodeIndex].planeNum));
		const HalfLifeEngine::SDK::Structures::DPlane_t &plane (m_bspFile.planes[planeIndex]);

		AddLogEntry (true, LogLevel_Default, true, "Clip node %u plane: %u([%.2f, %02.2f, %.2f], %.2f).", clipNodeIndex, planeIndex, plane.normal.x, plane.normal.y, plane.normal.z, plane.distance);
	}
/*	for (unsigned short modelIndex (0u); modelIndex < m_bspFile.models.GetElementNumber (); ++modelIndex)
	{
		const unsigned short pointPlaneIndex (static_cast <unsigned short> (m_bspFile.nodes[static_cast <unsigned short> (m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Point])].planeNum));
		const HalfLifeEngine::SDK::Structures::DPlane_t &pointPlane (m_bspFile.planes[pointPlaneIndex]);

		const unsigned short humanPlaneIndex (static_cast <unsigned short> (m_bspFile.clipNodes[static_cast <unsigned short> (m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Human])].planeNum));
		const HalfLifeEngine::SDK::Structures::DPlane_t &humanPlane (m_bspFile.planes[humanPlaneIndex]);
		const unsigned short largePlaneIndex (static_cast <unsigned short> (m_bspFile.clipNodes[static_cast <unsigned short> (m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Large])].planeNum));
		const HalfLifeEngine::SDK::Structures::DPlane_t &largePlane (m_bspFile.planes[largePlaneIndex]);
		const unsigned short headPlaneIndex (static_cast <unsigned short> (m_bspFile.clipNodes[static_cast <unsigned short> (m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Head])].planeNum));
		const HalfLifeEngine::SDK::Structures::DPlane_t &headPlane (m_bspFile.planes[headPlaneIndex]);

		AddLogEntry (true, LogLevel_Default, true, "Model %u planes: [point=%u([%.2f, %.2f, %.2f], %.2f), human=%u([%.2f, %.2f, %.2f], %.2f), large=%u([%.2f, %.2f, %.2f], %.2f), head=%u([%.2f, %.2f, %.2f], %.2f)].", modelIndex, pointPlaneIndex, pointPlane.normal.x, pointPlane.normal.y, pointPlane.normal.z, pointPlane.distance, humanPlaneIndex, humanPlane.normal.x, humanPlane.normal.y, humanPlane.normal.z, humanPlane.distance, largePlaneIndex, largePlane.normal.x, largePlane.normal.y, largePlane.normal.z, largePlane.distance, headPlaneIndex, headPlane.normal.x, headPlane.normal.y, headPlane.normal.z, headPlane.distance);
/*		AddLogEntry (true, LogLevel_Default, true, "Model %u:", modelIndex);

		for (unsigned short faceIndex (0u); faceIndex < m_bspFile.models.GetElementNumber (); ++faceIndex)
			AddLogEntry (true, LogLevel_Default, true, "\tPlane[]: [point=%i, human=%i, large=%i, head=%i].", faceIndex, m_bspFile.nodes[static_cast <unsigned short> (m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Point])].planeNum, m_bspFile.clipNodes[static_cast <unsigned short> (m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Human])].planeNum, m_bspFile.clipNodes[static_cast <unsigned short> (m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Large])].planeNum, m_bspFile.clipNodes[static_cast <unsigned short> (m_bspFile.models[modelIndex].headNode[HalfLifeEngine::SDK::Constants::Hull_Head])].planeNum);
*///	}

	// if a world map already exists...
	if (Load (bspFileSize))
	{
		// return if the loading process completed successfully
		return;
	}

	// compute the number of sectors this map should have (300 units-sized sectors by default)
	m_parallelsCount = static_cast <unsigned char> (m_bspFile.models[0u].boundingBox.GetSizeX () / GridCellSize) + 1u;
	m_meridiansCount = static_cast <unsigned char> (m_bspFile.models[0u].boundingBox.GetSizeY () / GridCellSize) + 1u;
/*
	// don't allow the parallels and meridians count to be higher than the maximal value allowed
	if (m_parallelsCount > MAX_MAP_PARALLELS)
		m_parallelsCount = MAX_MAP_PARALLELS;	// bound the number of parallels up to MAX_MAP_PARALLELS

	if (m_meridiansCount > MAX_MAP_MERIDIANS)
		m_meridiansCount = MAX_MAP_MERIDIANS;	// bound the number of meridians up to MAX_MAP_MERIDIANS
*/	InternalAssert (m_parallelsCount < MAX_MAP_PARALLELS);
	InternalAssert (m_meridiansCount < MAX_MAP_MERIDIANS);

	// translate each walkable face of the BSP file into an engine-independent structure, describing each face by its corners and its face delimiters (middle of face edges)

	unsigned short faceIndex;
	unsigned short cornerIndex;
	bool isDiscardable;
	Math::Vector3D corner, center;
	HalfLifeEngine::SDK::Constants::PointContentType_t contents;
	unsigned char i, j;

	// loop through all the faces of the BSP file and count the number of walkable faces...
	for (faceIndex = 0u; faceIndex < m_bspFile.models[0u].facesNum; ++faceIndex)
	{
		// quick access to the face
		const HalfLifeEngine::SDK::Structures::DFace_t &face (m_bspFile.faces[static_cast <unsigned short> (m_bspFile.models[0u].firstFaceIndex + faceIndex)]);
		const HalfLifeEngine::SDK::Structures::DPlane_t &plane (m_bspFile.planes[static_cast <unsigned short> (face.planeNum)]);

		// a simple check to see whether plane is flat or walkable < 45 degree
		// if this face is NOT walkable (i.e, normal NOT pointing straight up OR it's a ceiling)
		// Some faces are completely reverted -- thanks a lot evy for the bugfix
		if (face.side == 0u)
		{
			if (plane.normal.z < Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ())
				continue;	// discard this face
		}
		else if (plane.normal.z > -Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ())
			continue;	// discard this face

		isDiscardable = false;	// assume face is valid unless known otherwise
		center = Math::Vector3D::ZeroValue;	// prepare for computation of the center of the face

		// face MAY be walkable, check if it's not a top or side face (useless in the mesh...)
		for (cornerIndex = 0u; cornerIndex < face.surfEdgesNum; ++cornerIndex)
		{
			corner = GetFaceCornerByIndex (face, cornerIndex);	// get each corner successively...

			if
			(
				corner.x == m_bspFile.models[0u].boundingBox.mins.x || corner.x == m_bspFile.models[0u].boundingBox.maxs.x ||
				corner.y == m_bspFile.models[0u].boundingBox.mins.y || corner.y == m_bspFile.models[0u].boundingBox.maxs.y ||
				corner.z == m_bspFile.models[0u].boundingBox.maxs.z
			)
			{
				isDiscardable = true;	// is it a top or side face? Then it's discardable (if this face was meant to be discarded, don't process it)

				break;
			}

			center += corner;	// add this corner for the center's computation
		}

//		if (cornerIndex < face.surfEdgesNum)
		if (isDiscardable)
			continue;	// if this face was meant to be discarded, don't process it

		center /= face.surfEdgesNum;	// Final computation center.

		center.z += 32.0f;

		contents = HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (center);

		if (contents == HalfLifeEngine::SDK::Constants::Content_Solid || contents == HalfLifeEngine::SDK::Constants::Content_Sky)
			continue;	// if nobody can stand on this face, discard it too

		if (!m_walkFaces.PushBackDefaultElement ())
		{
			AddLogEntry (false, LogLevel_Critical, true, "BARADA!!!");

			return;
		}

		// quick access to walk face
		WalkFace_t &walkFace (m_walkFaces.GetLastElement ());

		// we know this map to have one walkable face more....

		// allocate enough memory to hold all this face's corner information
		walkFace.cornerCount = face.surfEdgesNum;	// number of edges
		walkFace.corners = new Math::Vector3D[face.surfEdgesNum];

		#if defined _DEBUG
			if (walkFace.corners == NULL)
			{
				AddLogEntry (false, LogLevel_Critical, true, "LookDownOnTheWorld(): Failure on %u bytes for %u-element corners array on walkface %u.", face.surfEdgesNum * sizeof (Math::Vector3D), face.surfEdgesNum, m_walkFaces.GetElementNumber () - 1u);

				return;
			}
		#endif	// if defined _DEBUG

		// face is walkable, loop though the edges and get the vertexes...
		for (cornerIndex = 0u; cornerIndex < face.surfEdgesNum; ++cornerIndex)
		{
			// get each corner successively... and store it as a corner of the polygon
			walkFace.corners[cornerIndex] = GetFaceCornerByIndex (face, cornerIndex);
		}

		// for each latitude/longitude sector of the map topology array...
		for (i = 0u; i < m_parallelsCount; ++i)
			for (j = 0u; j < m_meridiansCount; ++j)
			{
				// does this face belong to this topologic sector?
				if (!WalkFaceBelongsToSector (walkFace, i, j))
					continue;

				// store a pointer to this walkable face in this topological sector
				m_topology[i][j] += &walkFace;	// this topological sector holds now one face more
			}

		// we know now one walkable face more....

		// if we've computed 64 faces more in the BSP faces pool...
		if ((faceIndex & 64u - 1u) == 0u)
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (".");	// print a trailing dot as a progress bar
	}

	// and terminate the progress bar
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("	looking done!\n\t%u parallels, %u meridians, %.2f kilobytes world data.\n", m_parallelsCount, m_meridiansCount, Save (bspFileSize) / 1024.0f);

	AddLogEntry (true, LogLevel_Default, false, "\n\t%u leafs,\n\t%u models,\n\t%u entities,\n\t%u planes,\n\t%u vertexes,\n\t%u nodes,\n\t%u clip nodes,\n\t%u faces,\n\t%u edges,\n\t%u surf edges,\n\t%u mark surfaces,\n\t%u textures data,\n\t%u textures information loaded!", m_bspFile.leafs.GetElementNumber (), m_bspFile.models.GetElementNumber (), m_bspFile.entities.GetElementNumber (), m_bspFile.planes.GetElementNumber (), m_bspFile.vertexes.GetElementNumber (), m_bspFile.nodes.GetElementNumber (), m_bspFile.clipNodes.GetElementNumber (), m_bspFile.faces.GetElementNumber (), m_bspFile.edges.GetElementNumber (), m_bspFile.surfEdges.GetElementNumber (), m_bspFile.markSurfaces.GetElementNumber (), m_bspFile.texturesData.GetElementNumber (), m_bspFile.texturesInformation.GetElementNumber ());
}

void WorldMap::LoadWorldModelExtraInfo (void)
{
	/// @note Code from "Dark Places" engine project....

	// Reliability check.
	InternalAssert (m_worldModelExtraInfo.surfaces == NULL);

	BSPMapFile_t::FacesArray_t faces;
	const char *texname;
	unsigned short cornerIndex;
	bool isDiscardable;
	Math::Vector3D corner, center;
	HalfLifeEngine::SDK::Constants::PointContentType_t contents;

	int totaltris (0), totalverts (0);

	// loop through all the entities looking for "func_wall"...
	// And check for collisions with them...
	for (BSPMapFile_t::EntitiesArray_t::IndexType_t entityIndex (0u); entityIndex < GetBSPFile ().entities.GetElementNumber (); ++entityIndex)
	{
		const Entity_t &entity (GetBSPFile ().entities[entityIndex]);

		if (!entity.isStaticSolid)
			continue;

		const HalfLifeEngine::SDK::Structures::DModel_t &model (GetBSPFile ().models[entity.brushModelIndex]);

	// for all surfaces in the map
	for (unsigned short faceIndex (0u); faceIndex < model.facesNum; ++faceIndex)
	{
		// quick access to the face
		const HalfLifeEngine::SDK::Structures::DFace_t &face (GetBSPFile ().faces[static_cast <unsigned short> (model.firstFaceIndex + faceIndex)]);

		// Filter faces....
		{
/*		{
		const HalfLifeEngine::SDK::Structures::DPlane_t &plane (GetBSPFile ().planes[static_cast <unsigned short> (face.planeNum)]);

		// a simple check to see whether plane is flat or walkable < 45 degree
		// if this face is NOT walkable (i.e, normal NOT pointing straight up OR it's a ceiling)
		// Some faces are completely reverted -- thanks a lot evy for the bugfix
		if (face.side == 0u)
		{
			if (plane.normal.z < Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ())
				continue;	// discard this face
		}
		else if (plane.normal.z > -Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ())
			continue;	// discard this face

		isDiscardable = false;	// assume face is valid unless known otherwise
		center = Math::Vector3D::ZeroValue;	// prepare for computation of the center of the face

		// face MAY be walkable, check if it's not a top or side face (useless in the mesh...)
		for (cornerIndex = 0u; cornerIndex < face.surfEdgesNum; ++cornerIndex)
		{
			corner = GetFaceCornerByIndex (face, cornerIndex);	// get each corner successively...

			if
			(
				corner.x == model.boundingBox.mins.x || corner.x == model.boundingBox.maxs.x ||
				corner.y == model.boundingBox.mins.y || corner.y == model.boundingBox.maxs.y ||
				corner.z == model.boundingBox.maxs.z
			)
			{
				isDiscardable = true;	// is it a top or side face? Then it's discardable (if this face was meant to be discarded, don't process it)

				break;
			}

			center += corner;	// add this corner for the center's computation
		}

//		if (cornerIndex < face.surfEdgesNum)
		if (isDiscardable)
			continue;	// if this face was meant to be discarded, don't process it

		center /= face.surfEdgesNum;	// Final computation center.

		center.z += 32.0f;

		contents = HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (center);

		if (contents == HalfLifeEngine::SDK::Constants::Content_Solid || contents == HalfLifeEngine::SDK::Constants::Content_Sky)
			continue;	// if nobody can stand on this face, discard it too
		}
*/
		{
		texname = GetTextureName (face);

		if (texname == NULL)
			continue;

		if
		(
			strncmp (texname, "sky",    GET_STATIC_STRING_LENGTH ("sky")) == 0 ||
			strncmp (texname, "*lava",  GET_STATIC_STRING_LENGTH ("*lava")) == 0 ||
			strncmp (texname, "*slime", GET_STATIC_STRING_LENGTH ("*slime")) == 0 ||
			strncmp (texname, "*water", GET_STATIC_STRING_LENGTH ("*water")) == 0
		)
		{
//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Face[%u] texname: \"%s\" is skipped!\n", faceIndex, texname);

			continue;
		}
		}
		}

		totaltris += static_cast <int> (face.surfEdgesNum) - 2;
		totalverts += face.surfEdgesNum;

		faces += face;
	}
	}

	// Reliability checks.
	InternalAssert (!faces.IsEmpty ());
	InternalAssert (totaltris > 0);
	InternalAssert (totalverts > 0);

	m_worldModelExtraInfo.surfaces = new ExtraModelInfo_t::ExtraSurfaceInfo_t[faces.GetElementNumber ()];
	m_worldModelExtraInfo.surfacesNumber = faces.GetElementNumber ();
	m_worldModelExtraInfo.surfmesh.num_triangles = totaltris;

	m_worldModelExtraInfo.surfmesh.num_vertices = totalverts;

	m_worldModelExtraInfo.surfmesh.data_element3i = new int[3 * m_worldModelExtraInfo.surfmesh.num_triangles];
	m_worldModelExtraInfo.surfmesh.data_vertex3f = new Math::Vector3D[m_worldModelExtraInfo.surfmesh.num_vertices];

	totaltris = totalverts = 0;

	// for all surfaces in the map
	for (BSPMapFile_t::FacesArray_t::IndexType_t faceIndex (0u); faceIndex < faces.GetElementNumber (); ++faceIndex)
	{
		// quick access to the face
		const HalfLifeEngine::SDK::Structures::DFace_t &face (faces[faceIndex]);
		ExtraModelInfo_t::ExtraSurfaceInfo_t &extendedSurface (m_worldModelExtraInfo.surfaces[faceIndex]);

		extendedSurface.num_firsttriangle = totaltris;
		extendedSurface.num_triangles = static_cast <int> (face.surfEdgesNum) - 2;
		extendedSurface.num_firstvertex = totalverts;
		extendedSurface.num_vertices = face.surfEdgesNum;

		totaltris += static_cast <int> (face.surfEdgesNum) - 2;
		totalverts += face.surfEdgesNum;

		// convert edges back to a normal polygon
		for (unsigned short j (0u); j < extendedSurface.num_vertices; ++j)
			m_worldModelExtraInfo.surfmesh.data_vertex3f[extendedSurface.num_firstvertex + j] = GetFaceCornerByIndex (face, j);

		for (int j (0); j < extendedSurface.num_triangles; ++j)
		{
			(m_worldModelExtraInfo.surfmesh.data_element3i + 3 * extendedSurface.num_firsttriangle)[j * 3 + 0] = 0 + extendedSurface.num_firstvertex;
			(m_worldModelExtraInfo.surfmesh.data_element3i + 3 * extendedSurface.num_firsttriangle)[j * 3 + 1] = j + 1 + extendedSurface.num_firstvertex;
			(m_worldModelExtraInfo.surfmesh.data_element3i + 3 * extendedSurface.num_firsttriangle)[j * 3 + 2] = j + 2 + extendedSurface.num_firstvertex;
		}
	}
/*
	// Reliability check.
	InternalAssert (m_worldModelExtraInfo.surfaces == NULL);

	m_worldModelExtraInfo.surfaces = new ExtraModelInfo_t::ExtraSurfaceInfo_t[pModel->numsurfaces];

	int i;
	int totaltris (0), totalverts (0);

	// for all surfaces in the map
	for (i = 0; i < pModel->numsurfaces; ++i)
	{
		const HalfLifeEngine::SDK::Structures::msurface_t &surface (pModel->surfaces[i]);

		totaltris += surface.numedges - 2;
		totalverts += surface.numedges;
	}

	m_worldModelExtraInfo.surfmesh.num_triangles = totaltris;

	// Reliability check.
	InternalAssert (m_worldModelExtraInfo.surfmesh.num_triangles > 0);

	m_worldModelExtraInfo.surfmesh.num_vertices = totalverts;

	// Reliability check.
	InternalAssert (m_worldModelExtraInfo.surfmesh.num_vertices > 0);

	m_worldModelExtraInfo.surfmesh.data_element3i = new int[3 * m_worldModelExtraInfo.surfmesh.num_triangles];
	m_worldModelExtraInfo.surfmesh.data_vertex3f = new Math::Vector3D[m_worldModelExtraInfo.surfmesh.num_vertices];

	totaltris = totalverts = 0;

	// for all surfaces in the map
	for (i = 0; i < pModel->numsurfaces; ++i)
	{
		const HalfLifeEngine::SDK::Structures::msurface_t &surface (pModel->surfaces[i]);
		ExtraModelInfo_t::ExtraSurfaceInfo_t &extendedSurface (m_worldModelExtraInfo.surfaces[i]);

		extendedSurface.num_firsttriangle = totaltris;
		extendedSurface.num_triangles = surface.numedges - 2;
		extendedSurface.num_firstvertex = totalverts;
		extendedSurface.num_vertices = surface.numedges;

		totaltris += surface.numedges - 2;
		totalverts += surface.numedges;

		// convert edges back to a normal polygon
		for (int j (0); j < extendedSurface.num_vertices; ++j)
		{
			const int lindex (pModel->surfedges[surface.firstedge + j]);
			Math::Vector3D &v (m_worldModelExtraInfo.surfmesh.data_vertex3f[extendedSurface.num_firstvertex + j]);
			// note: the q1bsp format does not allow a 0 surfedge (it would have no negative counterpart)
			if (lindex >= 0)
				v = pModel->vertexes[pModel->edges[lindex].v[0]].point;
			else
				v = pModel->vertexes[pModel->edges[-lindex].v[1]].point;
		}

		for (int j (0); j < extendedSurface.num_triangles; ++j)
		{
			(m_worldModelExtraInfo.surfmesh.data_element3i + 3 * extendedSurface.num_firsttriangle)[j * 3 + 0] = 0 + extendedSurface.num_firstvertex;
			(m_worldModelExtraInfo.surfmesh.data_element3i + 3 * extendedSurface.num_firsttriangle)[j * 3 + 1] = j + 1 + extendedSurface.num_firstvertex;
			(m_worldModelExtraInfo.surfmesh.data_element3i + 3 * extendedSurface.num_firsttriangle)[j * 3 + 2] = j + 2 + extendedSurface.num_firstvertex;
		}
	}*/
}
void WorldMap::Mod_Decompile_OBJ(HalfLifeEngine::SDK::Structures::Model_t *model, const char *filename, const char *mtlfilename, const char *originalfilename) const
{
	/// @note Code from "Dark Places" engine project....

	// Reliability check.
	InternalAssert (model == g_server->GetServer ().worldmodel);

	int vertexindex, surfaceindex, triangleindex/*, textureindex*/, countvertices = 0, countsurfaces = 0, countfaces = 0, counttextures = 0;
	int a, b, c;
	const int *e;
	const Math::Vector3D *v;//, *vn;
//	const float *vt;
	size_t outbufferpos = 0;
	size_t outbuffermax = 0x100000;
	char *outbuffer = new char[outbuffermax], *oldbuffer;
	const HalfLifeEngine::SDK::Structures::msurface_t *surface;
	const ExtraModelInfo_t::ExtraSurfaceInfo_t *extendedSurface;
/*	const int maxtextures = 256;
	char *texturenames = new char[maxtextures * MAX_QPATH];
	const char *texname;

	// construct the mtllib file
	outbufferpos += _snprintf(outbuffer + outbufferpos, outbuffermax - outbufferpos, "# mtllib for %s exported by darkplaces engine\n", originalfilename);

	for (surfaceindex = 0, surface = model->surfaces; surfaceindex < model->num_surfaces;surfaceindex++, surface++)
	{
		countsurfaces++;
		countvertices += surface->num_vertices;
		countfaces += surface->num_triangles;
		texname = (surface->texture && surface->texture->name[0]) ? surface->texture->name : "default";
		for (textureindex = 0;textureindex < counttextures;textureindex++)
			if (!strcmp(texturenames + textureindex * MAX_QPATH, texname))
				break;
		if (textureindex < counttextures)
			continue; // already wrote this material entry
		if (textureindex >= maxtextures)
			continue; // just a precaution
		textureindex = counttextures++;
		strlcpy(texturenames + textureindex * MAX_QPATH, texname, MAX_QPATH);
		if (outbufferpos >= outbuffermax >> 1)
		{
			outbuffermax *= 2;
			oldbuffer = outbuffer;
			outbuffer = new char[outbuffermax];
			memcpy(outbuffer, oldbuffer, outbufferpos);
			delete [] oldbuffer;
		}
		outbufferpos += _snprintf(outbuffer + outbufferpos, outbuffermax - outbufferpos, "newmtl %s\nNs 96.078431\nKa 0 0 0\nKd 0.64 0.64 0.64\nKs 0.5 0.5 0.5\nNi 1\nd 1\nillum 2\nmap_Kd %s%s\n\n", texname, texname, strstr(texname, ".tga") ? "" : ".tga");
	}

	// write the mtllib file
	FS_WriteFile(mtlfilename, outbuffer, outbufferpos);
	outbufferpos = 0;
*/
	// construct the obj file
	outbufferpos += _snprintf(outbuffer + outbufferpos, outbuffermax - outbufferpos, "# model exported from %s by darkplaces engine\n# %i vertices, %i faces, %i surfaces\nmtllib %s\n", originalfilename, countvertices, countfaces, countsurfaces, mtlfilename);

	for (vertexindex = 0, v = m_worldModelExtraInfo.surfmesh.data_vertex3f/*, vn = model->surfmesh.data_normal3f, vt = model->surfmesh.data_texcoordtexture2f*/; vertexindex < m_worldModelExtraInfo.surfmesh.num_vertices; ++vertexindex, ++v/*, vn += 3, vt += 2*/)
	{
		if (outbufferpos >= outbuffermax >> 1)
		{
			outbuffermax *= 2;
			oldbuffer = outbuffer;
			outbuffer = new char[outbuffermax];
			memcpy(outbuffer, oldbuffer, outbufferpos);
			delete [] oldbuffer;
		}
//		outbufferpos += _snprintf(outbuffer + outbufferpos, outbuffermax - outbufferpos, "v %f %f %f\nvn %f %f %f\nvt %f %f\n", v->x, v->z, v->y, vn->x, vn->z, -vn.y, vt[0], 1-vt[1]);
		outbufferpos += _snprintf(outbuffer + outbufferpos, outbuffermax - outbufferpos, "v %f %f %f\n", v->x, v->z, v->y);
	}
	for (surfaceindex = 0, surface = model->surfaces, extendedSurface = m_worldModelExtraInfo.surfaces; surfaceindex < m_worldModelExtraInfo.surfacesNumber/*model->numsurfaces*/; ++surfaceindex, ++surface, ++extendedSurface)
	{
//		outbufferpos += _snprintf(outbuffer + outbufferpos, outbuffermax - outbufferpos, "usemtl %s\n", (surface->texture && surface->texture->name[0]) ? surface->texture->name : "default");

		for (triangleindex = 0, e = m_worldModelExtraInfo.surfmesh.data_element3i + extendedSurface->num_firsttriangle * 3; triangleindex < extendedSurface->num_triangles; ++triangleindex, e += 3)
		{
			if (outbufferpos >= outbuffermax >> 1)
			{
				outbuffermax *= 2;
				oldbuffer = outbuffer;
				outbuffer = new char[outbuffermax];
				memcpy(outbuffer, oldbuffer, outbufferpos);
				delete [] oldbuffer;
			}
			a = e[0] + 1;
			b = e[1] + 1;
			c = e[2] + 1;
			outbufferpos += _snprintf(outbuffer + outbufferpos, outbuffermax - outbufferpos, "f %i/%i/%i %i/%i/%i %i/%i/%i\n", a,a,a,b,b,b,c,c,c);
		}
	}

	// write the obj file
	{
//	FS_WriteFile(filename, outbuffer, outbufferpos);
	STDIOFile file (filename, "wt");
	file.Write (outbuffer, outbufferpos, 1u);
	}

	// clean up
	delete [] outbuffer;
//	delete [] texturenames;

	// print some stats
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Wrote %s (%i bytes, %i vertices, %i faces, %i surfaces with %i distinct textures)\n", filename, (int)outbufferpos, countvertices, countfaces, countsurfaces, counttextures);
}
/*
================
Mod_Decompile_f

decompiles a model to editable files
================
*/
void WorldMap::Mod_Decompile_f (void) const
{
	/// @note Code from "Dark Places" engine project....

	int i, j, k, l, first, count;
	HalfLifeEngine::SDK::Structures::Model_t *mod;
	char inname[MAX_PATH];
	char outname[MAX_PATH];
	char mtlname[MAX_PATH];
	char basename[MAX_PATH];
	char animname[MAX_PATH];
	char animname2[MAX_PATH];
	char zymtextbuffer[16384];
	char dpmtextbuffer[16384];
	char framegroupstextbuffer[16384];
	int zymtextsize = 0;
	int dpmtextsize = 0;
	int framegroupstextsize = 0;
/*
	if (Cmd_Argc() != 2)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("usage: modeldecompile <filename>\n");
		return;
	}
*/
//	strlcpy(inname, /*Cmd_Argv(1)*/, sizeof(inname));
//	FS_StripExtension(inname, basename, sizeof(basename));

	mod = g_server->GetServer ().worldmodel;//Mod_ForName(inname, false, true, inname[0] == '*' ? cl.model_name[1] : NULL);
	if (!mod)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("No such model\n");
		return;
	}
	if (!/*mod->*/m_worldModelExtraInfo.surfmesh.num_triangles)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Empty model (or sprite)\n");
		return;
	}
/*	if (mod->brush.submodel)
	{
		// if we're decompiling a submodel, be sure to give it a proper name based on its parent
		FS_StripExtension(cl.model_name[1], outname, sizeof(outname));
		_snprintf(basename, sizeof(basename), "%s/%s", outname, mod->name);
		outname[0] = '\0';
	}
*/
	// export OBJ if possible (not on sprites)
	{
//		_snprintf(outname, sizeof(outname), "%s_decompiled.obj", basename);
//		_snprintf(mtlname, sizeof(mtlname), "%s_decompiled.mtl", basename);
		Mod_Decompile_OBJ(mod, /*outname*/FormatBuffer ("C:/Users/Ýäóàðä/Desktop/decompiled_%s.obj", /*mod->name*/HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ()), mtlname, inname);
	}
/*
	// export SMD if possible (only for skeletal models)
	if (mod->num_bones)
	{
		_snprintf(outname, sizeof(outname), "%s_decompiled/ref1.smd", basename);
		Mod_Decompile_SMD(mod, outname, 0, 1, true);
		zymtextsize += _snprintf(zymtextbuffer + zymtextsize, sizeof(zymtextbuffer) - zymtextsize, "output out.zym\nscale 1\norigin 0 0 0\nmesh ref1.smd\n");
		dpmtextsize += _snprintf(dpmtextbuffer + dpmtextsize, sizeof(dpmtextbuffer) - dpmtextsize, "outputdir .\nmodel out\nscale 1\norigin 0 0 0\nscene ref1.smd\n");

		for (i = 0;i < mod->numframes;i = j)
		{
			strlcpy(animname, mod->animscenes[i].name, sizeof(animname));
			first = mod->animscenes[i].firstframe;
			if (mod->animscenes[i].framecount > 1)
			{
				// framegroup anim
				count = mod->animscenes[i].framecount;
				j = i + 1;
			}
			else
			{
				// individual frame
				// check for additional frames with same name
				for (l = 0, k = strlen(animname);animname[l];l++)
					if(animname[l] < '0' || animname[l] > '9')
						k = l + 1;
				if(k > 0 && animname[k-1] == '_')
					--k;
				animname[k] = 0;
				count = mod->num_poses - first;
				for (j = i + 1;j < mod->numframes;j++)
				{
					strlcpy(animname2, mod->animscenes[j].name, sizeof(animname2));
					for (l = 0, k = strlen(animname2);animname2[l];l++)
						if(animname2[l] < '0' || animname2[l] > '9')
							k = l + 1;
					if(k > 0 && animname[k-1] == '_')
						--k;
					animname2[k] = 0;
					if (strcmp(animname2, animname) || mod->animscenes[j].framecount > 1)
					{
						count = mod->animscenes[j].firstframe - first;
						break;
					}
				}
				// if it's only one frame, use the original frame name
				if (j == i + 1)
					strlcpy(animname, mod->animscenes[i].name, sizeof(animname));
				
			}
			_snprintf(outname, sizeof(outname), "%s_decompiled/%s.smd", basename, animname);
			Mod_Decompile_SMD(mod, outname, first, count, false);
			if (zymtextsize < (int)sizeof(zymtextbuffer) - 100)
			{
				zymtextsize += _snprintf(zymtextbuffer + zymtextsize, sizeof(zymtextbuffer) - zymtextsize, "scene %s.smd fps %g %s\n", animname, mod->animscenes[i].framerate, mod->animscenes[i].loop ? "" : " noloop");
			}
			if (dpmtextsize < (int)sizeof(dpmtextbuffer) - 100)
			{
				dpmtextsize += _snprintf(dpmtextbuffer + dpmtextsize, sizeof(dpmtextbuffer) - dpmtextsize, "scene %s.smd fps %g %s\n", animname, mod->animscenes[i].framerate, mod->animscenes[i].loop ? "" : " noloop");
			}
			if (framegroupstextsize < (int)sizeof(framegroupstextbuffer) - 100)
			{
				framegroupstextsize += _snprintf(framegroupstextbuffer + framegroupstextsize, sizeof(framegroupstextbuffer) - framegroupstextsize, "%d %d %f %d // %s\n", first, count, mod->animscenes[i].framerate, mod->animscenes[i].loop, animname);
			}
		}
		if (zymtextsize)
			FS_WriteFile(va("%s_decompiled/out_zym.txt", basename), zymtextbuffer, (fs_offset_t)zymtextsize);
		if (dpmtextsize)
			FS_WriteFile(va("%s_decompiled/out_dpm.txt", basename), dpmtextbuffer, (fs_offset_t)dpmtextsize);
		if (framegroupstextsize)
			FS_WriteFile(va("%s_decompiled.framegroups", basename), framegroupstextbuffer, (fs_offset_t)framegroupstextsize);
	}*/
}

const bool WorldMap::WalkFaceBelongsToSector (const WalkFace_t &face, const unsigned char sector_i, const unsigned char sector_j) const
{
	// this function returns true if the walkFace pointed to by 'face' belongs to the topological
	// sector whose position in the global array is [sector_i][sector_j], false otherwise. The
	// local variables are defined static so as to speedup recursive calls of this function, which
	// is extensively used in LookDownOnTheWorld().

	// Math code courtesy of Paul "Cheesemonster" Murphy (thanks mate!).

	static float x, y, a, b, angle, angleDifference;
	static Math::Vector3D bound1, bound2, segmentLeft, segmentRight;
	const Math::Vector2D temp (m_bspFile.models[0u].boundingBox.GetSizeX () / m_parallelsCount, m_bspFile.models[0u].boundingBox.GetSizeY () / m_meridiansCount);

	// first compute the left, right, top and bottom coordinates indices of the sector
	const float sectorLeft (m_bspFile.models[0u].boundingBox.mins.x + sector_i * temp.x);
	const float sectorRight (m_bspFile.models[0u].boundingBox.mins.x + (sector_i + 1u) * temp.x);
	const float sectorBottom (m_bspFile.models[0u].boundingBox.mins.y + sector_j * temp.y);
	const float sectorTop (m_bspFile.models[0u].boundingBox.mins.y + (sector_j + 1u) * temp.y);

	// reset angle which will hold the sum of angles to face corners
	angle = 0.0f;

	// state that we've not found the minimal angle difference between two corners of this face
	// for the point we consider to be "inside" it. This is very important. Since a face is a
	// convex polygon with N corners, the minimal angle difference between any point in this
	// face and any of two successive corners of the face must be (360 / N) degrees. Example:
	// if you are standing strictly in the middle of a square, you will be figuring four 90
	// degree angles between each corner of the face. Stand anywhere else inside the square and
	// you will necessarily have to figure AT LEAST one of these four angles to be MORE than
	// 90 degrees. This check ensures that in case you are standing kilometers away from the
	// face, all these angles summed up eventually still being very close to zero, the algorithm
	// won't be fooled by thinking that you are standing in it (in fact, you are not only OUT of
	// it, but KILOMETERS AWAY).
	const float minimumAngleDifference (360.0f / face.cornerCount);
	static bool minimumAngleDifferenceFound;

	minimumAngleDifferenceFound = false;

	// loop though the corners of this face...
	for (unsigned short cornerIndex (0u); cornerIndex < face.cornerCount; ++cornerIndex)
	{
		// locate the first vertice of this edge
		bound1 = face.corners[cornerIndex];

		// locate the second vertice of this edge. If cornerIndex < face->cornerCount - 1, get next corner in the array, else loop back to corner zero at last corner
		bound2 = face.corners[cornerIndex < face.cornerCount - 1u ? cornerIndex + 1u : 0u];

		// now we have one of the edges of the face, let's see if this segment is included within
		// the sector, completely crosses it, or crosses at least one of the sector's boundaries;
		// in this case, our face will belong to this sector indeed. But beware! face could STILL
		// belong to sector if it was much larger than the sector itself, and the sector would be
		// COMPLETELY comprised within the face (in this case, the sector would have one face and
		// only one, this face). To check for that extra situation, we use a method that Jeffrey
		// 'botman' Broome inspired us, i.e, we take one of the corner of the sector, and see if the
		// sum of the angles that are (edge N bound 1 -> corner of sector -> edge N bound 2) for
		// each edge N of the face, is 360; in this case, the corner of the sector will be just
		// on top of face, and it'll mean that face belongs to the specified sector. That's why we
		// first go for the inclusion/intersection check, and after that we go for the angles.

		// its obvious it is in sector, if one or both points are inside the box
		if
		(
			(bound1.x >= sectorLeft && bound1.x <= sectorRight && bound1.y >= sectorBottom && bound1.y <= sectorTop) ||
			(bound2.x >= sectorLeft && bound2.x <= sectorRight && bound2.y >= sectorBottom && bound2.y <= sectorTop)
		)
			return true;	// if so, segment belongs to sector

		// at this point, NONE of the segment bounds are inside the box

		// is this segment vertical?
		if (bound1.x == bound2.x)
		{
			// is this vertical segment comprised between left and righ boundaries of the sector?
			if (bound1.x >= sectorLeft && bound1.x <= sectorRight)
			{
				// if so, it belongs to sector if, and only if, the bottom bound is lower than the
				// bottom limit of the sector and the top bound is higher than the top limit of it.

				// is the first bound the bottom bound?
				if (bound1.y < bound2.y)
				{
					if (bound1.y <= sectorBottom && bound2.y >= sectorTop)
						return true;	// segment crosses the sector completely
				}
				else
				{
					if (bound2.y <= sectorBottom && bound1.y >= sectorTop)
						return true;	// segment crosses the sector completely
				}
			}

			// if this is not the case, either the segment is:
			// - completely on the left of the sector
			// - completely on the right of the sector
			// - completely beyond the top of the sector
			// - or it is completely under the bottom of it.
		}
		else if (bound1.y == bound2.y)	// else is this segment horizontal?
		{
			// is horizontal segment comprised between the bottom and top boundaries of the sector?
			if (bound1.y >= sectorBottom && bound1.y <= sectorTop)
			{
				// if so, it belongs to sector if, and only if, the left bound is beyond the left
				// limit of the sector and the right bound is beyond the right limit of it.

				// is the first bound the left bound?
				if (bound1.x < bound2.x)
				{
					if (bound1.x <= sectorLeft && bound2.x >= sectorRight)
						return true;	// segment crosses the sector completely
				}
				else
				{
					if (bound2.x <= sectorLeft && bound1.x >= sectorRight)
						return true;	// segment crosses the sector completely
				}
			}

			// if this is not the case, either the segment is:
			// - completely on the left of the sector
			// - completely on the right of the sector
			// - completely beyond the top of the sector
			// - or it is completely under the bottom of it.
		}
		else	// else segment is neither horizontal nor vertical, but just bent (grr)
		{
			// we have to compute the bastard's equation.

			// arrange the bounds of the segment in the right order relatively to the X axis
			if (bound1.x < bound2.x)
			{
				segmentLeft = bound1;
				segmentRight = bound2;
			}
			else
			{
				segmentLeft = bound2;
				segmentRight = bound1;
			}

			// the equation of a line is under the form y = ax + b, we need to find a and b.
			a = (bound2.x - bound1.y) / (bound2.x - bound1.x);	// compute the slope: a
			b = bound1.y - a * bound1.x;	// compute the Y offset at origin: b

			// at this point, none of the segment bounds are inside the box; but now, we do know its
			// equation, so we can check if the segment intersects either of the sector's boundaries.

			// compute coordinates of intersection point of line and sector's LEFT bound
			// coordinates are: (x, y)
			x = sectorLeft;
			y = a * x + b;

			// does this intersection point belongs to both segments?
			if (x >= segmentLeft.x && x <= segmentRight.x && y >= segmentLeft.y && y <= segmentRight.y && y >= sectorBottom && y <= sectorTop)
				return true;	// the segment crosses the sector's LEFT boundary

			// compute coordinates of intersection point of line and sector's RIGHT bound
			// coordinates are: (x, y)
			x = sectorRight;
			y = a * x + b;

			// does this intersection point belongs to both segments?
			if (x >= segmentLeft.x && x <= segmentRight.x && y >= segmentLeft.y && y <= segmentRight.y && y >= sectorBottom && y <= sectorTop)
				return true;	// the segment crosses the sector's LEFT boundary

			// at this point, none of the segment bounds are inside the box NOR does it intersects
			// the left and right boundaries of the sector; let's turn its equation upside down so
			// that we have x = f(y)

			// compute coordinates of intersection point of line and sector's TOP bound
			// coordinates are: (x, y)
			y = sectorTop;
			x = (y - b) / a;

			// does this intersection point belongs to both segments?
			if (x >= segmentLeft.x && x <= segmentRight.x && y >= segmentLeft.y && y <= segmentRight.y && x >= sectorLeft && x <= sectorRight)
				return true;	// the segment crosses the sector's TOP boundary

			// compute coordinates of intersection point of line and sector's BOTTOM bound
			// coordinates are: (x, y)
			y = sectorBottom;
			x = (y - b) / a;

			// does this intersection point belongs to both segments?
			if (x >= segmentLeft.x && x <= segmentRight.x && y >= segmentLeft.y && y <= segmentRight.y && x >= sectorLeft && x <= sectorRight)
				return true;	// the segment crosses the sector's BOTTOM boundary
		}

		// at this point, we are assured that none of the segment bounds are inside the box AND
		// that it intersects NONE of the four segments that make the boundaries of the sector.
		// The walkface may still be larger than the sector and the sector comprised "inside" it.

		// figure out the angle difference between this point and one corner of the sector
		angleDifference = Math::fabsf ((Math::Vector3D (bound1.x, bound1.y, 0.0f) - Math::Vector3D (sectorLeft, sectorBottom, 0.0f)).GetAngleBetween (Math::Vector3D (bound2.x, bound2.y, 0.0f) - Math::Vector3D (sectorLeft, sectorBottom, 0.0f)));

		// is this angle GREATER than the minimal angle difference?
		if (angleDifference > minimumAngleDifference)
			minimumAngleDifferenceFound = true;	// ha, this indicates that this face is NOT kilometers away

		// now sum up all the angles corner - point - next corner to see if we have 360 (thx botman)
		angle += angleDifference;

		// and go looping again for the next edge of the face...
	}

	// okay, here, NONE of the segment inclusion/intersection checks succeeded, so it's time to
	// check whether we're in that particular case where the sector is completely included within
	// the face, using the angle method we computed earlier.

	// has the minimal angle difference been NOT found?
	if (!minimumAngleDifferenceFound)
		return false;	// then this face is KILOMETERS away from the sector, so forget it.

	// if the resulting angle is close to 360, then the point is likely to be on the face
	if (Math::fabsf (Math::AngleNormalize (angle - 360.0f)) < Math::RawEpsilon)
		return true;	// sector is included within this face, so face belongs to sector

	// all our checks failed, face can't possibly belong to sector, return false.
	return false;
}

const bool WorldMap::Load (const unsigned int bspFileSize)
{
	// this function checks if a world map created by LookDownOnTheWorld() exists in a .MAP file
	// on disk, and in case it does, opens and loads it, and returns true if the loading process
	// completed successfully. If such a map doesn't exist or an error occured in the loading
	// process, this function returns false. The format of a .MAP file is divided into chunks.
	// For an explanation of the different chunks of which .MAP files are made, refer to the
	// Save() function.

	AddLogEntry (true, LogLevel_Default, false, "Trying to load .MAP file for current map....\n");

	// load the map file and get the size it claims the BSP file to have
	STDIOFile file (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/addons/YaPB/World maps/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + '.' + FileExtention, "rb");	// open this file in binary read mode

	if (!file.IsValid ())
		return false;	// map file not found, return an error condition

	unsigned int mapFileSize;

	file.Seek (sizeof ("YAPB_WORLD_MAP"), BaseFile::SeekType_Current);	// skip the YAPB_WORLD_MAP tag
	file.Seek (sizeof ("[file size]"), BaseFile::SeekType_Current);	// skip the filesize chunk tag

	// and read the map file size there
	file.Read (mapFileSize);

	// is the recorded file size NOT the same as the actual BSP file size?
	if (bspFileSize != mapFileSize)
	{
		// map file is badly authenticated, return an error condition
		return false;
	}

	// else world map file is OK, load its data completely this time

	file.Seek (0L, BaseFile::SeekType_Start);	// seek at start of file
	file.Seek (sizeof ("YAPB_WORLD_MAP"), BaseFile::SeekType_Current);	// skip the YAPB_WORLD_MAP tag
	file.Seek (sizeof ("[file size]"), BaseFile::SeekType_Current);	// skip the filesize chunk tag
	file.Seek (sizeof (bspFileSize), BaseFile::SeekType_Current);			// skip the filesize chunk data (we already read it)

	// seek at start of walkable faces chunk data
	file.Seek (sizeof ("[walk faces]"), BaseFile::SeekType_Current);

	unsigned short walkFacesCount;

	// read the walkable faces lump (and mallocate for it on the fly...)
	file.Read (walkFacesCount);

	if (walkFacesCount == 0u)
		return false;	// error, no walkable faces have been recorded

	unsigned short faceIndex;

	// tell people what we are doing
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Fetching world map.\n");

	for (faceIndex = 0u; faceIndex < walkFacesCount; ++faceIndex)
	{
		// we know this map to have one walkable face more....
		if (!m_walkFaces.PushBackDefaultElement ())
		{
			AddLogEntry (false, LogLevel_Critical, true, "BARADA!!!");

			return false;
		}

		// quick access to walk face
		WalkFace_t &walkFace (m_walkFaces.GetLastElement ());

		// read # of corners for this face
		file.Read (walkFace.cornerCount);

		walkFace.corners = new Math::Vector3D[walkFace.cornerCount];

		#if defined _DEBUG
			if (walkFace.corners == NULL)
			{
				TerminateOnMalloc ();

				return false;
			}
		#endif	// if defined _DEBUG

		for (unsigned short cornerIndex (0u); cornerIndex < walkFace.cornerCount; ++cornerIndex)
			file.Read (walkFace.corners[cornerIndex]);
	}

	// seek at start of topology chunk data
	file.Seek (sizeof ("[topology]"), BaseFile::SeekType_Current);

	// read the topology chunk data (and mallocate for it on the fly...)
	file.Read (m_parallelsCount);
	file.Read (m_meridiansCount);

	// Reliability checks.
	InternalAssert (m_parallelsCount < MAX_MAP_PARALLELS);
	InternalAssert (m_meridiansCount < MAX_MAP_MERIDIANS);

	unsigned short arrayIndex;

	for (unsigned char i (0u), j; i < m_parallelsCount; ++i)
	{
		for (j = 0u; j < m_meridiansCount; ++j)
		{
			unsigned short count;

			// read # faces in sector
			file.Read (count);

			// translate each face array index into a pointer
			for (faceIndex = 0u; faceIndex < count; ++faceIndex)
			{
				file.Read (arrayIndex);

				// test this index against overflow
				if (arrayIndex >= m_walkFaces.GetElementNumber ())
				{
					AddLogEntry (false, LogLevel_Critical, false, "Load(): bad face array index: %u (max: %u) at: [%u][%u], index: %u/%u.", arrayIndex, m_walkFaces.GetElementNumber () - 1u, i, j, faceIndex, count);

					return false;
				}

				WalkFace_t *const walkFace (reinterpret_cast <WalkFace_t *> (reinterpret_cast <unsigned int> (m_walkFaces.GetData ()) + arrayIndex * sizeof (WalkFace_t)));

				// test this pointer against access violation (pointers are plain evil)
				if (!m_walkFaces.IsInside (walkFace))
				{
					AddLogEntry (false, LogLevel_Critical, false, "Load(): bad face pointer: %u (range: %u - %u) at: [%u][%u], index: %u/%u.", walkFace, m_walkFaces.GetData (), m_walkFaces.GetData () + (m_walkFaces.GetElementNumber () - 1u), i, j, faceIndex, count);

					return false;
				}

				m_topology[i][j] += walkFace;
			}
		}

		// print a trailing dot as a progress bar
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (".");
	}

	// finished, terminate the progress bar
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("	loading done!\n	%u parallels, %u meridians, %.2f kilobytes world data.\n", m_parallelsCount, m_meridiansCount, file.GetSize () / 1024.0f);

	AddLogEntry (true, LogLevel_Default, false, "\n\t%u leafs,\n\t%u models,\n\t%u entities,\n\t%u planes,\n\t%u vertexes,\n\t%u nodes,\n\t%u clip nodes,\n\t%u faces,\n\t%u edges,\n\t%u surf edges,\n\t%u mark surfaces,\n\t%u textures data,\n\t%u textures information loaded!", m_bspFile.leafs.GetElementNumber (), m_bspFile.models.GetElementNumber (), m_bspFile.entities.GetElementNumber (), m_bspFile.planes.GetElementNumber (), m_bspFile.vertexes.GetElementNumber (), m_bspFile.nodes.GetElementNumber (), m_bspFile.clipNodes.GetElementNumber (), m_bspFile.faces.GetElementNumber (), m_bspFile.edges.GetElementNumber (), m_bspFile.surfEdges.GetElementNumber (), m_bspFile.markSurfaces.GetElementNumber (), m_bspFile.texturesData.GetElementNumber (), m_bspFile.texturesInformation.GetElementNumber ());

	// and return the error state (no error)
	return true;
}

const unsigned int WorldMap::Save (const unsigned int bspFileSize) const
{
	// this function saves the loaded world map created by LookDownOnTheWorld() in a .MAP file on
	// disk. The format of this file is divided into chunks. The first chunk is an authentication
	// tag, which has the value "YAPB_WORLD_MAP" followed by an end of string null marker. All the other
	// chunks are explicitly named in ASCII characters in the file, preceding the chunk data. The
	// chunks coming after the "YAPB_WORLD_MAP" marker are:
	//	[file size] - tells the size of the BSP file this world map has been drawn for
	//	[world size] - gives info about the virtual world's bounding box mins and maxs limits
	//	[walk faces] - array of the walkable faces data, involving their corners and delimiters
	//	[topology] - number of parallels and meridians in this map, followed by the sector hash table

	const DynamicString directory (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/World maps");

	if (!IsDirectoryExists (directory) && !CreatePath (directory))
		return false;

	const DynamicString filename (directory + '/' + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + '.' + FileExtention);

	// open or create such a file in binary write mode
	STDIOFile file (filename, "wb");

	if (!file.IsValid ())
	{
		AddLogEntry (false, LogLevel_Critical, false, "Unable to save new world map to '%s'.", filename.GetData ());

		return 0u;
	}

	unsigned short faceIndex;

	// write the authentication chunk
	file.Write ("YAPB_WORLD_MAP", sizeof ("YAPB_WORLD_MAP"));

	// don't write the file size chunk yet (will do this when the map will be fully saved)
	// this ensure an error in the save process won't let a badly authenticated file on disk
	file.Write ("[file size]", sizeof ("[file size]"));
//	file.Write ("\0\0\0\0", sizeof (bspFileSize));	// fill the field with zeroes (temporarily)
	file.Write (bspFileSize);

	// write the walkable faces chunk
	file.Write ("[walk faces]", sizeof ("[walk faces]"));

	unsigned short count (m_walkFaces.GetElementNumber ());

	file.Write (count);	// write the number of faces

	for (faceIndex = 0u; faceIndex < count; ++faceIndex)
	{
		// write # of corners for this face
		file.Write (m_walkFaces[faceIndex].cornerCount);

		for (unsigned short cornerIndex (0u); cornerIndex < m_walkFaces[faceIndex].cornerCount; ++cornerIndex)
			file.Write (m_walkFaces[faceIndex].corners[cornerIndex]);	// write the corner
	}

	// write the topology chunk
	file.Write ("[topology]", sizeof ("[topology]"));
	file.Write (m_parallelsCount);
	file.Write (m_meridiansCount);

	for (unsigned char i (0u), j; i < m_parallelsCount; ++i)
	{
		for (j = 0u; j < m_meridiansCount; ++j)
		{
			count = m_topology[i][j].GetElementNumber ();

			// write # faces in sector
			file.Write (count);

			for (faceIndex = 0u; faceIndex < count; ++faceIndex)
			{
				// translate the pointer address into an array relative index
				const unsigned short arrayIndex (WalkFaceIndexOf (*m_topology[i][j][faceIndex]));

				file.Write (arrayIndex);
			}
		}

		// print a trailing dot as a progress bar
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (".");
	}

	// get the file size, i.e our current position before we rewind
	const unsigned int size (file.GetSize ());
/*
	// now we're ready to write the authentication lump
	file.Seek (0L, BaseFile::SeekType_Start);	// rewind the file
	file.Seek (sizeof ("YAPB_WORLD_MAP"), BaseFile::SeekType_Current);	// skip the YAPB_WORLD_MAP tag
	file.Seek (sizeof ("[file size]"), BaseFile::SeekType_Current);	// skip the filesize chunk tag

	// and write the file size
	file.Write (bspFileSize);
*/
	// and return the world data size
	return size;
}

const Sector_t &WorldMap::GetSectorUnder (HalfLifeEngine::SDK::Classes::Edict *const entity) const
{
	// this function returns the topological sector in which entity is currently located.
	// All the local variables have been declared static to speedup recurrent calls of this function.

	// Reliability check.
	InternalAssert (entity->IsValid ());

	// return a pointer to the appropriate sector
	return GetSectorUnder (entity->GetOrigin ());
}

const Sector_t &WorldMap::GetSectorUnder (const Math::Vector2D &location) const
{
	// this function returns the topological sector in which location is currently located.
	// All the local variables have been declared static to speedup recurrent calls of this function.

	static unsigned char i, j;

	// compute the latitude and longitude in the topologic array,
	// and handle the cases where the delimiter is just on the right or upper edge of the array...
//	i = static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((location.x - m_bspFile.models[0u].boundingBox.mins.x) / (m_bspFile.models[0u].boundingBox.maxs.x - m_bspFile.models[0u].boundingBox.mins.x) * m_parallelsCount), 0, m_parallelsCount - 1));
//	j = static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((location.y - m_bspFile.models[0u].boundingBox.mins.y) / (m_bspFile.models[0u].boundingBox.maxs.y - m_bspFile.models[0u].boundingBox.mins.y) * m_meridiansCount), 0, m_meridiansCount - 1));
	i = static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((location.x - m_bspFile.models[0u].boundingBox.mins.x) / GridCellSize), 0, m_parallelsCount - 1));
	j = static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((location.y - m_bspFile.models[0u].boundingBox.mins.y) / GridCellSize), 0, m_meridiansCount - 1));

	// and return a pointer to the appropriate sector
	return m_topology[i][j];
}

WalkFace_t *const WorldMap::GetWalkFaceUnder (HalfLifeEngine::SDK::Classes::Edict *const player)
{
	// this function returns the ground face supporting 'player' on floor. All the local variables
	// have been made static to speedup recurrent calls of this function.
	// A player is HalfLifeEngine::SDK::Constants::HumanWidth units wide, and can climb up slopes up to 45°.

	// Reliability check.
	InternalAssert (player->IsValid ());

	// Reliability check.
	if (!player->IsPlayerOnFloor ())
		return NULL;

	static HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
	const HalfLifeEngine::SDK::Classes::Edict *const worldEdict (HalfLifeEngine::Globals::g_halfLifeEngine->GetEdictOfIndex (0u));
	static Math::Vector3D hitPoint;

	// get a quick access to the player origin
	Math::Vector3D entityOrigin (player->GetBottomBModelOrigin ());	// this entity has a bbox

	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (entityOrigin + Math::Vector3D (0.0f, 0.0f, 10.0f), entityOrigin - Math::Vector3D (0.0f, 0.0f, 10.0f), true, HalfLifeEngine::SDK::Constants::Hull_Point, player, traceResult);

	hitPoint = traceResult.endPosition;	// remember the hit point

	// found something?
	if (traceResult.fraction < 1.0f)
	{
		if (traceResult.hitEntity != worldEdict)
			return NULL;	// ground is not the world entity! Return a NULL walkface.
	}
	// else this entity is partially on the ground. This sucks, because we'll have to check the
	// 4 corners of its bounding box in order to see which one is the supporting one.
	else
	{
		static float nearest_fraction;

		nearest_fraction = 1.0f;	// set this to be sure

		// find out which corner of the bounding box is the supporting one
		entityOrigin.x = player->variables.origin.x - HalfLifeEngine::SDK::Constants::HumanWidth * 0.5f;	// corner 1
		entityOrigin.y = player->variables.origin.y - HalfLifeEngine::SDK::Constants::HumanWidth * 0.5f;
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (entityOrigin + Math::Vector3D (0.0f, 0.0f, 10.0f), entityOrigin + Math::Vector3D (0.0f, 0.0f, -20.0f), true, HalfLifeEngine::SDK::Constants::Hull_Point, player, traceResult);

		if (traceResult.fraction < nearest_fraction && traceResult.hitEntity == worldEdict)
			hitPoint = traceResult.endPosition;

		entityOrigin.x = player->variables.origin.x - HalfLifeEngine::SDK::Constants::HumanWidth * 0.5f;	// corner 2
		entityOrigin.y = player->variables.origin.y + HalfLifeEngine::SDK::Constants::HumanWidth * 0.5f;
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (entityOrigin + Math::Vector3D (0.0f, 0.0f, 10.0f), entityOrigin + Math::Vector3D (0.0f, 0.0f, -20.0f), true, HalfLifeEngine::SDK::Constants::Hull_Point, player, traceResult);

		if (traceResult.fraction < nearest_fraction && traceResult.hitEntity == worldEdict)
			hitPoint = traceResult.endPosition;

		entityOrigin.x = player->variables.origin.x + HalfLifeEngine::SDK::Constants::HumanWidth * 0.5f;	// corner 3
		entityOrigin.y = player->variables.origin.y + HalfLifeEngine::SDK::Constants::HumanWidth * 0.5f;
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (entityOrigin + Math::Vector3D (0.0f, 0.0f, 10.0f), entityOrigin + Math::Vector3D (0, 0, -20), true, HalfLifeEngine::SDK::Constants::Hull_Point, player, traceResult);

		if (traceResult.fraction < nearest_fraction && traceResult.hitEntity == worldEdict)
			hitPoint = traceResult.endPosition;

		entityOrigin.x = player->variables.origin.x + HalfLifeEngine::SDK::Constants::HumanWidth * 0.5f;	// corner 4
		entityOrigin.y = player->variables.origin.y - HalfLifeEngine::SDK::Constants::HumanWidth * 0.5f;
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (entityOrigin + Math::Vector3D (0.0f, 0.0f, 10.0f), entityOrigin + Math::Vector3D (0.0f, 0.0f, -20.0f), true, HalfLifeEngine::SDK::Constants::Hull_Point, player, traceResult);

		if (traceResult.fraction < nearest_fraction && traceResult.hitEntity == worldEdict)
			hitPoint = traceResult.endPosition;

		if (nearest_fraction == 1.0f)
			return NULL;	// if no frikkin hit point was found on the FOUR corners, give up. Bah.
	}

	static float angle, angleDifference;
	static bool minimumAngleDifferenceFound;
	static WalkFace_t *face;
	static Math::Vector3D bound1, bound2;

	// get the sector it is in the topology hash table
	const Sector_t &sector (GetSectorUnder (hitPoint));

	// loop through all the face we know to be in this topological zone
	for (unsigned short faceIndex (0u); faceIndex < sector.GetElementNumber (); ++faceIndex)
	{
		// quick access to the face
		face = sector[faceIndex];

		// reset angle
		angle = 0.0f;

		// reset the minimal angle difference to be found
		const float minimumAngleDifference (360.0f / face->cornerCount);

		minimumAngleDifferenceFound = false;

		// loop though the corners of this face...
		for (unsigned short cornerIndex (0u); cornerIndex < face->cornerCount; ++cornerIndex)
		{
			// locate the first vertice of this corner
			bound1 = face->corners[cornerIndex];

			// locate the second vertice of this corner
			if (cornerIndex < face->cornerCount - 1u)
				bound2 = face->corners[cornerIndex + 1u];	// next corner in the array
			else
				bound2 = face->corners[0u];	// loop back to corner zero at last corner

			// figure out the angle difference between this point and one corner of the sector
			angleDifference = Math::fabsf ((bound1 - hitPoint).GetAngleBetween (bound2 - hitPoint));

			// is this angle GREATER than the minimal angle difference?
			if (angleDifference > minimumAngleDifference)
				minimumAngleDifferenceFound = true;	// ha, this indicates that this face is NOT kilometers away

			// sum up all the angles corner - point - next corner and check if we have 360 (thx botman)
			angle += angleDifference;
		}

		// if the resulting angle is close to 360, then the point is likely to be on the face
		if (Math::fabsf (Math::AngleNormalize (angle - 360.0f)) < Math::RawEpsilon)
			return face;	// assume entity is on this face
	}

	// if navigation debug level is very high, let us know that we couldn't find any walk face
//	if (DebugLevel.navigation > 2)
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("GetWalkFaceUnder(): could not determine walk face under player \"%s\".\n", player->GetNetName ().GetData ());

	// not found a face on which entity could be on...
	return NULL;
}

WalkFace_t *const WorldMap::GetWalkFaceUnder (const Math::Vector3D &location)
{
	// this function returns the ground face under location on the floor.
	// The local variables have been made static to speedup recurrent calls of this function.

	static Math::Vector3D loweredLocation, bound1, bound2;

	// get a quick access to a lowered version of location
	loweredLocation = DropToFloor (location);	// drop location on the floor

	// Reliability check.
	if (loweredLocation == Math::Vector3D::ZeroValue)
		return NULL;

	static float angle, angleDifference;
	static bool minimumAngleDifferenceFound;
	static WalkFace_t *face;

	// get the sector it is in the topology hash table
	const Sector_t &sector (GetSectorUnder (loweredLocation));

	// loop through all the face we know to be in this topological sector
	for (unsigned short faceIndex (0u); faceIndex < sector.GetElementNumber (); ++faceIndex)
	{
		// quick access to the face
		face = sector[faceIndex];

		// reset angle
		angle = 0.0f;

		// reset the minimal angle difference to be found
		const float minimumAngleDifference (360.0f / face->cornerCount);

		minimumAngleDifferenceFound = false;

		// loop though the corners of this face...
		for (unsigned short cornerIndex (0u); cornerIndex < face->cornerCount; ++cornerIndex)
		{
			// locate the first vertice of this corner
			bound1 = face->corners[cornerIndex];

			// locate the second vertice of this corner
			if (cornerIndex < face->cornerCount - 1u)
				bound2 = face->corners[cornerIndex + 1u];	// next corner in the array
			else
				bound2 = face->corners[0u];	// loop back to corner zero at last corner

			// figure out the angle difference between this point and one corner of the sector
			angleDifference = Math::fabsf ((bound1 - loweredLocation).GetAngleBetween (bound2 - loweredLocation));

			// is this angle GREATER than the minimal angle difference?
			if (angleDifference > minimumAngleDifference)
				minimumAngleDifferenceFound = true;	// ha, this indicates that this face is NOT kilometers away

			// sum up all the angles corner - point - next corner and check if we have 360 (thx botman)
			angle += angleDifference;
		}

		// if the minimal angle difference has been found
		// AND the resulting angle is close to 360, then the point is likely to be on the face
		if (minimumAngleDifferenceFound && Math::fabsf (Math::AngleNormalize (angle - 360.0f)) < Math::RawEpsilon)
			return face;	// assume location vector is on this face
	}

	// if navigation debug level is very high, let us know that we couldn't find any walkFace
//	if (DebugLevel.navigation > 2)
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("GetWalkFaceUnder(): could not determine walk face for location (%.2f, %.2f, %.2f).\n", location.x, location.y, location.z);

	// not found a face to which location vector could belong...
	return NULL;
}

const unsigned short WorldMap::WalkFaceIndexOf (const WalkFace_t &walkFace) const
{
	// this function converts a 'walkFace' pointer into its corresponding index in the global walk faces array.
	// Local variables have been declared static to speedup recurrent calls of this function.

//	const unsigned short index (static_cast <unsigned short> ((reinterpret_cast <unsigned int> (&walkFace) - reinterpret_cast <unsigned int> (m_walkFaces.GetData ())) / sizeof (WalkFace_t)));
	const unsigned short index (m_walkFaces.GetElementIndex (&walkFace));

	// check for the index validity (it must ALWAYS be valid, so bomb out on error)
	if (index >= m_walkFaces.GetElementNumber ())
	{
		AddLogEntry (false, LogLevel_Critical, false, "WalkFaceIndexOf(): bad face array index %u (range 0 - %u).", index, m_walkFaces.GetElementNumber () - 1u);

		return 0u;
	}

	// looks like we found a valid index, so return it
	return index;
}

void WorldMap::FreeMapData (void)
{
	// this function is in charge of freeing all the memory space allocated for the map data,
	// because the map is being shutdown. A check is made upon the space pointer we want to free
	// not to free it twice in case it'd have already been done (malloc and free implementations
	// are usually so crappy they hardly ever give error messages, rather crash without warning).
	// For safety reasons, we also reset the pointer to NULL, for further malloc()s not to be
	// confused about what to allocate and where. Note, it is always safest to free things in the
	// reverse order they have been allocated, because of interdependency reasons (for example,
	// the map topology hash table relies on the map walk faces array).

	// for each latitude/longitude sector in the topological hash table, see if we need to free something there
	for (unsigned char i (0u), j; i < MAX_MAP_PARALLELS; ++i)
		for (j = 0u; j < MAX_MAP_MERIDIANS; ++j)
		{
			// do we need to free this slot in the map's topological hash table?
			m_topology[i][j].RemoveAll ();	// don't forget to free the hash table data...
		}

	// do we need to free the map's walkable faces database? (probably)
	{
		// for each walkable face slot in the array...
		for (unsigned short index (0u); index < m_walkFaces.GetElementNumber (); ++index)
			delete [] m_walkFaces[index].corners;	// free the walkable face corners

		m_walkFaces.RemoveAll ();
	}

	m_windings.DeleteAndPurgeAll ();
}

const Math::Vector3D WorldMap::DropToFloor (const Math::Vector3D &location) const
{
	// this function takes the vector specified as the parameter (location) and traces straight
	// down to find a location along the same Z axis that reflects the ground height. It returns
	// then a "dropped to floor" version of the original vector. One TraceLine is involved.

	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	// trace down from location and return a vector at ground height
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (location, location - Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::MapSize), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);

	if (traceResult.fraction < 1.0f)
		return traceResult.endPosition;	// ground was found, return a lowered vector

	// aargh, ground was not found!
	return Math::Vector3D::ZeroValue;
}

void WorldMap::DrawSector (const Human *const client, const Sector_t &sector, const Color <> &color, const unsigned char duration) const
{
	// this function is a higher level wrapper for DrawLine() which purpose is to simplify
	// the computation of the boundaries of the map topologic sector to which belongs the spatial
	// location 'origin'. It draws a line visible from the client side of the player whose player
	// entity is pointed to by 'client', around the perimeter of the sector to which 'origin'
	// belongs, supposed to last life tenths seconds, and having the color defined by RGB.

	// Reliability check.
	if (!client->IsValid ())
		return;

	// we have to determine the position in the topological array where sector is located

	const Math::Vector2D temp (m_bspFile.models[0u].boundingBox.GetSizeX () / m_parallelsCount, m_bspFile.models[0u].boundingBox.GetSizeY () / m_meridiansCount);

	// loop through array, stop when bucket found (could be faster...)
	for (unsigned char i (0u), j; i < m_parallelsCount; ++i)
		for (j = 0u; j < m_meridiansCount; ++j)
			if (m_topology[i] + j == &sector)
			{
				// now compute the left, right, top and bottom coordinates indices of the sector
				const float sectorLeft (m_bspFile.models[0u].boundingBox.mins.x + temp.x * i);
				const float sectorRight (m_bspFile.models[0u].boundingBox.mins.x + temp.x * (i + 1u));
				const float sectorBottom (m_bspFile.models[0u].boundingBox.mins.y + temp.y * j);
				const float sectorTop (m_bspFile.models[0u].boundingBox.mins.y + temp.y * (j + 1u));

				// and draw the perimeter around the sector (hopefully the player sees it)
				client->DrawLine (Math::Vector3D (sectorLeft, sectorTop, client->GetOrigin ().z + 1.0f), Math::Vector3D (sectorLeft, sectorBottom, client->GetOrigin ().z + 1.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, color, 255u, 255u, duration);
				client->DrawLine (Math::Vector3D (sectorLeft, sectorBottom, client->GetOrigin ().z + 1.0f), Math::Vector3D (sectorRight, sectorBottom, client->GetOrigin ().z + 1.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, color, 255u, 255u, duration);
				client->DrawLine (Math::Vector3D (sectorRight, sectorBottom, client->GetOrigin ().z + 1.0f), Math::Vector3D (sectorRight, sectorTop, client->GetOrigin ().z + 1.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, color, 255u, 255u, duration);
				client->DrawLine (Math::Vector3D (sectorRight, sectorTop, client->GetOrigin ().z + 1.0f), Math::Vector3D (sectorLeft, sectorTop, client->GetOrigin ().z + 1.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, color, 255u, 255u, duration);

				// finished
				return;
			}

	// sector not found in topological array?? WTF???
	AddLogEntry (false, LogLevel_Critical, false, "DrawSector(): function called for unknown sector!");
}

void WorldMap::DrawWalkFace (const Human *const client, const WalkFace_t &face, const Color <> &color, const unsigned char duration) const
{
	// this function is a higher level wrapper for DrawLine() which purpose is to simplify
	// the computation of the boundaries of the face pointed to by face. It draws lines visible
	// from the client side of the player whose player entity is pointed to by 'client', around the
	// perimeter of the face pointed to by face, which is supposed to last life tenths seconds,
	// and having the color defined by RGB.

	// Reliability check.
	InternalAssert (client->IsValid ());

	Math::Vector3D bound1, bound2;

	// draw the perimeter around the face
	for (unsigned short cornerIndex (0u); cornerIndex < face.cornerCount; ++cornerIndex)
	{
		// locate the first vertice of this corner and raise it 2 units up for better visibility
		bound1 = face.corners[cornerIndex];

		bound1.z += 2.0f;

		// locate the second vertice of this corner and raise it 2 units up for better visibility
		if (cornerIndex < face.cornerCount - 1u)
			bound2 = face.corners[cornerIndex + 1u];	// next corner in the array
		else
			bound2 = face.corners[0u];	// loop back to corner zero at last corner

		bound2.z += 2.0f;

		// draw a line between these 2 points
		client->DrawLine (bound1, bound2, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, color, 255u, 255u, duration);
	}
}

void WorldMap::DrawNavigationLink (const Human *const client, const NavigationLink_t *const link, const unsigned char duration) const
{
	// this function is a higher level wrapper for DrawLine() which purpose is to simplify
	// the drawing of the navigation link pointed to by link, represented under the form of a
	// glowing vertical bar, and being 30 units high. This effect is only visible from the client
	// side of the player whose player entity is pointed to by 'client', during life tenths seconds,
	// and having a color defined by the type of reachability this link has.

	// Reliability check.
	if (!client->IsValid ())
		return;

	#if defined _DEBUG
		// Reliability check.
		if (link == NULL)
		{
			AddLogEntry (false, LogLevel_Critical, false, "DrawNavigationLink(): called with null 'link' pointer!");

			return;
		}
	#endif	// if defined _DEBUG

	// draw the navigation link
	client->DrawLine
	(
		link->origin + Math::Vector3D (0.0f, 0.0f, -15.0f),
		link->origin + Math::Vector3D (0.0f, 0.0f, 15.0f),
		HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser),
		10u,
		0u,

		// set the navigation link color according to its reachability
		(link->reachability & NavigationLink_t::REACHABILITY_FALLEDGE) ?
			Color <> (0u, 0u, 255u) :	// BLUE color for falledge reachability
		(link->reachability & NavigationLink_t::REACHABILITY_LADDER) ?
			Color <> (0u, 255u, 0u) :	// GREEN color for ladder reachability
		(link->reachability & NavigationLink_t::REACHABILITY_ELEVATOR) ?
			Color <> (0u, 255u, 255u) :	// CYAN color for elevator reachability
		(link->reachability & NavigationLink_t::REACHABILITY_PLATFORM) ?
			Color <> (255u, 0u, 0u) :	// RED color for platform reachability
		(link->reachability & NavigationLink_t::REACHABILITY_CONVEYOR) ?
			Color <> (255u, 0u, 255u) :	// PURPLE color for conveyor reachability
		(link->reachability & NavigationLink_t::REACHABILITY_TRAIN) ?
			Color <> (255u, 255u, 0u) :	// YELLOW color for train reachability
		Color <> (255u, 255u, 255u),	// WHITE color for normal reachability

		255u,
		255u,
		duration
	);
}

void WorldMap::WriteDebugBitmap (void) const
{
	// this function allocates memory and clears the debug bitmap buffer.

	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Drawing global mesh (%u walk faces) to bitmap file...\n", m_walkFaces.GetElementNumber ());

	BMPFileCreator BMPFile (m_bspFile.models[0u].boundingBox);

	// cycle through all walk faces in the navmesh...
	for (unsigned short index (0u), cornerIndex; index < m_walkFaces.GetElementNumber (); ++index)
	{
		// this function necessary so as to draw each of the edges of the walk face pointed to by walk face, using the palette color specified by color.

		const WalkFace_t &walkFace (m_walkFaces[index]);

		// for each walk face, cycle through all corners...
		for (cornerIndex = 0u; cornerIndex < walkFace.cornerCount; ++cornerIndex)
		{
			// get the start corner, take the next one for the end corner, on last corner, loop back to corner #0
			BMPFile.DrawLine (walkFace.corners[cornerIndex], walkFace.corners[cornerIndex < walkFace.cornerCount - 1u ? cornerIndex + 1u : 0u]);	// and draw them in the bitmap
		}
	}

	const DynamicString filename (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + "-MESH.BMP");

	// and then draw it
	if (BMPFile.Write (filename))
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("BMP file created: '%s'.\n", filename.GetData ());

	// finished, walkface has been printed into the BMP dot matrix
}

// find the contents of a coordinate in 3D space...
inline const HalfLifeEngine::SDK::Constants::PointContentType_t PointContents (const Math::Vector3D &point)
{
	// return contents (HalfLifeEngine::SDK::Constants::Content_Empty, HalfLifeEngine::SDK::Constants::Content_Solid, HalfLifeEngine::SDK::Constants::Content_Water, etc.)
	return g_server->GetNavigationMeshManager ().GetWorldMap ().GetHullPointContents (0, point);
}

// trace a line from start to end, fill in HalfLifeEngine::SDK::Structures::TraceResult_t structure with result...
void TraceLine (const Math::Vector3D &start, Math::Vector3D end, HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult)
{
	HalfLifeEngine::SDK::Constants::PointContentType_t startContent, endContent;
	unsigned short numsteps, totalsteps;
	Math::Vector3D move, step, position;
	float dist, trace_dist;

	memset (&traceResult, 0, sizeof (traceResult));

	if
	(
		start.x < -4095.0f || start.x > 4095.0f ||
		start.y < -4095.0f || start.y > 4095.0f ||
		start.z < -4095.0f || start.z > 4095.0f
	)
	{
		// start beyond edge of world is INVALID!!!
		AddLogEntry (false, LogLevel_Critical, true, "TraceLine: start point beyond edge of world!");
	}

	if (end.x > 4095.0f)
	{
		const float percent (4095.0f / end.x);
		end.x = 4095.0f;
		end.y *= percent;
		end.z *= percent;
	}

	if (end.y > 4095.0f)
	{
		const float percent (4095.0f / end.y);
		end.y = 4095.0f;
		end.x *= percent;
		end.z *= percent;
	}

	if (end.z > 4095.0f)
	{
		const float percent (4095.0f / end.z);
		end.x *= percent;
		end.y *= percent;
		end.z = 4095.0f;
	}

	if (end.x < -4095.0f)
	{
		const float percent (4095.0f / end.x);
		end.x = -4095.0f;
		end.y *= percent;
		end.z *= percent;
	}

	if (end.y < -4095.0f)
	{
		const float percent (4095.0f / end.y);
		end.y = -4095.0f;
		end.x *= percent;
		end.z *= percent;
	}

	if (end.z < -4095.0f)
	{
		const float percent (4095.0f / end.z);
		end.z = -4095.0f;
		end.x *= percent;
		end.y *= percent;
	}

	// find the starting and ending leafs...
	startContent = PointContents (start);
	endContent = PointContents (end);

	// set endPosition, fraction and contents to the default (trace completed)
	traceResult.endPosition = end;
	traceResult.fraction = 1.0f;
//	traceResult.contents = endContent;

	if (startContent == HalfLifeEngine::SDK::Constants::Content_Solid)
	{
		traceResult.isStartSolid = true;

		if (endContent == HalfLifeEngine::SDK::Constants::Content_Solid)
			traceResult.isAllSolid = true;
	}
/*
	// is start and end leaf the same (couldn't possibly hit the world)...
	if (startContent == endContent)
		return;
*/
	// get the length of each interation of the loop...
	move = end - start;
	dist = move.GetLength ();

	// determine the number of steps from start to end...
	if (dist > 1.0f)
		numsteps = totalsteps = static_cast <unsigned short> (dist) + 1u;
	else
		numsteps = totalsteps = 1u;

	// calculate the length of the step vector...
	step = move * 2.0f / numsteps;

	position = start;

	do
	{
		position += step;

		endContent = PointContents (position);

		if (endContent == HalfLifeEngine::SDK::Constants::Content_Empty)	// we hit something...
//		if (endContent != HalfLifeEngine::SDK::Constants::Content_Solid &&  // we hit something solid...
//			endContent != HalfLifeEngine::SDK::Constants::Content_Sky)      // we hit the sky
			continue;

		// store the hit position
//		traceResult.hitpos = position;

		// back off one step before solid
		position -= step;

		// store the end position and end position contents
		traceResult.endPosition = position;
//		traceResult.contents = endContent;

		move = position - start;
		trace_dist = move.GetLength ();
		traceResult.fraction = trace_dist / dist;

		break;  // break out of while loop
	} while (--numsteps > 0u);
}

const bool PlayerRecursiveHullCheck
(
	const short firstClipNodeIndex,
	const short clipNodeNum,			// Clip node to analyse from
	const float sourceFraction, const float destinationFraction,	// Initialized to 0.0 and 1.0
	const Math::Vector3D &source, const Math::Vector3D &destination,	// source = start, destination = end
	HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult
)
{
/*	#if defined _DEBUG
		if (firstClipNodeIndex < HalfLifeEngine::SDK::Constants::Hull_Human || firstClipNodeIndex > HalfLifeEngine::SDK::Constants::Hull_Head)
			AddLogEntry (false, LogLevel_Critical, true, "Wrong 'firstClipNodeIndex' in PlayerRecursiveHullCheck()!");
	#endif	// if defined _DEBUG
*/
	// check for empty
	// if < 0, we are in a content node
	if (clipNodeNum < 0)
	{
		if (clipNodeNum == HalfLifeEngine::SDK::Constants::Content_Solid)
			traceResult.isStartSolid = true;
		else
		{
			traceResult.isAllSolid = false;

			if (clipNodeNum == HalfLifeEngine::SDK::Constants::Content_Empty)
				traceResult.isInOpen = true;
			else
				traceResult.isInWater = true;
		}

		// empty
		return true;
	}
/*! NEEDS TO BE INVOLVED!!!!!!!!!!!!!!!!
	if (hull->firstClipNodeIndex >= hull->lastClipNodeIndex)
	{
		traceResult.isAllSolid = false;
		traceResult.isInOpen = true;

		return true;
	}
*/
	// this is a clip node
	#if defined _DEBUG
		if (clipNodeNum >= g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes.GetElementNumber ())
			AddLogEntry (false, LogLevel_Critical, true, "PlayerRecursiveHullCheck(): Bad clip node number!");
	#endif	// if defined _DEBUG

	// find the point distances to the seperating plane
	const HalfLifeEngine::SDK::Structures::DClipNode_t &clipNode (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes[static_cast <unsigned short> (clipNodeNum)]);

	#if defined _DEBUG
		if (clipNode.planeNum < 0 || clipNode.planeNum >= g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes.GetElementNumber ())
			AddLogEntry (false, LogLevel_Critical, true, "PlayerRecursiveHullCheck(): Bad plane number!");
	#endif	// if defined _DEBUG

	const HalfLifeEngine::SDK::Structures::DPlane_t &plane (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes[static_cast <unsigned short> (clipNode.planeNum)]);
	float sourceDistance, destinationDistance;

	// adjust/find the plane distances
	if (plane.type <= HalfLifeEngine::SDK::Structures::DPlane_t::Type_Z)
	{
		// easier for axial planes
		sourceDistance = source[plane.type] - plane.distance;
		destinationDistance = destination[plane.type] - plane.distance;
	}
	else
	{
		// f(x,y,z) = Ax+Ay+Az-D
		sourceDistance = (plane.normal | source) - plane.distance;
		destinationDistance = (plane.normal | destination) - plane.distance;
	}

	// Check whether 'source' & 'destination' are on the same side of the plane
	// Then, continue to check in this smaller space
	// See which sides we need to consider
	if (sourceDistance >= 0.0f && destinationDistance >= 0.0f)	// both points are in front of the plane
		return PlayerRecursiveHullCheck (firstClipNodeIndex, clipNode.children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Front], sourceFraction, destinationFraction, source, destination, traceResult);	// so check the front child
/*
	// the line spans the splitting plane
	#if defined _DEBUG
		// 'source' & 'destination' are on opposite side of the plane...
		if (sourceDistance == destinationDistance)
			AddLogEntry (false, LogLevel_Critical, true, "PlayerRecursiveHullCheck(): Same distance from plane!");
	#endif	// if defined _DEBUG
*/
	// 1/32 epsilon to keep floating point happy
	#define	DIST_EPSILON 0.03125f

	// put the crosspoint DIST_EPSILON pixels on the near side
	float fraction;
	Math::Plane::PointSide_t side;

	// Split the segment into two
	if (sourceDistance < 0.0f)
	{
		if (destinationDistance < 0.0f)	// both points are behind the plane
			return PlayerRecursiveHullCheck (firstClipNodeIndex, clipNode.children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Back], sourceFraction, destinationFraction, source, destination, traceResult);	// so check the back child

		fraction = Math::GetClampedValueInRange ((sourceDistance + DIST_EPSILON) / (sourceDistance - destinationDistance), 0.0f, 1.0f);	// (make sure the fraction is valid)
		side = Math::Plane::PointSide_Back;	// back
	}
	else
	{
		fraction = Math::GetClampedValueInRange ((sourceDistance - DIST_EPSILON) / (sourceDistance - destinationDistance), 0.0f, 1.0f);	// (make sure the fraction is valid)
		side = Math::Plane::PointSide_Front;	// front
	}

	// Calculate the middle points
	float middleFraction (Math::GetInterpolated (sourceFraction, destinationFraction, fraction));
	Math::Vector3D middle (source.GetInterpolated (destination, fraction));

	// Check the first side
	if (!PlayerRecursiveHullCheck (firstClipNodeIndex, clipNode.children[side], sourceFraction, middleFraction, source, middle, traceResult))	// move up to the clip node
		return false;

	// Check the second side
	if (g_server->GetNavigationMeshManager ().GetWorldMap ().GetHullPointContents (clipNode.children[side ^ 1u], middle) != HalfLifeEngine::SDK::Constants::Content_Solid)
		return PlayerRecursiveHullCheck (firstClipNodeIndex, clipNode.children[side ^ 1u], middleFraction, destinationFraction, middle, destination, traceResult);	// go past the clip node

	if (traceResult.isAllSolid)
		return false;	// never got out of the solid area

	// the other side of the clip node is solid, this is the impact point
	if (side == Math::Plane::PointSide_Front)
	{
		traceResult.planeNormal = plane.normal;
		traceResult.planeDistance = plane.distance;
	}
	else
	{
		traceResult.planeNormal = -plane.normal;
		traceResult.planeDistance = -plane.distance;
	}

	// If middle is SOLID, move middle backwards until it is in EMPTY space
	while (g_server->GetNavigationMeshManager ().GetWorldMap ().GetHullPointContents (firstClipNodeIndex, middle) == HalfLifeEngine::SDK::Constants::Content_Solid)
	{
		fraction -= 0.05f;

		// shouldn't really happen, but does occasionally
		if (fraction < 0.0f)
		{
/*			#if defined _DEBUG
				AddLogEntry (true, LogLevel_Warning, true, "PlayerRecursiveHullCheck(): Trace backed up past 0.0.");
			#endif	// if defined _DEBUG
*/
			break;	// Exit loop immediately!
		}

		// Adjust middle to reflect the value of fraction
		middleFraction = Math::GetInterpolated (sourceFraction, destinationFraction, fraction);
		middle = source.GetInterpolated (destination, fraction);
	}

	traceResult.fraction = middleFraction;
	traceResult.endPosition = middle;
//	traceResult.hitGroup = clipNode.planeNum;	/// FOR DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	return false;
}

const bool RecursiveHullCheck
(
	const short firstClipNodeIndex,
	const short clipNodeNum,	// Clip node to analyse from
	const float sourceFraction, const float destinationFraction,	// Initialized to 0.0 and 1.0
	const Math::Vector3D &source, const Math::Vector3D &destination,	// source = start, destination = end
	HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult
)
{
/*	#if defined _DEBUG
		if (firstClipNodeIndex < HalfLifeEngine::SDK::Constants::Hull_Human || firstClipNodeIndex > HalfLifeEngine::SDK::Constants::Hull_Head)
			AddLogEntry (false, LogLevel_Critical, true, "Wrong 'firstClipNodeIndex' in RecursiveHullCheck()!");
	#endif	// if defined _DEBUG
*/
	// check for empty
	// if < 0, we are in a content node
	if (clipNodeNum < 0)
	{
		if (clipNodeNum == HalfLifeEngine::SDK::Constants::Content_Solid)
			traceResult.isStartSolid = true;
		else
		{
			traceResult.isAllSolid = false;

			if (clipNodeNum == HalfLifeEngine::SDK::Constants::Content_Empty)
				traceResult.isInOpen = true;
			else if (clipNodeNum != HalfLifeEngine::SDK::Constants::Content_Translucent)
				traceResult.isInWater = true;
		}

		// empty
		return true;
	}

	// this is a clip node
	#if defined _DEBUG
		if (clipNodeNum >= g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes.GetElementNumber ())
			AddLogEntry (false, LogLevel_Critical, true, "RecursiveHullCheck(): Bad clip node number!");
	#endif	// if defined _DEBUG

	// find the point distances to the seperating plane
	const HalfLifeEngine::SDK::Structures::DClipNode_t &clipNode (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes[static_cast <unsigned short> (clipNodeNum)]);

	#if defined _DEBUG
		if (clipNode.planeNum < 0 || clipNode.planeNum >= g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes.GetElementNumber ())
			AddLogEntry (false, LogLevel_Critical, true, "RecursiveHullCheck(): Bad plane number!");
	#endif	// if defined _DEBUG

	const HalfLifeEngine::SDK::Structures::DPlane_t &plane (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes[static_cast <unsigned short> (clipNode.planeNum)]);
	float sourceDistance, destinationDistance;

	// adjust/find the plane distances
	if (plane.type <= HalfLifeEngine::SDK::Structures::DPlane_t::Type_Z)
	{
		// easier for axial planes
		sourceDistance = source[plane.type] - plane.distance;
		destinationDistance = destination[plane.type] - plane.distance;
	}
	else
	{
		// f(x,y,z) = Ax+Ay+Az-D
		sourceDistance = (plane.normal | source) - plane.distance;
		destinationDistance = (plane.normal | destination) - plane.distance;
	}
#if 0
	const Math::Vector3D offset
	(
		plane.normal.x >= 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].mins.x : HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].maxs.x,
		plane.normal.y >= 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].mins.y : HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].maxs.y,
		plane.normal.z >= 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].mins.z : HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].maxs.z
	);
	const float newPlaneDistanceSource (offset | plane.normal);
	const float newPlaneDistanceDestination (-newPlaneDistanceSource);

	if (sourceDistance < 0.0f)
		sourceDistance += newPlaneDistanceSource;
	else
		sourceDistance -= newPlaneDistanceSource;

	if (destinationDistance < 0.0f)
		destinationDistance += newPlaneDistanceDestination;
	else
		destinationDistance -= newPlaneDistanceDestination;
#endif	// if 0
	// Check whether 'source' & 'destination' are on the same side of the plane
	// Then, continue to check in this smaller space
	// See which sides we need to consider
	if (sourceDistance >= 0.0f && destinationDistance >= 0.0f)	// both points are in front of the plane
		return RecursiveHullCheck (firstClipNodeIndex, clipNode.children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Front], sourceFraction, destinationFraction, source, destination, traceResult);	// so check the front child
/*
	// the line spans the splitting plane
	#if defined _DEBUG
		// 'source' & 'destination' are on opposite side of the plane...
		if (sourceDistance == destinationDistance)
			AddLogEntry (false, LogLevel_Critical, true, "RecursiveHullCheck(): Same distance from plane!");
	#endif	// if defined _DEBUG
*/
	// 1/32 epsilon to keep floating point happy
	#define	DIST_EPSILON 0.03125f

	// put the crosspoint DIST_EPSILON pixels on the near side
	float fraction;
	Math::Plane::PointSide_t side;

	// Split the segment into two
	if (sourceDistance < 0.0f)
	{
		if (destinationDistance < 0.0f)	// both points are behind the plane
			return RecursiveHullCheck (firstClipNodeIndex, clipNode.children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Back], sourceFraction, destinationFraction, source, destination, traceResult);	// so check the back child

		fraction = Math::GetClampedValueInRange ((sourceDistance + DIST_EPSILON) / (sourceDistance - destinationDistance), 0.0f, 1.0f);	// (make sure the fraction is valid)
		side = Math::Plane::PointSide_Back;	// back
	}
	else
	{
		fraction = Math::GetClampedValueInRange ((sourceDistance - DIST_EPSILON) / (sourceDistance - destinationDistance), 0.0f, 1.0f);	// (make sure the fraction is valid)
		side = Math::Plane::PointSide_Front;	// front
	}

	// Calculate the middle points
	float middleFraction (Math::GetInterpolated (sourceFraction, destinationFraction, fraction));
	Math::Vector3D middle (source.GetInterpolated (destination, fraction));

	// Check the first side
	if (!RecursiveHullCheck (firstClipNodeIndex, clipNode.children[side], sourceFraction, middleFraction, source, middle, traceResult))	// move up to the clip node
		return false;

	// Check the second side
	if (g_server->GetNavigationMeshManager ().GetWorldMap ().GetHullPointContents (clipNode.children[side ^ 1u], middle) != HalfLifeEngine::SDK::Constants::Content_Solid)
		return RecursiveHullCheck (firstClipNodeIndex, clipNode.children[side ^ 1u], middleFraction, destinationFraction, middle, destination, traceResult);	// go past the clip node

	if (traceResult.isAllSolid)
		return false;	// never got out of the solid area

	// the other side of the clip node is solid, this is the impact point
	if (side == Math::Plane::PointSide_Front)
	{
		traceResult.planeNormal = plane.normal;
		traceResult.planeDistance = plane.distance;
	}
	else
	{
		traceResult.planeNormal = -plane.normal;
		traceResult.planeDistance = -plane.distance;
	}

	// If middle is SOLID, move middle backwards until it is in EMPTY space
	while (g_server->GetNavigationMeshManager ().GetWorldMap ().GetHullPointContents (firstClipNodeIndex, middle) == HalfLifeEngine::SDK::Constants::Content_Solid)
	{
		fraction -= Math::RawEpsilon;

		// shouldn't really happen, but does occasionally
		if (fraction < 0.0f)
		{
/*			#if defined _DEBUG
				AddLogEntry (true, LogLevel_Warning, true, "RecursiveHullCheck(): Trace backed up past 0.0.");
			#endif	// if defined _DEBUG
*/
			break;	// Exit loop immediately!
		}

		// Adjust middle to reflect the value of fraction
		middleFraction = Math::GetInterpolated (sourceFraction, destinationFraction, fraction);
		middle = source.GetInterpolated (destination, fraction);
	}

	traceResult.fraction = middleFraction;
	traceResult.endPosition = middle;
	traceResult.hitGroup = static_cast <HalfLifeEngine::SDK::Constants::PlayerBodyPart_t> (clipNodeNum/*clipNode.planeNum*/);	/// FOR DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Hit plane real normal=[%.2f, %.2f, %.2f], distance=%.2f.", plane.normal.x, plane.normal.y, plane.normal.z, plane.distance);

	return false;
}
#if 0
namespace NEW
{
/*
===============================================================================

HULL BOXES

===============================================================================
*/
#define	SIDE_FRONT	0
#define	SIDE_BACK	1
#define	SIDE_ON		2

static	HalfLifeEngine::SDK::Structures::Hull_t		box_hull;
static	HalfLifeEngine::SDK::Structures::DClipNode_t	box_clipnodes[6];
static	HalfLifeEngine::SDK::Structures::MPlane_t	box_planes[6];

/*
===================
SV_InitBoxHull

Set up the planes and clipNodes so that the six floats of a bounding box
can just be stored out and get a proper HalfLifeEngine::SDK::Structures::Hull_t structure.
===================
*/
void SV_InitBoxHull (void)
{
	int		i;
	int		side;

	box_hull.clipNodes = box_clipnodes;
	box_hull.planes = box_planes;
	box_hull.firstClipNodeIndex = 0;
	box_hull.lastClipNodeIndex = 5;

	for (i = 0u; i < 6u; ++i)
	{
		box_clipnodes[i].planeNum = i;

		side = i&1;

		box_clipnodes[i].children[side] = HalfLifeEngine::SDK::Constants::Content_Empty;
		if (i != 5)
			box_clipnodes[i].children[side^1] = i + 1;
		else
			box_clipnodes[i].children[side^1] = HalfLifeEngine::SDK::Constants::Content_Solid;

		box_planes[i].type = i >> 1;
		box_planes[i].normal[i >> 1] = 1;
	}
}


/*
===================
SV_HullForBox

To keep everything totally uniform, bounding boxes are turned into small
BSP trees instead of being compared directly.
===================
*/
HalfLifeEngine::SDK::Structures::Hull_t	*SV_HullForBox (const Math::Vector3D &mins, const Math::Vector3D &maxs)
{
	box_planes[0].distance = maxs.x;
	box_planes[1].distance = mins.x;
	box_planes[2].distance = maxs.y;
	box_planes[3].distance = mins.y;
	box_planes[4].distance = maxs.z;
	box_planes[5].distance = mins.z;

	return &box_hull;
}
int GetHullPointContents (const HalfLifeEngine::SDK::Structures::Hull_t *hull, int num, const Math::Vector3D &p)
{
	float		d;
	HalfLifeEngine::SDK::Structures::DClipNode_t	*node;
	HalfLifeEngine::SDK::Structures::MPlane_t	*plane;

	while (num >= 0)
	{
//		if (num < hull->firstClipNodeIndex || num > hull->lastClipNodeIndex)
//			AddLogEntry (false, LogLevel_Critical, true, "SV_HullPointContents: bad node number");

		node = hull->clipNodes + num;
		plane = hull->planes + node->planeNum;

		if (plane->type < 3)
			d = p[plane->type] - plane->distance;
		else
			d = (plane->normal | p) - plane->distance;
		if (d < 0)
			num = node->children[1];
		else
			num = node->children[0];
	}

	return num;
}
const bool RecursiveHullCheck
(
	const HalfLifeEngine::SDK::Structures::Hull_t *const hull,
	const short clipNodeNum,			// Clip node to analyse from
	const float sourceFraction, const float destinationFraction,	// Initialized to 0.0 and 1.0
	const Math::Vector3D &source, const Math::Vector3D &destination,	// source = start, destination = end
	HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult
)
{
/*	#if defined _DEBUG
		if (clipNodeNum < hull->firstClipNode || clipNodeNum > hull->lastClipNode)
			AddLogEntry (false, LogLevel_Critical, true, "Wrong 'hull' in RecursiveHullCheck()!");
	#endif	// if defined _DEBUG
*/
	// check for empty
	// if < 0, we are in a content node
	if (clipNodeNum < 0)
	{
		if (clipNodeNum == HalfLifeEngine::SDK::Constants::Content_Solid)
			traceResult.isStartSolid = true;
		else
		{
			traceResult.isAllSolid = false;

			if (clipNodeNum == HalfLifeEngine::SDK::Constants::Content_Empty)
				traceResult.isInOpen = true;
			else if (clipNodeNum != HalfLifeEngine::SDK::Constants::Content_Translucent)
				traceResult.isInWater = true;
		}

		// empty
		return true;
	}

	// this is a clip node
	#if defined _DEBUG
		if (clipNodeNum >= g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes.GetElementNumber ())
			AddLogEntry (false, LogLevel_Critical, true, "RecursiveHullCheck(): Bad clip node number!");
	#endif	// if defined _DEBUG

	// find the point distances to the seperating plane
	const HalfLifeEngine::SDK::Structures::DClipNode_t &clipNode (hull->clipNodes[clipNodeNum]);

	#if defined _DEBUG
		if (clipNode.planeNum < 0 || clipNode.planeNum >= g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes.GetElementNumber ())
			AddLogEntry (false, LogLevel_Critical, true, "RecursiveHullCheck(): Bad plane number!");
	#endif	// if defined _DEBUG

	const HalfLifeEngine::SDK::Structures::MPlane_t &plane (hull->planes[clipNode.planeNum]);
	float sourceDistance, destinationDistance;

	// adjust/find the plane distances
	if (plane.type <= HalfLifeEngine::SDK::Structures::DPlane_t::Type_Z)
	{
		// easier for axial planes
		sourceDistance = source[plane.type] - plane.distance;
		destinationDistance = destination[plane.type] - plane.distance;
	}
	else
	{
		// f(x,y,z) = Ax+Ay+Az-D
		sourceDistance = (plane.normal | source) - plane.distance;
		destinationDistance = (plane.normal | destination) - plane.distance;
	}

	// Check whether 'source' & 'destination' are on the same side of the plane
	// Then, continue to check in this smaller space
	// See which sides we need to consider
	if (sourceDistance >= 0.0f && destinationDistance >= 0.0f)	// both points are in front of the plane
		return RecursiveHullCheck (hull, clipNode.children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Front], sourceFraction, destinationFraction, source, destination, traceResult);	// so check the front child

	// the line spans the splitting plane
	#if defined _DEBUG
		// 'source' & 'destination' are on opposite side of the plane...
		if (sourceDistance == destinationDistance)
			AddLogEntry (false, LogLevel_Critical, true, "RecursiveHullCheck(): Same distance from plane!");
	#endif	// if defined _DEBUG

	// 1/32 epsilon to keep floating point happy
	#define	DIST_EPSILON 0.03125f

	// put the crosspoint DIST_EPSILON pixels on the near side
	float fraction;
	Math::Plane::PointSide_t side;

	// Split the segment into two
	if (sourceDistance < 0.0f)
	{
		if (destinationDistance < 0.0f)	// both points are behind the plane
			return RecursiveHullCheck (hull, clipNode.children[HalfLifeEngine::SDK::Structures::DClipNode_t::ChildrenType_Back], sourceFraction, destinationFraction, source, destination, traceResult);	// so check the back child

		fraction = Math::GetClampedValueInRange ((sourceDistance + DIST_EPSILON) / (sourceDistance - destinationDistance), 0.0f, 1.0f);	// (make sure the fraction is valid)
		side = Math::Plane::PointSide_Back;	// back
	}
	else
	{
		fraction = Math::GetClampedValueInRange ((sourceDistance - DIST_EPSILON) / (sourceDistance - destinationDistance), 0.0f, 1.0f);	// (make sure the fraction is valid)
		side = Math::Plane::PointSide_Front;	// front
	}

	// Calculate the middle points
	float middleFraction (Math::GetInterpolated (sourceFraction, destinationFraction, fraction));
	Math::Vector3D middle (source.GetInterpolated (destination, fraction));

	// Check the first side
	if (!RecursiveHullCheck (hull, clipNode.children[side], sourceFraction, middleFraction, source, middle, traceResult))	// move up to the clip node
		return false;

	// Check the second side
	if (GetHullPointContents (hull, clipNode.children[side ^ 1u], middle) != HalfLifeEngine::SDK::Constants::Content_Solid)
		return RecursiveHullCheck (hull, clipNode.children[side ^ 1u], middleFraction, destinationFraction, middle, destination, traceResult);	// go past the clip node

	if (traceResult.isAllSolid)
		return false;	// never got out of the solid area

	// the other side of the clip node is solid, this is the impact point
	if (side == Math::Plane::PointSide_Front)
	{
		traceResult.planeNormal = plane.normal;
		traceResult.planeDistance = plane.distance;
	}
	else
	{
		traceResult.planeNormal = -plane.normal;
		traceResult.planeDistance = -plane.distance;
	}

	// If middle is SOLID, move middle backwards until it is in EMPTY space
	while (GetHullPointContents (hull, hull->firstClipNode, middle) == HalfLifeEngine::SDK::Constants::Content_Solid)
	{
		fraction -= Math::RawEpsilon;

		// shouldn't really happen, but does occasionally
		if (fraction < 0.0f)
		{
/*			#if defined _DEBUG
				AddLogEntry (true, LogLevel_Warning, true, "RecursiveHullCheck(): Trace backed up past 0.0.");
			#endif	// if defined _DEBUG
*/
			break;	// Exit loop immediately!
		}

		// Adjust middle to reflect the value of fraction
		middleFraction = Math::GetInterpolated (sourceFraction, destinationFraction, fraction);
		middle = source.GetInterpolated (destination, fraction);
	}

	traceResult.fraction = middleFraction;
	traceResult.endPosition = middle;
//	traceResult.hitGroup = clipNode.planeNum;	/// FOR DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	return false;
}
}
#endif	// if 0
#if 0
namespace HalfLifeEngine
{
namespace Utilities
{
// world query functions

struct trace_t	/// @todo SWAP WITH 'SDK::Structures::EngineTraceResult_t'
{
/* Off=0  (0) */	BOOL                      allsolid;		// if true, plane is not valid
/* Off=4  (1) */	BOOL                      startsolid;	// if true, the initial point was in a solid area
/* Off=8  (2) */	BOOL                      inopen;
/* Off=12 (3) */	BOOL                      inwater;
/* Off=16 (4) */	float                     fraction;		// time completed, 1.0 = didn't hit anything
/* Off=20 (5) */	Math::Vector3D            endpos;		// final position
/* Off=32 (8) */	Math::Plane               plane;		// surface normal at impact
/* Off=48 (12) */	SDK::Classes::Edict      *ent;			// entity the surface is on
/* Off=52 (13) */	int                       hitgroup;
};	// sizeof (trace_t) == 56 (14)

/*

entities never clip against themselves, or their owner

line of sight checks trace->crosscontent, but bullets don't

*/

struct moveclip_t
{
/* Off=0   (0) */	Math::Vector3D         boxmins, boxmaxs;	// enclose the test object along entire move
/* Off=24  (6) */	float                 *mins, *maxs;			// size of the moving object
/* Off=32  (8) */	Math::Vector3D         mins2, maxs2;		// size when clipping against monsters
/* Off=56  (14) */	float                 *start, *end;
/* Off=64  (16) */	trace_t                trace;
/* Off=120 (30) */	unsigned short         type;
/* Off=122 (30.5) */	unsigned short         ignoreGlass;		// As boolean
/* Off=124 (31) */	SDK::Classes::Edict   *passedict;

/* Off=128 (32) */	BOOL                   isFL_MONSTERCLIP;
};	// sizeof (moveclip_t) == 132 (33)

#define	nanmask (255u << 23u)
#define	IS_NAN(x) (((*(int *)&x) &nanmask) == nanmask)

static double host_frametime (0.1f);

/*
==================
ClipVelocity

Slide off of the impacting object
returns the blocked flags (1 = floor, 2 = step / wall)
==================
*/
/*const unsigned char*/void ClipVelocity (const Math::Vector3D &in, const Math::Vector3D &normal, Math::Vector3D &out, const float overbounce)
{
	float change;
	const float backoff ((in | normal) * overbounce);	// Determine how far along plane to slide based on incoming direction. Scale by overbounce factor.

	for (unsigned char axisIndex (Math::Vector3D::AxisID_X); axisIndex < Math::Vector3D::AxisID_Total; ++axisIndex)
	{
		change = normal[axisIndex] * backoff;
		out[axisIndex] = in[axisIndex] - change;

		// If out velocity is too small, zero it out.
		if (out[axisIndex] > -SDK::Constants::StopEpsilon && out[axisIndex] < SDK::Constants::StopEpsilon)
			out[axisIndex] = 0.0f;
	}

	// return blocked flags (If the plane that is blocking us has a positive z component, then assume it's a floor ELSE if the plane has no Z, it is vertical (wall/step) ELSE assume unblocked)
//	return normal.z > 0.0f ? 1u/*floor*/ : normal.z == 0.0f ? 2u/*step*/ : 0u;
}

void SV_CheckVelocity (SDK::Classes::Edict *ent)
{
	const float sv_maxvelocity (Globals::g_halfLifeEngine->ConsoleVariable_GetValue <float> ("sv_maxvelocity"));

//
// bound velocity
//
	for (unsigned char axisIndex (Math::Vector3D::AxisID_X); axisIndex < Math::Vector3D::AxisID_Total; ++axisIndex)
	{
		if (IS_NAN(ent->variables.velocity[axisIndex]))
		{
			AddLogEntry (true, LogLevel_Warning, false, "Got a NaN velocity on %s.", ent->GetClassName ().GetData ());

			ent->variables.velocity[axisIndex] = 0.0f;
		}

		if (IS_NAN(ent->variables.origin[axisIndex]))
		{
			AddLogEntry (true, LogLevel_Warning, false, "Got a NaN origin on %s.", ent->GetClassName ().GetData ());

			ent->variables.origin[axisIndex] = 0.0f;
		}

		if (ent->variables.velocity[axisIndex] > sv_maxvelocity)
		{
			AddLogEntry (true, LogLevel_Warning, false, "Got a velocity too high on %s.", ent->GetClassName ().GetData ());

			ent->variables.velocity[axisIndex] = sv_maxvelocity;
		}
		else if (ent->variables.velocity[axisIndex] < -sv_maxvelocity)
		{
			AddLogEntry (true, LogLevel_Warning, false, "Got a velocity too low on %s.", ent->GetClassName ().GetData ());

			ent->variables.velocity[axisIndex] = -sv_maxvelocity;
		}
	}
}
void SV_AddGravity (SDK::Classes::Edict *ent)
{
	const float ent_gravity (ent->variables.gravity == 0.0f ? 1.0f : ent->variables.gravity);

	ent->variables.velocity.z -= ent_gravity * Console::Server::variableManager.GetVariable (Console::Server::VariableID_Gravity)->GetValue <float> () * host_frametime;
	ent->variables.velocity.z += ent->variables.baseVelocity.z * host_frametime;

	ent->variables.baseVelocity.z = 0.0f;

	SV_CheckVelocity (ent);
}
SDK::Structures::Hull_t *const SV_HullForBsp (const SDK::Classes::Edict *const ent, const Math::Vector3D &mins, const Math::Vector3D &maxs, Math::Vector3D &offset)
{
	SDK::Structures::Hull_t *hull;

	SDK::Structures::Model_t *const model (g_server->GetServer ().models[ent->variables.modelIndex]);

	if (model == NULL || model->type != SDK::Constants::ModelType_Brush)
		AddLogEntry (false, LogLevel_Critical, true, "Hit a %s with no model (%s)", ent->GetClassName ().GetData (), ent->GetNonPlayerModel ().GetData ());

	const float sizeX (maxs.x - mins.x);

	if (sizeX > 8.0f)
	{
		hull = model->hulls + (sizeX > 36.0f ? SDK::Constants::Hull_Large : maxs.z - mins.z > 36.0f ? SDK::Constants::Hull_Human : SDK::Constants::Hull_Head);

		// calculate an offset value to center the origin
		offset = hull->clipMins - mins;
	}
	else
	{
		hull = model->hulls + SDK::Constants::Hull_Point;

		// calculate an offset value to center the origin
		offset = model->clipMins;
	}

	// calculate an offset value to center the origin
	offset += ent->variables.origin;

	return hull;
}
const unsigned char maximumStudioHullHitGroups (128u);
extern int cache_hull_hitgroup[maximumStudioHullHitGroups];
const unsigned char maximumStudioHulls (128u);
extern SDK::Structures::Hull_t cache_hull[maximumStudioHulls];	//! @todo RECHECK INDEXES!!!
const unsigned char maximumStudioPlanes (6u);
extern SDK::Structures::MPlane_t cache_planes[maximumStudioHulls][maximumStudioPlanes];	//! @todo RECHECK INDEXES!!!
extern int nCurrentHull;
extern int nCurrentPlane;
struct StudioCache_t
{
/* Off=0  (0) */	float                     frame;
/* Off=4  (1) */	int                       sequence;
/* Off=8  (2) */	Math::Angles3D            angles;
/* Off=20 (5) */	Math::Vector3D            origin;
/* Off=32 (8) */	Math::Vector3D            size;
/* Off=44 (11) */	unsigned char             controller[4u];
/* Off=48 (12) */	unsigned char             blending[2u];
/* Off=52 (13) */	SDK::Structures::Model_t *model;
/* Off=56 (14) */	unsigned int              hullIndex;	// (cache_hull / cache_hull_hitgroup)[hullIndex]
/* Off=60 (15) */	unsigned int              planeIndex;	// cache_planes[planeIndex]
/* Off=64 (16) */	unsigned int              hitBoxNumber;	// studio_hull_hitgroup[hitBoxNumber]
};	// sizeof (StudioCache_t) == 68 (17)
const unsigned char maximumStudioCaches (16u);
extern StudioCache_t rgStudioCache[maximumStudioCaches];
extern int r_cachecurrent;
extern int studio_hull_hitgroup[maximumStudioHullHitGroups];
extern SDK::Structures::Hull_t studio_hull[maximumStudioHulls];	//! @todo RECHECK INDEXES!!!
const unsigned char maximumStudioClipNodes (6u);
extern SDK::Structures::DClipNode_t studio_clipnodes[maximumStudioClipNodes];
extern SDK::Structures::MPlane_t studio_planes[maximumStudioHulls][maximumStudioPlanes];	//! @todo RECHECK INDEXES!!!
StudioCache_t *const R_CheckStudioCache (SDK::Structures::Model_t *model, const float frame, const int sequence, const Math::Angles3D &angles, const Math::Vector3D &origin, const Math::Vector3D &size, const unsigned char controller[4u], const unsigned char blending[2u])
{
	for (unsigned char cacheIndex (0u); cacheIndex < maximumStudioCaches; ++cacheIndex)
	{
		StudioCache_t &currentCache (rgStudioCache[(r_cachecurrent - cacheIndex) & (maximumStudioCaches - 1u)]);

		if
		(
			currentCache.model == model &&
			currentCache.frame == frame &&
			currentCache.sequence == sequence &&
			currentCache.angles == angles &&
			currentCache.origin == origin &&
			currentCache.size == size &&
			memcmp (currentCache.controller, controller, sizeof (currentCache.controller)) == 0 &&
			memcmp (currentCache.blending, blending, sizeof (currentCache.blending)) == 0
		)
			return &currentCache;
	}

	return NULL;
}
void SV_SetStudioHullPlane (SDK::Structures::MPlane_t &plane, const int bone, const Math::Vector3D::AxisID_t axis, const float value)
{
	int result; // eax@1
	int v5; // eax@1
	float v6; // esi@1
	float v7; // eax@1
	float v9; // [sp+10h] [bp+Ch]@1

	plane.type = SDK::Structures::MPlane_t::Type_AnyZ;

	v5 = 4 * (axis + 12 * bone);
	LODWORD(v9) = *(int *)((char *)dword_275AB60 + v5);
	plane.normal.x = v9;
	LODWORD(v6) = *(int *)((char *)&dword_275AB70 + v5);
	LODWORD(v7) = *(int *)((char *)&dword_275AB80 + v5);
	plane.normal.x = v7;
	result = 48 * bone;
	plane.normal.x = v6;
	plane.distance = v7 * *(float *)((char *)flt_275AB8C + result)
                        + v6 * *(float *)((char *)flt_275AB7C + result)
                        + v9 * *(float *)((char *)flt_275AB6C + result)
                        + value;
}
SDK::Structures::Hull_t *R_StudioHull (SDK::Structures::Model_t *model, const float frame, const int sequence, const Math::Angles3D &angles, const Math::Vector3D &origin, const Math::Vector3D &size, const unsigned char controller[4u], const unsigned char blending[2u], unsigned short &hitBoxNumber, const SDK::Classes::Edict *const ent, const bool isCounterStrike)
{
	SV_InitStudioHull ();

	if (r_cachestudio.value != 0.0f)
	{
		StudioCache_t *const studioCache (R_CheckStudioCache (model, frame, sequence, angles, origin, size, controller, blending));

		if (studioCache != NULL)
		{
			memcpy (studio_planes, cache_planes[studioCache->planeIndex], sizeof (cache_planes[studioCache->planeIndex]) * studioCache->hitBoxNumber);
			memcpy (studio_hull, cache_hull[studioCache->hullIndex], sizeof (cache_hull[studioCache->hullIndex]) * studioCache->hitBoxNumber);
			memcpy (studio_hull_hitgroup, cache_hull_hitgroup[studioCache->hullIndex], sizeof (cache_hull_hitgroup[studioCache->hullIndex]) * studioCache->hitBoxNumber);

			hitBoxNumber = studioCache->hitBoxNumber;

			return studio_hull;
		}
	}

	Math::Angles3D tempAngles (angles);
	const SDK::Structures::StudioHeader_t *const studioHeader (ent->GetModelPointer ());

	tempAngles.pitch = -tempAngles.pitch;

	Globals::g_halfLifeEngine->GetBlendingInterface ().SV_StudioSetupBones
	(
		model,
		frame,
		sequence,
		tempAngles,
		origin,
		controller,
		blending,
		-1,
		ent
	);

	if (studioHeader->numhitboxes > 0)
	{
		const HalfLifeEngine::SDK::Structures::mstudiobbox_t *const pbboxes (reinterpret_cast <const HalfLifeEngine::SDK::Structures::mstudiobbox_t *> (reinterpret_cast <const unsigned char *> (studioHeader) + studioHeader->hitboxindex));
		unsigned short hitBoxIndex (0u);
		unsigned short studioPlaneIndex (0u);

		do
		{
			const HalfLifeEngine::SDK::Structures::mstudiobbox_t &bbox (pbboxes[hitBoxIndex]);

			if (isCounterStrike && hitBoxIndex == 21)
				continue;

			SV_SetStudioHullPlane (studio_planes[studioPlaneIndex][0u]/*! @todo RECHECK INDEXES!!! */, bbox.bone, Math::Vector3D::AxisID_X, bbox.boundingBox.maxs.x);
			SV_SetStudioHullPlane (studio_planes[studioPlaneIndex][1u]/*! @todo RECHECK INDEXES!!! */, bbox.bone, Math::Vector3D::AxisID_X, bbox.boundingBox.mins.x);
			SV_SetStudioHullPlane (studio_planes[studioPlaneIndex][2u]/*! @todo RECHECK INDEXES!!! */, bbox.bone, Math::Vector3D::AxisID_Y, bbox.boundingBox.maxs.y);
			SV_SetStudioHullPlane (studio_planes[studioPlaneIndex][3u]/*! @todo RECHECK INDEXES!!! */, bbox.bone, Math::Vector3D::AxisID_Y, bbox.boundingBox.mins.y);
			SV_SetStudioHullPlane (studio_planes[studioPlaneIndex][4u]/*! @todo RECHECK INDEXES!!! */, bbox.bone, Math::Vector3D::AxisID_Z, bbox.boundingBox.maxs.z);
			SV_SetStudioHullPlane (studio_planes[studioPlaneIndex][5u]/*! @todo RECHECK INDEXES!!! */, bbox.bone, Math::Vector3D::AxisID_Z, bbox.boundingBox.mins.z);

			/// @todo .......

			++studioPlaneIndex;
		} while (++hitBoxIndex < studioHeader->numhitboxes);
	}

	int numhitboxes (studioHeader->numhitboxes);

	if (isCounterStrike)
		--numhitboxes;

	hitBoxNumber = numhitboxes;

	if (r_cachestudio.value != 0.0f)
		R_AddToStudioCache(frame, sequence, angles, origin, size, controller, blending, model, studio_hull, numhitboxes);

	return studio_hull;
}
const int SV_HitgroupForStudioHull (const unsigned int hitBoxIndex)
{
	return studio_hull_hitgroup[hitBoxIndex];
}
SDK::Structures::Hull_t *const SV_HullForStudioModel (const SDK::Classes::Edict *const ent, const Math::Vector3D &mins, const Math::Vector3D &maxs, Math::Vector3D &offset, unsigned int &hitBoxNumber)
{
	SDK::Structures::Model_t *model;
	SDK::Structures::Hull_t *hull;
	Math::Vector3D size (maxs - mins);
	bool isClientTrace (false);
	bool isCounterStrike (false);
	float scale (0.5f);

	model = g_server->GetServer ().models[ent->variables.modelIndex];

	if (size == Math::Vector3D::ZeroValue && !(Globals::g_halfLifeEngine->GetGlobalVariables ()->traceFlags & SDK::Structures::GlobalVariables_t::FTRACE_SIMPLEBOX))
	{
		isClientTrace = true;

		if (ent->variables.flags & FL_CLIENT)
		{
			if (sv_clienttrace.value != 0.0f)
			{
				size = 1.0f;
				scale = sv_clienttrace.value * 0.5f;
			}
			else
				isClientTrace = false;
		}
	}

	if (ent->variables.gameState == 1 && (g_bIsTerrorStrike || g_bIsCStrike || g_bIsCZero))
		isCounterStrike = true;

	if (!(model->flags & 2u) && !isClientTrace)
	{
		hitBoxNumber = 1u;

		return SV_HullForEntity (ent, mins, maxs, offset);
	}

	size -= scale;

	offset = Math::Vector3D::ZeroValue;

	if (ent->variables.flags & FL_CLIENT)
	{
		Math::Angles3D angles (ent->variables.modelAngles);
		const SDK::Structures::StudioHeader_t *const studioHeader (ent->GetModelPointer ());
		const SDK::Structures::mstudioseqdesc_t *const sequenceDescription (reinterpret_cast <const SDK::Structures::mstudioseqdesc_t *> (reinterpret_cast <const unsigned char *> (studioHeader) + studioHeader->seqindex));
		int blend;

		Utilities::R_StudioPlayerBlend (sequenceDescription[ent->variables.sequence], blend, angles.pitch);

		const unsigned char controller[4u] = {127u, 127u, 127u, 127u};
		const unsigned char blending[2u] = {static_cast <unsigned char> (blend), 0u};	/// @warning I'M NOT SHURE ABOUT THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		return R_StudioHull (model, ent->variables.frame, ent->variables.sequence, angles, ent->variables.origin, size, controller, blending, hitBoxNumber, ent, isCounterStrike);
	}

	return R_StudioHull (model, ent->variables.frame, ent->variables.sequence, ent->variables.modelAngles, ent->variables.origin, size, ent->variables.controller, ent->variables.blending, hitBoxNumber, ent, isCounterStrike);
}
/*
================
SV_HullForEntity

Returns a hull that can be used for testing or clipping an object of mins/maxs size.
Offset is filled in to contain the adjustment that must be added to the
testing object's origin to get a point to use with the returned hull.
================
*/
SDK::Structures::Hull_t *SV_HullForEntity (const SDK::Classes::Edict *const ent, const Math::Vector3D &mins, const Math::Vector3D &maxs, Math::Vector3D &offset)
{
	SDK::Structures::Hull_t *hull;

// decide which clipping hull to use, based on the size
	if (ent->variables.solid == SDK::Constants::SOLID_BSP)
	{	// explicit hulls in the BSP model
		if (ent->variables.moveType != SDK::Constants::MOVETYPE_PUSH && ent->variables.moveType != SDK::Constants::MOVETYPE_PUSHSTEP)
			AddLogEntry (false, LogLevel_Critical, true, "SOLID_BSP without MOVETYPE_PUSH");

		return SV_HullForBsp (ent, mins, maxs, offset);
	}
	else
	{
		// create a temp hull from bounding box sizes
		const Math::Vector3D hullMins (ent->variables.boundingBox.mins - maxs);
		const Math::Vector3D hullMaxs (ent->variables.boundingBox.maxs - mins);

		hull = SV_HullForBox (hullMins, hullMaxs);

		offset = ent->variables.origin;
	}


	return hull;
}
/*
==================
SV_SingleClipMoveToEntity aka SV_ClipMoveToEntity

Handles selection or creation of a clipping hull, and offseting (and eventually rotation) of the end points
==================
*/
void SV_SingleClipMoveToEntity (SDK::Classes::Edict *const ent, const Math::Vector3D &start, const Math::Vector3D &mins, const Math::Vector3D &maxs, const Math::Vector3D &end, trace_t &traceResult)
{
	Math::Vector3D		offset;
	Math::Vector3D		start_l, end_l;
	SDK::Structures::Hull_t		*hull;
	bool edictIsSolidBSPAndHasModelAngles (false);
	unsigned int hitBoxNumber;

// fill in a default trace result
	memset (&traceResult, 0, sizeof (traceResult));
	traceResult.fraction = 1.0f;
	traceResult.allsolid = true;
	traceResult.endpos = end;

// get the clipping hull
	if (g_server->GetServer ().models[ent->variables.modelIndex]->type == SDK::Constants::ModelType_Studio)
		hull = SV_HullForStudioModel (ent, mins, maxs, offset, hitBoxNumber);
	else
	{
		hull = SV_HullForEntity (ent, mins, maxs, offset);

		hitBoxNumber = 1u;
	}

	start_l = start - offset;
	end_l = end - offset;

	// rotate start and end into the models frame of reference
	if (ent->variables.solid == SDK::Constants::SOLID_BSP && ent->variables.modelAngles != Math::Angles3D::ZeroValue)
	{
		Math::Vector3D temp;

		edictIsSolidBSPAndHasModelAngles = true;

		ent->variables.modelAngles.MakeVectors ();

		temp = start_l;
		start_l.x = temp | Globals::g_halfLifeEngine->GetGlobalVariables ()->forward;
		start_l.y = -(temp | Globals::g_halfLifeEngine->GetGlobalVariables ()->right);
		start_l.z = temp | Globals::g_halfLifeEngine->GetGlobalVariables ()->upward;

		temp = end_l;
		end_l.x = temp | Globals::g_halfLifeEngine->GetGlobalVariables ()->forward;
		end_l.y = -(temp | Globals::g_halfLifeEngine->GetGlobalVariables ()->right);
		end_l.z = temp | Globals::g_halfLifeEngine->GetGlobalVariables ()->upward;
	}

// trace a line through the apropriate clipping hull
	if (hitBoxNumber == 1u)
		SV_RecursiveHullCheck (hull, hull->firstClipNodeIndex, 0.0f, 1.0f, start_l, end_l, &traceResult);
	else
	{
		trace_t tempTraceResult;
		unsigned int currentHitBoxIndex (0u);

		for (unsigned int hitBoxIndex (0u); hitBoxIndex < hitBoxNumber; ++hitBoxIndex, ++hull)
		{
			// fill in a default trace result
			memset (&tempTraceResult, 0, sizeof (tempTraceResult));
			tempTraceResult.fraction = 1.0f;
			tempTraceResult.allsolid = true;
			tempTraceResult.endpos = end;

			SV_RecursiveHullCheck (hull, hull->firstClipNodeIndex, 0.0f, 1.0f, start_l, end_l, &tempTraceResult);

			if (hitBoxIndex > 0u && !tempTraceResult.allsolid && !tempTraceResult.startsolid && tempTraceResult.fraction >= traceResult.fraction)
				continue;

			if (traceResult.startsolid)
			{
				traceResult = tempTraceResult;
				traceResult.startsolid = true;
			}
			else
				traceResult = tempTraceResult;

			currentHitBoxIndex = hitBoxIndex;
		}

		traceResult.hitgroup = SV_HitgroupForStudioHull (currentHitBoxIndex);
	}

	// rotate endpos back to world frame of reference
	if (traceResult.fraction < 1.0f)
	{
		if (edictIsSolidBSPAndHasModelAngles)
		{
			ent->variables.modelAngles.MakeTransposedVectors ();

			const Math::Vector3D temp (traceResult.plane.normal);

			traceResult.plane.normal.x = temp | Globals::g_halfLifeEngine->GetGlobalVariables ()->forward;
			traceResult.plane.normal.y = temp | Globals::g_halfLifeEngine->GetGlobalVariables ()->right;
			traceResult.plane.normal.z = temp | Globals::g_halfLifeEngine->GetGlobalVariables ()->upward;
		}

		// fix trace up by the offset
		traceResult.endpos.Interpolate (start, end, traceResult.fraction);
		traceResult.ent = ent;	// did we clip the move?
	}
	else if (traceResult.startsolid)	// did we clip the move?
		traceResult.ent = ent;
}
void SV_MoveBounds (const Math::Vector3D &start, const Math::Vector3D &mins, const Math::Vector3D &maxs, const Math::Vector3D &end, Math::Vector3D &boxmins, Math::Vector3D &boxmaxs)
{
	for (unsigned char axisIndex (Math::Vector3D::AxisID_X); axisIndex < Math::Vector3D::AxisID_Total; ++axisIndex)
	{
		if (end[axisIndex] > start[axisIndex])
		{
			boxmins[axisIndex] = start[axisIndex] + mins[axisIndex] - 1.0f;
			boxmaxs[axisIndex] = end[axisIndex] + maxs[axisIndex] + 1.0f;
		}
		else
		{
			boxmins[axisIndex] = end[axisIndex] + mins[axisIndex] - 1.0f;
			boxmaxs[axisIndex] = start[axisIndex] + maxs[axisIndex] + 1.0f;
		}
	}
}
const bool IsSphereIntersectLine (const Math::Vector3D &origin, const float radius, const Math::Vector3D &start, const Math::Vector3D &direction)
{
	const Math::Vector3D delta (start - origin);
	const float dot ((delta | direction) * 2.0f);

	return MATH_GET_SQUARED (dot) - (delta.GetLengthSquared () - radius) * direction.GetLengthSquared () * 4.0f > 0.0000001f;
}
const bool SV_CheckSphereIntersection (SDK::Classes::Edict *const entity, const Math::Vector3D &start, const Math::Vector3D &end)
{
	if (!(entity->variables.flags & SDK::Constants::FL_CLIENT))
		return true;

	const SDK::Structures::StudioHeader_t *const studioHeader (Mod_Extradata (&sv.models[entity->variables.modelIndex]));
	const SDK::Structures::mstudioseqdesc_t *const seqdescs (reinterpret_cast <const SDK::Structures::mstudioseqdesc_t *> (reinterpret_cast <const unsigned char *> (studioHeader) + studioHeader->seqindex));

	return IsSphereIntersectLine (entity->variables.origin, seqdescs[entity->variables.sequence].boundingBox.GetRadius (), start, end - start);
}
/*
====================
SV_ClipMoveToEntities aka SV_ClipToLinks

Mins and maxs enclose the entire area swept by the move
====================
*/
void SV_ClipMoveToEntities (SDK::Structures::EdictAreaNode_t *const node, moveclip_t &clip)
{
	SDK::Classes::Edict *touch;
	trace_t trace;

// touch linked edicts
	for (SDK::Structures::Link_t *link (node->solidEdicts.next), *next; link != &node->solidEdicts; link = next)
	{
		next = link->next;
		touch = Utilities::GetEdictFromAreaLink (link);

		if (touch->variables.groupInfo > 0u && clip.passedict != NULL && clip.passedict->variables.groupInfo > 0u)
		{
			if
			(
				(g_groupop == SDK::Constants::GROUP_OP_NAND && (touch->variables.groupInfo & clip.passedict->variables.groupInfo)) ||
				(g_groupop == SDK::Constants::GROUP_OP_AND && !(touch->variables.groupInfo & clip.passedict->variables.groupInfo))
			)
				continue;
		}

		if (touch->variables.solid == SDK::Constants::SOLID_NOT)
			continue;	// deactivated
		if (touch == clip.passedict)
			continue;	// don't clip against the pass entity
		if (touch->variables.solid == SDK::Constants::SOLID_TRIGGER)
			AddLogEntry (false, LogLevel_Critical, true, "Trigger in clipping list");

		if (Globals::g_halfLifeEngine->GetNewGameDLLFunctionTable ().pfnShouldCollide != NULL && !(*Globals::g_halfLifeEngine->GetNewGameDLLFunctionTable ().pfnShouldCollide) (touch, clip.passedict))
			return;

		if (touch->variables.solid == SDK::Constants::SOLID_BSP)
		{
			if ((touch->variables.flags & SDK::Constants::FL_MONSTERCLIP) && !clip.isFL_MONSTERCLIP)
				continue;
		}
		else if (clip.type == SDK::Constants::TraceIgnore_Monsters && touch->variables.moveType != SDK::Constants::MOVETYPE_PUSHSTEP)
			continue;

		if (clip.ignoreGlass && touch->IsTransparent ())
			continue;

		if (!Math::IsBoundingBoxesTouching (clip.boxmins, clip.boxmaxs, touch->variables.absoluteBoundingBox.mins, touch->variables.absoluteBoundingBox.maxs))
			continue;	// not touching

		if (touch->variables.solid != SDK::Constants::SOLID_SLIDEBOX && !SV_CheckSphereIntersection (touch, clip.start, clip.end))
			continue;

		if (clip.passedict != NULL && clip.passedict->variables.size.x != 0.0f && touch->variables.size.x == 0.0f)
			continue;	// points never interact

		// might intersect, so do an exact clip
		if (clip.trace.allsolid)
			return;	// Stop if we're in allsolid

		// see if we should ignore this entity
		if (clip.passedict != NULL)
		{
		 	if (touch->variables.owner == clip.passedict)
				continue;	// don't clip against own missiles
			if (clip.passedict->variables.owner == touch)
				continue;	// don't clip against owner
		}

		if (touch->variables.flags & SDK::Constants::FL_MONSTER)
			SV_SingleClipMoveToEntity (touch, clip.start, clip.mins2, clip.maxs2, clip.end, trace);
		else
			SV_SingleClipMoveToEntity (touch, clip.start, clip.mins, clip.maxs, clip.end, trace);

		if (trace.allsolid || trace.startsolid || trace.fraction < clip.trace.fraction)
		{
			// Make sure the ray is always shorter than it currently is

			trace.ent = touch;

		 	if (clip.trace.startsolid)
			{
				clip.trace = trace;
				clip.trace.startsolid = true;
			}
			else
				clip.trace = trace;
		}
//! @warning REALLY NOT EXISTS?!?!?!?!?!?!?!??!?!?!?!??!?!?!?!?!??!?!?!?!?!??!?!?!?!?!?!?!		else if (trace.startsolid)
//			clip.trace.startsolid = true;
	}

	if (node->axis == -1)
		return;	// terminal node

// recurse down both sides
	if (clip.boxmaxs[node->axis] > node->distance)
		SV_ClipMoveToEntities (node->children[0u], clip);

	if (clip.boxmins[node->axis] < node->distance)
		SV_ClipMoveToEntities (node->children[1u], clip);
}
trace_t &SV_Move (moveclip_t &clip, const Math::Vector3D &start, const Math::Vector3D &mins, const Math::Vector3D &maxs, const Math::Vector3D &end, const unsigned short type, SDK::Classes::Edict *const passedict, const bool isFL_MONSTERCLIP)
{
// mins and maxs are reletive

// if the entire move stays in a solid volume, trace.allsolid will be set

// if the starting point is in a solid, it will be allowed to move out to an open area

// nomonsters is used for line of sight or edge testing, where mosnters shouldn't be considered solid objects

// passedict is explicitly excluded from clipping checks (normally NULL)

	memset (&clip, 0, sizeof (clip));

// clip to world
	SV_SingleClipMoveToEntity (g_server->GetServer ().edicts + SDK::Constants::WorldspawnEntitySlotIndex, start, mins, maxs, end, clip.trace);

	if (clip.trace.fraction == 0.0f)
		return clip.trace;	// blocked immediately by the world

	// Save the world collision fraction.
	const float worldFraction (clip.trace.fraction);

	// Fill in a default trace result
	clip.trace.fraction = 1.0f;
	clip.start = start;
	clip.end = end;
	clip.mins = mins;
	clip.maxs = maxs;
	clip.type = type;
	clip.ignoreGlass = (type & SDK::Constants::TraceIgnore_Glass) > 0u;//type >> 8u;
	clip.passedict = passedict;
	clip.isFL_MONSTERCLIP = isFL_MONSTERCLIP;

	if (type == SDK::Constants::TraceIgnore_Missile)
	{
		clip.mins2 = -15.0f;
		clip.maxs2 =  15.0f;
	}
	else
	{
		clip.mins2 = mins;
		clip.maxs2 = maxs;
	}

	// create the bounding box of the entire move
	// we can limit it to the part of the move not
	// already clipped off by the world, which can be
	// a significant savings for line of sight and shot traces
	SV_MoveBounds (start, clip.mins2, clip.maxs2, end, clip.boxmins, clip.boxmaxs);

	// clip to other solid entities
	SV_ClipMoveToEntities (g_server->GetEdictAreaNodes (), clip);

	// Fix up the fraction so it's appropriate given the original unclipped-to-world ray
	clip.trace.fraction *= worldFraction;

	return clip.trace;
}

/*
===============================================================================

PUSHMOVE

===============================================================================
*/

/*
============
SV_PushEntity

Does not change the entities velocity at all
============
*/
trace_t &SV_PushEntity (trace_t &trace, SDK::Classes::Edict *ent, const Math::Vector3D &push)
{
	const Math::Vector3D end (ent->variables.origin + push);

	if (ent->variables.moveType == SDK::Constants::MOVETYPE_FLYMISSILE)
		trace = SV_Move (ent->variables.origin, ent->variables.boundingBox.mins, ent->variables.boundingBox.maxs, end, SDK::Constants::TraceIgnore_Missile, ent);
	else if (ent->variables.solid == SOLID_TRIGGER || ent->variables.solid == SDK::Constants::SOLID_NOT)
	// only clip against bmodels
		trace = SV_Move (ent->variables.origin, ent->variables.boundingBox.mins, ent->variables.boundingBox.maxs, end, SDK::Constants::TraceIgnore_Monsters, ent);
	else
		trace = SV_Move (ent->variables.origin, ent->variables.boundingBox.mins, ent->variables.boundingBox.maxs, end, SDK::Constants::TraceIgnore_None, ent);

	if (trace.fraction > 0.0f)
		ent->variables.origin = trace.endpos;

//	ent->variables.origin = trace.endpos;
	SV_LinkEdict (ent, true);

	if (trace.ent)
		SV_Impact (ent, trace.ent, trace);

	return trace;
}

/*
==============================================================================

TOSS / BOUNCE

==============================================================================
*/

/*
=============
SV_CheckWaterTransition

=============
*/
void SV_CheckWaterTransition (SDK::Classes::Edict *ent)
{
	SDK::Constants::PointContentType_t cont;
	Math::Vector3D point;

	point.x = Math::GetMidPoint (ent->variables.absoluteBoundingBox.maxs.x, ent->variables.absoluteBoundingBox.mins.x);
	point.y = Math::GetMidPoint (ent->variables.absoluteBoundingBox.maxs.y, ent->variables.absoluteBoundingBox.mins.y);
	point.z = ent->variables.absoluteBoundingBox.mins.z + 1.0f;

	cont = Globals::g_halfLifeEngine->GetPointContents (point);

	if (ent->variables.waterType == SDK::Constants::PointContentType_None)
	{	// just spawned here
		ent->variables.waterType = cont;
		ent->variables.waterLevel = 1;

		return;
	}

	if (cont > SDK::Constants::Content_Water || cont <= SDK::Constants::Content_Translucent)
	{
		if (ent->variables.waterType != SDK::Constants::Content_Empty)
		{	// just crossed into water
			SV_StartSound (ent, 0, "player/pl_wade2.wav", 255, 1);
		}

		ent->variables.waterType = SDK::Constants::Content_Empty;
		ent->variables.waterLevel = 0;
	}
	else
	{
		if (ent->variables.waterType == SDK::Constants::Content_Empty)
		{	// just crossed into water
			SV_StartSound (ent, 0, "player/pl_wade1.wav", 255, 1);

			ent->variables.velocity.z *= 0.5f;
		}

		ent->variables.waterType = cont;
		ent->variables.waterLevel = 1;

		if (ent->variables.absoluteBoundingBox.mins.z == ent->variables.absoluteBoundingBox.maxs.z)
		{
			ent->variables.waterLevel = 3;

			return;
		}

		g_groupmask = ent->variables.groupInfo;

		point.z = Math::GetMidPoint (ent->variables.absoluteBoundingBox.mins.z, ent->variables.absoluteBoundingBox.maxs.z);
		cont = Globals::g_halfLifeEngine->GetPointContents (point);

		if (cont > SDK::Constants::Content_Water || cont <= SDK::Constants::Content_Translucent)
			return;

		ent->variables.waterLevel = 2;

		g_groupmask = ent->variables.groupInfo;

		point += ent->variables.viewOffset;

		cont = Globals::g_halfLifeEngine->GetPointContents (point);

		if (cont <= SDK::Constants::Content_Water && cont > SDK::Constants::Content_Translucent)
			ent->variables.waterLevel = 3;
	}
}

/*
=============
SV_CheckWater
=============
*/
bool SV_CheckWater (SDK::Classes::Edict *ent)
{
	Math::Vector3D	point;
	SDK::Constants::PointContentType_t cont;

	point.x = Math::GetMidPoint (ent->variables.absoluteBoundingBox.maxs.x, ent->variables.absoluteBoundingBox.mins.x);
	point.y = Math::GetMidPoint (ent->variables.absoluteBoundingBox.maxs.y, ent->variables.absoluteBoundingBox.mins.y);
	point.z = ent->variables.absoluteBoundingBox.mins.z + 1.0f;

	g_groupmask = ent->variables.groupInfo;

	ent->variables.waterLevel = 0;
	ent->variables.waterType = SDK::Constants::Content_Empty;
	cont = Globals::g_halfLifeEngine->GetPointContents (point);
	if (cont <= SDK::Constants::Content_Water && cont > SDK::Constants::Content_Translucent)
	{
		SDK::Constants::PointContentType_t content2;

		ent->variables.waterType = cont;
		ent->variables.waterLevel = 1;
/*		point.z = ent->variables.origin.z + Math::GetMidPoint (ent->variables.boundingBox.mins.z, ent->variables.boundingBox.maxs.z);
		cont = Globals::g_halfLifeEngine->GetPointContents (point);
		if (cont <= SDK::Constants::Content_Water)
		{
			ent->variables.waterLevel = 2;
			point.z = ent->variables.origin.z + ent->variables.viewOffset.z;
			cont = Globals::g_halfLifeEngine->GetPointContents (point);
			if (cont <= SDK::Constants::Content_Water)
				ent->variables.waterLevel = 3;
		}*/
		if (ent->variables.absoluteBoundingBox.mins.z == ent->variables.absoluteBoundingBox.maxs.z || (point.z = Math::GetMidPoint (ent->variables.absoluteBoundingBox.maxs.z, ent->variables.absoluteBoundingBox.mins.z), g_groupmask = ent->variables.groupInfo, content2 = Globals::g_halfLifeEngine->GetPointContents (point), content2 <= SDK::Constants::Content_Water) && content2 > SDK::Constants::Content_Translucent && (point += ent->variables.viewOffset, ent->variables.waterLevel = 2, g_groupmask = ent->variables.groupInfo, content2 = Globals::g_halfLifeEngine->GetPointContents (point), content2 <= SDK::Constants::Content_Water) && content2 > SDK::Constants::Content_Translucent)
			ent->variables.waterLevel = 3;

		if (cont <= SDK::Constants::Content_Current_0 && cont >= SDK::Constants::Content_Current_Down)
		{
			static const Math::Vector3D current_table[] =
			{
				{ 1.0f,  0.0f,  0.0f},
				{ 0.0f,  1.0f,  0.0f},
				{-1.0f,  0.0f,  0.0f},
				{ 0.0f, -1.0f,  0.0f},
				{ 0.0f,  0.0f,  1.0f},
				{ 0.0f,  0.0f, -1.0f}
			};

			ent->variables.baseVelocity += ent->variables.waterLevel * 50.0f * current_table[SDK::Constants::Content_Current_0 - cont];
		}
	}

	return ent->variables.waterLevel > 1;
}

/*
=============
SV_Physics_Toss

Toss, bounce, and fly movement. When onground, do nothing.
=============
*/
void SV_Physics_Toss (SDK::Classes::Edict *ent)
{
	trace_t	trace;
	Math::Vector3D	move;
	float	backoff;
/*#ifdef QUAKE2
	SDK::Classes::Edict	*groundEntity;

	groundEntity = ent->variables.groundEntity;
	if (groundEntity->variables.flags & SDK::Constants::FL_CONVEYOR)
		ent->variables.baseVelocity = groundEntity->variables.speed * groundEntity->variables.moveDirection;
	else
		ent->variables.baseVelocity = Math::Vector3D::ZeroValue;*/
	SV_CheckWater (ent);
//#endif
	// regular thinking
	if (!SV_RunThink (ent))
		return;

	/// @warning FLAGS IS WRONG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (ent->variables.velocity.z > 0.0f || ent->variables.groundEntity == NULL || (ent->variables.groundEntity->variables.flags & SDK::Constants::FL_CONVEYOR/*(SDK::Constants::FL_CLIENT | SDK::Constants::FL_MONSTER)*/))
//	if (ent->variables.velocity.z > 0.0f)
		ent->variables.flags &= ~SDK::Constants::FL_ONGROUND;

// if onground, return without moving
	if ((ent->variables.flags & SDK::Constants::FL_ONGROUND) && ent->variables.origin.AreEqual (Math::Vector3D::ZeroValue) && (ent->variables.angleVelocity = Math::Angles3D::ZeroValue, ent->variables.baseVelocity.AreEqual (Math::Vector3D::ZeroValue)))
/*	if (ent->variables.flags & SDK::Constants::FL_ONGROUND)
//@@
		if (VectorCompare(ent->variables.baseVelocity, Math::Vector3D::ZeroValue))*/
			return;

	SV_CheckVelocity (ent);

// add gravity
	if (/*!(ent->variables.flags & SDK::Constants::FL_ONGROUND)
		&& */ent->variables.moveType != SDK::Constants::MOVETYPE_FLY
		&& ent->variables.moveType != SDK::Constants::MOVETYPE_BOUNCEMISSILE
		&& ent->variables.moveType != SDK::Constants::MOVETYPE_FLYMISSILE)
			SV_AddGravity (ent);

// move model angles
	ent->variables.modelAngles += host_frametime * ent->variables.angleVelocity;

// move origin
	// Base velocity is not properly accounted for since this entity will move again after the bounce without taking it into account
	ent->variables.velocity += ent->variables.baseVelocity;

	SV_CheckVelocity (ent);

	move = host_frametime * ent->variables.velocity;
	ent->variables.velocity -= ent->variables.baseVelocity;

	SV_PushEntity (trace, ent, move);	// Should this clear base velocity

	SV_CheckVelocity (ent);

	if (trace.allsolid)
	{
		// entity is trapped in another solid
		ent->variables.velocity = Math::Vector3D::ZeroValue;
		ent->variables.angleVelocity = Math::Angles3D::ZeroValue;

		return;
	}

	if (trace.fraction == 1.0f)
	{
// check for in water
		SV_CheckWaterTransition (ent);

		return;
	}

	if (ent->free)
		return;

	if (ent->variables.moveType == SDK::Constants::MOVETYPE_BOUNCE)
		backoff = 2.0f - ent->variables.friction;
	else if (ent->variables.moveType == SDK::Constants::MOVETYPE_BOUNCEMISSILE)
		backoff = 2.0f;
	else
		backoff = 1.0f;

	ClipVelocity (ent->variables.velocity, trace.plane.normal, ent->variables.velocity, backoff);

// stop if on ground
	if (trace.plane.normal.z > 0.7f)
	{
		move = ent->variables.baseVelocity + ent->variables.velocity;

		if (move.z < Console::Server::variableManager.GetVariable (Console::Server::VariableID_Gravity)->GetValue <float> () * host_frametime)
		{
			// we're rolling on the ground, add static friction.
			ent->variables.flags |= SDK::Constants::FL_ONGROUND;
			ent->variables.groundEntity = trace.ent;
			ent->variables.velocity.z = 0.0f;
		}

		if (move.GetLengthSquared () >= MATH_GET_SQUARED (30.0f) && (ent->variables.moveType == SDK::Constants::MOVETYPE_BOUNCE || ent->variables.moveType == SDK::Constants::MOVETYPE_BOUNCEMISSILE))
		{
			const float scale ((1.0f - trace.fraction) * host_frametime * 0.9f);

			move = scale * ent->variables.velocity + scale * ent->variables.baseVelocity;

			SV_PushEntity (trace, ent, move);
		}
		else
		{
			ent->variables.flags |= SDK::Constants::FL_ONGROUND;
			ent->variables.groundEntity = trace.ent;
			ent->variables.velocity = Math::Vector3D::ZeroValue;
			ent->variables.angleVelocity = Math::Angles3D::ZeroValue;
/*
			if (ent->variables.velocity.z < 60.0f || (ent->variables.moveType != SDK::Constants::MOVETYPE_BOUNCE && ent->variables.moveType != SDK::Constants::MOVETYPE_BOUNCEMISSILE))
			{
				ent->variables.flags |= SDK::Constants::FL_ONGROUND;
				ent->variables.groundEntity = trace.ent;
				ent->variables.velocity = Math::Vector3D::ZeroValue;
				ent->variables.angleVelocity = Math::Angles3D::ZeroValue;
			}*/
		}
	}

// check for in water
	SV_CheckWaterTransition (ent);
}
/*trace_t SV_Trace_Toss (SDK::Classes::Edict *ent, SDK::Classes::Edict *ignore)
{
	SDK::Classes::Edict tempent, *tent;
	trace_t trace;
	Math::Vector3D end;
	const double save_frametime (host_frametime);

	host_frametime = 0.05;

	tempent = *ent;
	tent = &tempent;

	do
	{
//		SV_CheckVelocity (tent);
		SV_AddGravity (tent);

		tent->variables.modelAngles += tent->variables.angleVelocity * host_frametime;

		end = tent->variables.origin + tent->variables.velocity * host_frametime;

		trace = SV_Move (tent->variables.origin, tent->variables.boundingBox.mins, tent->variables.boundingBox.maxs, end, SDK::Constants::TraceIgnore_None, tent);

		tent->variables.origin = trace.endpos;
	} while (trace.ent == NULL || trace.ent == ent || trace.ent == ignore);

	host_frametime = save_frametime;

	return trace;
}*/
#endif	// if 0
namespace HalfLifeEngine
{
namespace Utilities
{
/*
==================
ClipVelocity

Slide off of the impacting object
returns the blocked flags (1 = floor, 2 = step / wall)
==================
*/
inline /*const unsigned char*/void ClipVelocity (const Math::Vector3D &in, const Math::Vector3D &normal, Math::Vector3D &out, const float overbounce)
{
	float change;
	const float backoff ((in | normal) * overbounce);	// Determine how far along plane to slide based on incoming direction. Scale by overbounce factor.

	for (unsigned char axisIndex (Math::Vector3D::AxisID_X); axisIndex < Math::Vector3D::AxisID_Total; ++axisIndex)
	{
		change = normal[axisIndex] * backoff;
		out[axisIndex] = in[axisIndex] - change;

		// If out velocity is too small, zero it out.
		if (out[axisIndex] > -SDK::Constants::StopEpsilon && out[axisIndex] < SDK::Constants::StopEpsilon)
			out[axisIndex] = 0.0f;
	}

	// return blocked flags (If the plane that is blocking us has a positive z component, then assume it's a floor ELSE if the plane has no Z, it is vertical (wall/step) ELSE assume unblocked)
//	return normal.z > 0.0f ? 1u/*floor*/ : normal.z == 0.0f ? 2u/*step*/ : 0u;
}

void TraceThrownGrenadeToss (const SDK::Classes::Edict *const grenade, PseudoGrenadeEntityVariables_t &pseudoGrenadeEntityVariables)
{
	struct PseudoGrenadeEntityVariablesExtended_t : PseudoGrenadeEntityVariables_t
	{
		Math::Vector3D                      velocity;		// current movement direction
		Math::Vector3D                      baseVelocity;	// if standing on conveyor, e.g.
		SDK::Constants::PointContentType_t  waterType;
		SDK::Classes::Edict                *groundEntity;	// if standing on a entity, the pointer to that entity
		float                               friction;		// inverse elasticity of 'MOVETYPE_BOUNCE'
		float                               nextThinkTime;	// Time next call BaseEntity::Think() function.

		unsigned char                       numberFloorHits;
	} pseudoGrenadeEntityVariablesExtended;

	pseudoGrenadeEntityVariablesExtended.origin = grenade->variables.origin;
	pseudoGrenadeEntityVariablesExtended.waterLevel = grenade->variables.waterLevel;
	pseudoGrenadeEntityVariablesExtended.flags = grenade->variables.flags;
	pseudoGrenadeEntityVariablesExtended.velocity = grenade->variables.velocity;
	pseudoGrenadeEntityVariablesExtended.baseVelocity = grenade->variables.baseVelocity;
	pseudoGrenadeEntityVariablesExtended.waterType = grenade->variables.waterType;
	pseudoGrenadeEntityVariablesExtended.groundEntity = grenade->variables.groundEntity;
	pseudoGrenadeEntityVariablesExtended.friction = grenade->variables.friction;
	pseudoGrenadeEntityVariablesExtended.nextThinkTime = grenade->variables.nextThinkTime;
	pseudoGrenadeEntityVariablesExtended.numberFloorHits = 0u;

	const unsigned char maximumFloorHits (10u);

	pseudoGrenadeEntityVariables = pseudoGrenadeEntityVariablesExtended;

	SDK::Structures::TraceResult_t traceResult;
	const float pseudoFrameTimeInterval (/*0.05f*/Globals::g_halfLifeEngine->GetGlobalVariables ()->frameTime);
	float pseudoTime (Globals::g_halfLifeEngine->GetTime ());
	Math::Vector3D to;

	InternalAssert (grenade->variables.gravity != 0.0f);
	InternalAssert (grenade->variables.nextThinkTime > 0.0f);

	const float gravityAdjust (grenade->variables.gravity * Console::Server::variableManager.GetVariable (Console::Server::VariableID_Gravity)->GetValue <float> () * pseudoFrameTimeInterval);
	DynamicArray <SDK::Classes::Edict *, unsigned char> touchedBreakables;
	DynamicArray <SDK::Classes::Edict *, unsigned char> touchedPushables;

	// Run pseudo frames....
	do
	{
		pseudoGrenadeEntityVariablesExtended.baseVelocity = pseudoGrenadeEntityVariablesExtended.groundEntity != NULL && (pseudoGrenadeEntityVariablesExtended.groundEntity->variables.flags & SDK::Constants::FL_CONVEYOR) ? pseudoGrenadeEntityVariablesExtended.groundEntity->variables.speed * pseudoGrenadeEntityVariablesExtended.groundEntity->variables.moveDirection : Math::Vector3D::ZeroValue;

		for (SDK::Classes::Edict *pushable (Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "trigger_push")); pushable->IsValid (); pushable = Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (pushable, "trigger_push"))
		{
			if (pushable->variables.solid == SDK::Constants::SOLID_NOT || !pseudoGrenadeEntityVariablesExtended.origin.IsInsideBoundingBox (pushable->variables.absoluteBoundingBox.mins, pushable->variables.absoluteBoundingBox.maxs))
				continue;

			// Instant trigger, just transfer velocity and remove
			if (pushable->variables.spawnFlags & SDK::Constants::SF_TRIG_PUSH_ONCE)
			{
				pseudoGrenadeEntityVariablesExtended.velocity += pushable->variables.speed * pushable->variables.moveDirection;

				if (pseudoGrenadeEntityVariablesExtended.velocity.z > 0.0f)
					pseudoGrenadeEntityVariablesExtended.flags &= ~SDK::Constants::FL_ONGROUND;

//				UTIL_Remove (this);
				{
				pushable->variables.solid = SDK::Constants::SOLID_NOT;

				touchedPushables += pushable;
				}
			}
			else
			{	// Push field, transfer to base velocity
				Math::Vector3D vecPush (pushable->variables.speed * pushable->variables.moveDirection);

				if (pseudoGrenadeEntityVariablesExtended.flags & SDK::Constants::FL_BASEVELOCITY)
					vecPush += pseudoGrenadeEntityVariablesExtended.baseVelocity;

				pseudoGrenadeEntityVariablesExtended.baseVelocity = vecPush;

				pseudoGrenadeEntityVariablesExtended.flags |= SDK::Constants::FL_BASEVELOCITY;
			}
		}

//		SV_Physics_Toss (grenade);
		{
//		SV_CheckWater (grenade);
		{
		pseudoGrenadeEntityVariablesExtended.waterLevel = 0;
		pseudoGrenadeEntityVariablesExtended.waterType = SDK::Constants::Content_Empty;
		const SDK::Constants::PointContentType_t cont (Globals::g_halfLifeEngine->GetPointContents (pseudoGrenadeEntityVariablesExtended.origin));

		if (cont <= SDK::Constants::Content_Water && cont > SDK::Constants::Content_Translucent)
		{
			pseudoGrenadeEntityVariablesExtended.waterType = cont;
			pseudoGrenadeEntityVariablesExtended.waterLevel = 3;

			if (cont <= SDK::Constants::Content_Current_0 && cont >= SDK::Constants::Content_Current_Down)
			{
				static const Math::Vector3D current_table[] =
				{
					Math::Vector3D ( 1.0f,  0.0f,  0.0f),
					Math::Vector3D ( 0.0f,  1.0f,  0.0f),
					Math::Vector3D (-1.0f,  0.0f,  0.0f),
					Math::Vector3D ( 0.0f, -1.0f,  0.0f),
					Math::Vector3D ( 0.0f,  0.0f,  1.0f),
					Math::Vector3D ( 0.0f,  0.0f, -1.0f)
				};

				pseudoGrenadeEntityVariablesExtended.baseVelocity += pseudoGrenadeEntityVariablesExtended.waterLevel * 50.0f * current_table[SDK::Constants::Content_Current_0 - cont];
			}
		}
		}

	// regular thinking
/*		if (!SV_RunThink (ent))
			continue;*/
		{
			float thinkTime (pseudoGrenadeEntityVariablesExtended.nextThinkTime);

			if (thinkTime <= pseudoTime + pseudoFrameTimeInterval)
			{
//				if (!IsInWorld (grenade->variables))
				if
				(
					// check position
					pseudoGrenadeEntityVariablesExtended.origin.x >=  SDK::Constants::HalfMapSize ||
					pseudoGrenadeEntityVariablesExtended.origin.y >=  SDK::Constants::HalfMapSize ||
					pseudoGrenadeEntityVariablesExtended.origin.z >=  SDK::Constants::HalfMapSize ||
					pseudoGrenadeEntityVariablesExtended.origin.x <= -SDK::Constants::HalfMapSize ||
					pseudoGrenadeEntityVariablesExtended.origin.y <= -SDK::Constants::HalfMapSize ||
					pseudoGrenadeEntityVariablesExtended.origin.z <= -SDK::Constants::HalfMapSize ||

					// check speed
					pseudoGrenadeEntityVariablesExtended.velocity.x >=  2000.0f ||
					pseudoGrenadeEntityVariablesExtended.velocity.y >=  2000.0f ||
					pseudoGrenadeEntityVariablesExtended.velocity.z >=  2000.0f ||
					pseudoGrenadeEntityVariablesExtended.velocity.x <= -2000.0f ||
					pseudoGrenadeEntityVariablesExtended.velocity.y <= -2000.0f ||
					pseudoGrenadeEntityVariablesExtended.velocity.z <= -2000.0f
				)
				{
					pseudoGrenadeEntityVariablesExtended.flags &= ~SDK::Constants::FL_BASEVELOCITY;
					pseudoGrenadeEntityVariablesExtended.flags |= SDK::Constants::FL_KILLME;

					pseudoGrenadeEntityVariables = pseudoGrenadeEntityVariablesExtended;

					// Restore touched breakables solid types
					for (unsigned char index (0u); index < touchedBreakables.GetElementNumber (); ++index)
						touchedBreakables[index]->variables.solid = SDK::Constants::SOLID_BSP;

					for (unsigned char index (0u); index < touchedPushables.GetElementNumber (); ++index)
						touchedPushables[index]->variables.solid = SDK::Constants::SOLID_TRIGGER;

					return;
				}

				if (thinkTime < pseudoTime)
					thinkTime = pseudoTime;	// don't let things stay in the past.
											// it is possible to start that way
											// by a trigger with a local time.

				pseudoTime = thinkTime;

				pseudoGrenadeEntityVariablesExtended.nextThinkTime = pseudoTime + 0.1f;

				if (grenade->IsGrenadeIsSmokeGrenade () && (pseudoGrenadeEntityVariablesExtended.flags & SDK::Constants::FL_ONGROUND))
					pseudoGrenadeEntityVariablesExtended.velocity *= 0.95f;

				if (pseudoGrenadeEntityVariablesExtended.waterLevel > 0u)
					pseudoGrenadeEntityVariablesExtended.velocity *= 0.5f;
			}
		}

		/// @todo NEED TO CHECK FLAGS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (pseudoGrenadeEntityVariablesExtended.velocity.z > 0.0f || pseudoGrenadeEntityVariablesExtended.groundEntity == NULL || (pseudoGrenadeEntityVariablesExtended.groundEntity->variables.flags & SDK::Constants::FL_CONVEYOR))
			pseudoGrenadeEntityVariablesExtended.flags &= ~SDK::Constants::FL_ONGROUND;

// if onground, return without moving
		if ((pseudoGrenadeEntityVariablesExtended.flags & SDK::Constants::FL_ONGROUND) && pseudoGrenadeEntityVariablesExtended.origin.AreEqual (Math::Vector3D::ZeroValue) && pseudoGrenadeEntityVariablesExtended.baseVelocity.AreEqual (Math::Vector3D::ZeroValue))
		{
			pseudoGrenadeEntityVariablesExtended.flags &= ~SDK::Constants::FL_BASEVELOCITY;

			continue;
		}

//		SV_AddGravity (grenade);
		{
			pseudoGrenadeEntityVariablesExtended.velocity.z -= gravityAdjust;
			pseudoGrenadeEntityVariablesExtended.velocity.z += pseudoGrenadeEntityVariablesExtended.baseVelocity.z * pseudoFrameTimeInterval;

			pseudoGrenadeEntityVariablesExtended.baseVelocity.z = 0.0f;
		}

// move origin
	// Base velocity is not properly accounted for since this entity will move again after the bounce without taking it into account
		to = pseudoGrenadeEntityVariablesExtended.origin + (pseudoGrenadeEntityVariablesExtended.velocity + pseudoGrenadeEntityVariablesExtended.baseVelocity) * pseudoFrameTimeInterval;

//		SV_PushEntity (traceResult, grenade, pseudoGrenadeEntityVariablesExtended.velocity * pseudoFrameTimeInterval);	// Should this clear base velocity

//		Globals::g_halfLifeEngine->TraceLine (pseudoGrenadeEntityVariablesExtended.origin, to, SDK::Constants::TraceIgnore_None, grenade->variables.owner, traceResult);
		Globals::g_halfLifeEngine->TraceHull (pseudoGrenadeEntityVariablesExtended.origin, to, false, SDK::Constants::Hull_Point, grenade->variables.owner, traceResult);	/// @note I use trace hull as this function returns NULL pointer of hitEntity when no collides, but trace line wasn't!!!

//		Globals::g_halfLifeEngine->DrawLine (pseudoGrenadeEntityVariablesExtended.origin, to, Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
/*
		if (traceResult.fraction < 1.0f)
		{
			// fix trace up by the offset
			traceResult.endPosition.Interpolate (pseudoGrenadeEntityVariablesExtended.origin, to, traceResult.fraction);
		}
*/
		if (traceResult.fraction > 0.0f)
		{
/*			const float oldFraction (traceResult.fraction);

			traceResult.fraction = 1.0f;

			traceResult.fraction *= oldFraction;
*/
			pseudoGrenadeEntityVariablesExtended.origin = traceResult.endPosition;
		}
/*
//		SV_LinkEdict (grenade, true);
		{
//		pseudoGrenadeEntityVariablesExtended.baseVelocity = pseudoGrenadeEntityVariablesExtended.groundEntity != NULL && (pseudoGrenadeEntityVariablesExtended.groundEntity->variables.flags & SDK::Constants::FL_CONVEYOR) ? pseudoGrenadeEntityVariablesExtended.groundEntity->variables.speed * pseudoGrenadeEntityVariablesExtended.groundEntity->variables.moveDirection : Math::Vector3D::ZeroValue;

		for (SDK::Classes::Edict *pushable (Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "trigger_push")); pushable->IsValid (); pushable = Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (pushable, "trigger_push"))
		{
			if (pushable->variables.solid == SDK::Constants::SOLID_NOT || !pseudoGrenadeEntityVariablesExtended.origin.IsInsideBoundingBox (pushable->variables.absoluteBoundingBox.mins, pushable->variables.absoluteBoundingBox.maxs))
				continue;

			// Instant trigger, just transfer velocity and remove
			if (pushable->variables.spawnFlags & SDK::Constants::SF_TRIG_PUSH_ONCE)
			{
				pseudoGrenadeEntityVariablesExtended.velocity += pushable->variables.speed * pushable->variables.moveDirection;

				if (pseudoGrenadeEntityVariablesExtended.velocity.z > 0.0f)
					pseudoGrenadeEntityVariablesExtended.flags &= ~SDK::Constants::FL_ONGROUND;

//				UTIL_Remove (this);
				{
				pushable->variables.solid = SDK::Constants::SOLID_NOT;

				touchedPushables += pushable;
				}
			}
			else
			{	// Push field, transfer to base velocity
				Math::Vector3D vecPush (pushable->variables.speed * pushable->variables.moveDirection);
/*
				if (pseudoGrenadeEntityVariablesExtended.flags & SDK::Constants::FL_BASEVELOCITY)
					vecPush += pseudoGrenadeEntityVariablesExtended.baseVelocity;
*//*
				pseudoGrenadeEntityVariablesExtended.baseVelocity = vecPush;

//				pseudoGrenadeEntityVariablesExtended.flags |= SDK::Constants::FL_BASEVELOCITY;
			}
		}
		}
*/
		if (traceResult.hitEntity != NULL)
		{
//			SV_Impact (grenade, traceResult.hitEntity, traceResult);
			{
//			pseudoGrenadeEntityVariablesExtended.baseVelocity = (traceResult.hitEntity->variables.flags & SDK::Constants::FL_CONVEYOR) ? traceResult.hitEntity->variables.speed * traceResult.hitEntity->variables.moveDirection : Math::Vector3D::ZeroValue;

			if (traceResult.hitEntity->GetClassName () == "func_breakable" && traceResult.hitEntity->variables.renderMode != SDK::Constants::kRenderNormal)
			{
				pseudoGrenadeEntityVariablesExtended.velocity *= -2.0f;

				traceResult.hitEntity->variables.solid = SDK::Constants::SOLID_NOT;

				touchedBreakables += traceResult.hitEntity;
			}
			else
			{
				if (pseudoGrenadeEntityVariablesExtended.flags & SDK::Constants::FL_ONGROUND)
				{
					// add a bit of static friction
					pseudoGrenadeEntityVariablesExtended.velocity *= 0.8f;
				}
				else
				{
					if (pseudoGrenadeEntityVariablesExtended.numberFloorHits >= maximumFloorHits)
					{
						pseudoGrenadeEntityVariablesExtended.groundEntity = Globals::g_halfLifeEngine->GetEdictOfIndex (0u);
						pseudoGrenadeEntityVariablesExtended.flags |= SDK::Constants::FL_ONGROUND;
						pseudoGrenadeEntityVariablesExtended.velocity = Math::Vector3D::ZeroValue;
					}

					++pseudoGrenadeEntityVariablesExtended.numberFloorHits;
				}
			}
			}
		}

		if (traceResult.isAllSolid)
		{
			// grenade is trapped in another solid
			pseudoGrenadeEntityVariablesExtended.velocity = Math::Vector3D::ZeroValue;

			pseudoGrenadeEntityVariablesExtended.flags &= ~SDK::Constants::FL_BASEVELOCITY;

			pseudoGrenadeEntityVariables = pseudoGrenadeEntityVariablesExtended;

			// Restore touched breakables solid types
			for (unsigned char index (0u); index < touchedBreakables.GetElementNumber (); ++index)
				touchedBreakables[index]->variables.solid = SDK::Constants::SOLID_BSP;

			for (unsigned char index (0u); index < touchedPushables.GetElementNumber (); ++index)
				touchedPushables[index]->variables.solid = SDK::Constants::SOLID_TRIGGER;

			return;
		}

		if (traceResult.fraction < 1.0f)
		{
			ClipVelocity (pseudoGrenadeEntityVariablesExtended.velocity, traceResult.planeNormal, pseudoGrenadeEntityVariablesExtended.velocity, 2.0f - grenade->variables.friction);

			// stop if on ground
			if (traceResult.planeNormal.z > 0.7f)
			{
				Math::Vector3D move (pseudoGrenadeEntityVariablesExtended.baseVelocity + pseudoGrenadeEntityVariablesExtended.velocity);

				if (move.z < Console::Server::variableManager.GetVariable (Console::Server::VariableID_Gravity)->GetValue <float> () * pseudoFrameTimeInterval)
				{
					// we're rolling on the ground, add static friction.
					pseudoGrenadeEntityVariablesExtended.flags |= SDK::Constants::FL_ONGROUND;
					pseudoGrenadeEntityVariablesExtended.groundEntity = traceResult.hitEntity;
					pseudoGrenadeEntityVariablesExtended.velocity.z = 0.0f;
				}

				if (move.GetLengthSquared () >= MATH_GET_SQUARED (30.0f))
				{
					const float scale ((1.0f - traceResult.fraction) * pseudoFrameTimeInterval * 0.9f);

					move = scale * pseudoGrenadeEntityVariablesExtended.velocity + scale * pseudoGrenadeEntityVariablesExtended.baseVelocity;
					to = pseudoGrenadeEntityVariablesExtended.origin + move;

//					SV_PushEntity (traceResult, grenade, move);
//					Globals::g_halfLifeEngine->TraceLine (pseudoGrenadeEntityVariablesExtended.origin, to, SDK::Constants::TraceIgnore_None, grenade->variables.owner, traceResult);
					Globals::g_halfLifeEngine->TraceHull (pseudoGrenadeEntityVariablesExtended.origin, to, false, SDK::Constants::Hull_Point, grenade->variables.owner, traceResult);	/// @note I use trace hull as this function returns NULL pointer of hitEntity when no collides, but trace line wasn't!!!

//					Globals::g_halfLifeEngine->DrawLine (pseudoGrenadeEntityVariablesExtended.origin, to, Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
/*
					if (traceResult.fraction < 1.0f)
					{
						// fix trace up by the offset
						traceResult.endPosition.Interpolate (pseudoGrenadeEntityVariablesExtended.origin, to, traceResult.fraction);
					}
*/
					if (traceResult.fraction > 0.0f)
					{
/*						const float oldFraction (traceResult.fraction);

						traceResult.fraction = 1.0f;

						traceResult.fraction *= oldFraction;
*/
						pseudoGrenadeEntityVariablesExtended.origin = traceResult.endPosition;
					}
/*
//					SV_LinkEdict (grenade, true);
					{
//					pseudoGrenadeEntityVariablesExtended.baseVelocity += pseudoGrenadeEntityVariablesExtended.groundEntity != NULL && (pseudoGrenadeEntityVariablesExtended.groundEntity->variables.flags & SDK::Constants::FL_CONVEYOR) ? pseudoGrenadeEntityVariablesExtended.groundEntity->variables.speed * pseudoGrenadeEntityVariablesExtended.groundEntity->variables.moveDirection : Math::Vector3D::ZeroValue;

					for (SDK::Classes::Edict *pushable (Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "trigger_push")); pushable->IsValid (); pushable = Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (pushable, "trigger_push"))
					{
						if (pushable->variables.solid == SDK::Constants::SOLID_NOT || !pseudoGrenadeEntityVariablesExtended.origin.IsInsideBoundingBox (pushable->variables.absoluteBoundingBox.mins, pushable->variables.absoluteBoundingBox.maxs))
							continue;

						// Instant trigger, just transfer velocity and remove
						if (pushable->variables.spawnFlags & SDK::Constants::SF_TRIG_PUSH_ONCE)
						{
							pseudoGrenadeEntityVariablesExtended.velocity += pushable->variables.speed * pushable->variables.moveDirection;

							if (pseudoGrenadeEntityVariablesExtended.velocity.z > 0.0f)
								pseudoGrenadeEntityVariablesExtended.flags &= ~SDK::Constants::FL_ONGROUND;

//							UTIL_Remove (this);
							{
							pushable->variables.solid = SDK::Constants::SOLID_NOT;

							touchedPushables += pushable;
							}
						}
						else
						{	// Push field, transfer to base velocity
							Math::Vector3D vecPush (pushable->variables.speed * pushable->variables.moveDirection);
/*
							if (pseudoGrenadeEntityVariablesExtended.flags & SDK::Constants::FL_BASEVELOCITY)
								vecPush += pseudoGrenadeEntityVariablesExtended.baseVelocity;
*//*
							pseudoGrenadeEntityVariablesExtended.baseVelocity = vecPush;

//							pseudoGrenadeEntityVariablesExtended.flags |= SDK::Constants::FL_BASEVELOCITY;
						}
					}
					}
*/
					if (traceResult.hitEntity != NULL)
					{
//						SV_Impact (grenade, traceResult.hitEntity, traceResult);
						{
/*						if (traceResult.hitEntity->variables.flags & SDK::Constants::FL_CONVEYOR)
							pseudoGrenadeEntityVariablesExtended.baseVelocity += traceResult.hitEntity->variables.speed * traceResult.hitEntity->variables.moveDirection;
*/
						if (traceResult.hitEntity->GetClassName () == "func_breakable" && traceResult.hitEntity->variables.renderMode != SDK::Constants::kRenderNormal)
						{
							pseudoGrenadeEntityVariablesExtended.velocity *= -2.0f;

							traceResult.hitEntity->variables.solid = SDK::Constants::SOLID_NOT;

							touchedBreakables += traceResult.hitEntity;
						}
						else
						{
							if (pseudoGrenadeEntityVariablesExtended.flags & SDK::Constants::FL_ONGROUND)
							{
								// add a bit of static friction
								pseudoGrenadeEntityVariablesExtended.velocity *= 0.8f;
							}
							else
							{
								if (pseudoGrenadeEntityVariablesExtended.numberFloorHits >= maximumFloorHits)
								{
									pseudoGrenadeEntityVariablesExtended.groundEntity = Globals::g_halfLifeEngine->GetEdictOfIndex (0u);
									pseudoGrenadeEntityVariablesExtended.flags |= SDK::Constants::FL_ONGROUND;
									pseudoGrenadeEntityVariablesExtended.velocity = Math::Vector3D::ZeroValue;
								}

								++pseudoGrenadeEntityVariablesExtended.numberFloorHits;
							}
						}
						}
					}
				}
				else
				{
					pseudoGrenadeEntityVariablesExtended.flags |= SDK::Constants::FL_ONGROUND;
					pseudoGrenadeEntityVariablesExtended.groundEntity = traceResult.hitEntity;
					pseudoGrenadeEntityVariablesExtended.velocity = Math::Vector3D::ZeroValue;
				}
			}
		}

		// check for in water
//		SV_CheckWaterTransition (grenade);
		{
		const SDK::Constants::PointContentType_t cont (Globals::g_halfLifeEngine->GetPointContents (pseudoGrenadeEntityVariablesExtended.origin));

		if (pseudoGrenadeEntityVariablesExtended.waterType == SDK::Constants::PointContentType_None)
		{	// just spawned here
			pseudoGrenadeEntityVariablesExtended.waterType = cont;
			pseudoGrenadeEntityVariablesExtended.waterLevel = 1;

			pseudoGrenadeEntityVariablesExtended.flags &= ~SDK::Constants::FL_BASEVELOCITY;

			continue;
		}

		if (cont > SDK::Constants::Content_Water || cont <= SDK::Constants::Content_Translucent)
		{
			pseudoGrenadeEntityVariablesExtended.waterType = SDK::Constants::Content_Empty;
			pseudoGrenadeEntityVariablesExtended.waterLevel = 0;
		}
		else
		{
			if (pseudoGrenadeEntityVariablesExtended.waterType == SDK::Constants::Content_Empty)
			{	// just crossed into water
				pseudoGrenadeEntityVariablesExtended.velocity.z *= 0.5f;
			}

			pseudoGrenadeEntityVariablesExtended.waterType = cont;
			pseudoGrenadeEntityVariablesExtended.waterLevel = 3;
		}
		}
		}

		pseudoGrenadeEntityVariablesExtended.flags &= ~SDK::Constants::FL_BASEVELOCITY;
	} while (grenade->IsGrenadeIsSmokeGrenade () ? grenade->variables.damageTime > (pseudoTime += pseudoFrameTimeInterval) || !(pseudoGrenadeEntityVariablesExtended.flags & SDK::Constants::FL_ONGROUND) : grenade->variables.damageTime > (pseudoTime += pseudoFrameTimeInterval));

	pseudoGrenadeEntityVariables = pseudoGrenadeEntityVariablesExtended;

	// Restore touched breakables solid types
	for (unsigned char index (0u); index < touchedBreakables.GetElementNumber (); ++index)
		touchedBreakables[index]->variables.solid = SDK::Constants::SOLID_BSP;

	for (unsigned char index (0u); index < touchedPushables.GetElementNumber (); ++index)
		touchedPushables[index]->variables.solid = SDK::Constants::SOLID_TRIGGER;
}
void TraceGrenadeToss (SDK::Classes::Edict *const grenadeOwner, const Server::ThrownGrenadesManager::ThrownGrenade_t::Type_t grenadeType, PseudoGrenadeEntityVariables_t &pseudoGrenadeEntityVariables)
{
	// Don't use me!!!

	SDK::Classes::Edict pseudoGrenade;

	Math::Angles3D anglesThrow (grenadeOwner->variables.viewAngles + grenadeOwner->GetPunchAngles ());

	if (anglesThrow.pitch < 0.0f)
		anglesThrow.pitch = -10.0f + anglesThrow.pitch * ((90.0f - 10.0f) / 90.0f);
	else
		anglesThrow.pitch = -10.0f + anglesThrow.pitch * ((90.0f + 10.0f) / 90.0f);

	float velocity ((90.0f - anglesThrow.pitch) * 6.0f);

	if (velocity > 750.0f)
		velocity = 750.0f;

	anglesThrow.MakeForwardVector ();

	const Math::Vector3D vectorSource (grenadeOwner->GetEyePosition () + Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * 16.0f);
	const Math::Vector3D vectorThrow (Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * velocity + grenadeOwner->variables.velocity);

	pseudoGrenade.variables.origin = vectorSource;
	pseudoGrenade.variables.velocity = vectorThrow;
	pseudoGrenade.variables.baseVelocity = Math::Vector3D::ZeroValue;
	pseudoGrenade.variables.owner = grenadeOwner;
	pseudoGrenade.variables.damageTime = SDK::Constants::GrenadeExplodeDelayTime;
	pseudoGrenade.variables.nextThinkTime = Globals::g_halfLifeEngine->GetTime () + 0.1f;
	pseudoGrenade.variables.flags = SDK::Constants::EntityFlag_None;
	pseudoGrenade.variables.groundEntity = NULL;
	pseudoGrenade.variables.waterLevel = 0u;
	pseudoGrenade.variables.waterType = SDK::Constants::PointContentType_None;

	// HE
	if (grenadeType == Server::ThrownGrenadesManager::ThrownGrenade_t::Type_Explosive)
	{
		pseudoGrenade.variables.gravity = 0.55f;
		pseudoGrenade.variables.friction = 0.7f;
	}
	else
	{
		// Same values for flashbang/smoke grenades....
		pseudoGrenade.variables.gravity = 0.5f;
		pseudoGrenade.variables.friction = 0.8f;

		// SMOKE
		if (grenadeType == Server::ThrownGrenadesManager::ThrownGrenade_t::Type_Smoke)
			pseudoGrenade.variables.model = Globals::g_halfLifeEngine->GetOffsetFromString ("models/w_smokegrenade.mdl");
	}

	TraceThrownGrenadeToss (&pseudoGrenade, pseudoGrenadeEntityVariables);
}
}
}

inline void ModelTraceHull (const unsigned short firstClipNodeIndex, const Math::Vector3D &source, const Math::Vector3D &destination, HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult)
{
	// Fill in a default trace result
	memset (&traceResult, 0, sizeof (traceResult));

	traceResult.fraction = 1.0f;
	traceResult.isAllSolid = true;
	traceResult.endPosition = destination;

	// general sweeping through world
	RecursiveHullCheck (firstClipNodeIndex, firstClipNodeIndex, 0.0f, 1.0f, source, destination, traceResult);
}
inline void ModelTraceHull (const unsigned short modelIndex, const HalfLifeEngine::SDK::Constants::Hull_t hullNumber, const Math::Vector3D &source, const Math::Vector3D &destination, HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult)
{
	ModelTraceHull (static_cast <short> (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().models[modelIndex].headNode[hullNumber]), source, destination, traceResult);
}

void UTIL_TraceHull (const Math::Vector3D &source, const Math::Vector3D &destination, const HalfLifeEngine::SDK::Constants::Hull_t hullNumber, HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult)
{
//	TraceLine (source, destination, traceResult);

	HalfLifeEngine::SDK::Structures::TraceResult_t EntTrace;

	ModelTraceHull (0u, hullNumber, source, destination, traceResult);
/*
	static bool initCalled (false);

	if (!initCalled)
	{
		NEW::SV_InitBoxHull ();

		initCalled = true;
	}

	// create a temp hull from bounding box sizes
	HalfLifeEngine::SDK::Structures::Hull_t *const newHull (NEW::SV_HullForBox (/*Math::Vector3D::ZeroValue, Math::Vector3D::ZeroValue*//*Math::Vector3D (4096.0f), Math::Vector3D (4096.0f)));

	NEW::RecursiveHullCheck (newHull, newHull->firstClipNodeIndex, 0.0f, 1.0f, source, destination, traceResult);*/
#if 0
	if (traceResult.fraction < 1.0f)
	{
//		for (unsigned short index (0u); index < g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().nodes.GetElementNumber (); ++index)
//		for (unsigned short index (0u); index < g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().faces.GetElementNumber (); ++index)
		for (unsigned short index (0u); index < g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes.GetElementNumber (); ++index)
//			if (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().nodes[index].planeNum == traceResult.hitGroup)
//			if (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().faces[index].planeNum == traceResult.hitGroup)
			if (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes[index].planeNum == traceResult.hitGroup)
			{
				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Hit plane(%i) was found in BSP nodes!!!!!!!", traceResult.hitGroup);

				return;
			}

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Hit plane(%i) was NOT found in BSP nodes!!!!!!!", traceResult.hitGroup);
	}
#endif
	// loop through all the entities looking for "func_wall"...
	// And check for collisions with them...
	for (unsigned short entityIndex (1u/* skip "worldspawn" entity */); entityIndex < g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().entities.GetElementNumber (); ++entityIndex)
	{
		const WorldMap::Entity_t &entity (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().entities[entityIndex]);

		if (!entity.isStaticSolid)
			continue;

		// Reliability check.
		InternalAssert (entity.brushModelIndex > 0u);
//		if (entity.brushModelIndex == 0u)
//			continue;

		ModelTraceHull (entity.brushModelIndex, hullNumber, source, destination, EntTrace);

		if (traceResult.fraction > EntTrace.fraction)
		{
			traceResult = EntTrace;

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "I hit entity %s.", entity.className.GetData ());
		}
	}
}

const float	K_EpsilonTrace (0.01f);

void RayTrace::GetHullOffsets (const Math::Vector3D &planeNormal, const Math::Plane::PointSide_t side, float &offsetN, float &offsetF) const
{
	switch (m_type)
	{
		case Type_Point:
			offsetN = offsetF = 0.0f;	// Zero offset.

			break;

		case Type_AxisAlignedBox:
			offsetF = -(offsetN = m_hullSize.GetAbsoluteDotProduct (planeNormal));

			break;

		case Type_AxisAlignedCylinder:
			offsetF = -(offsetN = Math::fabsf (m_hullSize.y * planeNormal[m_axisID]) + GetRadius () * Math::sqrtf (planeNormal[m_B1] * planeNormal[m_B1] + planeNormal[m_B2] * planeNormal[m_B2]));

			break;

		case Type_Sphere:
			offsetF = -(offsetN = GetRadius ());

			break;

		case Type_Ellipsoid:
			offsetF = -(offsetN = (m_hullSize * planeNormal).GetLength ());

			break;

		case Type_Player:
		{
			/**
			Actually not consistent with actual moves of half-life player edicts...
			the hull is the AABox clipped by a cone the vertex of which is the center
			of the bottom AABox square base and is the cone lowest point, and
			with a (half-)angle of cos = Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ();
			On walkable faces, the hull hits the ground on the cone vertex.
			*/
			offsetF = -(offsetN = m_hullSize.GetAbsoluteDotProduct (planeNormal));
			const float Nz_a (side == Math::Plane::PointSide_Back ? -planeNormal.z : planeNormal.z);

			if (Nz_a > 0.0f)
			{
				offsetF = -m_hullSize.z * Nz_a;

				if (Nz_a > Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ())
					offsetN = -offsetF;
			}
			else if (Nz_a < 0.0f)
			{
				offsetF = m_hullSize.z * Nz_a;

				if (Nz_a < -Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ())
					offsetN = -offsetF;
			}

			break;
		}
/*
		case any convex hull:
		{
			Math::Vector3D Norm_a (planeNormal);

			if (side == Math::Plane::PointSide_Back)
				Norm_a = -Norm_a;

			offsetN > 0.0f: distance from hull center to touching plane in the direction of -Norm_a
			offsetF < 0.0f: distance from hull center to touching plane in the direction of Norm_a
			The hull center can be any point, remembering that it is implicitely the point moved from start to end.

			break;
		}
*/
		#if defined _DEBUG
			default:
				AddLogEntry (false, LogLevel_Critical, true, "TraceBUG: RayTrace::GetHullOffsets(): unknown trace type!");
		#endif	// if defined _DEBUG
	}
}

void RayTrace::Execute (void)
{
	const float	epsilonTrace (0.01f);
	Math::Vector3D start (m_source), end (m_destination);
	const unsigned short startLeafNodeIndex (g_server->GetNavigationMeshManager ().GetWorldMap ().FindLeaf (start));
	const HalfLifeEngine::SDK::Structures::DLeaf_t &startLeaf (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().leafs[startLeafNodeIndex]);

	memset (&m_result, 0, sizeof (m_result));

	m_result.planeNum = -1;
	m_result.faceNum = -1;
	m_result.endPosition = end;
	m_result.startLeafNum = startLeafNodeIndex;
	m_result.fraction = 1.0f;
	m_result.startContents = startLeaf.contents;

	if (startLeaf.contents == HalfLifeEngine::SDK::Constants::Content_Solid)
		m_result.isStartSolid = true;

	m_tempResult.planeNum = -1;
	m_tempResult.fraction = 1.0f;
	m_tempResult.furthestNonBlockingLeafNum = -1;
	const unsigned char NodeNum_a (0u);	// start at the root of the BSP tree

	InternalAssert (m_parameters == NULL);

	m_parameters = new TraceParameters_t ();

	InternalAssert (m_parameters != NULL);

	m_parameters->NodeNum = NodeNum_a;
	m_parameters->StartFrac = 0.0f;
	m_parameters->EndFrac = 1.0f;
	m_parameters->StartPoint = start;
	m_parameters->EndPoint = end;
	m_parameters->StartPlane = -1;
	m_parameters->EndPlane = -1;

	// m_type = Type_Point; TraceMovePointR (NodeNum_a, *m_parameters, m_result); delete m_parameters;
	TraceMove (m_result);

	if (m_result.fraction > 0.0f && m_result.fraction < 1.0f)
	{
		m_result.planePointer = &g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes[static_cast <unsigned short> (m_result.planeNum)];
		m_result.plane = *m_result.planePointer;
		m_result.endPosition = start.GetInterpolated (end, m_result.fraction);

		const Math::Vector3D &Delta_a (end - start);
		const float Length_a (Delta_a.GetLength ());

		m_result.hitDistance = Length_a * m_result.fraction;

		if (m_type == Type_Point)
		{
			if (m_flags & (Flag_Face | Flag_Texture))
			{
				Result_t TraceF_a;

				TraceF_a = m_result;
/*
				// off plane, normal to plane && closer to start
//				Offset_a = -Offset_a;

				TraceF_a.endPosition -= plane->normal * Offset_a;
*/
				m_result.faceNum = TraceMoveFindFace (start, TraceF_a);
#if defined _DEBUG
				if (m_result.faceNum == -2)
					AddLogEntry (false, LogLevel_Critical, true, "TraceBUG: No Face on Plane; start=%10.4f %10.4f %10.4f end=%10.4f %10.4f %10.4f\n", start.x, start.y, start.z, end.x, end.y, end.z);
				else if (m_result.faceNum == -1)
					AddLogEntry (false, LogLevel_Critical, true, "TraceBUG: Endpos not on Face; start=%10.4f %10.4f %10.4f end=%10.4f %10.4f %10.4f\n", start.x, start.y, start.z, end.x, end.y, end.z);
#endif	// if defined _DEBUG
				if ((m_flags & Flag_Texture) && m_result.faceNum >= 0)
					m_result.textureName = g_server->GetNavigationMeshManager ().GetWorldMap ().GetTextureName (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().faces[static_cast <unsigned short> (m_result.faceNum)]);
			}

			// endPosition moved slightly off plane, normal to plane && in blocking leaf;
			// this ensure that FindLeaf(endPosition) returns the same leaf number as blockingLeafNum
			Result_t Trace_a (m_result);
			const HalfLifeEngine::SDK::Structures::DPlane_t &plane (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes[static_cast <unsigned short> (m_result.planeNum)]);
			const float Offset_a (plane.normal | Delta_a);

			// change to "if (Offset_a < 0.0f)" if endPosition needs to be IN the NON blocking leaf
			Trace_a.endPosition += plane.normal * (Offset_a > 0.0f ? epsilonTrace : -epsilonTrace);
#if defined _DEBUG
			const int LeafF0_a (g_server->GetNavigationMeshManager ().GetWorldMap ().FindLeaf (Trace_a.endPosition));

			InternalAssert (Trace_a.contents != HalfLifeEngine::SDK::Constants::Content_Solid || LeafF0_a == 0);
//			if (Trace_a.contents == HalfLifeEngine::SDK::Constants::Content_Solid && LeafF0_a != 0)
//				AddLogEntry (false, LogLevel_Critical, true, "TraceBUG: FindLeaf discrepancy on endPosition: Blocking Leaf should be solid");
#endif	// if defined _DEBUG
		}
		else
		{
			m_result.furthestNonBlockingLeafNum = g_server->GetNavigationMeshManager ().GetWorldMap ().FindLeaf (m_result.endPosition);
/*
			const HalfLifeEngine::SDK::Structures::DPlane_t &plane (&g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes[static_cast <unsigned short> (m_result.planeNum)]);
			float Offset_a;

			if (m_type == Type_AxisAlignedBox)
				Offset_a = m_hullSize.GetAbsoluteDotProduct (plane.normal);
			else if (m_type == Type_AxisAlignedCylinder)
				Offset_a = Math::fabsf (m_hullSize.z * plane.normal[m_axisID]) + GetRadius () * Math::sqrtf (1.0f - plane.normal[m_axisID] * plane.normal[m_axisID]);
			else if (m_type == Type_Sphere)
				Offset_a = GetRadius ();

			float DistSE_a (-(plane.normal | start));
			const float OppDistStart_a (DistSE_a + plane.distance);

			if (OppDistStart_a <= 0.0f)
				Offset_a = -Offset_a;

			DistSE_a += plane.normal | end;
			const float Frac_a ((OppDistStart_a + Offset_a) / DistSE_a);
			const Math::Vector3D Endpos_a (start.GetInterpolated (end, Frac_a));

			Delta_a = Endpos_a - m_result.endPosition;

			const float Bug_a (Delta_a.GetLength ());
//			InternalAssert (Bug_a < epsilonTrace);
*/
		}
	}
	else if (m_result.fraction == 1.0f)// && m_result.fraction != 0.0f)
		m_result.furthestNonBlockingLeafNum = g_server->GetNavigationMeshManager ().GetWorldMap ().FindLeaf (m_result.endPosition);
}

const bool RayTrace::TraceToLeaf (const unsigned short leafNodeIndex, Result_t &result) const
{
	const HalfLifeEngine::SDK::Structures::DLeaf_t &leaf (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().leafs[leafNodeIndex]);

	if (leaf.contents == HalfLifeEngine::SDK::Constants::Content_Solid || leaf.contents == HalfLifeEngine::SDK::Constants::Content_Sky)
	{
		result.contents = leaf.contents;

		if (leaf.contents == HalfLifeEngine::SDK::Constants::Content_Solid && result.isStartSolid)
			result.isAllSolid = true;

		return true;
	}
	else
	{
		// TO DO : trace against entities (partially) in leaf? (solid brush models + studio models)
		// TO DO before?: FindLeaf (entvar, leaflist) returning the leaf(s) the ent crosses (at least for non moving entities)
		// and be able to use tha data the other way round (from a leaf to entities that are in)
		return false;
	}
}

const int RayTrace::TraceMoveFindFace (const Math::Vector3D &/*start*/, Result_t &result) const
{
	if (result.fraction == 1.0f)
		return -1;

	int Pb_a (-2);
	const HalfLifeEngine::SDK::Structures::DLeaf_t &endLeaf (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().leafs[static_cast <unsigned short> (result.furthestNonBlockingLeafNum)]);
	const int planeHitNum (result.planeNum);
	const HalfLifeEngine::SDK::Structures::DPlane_t &planeHit (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes[static_cast <unsigned short> (result.planeNum)]);

	for (unsigned short markSurfaceIndex (0u), faceIndex; markSurfaceIndex < endLeaf.markSurfacesNum; ++markSurfaceIndex)
	{
		faceIndex = g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().markSurfaces[static_cast <unsigned short> (endLeaf.firstMarkSurface + markSurfaceIndex)];

		const HalfLifeEngine::SDK::Structures::DFace_t &face (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().faces[faceIndex]);

		if (face.planeNum != planeHitNum)
			continue;

		Pb_a = -1;
		//check if inters is on face (ray crosses plane inside face polygon)
		unsigned char Sign_a (3u);
		unsigned char SignPrev_a (3u);
		Math::Vector3D vertex0;
		Math::Vector3D vertex1;
		Math::Vector3D VToIsec_a;
		Math::Vector3D edge;
		unsigned short vertexID (g_server->GetNavigationMeshManager ().GetWorldMap ().GetFirstVertexID (face, 0u));
		vertex1 = g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().vertexes[vertexID].point;
		unsigned short edgeIndex (1u);

		while ((Sign_a & SignPrev_a) && edgeIndex < face.surfEdgesNum)
		{
			vertex0 = vertex1;
			vertexID = g_server->GetNavigationMeshManager ().GetWorldMap ().GetFirstVertexID (face, edgeIndex);
			vertex1 = g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().vertexes[vertexID].point;
			VToIsec_a = vertex0 - result.endPosition;
			edge = vertex1 - vertex0;
			const float Dot_a ((edge ^ VToIsec_a) | planeHit.normal);

//			if (Dot_a == 0.0f)
			if (Math::fabsf (Dot_a) < K_EpsilonTrace)
				return faceIndex;

			SignPrev_a = Sign_a;
			Sign_a = Dot_a > 0.0f ? 1u : 2u;
			++edgeIndex;
		}

		if (Sign_a & SignPrev_a)
			return faceIndex;
	}

	return Pb_a;
}

const char RayTrace::TraceMovePointR (const int nodeIndex, const TraceParameters_t &traceParameters, Result_t &result)
{
	// this works only for point trace.
	// when starting from a solid leaf, the hit, if any, is computed as if back-face solid faces do not
	// exist. In other words, given a direction,
	// the returned values are always the same whatever the starting point.

	//_ the "if (result.fraction > ...)" checks and using the Side_a value
	// result in looking for the nearest (from m_source) blocking leaf
	//
	if (nodeIndex < 0)	// in a leaf
	{
		// not a blocking leaf; store data in case its neighbour (further from m_source) blocks
		if (!TraceToLeaf (static_cast <unsigned short> (-(nodeIndex + 1)), result))
		{
			m_tempResult.planeNum = traceParameters.EndPlane;
			m_tempResult.fraction = traceParameters.EndFrac;
			m_tempResult.furthestNonBlockingLeafNum = -(nodeIndex + 1);	// this one=last (closer to m_source) non-blocking neighbour leaf of possibly blocking neighbour leaf

			return 0;
		}
		else
		{
			result.blockingLeafNum = -(nodeIndex + 1);
			result.furthestNonBlockingLeafNum = m_tempResult.furthestNonBlockingLeafNum;	// last leaf before hitting this one
			result.fraction = m_tempResult.fraction;
			result.planeNum = m_tempResult.planeNum;

			return 1;
		}
	}

	if (result.fraction <= traceParameters.StartFrac)
		return -1;

	const HalfLifeEngine::SDK::Structures::DNode_t &node (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().nodes[static_cast <unsigned short> (nodeIndex)]);
	const HalfLifeEngine::SDK::Structures::DPlane_t &plane (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes[static_cast <unsigned short> (node.planeNum)]);
	const float DistNear_a ((plane.normal | traceParameters.StartPoint) - plane.distance);
	const float DistFar_a ((plane.normal | traceParameters.EndPoint) - plane.distance);

	if (DistNear_a >= 0.0f && DistFar_a >= 0.0f)
	{
		TraceMovePointR (node.children[HalfLifeEngine::SDK::Structures::DNode_t::ChildrenType_Front], traceParameters, result);

		if (DistNear_a == 0.0f && DistFar_a == 0.0f)
			TraceMovePointR (node.children[HalfLifeEngine::SDK::Structures::DNode_t::ChildrenType_Back], traceParameters, result);
	}
	else if (DistNear_a <= 0.0f && DistFar_a <= 0.0f)
		TraceMovePointR (node.children[HalfLifeEngine::SDK::Structures::DNode_t::ChildrenType_Back], traceParameters, result);
	else
	{
		TraceParameters_t TP_a (traceParameters);
		const Math::Plane::PointSide_t Side_a (DistNear_a < 0.0f ? Math::Plane::PointSide_Back : Math::Plane::PointSide_Front);	// front/on is zero
		const float Frac_a (-DistNear_a / (DistFar_a - DistNear_a));	// intersection with plane
		const float kInterp_a (Math::GetInterpolated (traceParameters.StartFrac, traceParameters.EndFrac, Frac_a));

		InternalAssert (traceParameters.StartFrac <= kInterp_a && kInterp_a <= traceParameters.EndFrac);	// sometimes == due to float rounding

//		const Math::Vector3D &Interp_a (m_source.GetInterpolated (m_destination, kInterp_a));
		const Math::Vector3D &Interp_a (traceParameters.StartPoint.GetInterpolated (traceParameters.EndPoint, Frac_a));

		TP_a.EndFrac = kInterp_a;
		TP_a.EndPoint = Interp_a;
		TP_a.EndPlane = node.planeNum;
		TraceMovePointR (node.children[Side_a], TP_a, result);

		if (result.fraction > kInterp_a)
		{
			TP_a = traceParameters;
			TP_a.StartFrac = kInterp_a;
			TP_a.StartPoint = Interp_a;
			TP_a.StartPlane = node.planeNum;
			TraceMovePointR (node.children[Side_a ^ 1u], TP_a, result);
		}
	}

	return -1;
}

void RayTrace::TraceMove (Result_t &result)
{
	// 1.As opposed to traceMovePoint, traces starting in a solid leaf "stop dead"

	// 2.For traces other than point trace (m_type != Type_Point),
	// this does not work in all cases.
	// in all cases (I think), fraction will be less then 1 if something is hit on the way,
	// but the fraction value, endPosition, ... may be meaningless when the ACTUAL hit point is not
	// a vertex (a full edge/face span hit include >= 1 vertex and then is OK)
	// For the sphere, the true hit point must be on the [m_source, m_destination] line
	// For the cylinder, the true hit point must be on one of the circle bases.

	// In other words, hits in the middle of an edge or a face result in inaccurate fraction, endPosition....
	// the worst case is probably moving the hull towards an acute wegde pointing at the hull

	// It will then work 100% in particular
	//-for traces strictly within a convex closed volume.
	//-and for AABox traces (m_type == Type_AxisAlignedBox) against axis aligned objects in the world.


	//_ the "if (result.fraction > ...)" checks and using the Side_a value
	// result in looking for the nearest (from m_source) blocking leaf
	//

	// instead of popping m_parameters, deleting it, then pushing a new TraceParameters_t *, m_parameters is re-used whenever possible.

	TraceParameters_t *pArgs2_n;

	do
	{
		if (m_parameters->State == 20)
		{
			if (result.fraction > m_parameters->StartFrac)
				m_parameters->State = 0;
			else
			{
				TraceParameters_t *const pToDel_a (m_parameters);

				m_parameters = m_parameters->Next;

				delete pToDel_a;
			}
		}
		else if (m_parameters->NodeNum < 0)	// in a leaf
		{
			if (!TraceToLeaf (static_cast <unsigned short> (-(m_parameters->NodeNum + 1)), result))	// not a blocking leaf
				m_tempResult.furthestNonBlockingLeafNum = -(m_parameters->NodeNum + 1);
			else if (m_parameters->StartFrac <= result.fraction)
			{
				result.blockingLeafNum = -(m_parameters->NodeNum + 1);
				result.furthestNonBlockingLeafNum = m_tempResult.furthestNonBlockingLeafNum;
				result.fraction = m_parameters->StartFrac;
				result.planeNum = m_parameters->StartPlane;
			}

			TraceParameters_t *const pToDel_a (m_parameters);

			m_parameters = m_parameters->Next;

			delete pToDel_a;
		}
		else if (result.fraction >= m_parameters->StartFrac)
		{
			const HalfLifeEngine::SDK::Structures::DNode_t &node (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().nodes[static_cast <unsigned short> (m_parameters->NodeNum)]);
			const HalfLifeEngine::SDK::Structures::DPlane_t &plane (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes[static_cast <unsigned short> (node.planeNum)]);
			float DistNear_a ((plane.normal | m_parameters->StartPoint) - plane.distance);
			float DistFar_a ((plane.normal | m_parameters->EndPoint) - plane.distance);
			Math::Plane::PointSide_t Side2_a (Math::Plane::PointSide_Front);
			const Math::Plane::PointSide_t Side_a (DistNear_a < DistFar_a ? Math::Plane::PointSide_Back : Math::Plane::PointSide_Front);
			float offsetN;
			float offsetF;

			if (DistNear_a > DistFar_a)
			{
				Side2_a = Math::Plane::PointSide_Back;
				DistNear_a = -DistNear_a;
				DistFar_a = -DistFar_a;
			}

			GetHullOffsets (plane.normal, Side2_a, offsetN, offsetF);

			if (DistNear_a >= offsetN && DistFar_a >= offsetN)	// > 0
			{
				if (m_type == Type_Point && DistNear_a == 0.0f && DistFar_a == 0.0f)
				{
					// push next to process
					pArgs2_n = new TraceParameters_t ();
					*pArgs2_n = *m_parameters;
					pArgs2_n->NodeNum = node.children[Side2_a ^ 1u];
					pArgs2_n->State = 0;
					pArgs2_n->Next = m_parameters->Next;
					m_parameters->Next = pArgs2_n;
				}

				// re-use m_parameters
				m_parameters->State = 0;
				m_parameters->NodeNum = node.children[Side2_a];
			}
			else if (DistNear_a <= offsetF && DistFar_a <= offsetF)	// < 0
			{
				// re-use m_parameters
				m_parameters->State = 0;
				m_parameters->NodeNum = node.children[Side2_a ^ 1u];
			}
			else
			{
				const float DDist_a (DistFar_a - DistNear_a);	// >= 0
				float FracN_a;
				float FracF_a;

				if (DDist_a == 0.0f)
				{
					FracF_a = 0.0f;
					FracN_a = 1.0f;
				}
				else
				{
					// near: ends beyond plane (or on TP0_a.EndPoint if closer to StartPoint)
					FracN_a = (-DistNear_a + offsetN) * (1.0f / DDist_a);
					// far: starts before plane (or on TP0_a.StartPoint if closer to EndPoint)
					FracF_a = (-DistNear_a + offsetF) * (1.0f / DDist_a);

					// OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//					InternalAssert (FracN_a == Math::GetClampedValueInRange (FracN_a, 0.0f, 1.0f));
//					InternalAssert (FracF_a == Math::GetClampedValueInRange (FracF_a, 0.0f, 1.0f));
				}

				// push next to process
				pArgs2_n = new TraceParameters_t ();
				*pArgs2_n = *m_parameters;
				pArgs2_n->NodeNum = node.children[Side_a ^ 1u];
				pArgs2_n->StartPlane = node.planeNum;
				pArgs2_n->State = 20;

				if (FracF_a > 0.0f)
				{
					pArgs2_n->StartFrac = Math::GetInterpolated (m_parameters->StartFrac, m_parameters->EndFrac, FracF_a);
					pArgs2_n->StartPoint = m_parameters->StartPoint.GetInterpolated (m_parameters->EndPoint, FracF_a);
				}

				pArgs2_n->Next = m_parameters->Next;
				m_parameters->Next = pArgs2_n;

				// re-use m_parameters
				if (FracN_a < 1.0f)
				{
					m_parameters->EndFrac = Math::GetInterpolated (m_parameters->StartFrac, m_parameters->EndFrac, FracN_a);
					m_parameters->EndPoint = m_parameters->StartPoint.GetInterpolated (m_parameters->EndPoint, FracN_a);
				}

				m_parameters->NodeNum = node.children[Side_a];
				m_parameters->State = 0;
			}
		}
		else
		{
			TraceParameters_t *const pToDel_a (m_parameters);

			m_parameters = m_parameters->Next;

			delete pToDel_a;
		}
	} while (m_parameters != NULL);
}
/*
// solid objects
for (unsigned short entityIndex (0u); entityIndex < g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().entities.GetElementNumber (); ++entityIndex)
{
	if (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().entities[entityIndex].brushModelIndex > 0u)
	{
		if (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().entities[entityIndex].renderAmount == 255.0f)
		{
			const unsigned short modelIndex (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().entities[entityIndex].brushModelIndex);
			const HalfLifeEngine::SDK::Structures::DModel_t &model (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().models[modelIndex]);

			for (unsigned short modelFaceIndex (0u); modelFaceIndex < model.facesNum; ++modelFaceIndex)
			{
				const HalfLifeEngine::SDK::Structures::DFace_t &face (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().faces[model.firstFaceIndex + modelFaceIndex]);

				// SOME ACTIONS!
			}
		}
	}
	else if (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().entities[entityIndex].studioModel != NULL)
	{
		// faces in world?
	}
}

to get studioModel faces in world?
see StudioModel::DrawModel?
*/
#if 0
void CM_ClearTrace( trace_t *trace )
{
	memset( trace, 0, sizeof(*trace));
	trace->fraction = 1.0f;
	trace->fractionleftsolid = 0.0f;
	trace->surface = nullsurface;
}

// 1/256 epsilon to keep floating point happy
#define DIST_EPSILON 0.00390625f

/*
===============================================================================

BOX TRACING

===============================================================================
*/

/*
================
CM_ClipBoxToBrush
================
*/
void FASTCALL CM_ClipBoxToBrush( CCollisionBSPData *pBSPData, const Math::Vector3D& mins, const Math::Vector3D& maxs, const Math::Vector3D& source, const Math::Vector3D& destination, trace_t *trace, cbrush_t *brush )
{
        if (!brush->numsides)
                return;

        g_CollisionCounts.m_BrushTraces++;

        float enterfrac = NEVER_UPDATED;
        float leavefrac = 1.0f;
        cplane_t* clipplane = NULL;

        bool getout = false;
        bool startout = false;
        cbrushside_t* leadside = NULL;

        float distance;

        cbrushside_t *side = &pBSPData->map_brushsides[brush->firstbrushside];
        for (int i (0); i < brush->numsides; ++i, ++side)
        {
                cplane_t *plane = side->plane;

                if (!trace_ispoint)
                {
					// general axial box case

					// push the plane out appropriately for mins/maxs

					// FIXME: special case for axial - NOTE: These axial planes are not always positive!!!
					// FIXME: use signbits into 8 way lookup for each mins/maxs

					// Compute the sign bits
					//unsigned int nSigns  =  *reinterpret_cast <int *> (&plane->normal.x) & 0x80000000;
					//             nSigns |= (*reinterpret_cast <int *> (&plane->normal.y) & 0x80000000) >> 1;
					//             nSigns |= (*reinterpret_cast <int *> (&plane->normal.z) & 0x80000000) >> 2;
//			for (unsigned char j (0u); j < 3u; ++j)
//			{
				// Set signmask to either 0 if the sign is negative, or 0xFFFFFFFF is the sign is positive:
				//int signmask = (((*(int *)&(plane->normal[j]))&0x80000000) >> 31) - 1;

				//float temp = maxs[j];
				//*(int *)&(ofs[j]) =    (~signmask) & (*(int *)&temp);
				//float temp1 = mins[j];
				//*(int *)&(ofs[j]) |=   (signmask) & (*(int *)&temp1);
//			}

					const Math::Vector3D offset
					(
						plane->normal.x >= 0.0f ? mins.x : maxs.x,
						plane->normal.y >= 0.0f ? mins.y : maxs.y,
						plane->normal.z >= 0.0f ? mins.z : maxs.z
					);

					distance = offset | plane->normal;
					distance = plane->distance - distance;
                }
                else
                {
                        // special point case
                        distance = plane->distance;
                        // don't trace rays against bevel planes
                        if( side->bBevel )
                                continue;
                }

                float d1 = (source | plane->normal) - distance;
                float d2 = (destination | plane->normal) - distance;

                // if completely in front of face, no intersection
                if( d1 > 0.0f )
                {
                        startout = true;

                        // d1 > 0.0f && d2 > 0.0f
                        if( d2 > 0.0f )
                                return;

                }
				else
                {
                        // d1 <= 0.0f && d2 <= 0.0f
                        if( d2 <= 0.0f )
                                continue;

                        // d2 > 0.0f
                        getout = true;
                }

                // crosses face
                if (d1 > d2)
                {       // enter
                        // JAY: This could be negative if d1 is less than the epsilon.
                        // If the trace is short (d1-d2 is small) then it could produce a large
                        // negative fraction. I can't believe this didn't break Q2!
                        float f (d1-DIST_EPSILON);
                        if ( f < 0.0f )
                                f = 0.0f;
                        f /= d1 - d2;
                        if (f > enterfrac)
                        {
                                enterfrac = f;
                                clipplane = plane;
                                leadside = side;
                        }
                }
                else
                {       // leave
                        float f = (d1+DIST_EPSILON) / (d1-d2);
                        if (leavefrac > f)
                            leavefrac = f;
                }
        }

        // when this happens, we entered the brush *after* leaving the previous brush.
        // Therefore, we're still outside!

        // NOTE: We only do this test against points because fractionleftsolid is
        // not possible to compute for brush sweeps without a *lot* more computation
        // So, client code will never get fractionleftsolid for box sweeps
        if (trace_ispoint && startout)
        {
                // Add a little sludge.  The sludge should already be in the fractionleftsolid
                // (for all intents and purposes is a leavefrac value) and enterfrac values.
                // Both of these values have ±DIST_EPSILON values calculated in. Thus, I
                // think the test should be against "0.0."  If we experience new "left solid"
                // problems you may want to take a closer look here!
//              if ((trace->fractionleftsolid - enterfrac) > -1e-6)
                if ((trace->fractionleftsolid - enterfrac) > 0.0f )
                        startout = false;
        }

        if (!startout)
        {       // original point was inside brush
                trace->startsolid = true;
                // return starting contents
                trace->contents = brush->contents;

                if (!getout)
                {
                        trace->allsolid = true;
                        trace->fraction = 0.0f;
                        trace->fractionleftsolid = 1.0f;
                }
                else
                {
                        // if leavefrac == 1, this means it's never been updated or we're in allsolid
                        // the allsolid case was handled above
                        if ((leavefrac != 1) && (leavefrac > trace->fractionleftsolid))
                        {
                                trace->fractionleftsolid = leavefrac;

                                // This could occur if a previous trace didn't start us in solid
                                if (trace->fraction <= leavefrac)
                                {
                                        trace->fraction = 1.0f;
                                        trace->surface = nullsurface;
                                }
                        }
                }
                return;
        }

        // We haven't hit anything at all until we've left...
        if (enterfrac < leavefrac)
        {
                if (enterfrac > NEVER_UPDATED && enterfrac < trace->fraction)
                {
                        if (enterfrac < 0)
                                enterfrac = 0;
                        trace->fraction = enterfrac;
                        trace_bDispHit = false;
                        trace->plane = *clipplane;
                        trace->surface = *leadside->surface;
                        trace->contents = brush->contents;
                }
        }
}
/*
================
CM_TraceToLeaf
================
*/
void FASTCALL CM_TraceToLeaf (CCollisionBSPData *pBSPData, int ndxLeaf, float startFrac, float endFrac)
{
	int nCurrentCheckCount = CurrentCheckCount();
	int nDepth = CurrentCheckCountDepth();

	// get the leaf
	cleaf_t *pLeaf = &pBSPData->map_leafs[ndxLeaf];

	//
	// trace ray/box sweep against all brushes in this leaf
	//
	for (int ndxLeafBrush (0); ndxLeafBrush < pLeaf->numleafbrushes; ++ndxLeafBrush)
	{
		// get the current brush
		int ndxBrush = pBSPData->map_leafbrushes[pLeaf->firstleafbrush+ndxLeafBrush];
		cbrush_t *pBrush = &pBSPData->map_brushes[ndxBrush];

		// make sure we only check this brush once per trace/stab
		if( pBrush->checkcount[nDepth] == nCurrentCheckCount )
			continue;

		// mark the brush as checked
		pBrush->checkcount[nDepth] = nCurrentCheckCount;

		// only collide with objects you are interested in
		if( !( pBrush->contents & trace_contents ) )
			continue;

		// trace against the brush and find impact point -- if any?
		// NOTE: trace_trace.fraction == 0.0f only when trace starts inside of a brush!
		CM_ClipBoxToBrush( pBSPData, trace_mins, trace_maxs, trace_start, trace_end, &trace_trace, pBrush );
		if( !trace_trace.fraction )
			return;
	}

	InternalAssert( nDepth == CurrentCheckCountDepth() );
	InternalAssert( nCurrentCheckCount == CurrentCheckCount() );

	// TODO: this may be redundant
	if( trace_trace.startsolid )
		return;

	// Collide (test) against displacement surfaces in this leaf.
	if( pLeaf->m_pDisplacements )
	{
		//
		// trace ray/swept box against all displacement surfaces in this leaf
		//
		for( CDispIterator it( pLeaf->m_pDisplacements, CDispLeafLink::LIST_LEAF ); it.IsValid(); )
		{
			CDispCollTree *pDispTree = static_cast<CDispCollTree*>( it.Inc()->m_pDispInfo );

			// make sure we only check this brush once per trace/stab
			if( pDispTree->GetCheckCount(nDepth) == nCurrentCheckCount )
				continue;

			// mark the brush as checked
			if( !trace_ispoint )
				pDispTree->SetCheckCount( nDepth, nCurrentCheckCount );

			// only collide with objects you are interested in
			if( !( pDispTree->GetContents() & trace_contents ) )
				continue;

			CM_TraceToDispTree( pDispTree, trace_start, trace_end, trace_mins, trace_maxs, startFrac, endFrac, &trace_trace, trace_ispoint == 1);
			if( !trace_trace.fraction )
				break;
		}

		CM_PostTraceToDispTree();
	}

	InternalAssert( nDepth == CurrentCheckCountDepth() );
	InternalAssert( nCurrentCheckCount == CurrentCheckCount() );
}
/*
======================
CM_RecursiveHullCheck
======================

Traverse all the contacted leafs from the start to the end position.
If the trace is a point, they will be exactly in order, but for larger
trace volumes it is possible to hit something in a later leaf with
a smaller intercept fraction.

Attempt to do whatever is nessecary to get this function to unroll at least once
*/
void FASTCALL CM_RecursiveHullCheck (CCollisionBSPData *const pBSPData, const int num, const float sourceFraction, const float destinationFraction, const Math::Vector3D &source, const Math::Vector3D &destination)
{
	if (trace_trace.fraction <= sourceFraction)
		return;	// already hit something nearer

	cnode_t *node (NULL);
	cplane_t *plane;
	float sourceDistance (0.0f), destinationDistance (0.0f), offset (0.0f);

	// find the point distances to the seperating plane
	// and the offset for the size of the box

	// NJS: Hoisted loop invariant comparison to trace_ispoint

	if (trace_ispoint)
	{
		while (num >= 0)
		{
			node = pBSPData->map_rootnode + num;
			plane = node->plane;

			if (plane->type <= HalfLifeEngine::SDK::Structures::DPlane_t::Type_Z)
			{
				sourceDistance = source[plane->type] - plane->distance;
				destinationDistance = destination[plane->type] - plane->distance;

				offset = trace_extents[plane->type];
			}
			else
			{
				sourceDistance = (plane->normal | source) - plane->distance;
				destinationDistance = (plane->normal | destination) - plane->distance;

				offset = 0.0f;
			}

			// see which sides we need to consider
			if (sourceDistance > offset && destinationDistance > offset)
//			if (sourceDistance >= offset && destinationDistance >= offset)
			{
				num = node->children[0u];

				continue;
			}

			if (sourceDistance < -offset && destinationDistance < -offset)
			{
				num = node->children[1u];

				continue;
			}

			break;
		}
	}
	else
	{
		while (num >= 0)
		{
			node = pBSPData->map_rootnode + num;
			plane = node->plane;

			if (plane->type <= HalfLifeEngine::SDK::Structures::DPlane_t::Type_Z)
			{
				sourceDistance = source[plane->type] - plane->distance;
				destinationDistance = destination[plane->type] - plane->distance;

				offset = trace_extents[plane->type];
			}
			else
			{
				sourceDistance = DotProduct (plane->normal, source) - plane->distance;
				destinationDistance = DotProduct (plane->normal, destination) - plane->distance;

				offset = trace_extents.GetAbsoluteDotProduct (plane->normal);
			}

			// see which sides we need to consider
			if (sourceDistance > offset && destinationDistance > offset)
//			if (sourceDistance >= offset && destinationDistance >= offset)
			{
				num = node->children[0u];

				continue;
			}

			if (sourceDistance < -offset && destinationDistance < -offset)
			{
				num = node->children[1u];

				continue;
			}

			break;
		}
	}

	// if < 0, we are in a leaf node
	if (num < 0)
	{
		// this is a leaf

		CM_TraceToLeaf (pBSPData, -1 - num, sourceFraction, destinationFraction);

		// don't have to do anything else for leaves
		return;
	}

	// this is a node

	float fraction1, fraction2;
	float invertedDistance;
	Math::Vector3D middle;
	float middleFraction;
	Math::Plane::PointSide_t side;

	// split the segment into two

	// put the crosspoint DIST_EPSILON pixels on the near side
	if (sourceDistance < destinationDistance)
	{
		side = Math::Plane::PointSide_Back;	// back

		invertedDistance = 1.0f / (sourceDistance - destinationDistance);

		// make sure the numbers are valid
		fraction1 = Math::GetClampedValueInRange ((sourceDistance - offset - DIST_EPSILON) * invertedDistance, 0.0f, 1.0f);	// (make sure the fraction 1 is valid)
		fraction2 = Math::GetClampedValueInRange ((sourceDistance + offset + DIST_EPSILON) * invertedDistance, 0.0f, 1.0f);	// (make sure the fraction 2 is valid)
	}
	else
	{
		side = Math::Plane::PointSide_Front;	// front

		if (sourceDistance > destinationDistance)
		{
			invertedDistance = 1.0f / (sourceDistance - destinationDistance);

			// make sure the numbers are valid
			fraction1 = Math::GetClampedValueInRange ((sourceDistance + offset + DIST_EPSILON) * invertedDistance, 0.0f, 1.0f);	// (make sure the fraction 1 is valid)
			fraction2 = Math::GetClampedValueInRange ((sourceDistance - offset - DIST_EPSILON) * invertedDistance, 0.0f, 1.0f);	// (make sure the fraction 2 is valid)
		}
		else
		{
			fraction1 = 1.0f;
			fraction2 = 0.0f;
		}
	}

	// calculate the middle point for the first side
	middleFraction = Math::GetInterpolated (sourceFraction, destinationFraction, fraction1);
	middle = source.GetInterpolated (destination, fraction1);

	// move up to the node
	// check the first side
	CM_RecursiveHullCheck (pBSPData, node->children[side], sourceFraction, middleFraction, source, middle);

	// calculate the middle point for the second side
	middleFraction = Math::GetInterpolated (sourceFraction, destinationFraction, fraction2);
	middle = source.GetInterpolated (destination, fraction2);

	// go past to the node
	// check the second side
	CM_RecursiveHullCheck (pBSPData, node->children[side ^ 1u], middleFraction, destinationFraction, middle, destination);
}

//-----------------------------------------------------------------------------
// Box/plane test (slow version)
//-----------------------------------------------------------------------------
inline int FASTCALL BoxOnPlaneSide2 (const Math::Vector3D& emins, const Math::Vector3D& emaxs, const cplane_t *p, float tolerance = 0.0f )
{
	Math::Vector3D	corners[2];

	if (p->normal[0] < 0)
	{
		corners[0][0] = emins[0];
		corners[1][0] = emaxs[0];
	}
	else
	{
		corners[1][0] = emins[0];
		corners[0][0] = emaxs[0];
	}

	if (p->normal[1] < 0)
	{
		corners[0][1] = emins[1];
		corners[1][1] = emaxs[1];
	}
	else
	{
		corners[1][1] = emins[1];
		corners[0][1] = emaxs[1];
	}

	if (p->normal[2] < 0)
	{
		corners[0][2] = emins[2];
		corners[1][2] = emaxs[2];
	}
	else
	{
		corners[1][2] = emins[2];
		corners[0][2] = emaxs[2];
	}

	int sides = 0;

	float dist1 = DotProduct (p->normal, corners[0]) - p->distance;
	if (dist1 >= tolerance)
		sides = 1;

	float dist2 = DotProduct (p->normal, corners[1]) - p->distance;
	if (dist2 < -tolerance)
		sides |= 2;

	return sides;
}

/*
=============
CM_BoxLeafnums

Fills in a list of all the leafs touched
=============
*/

static int              leaf_count, leaf_maxcount;
static int              *leaf_list;
static const Math::Vector3D *leaf_mins, *leaf_maxs;
static int              leaf_topnode;

void CM_BoxLeafnums_r( CCollisionBSPData *pBSPData, int nodenum )
{
        cplane_t        *plane;
        cnode_t         *node;
        int             s;
        int prev_topnode = -1;

        while (1)
        {
                if (nodenum < 0)
                {
                        // This handles the case when the box lies completely
                        // within a single node. In that case, the top node should be
                        // the parent of the leaf
                        if (leaf_topnode == -1)
                                leaf_topnode = prev_topnode;

                        if (leaf_count >= leaf_maxcount)
                        {
//                              Com_Printf ("CM_BoxLeafnums_r: overflow\n");
                                return;
                        }
                        leaf_list[leaf_count++] = -1 - nodenum;
                        return;
                }

                node = &pBSPData->map_rootnode[nodenum];
                plane = node->plane;
//              s = BoxOnPlaneSide (leaf_mins, leaf_maxs, plane);
//              s = BOX_ON_PLANE_SIDE(*leaf_mins, *leaf_maxs, plane);
                Math::Vector3D mins, maxs;
                mins = *leaf_mins;
                maxs = *leaf_maxs;
                s = BoxOnPlaneSide2( mins, maxs, plane );

                prev_topnode = nodenum;
                if (s == 1)
                        nodenum = node->children[0];
                else if (s == 2)
                        nodenum = node->children[1];
                else
                {       // go down both
                        if (leaf_topnode == -1)
                                leaf_topnode = nodenum;
                        CM_BoxLeafnums_r (pBSPData, node->children[0]);
                        nodenum = node->children[1];
                }
        }
}

int CM_BoxLeafnums_headnode ( CCollisionBSPData *pBSPData, const Math::Vector3D& mins, const Math::Vector3D& maxs, int *list, int listsize, int headnode, int *topnode)
{
	leaf_list = list;
	leaf_count = 0;
	leaf_maxcount = listsize;
	leaf_mins = &mins;
	leaf_maxs = &maxs;

	leaf_topnode = -1;

	CM_BoxLeafnums_r (pBSPData, headnode);

	if (topnode)
		*topnode = leaf_topnode;

	return leaf_count;
}

//-----------------------------------------------------------------------------
// Test an unswept box
//-----------------------------------------------------------------------------
static inline void CM_UnsweptBoxTrace( CCollisionBSPData *pBSPData, const Ray_t& ray, int headnode, int brushmask )
{
	int             leafs[1024];
	int             i, numleafs;
	Math::Vector3D  boxMins, boxMaxs;
	int             topnode;

	VectorAdd (ray.m_Start, ray.m_Extents, boxMaxs);
	VectorSubtract (ray.m_Start, ray.m_Extents, boxMins);

	// Bloat the box a little
	for (i = 0; i < 3; ++i)
	{
		--boxMins[i];
		++boxMaxs[i];
	}

	bool bFoundNonSolidLeaf = false;
	numleafs = CM_BoxLeafnums_headnode ( pBSPData, boxMins, boxMaxs, leafs, 1024, headnode, &topnode);
	for (i=0 ; i<numleafs ; i++)
	{
		if ((pBSPData->map_leafs[leafs[i]].contents & CONTENTS_SOLID) == 0)
		{
			bFoundNonSolidLeaf = true;
		}

		CM_TestInLeaf ( pBSPData, leafs[i] );
		if (trace_trace.allsolid)
			break;
	}

	if (!bFoundNonSolidLeaf)
	{
		trace_trace.allsolid = trace_trace.startsolid = true;
		trace_trace.fraction = 0.0f;
		trace_trace.fractionleftsolid = 1.0f;
	}
}

void CM_BoxTrace( const Ray_t& ray, int headnode, int brushmask, bool computeEndpt, trace_t& tr )
{
        g_EngineStats.IncrementCountedStat( ENGINE_STATS_NUM_BOX_TRACES, 1 );
        MEASURE_TIMED_STAT( ENGINE_STATS_BOX_TRACE_TIME );

        // for multi-check avoidance
        BeginCheckCount();

        // for statistics, may be zeroed
        g_CollisionCounts.m_Traces++;

        // fill in a default trace
        CM_ClearTrace( &trace_trace );

        CCollisionBSPData *pBSPData = GetCollisionBSPData();

        // check if the map is not loaded
        if (!pBSPData->numnodes)
        {
                tr = trace_trace;
                EndCheckCount();
                return;
        }

        trace_bDispHit = false;
        trace_StabDir.Init();
        trace_contents = brushmask;
        VectorCopy (ray.m_Start, trace_start);
        VectorAdd  (ray.m_Start, ray.m_Delta, trace_end);
        VectorMultiply (ray.m_Extents, -1.0f, trace_mins);
        VectorCopy (ray.m_Extents, trace_maxs);
        VectorCopy (ray.m_Extents, trace_extents);
        trace_ispoint = ray.m_IsRay;


        if (!ray.m_IsSwept)
        {
                // check for position test special case
                CM_UnsweptBoxTrace( pBSPData, ray, headnode, brushmask );
        }
        else
        {
                // general sweeping through world
                CM_RecursiveHullCheck( pBSPData, headnode, 0, 1, trace_start, trace_end );
        }
        // Compute the trace start + end points
        if (computeEndpt)
        {
                CM_ComputeTraceEndpoints( ray, trace_trace );
        }

        // Copy off the results
        tr = trace_trace;
        EndCheckCount();
        InternalAssert( !ray.m_IsRay || tr.allsolid || (tr.fraction >= tr.fractionleftsolid) );
}

void SetTraceEntity( ICollideable *pCollideable, trace_t *pTrace )
{
	if ( !pTrace->DidHit() )
		return;

	IHandleEntity *pHandleEntity = pCollideable->GetEntityHandle();
	if ( !StaticPropMgr()->IsStaticProp( pHandleEntity ) )
	{
		pTrace->m_pEnt = (CBaseEntity*)(pHandleEntity);
	}
	else
	{
		// For static props, point to the world, hitbox is the prop index
		pTrace->m_pEnt = (CBaseEntity*)(sv.edicts->GetIServerEntity());
		pTrace->hitbox = StaticPropMgr()->GetStaticPropIndex( pHandleEntity ) + 1;
	}
}

//-----------------------------------------------------------------------------
// A version that simply accepts a ray (can work as a traceline or tracehull)
//-----------------------------------------------------------------------------
void TraceRay( const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace)
{
        CTraceFilterHitAll traceFilter;
        if ( !pTraceFilter )
        {
                pTraceFilter = &traceFilter;
        }

        // Gather statistics.
        g_EngineStats.IncrementCountedStat( ENGINE_STATS_NUM_TRACE_LINES, 1 );
        MEASURE_TIMED_STAT( ENGINE_STATS_TRACE_LINE_TIME );

        CM_ClearTrace( pTrace );

        // Collide with the world.
        if ( pTraceFilter->GetTraceType() != TRACE_ENTITIES_ONLY )
        {
                ICollideable *pCollide = GetWorldCollideable();

                // Make sure the world entity is unrotated
                // FIXME: BAH! The !pCollide test here is because of
                // CStaticProp::PrecacheLighting.. it's occurring too early
                // need to fix that later
                InternalAssert(!pCollide || pCollide->GetCollisionOrigin() == vec3_origin );
                InternalAssert(!pCollide || pCollide->GetCollisionAngles() == vec3_angle );

                CM_BoxTrace( ray, 0, fMask, true, *pTrace );
                SetTraceEntity( pCollide, pTrace );

                // Blocked by the world.
                if ( pTrace->fraction == 0 )
                        return;

                // Early out if we only trace against the world
                if ( pTraceFilter->GetTraceType() == TRACE_WORLD_ONLY )
                        return;
        }

        // Save the world collision fraction.
        float flWorldFraction = pTrace->fraction;

        // Create a ray that extends only until we hit the world
        // and adjust the trace accordingly
        Ray_t entityRay = ray;
        entityRay.m_Delta *= pTrace->fraction;

        // We know this is safe because if pTrace->fraction == 0
        // we would have exited above
        pTrace->fractionleftsolid /= pTrace->fraction;
        pTrace->fraction = 1.0;

        // Collide with entities along the ray
        // FIXME: Hitbox code causes this to be re-entrant for the IK stuff.
        // If we could eliminate that, this could be static and therefore
        // not have to reallocate memory all the time
        CEntitiesAlongRay enumerator;
        enumerator.Reset();
        SpatialPartition()->EnumerateElementsAlongRay( SpatialPartitionMask(), entityRay, false, &enumerator );

        bool bNoStaticProps = pTraceFilter->GetTraceType() == TRACE_ENTITIES_ONLY;
        bool bFilterStaticProps = pTraceFilter->GetTraceType() == TRACE_EVERYTHING_FILTER_PROPS;

        trace_t tr;
        ICollideable *pCollideable;
        const char *pDebugName;
        int nCount = enumerator.m_EntityHandles.Count();
        for ( int i = 0; i < nCount; ++i )
        {
                // Generate a collideable
                IHandleEntity *pHandleEntity = enumerator.m_EntityHandles[i];
                HandleEntityToCollideable( pHandleEntity, &pCollideable, &pDebugName );

                // Check for error condition
                if ( !IsSolid( pCollideable->GetSolid(), pCollideable->GetSolidFlags() ) )
                {
                        char temp[1024];
                        Q_snprintf(temp, sizeof( temp ), "%s in solid list (not solid)\n", pDebugName );
                        Sys_Error (temp);
                }

                if ( !StaticPropMgr()->IsStaticProp( pHandleEntity ) )
                {
                        if ( !pTraceFilter->ShouldHitEntity( pHandleEntity, fMask ) )
                                continue;
                }
                else
                {
                        // FIXME: Could remove this check here by
                        // using a different spatial partition mask. Look into it
                        // if we want more speedups here.
                        if ( bNoStaticProps )
                                continue;

                        if ( bFilterStaticProps )
                        {
                                if ( !pTraceFilter->ShouldHitEntity( pHandleEntity, fMask ) )
                                        continue;
                        }
                }

                ClipRayToCollideable( entityRay, fMask, pCollideable, &tr );

                // Make sure the ray is always shorter than it currently is
                ClipTraceToTrace( tr, pTrace );

                // Stop if we're in allsolid
                if (pTrace->allsolid)
                        break;
        }

        // Fix up the fractions so they are appropriate given the original
        // unclipped-to-world ray
        pTrace->fraction *= flWorldFraction;
        pTrace->fractionleftsolid *= flWorldFraction;

#ifdef _DEBUG
        Math::Vector3D vecOffset, vecEndTest;
        VectorAdd( ray.m_Start, ray.m_StartOffset, vecOffset );
        VectorMA( vecOffset, pTrace->fractionleftsolid, ray.m_Delta, vecEndTest );
        InternalAssert( VectorsAreEqual( vecEndTest, pTrace->startpos, 0.1f ) );
        VectorMA( vecOffset, pTrace->fraction, ray.m_Delta, vecEndTest );
        InternalAssert( VectorsAreEqual( vecEndTest, pTrace->endpos, 0.1f ) );
//      InternalAssert( !ray.m_IsRay || pTrace->allsolid || pTrace->fraction >= pTrace->fractionleftsolid );
#endif

        if ( !ray.m_IsRay )
        {
                // Make sure no fractionleftsolid can be used with box sweeps
                VectorAdd( ray.m_Start, ray.m_StartOffset, pTrace->startpos );
                pTrace->fractionleftsolid = 0;

#ifdef _DEBUG
                pTrace->fractionleftsolid = VEC_T_NAN;
#endif
        }
}
#endif	// if 0