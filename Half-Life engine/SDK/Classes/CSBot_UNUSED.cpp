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
// CSBot.cpp
//
// Class: CSBot
//
// Version: $ID:$
//

#include <Core.hpp>

namespace HalfLifeEngine
{
namespace SDK
{
namespace Classes
{
const bool CCSBot::Initialize (Profile *const profile)
{
	CBot::Initialize (profile);

	m_isDead = false;
	m_morale = MoraleType_Positive;
	m_UNKNOWN99 = 0u;
	m_currentSamplingNode = NULL;
	m_combatRange = g_engfuncs.pfnRandomFloat (325.0f, 425.0f);
	m_name[0u] = '\0';
	m_safeEndTime = m_profile->aggression * 5.0f + 15.0f;

	ResetValues ();
	StartNormalProcess ();

	return true;
}

void CCSBot::ResetValues (void)
{
	m_chatter.Reset ();
	m_gameState.Reset ();
	m_touchPlayerEntity = NULL;
	m_touchPlayerTimestamp = 0.0f;
	m_hurryTimer.Invalidate ();
	m_isStucked = false;
	m_stuckTimestamp = 0.0f;
	m_wiggleMoveInDirectionGlobalDuration = 0.0f;
	m_wiggleJumpTimestamp = 0.0f;
	m_currentPathInformationNumber = 0u;
	m_currentPathIndex = 0u;
	m_currentPathIndexSetTimestamp = 0.0f;
	m_currentArea = NULL;
	m_lastKnownArea = NULL;
	m_isFriendInTheWayNextCallTimer.Invalidate ();
	m_isFriendInTheWay = false;
	m_canResetStuckMonitor = false;
	m_disposition = DispositionType_EngageAndInvestigate;
	m_enemy = NULL;
	m_isThrowingGrenade = false;
	m_isSafe = true;
	m_nearbyEnemyCount = 0u;
	m_enemyPlace = UNDEFINED_PLACE;
	m_nearbyFriendCount = 0u;
	m_closestVisibleFriend = NULL;
	m_closestVisibleHumanFriend = NULL;

	for (unsigned char index (0u); index < Constants::MaximumClients; ++index)
	{
		m_alivePlayersInfo[index].setTimestamp = 0.0f;
		m_alivePlayersInfo[index].isEnemy = false;
	}

	m_isEnemyVisible = false;
	m_enemyVisibleBodyParts = VisiblePartType_None;
	m_lastSawEnemyTimestamp = 0.0f;
	m_firstSawEnemyTimestamp = 0.0f;
	m_acquiredCurrentEnemyTimestamp = 0.0f;
	m_isEnemyDead = true;
	m_attacker = NULL;
	m_attackedTimestamp = 0.0f;
	m_enemyDeathTimestamp = 0.0f;
	m_lastVictimID = 0u;
	m_isAimingAtEnemy = false;
	m_fireWeaponAtEnemyNextCallTimestamp = 0.0f;
	m_weaponEquipTimestamp = -1.0f;
	m_isFollowing = false;
	m_followingTarget = NULL;
	m_followingStartTime = 0.0f;
	m_nextAutoFollowTime = 0.0f;
	m_updateReactionQueue_UNKNOWN0 = 0u;
	m_updateReactionQueue_UNKNOWN1 = 0u;
	m_currentRecognizedEnemiesQueueIndex = 0u;
	m_bomber = NULL;
	m_nextLookAtHidingApproachPointGlobalTime = 0.0f;
	m_inhibitLookAroundGlobalTime = 0.0f;
	m_lookAnglePitch = 0.0f;
	m_lookAnglePitchUNKNOWN = 0.0f;
	m_lookAngleYaw = 0.0f;
	m_lookAngleYawUNKNOWN = 0.0f;
	m_setAimOffsetNextCallTimestamp = 0.0f;
	m_attackBeginTimestamp = 0.0f;
	m_lookAction = LookAction_None;
	m_currentSpotEncounter = NULL;
	m_nextLookAtCurrentSpotEncounterGlobalTime = 0.0f;
	m_updatePeripheralVisionCallTimestamp = 0.0f;
	m_distancesToPreviousOriginSinceStuckCurrentIndex0 = 0u;
	m_distancesToPreviousOriginSinceStuckCurrentIndex1 = 0u;
	m_previousOrigin = m_variables != NULL ? GetOrigin () : Math::Vector3D::ZeroValue;
	m_currentRadioCommand = Constants::GameEventType_INVALID;
	m_receiveRadioMessageTimestamp = 0.0f;
	m_sendRadioMessageTimestamp = 0.0f;
	m_radioMessageSender = NULL;
	m_noisePosition = Math::Vector3D::ZeroValue;
	m_voiceFeedbackGlobalDuration = 0.0f;
	m_hostageEscortCount = 0u;
	m_updateHostageEscortCountTimestamp = 0.0f;
	m_noiseHearGlobalTimeDuration = 0.0f;
	m_shouldInvestigateNoiseCallTimestamp = 0.0f;
	m_isUNKNOWN96 = false;
	m_setStateTimestamp = 0.0f;
	SetTask (TaskType_SeekAndDestroy);
	m_approachPointsNumber = 0u;
	m_approachPointsCenter = Math::Vector3D::ZeroValue;
	m_hidingSpotsToCheckNumber = 0u;
	m_isUNKNOWN0 = false;

	StandUp ();
	Run ();

	m_forceRunTimer.Invalidate ();
	m_currentLadder = NULL;
	m_computePathNextCallTimer.Invalidate ();
	m_huntState.m_huntArea = NULL;

	if (m_isDead)
		DecreaseMorale ();

	m_isDead = false;
	m_isRogue = false;
	m_surpriseTime = 0.0f;
	m_surprisedTime = 0.0f;
	m_goalEntity = NULL;
	m_touchPlayerEntity = NULL;
	m_enemy = NULL;

	StopAttacking (m_touchPlayerEntity);
	Idle ();
}

void CCSBot::SpawnBot (void)
{
	TheBots->ValidateMapData ();

	ResetValues ();
	strcpy (m_name, GetNetName ());
	Buy ();

	m_pfnTouch = BotTouch;

	if (TheNavAreaList.IsEmpty () && !CCSBotManager::m_isLearningMap)
	{
		CCSBotManager::m_isLearningMap = true;

		StartLearnProcess ();
	}
}

void CCSBot::RoundRespawn (void)
{
	BasePlayer::RoundRespawn ();

	EndVoiceFeedback (true);
}

void CCSBot::Disconnect (void)
{
	EndVoiceFeedback (true);

	if (m_currentGenerationState == GenerationStateType_None)
	{
		g_engfuncs.pfnMessageBegin (Constants::MSG_ALL, gmsgBotProgress, NULL, NULL);
		g_engfuncs.pfnWriteByte (Constants::ProgressBarType_Remove);
		g_engfuncs.pfnMessageEnd ();
	}
}

void CCSBot::SetPathIndex (const PathNodeArray_t::IndexType_t newPathIndex)
{
	InternalAssert (HasRoute ());
	InternalAssert (newPathIndex > 0u);

	const PathNodeArray_t::IndexType_t lastPathIndex (m_path.GetLastElementIndex ());

	m_currentPathIndex = newPathIndex > lastPathIndex ? lastPathIndex : newPathIndex;
	m_currentPathIndexSetTimestamp = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();

	if (m_path[m_currentPathIndex].ladder != NULL)
	{
		SetupLadderMovement ();

		return;
	}

	m_currentLadder = NULL;
	m_currentSpotEncounter = m_currentPathIndex >= m_path.GetElementNumber ()/*! @note DO WE NEED THIS CHECK?!?!??!?!?!*/ || m_currentPathIndex < 2u ? NULL : m_path[m_currentPathIndex - 1u].area->GetSpotEncounter (m_path[m_currentPathIndex - 2u].area, m_path[m_currentPathIndex].area);
}

void CCSBot::BuildTrivialPath (const Math::Vector3D &destination)
{
	m_currentPathIndex = 1u;
	m_currentPathInformationNumber = 2u;

	m_path[0u].area = m_lastKnownArea;
	m_path[0u].point = pev->origin;
	m_path[0u].point.z = m_lastKnownArea->GetZ (pev->origin);
	m_path[0u].ladder = NULL;
	m_path[0u].traverseType = NUM_TRAVERSE_TYPES;

	m_path[1u].area = m_lastKnownArea;
	m_path[1u].point = destination;
	m_path[1u].point.z = m_lastKnownArea->GetZ (destination);
	m_path[1u].ladder = NULL;
	m_path[1u].traverseType = NUM_TRAVERSE_TYPES;

	m_currentSpotEncounter = NULL;
	m_currentPathIndexSetTimestamp = gpGlobals->time;
	m_currentLadder = NULL;
	m_goalOrigin = destination;
}

void CCSBot::DestroyPath (void)
{
	m_currentPathInformationNumber = 0u;
	m_currentLadder = NULL;
}
const bool CCSBot::ComputePath (NavigationArea *const destination, const Math::Vector3D *const destinationVector, const NavigationMesh::RouteType_t routeType)
{
	if (!m_computePathNextCallTimer.IsElapsed ())
		return;

	m_computePathNextCallTimer.Start (g_engfuncs.pfnRandomFloat (0.4f, 0.6f));

	DestroyPath ();

	if (m_lastKnownArea == NULL)
		return false;

	// determine actual goal position
	Math::Vector3D actualGoalPos;

	if (destination != NULL)
	{
		if (destinationVector == NULL)
			actualGoalPos = destination->GetCenter ();
		else
			actualGoalPos = *destinationVector;

		actualGoalPos.z = destination->GetZ ();
	}
	else
	{
		if (destinationVector == NULL)
			return false;

		actualGoalPos = *destinationVector;

		NavigationMesh::GetGroundHeight (actualGoalPos, &actualGoalPos.z);
	}

	if (m_lastKnownArea == destination)
		goto Label_BuildTrivialPathAndReturn;

	NavigationArea *closestArea;

	NavAreaBuildPath (m_lastKnownArea, destination, destinationVector, PathCost (this, routeType), &closestArea);

	unsigned short areasNumberInPath (0u);
	NavigationArea *area (closestArea);

	for (/* Empty */; area != NULL; area = area->GetParent ())
		++areasNumberInPath;

	if (areasNumberInPath > MaximumPathNodesNumber - 1u)
		areasNumberInPath = MaximumPathNodesNumber - 1u;
	else
	{
		if (areasNumberInPath == 0u)
			return false;

		if (areasNumberInPath == 1u)
		{
			Label_BuildTrivialPathAndReturn:
			BuildTrivialPath (actualGoalPos);

			return true;
		}
	}

	m_currentPathInformationNumber = areasNumberInPath;

	// Fill route....
	for (area = closestArea; areasNumberInPath > 0u && area != NULL; area = area->GetParent ())
	{
		--areasNumberInPath;

		route[areasNumberInPath].area = area;
		route[areasNumberInPath].traverseType = area->GetParentHow ();
	}

	if (!ComputePathPositions ())
	{
		PrintIfWatched ("Error building path\n");

		DestroyPath ();

		return false;
	}

	if (destinationVector == NULL)
	{
		switch (m_path[m_currentPathInformationNumber - 1u].traverseType)
		{
			case NORTH:
			case SOUTH:
				actualGoalPos.x = m_path[m_currentPathInformationNumber - 1u].point.x;
				actualGoalPos.y = closestArea->GetCenter ().y;

				break;

			case EAST:
			case WEST:
				actualGoalPos.x = closestArea->GetCenter ().x;
				actualGoalPos.y = m_path[m_currentPathInformationNumber - 1u].point.y;

				break;
		}

		NavigationMesh::GetGroundHeight (actualGoalPos, &actualGoalPos.z);
	}

	m_path[m_currentPathInformationNumber].area = closestArea;
	m_path[m_currentPathInformationNumber].point = actualGoalPos;
	m_path[m_currentPathInformationNumber].ladder = NULL;
	m_path[m_currentPathInformationNumber].traverseType = NUM_TRAVERSE_TYPES;

	++m_currentPathInformationNumber;

	m_currentPathIndex = 1u;
	m_currentPathIndexSetTimestamp = gpGlobals->time;
	m_goalOrigin = m_path[1u].point;
	m_currentSpotEncounter = NULL;

	if (m_path[1u].ladder == NULL)
		m_currentLadder = NULL;
	else
		SetupLadderMovement ();

	return true;
}

const bool CCSBot::ComputePathPositions (void)
{
	/// @todo CHECK ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (m_currentPathInformationNumber == 0u)
		return;

	m_path[0u].point = m_path[0u].area->GetCenter ();
	m_path[0u].ladder = NULL;
	m_path[0u].traverseType = NUM_TRAVERSE_TYPES;

	if (m_currentPathInformationNumber < 2u)
		return;

	for (unsigned short index (1u); index < m_currentPathInformationNumber; ++index)
	{
		const PathNode_t &path (m_path[index - 1u]);
		const PathNode_t &nextPath (m_path[index]);

		if (nextPath.traverseType <= GO_WEST)
		{
			nextPath.ladder = NULL;

			nextPath.point = path.area->GetClosestPointInPortal2D (nextPath.area, nextPath.traverseType, path.point);

			NavigationMesh::AddDirectionVector (nextPath.point, nextPath.traverseType, 5.0f);

			nextPath.point.z = path.area->GetZ (nextPath.point);

			if (nextPath.area->IsConnected (path.area))
				continue;

			const Math::Vector2D direction (NavigationMesh::GetDirectionToVector2D (nextPath.traverseType) * NavigationMesh::GenerationStepSize);

			nextPath.point += direction;

			if (m_currentPathInformationNumber >= MaximumPathNodesNumber - 1u)
				continue/*SHOULD break*/;

			for (unsigned short index2 (m_currentPathInformationNumber); index2 > index; --index2)
				m_path[index2] = m_path[index2 - 1u];

			++m_currentPathInformationNumber;
			++index;

			m_path[index].point = nextPath.point + direction;	/// AGAIN APPENDING 'direction' TO 'nextPath.point'?!?!?!?!?!?!?!?!?!??!?!?!?!??!?!?!???
			m_path[index].point.z = nextPath.area->GetZ (m_path[index].point);

			continue;
		}

		if (nextPath.traverseType == GO_LADDER_UP)
		{
			const NavigationMesh::NavLadderConnectArray &ladderList (path.area->GetConnectedLadders (NavigationMesh::NavigationLadder::Direction_Up));
			NavigationMesh::NavLadderConnectArray::IndexType_t connectLadderIndex (0u);

			for (/* Empty */; connectLadderIndex < ladderList.GetElementNumber (); ++connectLadderIndex)
			{
				const NavigationLadder *const connectedLadder (ladderList[connectLadderIndex]);

				if (connectedLadder->GetConnectedArea (NavigationMesh::NavigationLadder::ConnectionType_TopForward) != nextPath.area)
					continue;

				if (connectedLadder->GetConnectedArea (NavigationMesh::NavigationLadder::ConnectionType_TopLeft) != nextPath.area && connectedLadder->GetConnectedArea (NavigationMesh::NavigationLadder::ConnectionType_TopRight) != nextPath.area)
					continue;

				nextPath.ladder = connectedLadder;
				nextPath.point = connectedLadder->GetBottom ();

				NavigationMesh::AddDirectionVector (nextPath.point, connectedLadder->GetDirection (), Constants::HumanWidth);
///				nextPath.point += connectedLadder->GetNormal () * Constants::HumanWidth;	/// @note FROM NEW SOURCE ENGINE INSTEAD OF LINE ABOVE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

				break;
			}

			if (connectLadderIndex == ladderList.GetElementNumber ())
				goto Label_UnableToFindLadderInPath;

			continue;
		}

		if (nextPath.traverseType == GO_LADDER_DOWN)
		{
			const NavigationMesh::NavLadderConnectArray &ladderList (path.area->GetConnectedLadders (NavigationMesh::NavigationLadder::Direction_Down));
			NavigationMesh::NavLadderConnectArray::IndexType_t connectLadderIndex (0u);

			for (/* Empty */; connectLadderIndex < ladderList.GetElementNumber (); ++connectLadderIndex)
			{
				const NavigationLadder *const connectedLadder (ladderList[connectLadderIndex]);

				if (connectedLadder->GetConnectedArea (NavigationMesh::NavigationLadder::ConnectionType_Bottom) != nextPath.area)
					continue;

				nextPath.ladder = connectedLadder;
				nextPath.point = connectedLadder->GetTop ();

				NavigationMesh::AddDirectionVector (nextPath.point, NavigationMesh::GetOppositeDirection (connectedLadder->GetDirection ()), Constants::HumanWidth);
///				nextPath.point -= connectedLadder->GetNormal () * Constants::HumanWidth;	/// @note FROM NEW SOURCE ENGINE INSTEAD OF LINE ABOVE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

				break;
			}

			if (connectLadderIndex == ladderList.GetElementNumber ())
			{
				Label_UnableToFindLadderInPath:
				PrintIfWatched ("ERROR: Can't find ladder in path\n");

				return false;
			}

			continue;
		}
	}

	return true;
}

void CCSBot::MoveTowardsPosition (const Math::Vector2D &position)
{
	/// @todo CHECK ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	const Math::Vector2D direction (position - GetOrigin ());
	const Math::Vector2D directionNormalized (direction.Normalize ());

	if ((m_lastKnownArea == NULL || !m_lastKnownArea->HasAttributes (NAV_MESH_NO_JUMP)/*! @note This should be some inlined function!!!! */) && !IsOnLadder () && !m_isUNKNOWN0)
	{
		Math::Vector3D point (GetOrigin () + directionNormalized * 80.0f + Math::Vector3D (0.0f, 0.0f, 36.0f));
		bool UNKNOWN (false);
		float height;

		if (m_doRun)
		{
			Math::Vector3D normal;

			if (NavigationMesh::GetSimpleGroundHeight (point, &height, &normal))
			{
				if (m_lastKnownArea != NULL && m_lastKnownArea->IsOverlapping (point))	/// @note This should be some inlined function!!!!
				{
					const float z (m_lastKnownArea->GetZ (point));

					if (height < z)
						height = z;
				}

				if (normal.z > 0.9f)
					UNKNOWN = DiscontinuityJump (height, true, false);
			}
		}

		if (!UNKNOWN)
		{
			point = GetOrigin () + directionNormalized * 30.0f + Math::Vector3D (0.0f, 0.0f, 36.0f);

			if (NavigationMesh::GetSimpleGroundHeight (point, &height))
			{
				if (m_lastKnownArea != NULL && m_lastKnownArea->IsOverlapping (point))	/// @note This should be some inlined function!!!!
				{
					const float z (m_lastKnownArea->GetZ (point));

					if (height < z)
						height = z;
				}

				UNKNOWN = DiscontinuityJump (height, false, false);
			}

			if (!UNKNOWN)
			{
				point = GetOrigin () + directionNormalized * 10.0f + Math::Vector3D (0.0f, 0.0f, 36.0f);

				if (NavigationMesh::GetSimpleGroundHeight (point, &height))
				{
					if (m_lastKnownArea != NULL && m_lastKnownArea->IsOverlapping (point))	/// @note This should be some inlined function!!!!
					{
						const float z (m_lastKnownArea->GetZ (point));

						if (height < z)
							height = z;
					}

					DiscontinuityJump (height, true, true);
				}
			}
		}
	}

	const float yawAngle (GetViewAngles ().yaw);
	const float cosine (BotCOS (yawAngle)), sine (BotSIN (yawAngle));
	const BaseNavigator::Speed_t speed
	(
		cosine * directionNormalized.x + sine * directionNormalized.y,
		cosine * directionNormalized.y + -sine * directionNormalized.x
	);

	if (speed.move > 0.25f)
		MoveForward ();
	else if (speed.move < -0.25f)
		MoveBackward ();

	if (!m_touchPlayerEntity)
	{
		if (speed.strafe >= 0.25f)
			StrafeLeft ();
		else if (speed.strafe <= -0.25f)
			StrafeRight ();
	}
}

const bool CCSBot::IsStraightLinePathWalkable (const Math::Vector3D &origin)
{
	return true;
}
const unsigned short CCSBot::FindOurPositionOnPath (Math::Vector3D *const origin, const bool isUNKNOWN)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (m_currentPathInformationNumber == 0u)
		return -1;
/*
	const Math::Vector3D botOrigin (GetOrigin () - Math::Vector3D (0.0f, 0.0f, m_doCrouch ? 18.0f : 36.0f));

	if (isUNKNOWN)
	{
		v9 = m_currentPathIndex;
		v8 = v9 - 3;
		if (v9 - 3 < 1)
			v8 = 1;
		v10 = v9 + 3;
		if (v9 + 3 > m_currentPathInformationNumber)
			v10 = m_currentPathInformationNumber;
	}
	else
	{
		v10 = m_currentPathInformationNumber;
		v8 = 1;
	}

	Math::Vector3D direction;
	float directionLength;
	float v18;

	for (; v8 < v10; ++v8)
	{
		direction = m_path[v8].point - m_path[v8 - 1u].point;
		directionLength = direction.GetLength ();
		direction /= directionLength;	// Normalize.
		v18 = m_path[v8 - 1u].point.GetDistanceSquared (botOrigin);

		if (v18 > 0.0f)
		{
			if (directionLength > v18)
		}
		else
		{
			
		}
	}

	return -1;
*/
	v42 = -16.0;
	v43 = -16.0;
	v44 = -18.0;
	Vector__operator=Vector((int)&v52, (int)&v42);
	v5 = Vector__operator=xyz((int)&v70, -16.0f, -16.0f, -36.0f);
	Vector__operator=Vector((int)&v55, v5);
	v6 = *(_DWORD *)(this + 4);
	if (m_doCrouch)
		v7 = v54;
	else
		v7 = v57;
	v41 = v7 + *(float *)(v6 + 16);
	v47 = *(float *)(v6 + 8);
	v48 = *(float *)(v6 + 12);
	v42 = v47;
	v43 = v48;
	v44 = v41 + 36.0;
	Vector__operator=Vector((int)&v49, (int)&v42);
	v65 = v50;
	v64 = v49;
	v66 = v51;
	v45 = 1.0e10;
	v46 = -1;
	if (isUNKNOWN)
	{
		v8 = m_currentPathIndex;
		v9 = v8 - 3;
		if (v8 - 3 < 1)
			v9 = 1;
		v10 = v8 + 3;
		if (v8 + 3 > m_currentPathInformationNumber)
			v10 = m_currentPathInformationNumber;
	}
	else
	{
		v10 = m_currentPathInformationNumber;
		v9 = 1;
	}
	v11 = v9;
	if (v9 >= v10)
		return -1;

	v12 = m_path[v9 - 1u].point.z;
	do
	{
		v49 = *(float *)(v12 + 16) - *(float *)(v12 - 8);
		v50 = *(float *)(v12 + 20) - *(float *)(v12 - 4);
		v51 = *(float *)(v12 + 24) - *(float *)v12;
		Vector__operator=Vector((int)&v58, (int)&v49);
		v35 = v58;
		v36 = v59;
		v37 = v60;
		v13 = VectorLength((int)&v35);
		v34 = v13;
		if (v13 <= 0.0)
		{
			v35 = 0.0;
			v36 = 0.0;
			v37 = 1.0;
		}
		else
		{
			v35 = v35 / v34;
			v36 = v36 / v34;
			v37 = v37 / v34;
		}
		*(float *)&v14 = v41 - *(float *)v12;
		*(float *)&v15 = v48 - *(float *)(v12 - 4);
		*(float *)&v16 = v47 - *(float *)(v12 - 8);
		v17 = Vector__operator=xyz((int)&v71, v16, v15, v14);
		Vector__operator=Vector((int)&v67, v17);
		v18 = v69 * v37 + v68 * v36 + v67 * v35;
		v33 = v18;
		if (v18 > 0.0)
		{
			if (v34 > (double)v33)
			{
				v19 = sub_10009A40((int)&v35, (int)&v74, v33);
				Vector__operator=Vector((int)&v61, v19);
				*(float *)&v20 = v63 + *(float *)v12;
				*(float *)&v21 = v62 + *(float *)(v12 - 4);
				*(float *)&v22 = v61 + *(float *)(v12 - 8);
				v23 = Vector__operator=xyz((int)&v72, v22, v21, v20);
				Vector__operator=Vector((int)&v55, v23);
				v38 = v55;
				v39 = v56;
				v40 = v57;
			}
			else
			{
				LODWORD(v38) = *(_DWORD *)(v12 + 16);
				LODWORD(v39) = *(_DWORD *)(v12 + 20);
				LODWORD(v40) = *(_DWORD *)(v12 + 24);
			}
		}
		else
		{
			LODWORD(v38) = *(_DWORD *)(v12 - 8);
			LODWORD(v39) = *(_DWORD *)(v12 - 4);
			LODWORD(v40) = *(_DWORD *)v12;
		}
		*(float *)&v24 = v40 - v41;
		*(float *)&v25 = v39 - v48;
		*(float *)&v26 = v38 - v47;
		v27 = Vector__operator=xyz((int)&v73, v26, v25, v24);
		Vector__operator=Vector((int)&v42, v27);
		v28 = v44 * v44 + v43 * v43 + v42 * v42;
		if (v28 < v45)
		{
			v54 = 36.0;
			v52 = 0;
			v53 = 0;
			v30 = v52 + v38;
			v31 = v53 + v39;
			v32 = v54 + v40;
			Vector__operator=Vector((int)&v70, v30);
			if (IsWalkableTraceLineClear ((int)&v64, (int)&v70, 3u/*WALK_THRU_DOORS | WALK_THRU_BREAKABLES - NOT SHURE!!*/) && IsStraightLinePathWalkable ((int)&v38))
			{
				v29 = v28;
				v45 = v29;
				if (origin != NULL)
				{
					v31 = v39;
					origin.x = LODWORD(v38);
					v32 = v40;
					origin.y = LODWORD(v31);
					origin.z = LODWORD(v32);
				}
				v46 = v11 - 1;
			}
		}
		++v11;
		v12 += 24;
	} while (v11 < v10);

	return v46;
}
const unsigned short CCSBot::FindPathPoint (const float distance, Math::Vector3D &origin, unsigned short *const outCurrentPathIndex)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	Math::Vector3D ourPosition;
	const unsigned short currentPathIndex (FindOurPositionOnPath (&ourPosition, true));

	if (outCurrentPathIndex != NULL)
		*outCurrentPathIndex = currentPathIndex;

	if (static_cast <const short> (currentPathIndex) < 0)
		return m_currentPathIndex;

	if (m_doCrouch)
	{
/*
		unsigned int resultIndex (currentPathIndex + 1u);

		for (origin = GetPathPosition (resultIndex); resultIndex < m_currentPathInformationNumber; ++resultIndex)
		{
		}
*/
		unsigned int resultIndex (currentPathIndex + 1u);

		if (resultIndex >= m_currentPathInformationNumber)
			resultIndex = m_currentPathInformationNumber - 1u;

		origin = GetPathPosition (resultIndex);

		for (; ; ++resultIndex)
		{
			if (ourPosition.AsVector2D ().GetDistanceSquared (origin) >= MATH_GET_SQUARED (20.0f))
				break;

			if (++resultIndex >= m_currentPathInformationNumber)
				return m_currentPathInformationNumber - 1u;

			origin = GetPathPosition (resultIndex);
		}
	}
	else
	{
		if (currentPathIndex < m_currentPathInformationNumber - 1)
		{
			v11 = this + 24 * currentPathIndex + 3104;
			do
			{
				v12 = *(float *)(v11 - 8) - ourPosition.x;
				LODWORD(v13) = *(_DWORD *)v11;
				LODWORD(v101) = *(_DWORD *)(v11 - 4);
				v102 = v13;
				*(float *)&v111 = v12;
				*(float *)&v112 = v101 - ourPosition.y;
				*(float *)&v113 = v13 - ourPosition.z;
				Vector__operator=Vector((int)&v108, (int)&v111);
				if (v108 * v108 + v109 * v109 >= MATH_GET_SQUARED (20.0f))
					break;
				++currentPathIndex;
				v11 += 24;
			}
			while (currentPathIndex < m_currentPathInformationNumber - 1);
		}
		if (currentPathIndex > m_currentPathIndex
			&& currentPathIndex < m_currentPathInformationNumber
			&& (*(_DWORD *)(this + 24 * currentPathIndex + 3084) || *(_BYTE *)(*(_DWORD *)(this + 24 * currentPathIndex + 3064) + 40) & 2))
		{
			v14 = this + 8 * (3 * currentPathIndex + 384);
			origin.x = *(_DWORD *)v14;
			origin.y = *(_DWORD *)(v14 + 4);
			origin.z = *(_DWORD *)(v14 + 8);
			return currentPathIndex;
		}
		v15 = m_currentPathInformationNumber;
		resultIndex = currentPathIndex + 1;
		if (resultIndex < v15 || (resultIndex = v15 - 1, v15 - 1 < v15))
		{
			if (*(_DWORD *)(this + 24 * resultIndex + 3084) || *(_BYTE *)(*(_DWORD *)(this + 24 * resultIndex + 3064) + 40) & 2)
			{
				v16 = this + 8 * (3 * resultIndex + 384);
				origin.x = *(_DWORD *)v16;
				origin.y = *(_DWORD *)(v16 + 4);
				origin.z = *(_DWORD *)(v16 + 8);
				return resultIndex;
			}
		}
		v17 = 3 * resultIndex + 381;
		v18 = 3 * resultIndex + 384;
		v19 = *(float *)(this + 8 * v18) - *(float *)(this + 8 * v17);
		v20 = this + 8 * v17;
		v21 = this + 8 * v18;
		v100 = v19;
		v101 = *(float *)(v21 + 4) - *(float *)(v20 + 4);
		v102 = *(float *)(v21 + 8) - *(float *)(v20 + 8);
		Vector__operator=Vector((int)&v105, (int)&v100);
		v22 = sqrt(*(float *)&v105 * *(float *)&v105 + *(float *)&v107 * *(float *)&v107 + *(float *)&v106 * *(float *)&v106);
		if (v22 <= 0.0)
		{
			v123 = 0.0;
			v124 = 0.0;
			v125 = 1.0;
		}
		else
		{
			v123 = *(float *)&v105 / v22;
			v124 = *(float *)&v106 / v22;
			v125 = *(float *)&v107 / v22;
		}
		v100 = -16.0;
		v101 = -16.0;
		v102 = -18.0;
		Vector__operator=Vector((int)&v127, (int)&v100);
		v23 = Vector__operator=xyz((int)&v145, -1048576000, -1048576000, -1039138816);
		Vector__operator=Vector((int)&v120, v23);
		v24 = *(_DWORD *)(this + 4);
		if (m_doCrouch)
			v25 = v128;
		else
			v25 = *(float *)&v122;
		v103 = v25 + *(float *)(v24 + 16);
		v108 = *(float *)(v24 + 8);
		v109 = *(float *)(v24 + 12);
		v100 = v108;
		v101 = v109;
		v102 = v103 + 36.0;
		Vector__operator=Vector((int)&v111, (int)&v100);
		v138 = v111;
		v139 = v112;
		v114 = v123;
		v26 = m_currentPathInformationNumber;
		v140 = v113;
		v115 = v124;
		v27 = resultIndex;
		v126 = 0.0;
		v99 = 1;
		v116 = v125;
		v98 = 0;
		v96 = resultIndex;
		if (resultIndex >= v26)
			goto LABEL_102;
		v28 = this + 24 * resultIndex + 3056;
		while (1)
		{
			LODWORD(v29) = *(_DWORD *)(v28 + 20);
			LODWORD(v117) = *(_DWORD *)(v28 + 16);
			LODWORD(v30) = *(_DWORD *)(v28 + 24);
			v31 = v117 - *(float *)(v28 - 8);
			v118 = v29;
			v119 = v30;
			v152 = v31;
			v153 = v29 - *(float *)(v28 - 4);
			v154 = v30 - *(float *)v28;
			Vector__operator=Vector((int)&v146, (int)&v152);
			v134 = v148;
			v132 = v146;
			v133 = v147;
			v32 = VectorLength((int)&v132);
			if (v32 == 0.0)
			{
				v149 = 0;
				v150 = 0;
				v151 = 1065353216;
				v95 = (int)&v149;
			}
			else
			{
				v33 = 1.0 / v32;
				*(float *)&v34 = v134 * v33;
				*(float *)&v35 = v133 * v33;
				*(float *)&v36 = v33 * v132;
				v95 = Vector__operator=xyz((int)&v164, v36, v35, v34);
			}
			Vector__operator=Vector((int)&v105, v95);
			v141 = v105;
			v142 = v106;
			v143 = v107;
			if (*(float *)&v107 * v125 + *(float *)&v106 * v124 + *(float *)&v105 * v123 < 0.0)
				break;
			if (*(float *)&v107 * v116 + *(float *)&v106 * v115 + *(float *)&v105 * v114 < 0.5)
			{
				v98 = 1;
				break;
			}
			v129 = v117;
			LODWORD(v115) = v106;
			v130 = v118;
			LODWORD(v114) = v141;
			v131 = v119 + 36.0;
			LODWORD(v116) = v107;
			Vector__operator=Vector((int)&v120, (int)&v129);
			v111 = LODWORD(v120);
			v112 = LODWORD(v121);
			v113 = v122;
			v37 = 0;
			Vector__operator=Vector((int)&v100, (int)&v138);
			while (1)
			{
				UTIL_TraceLine((int)&v100, (int)&v111, 1, v37, (int)&v156);
				if (v157 == 1.0)
					break;
				v38 = v159;
				if (v159)
					v38 = v159 + 128;
				if (!IsEntityWalkable(v38, 2u))
				{
					if (v157 != 1.0)
					{
						v99 = 0;
						goto LABEL_59;
					}
					break;
				}
				v37 = v159;
				v39 = sub_10009A00((int)&v111, (int)&v163, (int)&v138);
				Vector__operator=Vector((int)&v127, v39);
				sub_1002B050((int)&v127);
				v40 = sub_1002B0B0((int)&v160, 5.0, (int)&v127);
				v41 = sub_100099C0((int)&v158, (int)&v161, v40);
				LODWORD(v100) = *(_DWORD *)v41;
				LODWORD(v101) = *(_DWORD *)(v41 + 4);
				LODWORD(v102) = *(_DWORD *)(v41 + 8);
			}
			if (v96 >= m_currentPathInformationNumber || !*(_DWORD *)(v28 + 28) && !(*(_BYTE *)(*(_DWORD *)(v28 + 8) + 40) & 2))
			{
				v42 = IsStraightLinePathWalkable ((int)&v117) == 0;
				v43 = v96;
				if (v42)
					goto LABEL_58;
				v44 = v119;
				if (v96 == resultIndex)
				{
					*(float *)&v45 = v44 - v103;
					*(float *)&v46 = v118 - v109;
					*(float *)&v47 = v117 - v108;
					v48 = Vector__operator=xyz((int)&v155, v47, v46, v45);
					Vector__operator=Vector((int)&v162, v48);
					v49 = &v162;
				}
				else
				{
					*(float *)&v50 = v44 - *(float *)v28;
					*(float *)&v51 = v118 - *(float *)(v28 - 4);
					*(float *)&v52 = v117 - *(float *)(v28 - 8);
					v53 = Vector__operator=xyz((int)&v144, v52, v51, v50);
					Vector__operator=Vector((int)&v145, v53);
					v49 = &v145;
				}
				v54 = sqrt(*((float *)v49 + 1) * *((float *)v49 + 1) + *(float *)v49 * *(float *)v49) + v126;
				v126 = v54;
				if (v54 < distance)
				{
					v28 += 24;
					++v96;
					if (v96 < m_currentPathInformationNumber)
						continue;
				}
			}
			goto LABEL_59;
		}
		v43 = v96;
LABEL_58:
		v96 = v43 - 1;
LABEL_59:
		v27 = v96;
		if (v96 >= resultIndex)
		{
LABEL_102:
			v56 = m_currentPathInformationNumber;
			if (v27 >= v56)
				v55 = v56 - 1;
			else
				v55 = v27;
		}
		else
		{
			v55 = resultIndex;
		}
		if (v55)
		{
			v57 = 3 * v55 + 384;
			v58 = 3 * v55 + 381;
			v59 = this + 8 * v57;
			v60 = this + 8 * v58;
			*(float *)&v61 = *(float *)(this + 8 * v57 + 8) - *(float *)(this + 8 * v58 + 8);
			*(float *)&v62 = *(float *)(v59 + 4) - *(float *)(v60 + 4);
			*(float *)&v63 = *(float *)v59 - *(float *)v60;
			v64 = Vector__operator=xyz((int)&v144, v63, v62, v61);
			Vector__operator=Vector((int)&v114, v64);
			v110 = v116;
			v108 = v114;
			v109 = v115;
			v103 = sqrt(v115 * v115 + v114 * v114);
			v65 = 1.0 - (v126 - distance) / v103;
			v97 = v65;
			if (v65 >= 0.0)
			{
				if (v97 > 1.0)
					v97 = 1.0;
			}
			else
			{
				v97 = 0.0;
			}
			v66 = sub_10009A40((int)&v108, (int)&v144, v97);
			Vector__operator=Vector((int)&v120, v66);
			*(float *)&v67 = *(float *)&v122 + *(float *)(v60 + 8);
			*(float *)&v68 = v121 + *(float *)(v60 + 4);
			*(float *)&v69 = v120 + *(float *)v60;
			v70 = Vector__operator=xyz((int)&v155, v69, v68, v67);
			Vector__operator=Vector((int)&v129, v70);
			v71 = v130;
			origin.x = LODWORD(v129);
			v72 = v131;
			origin.y = LODWORD(v71);
			origin.z = LODWORD(v72);
			if (!v99)
			{
				v74 = origin.y;
				v75 = origin.x;
				v103 = 25.0 / v103;
				*(float *)&v76 = origin.z + 36.0;
				v77 = Vector__operator=xyz((int)&v144, v75, v74, v76);
				Vector__operator=Vector((int)&v120, v77);
				LODWORD(v102) = v122;
				v100 = v120;
				v101 = v121;
				if (v97 > 0.0)
				{
LABEL_73:
					Iostream_init::Iostream_init(&v156);
					v78 = 0;
					Vector__operator=Vector((int)&v111, (int)&v138);
					while (1)
					{
						UTIL_TraceLine((int)&v111, (int)&v100, 1, v78, (int)&v156);
						if (v157 == 1.0)
							break;
						v79 = v159;
						if (v159)
							v79 = v159 + 128;
						if (!IsEntityWalkable(v79, 2u))
						{
							if (v157 != 1.0)
							{
								v97 = v97 - v103;
								v83 = sub_10009A40((int)&v108, (int)&v160, v97);
								Vector__operator=Vector((int)&v145, v83);
								v84 = sub_100099C0(v60, (int)&v163, (int)&v145);
								origin.x = *(_DWORD *)v84;
								origin.y = *(_DWORD *)(v84 + 4);
								origin.z = *(_DWORD *)(v84 + 8);
								if (v97 > 0.0)
									goto LABEL_73;
								goto LABEL_83;
							}
							break;
						}
						v78 = v159;
						v80 = sub_10009A00((int)&v100, (int)&v144, (int)&v138);
						Vector__operator=Vector((int)&v127, v80);
						sub_1002B050((int)&v127);
						v81 = sub_1002B0B0((int)&v155, 5.0, (int)&v127);
						v82 = sub_100099C0((int)&v158, (int)&v161, v81);
						v111 = *(_DWORD *)v82;
						v112 = *(_DWORD *)(v82 + 4);
						v113 = *(_DWORD *)(v82 + 8);
					}
					if (v97 > 0.0)
						goto LABEL_85;
				}
LABEL_83:
				*(_DWORD *)origin = *(_DWORD *)v60;
				*(_DWORD *)(origin + 4) = *(_DWORD *)(v60 + 4);
				*(_DWORD *)(origin + 8) = *(_DWORD *)(v60 + 8);
			}
		}
		else
			origin = m_path[0u].point;
LABEL_85:
		if (!v98)
		{
			v85 = *(_DWORD *)(this + 4);
			v103 = origin.x - *(float *)(v85 + 8);
			v104 = origin.y - *(float *)(v85 + 12);
			v86 = sub_1002B020((int)&v123, (int)&v126);
			if (v104 * *(float *)(v86 + 4) + v103 * *(float *)v86 < 0.0 || sub_1002AFC0((int)&v103, 50.0))
			{
				v87 = resultIndex;
				if (resultIndex < m_currentPathInformationNumber)
				{
					v88 = this + 24 * resultIndex + 3076;
					while (1)
					{
						v89 = *(_DWORD *)(this + 4);
						v103 = *(float *)(v88 - 4) - *(float *)(v89 + 8);
						v90 = *(float *)v88 - *(float *)(v89 + 12);
						v91 = *(_DWORD *)(v88 + 8);
						v104 = v90;
						if (v91)
							break;
						if (*(_BYTE *)(*(_DWORD *)(v88 - 12) + 40) & 2 || sub_1002AFF0((int)&v103, 50.0))
							break;
						++v87;
						v88 += 24;
						if (v87 >= m_currentPathInformationNumber)
							goto LABEL_96;
					}
					resultIndex = v87;
					v92 = this + 8 * (3 * v87 + 384);
					origin.x = *(_DWORD *)v92;
					origin.y = *(_DWORD *)(v92 + 4);
					origin.z = *(_DWORD *)(v92 + 8);
				}
LABEL_96:
				v93 = m_currentPathInformationNumber;
				if (v87 == v93)
				{
					v94 = this + 8 * (3 * v93 + 381);
					origin.x = *(_DWORD *)v94;
					origin.y = *(_DWORD *)(v94 + 4);
					origin.z = *(_DWORD *)(v94 + 8);

					resultIndex = m_currentPathInformationNumber - 1;
				}
			}
		}
	}
	return resultIndex;
}

static const float CCSBot::GetApproximateFallDamage (float distance)
{
	distance *= 0.21780001f;	/// @todo MAKE UNDERSTANDABLE VALUE!!!
	distance -= 26.0f;

	return distance < 0.0f ? 0.0f : distance;
}

const bool CCSBot::IsFriendInTheWay (const Math::Vector3D &goalOrigin)
{
	if (!m_isFriendInTheWayNextCallTimer.IsElapsed ())
		return m_isFriendInTheWay;

	m_isFriendInTheWayNextCallTimer.Start (0.5f);

	m_isFriendInTheWay = false;

	Math::Vector3D directionToGoalNormalized (goalOrigin - GetOrigin ());
	const float distanceToGoal (directionToGoalNormalized.GetLength ());

	directionToGoalNormalized /= distanceToGoal;	// Normalize.

	for (unsigned char index (1u); index <= gpGlobals->maximumClients; ++index)
	{
		BasePlayer *const player (UTIL_PlayerByIndex (index));

		if (player == NULL || FNullEnt (player->m_variables) || !player->IsAlive () || player->m_team != m_team || player == this)
			continue;

		const Math::Vector3D directionToTemmate (player->GetOrigin () - GetOrigin ());

		if (directionToTemmate.GetLengthSquared () > MATH_GET_SQUARED (100.0f))
			continue;

		const float dot (directionToGoalNormalized | directionToTemmate);

		if (dot <= 0.0f)
			continue;

		Math::Vector3D point;

		if (dot < distanceToGoal)
			point = GetOrigin () + directionToGoalNormalized * dot;
		else
			point = goalOrigin;

		if (player->GetOrigin ().GetDistanceSquared (point) < MATH_GET_SQUARED (30.0f))
		{
			m_isFriendInTheWay = true;

			break;
		}
	}

	return m_isFriendInTheWay;
}

void CCSBot::FeelerReflexAdjustment (Math::Vector3D &newGoalOrigin)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (m_lastKnownArea != NULL && m_lastKnownArea->HasAttributes (NAV_MESH_PRECISE))	/// @note This should be some inlined function!!!!
		return;

		v4 = BotSIN(m_lookForwardAngle);
		LODWORD(v5) = m_lookForwardAngle;
		v57 = v4;
		*(float *)&v78 = BotCOS(v5);
		v6 = m_doCrouch;
		*(float *)&v67 = -v57;
		v79 = v57;
		v68 = v78;
		LODWORD(v80) = 0;
		v69 = 0.0;
		v77 = 15.0;
		if (!v6)
			v77 = 20.0;
		v7 = m_doRun;
		v57 = 50.0;
		v82 = 18.0 + 0.1;
		if (!v7)
			v57 = 30.0;
		if (v6)
			v57 = 20.0;
		if (GetSimpleGroundHeightWithFloor (GetEyePosition (), (int)&v84, (int)&v73))
		{
			*(float *)&v54 = v75 * *(float *)&v68 - v69 * v74;
			*(float *)&v55 = v69 * v73 - v75 * *(float *)&v67;
			*(float *)&v56 = v74 * *(float *)&v67 - *(float *)&v68 * v73;
			Vector__operator=Vector((int)&v64, (int)&v54);
			v78 = v64;
			v79 = v65;
			v80 = v66;
			v52 = *(float *)&v64 * v74 - v65 * v73;
			*(float *)&v51 = v66 * v73 - *(float *)&v64 * v75;
			*(float *)&v50 = v65 * v75 - v66 * v74;
			v9 = Vector__operator=xyz((int)&v58, v50, v51, SLODWORD(v52));
			Vector__operator=Vector((int)&v54, v9);
			v67 = v54;
			v68 = v55;
			LODWORD(v69) = v56;
			v10 = Vector__operator=xyz((int)&v81, -1048576000, -1048576000, -1047527424);
			Vector__operator=Vector((int)&v58, v10);
			v11 = Vector__operator=xyz((int)&v76, -1048576000, -1048576000, -1039138816);
			Vector__operator=Vector((int)&v54, v11);
			v12 = *(_DWORD *)(this + 4);
			if (m_doCrouch)
				v13 = v60;
			else
				v13 = *(float *)&v56;
			v14 = v13 + *(float *)(v12 + 16);
			*(float *)&v64 = *(float *)(v12 + 8);
			v15 = v77;
			v65 = *(float *)(v12 + 12);
			v66 = v14 + v82;
			v16 = sub_10009A40((int)&v67, (int)&v76, v77);
			Vector__operator=Vector((int)&v54, v16);
			v52 = v66 + *(float *)&v56;
			*(float *)&v51 = v65 + *(float *)&v55;
			*(float *)&v50 = *(float *)&v54 + *(float *)&v64;
			v17 = Vector__operator=xyz((int)&v81, v50, v51, SLODWORD(v52));
			Vector__operator=Vector((int)&v58, v17);
			v18 = v57;
			v61 = v58;
			v63 = v60;
			v62 = v59;
			v19 = sub_10009A40((int)&v78, (int)&v76, v57);
			Vector__operator=Vector((int)&v58, v19);
			v52 = v63 + v60;
			*(float *)&v51 = v62 + v59;
			*(float *)&v50 = *(float *)&v58 + *(float *)&v61;
			v20 = Vector__operator=xyz((int)&v81, v50, v51, SLODWORD(v52));
			Vector__operator=Vector((int)&v54, v20);
			v71 = v55;
			v70 = v54;
			v72 = v56;
			v21 = Vector__operator=xyz((int)&v76, -1048576000, -1048576000, -1047527424);
			Vector__operator=Vector((int)&v81, v21);
			v22 = Vector__operator=xyz((int)&v58, -1048576000, -1048576000, -1039138816);
			Vector__operator=Vector((int)&v54, v22);
			Iostream_init::Iostream_init(&v85);
			v23 = 0;
			Vector__operator=Vector((int)&v54, (int)&v61);
			while (1)
			{
				UTIL_TraceLine((int)&v54, (int)&v70, 1, v23, (int)&v85);
				if (v86 == 1.0)
					goto LABEL_20;
				v24 = v88;
				if (v88)
					v24 = v88 + 128;
				if (!IsEntityWalkable(v24, 3u))
					break;
				v23 = v88;
				v25 = sub_10009A00((int)&v70, (int)&v76, (int)&v61);
				Vector__operator=Vector((int)&v58, v25);
				sub_1002B050((int)&v58);
				v26 = sub_1002B0B0((int)&v81, 5.0, (int)&v58);
				v27 = sub_100099C0((int)&v87, (int)&v83, v26);
				v54 = *(_DWORD *)v27;
				v55 = *(_DWORD *)(v27 + 4);
				v56 = *(_DWORD *)(v27 + 8);
			}
			if (v86 == 1.0)
			{
LABEL_20:
				v53 = 1;
				goto LABEL_22;
			}
			v53 = 0;
LABEL_22:
			if (cv_bot_traceview.value == 1.0 && IsLocalPlayerWatchingMe() || cv_bot_traceview.value == 10.0)
			{
				v52 = 0.0;
				if (v53)
				{
					v51 = 255;
					*(float *)&v50 = 0.0;
				}
				else
				{
					*(float *)&v51 = 0.0;
					v50 = 255;
				}
				v49 = 1;
				Vector__operator=Vector((int)&v46, (int)&v70);
				Vector__operator=Vector((int)&v43, (int)&v61);
				UTIL_DrawBeamPoints(v43, v44, v45, v46, v47, v48, v49, v50, v51, LOBYTE(v52));
			}
			v28 = sub_10009A40((int)&v67, (int)&v83, v15);
			Vector__operator=Vector((int)&v58, v28);
			v52 = v66 - v60;
			*(float *)&v51 = v65 - v59;
			*(float *)&v50 = *(float *)&v64 - *(float *)&v58;
			v29 = Vector__operator=xyz((int)&v76, v50, v51, SLODWORD(v52));
			Vector__operator=Vector((int)&v54, v29);
			v61 = v54;
			LODWORD(v63) = v56;
			LODWORD(v62) = v55;
			v30 = sub_10009A40((int)&v78, (int)&v83, v18);
			Vector__operator=Vector((int)&v58, v30);
			v52 = v63 + v60;
			*(float *)&v51 = v62 + v59;
			*(float *)&v50 = *(float *)&v58 + *(float *)&v61;
			v31 = Vector__operator=xyz((int)&v76, v50, v51, SLODWORD(v52));
			Vector__operator=Vector((int)&v54, v31);
			v71 = v55;
			v70 = v54;
			v72 = v56;
			Iostream_init::Iostream_init(&v85);
			v32 = 0;
			Vector__operator=Vector((int)&v54, (int)&v61);
			while (1)
			{
				UTIL_TraceLine((int)&v54, (int)&v70, 1, v32, (int)&v85);
				if (v86 == 1.0)
					goto LABEL_36;
				v33 = v88;
				if (v88)
					v33 = v88 + 128;
				if (!IsEntityWalkable(v33, 3u))
					break;
				v32 = v88;
				v34 = sub_10009A00((int)&v70, (int)&v83, (int)&v61);
				Vector__operator=Vector((int)&v58, v34);
				sub_1002B050((int)&v58);
				v35 = sub_1002B0B0((int)&v76, 5.0, (int)&v58);
				v36 = sub_100099C0((int)&v87, (int)&v81, v35);
				v54 = *(_DWORD *)v36;
				v55 = *(_DWORD *)(v36 + 4);
				v56 = *(_DWORD *)(v36 + 8);
			}
			if (v86 == 1.0)
			{
LABEL_36:
				v37 = 1;
				goto LABEL_38;
			}
			v37 = 0;
LABEL_38:
			if (cv_bot_traceview.value == 1.0 && IsLocalPlayerWatchingMe () || cv_bot_traceview.value == 10.0)
			{
				v52 = 0.0;
				if (v37)
				{
					v51 = 255;
					*(float *)&v50 = 0.0;
				}
				else
				{
					*(float *)&v51 = 0.0;
					v50 = 255;
				}
				v49 = 1;
				Vector__operator=Vector((int)&v46, (int)&v70);
				Vector__operator=Vector((int)&v43, (int)&v61);
				UTIL_DrawBeamPoints(v43, v44, v45, v46, v47, v48, v49, v50, v51, LOBYTE(v52));
			}
			v38 = m_doCrouch;
			v57 = 150.0;
			if (!v38)
				v57 = 300.0;
			LOBYTE(v3) = v53;
			if (v37)
			{
				if (!v53)
				{
					v40 = sub_10009A40((int)&v67, (int)&v83, v57);
					Vector__operator=Vector((int)&v58, v40);
					v41 = sub_10009A00(newGoalOrigin, (int)&v76, (int)&v58);
					*(_DWORD *)newGoalOrigin = *(_DWORD *)v41;
					*(_DWORD *)(newGoalOrigin + 4) = *(_DWORD *)(v41 + 4);
					v3 = *(_DWORD *)(v41 + 8);
					*(_DWORD *)(newGoalOrigin + 8) = v3;
				}
			}
			else
			{
				if (v53)
				{
					v39 = sub_10009A40((int)&v67, (int)&v83, v57);
					Vector__operator=Vector((int)&v58, v39);
					v3 = sub_100099C0(newGoalOrigin, (int)&v76, (int)&v58);
					*(_DWORD *)newGoalOrigin = *(_DWORD *)v3;
					*(_DWORD *)(newGoalOrigin + 4) = *(_DWORD *)(v3 + 4);
					*(_DWORD *)(newGoalOrigin + 8) = *(_DWORD *)(v3 + 8);
				}
			}
		}
}

enum PathMovementState_t
{
	PathMovementState_Failed = 0u,	// CHECK NAME
	PathMovementState_Succeed = 1u,	// CHECK NAME

	PathMovementState_PathIsEmpty = 2u	// CHECK NAME
};
PathMovementState_t CCSBot::UpdatePathMovement (const bool a2)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (m_currentPathInformationNumber == 0u)
		return PathMovementState_PathIsEmpty;

	if (cv_bot_walk.value != 0.0f)
		Walk ();

	if (UpdateLadderMovement ())
		return PathMovementState_Failed;

	if (m_currentPathInformationNumber == 0u)
		return PathMovementState_PathIsEmpty;

	if (IsOnLadder ())
		Jump (true);

	Math::Vector3D directionToGoal (m_path[m_currentPathInformationNumber - 1u].point - GetOrigin ());

	directionToGoal.z = m_path[m_currentPathInformationNumber - 1u].point.z - (GetOrigin ().z - (m_doCrouch ? 18.0f : 36.0f));
//	directionToGoal.z -= m_doCrouch ? 18.0f : 36.0f;

	const float distanceToGoal (directionToGoal.GetLengthSquared ());

	if (m_currentPathIndex < m_currentPathInformationNumber - 1u || distanceToGoal >= MATH_GET_SQUARED (200.0f))
		goto LABEL_118;

	if (a2 && !m_doCrouch)
		Walk ();

	if (distanceToGoal >= MATH_GET_SQUARED (20.0f))
	{
LABEL_118:
		unsigned short currentPathIndex (0u);
		const unsigned short goalPathIndex (FindPathPoint (300.0f, &m_goalOrigin, &currentPathIndex));

		if (m_currentPathIndex < m_currentPathInformationNumber - 1u && GetOrigin ().GetDistanceSquared (m_goalOrigin) < MATH_GET_SQUARED (100.0f))
		{
			ClearLookAt ();
			InhibitLookAround (0.5f);
		}

		if (goalPathIndex > m_currentPathIndex)
			SetPathIndex (goalPathIndex);

		if (m_currentLadder == NULL)
			for (/* Empty */; currentPathIndex < m_currentPathInformationNumber; ++currentPathIndex)
			{
				const NavigationMesh::NavigationArea *const area (m_path[currentPathIndex].area);

				if (area->HasAttributes (NAV_MESH_JUMP) || GetOrigin ().AsVector2D ().GetDistanceSquared (area->GetClosestPointOnArea (GetOrigin ())) > MATH_GET_SQUARED (50.0f))
				{
					if (!IsJumping ())
						StandUp ();

					break;
				}

				if (area->HasAttributes (NAV_MESH_CROUCH))
				{
					Crouch ();

					break;
				}
			}

		m_lookForwardAngle = (m_goalOrigin.AsVector2D () - GetOrigin ().AsVector2D ()).ToYaw ();

		Math::Vector3D lookDirection;

		if (m_currentPathIndex == 0u)
			lookDirection = m_path[1u].point;
		else
		{
			if (m_currentPathIndex >= m_currentPathInformationNumber)
				lookDirection = m_path[m_currentPathInformationNumber - 1u].point - GetOrigin ();
			else
				lookDirection = m_path[m_currentPathIndex].point - GetOrigin ();

			if (m_path[m_currentPathIndex].ladder == NULL)
			{
				if (m_currentPathIndex == 0u || m_currentPathIndex >= m_currentPathInformationNumber || m_currentPathIndex <= m_currentPathIndex - 1u)
				{
LABEL_51:
					const float lookDistance (500.0f);
					float distance (lookDirection.GetLength2D ());

					if (distance < lookDistance)
					{
						for (unsigned short index (m_currentPathIndex + 1u); index < m_currentPathInformationNumber; ++index)
						{
							const Math::Vector3D direction (m_path[index].point - m_path[index - 1u].point);
							const float directionLength (direction.GetLength2D ());
							const float directionLengthTotal (distance + directionLength);

							if (directionLengthTotal >= lookDistance)
							{
								const float fraction ((lookDistance - distance) / directionLengthTotal);

								if (fraction <= 0.0f)
									lookDirection = m_path[index - 1u].point;
								else if (fraction >= 1.0f)
									lookDirection = m_path[index].point;
								else
									lookDirection = m_path[index - 1u].point + direction * fraction;

								lookDirection -= GetOrigin ();

								break;
							}

							if (m_path[index].ladder != NULL || m_path[index].area->HasAttributes (NAV_MESH_JUMP))
							{
								lookDirection = m_path[index].point - GetOrigin ();

								break;
							}

							distance = directionLengthTotal;
						}

						if (index == m_currentPathInformationNumber)
							lookDirection = m_path[m_currentPathInformationNumber - 1u].point - GetOrigin ();
					}
				}
				else
				{
					unsigned short index (m_currentPathIndex - 1u);

					while (!m_path[index].area->HasAttributes (NAV_MESH_JUMP) || m_path[index + 1u].point.z <= m_path[index].point.z)
						if (++index >= m_currentPathIndex)
							goto LABEL_51;
				}
			}
		}

		m_lookAheadAngle = lookDirection.ToYaw ();

		Math::Vector3D newGoalOrigin (m_goalOrigin);

		if (distanceToGoal >= MATH_GET_SQUARED (50.0f))
		{
			if (m_currentPathIndex > 0u && m_currentPathIndex < m_currentPathInformationNumber && m_currentPathIndex - 1u < m_currentPathIndex)
			{
				unsigned short index (m_currentPathIndex - 1u);

				while (!m_path[index].area->HasAttributes (NAV_MESH_JUMP) || m_path[index + 1u].point.z <= m_path[index].point.z)
					if (++index >= m_currentPathIndex)
						goto LABEL_79;
			}
			else
			{
LABEL_79:
				if (!IsJumping ())
					FeelerReflexAdjustment (newGoalOrigin);
			}
		}

		if (cv_bot_traceview.value == 1.0f && IsLocalPlayerWatchingMe () || cv_bot_traceview.value == 10.0f)
		{
			DrawPath ();

			UTIL_DrawBeamPoints (m_path[m_currentPathIndex].point, m_path[m_currentPathIndex].point + Math::Vector3D (0.0f, 0.0f, 50.0f), 1, 255, 255, 0);
			UTIL_DrawBeamPoints (newGoalOrigin, newGoalOrigin + Math::Vector3D (0.0f, 0.0f, 50.0f), 1, 255, 0, 255);
			UTIL_DrawBeamPoints (GetOrigin (), newGoalOrigin + Math::Vector3D (0.0f, 0.0f, 50.0f), 1, 255, 0, 255);
		}

		if (IsAttacking () || !IsFriendInTheWay (m_goalOrigin))
		{
			if (!m_canResetStuckMonitor)
			{
LABEL_95:
				MoveTowardsPosition (newGoalOrigin);

				if (m_isStucked && !IsJumping ())
					Wiggle ();

LABEL_98:
				if
				(
					m_goalOrigin.z - (GetOrigin ().z - (m_doCrouch ? 18.0f : 36.0f)) > 58.0f &&
					GetOrigin ().AsVector2D ().GetDistanceSquared (m_goalOrigin) < MATH_GET_SQUARED (75.0f) &&
					(
						m_currentPathIndex >= m_currentPathInformationNumber - 1u ||
						m_path[m_currentPathIndex + 1u].point.z - (GetOrigin ().z - (m_doCrouch ? 18.0f : 36.0f)) > 58.0f
					)
				)
					PrintIfWatched ("I fell off!\n");
				else if (gpGlobals->time - m_currentPathIndexSetTimestamp <= 5.0f)
					return PathMovementState_Failed;

				if (m_currentPathIndex >= m_currentPathInformationNumber - 1u)
					PrintIfWatched ("Giving up trying to get to end of path\n");
				else
					PrintIfWatched ("Giving up trying to get to area #%d\n", m_path[m_currentPathIndex].area->GetID ());

				Run ();
				StandUp ();
				DestroyPath ();

				return PathMovementState_PathIsEmpty;
			}

			m_canResetStuckMonitor = false;

			ResetStuckMonitor ();
		}
		else
		{
			if (m_canResetStuckMonitor)
			{
				if (m_resetStuckMonitorNextCallTimer.IsElapsed ())
				{
					m_canResetStuckMonitor = false;

					ResetStuckMonitor ();
					DestroyPath ();
				}
			}
			else
			{
				m_canResetStuckMonitor = true;

				m_resetStuckMonitorNextCallTimer.Start (5.0f - m_profile->aggression * 3.0f);
			}
		}

		if (m_canResetStuckMonitor && !m_resetStuckMonitorNextCallTimer.IsElapsed ())
			goto LABEL_98;

		goto LABEL_95;
	}

	DestroyPath ();

	if (a2)
		Run ();

	return PathMovementState_Succeed;
}

void CCSBot::ResetStuckMonitor (void)
{
	if (m_isStucked && cv_bot_debug.value > 0.0 && IsLocalPlayerWatchingMe ())
		Globals::g_halfLifeEngine->EmitSound (GetEdict (), Constants::SoundChannel_Item, "buttons/bell1.wav");

	m_isStucked = false;
	m_stuckTimestamp = 0.0f;
	m_wiggleJumpTimestamp = 0.0f;
	m_distancesToPreviousOriginSinceStuckCurrentIndex0 = 0u;
	m_distancesToPreviousOriginSinceStuckCurrentIndex1 = 0u;
	m_currentPathIndexSetTimestamp = gpGlobals->time;
}

void CCSBot::StuckCheck (void)
{
	if (m_isStucked)
	{
		if (GetOrigin ().GetDistanceSquared (m_stuckOrigin) > MATH_GET_SQUARED (75.0f))
		{
			if (cv_bot_debug.value > 0.0f && IsLocalPlayerWatchingMe ())
				Globals::g_halfLifeEngine->EmitSound (GetEdict (), Constants::SoundChannel_Item, "buttons/bell1.wav");

			m_isStucked = false;
			m_stuckTimestamp = 0.0f;
			m_wiggleJumpTimestamp = 0.0f;
			m_distancesToPreviousOriginSinceStuckCurrentIndex0 = 0u;
			m_distancesToPreviousOriginSinceStuckCurrentIndex1 = 0u;
			m_currentPathIndexSetTimestamp = 0.0f;

			PrintIfWatched ("UN-STUCK\n");
		}
	}
	else
	{
		const float distance (IsJumping () ? GetOrigin ().GetDistance2D (m_previousOrigin) : GetOrigin ().GetDistance (m_previousOrigin));

		m_distancesToPreviousOriginSinceStuck[m_distancesToPreviousOriginSinceStuckCurrentIndex0] = distance / g_flBotFullThinkInterval;

		if (++m_distancesToPreviousOriginSinceStuckCurrentIndex0 == MaximumStuckTable)
			m_distancesToPreviousOriginSinceStuckCurrentIndex0 = 0u;

		if (m_distancesToPreviousOriginSinceStuckCurrentIndex1 >= MaximumStuckTable)
		{
			float totalDistance (0.0f);
			const float distanceMaximum (m_currentLadder != NULL ? 10.0f : 20.0f);

			for (unsigned int index (0u); index < m_distancesToPreviousOriginSinceStuckCurrentIndex1; ++index)
				totalDistance += m_distancesToPreviousOriginSinceStuck[index];

			if (totalDistance / m_distancesToPreviousOriginSinceStuckCurrentIndex1 < distanceMaximum)
			{
				m_stuckTimestamp = gpGlobals->time;
				m_stuckOrigin = pev->origin;
				m_wiggleJumpTimestamp = gpGlobals->time + g_engfuncs.pfnRandomFloat (0.0f, 0.5f);

				PrintIfWatched ("STUCK\n");

				if (cv_bot_debug.value > 0.0f && IsLocalPlayerWatchingMe ())
					Globals::g_halfLifeEngine->EmitSound (GetEdict (), Constants::SoundChannel_Item, "buttons/button11.wav");

				m_isStucked = true;
			}
		}
		else
			++m_distancesToPreviousOriginSinceStuckCurrentIndex1;
	}

	m_previousOrigin = pev->origin;
}

void CCSBot::FireWeaponAtEnemy (void)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (!m_enemy)
	{
		m_isRapidFiring = false;

		return;
	}

	if (m_pActiveItem == NULL)
	{
LABEL_6:
		if (gpGlobals->time <= m_fireWeaponAtEnemyNextCallTimestamp)
			return;

		v4 = GetTimeSinceAcquiredCurrentEnemy ();

		if (v4 < m_profile->attackDelay)
			return;

		if (v4 < GetSurpriseDelay ())
			return;

		ClearSurpriseDelay ();

		if (IsRecognizedEnemyProtectedByShield () && IsPlayerFacingMe (m_enemy))
			return;

		if (IsActiveWeaponReloading ())
			return;

		if (IsActiveWeaponClipEmpty ())
			return;

		if (!m_isEnemyVisible)
			return;

		Math::Vector2D direction (m_enemyShootPosition.AsVector2D () - GetOrigin ().AsVector2D ());
		const float length (direction.GetLength ());
		direction /= length;	// Normalize.

		const float radians (Math::DegreeToRadian (m_variables->viewAngles.yaw));
		const float v14 (sin (radians) * direction.y + cos (radians) * direction.x);

		if (v14 <= cos (atan2 (1.0f, (IsUsingSniperRifle () ? 16.0f : 16.0f * 2.0f) / length)))
			return;
/** FROM NEW SOURCE ENGINE
		{
		const Math::Vector3D forward ((m_variables->viewAngles + m_variables.punchAngles).MakeForwardVector ());

		if (forward | direction <= cos (atan2 (1.0f, (IsUsingSniperRifle () ? 16.0f : 16.0f + 16.0f) / length)))
			return;
		}
*/
		if (friendlyfire.value <= 0.0f || !IsFriendInLineOfFire ())
		{
			if (IsUsingKnife ())
			{
				if (length < 75.0f)
				{
					ForceRun (5.0f);

					if (IsPlayerFacingMe (m_enemy))
					{
						if (g_engfuncs.pfnRandomFloat (0.0f, 100.0f) >= 33.3f)
							PrimaryAttack ();
						else
							SecondaryAttack ();
					}
					else
						SecondaryAttack ();
				}
			}
			else
				PrimaryAttack ();
		}

		if (IsUsingPistol ())
		{
			if (m_profile->skill <= 0.75f || length >= 999999.875f)
				m_fireWeaponAtEnemyNextCallTimestamp = g_engfuncs.pfnRandomFloat (0.15f, 1053609165);
			else
			{
				m_fireWeaponAtEnemyNextCallTimestamp = 0.0f;
				m_isRapidFiring = true;
			}
			goto LABEL_59;
		}

		if (m_profile->skill >= 0.5f)
		{
			if (length >= 400.0f)
			{
				// IsUsingMachinegun():
				if (m_pActiveItem == NULL)
					goto LABEL_55;
				v25 = m_pActiveItem->m_iId;
				if (v25 != Constants::WeaponID_M249)
				{
					// IsUsingSniperRifle():
					if (m_pActiveItem != NULL)
					{
						v21 = 0;
						switch (v25)
						{
							default:
								goto LABEL_55;
							case Constants::WeaponID_Scout:
							case Constants::WeaponID_SG550:
							case Constants::WeaponID_AWP:
							case Constants::WeaponID_G3SG1:
								goto LABEL_57;
						}
						goto LABEL_57;
					}
LABEL_55:
					if (length > 800.0f)
					{
						m_fireWeaponAtEnemyNextCallTimestamp = g_engfuncs.pfnRandomFloat (0.3f, 0.7f);
LABEL_59:
						m_fireWeaponAtEnemyNextCallTimestamp -= g_flBotFullThinkInterval;
						m_fireWeaponAtEnemyNextCallTimestamp += gpGlobals->time;
						return;
					}
LABEL_57:
					m_fireWeaponAtEnemyNextCallTimestamp = g_engfuncs.pfnRandomFloat (0.15f, 0.5f);
					goto LABEL_59;
				}
			}
		}
		m_fireWeaponAtEnemyNextCallTimestamp = 0.0f;
		goto LABEL_59;
	}

	// IsUsingSniperRifle():
	switch (m_pActiveItem->m_iId)
	{
		case Constants::WeaponID_Scout:
		case Constants::WeaponID_SG550:
		case Constants::WeaponID_AWP:
		case Constants::WeaponID_G3SG1:
			if (sub_10013DA0 (10.0f))
				goto LABEL_6;
			break;
		default:
			goto LABEL_6;
	}
}

void CCSBot::SetAimOffset (float accuracy)
{
	if (accuracy < 1.0f)
	{
		if (IsViewMoving (100.0f))
			m_attackBeginTimestamp = gpGlobals->time;

		float v3 ((1.0f - accuracy) * 5.0f);

		if (v3 < 2.0f)
			v3 = 2.0f;

		float v4 ((gpGlobals->time - m_attackBeginTimestamp) / v3);

		if (v4 > 0.75f)
			v4 = 0.75f;

		if (accuracy < v4)
			accuracy = v4;
	}

	PrintIfWatched ("Accuracy = %4.3f\n", accuracy);

	const float offset (GetOrigin ().GetDistance (m_lastKnownEnemyPosition) * m_iFOV * (1.0f / Constants::DefaultPlayerFieldOfView) * 0.1f/*0.05f IN CS SOURCE*/ * (1.0f - accuracy));

	m_aimOffset2.x = g_engfuncs.pfnRandomFloat (-offset, offset);
	m_aimOffset2.y = g_engfuncs.pfnRandomFloat (-offset, offset);
	m_aimOffset2.z = g_engfuncs.pfnRandomFloat (-offset, offset);

	m_setAimOffsetNextCallTimestamp = gpGlobals->time + g_engfuncs.pfnRandomFloat (0.25f, 1.0f);
}

void CCSBot::UpdateAimOffset (void)
{
	if (m_setAimOffsetNextCallTimestamp <= gpGlobals->time)
		SetAimOffset (m_profile->skill);

	m_aimOffset += (m_aimOffset2 - m_aimOffset) * 0.1f;
}

inline const ZoomLevel_t CCSBot::GetZoomLevel (void) const { return m_iFOV > 60 ? ZoomLevel_None : m_iFOV > 25 ? ZoomLevel_First : ZoomLevel_Second; }

const bool CCSBot::AdjustZoom (const float distance)
{
	if (!IsUsingSniperRifle ())
		goto Label_CheckNoneZoomLevel;

	if (distance <= 300.0f)
	{
		Label_CheckNoneZoomLevel:
			if (GetZoomLevel () == ZoomLevel_None)
				return false;
	}
	else
	{
		if (distance < 1500.0f)
		{
			if (GetZoomLevel () == ZoomLevel_First)
				return false;
		}
		else if (GetZoomLevel () == ZoomLevel_Second)
			return false;
	}

	SecondaryAttack ();

	return true;
}

void CCSBot::ReloadCheck (void)
{
	if (GetEnemiesRemaining () == 0u || IsDefusingBomb () || IsActiveWeaponReloading ())
		return;

	if (!IsActiveWeaponClipEmpty ())
	{
		if (GetTimeSinceLastSawEnemy () <= 3.0f || GetActiveWeaponAmmoRatio () > 0.6f || (m_profile->skill > 0.5f && IsAttacking ()))
			return;
	}
	else if (m_profile->skill > 0.5f && IsAttacking () && !IsUsingPistol () && !IsPistolEmpty ())
	{
		EquipPistol ();

		return;
	}

	if (IsUsingAWP () && !IsActiveWeaponClipEmpty ())
		return;

	Reload ();

	if (GetNearbyEnemyCount () > 0u && !IsHiding () && m_profile->skill * 100.0f + 25.0f > g_engfuncs.pfnRandomFloat (0.0f, 100.0f) && GetTimeSinceLastSawEnemy () < 5.0f)
	{
		PrintIfWatched ("Retreating to a safe spot to reload!\n");

		const Math::Vector3D *const spot (FindNearbyRetreatSpot (1000.0f));

		if (spot != NULL)
		{
			IgnoreEnemies (10.0f);
			Run ();
			StandUp ();
			Hide (*spot, 0.0f, false);
		}
	}
}

void CCSBot::SilencerCheck (void)
{
	if (IsDefusingBomb () || IsActiveWeaponReloading () || IsAttacking () || !DoesActiveWeaponHaveSilencer () || GetNearbyEnemyCount () > 0u)
		return;

	const bool isCurrentWeaponSilenced (m_pActiveItem->m_iWeaponState & (Constants::WeaponState_USP_SILENCED | Constants::WeaponState_M4A1_SILENCED) > 0u);

	if (isCurrentWeaponSilenced != m_profile->preferUsingSilencer && !HasShield ())
	{
		PrintIfWatched ("%s silencer!\n", isCurrentWeaponSilenced ? "Unequipping" : "Equipping");

		m_pActiveItem->SecondaryAttack ();
	}
}

void CCSBot::StrafeAwayFromPosition (const Math::Vector2D &position)
{
	const float angle (m_variables->viewAngles.yaw);
	const Math::Vector2D directionNormalized ((position - GetOrigin ()).Normalize ());

	if ((directionNormalized | Math::Vector2D (-BotSIN (angle), BotCOS (angle))) < 0.0f)
		StrafeLeft ();
	else
		StrafeRight ();
}

Math::Vector3D &CCSBot::GetEyePosition (void) { return m_eyePosition = GetOrigin () + m_variables->viewOffset; }

void CCSBot::UpdateLookAngles (void)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	/// @note FROM SOURCE ENGINE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (*(_DWORD *)(bot_mimic + 48) != 0)
		return;

	if (IsAttacking ())
	{
		v24 = 300.0f;
		v4 = 30.0f;
	}
	else
	{
		v24 = 200.0f;
		v4 = 25.0f;
	}
	v23 = m_lookAngleYaw;
	v26 = m_lookAnglePitch;
	if (m_currentLadder != NULL && !IsLookingAtSpot (PriorityType_High))
		ComputeLadderAngles ((int)&v23, (int)&v26);
	v5 = EyeAngles ();
	LODWORD(v28) = *(_DWORD *)v5;
	LODWORD(v29) = *(_DWORD *)(v5 + 4);
	v30 = *(_DWORD *)(v5 + 8);
	v7 = AngleNormalize (v23 - v29);
	v8 = 3000.0f;
	if (v7 >= 1.0f || v7 <= -1.0f)
	{
		v9 = v7;
		v10 = -3000.0f;
		v11 = v9 * v24 - m_lookAngleYawUNKNOWN * v4;
		if (v11 > 3000.0f || (v8 = v11, v11 >= -3000.0f))
			v10 = v8;
		v13 = v10 * g_flBotCommandInterval + m_lookAngleYawUNKNOWN;
		m_lookAngleYawUNKNOWN = v13;
		v29 = g_flBotCommandInterval * v13 + v29;
//		if (Math::fabsf (v10) > 1000.0)
//			*(float *)(this + 14340) = CountdownTimer__Now();
	}
	else
	{
		m_lookAngleYawUNKNOWN = 0.0f;
		v29 = v23;
	}
	v15 = AngleNormalize (v26 - v28);
	v16 = v15 * v24 + v15 * v24 - m_lookAnglePitchUNKNOWN * v4;
	if (v16 <= 3000.0f)
	{
		if (v16 < -3000.0f)
			v16 = -3000.0f;
	}
	else
		v16 = 3000.0f;
	v18 = v16 * g_flBotCommandInterval + m_lookAnglePitchUNKNOWN;
	m_lookAnglePitchUNKNOWN = v18;
	v19 = g_flBotCommandInterval * v18 + v28;
	v28 = v19;
	v20 = v19;
	v21 = v16;
	v22 = v20;
	if (Math::fabsf (v21) > 1000.0f)
	{
//		*(float *)(this + 14340) = CountdownTimer__Now();
		v22 = v28;
	}
	if (v22 >= -89.0f)
	{
		if (v22 > 89.0f)
			v28 = 89.0f;
	}
	else
		v28 = -89.0f;
	CBasePlayer__SnapEyeAngles(this, (int)&v28);
//	if (CountdownTimer__Now() <= *(float *)(this + 15580))
//		*(float *)(this + 14340) = CountdownTimer__Now();
}

const bool CCSBot::IsVisible (const Math::Vector3D &origin, const bool checkViewCone)
{
	if (m_blindEndTime > gpGlobals->time)
		return false;

	if (checkViewCone && !FInViewCone (&origin))
		return false;

	const Math::Vector3D &eyePosition (GetEyePosition ());

	if (TheBots->IsLineBlockedBySmoke (eyePosition, origin))
		return false;

	Structures::TraceResult_t traceResult;

	UTIL_TraceLine (eyePosition, origin, ignore_monsters, ignore_glass, GetEdict (), &traceResult);

	return traceResult.fraction == 1.0f;
}

const bool CCSBot::IsVisible (BasePlayer *const player, const bool checkViewCone, VisiblePartType_t *const outputVisibleBodyParts)
{
	VisiblePartType_t visibleBodyParts (VisiblePartType_None);
	Math::Vector3D origin (player->GetOrigin ());

	if (IsVisible (origin, checkViewCone))
		visibleBodyParts = VisiblePartType_Waist;

	origin.z += 25.0f;

	if (IsVisible (origin, checkViewCone))
		visibleBodyParts |= VisiblePartType_Head;

	origin.z = player->GetOrigin ().z - ((player->m_variables->flags & FL_DUCKING) ? 14.0f : 34.0f);

	if (IsVisible (origin, checkViewCone))
		visibleBodyParts |= VisiblePartType_Legs;

	Math::Vector2D directionNormalized ((player->GetOrigin ().AsVector2D () - GetOrigin ().AsVector2D ()).Normalize ());

	directionNormalized.y = -directionNormalized.y;

	directionNormalized *= 13.0f;

	origin.x = player->GetOrigin ().x + directionNormalized.y;
	origin.y = player->GetOrigin ().y + directionNormalized.x;
	origin.z = player->GetOrigin ().z;

	if (IsVisible (origin, checkViewCone))
		visibleBodyParts |= VisiblePartType_LeftSide;

	origin.x = player->GetOrigin ().x - directionNormalized.y;
	origin.y = player->GetOrigin ().y - directionNormalized.x;

	if (IsVisible (origin, checkViewCone))
		visibleBodyParts |= VisiblePartType_RightSide;

	if (outputVisibleBodyParts != NULL)
		*outputVisibleBodyParts = visibleBodyParts;

	return visibleBodyParts != VisiblePartType_None;
}

void CCSBot::UpdateLookAt (void)
{
	const Math::Angles2D angles (UTIL_VecToAngles (m_lookAtVector - GetEyePosition ()/*! @note Not actually calling of GetEyePosition() function, just 'm_variables->origin + m_variables->viewOffset', but whatever and who care?*/));

	m_lookAnglePitch = 360.0f - angles.pitch;
	m_lookAngleYaw = angles.yaw;
}

void CCSBot::SetLookAt (const char *const informationString, const Math::Vector3D &position, const PriorityType_t priority, const float duration, const bool checkDistance, const float maximumAngleDifference)
{/*
	if (position == NULL)
		return;
*/
	if (IsLookingAtSpot (priority))
		return;

	if (m_lookAction != LookAction_None
		&& fabsf (position.x - m_lookAtVector.x) < 10.0f
		&& fabsf (position.y - m_lookAtVector.y) < 10.0f
		&& fabsf (position.z - m_lookAtVector.z) < 10.0f)
	{
		m_lookAtDuration = duration;

		if (m_lookAtPriorityType >= priority)
			goto Label_UpdateVariables;
	}
	else
	{
		m_lookAtVector = position;
		m_lookAction = LookAction_LookTowardsSpot;
		m_lookAtDuration = duration;
	}

	m_lookAtPriorityType = priority;

	Label_UpdateVariables:
		m_lookAtMaximumAngleDifference = maximumAngleDifference;
		m_lookAtCheckDistance = checkDistance;
		m_lookAtString = informationString;
}

inline void CCSBot::InhibitLookAround (const float delay) { m_inhibitLookAroundGlobalTime = gpGlobals->time + delay; }

void CCSBot::UpdatePeripheralVision (void)
{
	if (gpGlobals->time - m_updatePeripheralVisionCallTimestamp < 0.29f)
		return;

	m_updatePeripheralVisionCallTimestamp = gpGlobals->time;

	if (m_currentSpotEncounter == NULL)
		return;

	for (const Node_t *node (m_currentSpotEncounter->spotList.m_head->next); node != m_currentSpotEncounter->spotList.m_head; node = node->next)
	{
		HidingSpot *const spot (node->element.spot);

		if (IsVisible (spot->GetPosition () + Math::Vector3D (0.0f, 0.0f, Constants::HalfHumanHeight), true))
			SetHidingSpotCheckTimestamp (spot);
	}
}

const float CCSBot::GetHidingSpotCheckTimestamp (const HidingSpot *const spot) const
{
	for (unsigned char index (0u); index < m_hidingSpotsToCheckNumber; ++index)
		if (m_hidingSpotsToCheck[index].spot->GetID () == spot->GetID ())
			return m_hidingSpotsToCheck[index].checkTimestamp;

	return -999999.9f;
}

void CCSBot::SetHidingSpotCheckTimestamp (HidingSpot *const spot)
{
	float checkTimestamp (gpGlobals->time + 1.0f);
	unsigned char spotToUpdate (0u);

	for (unsigned char index (0u); index < m_hidingSpotsToCheckNumber; ++index)
	{
		if (m_hidingSpotsToCheck[index].spot->GetID () == spot->GetID ())
		{
			m_hidingSpotsToCheck[index].checkTimestamp = gpGlobals->time;

			return;
		}

		if (checkTimestamp > m_hidingSpotsToCheck[index].checkTimestamp)
		{
			checkTimestamp = m_hidingSpotsToCheck[index].checkTimestamp;
			spotToUpdate = index;
		}
	}

	if (m_hidingSpotsToCheckNumber >= MaximumHidingSpotsToCheck)
	{
		m_hidingSpotsToCheck[spotToUpdate].spot = spot;
		m_hidingSpotsToCheck[spotToUpdate].checkTimestamp = gpGlobals->time;
	}
	else
	{
		m_hidingSpotsToCheck[m_hidingSpotsToCheckNumber].spot = spot;
		m_hidingSpotsToCheck[m_hidingSpotsToCheckNumber].checkTimestamp = gpGlobals->time;

		++m_hidingSpotsToCheckNumber;
	}
}

void CCSBot::UpdateHostageEscortCount (void)
{
	if (m_hostageEscortCount == 0u || gpGlobals->time - m_updateHostageEscortCountTimestamp < 1.0f)
		return;

	m_updateHostageEscortCountTimestamp = gpGlobals->time;
	m_hostageEscortCount = 0u;

	for (BaseEntity *hostage (UTIL_FindEntityByClassname (NULL, "hostage_entity"); hostage != NULL && hostage->GetEdict () != NULL && g_engfuncs.pfnEntOffsetOfPEntity (hostage->GetEdict ()) > 0u; hostage = UTIL_FindEntityByClassname (hostage, "hostage_entity")))
		if (hostage->GetHostagePointer ()->IsFollowing (this))
			++m_hostageEscortCount;
}

void CCSBot::ComputeApproachPoints (void)
{
	m_approachPointsNumber = 0u;

	if (m_lastKnownArea == NULL)
		return;

	Math::Vector3D point, blendPoint;

	for (unsigned char index (0u); index < m_lastKnownArea->GetApproachInfoCount (); ++index)
	{
		const NavigationArea::ApproachInfo_t &info (m_lastKnownArea->GetApproachInfo (index));

		if (info.here == NULL || info.prev == NULL)
			continue;

		if (info.prevToHereHow > GO_WEST)
			point = info.here->GetCenter ();
		else
		{
			info.prev->ComputePortal (info.here, static_cast <NavDirType_t> (info.prevToHereHow), point);

			point.z = info.here->GetZ (point);
		}

		if (BendLineOfSight (point, blendPoint))
		{
			m_approachPoints[m_approachPointsNumber] = blendPoint;

			++m_approachPointsNumber;
		}
	}
}

void CCSBot::UpdateReactionQueue (void)
{
	if (cv_bot_zombie.value > 0.0f)
		return;

	BasePlayer *const newEnemy (FindMostDangerousThreat ());

	if (newEnemy != NULL)
	{
		m_recognizedEnemiesInfo[m_updateReactionQueue_UNKNOWN0].handle = newEnemy;
		m_recognizedEnemiesInfo[m_updateReactionQueue_UNKNOWN0].isReloading = newEnemy->IsReloading ();
		m_recognizedEnemiesInfo[m_updateReactionQueue_UNKNOWN0].isProtectedByShield = newEnemy->IsProtectedByShield ();
	}
	else
	{
		m_recognizedEnemiesInfo[m_updateReactionQueue_UNKNOWN0].handle = NULL;
		m_recognizedEnemiesInfo[m_updateReactionQueue_UNKNOWN0].isReloading = false;
		m_recognizedEnemiesInfo[m_updateReactionQueue_UNKNOWN0].isProtectedByShield = false;
	}

	float reactionTime (m_profile->reactionTime);
	const float maximumReactionTime (g_flBotFullThinkInterval * MaximumReactionQueue - 0.01f);

	if (reactionTime > maximumReactionTime)
		reactionTime = maximumReactionTime;

	char currentRecognizedEnemiesQueueIndex (static_cast <char> (m_updateReactionQueue_UNKNOWN0) - static_cast <char> (reactionTime / g_flBotFullThinkInterval + 0.5f));

	if (currentRecognizedEnemiesQueueIndex < 0)
		currentRecognizedEnemiesQueueIndex += MaximumReactionQueue;

	m_currentRecognizedEnemiesQueueIndex = static_cast <unsigned char> (currentRecognizedEnemiesQueueIndex);

	if (++m_updateReactionQueue_UNKNOWN0 >= MaximumReactionQueue)
		m_updateReactionQueue_UNKNOWN0 = 0u;

	if (m_updateReactionQueue_UNKNOWN1 < MaximumReactionQueue)
		++m_updateReactionQueue_UNKNOWN1;
}

BaseEntity *const CCSBot::GetRecognizedEnemy (void) const
{
	if (m_currentRecognizedEnemiesQueueIndex >= m_updateReactionQueue_UNKNOWN1)
		return NULL;

	return m_recognizedEnemiesInfo[m_currentRecognizedEnemiesQueueIndex].handle;
}

const bool CCSBot::IsRecognizedEnemyReloading (void) const
{
	if (m_currentRecognizedEnemiesQueueIndex >= m_updateReactionQueue_UNKNOWN1)
		return false;

	return m_recognizedEnemiesInfo[m_currentRecognizedEnemiesQueueIndex].isReloading;
}

const bool CCSBot::IsRecognizedEnemyProtectedByShield (void) const
{
	if (m_currentRecognizedEnemiesQueueIndex >= m_updateReactionQueue_UNKNOWN1)
		return false;

	return m_recognizedEnemiesInfo[m_currentRecognizedEnemiesQueueIndex].isProtectedByShield;
}

void CCSBot::EquipBestWeapon (const bool ignoreLastEquipTimestamp/* = false*/)
{
	if (!ignoreLastEquipTimestamp && m_weaponEquipTimestamp > 0.0f && gpGlobals->time - m_weaponEquipTimestamp < 5.0f)
		return;

	BasePlayerItem *const primaryWeapon (m_rgpPlayerItems[Consntants::WeaponSlotID_PrimaryWeapons + 1u]);

	if (primaryWeapon != NULL)
	{
		const Consntants::WeponClass_t weaponClass (WeaponIDToWeaponClass(primaryWeapon->m_iId));

		if
		(
			(
				(cv_bot_allow_shotguns.value > 0.0f && weaponClass == Consntants::WeponClass_Shotgun) ||
				(cv_bot_allow_machine_guns.value > 0.0f && weaponClass == Consntants::WeponClass_MachineGun) ||
				(cv_bot_allow_rifles.value > 0.0f && weaponClass == Consntants::WeponClass_Rifle) ||
				(cv_bot_allow_shotguns.value > 0.0f && weaponClass == Consntants::WeponClass_Shotgun) ||
				(cv_bot_allow_snipers.value > 0.0f && weaponClass == Consntants::WeponClass_SniperRifle) ||
				(cv_bot_allow_sub_machine_guns.value > 0.0f && weaponClass == Consntants::WeponClass_SubMachineGun) ||
				(cv_bot_allow_shield.value > 0.0f && primaryWeapon->m_iId == 99)
			) &&
			DoEquip (primaryWeapon)
		)
			return;
	}

	// Below is just analog of code "if (!EquipPistol ())", but without check weapon equip timestamp.
	if (cv_bot_allow_pistols.value > 0.0f)
	{
		BasePlayerItem *const secondaryWeapon (m_rgpPlayerItems[Consntants::WeaponSlotID_SecondaryWeapons + 1u]);

		if (secondaryWeapon != NULL && (secondaryWeapon->m_iClip != 0 || m_rgAmmo[secondaryWeapon->m_iPrimaryAmmoType] > 0))
		{
			SelectItem (secondaryWeapon->GetClassName ());

			m_weaponEquipTimestamp = gpGlobals->time;

			return;
		}
	}

	EquipKnife ();
}

void CCSBot::HuntState::OnEnter (CCSBot *const bot)
{
	if (bot->IsUsingKnife () && bot->IsWellPastSafe () && !bot->IsHurrying ())
		bot->Walk ();
	else
		bot->Run ();

	bot->StandUp ();
	bot->SetDisposition (DispositionType_EngageAndInvestigate);
	bot->SetTask (TaskType_SeekAndDestroy);
	bot->DestroyPath ();
}

void CCSBot::HuntState::OnUpdate (CCSBot *const bot)
{
	if (gpGlobals->time - bot->m_setStateTimestamp > 30.0f)
	{
		bot->PrintIfWatched ("Giving up hunting, and being a rogue\n");

		bot->m_isRogue = false;

		bot->Idle ();

		return;
	}

	if (TheBots->m_mapType == BotManager::MapType_De)
	{
		if (bot->m_team == Constants::TeamID_Terrorist)
		{
			if (bot->m_isCanPlantBomb && (gpGlobals->time >= TheBots->m_timeToPlantBomb || (bot->m_clientMapZone & Constants::IN_BOMB_PLACE) && bot->GetTimeSinceLastSawEnemy () > 3.0f))
			{
				bot->Idle ();

				return;
			}

			if (bot->NoticeLooseBomb ())
			{
				bot->FetchBomb ();

				return;
			}

			if (!bot->IsRogue () && bot->m_bombState == BombState_Planted && bot->m_gameState.GetBombPosition ())
			{
				bot->SetTask (TaskType_GuardTickingBomb);
				bot->Hide (TheNavAreaGrid->GetNavArea (bot->m_gameState.GetBombPosition (), 120.0f), -1.0f, 750.0f, false);

				return;
			}
		}
		else
		{
			if (!bot->IsRogue () && bot->CanSeeLooseBomb ())
			{
				bot->m_currentTaskID = TaskType_GuardLooseBomb;
				bot->m_taskEntity = NULL;

				bot->Hide (TheBots->m_looseBombArea, -1.0f, 750.0f, false);
				bot->m_chatter.AnnouncePlan ("GoingToGuardLooseBomb", TheBots->m_looseBombArea->GetPlace ());

				return;
			}

			if (TheBots->m_isBombPlanted && (!bot->IsRogue () || !TheBots->m_bombDefuser))
			{
				bot->Idle ();

				return;
			}
		}
	}
	else if (TheBots->m_mapType == BotManager::MapType_Cs && bot->m_team == Constants::TeamID_Terrorist)
	{
		if (bot->m_gameState.AreAllHostagesBeingRescued () && bot->GuardRandomZone (500.0f))
		{
			bot->m_currentTaskID = TaskType_GuardingRescueZone;
			bot->m_taskEntity = NULL;

			bot->PrintIfWatched ("Trying to beat them to an escape zone!\n");
			bot->SetDisposition (DispositionType_OpportunityFire);
			bot->m_chatter.GuardingHostageEscapeZone (true);

			return;
		}

		if (!bot->IsRogue () && !bot->IsSafe ())
		{
			const Hostage *const nearestVisibleFreeHostage (bot->m_gameState.GetNearestVisibleFreeHostage ());

			if (nearestVisibleFreeHostage != NULL)
			{
				NavigationMesh::NavigationArea *const area (TheNavAreaGrid->GetNearestNavArea (nearestVisibleFreeHostage->GetOrigin (), false));

				if (area != NULL)
				{
					bot->m_currentTaskID = TaskType_GuardingHostages;
					bot->m_taskEntity = NULL;
					bot->Hide (area, -1.0f, 750.0f, false);
					bot->PrintIfWatched ("I'm guarding hostages\n");
					bot->m_chatter.GuardingHostages (area->GetPlace (), true);

					return;
				}
			}
		}
	}

	if (bot->ShouldInvestigateNoise (NULL))
	{
		bot->InvestigateNoise ();

		return;
	}

	bot->UpdateLookAround ();

	if (bot->m_lastKnownArea != m_huntArea && !bot->UpdatePathMovement (true))
		return;

	m_huntArea = NULL;

	unsigned int areasNumber (0u);
	float bestClearedTimeAmount (0.0f);

	for (const Node_t *node (TheNavAreaList.m_head->next); node != TheNavAreaList.m_head; node = node->next)
	{
		NavigationMesh::NavigationArea *const area (node->element);

		++areasNumber;

		if (area->GetExtent ().GetArea2D () < MATH_GET_SQUARED (150.0f))
			continue;

		const float clearedTimeAmount (gpGlobals->time - area->m_clearedTimestamp[bot->m_team - 1u]);

		if (clearedTimeAmount > bestClearedTimeAmount)
		{
			bestClearedTimeAmount = clearedTimeAmount;
			m_huntArea = area;
		}
	}

	unsigned int which (g_engfuncs.pfnRandomLong (0u, areasNumber - 1u));

	for (const Node_t *node (TheNavAreaList.m_head->next); node != TheNavAreaList.m_head; node = node->next)
	{
		m_huntArea = node->element;

		if (which == 0u)
			break;

		--which;
	}

	bot->ComputePath (m_huntArea, NULL, NavigationMesh::SAFEST_ROUTE);
}

void CCSBot::AttackState::OnEnter (CCSBot *const bot)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	bot->PushPostureContext ();
	bot->DestroyPath ();

	if (bot->m_enemy && bot->IsUsingKnife () && !bot->IsPlayerFacingMe (bot->m_enemy))
		bot->Walk ();
	else
		bot->Run ();

	bot->GetOffLadder ();
	bot->ResetStuckMonitor ();
	*(_DWORD *)(this + 16) = -1.0f;
	*(_BYTE *)(this + 24) = bot->m_isEnemyVisible;
	*(_DWORD *)(this + 8) = 0;
	*(_BYTE *)(this + 47) = true;
	*(_BYTE *)(this + 25) = false;
	*(_DWORD *)(this + 28) = 0;
	*(float *)(this + 40) = gpGlobals->time + g_engfuncs.pfnRandomFloat (1088421888, 10.0f);
	*(_BYTE *)(this + 36) = false;
	*(float *)(this + 32) = gpGlobals->time + g_engfuncs.pfnRandomFloat (2.0f, 10.0f);

	if (bot->IsEscapingFromBomb ())
		bot->EquipBestWeapon (false);

	if (bot->IsUsingKnife ())
	{
		*(_BYTE *)(this + 44) = false;

		bot->StandUp ();
	}
	else
	{
		if (*(_BYTE *)(this + 44))
			goto LABEL_41;

		if (bot->m_enemy)
		{
			if (bot->IsUsingSniperRifle ())
				v26 = 50.0f;
			else
			{
				if (bot->GetOrigin ().GetDistanceSuqared (bot->m_enemy->GetOrigin ()) <= MATH_GET_SQUARED (750.0f))
					v26 = (1.0f - bot->m_profile->aggression) * 20.0f;
				else
					v26 = 50.0f;
			}

			if (g_engfuncs.pfnRandomFloat (0.0f, 100.0f) < v26)
			{
				Structures::TraceResult_t traceResult;

				UTIL_TraceLine (bot->m_doCrouch ? bot->GetOrigin () : bot->GetOrigin () - Math::Vector3D (0.0f, 0.0f, 20.0f), bot->m_enemy->EyePosition (), ignore_monsters, ignore_glass, bot->GetEdict (), &traceResult);

				if (traceResult.fraction == 1.0f)
					*(_BYTE *)(this + 44) = true;
			}
		}

		if (*(_BYTE *)(this + 44))
		{
LABEL_41:
			bot->Crouch ();
			bot->PrintIfWatched ("Crouch and hold attack!\n");
		}
	}
	*(_DWORD *)(this + 20) = 0;
	*(_BYTE *)(this + 45) = false;
	LODWORD(v17) = bot->m_profile->skill;
	v27 = v17 * 80.0f;
	if (bot->IsUsingKnife ())
		v27 *= 2.0f;
	if (v17 > 0.5f && bot->IsOutnumbered ())
		v27 = 100.0f;
	*(_BYTE *)(this + 46) = v27 > g_engfuncs.pfnRandomFloat (0.0f, 100.0f);
	*(_BYTE *)(this + 48) = bot->m_profile->aggression * 100.0f < g_engfuncs.pfnRandomFloat (0.0f, 100.0f);
}

void CCSBot::AttackState::OnUpdate (CCSBot *const bot)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	bot->ResetStuckMonitor ();
	v4 = bot->m_pActiveItem;
	bot->m_isRapidFiring = false;
	if (v4 != NULL)
	{
		v5 = *(_DWORD *)(v4 + 172);
		if (v5 == 6 || v5 == 4 || v5 == 9 || v5 == 25)
			bot->EquipBestWeapon (false);
	}
	v6 = bot->m_enemy;
	if (!v6)
	{
		if (bot->m_currentTaskID == TaskType_Sniping)
		{
LABEL_9:
			bot->Hide (bot->m_lastKnownArea, -1.0f, 50.0, 0);
			return;
		}
LABEL_100:
		bot->StopAttacking (this);
		return;
	}
	if (!*(_BYTE *)(this + 24))
		*(_BYTE *)(this + 24) = bot->m_isEnemyVisible;
	if (gpGlobals->time > *(float *)(this + 56))
	{
		if (gpGlobals->time > *(float *)(this + 40))
			bot->m_chatter.PinnedDown ();
		else
		{
			if (!bot->IsOutnumbered () || !*(_BYTE *)(this + 48))
			{
				if (bot->OutnumberedCount () < 2u || bot->m_profile->aggression >= 1.0f)
					goto LABEL_24;
			}
			bot->m_chatter.Scared ();
		}
		v7 = g_engfuncs.pfnRandomFloat(1077936128, 1097859072);
		*(float *)(this + 56) = v7 + gpGlobals->time;
		*(float *)(this + 52) = v7;
		if (bot->TryToRetreat ())
		{
			if (bot->IsUsingSniperRifle ())
				bot->EquipPistol ();
		}
		else
			bot->PrintIfWatched ("I want to retreat, but no safe spots nearby!\n");
	}
LABEL_24:
	if (bot->IsUsingKnife ())
	{
		*(_BYTE *)(this + 44) = false;
		bot->StandUp ();
		if (bot->IsPlayerFacingMe (v6))
		{
			bot->ForceRun (5.0f);
			bot->m_hurryTimer.Start (10.0f);
		}
		else
			bot->Walk ();
		bot->FireWeaponAtEnemy ();
		if ((bot->m_currentPathInformationNumber == 0u || v6->GetOrigin ().GetDistanceSuqared (bot->m_path[m_currentPathInformationNumber - 1u].point) > MATH_GET_SQUARED (100.0f)) && gpGlobals->time > *(float *)(this + 16))
		{
			bot->ComputePath (TheNavAreaGrid.GetNearestNavArea (v6->GetOrigin (), false), v6->GetOrigin (), NavigationMesh::FASTEST_ROUTE);
			*(_DWORD *)(this + 12) = 0.5f;
			*(float *)(this + 16) = gpGlobals->time + 0.5f;
		}
		if (bot->UpdatePathMovement (false))
			bot->DestroyPath ();
		return;
	}
	if (bot->HasShield ())
	{
		if (!bot->m_isEnemyVisible || *(_BYTE *)(this + 36))
		{
			if (bot->IsProtectedByShield ())
			{
LABEL_49:
				bot->SecondaryAttack ();
				goto LABEL_50;
			}
		}
		else
		{
			if (bot->IsRecognizedEnemyReloading ()
				|| (v20 = bot->m_pActiveItem) != NULL && *(_DWORD *)(v20 + 216)
				|| !bot->IsPlayerLookingAtMe (v6))
			{
				if (bot->IsProtectedByShield ())
					bot->SecondaryAttack ();
				goto LABEL_50;
			}
			if (!bot->IsProtectedByShield ())
				goto LABEL_49;
		}
LABEL_50:
		if (gpGlobals->time > *(float *)(this + 32))
		{
			*(_BYTE *)(this + 36) = *(_BYTE *)(this + 36) == false;
			*(float *)(this + 32) = gpGlobals->time + g_engfuncs.pfnRandomFloat(0.5f, 2.0f);
		}
	}
	if (!bot->IsUsingSniperRifle ())
	{
		if (!bot->IsUsingShotgun ()
			|| (v25 = v6->GetOrigin (),
					v26 = bot->GetOrigin (),
					v59 = *(float *)v25 - *(float *)v26,
					v60 = *(float *)(v25 + 4) - *(float *)(v26 + 4),
					v61 = *(float *)(v25 + 8) - *(float *)(v26 + 8),
					Vector__operator=Vector((int)&v56, (int)&v59),
					v58 * v58 + v57 * v57 + v56 * v56 <= MATH_GET_SQUARED (1000.0f)))
			goto LABEL_58;
		goto LABEL_57;
	}
	v23 = v6->GetOrigin ();
	v24 = bot->GetOrigin ();
	v59 = *(float *)v23 - *(float *)v24;
	v60 = *(float *)(v23 + 4) - *(float *)(v24 + 4);
	v61 = *(float *)(v23 + 8) - *(float *)(v24 + 8);
	Vector__operator=Vector((int)&v56, (int)&v59);
	if (v56 * v56 + v58 * v58 + v57 * v57 < MATH_GET_SQUARED (310.0f))
LABEL_57:
		bot->EquipPistol ();
LABEL_58:
	if (!bot->IsUsingSniperRifle ())
		goto LABEL_152;
	if (bot->m_resumeZoom)
	{
		*(float *)(this + 20) = gpGlobals->time;
		return;
	}
	if (bot->m_iFOV > 60 && bot->AdjustZoom (bot->GetOrigin ().GetDistance (bot->m_enemyShootPosition)))
		*(float *)(this + 20) = gpGlobals->time;
	if (bot->m_profile->reactionTime + 0.2f <= gpGlobals->time - *(float *)(this + 20))
	{
LABEL_152:
		if (bot->m_enemyDeathTimestamp != 0.0f)
		{
			if (!bot->m_enemy)
				|| (v29 = bot->m_enemy, !v29->IsAlive ())
				&& 1.0f - bot->m_profile->skill < gpGlobals->time - bot->m_enemyDeathTimestamp)
			{
				if (bot->m_lastVictimID == g_engfuncs.pfnIndexOfEdict (v6->GetEdict ()) && bot->GetNearbyEnemyCount () <= 1u)
					bot->m_chatter.KilledMyEnemy (g_engfuncs.pfnIndexOfEdict (v6->GetEdict ()));

				if (bot->m_currentTaskID == TaskType_Sniping)
				{
					bot->Hide (bot->m_lastKnownArea, -1.0f, 50.0, 0);
					return;
				}
				goto LABEL_100;
			}
		}
		const float seeEnemyTime (bot->GetTimeSinceLastSawEnemy ());
		if (bot->m_isEnemyVisible)
		{
			*(_BYTE *)(this + 45) = false;
			if (*(_BYTE *)(this + 25))
			{
				*(_BYTE *)(this + 25) = false;
				*(float *)(this + 28) = gpGlobals->time + bot->m_profile->reactionTime;
			}
		}
		else
		{
			if (seeEnemyTime > 0.5f && bot->CanHearNearbyEnemyGunfire (-1.0f))
			{
				if (bot->m_currentTaskID == TaskType_Sniping)
					bot->Hide (bot->m_lastKnownArea, -1.0f, 50.0, 0);
				else
					bot->StopAttacking (this);
				if (bot->m_noiseHearGlobalTimeDuration > 0.0f)
				{
					bot->SetLookAt ("Nearby enemy gunfire", bot->m_noisePosition, PriorityType_High, 0.0f, false, 5.0f);
					bot->PrintIfWatched ("Checking nearby threatening enemy gunfire!\n");
				}
				return;
			}
			if (seeEnemyTime > 0.25f)
				*(_BYTE *)(this + 25) = true;
			if (seeEnemyTime > 0.1f)
			{
				if (bot->GetDisposition () != DispositionType_EngageAndInvestigate)
				{
					if (bot->m_currentTaskID == TaskType_Sniping)
					{
						bot->Hide (bot->m_lastKnownArea, -1.0f, 50.0, 0);
						return;
					}
					goto LABEL_100;
				}
				if (*(_BYTE *)(this + 24) && !*(_BYTE *)(this + 45))
				{
					if (g_engfuncs.pfnRandomFloat(0.0f, 100.0f) < 33.3f)
					{
						v33 = FindNearbyRetreatSpot (200.0);
						if (v33)
						{
							bot->IgnoreEnemies (1.0);
							bot->Run ();
							bot->StandUp ();
							bot->Hide (v33, g_engfuncs.pfnRandomFloat(1077936128, 1097859072), 1);
							return;
						}
					}
					*(_BYTE *)(this + 45) = true;
				}
			}
		}
		v37 = 1.0f - bot->m_profile->aggression + 1.0f;
		v37 *= 2.0f;
		if (bot->IsUsingSniperRifle ())
			v37 += 3.0f;
		else if (bot->m_doCrouch)
			v37 += 1.0f;

		if (bot->m_isEnemyVisible || seeEnemyTime <= v37 && *(_BYTE *)(this + 24))
		{
			if (!bot->m_isEnemyVisible && bot->GetTimeSinceAttacked () < 3.0f && bot->GetAttacker ())
			{
				if (bot->GetAttacker () != bot->m_enemy)
				{
					if (bot->IsVisible (bot->GetAttacker (), true, NULL))
					{
						bot->Attack (bot->GetAttacker ());
						bot->PrintIfWatched ("Switching targets to retaliate against new attacker!\n");
					}
					return;
				}
			}
			if (gpGlobals->time > *(float *)(this + 28))
				bot->FireWeaponAtEnemy ();
			if (*(_BYTE *)(this + 46) && !bot->IsUsingSniperRifle () && !*(_BYTE *)(this + 44))
			{
				v43 = *(float *)(v6->GetOrigin ()) - *(float *)(*(_DWORD *)(bot + 4) + 8);
				v44 = *(float *)(*(_DWORD *)(v6 + 4) + 12) - *(float *)(*(_DWORD *)(bot + 4) + 12);
				v64 = sqrt(v44 * v44 + v43 * v43);
				if (bot->m_profile->skill < 0.66f || !bot->m_isEnemyVisible)
				{
					if (v64 > bot->m_combatRange + 125.0f)
						bot->MoveForward ();
					else if (v64 < bot->m_combatRange - 125.0f)
						bot->MoveBackward ();
				}
				if (v64 <= 2000.0f && bot->IsPlayerFacingMe (v6))
				{
					if (gpGlobals->time >= *(float *)(this + 8))
					{
						do
						{
							if (!*(_BYTE *)(this + 47)
								|| bot->m_profile->skill <= 0.5f
								|| g_engfuncs.pfnRandomFloat(0.0f, 100.0f) >= 33.3f
								|| (v46 = *(float *)(*(_DWORD *)(bot + 4) + 32), UNDEF(v47), v48))
								v53 = 2;
							else
								v53 = 3;
							v49 = g_engfuncs.pfnRandomLong(0, v53);
						}
						while (!*(_BYTE *)(this + 47) && v49 == *(_DWORD *)(this + 4));
						*(_DWORD *)(this + 4) = v49;
						*(_BYTE *)(this + 47) = false;
						*(float *)(this + 8) = gpGlobals->time + g_engfuncs.pfnRandomFloat(1050253722, 1065353216);
					}
				}
				else
				{
					*(_DWORD *)(this + 4) = 0;
					*(_DWORD *)(this + 8) = 0;
				}
				switch (*(_DWORD *)(this + 4))
				{
					case 1:
						bot->StrafeLeft ();

						break;

					case 2:
						bot->StrafeRight ();

						break;

					case 3:
						if (bot->m_isEnemyVisible)
							bot->Jump (false);

						break;
				}
			}
		}
		else
		{
			if (bot->m_currentTaskID == TaskType_Sniping)
				goto LABEL_9;
			bot->m_currentTaskID = TaskType_MoveToLastKnownEnemyPosition;
			bot->m_taskEntity = v6;
			bot->MoveTo (bot->m_lastKnownEnemyPosition, NavigationMesh::SAFEST_ROUTE);
		}
	}
}

void CCSBot::AttackState::OnExit (CCSBot *const bot)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	bot->PrintIfWatched ("AttackState:OnExit()\n");

	*((_BYTE *)this + 44) = false;

	bot->m_noiseHearGlobalTimeDuration = 0.0f;

	bot->ResetStuckMonitor ();
	bot->PopPostureContext ();

	if (bot->IsProtectedByShield ())
		bot->SecondaryAttack ();

	bot->m_isRapidFiring = false;
	bot->m_surpriseTime = 0.0f;
	bot->m_surprisedTime = 0.0f;
}

void CCSBot::IdleState::OnEnter (CCSBot *const bot)
{
	bot->DestroyPath ();
	bot->SetEnemy (NULL);

	if (bot->IsUsingKnife () && bot->IsWellPastSafe () && !bot->IsHurrying ())
		bot->Walk ();

	bot->m_currentTaskID = TaskType_SeekAndDestroy;
	bot->m_taskEntity = NULL;
	bot->SetDisposition (DispositionType_EngageAndInvestigate);
}

template <typename CostFunctor> const CBotManager::MapZone_t *const CCSBotManager::GetClosestZone/*! @warning NOT ORIGINAL NAME!!! */ (NavigationArea *const area, const CostFunctor &functor/*, float *const outputClosestDistance - SOURCE ENGINE RELATED*/)
{
	if (area == NULL)
		return NULL;

	float closestDistance (100000000.0f);
	MapZone_t *const zone (NULL);

	for (unsigned char index (0u); index < m_mapZonesNumber; ++index)
	{
		if (m_mapZones[index].containingAreasNumber == 0u/*(FROM NEW SOURCE ENGINE) || m_mapZones[index].isBlocked*/)
			continue;

		const float distance (NavAreaTravelDistance (area, m_mapZones[index].containingAreas[0u], functor));

		if (/*(FROM NEW SOURCE ENGINE) distance >= 0.0f &&*/closestDistance > distance)
		{
			closestDistance = distance;
			zone = m_mapZones + index;
		}
	}
/*SOURCE ENGINE RELATED
	if (outputClosestDistance != NULL)
		*outputClosestDistance = closestDistance;
*/
	return zone;
}

void CCSBot::IdleState::OnUpdate (CCSBot *const bot)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (bot->m_lastKnownArea == NULL && !bot->StayOnNavMesh ())
		return;

	if (cv_bot_zombie.value > 0.0f)
	{
		bot->ResetStuckMonitor ();

		return;
	}

	if (bot->IsSafe () && !bot->EquipGrenade (false) && bot->m_profile->skill > 0.33f)
		bot->EquipKnife ();

	if (bot->m_gameState.IsRoundOver ())
	{
		if (bot->m_hostageEscortCount > 0u)
		{
			bot->m_currentTaskID = TaskType_RescueHostages;
			bot->m_taskEntity = NULL;
			bot->Run ();
			bot->SetDisposition (DispositionType_SelfDefense);
			v9 = (int)TheBots->GetRandomPositionInZone (TheBots->GetClosestZone (bot->m_lastKnownArea, PathCost (bot, NavigationMesh::FASTEST_ROUTE)/*, NULL - SOURCE ENGINE RELATED*/));
			bot->MoveTo (v9/*MAY BE NULL - SO POTENTIAL CRASH!!!*/, NavigationMesh::FASTEST_ROUTE);
			bot->PrintIfWatched ("Trying to rescue hostages at the end of the round\n");
			return;
		}
LABEL_252:
		bot->Hunt ();
		return;
	}
	if (bot->m_isFollowing)
	{
		bot->ContinueFollowing ();

		return;
	}
	if (TheBots->m_mapType == BotManager::MapType_De)
	{
		if (bot->m_team == Consntants::TeamID_Terrorist)
		{
			if (bot->m_bombState == BombState_Planted)
			{
				const char plantedBombsiteZoneIndex (bot->m_gameState.GetPlantedBombsite ());

				if (plantedBombsiteZoneIndex != -1)
				{
					const CBotManager::MapZone_t *const plantedBombsiteZone (TheBots->m_mapZones + plantedBombsiteZoneIndex);

					if ((char *)TheBots + 116 * plantedBombsiteZoneIndex != (void *)-20)
					{
						bot->m_currentTaskID = TaskType_GuardTickingBomb;
						bot->m_taskEntity = NULL;

						const NavigationMesh::Place_t place (TheNavAreaGrid.GetPlace (plantedBombsiteZone->center));
						const Math::Vector3D *spot;

						if (place != NavigationMesh::UNDEFINED_PLACE && (spot = FindRandomHidingSpot (bot, place, bot->IsSniper ())) != NULL)
							bot->Hide (*spot, -1.0f, false);
						else
							bot->Hide (TheNavAreaGrid.GetNearestNavArea (plantedBombsiteZone->center, false), -1.0f, 750.0f, false);

						return;
					}
					goto LABEL_252;
				}
				bot->m_chatter.RequestBombLocation ();
				v83 = (int)TheBots->GetRandomPositionInZone (TheBots->m_mapZones[bot->m_gameState.GetNextBombsiteToSearch ()]);
				if (v83 == NULL)
					goto LABEL_252;
				bot->m_currentTaskID = TaskType_FindTickingBomb;
LABEL_231:
				bot->m_taskEntity = NULL;
				bot->MoveTo(v83, NavigationMesh::SAFEST_ROUTE);
				return;
			}
			if (bot->m_isCanPlantBomb)
			{
				if (bot->m_clientMapZone & Constants::IN_BOMB_PLACE)
				{
					bot->m_currentTaskID = TaskType_PlantingBomb;
					bot->m_taskEntity = NULL;
					bot->PlantBomb ();
					bot->m_chatter.PlantingTheBomb (bot->GetPlace ());
					return;
				}
				if (gpGlobals->time >= TheBots->m_timeToPlantBomb)
				{
					v89 = (int)TheBots->GetRandomPositionInZone (TheBots->GetClosestZone (bot->m_lastKnownArea, PathCost (bot, NavigationMesh::SAFEST_ROUTE)/*, NULL - SOURCE ENGINE RELATED*/));
					if (v89 != NULL)
					{
						bot->m_currentTaskID = TaskType_PlantingBomb;
						bot->m_taskEntity = NULL;
						bot->Run ();
						bot->MoveTo (v89, NavigationMesh::SAFEST_ROUTE);
						return;
					}
				}
				goto LABEL_252;
			}
			if (bot->GetFriendsRemaining () > 0u && bot->IsSniper () && g_engfuncs.pfnRandomFloat (0.0f, 100.0f) < 10.0f)
			{
				bot->m_currentTaskID = TaskType_MoveToSniperSpot;
				bot->m_taskEntity = NULL;
				v10 = bot->m_lastKnownArea;
				v125 = false;
				v122 = 2000.0f;
				goto LABEL_30;
			}
			if (bot->NoticeLooseBomb ())
			{
				bot->FetchBomb ();
				return;
			}
			if (bot->IsRogue () || bot->m_bombState != BombState_Planted || !bot->m_gameState.GetBombPosition ())
				goto LABEL_252;
			bot->m_currentTaskID = TaskType_GuardTickingBomb;
			bot->m_taskEntity = NULL;
			v127 = false;
			LODWORD(v124) = 750.0f;
			v121 = -1.0f;
			LODWORD(v119) = 120.0f;
			v117 = bot->m_gameState.GetBombPosition ();
LABEL_199:
			v91 = TheNavAreaGrid.GetNavArea(v117, v119);
			bot->Hide(v91, v121, v124, v127);
			return;
		}
		if (bot->m_bombState == BombState_Planted)
		{
			v90 = bot->m_gameState.GetBombPosition ();
			if (v90)
			{
				if (TheBots->m_bombDefuser == NULL)
				{
					if (bot->IsDoingScenario ())
					{
						bot->m_currentTaskID = TaskType_DefusingBomb;
						bot->m_taskEntity = NULL;
						bot->SetDisposition (DispositionType_OpportunityFire);
						bot->MoveTo(v90, NavigationMesh::SAFEST_ROUTE);
						return;
					}
					bot->m_currentTaskID = TaskType_GuardingBombsite;
					bot->m_taskEntity = NULL;
					v126 = false;
					v123 = 750.0f;
					v120 = -1.0f;
					v34 = TheNavAreaGrid.GetNavArea(v90, 120.0f);
					goto LABEL_84;
				}
				if (!bot->IsRogue ())
				{
					bot->m_currentTaskID = TaskType_GuardBombDefuser;
					bot->m_taskEntity = NULL;
					v127 = false;
					LODWORD(v124) = 750.0f;
					v121 = -1.0f;
					LODWORD(v119) = 120.0f;
					v117 = v90;
					goto LABEL_199;
				}
			}
			else
			{
				if (bot->m_gameState.GetPlantedBombsite () == -1)
				{
					v129 = 0;
					*(float *)&v135 = 10000000.0f;
					v133 = 0;
					if (TheBots->m_mapZonesNumber > 0)
					{
						v94 = (int)((char *)TheBots + 92);
						do
						{
							if (*(_DWORD *)(v94 - 4) && !bot->m_gameState.IsBombsiteClear (v133))
							{
								v95 = bot->m_lastKnownArea;
								v96 = TheNavAreaGrid.GetNearestNavArea(v94, false);
								v97 = v96;
								if (v95 != NULL)
								{
									if (v96 != NULL)
									{
										if (v95 == v96)
										{
											this = 0.0f;
										}
										else
										{
											if (NavAreaBuildPath__H1Z16ShortestPathCost_P8CNavAreaT0PC6VectorRX01PP8CNavArea_b(v95, v96, 0, (int)&bot, 0))
											{
												v98 = *(_DWORD *)(v97 + 432);
												v99 = v97;
												v100 = v97 + 432;
												for (i = 0.0; v98; i = v105 + i)
												{
													*(float *)&v101 = *(float *)(v99 + 36) - *(float *)(v98 + 36);
													*(float *)&v102 = *(float *)(v99 + 32) - *(float *)(v98 + 32);
													*(float *)&v103 = *(float *)(v99 + 28) - *(float *)(v98 + 28);
													v104 = Vector__operator=xyz((int)&v138, v103, v102, v101);
													Vector__operator=Vector((int)&v137, v104);
													v105 = VectorLength((int)&v137);
													v99 = *(_DWORD *)v100;
													v98 = *(_DWORD *)(*(_DWORD *)v100 + 432);
													v100 = *(_DWORD *)v100 + 432;
												}
												this = i;
											}
											else
											{
												this = -1.0f;
											}
										}
									}
									else
									{
										this = -1.0f;
									}
								}
								else
								{
									this = -1.0f;
								}
								if (this < *(float *)&v135)
								{
									*(float *)&v135 = this;
									v129 = v94 - 72;
								}
							}
							v94 += 116;
							v106 = TheBots->m_mapZonesNumber;
							++v133;
						}
						while (v133 < v106);
						if (v129)
						{
							this = *(float *)&v135;
							if (*(float *)&v135 <= 2000.0f)
								goto LABEL_255;
						}
					}
					const unsigned char nextBombsiteToSearchZoneIndex (bot->m_gameState.GetNextBombsiteToSearch ());
					v129 = TheBots->m_mapZones + nextBombsiteToSearchZoneIndex;
					if ((char *)TheBots + 116 * nextBombsiteToSearchZoneIndex != (void *)-20)
					{
LABEL_255:
						v83 = (int)TheBots->GetRandomPositionInZone (v129);
						if (v83 != NULL)
						{
							bot->m_currentTaskID = TaskType_FindTickingBomb;
							goto LABEL_231;
						}
					}
				}
				else
				{
					const unsigned char plantedBombsiteZoneIndex (bot->m_gameState.GetPlantedBombsite ());
					v93 = TheBots->m_mapZones + plantedBombsiteZoneIndex;
					if ((char *)TheBots + 116 * plantedBombsiteZoneIndex != (void *)-20)
					{
						if (bot->IsDoingScenario ())
						{
							bot->m_currentTaskID = TaskType_DefusingBomb;
							bot->m_taskEntity = NULL;
							bot->MoveTo(v93->center, NavigationMesh::SAFEST_ROUTE);
							bot->SetDisposition (DispositionType_OpportunityFire);
							return;
						}
						bot->m_currentTaskID = TaskType_GuardingBombsite;
						bot->m_taskEntity = NULL;
						v126 = false;
						v123 = 750.0f;
						v120 = -1.0f;
						v34 = TheNavAreaGrid.GetNavArea(v93->center, 120.0f);
						goto LABEL_84;
					}
				}
			}
		}
		if (bot->IsSniper () && g_engfuncs.pfnRandomFloat(0.0f, 100.0f) <= 75.0f)
		{
			if (bot->m_gameState.IsLooseBombLocationKnown ())
			{
				v108 = bot->m_gameState.GetBombPosition ();
				v109 = TheNavAreaGrid.GetNearestNavArea(v108, false);
				bot->PrintIfWatched("Sniping near loose bomb\n");
				goto LABEL_239;
			}
			if (TheBots->m_mapZonesNumber > 0u)
			{
				v112 = TheBots->m_mapZones + g_engfuncs.pfnRandomLong (0u, TheBots->m_mapZonesNumber - 1u);
				if (v112 != NULL)
				{
					v109 = TheBots->GetRandomAreaInZone (v112);
					bot->PrintIfWatched ("Sniping near bombsite\n");
LABEL_239:
					if (v109 != NULL)
					{
						bot->m_currentTaskID = TaskType_MoveToSniperSpot;
						bot->m_taskEntity = NULL;
						v126 = false;
						v123 = 2000.0f;
						v120 = -1.0f;
						v118 = v109;
						goto LABEL_85;
					}
					goto LABEL_242;
				}
			}
		}
LABEL_242:
		if (bot->IsRogue ()
			|| TheBots->m_isDefenseRushing
			|| bot->m_gameState.IsLooseBombLocationKnown ()
			|| !bot->IsSafe () && !bot->HasNotSeenEnemyForLongTime ()
			|| bot->m_morale * -34.0 <= g_engfuncs.pfnRandomFloat (0.0f, 100.0f)
			|| (v114 = (bot->m_morale + 3) * 100.0f + 500.0f, TheBots->m_mapZonesNumber == 0u)
			|| (v115 = TheBots->m_mapZones + g_engfuncs.pfnRandomLong (0u, TheBots->m_mapZonesNumber - 1u)) == NULL
			|| (v116 = TheBots->GetRandomAreaInZone (v115)) == NULL)
			goto LABEL_252;
		bot->PrintIfWatched("I'm guarding a bombsite\n");
		bot->m_chatter.AnnouncePlan ("GoingToDefendBombsite", *(_DWORD *)(v116 + 44));
		bot->m_currentTaskID = TaskType_GuardingBombsite;
		bot->m_taskEntity = NULL;
		v126 = false;
		LODWORD(v123) = v114;
		v120 = -1.0f;
		v118 = v116;
LABEL_85:
		bot->Hide(v118, v120, v123, v126);
		bot->SetDisposition (DispositionType_OpportunityFire);
		return;
	}
	if (TheBots->m_mapType == BotManager::MapType_Cs)
	{
		if (bot->m_team == Consntants::TeamID_Terrorist)
		{
			if (bot->IsSafe ())
				v32 = true;
			else
				v32 = !bot->m_gameState.m_UNKNOWN1
					 && !bot->m_gameState.AreAllHostagesBeingRescued ()
					 && g_engfuncs.pfnRandomFloat(0.0f, 100.0f) > (gpGlobals->time - TheBots->m_roundStartTime - bot->m_safeEndTime) * 0.83333331;
			if (bot->IsSniper () && g_engfuncs.pfnRandomFloat(0.0f, 100.0f) <= 75.0f)
			{
				v33 = bot->m_gameState.GetRandomFreeHostagePosition ();
				if (v33 && v32)
				{
					bot->m_currentTaskID = TaskType_MoveToSniperSpot;
					bot->m_taskEntity = NULL;
					bot->PrintIfWatched("Sniping near hostages\n");
					v126 = false;
					v123 = 2000.0f;
					v120 = -1.0f;
					v34 = TheNavAreaGrid.GetNearestNavArea(v33, false);
LABEL_84:
					v118 = v34;
					goto LABEL_85;
				}
				if (bot->GuardRandomZone (2000.0f))
				{
					bot->m_currentTaskID = TaskType_MoveToSniperSpot;
					bot->m_taskEntity = NULL;
					bot->PrintIfWatched("Sniping near a rescue zone\n");
					bot->SetDisposition (DispositionType_OpportunityFire);
					return;
				}
			}
			if (!bot->IsSafe () && !bot->IsRogue ())
			{
				const Hostage *const nearestVisibleFreeHostage (bot->m_gameState.GetNearestVisibleFreeHostage ());

				if (nearestVisibleFreeHostage != NULL)
				{
					v36 = TheNavAreaGrid.GetNearestNavArea (nearestVisibleFreeHostage->GetOrigin (), false);
					if (v36 != NULL)
					{
						bot->m_currentTaskID = TaskType_GuardingHostages;
						bot->m_taskEntity = NULL;
						bot->Hide(v36, -1.0f, 750.0f, false);
						bot->PrintIfWatched("I'm guarding hostages I found\n");
						return;
					}
				}
			}
			if (bot->GetFriendsRemaining () > 0u)
			{
				if (bot->IsRogue ()
					|| TheBots->m_isDefenseRushing
					|| bot->m_morale * 25.0f + 70.0f > g_engfuncs.pfnRandomFloat(0.0f, 100.0f))
					goto LABEL_252;
			}
			v37 = bot->m_gameState.GetRandomFreeHostagePosition ();
			if (!v37 || !v32 || (v38 = TheNavAreaGrid.GetNearestNavArea(v37, false)) == NULL)
			{
				if (bot->GuardRandomZone (500.0f))
				{
					bot->m_currentTaskID = TaskType_GuardingRescueZone;
					bot->m_taskEntity = NULL;
					bot->PrintIfWatched("I'm guarding a rescue zone\n");
					bot->SetDisposition (DispositionType_OpportunityFire);
					bot->m_chatter.GuardingHostageEscapeZone(true);
					return;
				}
				goto LABEL_252;
			}
			bot->m_currentTaskID = TaskType_GuardingHostages;
			bot->m_taskEntity = NULL;
			bot->PrintIfWatched("I'm guarding hostages\n");
			v39 = (bot->m_morale + 3) * 250.0f + 750.0f;
			v40 = v39;
			bot->Hide(v38, -1.0f, v40, false);
			bot->SetDisposition (DispositionType_OpportunityFire);
			if (g_engfuncs.pfnRandomFloat (0.0f, 100.0f) < 50.0f)
				bot->m_chatter.GuardingHostages(*(_DWORD *)(v38 + 44), true);
			return;
		}
		if (bot->m_hostageEscortCount == 0u)
		{
			if (bot->GetFriendsRemaining () > 0u && bot->IsSniper () && g_engfuncs.pfnRandomFloat (0.0f, 100.0f) < 10.0f)
			{
LABEL_29:
				bot->m_currentTaskID = TaskType_MoveToSniperSpot;
				bot->m_taskEntity = NULL;
				v10 = bot->m_lastKnownArea;
				v125 = false;
				v122 = 2000.0f;
LABEL_30:
				bot->Hide (v10, g_engfuncs.pfnRandomFloat (10.0f, 30.0f), v122, v125);
				bot->SetDisposition (DispositionType_OpportunityFire);
				bot->PrintIfWatched("Sniping!\n");
				return;
			}
			if (bot->GetFriendsRemaining () > 0u && bot->m_hostageEscortCount == 0u && (bot->IsRogue () || g_engfuncs.pfnRandomFloat(0.0f, 100.0f) < 33.3f))
				goto LABEL_252;
		}
		v41 = bot->m_gameState.GetNearestFreeHostage (NULL);
		v129 = v41;
		if (!bot->IsDoingScenario ())
		{
			if (v41)
			{
				v42 = TheNavAreaGrid.GetNearestNavArea (v41->GetOrigin (), false);
				if (v42 != NULL)
				{
					bot->m_currentTaskID = TaskType_GuardingHostages;
					bot->m_taskEntity = NULL;
					bot->Hide(v42, -1.0f, 750.0f, false);
					bot->PrintIfWatched("I'm securing the hostages for a human to rescue\n");
					return;
				}
			}
			goto LABEL_252;
		}
		v43 = NULL;
		bot->m_goalEntity = NULL;
		if (bot->m_hostageEscortCount > 0u)
		{
			v44 = bot->m_lastKnownArea;
			LODWORD(i) = bot;
			v132 = 0;
			v133 = NULL;
			*(float *)&bot = 100000000.0f;
			if (v44)
			{
				v45 = TheBots->m_mapZonesNumber;
				*(float *)&v135 = 0.0f;
				if (v45 > 0)
				{
					v46 = (int)((char *)TheBots + 24);
					do
					{
						if (*(_DWORD *)(v46 + 64))
						{
							v47 = *(_DWORD *)v46;
							if (*(_DWORD *)v46)
							{
								if (v44 == v47)
								{
									j = 0.0f;
								}
								else
								{
									if (NavAreaBuildPath__H1Z8PathCost_P8CNavAreaT0PC6VectorRX01PP8CNavArea_b(v44, v47, 0, (int)&i, 0))
									{
										v49 = *(_DWORD *)(v47 + 432);
										v50 = v47;
										for (j = 0.0f; v49; j = j + sqrt(v53 * v53 + v52 * v52 + v51 * v51))
										{
											v51 = *(float *)(v50 + 28) - *(float *)(v49 + 28);
											v52 = *(float *)(v50 + 32) - *(float *)(v49 + 32);
											v53 = *(float *)(v50 + 36) - *(float *)(v49 + 36);
											v50 = v49;
											v49 = *(_DWORD *)(v49 + 432);
										}
									}
									else
									{
										j = -1.0f;
									}
								}
							}
							else
							{
								j = -1.0f;
							}
							if (j < *(float *)&bot)
							{
								*(float *)&bot = j;
								v133 = v46 - 4;
							}
						}
						v46 += 116;
						v54 = TheBots->m_mapZonesNumber;
						++v135;
					}
					while (v135 < v54);
				}
				v43 = v133;
			}
			else
			{
				v43 = NULL;
			}
		}
		if (v129)
		{
			if (v43 == NULL)
			{
LABEL_157:
				bot->m_currentTaskID = TaskType_CollectingHostages;
				bot->m_taskEntity = NULL;
				bot->Run ();
				v73 = v129;
				bot->m_goalEntity = v129;
				bot->m_waitForHostagePatience = false;
				m_checkWaitForHostagePatienceTimer.Invalidate ();
				bot->MoveTo(v73->GetOrigin (), bot->m_hostageEscortCount == 0u ? NavigationMesh::SAFEST_ROUTE : NavigationMesh::FASTEST_ROUTE);
				bot->PrintIfWatched("I'm collecting hostages\n");
				return;
			}
			v55 = bot->m_lastKnownArea;
			v56 = *(_DWORD *)(v43 + 4);
			v135 = bot;
			v136 = 0;
			if (v55)
			{
				if (v56)
				{
					if (v55 == v56)
					{
						*(float *)&bot = 0.0f;
					}
					else
					{
						if (NavAreaBuildPath__H1Z8PathCost_P8CNavAreaT0PC6VectorRX01PP8CNavArea_b(v55, v56, 0, (int)&v135, 0))
						{
							v57 = *(_DWORD *)(v56 + 432);
							v58 = v56;
							for (k = 0.0; v57; k = k + sqrt(v62 * v62 + v61 * v61 + v60 * v60))
							{
								v60 = *(float *)(v58 + 28) - *(float *)(v57 + 28);
								v61 = *(float *)(v58 + 32) - *(float *)(v57 + 32);
								v62 = *(float *)(v58 + 36) - *(float *)(v57 + 36);
								v58 = v57;
								v57 = *(_DWORD *)(v57 + 432);
							}
							*(float *)&bot = k;
						}
						else
						{
							*(float *)&bot = -1.0f;
						}
					}
				}
				else
				{
					*(float *)&bot = -1.0f;
				}
			}
			else
			{
				*(float *)&bot = -1.0f;
			}
			v63 = bot->m_lastKnownArea;
			v64 = TheNavAreaGrid.GetNearestNavArea(v129->GetOrigin (), false);
			v65 = v64;
			if (v63 && v64 != NULL)
			{
				if (v63 == v64)
				{
					v66 = 0.0;
				}
				else
				{
					if (!NavAreaBuildPath__H1Z8PathCost_P8CNavAreaT0PC6VectorRX01PP8CNavArea_b(v63, v64, 0, (int)&v135, 0))
						goto LABEL_159;
					v67 = *(_DWORD *)(v65 + 432);
					v68 = v65;
					for (l = 0.0; v67; l = l + sqrt(v72 * v72 + v71 * v71 + v70 * v70))
					{
						v70 = *(float *)(v68 + 28) - *(float *)(v67 + 28);
						v71 = *(float *)(v68 + 32) - *(float *)(v67 + 32);
						v72 = *(float *)(v68 + 36) - *(float *)(v67 + 36);
						v68 = v67;
						v67 = *(_DWORD *)(v67 + 432);
					}
					*(float *)&v135 = l;
					if (l < 0.0)
						goto LABEL_159;
					v66 = *(float *)&v135;
				}
				if (*(float *)&bot >= v66)
					goto LABEL_157;
			}
		}
		else
		{
			if (v43 == NULL)
				goto LABEL_252;
		}
LABEL_159:
		bot->m_currentTaskID = TaskType_RescueHostages;
		bot->m_taskEntity = NULL;
		bot->Run ();
		bot->SetDisposition (DispositionType_SelfDefense);
		v74 = (int)TheBots->GetRandomPositionInZone (v43);
		bot->MoveTo(v74/*MAY BE NULL - SO POTENTIAL CRASH!!!*/, NavigationMesh::FASTEST_ROUTE);
		bot->PrintIfWatched("I'm rescuing hostages\n");
		bot->m_chatter.EscortingHostages ();
		return;
	}
	if (TheBots->m_mapType != BotManager::MapType_As)
	{
LABEL_26:
		if (bot->GetFriendsRemaining () == 0u || !bot->IsSniper () || g_engfuncs.pfnRandomFloat(0.0f, 100.0f) >= 10.0f)
			goto LABEL_252;
		goto LABEL_29;
	}
	if (bot->m_team == Consntants::TeamID_Terrorist)
	{
		if (bot->IsSniper ())
		{
			if (g_engfuncs.pfnRandomFloat(0.0f, 100.0f) <= 75.0f)
			{
				if (TheBots->m_mapZonesNumber > 0u)
				{
					v13 = TheBots->m_mapZones + g_engfuncs.pfnRandomLong (0u, TheBots->m_mapZonesNumber - 1u);
					if (v13 != NULL)
					{
						v14 = TheBots->GetRandomAreaInZone (v13);
						if (v14 != NULL)
						{
							bot->m_currentTaskID = TaskType_MoveToSniperSpot;
							bot->m_taskEntity = NULL;
							bot->Hide(v14, -1.0f, 2000.0f, false);
							bot->SetDisposition (DispositionType_OpportunityFire);
							bot->PrintIfWatched("Sniping near escape zone\n");
							return;
						}
					}
				}
			}
		}
		if (bot->IsRogue ()
			|| TheBots->m_isDefenseRushing
			|| bot->m_morale * -34.0f <= g_engfuncs.pfnRandomFloat (0.0f, 100.0f)
			|| TheBots->m_mapZonesNumber == 0u
			|| (v16 = TheBots->m_mapZones + g_engfuncs.pfnRandomLong (0u, TheBots->m_mapZonesNumber - 1u)) == NULL
			|| (v17 = TheBots->GetRandomAreaInZone (v16)) == NULL)
			goto LABEL_252;
		bot->m_currentTaskID = TaskType_GuardingEscapeZone;
		bot->m_taskEntity = NULL;
		bot->PrintIfWatched("I'm guarding an escape zone\n");
		v126 = false;
		v123 = (bot->m_morale + 3) * 250.0f + 750.0f;
		v120 = -1.0f;
		v118 = v17;
		goto LABEL_85;
	}
	if (!bot->m_isVIP)
		goto LABEL_26;
	if (gpGlobals->time - TheBots->m_roundStartTime >= 20.0f)
	{
		v20 = bot->m_lastKnownArea;
		v129 = bot;
		v130 = 1;
		i = 0.0f;
		*(float *)&v135 = 100000000.0f;
		if (!v20)
			goto LABEL_252;
		v21 = TheBots->m_mapZonesNumber;
		v133 = 0;
		if (v21 > 0)
		{
			v22 = (int)((char *)TheBots + 24);
			do
			{
				if (*(_DWORD *)(v22 + 64))
				{
					v23 = *(_DWORD *)v22;
					if (*(_DWORD *)v22)
					{
						if (v20 == v23)
						{
							v24 = 0.0f;
						}
						else
						{
							if (NavAreaBuildPath__H1Z8PathCost_P8CNavAreaT0PC6VectorRX01PP8CNavArea_b(v20, v23, 0, (int)&v129, 0))
							{
								v25 = *(_DWORD *)(v23 + 432);
								v26 = v23;
								v27 = v23 + 432;
								for (*(float *)&bot = 0.0; v25; *(float *)&bot = v29 + *(float *)&bot)
								{
									v28 = sub_10009A00(v26 + 28, (int)&v137, v25 + 28);
									v29 = VectorLength(v28);
									v26 = *(_DWORD *)v27;
									v25 = *(_DWORD *)(*(_DWORD *)v27 + 432);
									v27 = *(_DWORD *)v27 + 432;
								}
								v24 = *(float *)&bot;
							}
							else
							{
								v24 = -1.0f;
							}
						}
					}
					else
					{
						v24 = -1.0f;
					}
					if (v24 < *(float *)&v135)
					{
						*(float *)&v135 = v24;
						LODWORD(i) = v22 - 4;
					}
				}
				v22 += 116;
				v30 = TheBots->m_mapZonesNumber;
				++v133;
			}
			while (v133 < v30);
		}
		v19 = LODWORD(i);
	}
	else
	{
		if (!TheBots->m_mapZonesNumber)
			goto LABEL_252;
		v19 = TheBots->m_mapZones + g_engfuncs.pfnRandomLong (0u, TheBots->m_mapZonesNumber - 1u);
	}
	if (!v19 || (v31 = (int)TheBots->GetRandomPositionInZone(v19)) == NULL)
		goto LABEL_252;
	bot->m_currentTaskID = TaskType_VIPEscape;
	bot->m_taskEntity = NULL;
	bot->Run ();
	bot->MoveTo (v31, NavigationMesh::SAFEST_ROUTE);
	if (bot->GetFriendsRemaining () > 0u && TheBots->GetRadioMessageInterval (Constants::GameEventType_RADIO_1_Follow_me, bot->m_team) > 30.0f)
		bot->SendRadioMessage (Constants::GameEventType_RADIO_1_Follow_me);
}

void CCSBot::MoveToState::OnEnter (CCSBot *const bot)
{
	if (bot->IsUsingKnife () && bot->IsWellPastSafe () && !bot->IsHurrying ())
		bot->Walk ();
	else
		bot->Run ();

	bot->ComputePath (TheNavAreaGrid.GetNavArea (m_goalPosition, 120.0f), &m_goalPosition, (bot->m_currentTaskID < TaskType_FindTickingBomb || (bot->m_currentTaskID > TaskType_DefusingBomb && bot->m_currentTaskID != TaskType_MoveToLastKnownEnemyPosition)) ? NavigationMesh::SAFEST_ROUTE ? NavigationMesh::FASTEST_ROUTE);

	m_isAlreadySaidGoingToPlantTheBomb = false;
	m_isAlreadySaidPlantingTheBomb = false;
}

void CCSBot::MoveToState::OnUpdate (CCSBot *const bot)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (bot->m_currentTaskID == TaskType_MoveToLastKnownEnemyPosition)
	{
		if (!bot->m_taskEntity || !bot->m_taskEntity->IsAlive ())
		{
			bot->PrintIfWatched ("The enemy I was chasing was killed - giving up.\n");

			return bot->Idle ();
		}
	}

	bot->UpdateLookAround ();

	if (TheBots->m_mapType == BotManager::MapType_De)
	{
		if (bot->m_currentTaskID == TaskType_FindTickingBomb)
		{
			if (bot->m_bombState != BombState_Planted || bot->m_gameState.GetPlantedBombsite () != -1)
				return bot->Idle ();

			for (unsigned char index (0u); index < TheBots->m_mapZonesNumber; ++index)
			{
				if (bot->m_gameState.IsBombsiteClear (index) || !bot->GetOrigin ().IsInsideBoundingBox (TheBots->m_mapZones[index].absoluteBoundingBox))
					continue;

				bot->m_gameState.ClearBombsite (index);

				if (bot->m_team == Constants::TeamID_CounterTerrorist)
					bot->m_chatter.BombsiteClear (index);

				return bot->Idle ();
			}
		}
		else
		{
			if (bot->m_team == Constants::TeamID_CounterTerrorist)
			{
				if (bot->m_bombState == BombState_Planted)
				{
					if (bot->m_currentTaskID != TaskType_DefusingBomb)
						return bot->Idle ();

					if (bot->CanSeePlantedBomb () && TheBots->m_bombDefuser)
					{
						v27 = new BotStatement (&bot->m_chatter, 3, 3.0f);
						v35 = "CoveringFriend";
						goto LABEL_57;
					}
				}
			}
			else if (bot->m_currentTaskID == TaskType_PlantingBomb && bot->GetFriendsRemaining() > 0u && !m_isAlreadySaidPlantingTheBomb)
			{
				if ((bot->m_clientMapZone & Constants::IN_BOMB_PLACE) && bot->GetPathDistanceRemaining () < 50.0f)
				{
					bot->m_chatter.PlantingTheBomb (bot->GetPlace ());
					m_isAlreadySaidPlantingTheBomb = true;
				}
				if (!m_isAlreadySaidGoingToPlantTheBomb && gpGlobals->time - bot->m_setStateTimestamp > 2.0f)
				{
					bot->m_chatter.GoingToPlantTheBomb (TheNavAreaGrid.GetPlace (m_goalPosition));
					m_isAlreadySaidGoingToPlantTheBomb = true;
				}
			}
		}
	}
	else if (TheBots->m_mapType == BotManager::MapType_Cs)
	{
		if (bot->m_currentTaskID == TaskType_CollectingHostages)
		{
			if (!bot->m_goalEntity || bot->m_goalEntity->IsFollowingSomeone ())
				return bot->Idle ();

			BaseEntity *const hostage (bot->m_goalEntity);
			const Math::Vector3D &hostageOrigin (hostage->GetOrigin ());

			if (m_goalPosition.GetDistanceSquared (hostageOrigin) > MATH_GET_SQUARED (75.0f))
				bot->ComputePath (TheNavAreaGrid.GetNavArea (hostageOrigin, 120.0f), &hostageOrigin, NavigationMesh::SAFEST_ROUTE);

			if (bot->m_currentLadder == NULL)
			{
				Math::Vector3D lookAt (hostage->GetOrigin ());

				lookAt.z += 72.0f * 0.75f;

				const float distanceSquared (bot->GetOrigin ().GetDistanceSquared (lookAt));

				if (distanceSquared < MATH_GET_SQUARED (100.0f))
				{
					bot->SetLookAt ("Hostage", lookAt, PriorityType_Low, 0.5f, false, 5.0f);

					switch (g_engfuncs.pfnRandomLong (0, 3))
					{
						case 0:
							bot->MoveForward ();

							break;

						case 1:
							bot->StrafeRight ();

							break;

						case 2:
							bot->MoveBackward ();

							break;

						case 3:
							bot->StrafeLeft ();

							break;
					}

					if (distanceSquared < MATH_GET_SQUARED (50.0f))
						return bot->UseEntity (bot->m_goalEntity);
				}
			}
		}
		else
		{
			if (bot->m_currentTaskID == TaskType_RescueHostages && bot->m_hostageEscortCount == 0u)
				return bot->Idle ();
		}
	}

	if (bot->UpdatePathMovement (true))
	{
		if (bot->m_currentTaskID == TaskType_PlantingBomb)
		{
			if ((bot->m_clientMapZone & Constants::IN_BOMB_PLACE) && bot->m_isCanPlantBomb)
				return bot->PlantBomb ();

			return bot->Idle ();
		}

		if (bot->m_currentTaskID != TaskType_DefusingBomb)
		{
			if (bot->m_currentTaskID != TaskType_MoveToLastKnownEnemyPosition || !bot->m_taskEntity || !bot->m_taskEntity->IsAlive ())
				return bot->Idle ();

			v27 = new BotStatement (&bot->m_chatter, 3, 3.0f);
			v35 = "LostEnemy";
LABEL_57:
			v27->AppendPhrase (TheBotPhrases->GetPhrase (v35));

			bot->m_chatter.AddStatement (v27, false);

			return bot->Idle ();
		}

		if (!bot->IsActiveWeaponReloading ())
		{
			const Math::Vector3D *const bombPosition (bot->m_gameState.GetBombPosition ());

			if (bombPosition != NULL && (bot->GetOrigin () - Math::Vector3D (0.0f, 0.0f, bot->m_doCrouch ? 18.0f : 36.0f)).GetDistanceSquared (*bombPosition) < MATH_GET_SQUARED (100.0f))
				return bot->DefuseBomb ();

			return bot->Idle ();
		}
	}
}

void CCSBot::MoveToState::OnExit (CCSBot *const bot)
{
	bot->Run ();
	bot->SetDisposition (DispositionType_EngageAndInvestigate);
}

void CCSBot::PlantBombState::OnEnter (CCSBot *const bot)
{
	bot->Crouch ();
	bot->SetDisposition (DispositionType_SelfDefense);

	const float angle (bot->m_variables->viewAngles.yaw);

	bot->SetLookAt ("Plant bomb on floor", Math::Vector3D (bot->GetOrigin ().x + BotCOS (angle) * 10.0f, bot->GetOrigin ().y + BotSIN (angle) * 10.0f, bot->GetOrigin ().z - (bot->m_doCrouch ? 18.0f : 36.0f)), PriorityType_High, -1.0f, false, 5.0f);
}

void CCSBot::PlantBombState::OnUpdate (CCSBot *const bot)
{
	if (bot->m_pActiveItem == NULL || bot->m_pActiveItem->GetClassName () != "weapon_c4")
		bot->SelectItem ("weapon_c4");
	else
		bot->PrimaryAttack ();

	if (!bot->m_isCanPlantBomb)
	{
		bot->m_currentTaskID = TaskType_GuardTickingBomb;
		bot->m_taskEntity = NULL;

		bot->Hide (NULL, -1.0f, 750.0f, false);
	}

	if (gpGlobals->time - bot->m_setStateTimestamp > 5.0f)
		bot->Idle ();
}

void CCSBot::PlantBombState::OnExit (CCSBot *const bot)
{
	bot->EquipBestWeapon (false);
	bot->StandUp ();
	bot->ResetStuckMonitor ();
	bot->SetDisposition (DispositionType_EngageAndInvestigate);
	bot->ClearLookAt ();
}

void CCSBot::UseEntityState::OnUpdate (CCSBot *const bot)
{
	if (bot->m_setStateTimestamp - gpGlobals->time <= 5.0f)
	{
		Math::Vector3D targetLookOrigin (m_target->GetOrigin ());

		targetLookOrigin.z += 72.0f * 0.5f;

		bot->SetLookAt ("Use entity", targetLookOrigin, PriorityType_High, -1.0f, false, 5.0f);

		if (!bot->IsLookingAtPosition (targetLookOrigin, 20.0f))
			return;

		if (TheBots->m_mapType == BotManager::MapType_Cs && bot->m_team == Constants::TeamID_CounterTerrorist && bot->m_currentTaskID == TaskType_CollectingHostages)
			++bot->m_hostageEscortCount;

		bot->UseEnvironment ();
	}

	bot->Idle ();
}

void CCSBot::UseEntityState::OnExit (CCSBot *const bot)
{
	bot->ClearLookAt ();
	bot->ResetStuckMonitor ();
}

void CCSBot::SetState (BotState *const newState)
{
	PrintIfWatched ("SetState: %s -> %s\n", m_currentState == NULL ? "NULL" : m_currentState->GetName (), newState->GetName ());

	if (IsAttacking ())
		StopAttacking ();

	if (m_currentState != NULL)
		m_currentState->OnExit (this);

	newState->OnEnter (this);

	m_currentState = newState;
	m_setStateTimestamp = gpGlobals->time;
}

void CCSBot::Idle (void)
{
	m_currentTaskID = TaskType_SeekAndDestroy;
	m_taskEntity = NULL;

	SetState (&m_idleState);
}

void CCSBot::EscapeFromBomb (void)
{
	m_currentTaskID = TaskType_EscapeFromBomb;
	m_taskEntity = NULL;

	SetState (&m_escapeFromBombState);
}

void CCSBot::Follow (BasePlayer *const target)
{
	if (target == NULL)
		return;

	if (!m_isFollowing || m_followingTarget != target)
		m_followingStartTime = gpGlobals->time;

	m_isFollowing = true;
	m_followingTarget = target;
	m_currentTaskID = TaskType_Follow;
	m_taskEntity = NULL;
	m_followState.m_leader = target;

	SetState (&m_followState);
}

void CCSBot::ContinueFollowing (void)
{
	m_currentTaskID = TaskType_Follow;
	m_taskEntity = NULL;
	m_followState.m_leader = m_followingTarget;

	SetState (&m_followState);
}

void CCSBot::StopFollowing (void)
{
	m_isFollowing = false;
	m_followingTarget = NULL;
	m_nextAutoFollowTime = gpGlobals->time + 10.0f;
}

void CCSBot::UseEntity (BaseEntity *const entity)
{
	m_useEntityState.m_target = entity;

	SetState (&m_useEntityState);
}

void CCSBot::Hide (NavigationArea *area, const float duration, const float searchRange, const bool holdPosition)
{
	DestroyPath ();

	const Math::Vector3D *spotSearchPosition;

	if (area == NULL)
	{
		area = m_lastKnownArea;

		if (area == NULL)
		{
			PrintIfWatched ("Hide from area is NULL.\n");
			Idle ();

			return;
		}

		spotSearchPosition = &GetOrigin ();
	}
	else
		spotSearchPosition = &area->GetCenter ();

	m_hideState.m_searchArea = area;
	m_hideState.m_searchRange = searchRange;
	m_hideState.m_duration = duration;
	m_hideState.m_holdPosition = holdPosition;

	const Math::Vector3D *spot (FindNearbyHidingSpot (this, *spotSearchPosition, area, searchRange, IsSniper (), false));

	if (spot == NULL)
	{
		PrintIfWatched ("No available hiding spots.\n");

		spot = &GetOrigin ();
	}

	m_hideState.m_hiddingSpot = *spot;

	if (ComputePath (TheNavAreaGrid.GetNavArea (*spot, 120.0f), spot, NavigationMesh::FASTEST_ROUTE))
		SetState (&m_hideState);
	else
	{
		PrintIfWatched ("Can't pathfind to hiding spot\n");
		Idle ();
	}
}

void CCSBot::Hide (const Math::Vector3D &position, const float duration, const bool holdPosition)
{
	NavigationMesh::NavigationArea *const area (TheNavAreaGrid.GetNearestNavArea (position, false));

	if (area == NULL)
	{
		PrintIfWatched ("Hiding spot off nav mesh\n");

		return Idle ();
	}

	DestroyPath ();

	m_hideState.m_searchArea = area;
	m_hideState.m_searchRange = 750.0f;
	m_hideState.m_duration = duration;
	m_hideState.m_holdPosition = holdPosition;
	m_hideState.m_hiddingSpot = position;

	if (ComputePath (area, position, NavigationMesh::FASTEST_ROUTE))
		SetState (&m_hideState);
	else
	{
		PrintIfWatched ("Can't pathfind to hiding spot\n");
		Idle ();
	}
}

const bool CCSBot::TryToHide (NavigationMesh::NavigationArea *area, const float duration, const float searchRange, const bool holdPosition, const bool a6)
{
	const Math::Vector3D *spotSearchPosition;

	if (area == NULL)
	{
		area = m_lastKnownArea;

		if (area == NULL)
		{
			PrintIfWatched ("Hide from area is NULL.\n");

			return false;
		}

		spotSearchPosition = &GetOrigin ();
	}
	else
		spotSearchPosition = &area->GetCenter ();

	m_hideState.m_searchArea = area;
	m_hideState.m_searchRange = searchRange;
	m_hideState.m_duration = duration;
	m_hideState.m_holdPosition = holdPosition;

	const Math::Vector3D *spot (FindNearbyHidingSpot (this, *spotSearchPosition, area, searchRange, IsSniper (), a6));

	if (spot == NULL)
	{
		PrintIfWatched ("No available hiding spots.\n");

		return false;
	}

	m_hideState.m_hiddingSpot = *spot;

	if (!ComputePath (TheNavAreaGrid.GetNavArea (*spot, 120.0f), spot, NavigationMesh::FASTEST_ROUTE))
	{
		PrintIfWatched ("Can't pathfind to hiding spot\n");

		return false;
	}

	SetState (&m_hideState);

	return true;
}

const bool CCSBot::TryToRetreat (void)
{
	const Math::Vector3D *const spot (FindNearbyRetreatSpot (1000.0f));

	if (spot == NULL)
		return false;

	IgnoreEnemies (10.0f);
	StandUp ();
	Run ();
	Hide (*spot, g_engfuncs.pfnRandomFloat (3.0f, 15.0f), false);
	PrintIfWatched ("Retreating to a safe spot!\n");

	return true;

}

void CCSBot::Hunt (void) { SetState (&m_huntState); }

void CCSBot::Attack (BasePlayer *const enemy)
{
	if (enemy == NULL || cv_bot_zombie.value > 0.0f || IsActiveWeaponReloading ())
		return;

	SetEnemy (enemy);

	if (m_isAttacking)
		return;

	if (IsAtHidingSpot ())
		m_attackState.m_crouchAndHoldAttack = g_engfuncs.pfnRandomFloat (0.0f, 100.0f) < 60.0f;
	else
		m_attackState.m_crouchAndHoldAttack = false;

	PrintIfWatched ("ATTACK BEGIN (reaction time = %g (+ update time), surprise time = %g, attack delay = %g)\n", m_profile->reactionTime, m_surpriseTime, m_profile->attackDelay);

	m_isAttacking = true;

	m_attackState.OnEnter (this);

	m_lastKnownEnemyPosition = enemy->GetOrigin ();
	m_lastSawEnemyTimestamp = m_attackBeginTimestamp = gpGlobals->time;

	const Math::Vector3D direction (enemy->GetOrigin () - GetOrigin ());
	float angleDifference (Math::fabsf (m_lookAngleYaw - UTIL_VecToAngles (direction).yaw));

	while (angleDifference > 180.0f)
		angleDifference -= 360.0f;

	if (angleDifference < 0.0f)
		angleDifference = -angleDifference;

	angleDifference *= 1.0f / 180.0f;

	SetAimOffset (m_profile->skill / (angleDifference + 1.0f));

	m_setAimOffsetNextCallTimestamp = gpGlobals->time + g_engfuncs.pfnRandomFloat (angleDifference + 0.25f, 1.5f);
}

void CCSBot::StopAttacking (void)
{
	PrintIfWatched ("ATTACK END\n");
	m_attackState.OnExit (this);

	m_isAttacking = false;

	if (m_isFollowing)
		Idle ();
}

inline const bool CCSBot::IsAttacking (void) const { return m_isAttacking; }
inline const bool CCSBot::IsEscapingFromBomb (void) const { return m_currentState == &m_escapeFromBombState; }
inline const bool CCSBot::IsDefusingBomb (void) const { return m_currentState == &m_defuseBombState; }
inline const bool CCSBot::IsHiding (void) const { return m_currentState == &m_hideState; }
inline const bool CCSBot::IsAtHidingSpot (void) const { return IsHiding () && m_isAtSpot; }
inline const bool CCSBot::IsHunting (void) const { return m_currentState == &m_huntState; }
inline const bool CCSBot::IsBuying (void) const { return m_currentState == &m_buyState; }

void CCSBot::MoveTo (const Math::Vector3D &goalPosition, const NavigationMesh::RouteType_t routeType)
{
	m_moveToState.m_goalPosition = goalPosition;
	m_moveToState.m_routeType = routeType;

	SetState (&m_moveToState);
}

void CCSBot::PlantBomb (void) { SetState (&m_plantBombState); }
void CCSBot::FetchBomb (void) { SetState (&m_fetchBombState); }
void CCSBot::DefuseBomb (void) { SetState (&m_defuseBombState); }
void CCSBot::InvestigateNoise (void) { SetState (&m_investigateNoiseState); }
void CCSBot::Buy (void) { SetState (&m_buyState); }

void CCSBot::Upkeep (void)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (CCSBotManager::m_isLearningMap || !IsAlive ())
		return;

	if (m_isRapidFiring)
		TogglePrimaryAttack ();

	if (m_isAimingAtEnemy)
	{
		UpdateAimOffset ();

		if (m_enemy)
		{
			const float offsetZ (m_doCrouch ? 18.0f : 36.0f);//GetOrigin ().z - (GetOrigin ().z - (m_doCrouch ? 18.0f : 36.0f))); - WTF??

			if (!m_isEnemyVisible)
			{
				m_enemyShootPosition = m_lastKnownEnemyPosition;
				goto LABEL_40;
			}

			if (m_profile->skill <= 0.5f)
				m_enemyShootPosition = m_enemy->GetOrigin ();
			else
			{
				const Math::Vector3D velocityDifference (m_enemy->m_variables->velocity - m_variables->velocity);

				m_enemyShootPosition = m_enemy->GetOrigin () + velocityDifference * g_flBotCommandInterval * 3.0f;
			}

			if (IsUsingAWP () || IsUsingShotgun () || IsUsingMachinegun () || m_profile->skill < 0.8f || IsActiveWeaponRecoilHigh () && !IsUsingPistol () && !IsUsingSniperRifle ())
			{
				if (IsEnemyPartVisible (VisiblePartType_Waist))
				{
					m_enemyShootPosition.z -= offsetZ * 0.25f;
					goto LABEL_40;
				}
			}
			else if (IsEnemyPartVisible (VisiblePartType_Head))
				goto LABEL_40;

			if (IsEnemyPartVisible (VisiblePartType_Waist))
			{
LABEL_37:
				m_enemyShootPosition.z -= offsetZ * 0.5f;
				goto LABEL_40;
			}

			if (!IsEnemyPartVisible (VisiblePartType_Head))
			{
				if (IsEnemyPartVisible (VisiblePartType_LeftSide))
				{
					const Math::Vector2D directionNormalized ((m_enemy->GetOrigin ().AsVector2D () - GetOrigin ().AsVector2D ()).Normalize ());

					m_enemyShootPosition.x += -directionNormalized.y * 16.0f;
					m_enemyShootPosition.y += directionNormalized.x * 16.0f;
				}
				else
				{
					if (!IsEnemyPartVisible (VisiblePartType_RightSide))
					{
						m_enemyShootPosition.z -= offsetZ * 2.0f;
						goto LABEL_40;
					}

					const Math::Vector2D directionNormalized ((m_enemy->GetOrigin ().AsVector2D () - GetOrigin ().AsVector2D ()).Normalize ());

					m_enemyShootPosition.x -= -directionNormalized.y * 16.0f;
					m_enemyShootPosition.y -= directionNormalized.x * 16.0f;
				}
				goto LABEL_37;
			}
LABEL_40:
			m_enemyShootPosition += m_aimOffset;

			const Math::Angles2D angles (UTIL_VecToAngles (m_enemyShootPosition - GetOrigin ()));

			m_lookAnglePitch = 360.0f - angles.pitch;
			m_lookAngleYaw = angles.yaw;
		}
	}
	else
	{
		if (m_lookAtCheckDistance && GetOrigin ().GetDistanceSquared (m_lookAtVector) < MATH_GET_SQUARED (100.0f))
			m_lookAction = LookAction_None;

		switch (m_lookAction)
		{
			case LookAction_None:
				m_lookAngleYaw = m_lookAheadAngle;
				m_lookAnglePitch = 0.0f;

				break;

			case LookAction_LookTowardsSpot:
				UpdateLookAt ();

				if (IsLookingAtPosition (m_lookAtVector, m_lookAtMaximumAngleDifference))
				{
					m_lookAction = LookAction_LookAtSpot;
					m_lookAtStartTimestamp = gpGlobals->time;
				}

				break;

			case LookAction_LookAtSpot:
				UpdateLookAt ();

				if (m_lookAtDuration >= 0.0f && gpGlobals->time - m_lookAtStartTimestamp > m_lookAtDuration)
					m_lookAction = LookAction_None;

				break;
		}

		const float multiplier (IsUsingSniperRifle () && IsUsingScope () ? 0.5f : 2.0f);

		m_lookAngleYaw += BotCOS (gpGlobals->time * 33.0f) * multiplier;
		m_lookAnglePitch += BotSIN (gpGlobals->time * 13.0f) * multiplier;
	}

	UpdateLookAngles ();
}

inline const float CCSBot::GetCombatRange (void) const { return m_combatRange; }

inline void CCSBot::SetRogue (const bool rogue) { m_isRogue = rogue; }

inline void CCSBot::Hurry (const float timeAmount) { m_hurryTimer.Start (timeAmount); }

inline const float CCSBot::GetSafeTime (void) const { return m_safeEndTime; }

inline const bool CCSBot::IsUnhealthy (void) const { return GetHealth () <= 40; }

inline const bool CCSBot::IsCarryingBomb (void) const { return m_isCanPlantBomb; }

inline const bool CCSBot::IsFollowing (void) const { return m_isFollowing; }

inline BaseEntity *const CCSBot::GetFollowLeader (void) const { return m_followingTarget; }

inline const float CCSBot::GetFollowDuration (void) const { return gpGlobals->time - m_followingStartTime; }

inline const bool CCSBot::CanAutoFollow (void) const
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	this = m_nextAutoFollowTime;
	UNDEF(this);
	v3 = this < gpGlobals->time;
	v4 = 0;
	v5 = this == gpGlobals->time;
	return (HIBYTE(this) & 0x45) == 1;
}

inline void CCSBot::AimAtEnemy (void) { m_isAimingAtEnemy = true; }

inline void CCSBot::StopAiming (void) { m_isAimingAtEnemy = false; }

inline const bool CCSBot::IsAimingAtEnemy (void) const { return m_isAimingAtEnemy; }

inline const bool CCSBot::IsSurprised (void) const
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	/// @todo CHECK ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*
	this = gpGlobals->time - m_surprisedTime;
	UNDEF(this);
	v3 = this < 5.0f;
	v4 = 0;
	v5 = this == 5.0f;
	return (HIBYTE(this) & 0x45) == 1;
*/
	return gpGlobals->time - m_surprisedTime < 5.0f
}

inline const float CCSBot::GetSurpriseDelay (void) const
{
	return IsSurprised () ? m_surpriseTime : 0.0f;
}

inline void CCSBot::ClearSurpriseDelay (void) { m_surpriseTime = m_surprisedTime = 0.0f; }

inline const float CCSBot::GetStateTimestamp (void) const { return m_setStateTimestamp; }

inline CCSBot::CSGameState &CCSBot::GetGameState (void) { return m_gameState; }

inline const CCSBot::CSGameState &CCSBot::GetGameState (void) const { return m_gameState; }

inline const bool CCSBot::IsAtBombsite (void) const { return (m_clientMapZone & Constants::IN_BOMB_PLACE) > 0u; }

inline void CCSBot::SetTask (const TaskType_t task, BaseEntity *const taskEntity/* = NULL*/)
{
	m_currentTaskID = task;
	m_taskEntity = taskEntity;
}

inline const TaskType_t CCSBot::GetTask (void) const { return m_currentTaskID; }

inline BaseEntity *const CCSBot::GetTaskEntity (void) const { return m_taskEntity; }

inline const MoraleType_t CCSBot::GetMorale (void) const { return m_morale; }

inline const bool CCSBot::IsNoiseHeard (void) const
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	this = m_noiseHearGlobalTimeDuration;
	UNDEF(this);
	v3 = this < 0.0f;
	v4 = 0;
	v5 = this == 0.0f;
	return (unsigned __int8)((HIBYTE(this) & 0x45) - 1) >= 0x40u
			&& gpGlobals->time - m_noiseHearGlobalTimeDuration >= m_profile->reactionTime;
}

inline NavigationArea *const CCSBot::GetNoiseArea (void) const { return m_noiseArea; }

inline void CCSBot::ForgetNoise (void) { m_noiseHearGlobalTimeDuration = 0.0f; }

inline const float CCSBot::GetNoiseRange (void) const
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	this = m_noiseHearGlobalTimeDuration;
	UNDEF(this);
	v3 = this < 0.0;
	v4 = 0;
	v5 = this == 0.0;
	if ((unsigned __int8)((HIBYTE(this) & 0x45) - 1) < 0x40u
		|| (v8 = gpGlobals->time - m_noiseHearGlobalTimeDuration,
				UNDEF(v7),
				v9 = v8 < m_profile->reactionTime,
				v10 = 0,
				v11 = v8 == m_profile->reactionTime,
				(HIBYTE(v7) & 0x45) == 1))
	{
		result = 1000000000.0f;
	}
	else
	{
		result = GetOrigin ().GetDistance (m_noisePosition);
	}
	return result;
}

inline const NoisePriority_t CCSBot::GetNoisePriority (void) const { return m_noisePriority; }

inline BotChatterInterface &CCSBot::GetChatter (void) { return m_chatter; }

inline const bool CCSBot::IsUsingVoice (void) const
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	this = m_voiceFeedbackGlobalDuration;
	UNDEF(this);
	v3 = this < 0.0f;
	v4 = 0;
	v5 = this == 0.0f;
	return (HIBYTE(this) & 0x44) != 64;
}

inline BaseEntity *const CCSBot::GetEnemy (void) const { return m_enemy; }

inline const unsigned char CCSBot::GetNearbyEnemyCount (void) const
{
	const unsigned char enemiesRemaining (GetEnemiesRemaining ());

	return enemiesRemaining >= m_nearbyEnemyCount ? m_nearbyEnemyCount : enemiesRemaining;
}

inline Place_t CCSBot::GetEnemyPlace (void) const { return m_enemyPlace; }

inline const bool CCSBot::CanSeeBomber (void) const { return m_bomber != NULL; }

inline BaseEntity *const CCSBot::GetBomber (void) const { return m_bomber; }

inline const unsigned char CCSBot::GetNearbyFriendCount (void) const
{
	const unsigned char friendsRemaining (GetFriendsRemaining ());

	return friendsRemaining >= m_nearbyFriendCount ? m_nearbyFriendCount : friendsRemaining;
}

inline BaseEntity *const CCSBot::GetClosestVisibleFriend (void) const { return m_closestVisibleFriend; }

inline BaseEntity *const CCSBot::GetClosestVisibleHumanFriend (void) const { return m_closestVisibleHumanFriend; }

inline const float CCSBot::GetTimeSinceAttacked (void) const { return gpGlobals->time - m_attackedTimestamp; }

inline const float CCSBot::GetFirstSawEnemyTimestamp (void) const { return m_firstSawEnemyTimestamp; }

inline const float CCSBot::GetLastSawEnemyTimestamp (void) const { return m_lastSawEnemyTimestamp; }

inline const float CCSBot::GetTimeSinceLastSawEnemy (void) const { return gpGlobals->time - GetLastSawEnemyTimestamp (); }

inline const float CCSBot::GetTimeSinceAcquiredCurrentEnemy (void) const { return gpGlobals->time - m_acquiredCurrentEnemyTimestamp; }

inline const Math::Vector3D &CCSBot::GetLastKnownEnemyPosition (void) const { return m_lastKnownEnemyPosition; }

inline const bool CCSBot::IsEnemyVisible (void) const { return m_isEnemyVisible; }

inline const float CCSBot::GetEnemyDeathTimestamp (void) const { return m_enemyDeathTimestamp; }

inline const unsigned int CCSBot::GetLastVictimID (void) const { return m_lastVictimID; }

inline const bool CCSBot::HasPath (void) const { return m_currentPathInformationNumber > 0u; }

inline NavigationArea *const CCSBot::GetLastKnownArea (void) const { return m_lastKnownArea; }

inline const Math::Vector3D &CCSBot::GetPathEndpoint (void) const { return m_path[m_currentPathInformationNumber - 1u].point; }

inline const Math::Vector3D &CCSBot::GetPathPosition (const unsigned int index) const { return m_path[index].point; }

inline const bool CCSBot::IsUsingLadder (void) const { return m_currentLadder != NULL; }

inline void CCSBot::SetGoalEntity (BaseEntity *const entity) { m_goalEntity = entity; }

inline BaseEntity *const CCSBot::GetGoalEntity (void) const { return m_goalEntity; }

inline void CCSBot::ForceRun (const float timeAmount)
{
	Run ();

	m_forceRunTimer.Start (timeAmount);
}

inline void CCSBot::SetLookAngles (const float lookAngleYaw, const float lookAnglePitch)
{
	m_lookAngleYaw = lookAngleYaw;
	m_lookAnglePitch = lookAnglePitch;
}

inline void CCSBot::SetForwardAngle (const float lookForwardAngle) { m_lookForwardAngle = lookForwardAngle; }
inline void CCSBot::SetLookAheadAngle (const float lookAheadAngle) { m_lookAheadAngle = lookAheadAngle; }

inline void CCSBot::ClearLookAt (void)
{
	m_lookAction = LookAction_None;
	m_lookAtString = NULL;
}

inline const bool CCSBot::IsLookingAtSpot (const PriorityType_t priority) const { return m_lookAction != LookAction_None && m_lookAtPriorityType >= priority; }

inline const bool CCSBot::IsViewMoving (const float a2) const
{
	return a2 <= m_lookAnglePitchUNKNOWN || -a2 >= m_lookAnglePitchUNKNOWN || a2 <= m_lookAngleYawUNKNOWN || -a2 >= m_lookAngleYawUNKNOWN;
}

inline const bool CCSBot::IsEnemyPartVisible (const VisiblePartType_t part) const { return m_isEnemyVisible && (m_enemyVisibleBodyParts & part) > 0u; }

void CCSBot::Update (void)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (CCSBotManager::m_isAnalysisRequested && !m_currentGenerationState)
	{
		CCSBotManager::m_isAnalysisRequested = false;

		StartAnalyzeAlphaProcess ();
	}
	switch (m_currentGenerationState)
	{
		case SAMPLE_WALKABLE_SPACE_AND_CREATE_AREAS_FROM_SAMPLES:
			UpdateLearnProcess ();
			return;
		case FIND_HIDING_SPOTS_AND_APPROACH_AREAS:
			UpdateAnalyzeAlphaProcess ();
			return;
		case FIND_ENCOUNTER_SPOTS_AND_SNIPER_SPOTS:
			UpdateAnalyzeBetaProcess ();
			return;
		case SAVE_NAV_MESH:
			UpdateSaveProcess ();
			return;
		default:
			m_chatter.Update ();
			if (m_voiceFeedbackGlobalDuration != 0.0f && (m_voiceFeedbackGlobalDuration <= gpGlobals->time || gpGlobals->time < m_startVoiceFeedbackTimestamp))
				EndVoiceFeedback (false);
			if (!IsAlive ())
			{
				m_isDead = true;
				BotDeathThink ();
				return;
			}
			if (((cv_bot_traceview.value == 100.0f && IsLocalPlayerWatchingMe ()) || cv_bot_traceview.value == 101.0f) && !IsFriendInLineOfFire ())
			{
				UTIL_MakeVectors (m_variables->punchAngles + m_variables->viewAngles);
				const Math::Vector3D gunPosition (GetGunPosition ());
				Color <> color;

				if (m_team == Constants::TeamID_Terrorist)
				{
					color.red = 255;
					color.green = 0;
					color.blue = 0;
				}
				else
				{
					color.red = 0;
					color.green = 50;
					color.blue = 255;
				}

				UTIL_DrawBeamPoints (gunPosition, gunPosition + gpGlobals->forward * 2000.0f, 1, color.red, color.green, color.blue);
			}
			if ((cv_bot_traceview.value == 2.0f && IsLocalPlayerWatchingMe ()) || cv_bot_traceview.value == 3.0f)
				DrawApproachPoints ();
			if (((cv_bot_traceview.value == 4.0f && IsLocalPlayerWatchingMe ()) || cv_bot_traceview.value == 5.0f) && m_currentSpotEncounter != NULL)
			{
				UTIL_DrawBeamPoints (m_currentSpotEncounter->path.from, m_currentSpotEncounter->path.to, 3, 0, 0, 255);

				const Math::Vector3D direction (m_currentSpotEncounter->path.to - m_currentSpotEncounter->path.from);

				for (const Node_t *node (m_currentSpotEncounter->spotList.m_head->next); node != m_currentSpotEncounter->spotList.m_head; node = node->next)
					UTIL_DrawBeamPoints (m_currentSpotEncounter->path.from + direction * node->element.t, node->element.spot->GetPosition (), 3, 0, 255, 255);
			}
			if (cv_bot_traceview.value == 1.0f && IsLocalPlayerWatchingMe ())
			{
				const Math::Vector3D &eyePosition (GetEyePosition ());
				Math::Vector2D direction;

				Math::GetCosineSine (Math::DegreeToRadian (m_lookAheadAngle), direction.x, direction.y);

				UTIL_DrawBeamPoints (eyePosition, eyePosition + direction * 50.0f, 1, 0, 255, 255);
			}
			if (cv_bot_stop.value != 0.0f)
				return;
			StuckCheck ();
			if (m_noiseHearGlobalTimeDuration > 0.0f && gpGlobals->time - m_noiseHearGlobalTimeDuration > 20.0f)
				m_noiseHearGlobalTimeDuration = 0.0f;
			if (m_currentArea == NULL || !m_currentArea->Contains (GetOrigin ()))
				m_currentArea = TheNavAreaGrid->GetNavArea (GetOrigin (), 120.0f);
			if (m_currentArea != NULL && m_currentArea != m_lastKnownArea)
			{
				m_lastKnownArea = m_currentArea;
				m_currentArea->m_clearedTimestamp[m_team - 1u] = gpGlobals->time;
			}
			if (GetOrigin ().GetDistanceSquared (m_approachPointsCenter) > MATH_GET_SQUARED (50.0f))
			{
				ComputeApproachPoints ();
				m_approachPointsCenter = GetOrigin ();
			}
			if (cv_bot_show_nav.value > 0.0f && m_lastKnownArea != NULL)
				m_lastKnownArea->DrawConnectedAreas ();
			if (gpGlobals->time < m_blindEndTime)
			{
				if (!IsAtHidingSpot ())
				{
					switch (m_blindAction)
					{
						case BlindActionType_MoveForward:
							MoveForward ();
							break;
						case BlindActionType_StrafeRight:
							StrafeRight ();
							break;
						case BlindActionType_MoveBackward:
							MoveBackward ();
							break;
						case BlindActionType_StrafeLeft:
							StrafeLeft ();
							break;
						default:
							Crouch ();
							break;
					}
				}
				if (m_blindDoPrimaryAttack)
					PrimaryAttack ();
				return;
			}
			UpdateReactionQueue ();
			BaseEntity *const newRecognizedEnemy (GetRecognizedEnemy ());
			if (newRecognizedEnemy != NULL)
			{
				AdjustSafeTime ();
				if (IsUsingGrenade ())
					ThrowGrenade (newRecognizedEnemy->GetOrigin ());
				else
				{
					if (GetDisposition () == DispositionType_IgnoreEnemies)
					{
						SetEnemy (newRecognizedEnemy);
						m_isEnemyVisible = true;
					}
					else
					{
						if (GetDisposition () == DispositionType_EngageAndInvestigate
							|| GetDisposition () == DispositionType_OpportunityFire
							|| IsPlayerLookingAtMe (newRecognizedEnemy)
							|| GetOrigin ().GetDistanceSquared (newRecognizedEnemy->GetOrigin ()) < MATH_GET_SQUARED (750.0f))
						{
							if ((!m_enemy || newRecognizedEnemy != m_enemy || !IsAttacking ()) && (!IsUsingKnife () || !IsHiding () || GetOrigin ().GetDistanceSquared (newRecognizedEnemy->GetOrigin ()) < MATH_GET_SQUARED (250.0f)))
								Attack (newRecognizedEnemy);
						}
						else
						{
							SetEnemy (newRecognizedEnemy);
							m_isEnemyVisible = true;
						}
					}
				}
				if (GetDisposition () != DispositionType_IgnoreEnemies && !IsAttacking () && GetTimeSinceAttacked () < 1.0f)
				{
					Attack (newRecognizedEnemy);
					PrintIfWatched ("Ouch! Retaliating!\n");
				}
				TheBots->m_lastSeenEnemyTimestamp = gpGlobals->time;
			}
			if (!m_enemy)
				m_isEnemyVisible = false;
			else
			{
				if (m_enemyDeathTimestamp == 0.0f || m_enemy->IsAlive () || 1.0f - m_profile->skill >= gpGlobals->time - m_enemyDeathTimestamp)
				{
					if (IsVisible (m_enemy, false, &m_enemyVisibleBodyParts))
					{
						m_isEnemyVisible = true;
						m_lastSawEnemyTimestamp = gpGlobals->time;
						m_lastKnownEnemyPosition = m_enemy->GetOrigin ();
					}
					else
						m_isEnemyVisible = false;

					if (m_enemy->IsAlive ())
					{
						m_enemyDeathTimestamp = 0.0f;
						m_isEnemyDead = false;
					}
					else if (m_enemyDeathTimestamp == 0.0f)
					{
						m_isEnemyDead = true;
						m_enemyDeathTimestamp = gpGlobals->time;
					}
				}
				else
				{
					m_enemy = NULL;
					m_isEnemyVisible = false;
				}
			}
			m_isAimingAtEnemy = m_enemy && GetTimeSinceLastSawEnemy () < 3.0f;
			if (GetDisposition () == DispositionType_IgnoreEnemies)
				FireWeaponAtEnemy ();
			if (IsEndOfSafeTime () && IsUsingGrenade () && !m_isThrowingGrenade && FindGrenadeTossPathTarget ((int)&v122))
				ThrowGrenade ((int)&v122);
			if (!IsUsingGrenade ())
				m_isThrowingGrenade = false;
			else if (m_isThrowingGrenade && (m_throwGrenadeAimTimer.IsElapsed () || m_lookAction == LookAction_LookAtSpot))
			{
				ClearPrimaryAttack ();

				m_isThrowingGrenade = false;
			}
			else
				PrimaryAttack ();
			if (IsHunting () && IsWellPastSafe () && IsUsingGrenade ())
				EquipBestWeapon (true);
			if (!IsSafe () && !IsUsingGrenade () && IsActiveWeaponOutOfAmmo ())
				EquipBestWeapon (false);
			if (!IsSafe () && !IsUsingGrenade () && IsUsingKnife () && !IsEscapingFromBomb ())
				EquipBestWeapon (false);
			if (!IsActiveWeaponReloading () && IsUsingPistol () && !IsPrimaryWeaponEmpty () && GetTimeSinceLastSawEnemy () > 5.0f)
				EquipBestWeapon (false);
			ReloadCheck ();
			SilencerCheck ();
			RespondToRadioCommands ();
			if (gpGlobals->time - m_touchPlayerTimestamp < 0.33f && m_touchPlayerEntity)
				StrafeAwayFromPosition (m_touchPlayerEntity->GetOrigin ());
			else
				m_touchPlayerEntity = NULL;
			if (m_isUNKNOWN0)
			{
				const float jumpDeltaTime (gpGlobals->time - m_discontinuityJumpTimestamp);

				if (jumpDeltaTime > 0.05f)
				{
					if (jumpDeltaTime < 0.6f)
						Crouch ();
					else
					{
						StandUp ();

						if (jumpDeltaTime > 0.75f)
							m_isUNKNOWN0 = false;
					}
				}
			}
			if (!IsAtHidingSpot () && !IsAttacking () && IsUsingSniperRifle () && IsUsingScope ())
				SecondaryAttack ();
			UpdatePeripheralVision ();
			if (m_bomber)
				m_chatter.SpottedBomber (m_bomber);
			if (CanSeeLooseBomb ())
				m_chatter.SpottedLooseBomb (TheBots->m_looseBomb);
			if (TheBots->m_mapType == BotManager::MapType_De)
			{
				float v105 (8.0f - m_profile->aggression * 2.0f);
				if (m_hasDefuseKit)
					v105 *= 0.66f;
				if (!IsEscapingFromBomb () && TheBots->m_isBombPlanted && m_gameState.IsPlantedBombLocationKnown () && TheBots->GetBombTimeLeft () < v105 && !IsDefusingBomb () && !IsAttacking ())
					EscapeFromBomb ();
			}
			else if (TheBots->m_mapType == BotManager::MapType_Cs)
			{
				if (m_team == Constants::TeamID_CounterTerrorist)
					UpdateHostageEscortCount ();
				else
				{
					const unsigned char result (m_gameState.ValidateHostagePositions ());
					if (result & 4u)
					{
						m_chatter.HostagesTaken ();

						Idle ();
					}
					else if (result & 2u)
					{
						m_gameState.m_UNKNOWN1 = true;

						Idle ();
					}
				}
			}
			if (gpGlobals->time - TheBots->m_roundStartTime <= 5.0f || m_profile->teamwork <= 0.4f || gpGlobals->time <= m_nextAutoFollowTime || IsBusy ())
				goto LABEL_204;
			if (!m_isFollowing)
			{
				if (!m_gameState.IsAtPlantedBombsite ())
				{
					if (g_engfuncs.pfnRandomFloat (0.0f, 1.0f) >= m_profile->teamwork)
						m_nextAutoFollowTime = gpGlobals->time + (1.0f - m_profile->teamwork) * 30.0f + 15.0f;
					else if (m_closestVisibleHumanFriend && gpGlobals->time > m_closestVisibleHumanFriend->m_botAllowAutoFollowTime && GetBotFollowCount (m_closestVisibleHumanFriend) < 2 && GetOrigin ().GetDistanceSquared (m_closestVisibleHumanFriend->GetOrigin ()) < MATH_GET_SQUARED (300.0f))
					{
						v67 = TheNavAreaGrid->GetNavArea (m_closestVisibleHumanFriend->GetOrigin (), 120.0);

						if (v67 != NULL && NavAreaTravelDistance (m_lastKnownArea, v67, PathCost (this, NavigationMesh::FASTEST_ROUTE)) < 300.0f)
						{
							Follow (m_closestVisibleHumanFriend);
							PrintIfWatched ("Auto-Following %s\n", m_closestVisibleHumanFriend->GetNetName ().GetData ());
							v75 = new BotStatement (&m_chatter, 3, 10.0f);
							v75->AppendPhrase (TheBotPhrases->GetPhrase (g_pGameRules->IsCareer () ? "FollowingCommander" : "FollowingSir"));
							m_chatter.AddStatement (v75, false);
						}
					}
				}
LABEL_204:
				if (!m_isFollowing)
					goto LABEL_245;
			}
			if (!m_followingTarget || !m_followingTarget->IsAlive ())
				StopFollowing ();
			if (m_profile->teamwork < 0.85f && m_profile->teamwork * 40.0f + 15.0f < GetFollowDuration ())
			{
				StopFollowing ();
				PrintIfWatched ("Stopping following - bored\n");
			}
			if (!m_isFollowing)
			{
LABEL_245:
				if (m_morale < MoraleType_Neutral && IsSafe () && GetSafeTimeRemaining () < 2.0f && IsHunting () && m_morale * -40.0f > g_engfuncs.pfnRandomFloat (0.0f, 100.0f) && (TheBots->IsOnOffense (this) || !TheBots->m_isDefenseRushing))
				{
					SetDisposition (DispositionType_OpportunityFire);
					Hide (m_lastKnownArea, g_engfuncs.pfnRandomFloat (3.0f, 15.0f), 750.0f, false);
					v87 = new BotStatement (&m_chatter, 3, 3.0f);
					v87->AppendPhrase (TheBotPhrases->GetPhrase ("WaitingHere"));
					m_chatter.AddStatement (v87, false);
				}
			}
			if (m_isAttacking)
				m_attackState.OnUpdate (this);
			else
				m_currentState->OnUpdate (this);
			if (m_isThrowingGrenade)
			{
				ResetStuckMonitor ();
				ClearMovement ();
			}
			if (IsReloading () && !m_isEnemyVisible)
			{
				ResetStuckMonitor ();
				ClearMovement ();
			}
			if (!IsAttacking () && m_checkWaitForHostagePatienceTimer.IsElapsed () && m_currentTaskID == TaskType_RescueHostages && GetRangeToFarthestEscortedHostage () > 500.0f)
			{
				if (m_waitForHostagePatience)
				{
					if (!m_waitForHostagePatienceTimer.IsElapsed ())
					{
						ResetStuckMonitor ();
						ClearMovement ();
					}
					else
					{
						m_waitForHostagePatience = false;
						m_checkWaitForHostagePatienceTimer.Start (3.0f);
					}
				}
				else
				{
					m_waitForHostagePatience = true;
					m_waitForHostagePatienceTimer.Start (10.0f);
				}
			}
			m_isSafe = IsSafe ();
			return;
	}
}

void CCSBot::UpdateLookAround (const bool a2/* = false - UNUSED!!!!!*/)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (m_inhibitLookAroundGlobalTime > gpGlobals->time)
		return;

	if (!m_isEnemyVisible && CanHearNearbyEnemyGunfire (1000.0f))
	{
		SetLookAt ("Check dangerous noise", m_noisePosition + Math::Vector3D (0.0f, 0.0f, 36.0f), PriorityType_High, 0.25f, false, 5.0f);

		m_inhibitLookAroundGlobalTime = gpGlobals->time + g_engfuncs.pfnRandomFloat (2.0f, 4.0f);

		return;
	}

	if (!IsLookingAtSpot (PriorityType_Medium) && GetTimeSinceLastSawEnemy () < 0.25f)
	{
		Math::Vector3D lookAt (m_lastKnownEnemyPosition);

		if (GetSimpleGroundHeight (lookAt, &lookAt.z, NULL))
		{
			lookAt.z += 36.0f;

			SetLookAt ("Last Enemy Position", lookAt, PriorityType_Medium, g_engfuncs.pfnRandomFloat (2.0f, 3.0f), true, 5.0f);

			return;
		}
	}

	if (UpdateLookAtNoise ())
		return;

	if (GetSpeedSquared () >= MATH_GET_SQUARED (10.0f))
	{
		if (IsUsingSniperRifle () && m_profile->skill > 0.4f)
		{
			if (IsViewMoving (1.0f))
			{
				if (m_iFOV <= 60)
					SecondaryAttack ();
			}
			else
				AdjustZoom (ComputeWeaponSightRange ());
		}

		if (m_lastKnownArea != NULL && m_nextLookAtHidingApproachPointGlobalTime <= gpGlobals->time)
		{
			m_nextLookAtHidingApproachPointGlobalTime = gpGlobals->time + (IsUsingSniperRifle () ? g_engfuncs.pfnRandomFloat (5.0f, 10.0f) : g_engfuncs.pfnRandomFloat (1.0f, 2.0f));

			if (m_approachPointsNumber > 0u)
				SetLookAt ("Approach Point (Hiding)", m_approachPoints[g_engfuncs.pfnRandomLong (0u, m_approachPointsNumber - 1u)] + Math::Vector3D (0.0f, 0.0f, 36.0f), PriorityType_Low, -1.0f, false, 5.0f);
			else
				ClearLookAt ();
		}
	}
	else
	{
		if (m_currentSpotEncounter != NULL
			&& !IsSafe ()
			&& !IsLookingAtSpot (PriorityType_Low)
			&& m_nextLookAtCurrentSpotEncounterGlobalTime <= gpGlobals->time)
		{
			float inversedSkill (1.0f - m_profile->skill);

			inversedSkill = MATH_GET_SQUARED (inversedSkill);
			inversedSkill = MATH_GET_SQUARED (inversedSkill);

			m_nextLookAtCurrentSpotEncounterGlobalTime = gpGlobals->time + g_engfuncs.pfnRandomFloat (10.0f, 30.0f) * inversedSkill;

			const Math::Vector3D direction (m_currentSpotEncounter->path.to - m_currentSpotEncounter->path.from);
			const float directionLength (direction.GetLength ());
			const float v24 (Math::fabsf (direction.x) >= Math::fabsf (direction.y) ? (m_variables->origin.x - m_currentSpotEncounter->path.from.x) / direction.x : (m_variables->origin.y - m_currentSpotEncounter->path.from.y) / direction.y);
			const float v30 (Math::GetClampedValueInRange (50.0f / directionLength + v24, 0.0f, 1.0f));
			const unsigned char MaximumSpots (8u);
			HidingSpot *spots[MaximumSpots];
			unsigned char nextSpotToSetIndex (0u), spotsNumber (0u);

			for (const Node_t *node (m_currentSpotEncounter->spotList.m_head->next); node != m_currentSpotEncounter->spotList.m_head; node = node->next)
			{
				const SpotOrder &spotOrder (node->element);

				if (gpGlobals->time - GetHidingSpotCheckTimestamp (spotOrder.spot) <= 10.0f)
					continue;

				if (spotOrder.t > v30)
					break;

				spots[nextSpotToSetIndex] = spotOrder.spot;

				if (++nextSpotToSetIndex >= MaximumSpots)
					nextSpotToSetIndex = 0u;

				if (spotsNumber < MaximumSpots)
					++spotsNumber;
			}

			if (spotsNumber > 0u)
			{
				HidingSpot *const spot (spots[g_engfuncs.pfnRandomLong (0u, spotsNumber - 1u)]);

				SetLookAt ("Encounter Spot", spot->GetPosition () + Math::Vector3D (0.0f, 0.0f, 36.0f), PriorityType_Low, 0.0f, true, 10.0f);

				SetHidingSpotCheckTimestamp (spot);
			}
		}
	}
}

BasePlayer *const CCSBot::FindMostDangerousThreat (void)
{
	Structures::TraceResult_t traceResult;
	float closestTemmateDistanceSquared (9999999.9f);
	float closestTemmateHumanDistanceSquared (9999999.9f);
	unsigned char threatsNumber (0u);

	enum { MaximumEnemies = Constants::MaximumClients / Constants::MaximumPlayableTeamsNumber };
	struct Enemy_t
	{
		BasePlayer *pointer;
		float       distanceSquared;
	};	// sizeof (Enemy_t) == 8 (2)

	Enemy_t enemies[MaximumEnemies];

	// Collect Threats:
	{
	m_bomber = NULL;
	m_closestVisibleFriend = NULL;
	m_closestVisibleHumanFriend = NULL;

	for (unsigned char index (1u); index <= gpGlobals->maximumClients; ++index)
	{
		BasePlayer *const player (UTIL_PlayerByIndex (index));

		if
		(
			player == NULL ||
			FNullEnt (player->m_variables) ||
			strcmp (player->GetName (), "") == 0 ||
			!player->IsPlayer () ||
			g_engfuncs.pfnIndexOfEdict (player->GetEdict ()) == g_engfuncs.pfnIndexOfEdict (GetEdict ()) ||
			!player->IsAlive ()
		)
			continue;

		// Same team?
		if (m_team == player->m_team)
		{
			UTIL_TraceLine (GetEyePosition (), player->GetOrigin (), ignore_monsters, ignore_glass, GetEdict (), &traceResult);

			if (traceResult.fraction < 1.0f)
				continue;

			const unsigned char playerIndex (static_cast <unsigned char> (g_engfuncs.pfnIndexOfEdict (player->GetEdict ())) - 1u);
			const float distanceSquared (GetOrigin ().GetDistanceSquared (player->GetOrigin ()));

			m_alivePlayersInfo[playerIndex].setTimestamp = gpGlobals->time;
			m_alivePlayersInfo[playerIndex].isEnemy = false;

			if (closestTemmateDistanceSquared > distanceSquared)
			{
				closestTemmateDistanceSquared = distanceSquared;
				m_closestVisibleFriend = player;
			}

			if (!player->IsBot () && closestTemmateHumanDistanceSquared > distanceSquared)
			{
				closestTemmateHumanDistanceSquared = distanceSquared;
				m_closestVisibleHumanFriend = player;
			}
		}
		else if (IsVisible (player, true, NULL))
		{
			const unsigned char playerIndex (static_cast <unsigned char> (g_engfuncs.pfnIndexOfEdict (player->GetEdict ())) - 1u);
			const float distanceSquared (GetOrigin ().GetDistanceSquared (player->GetOrigin ()));

			m_alivePlayersInfo[playerIndex].setTimestamp = gpGlobals->time;
			m_alivePlayersInfo[playerIndex].isEnemy = true;

			if (player->IsBombGuy ())
				m_bomber = player;

			// Sort Threats: (by distance)
			{
				if (threatsNumber == 0u)
				{
					threatsNumber = 1u;

					enemies[0u].pointer = player;
					enemies[0u].distanceSquared = distanceSquared;
				}
				else
				{
					unsigned char tempIndex (0u);

					for (/* Empty */; tempIndex < threatsNumber && enemies[tempIndex].distanceSquared <= distanceSquared; ++tempIndex);

					for (unsigned char tempIndex2 (threatsNumber - 1u); tempIndex2 >= tempIndex; --tempIndex2)
						enemies[tempIndex2 + 1u] = enemies[tempIndex2];	/// @warning CHECK ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

					enemies[tempIndex].pointer = player;
					enemies[tempIndex].distanceSquared = distanceSquared;

					if (threatsNumber < MaximumEnemies)
						++threatsNumber;
				}
			}
		}
	}
	}

	const unsigned char oldNearbyEnemyCount (m_nearbyEnemyCount), oldNearbyFriendCount (m_nearbyFriendCount);

	// Count nearby Friends & Enemies:
	{
		m_nearbyEnemyCount = m_nearbyFriendCount = 0u;

		for (unsigned char index (0u); index < Constants::MaximumClients; ++index)
		{
			if (m_alivePlayersInfo[index].setTimestamp <= 0.0f || gpGlobals->time - m_alivePlayersInfo[index].setTimestamp >= 3.0f)
				continue;

			if (m_alivePlayersInfo[index].isEnemy)
				++m_nearbyEnemyCount;
			else
				++m_nearbyFriendCount;
		}
	}

	if (oldNearbyEnemyCount == 0u && m_nearbyEnemyCount > 0u)
		m_firstSawEnemyTimestamp = gpGlobals->time;

	if (oldNearbyEnemyCount != m_nearbyEnemyCount || oldNearbyFriendCount != m_nearbyFriendCount)
		PrintIfWatched ("Nearby friends = %d, enemies = %d\n", m_nearbyFriendCount, m_nearbyEnemyCount);

	enum { MaximumPlaceRanks = 64u };
	struct PlaceRank_t
	{
		Place_t      place;
		unsigned int UNKNOWN_IndexOrCount;
	};	// sizeof (PlaceRank_t) == 8 (2)
	static PlaceRank_t placeRank[MaximumPlaceRanks];
	unsigned char UNKNOWN_Index0 (0u);
	unsigned char UNKNOWN_Index1 (0u);

	// Track enemy Place:
	{
		m_enemyPlace = UNDEFINED_PLACE;

		for (unsigned char index (0u); index < threatsNumber; ++index)
		{
			const NavigationArea *const enemyArea (enemies[index].pointer->IsBot () ? enemies[index].pointer->m_lastKnownArea ? TheNavMesh->GetNearestNavArea (enemies[index].pointer->GetOrigin ()));

			if (enemyArea == NULL || enemyArea->GetPlace () == UNDEFINED_PLACE)
				continue;

			unsigned char tempIndex (0u);

			for (/* Empty */; tempIndex < UNKNOWN_Index0 && placeRank[tempIndex].place != enemyArea->GetPlace (); ++tempIndex);

			if (tempIndex == UNKNOWN_Index0)
			{
				if (tempIndex < MaximumPlaceRanks)
				{
					placeRank[tempIndex].place = enemyArea->GetPlace ();
					placeRank[tempIndex].UNKNOWN_IndexOrCount = 1u;

					if (UNKNOWN_Index1 == 0u)
					{
						m_enemyPlace = placeRank[tempIndex].place;
						UNKNOWN_Index1 = placeRank[tempIndex].UNKNOWN_IndexOrCount;
					}

					UNKNOWN_Index0 = tempIndex + 1;
				}
			}
			else
			{
				if (++placeRank[tempIndex].UNKNOWN_IndexOrCount > UNKNOWN_Index1)
				{
					m_enemyPlace = placeRank[tempIndex].place;
					UNKNOWN_Index1 = placeRank[tempIndex].UNKNOWN_IndexOrCount;
				}
			}
		}
	}

	// Select Threat:
	{
		BasePlayer *bestEnemy (enemies[0u].pointer);

		for (unsigned char index (0u); index < threatsNumber; ++index)
		{
			BasePlayer *const enemy (enemies[index].pointer);

			if (!enemy->IsProtectedByShield ())
			{
				bestEnemy = enemy;

				break;
			}
		}

		return bestEnemy;
	}
}

class PathCost
{
	private:
		const CCSBot *const               m_bot;
		const NavigationMesh::RouteType_t m_routeType;

	public:
		inline PathCost (const CCSBot *const bot, const NavigationMesh::RouteType_t routeType) : m_bot (bot), m_routeType (routeType) { /* VOID */ }

	public:
		inline const float operator () (NavigationArea *const area, const NavigationArea *const fromArea, const NavigationLadder *const ladder) const
		{
			const float cost ((1.0f - m_bot->m_profile->aggression * 0.95f) * 100.0f);

			if (fromArea == NULL)
				return m_routeType == NavigationMesh::SAFEST_ROUTE ? area->GetDanger (m_bot->m_team - 1u) * cost : 0.0f;

			if (fromArea->HasAttributes (NAV_MESH_JUMP) && area->HasAttributes (NAV_MESH_JUMP))
				return -1.0f;

			const float distance (ladder != NULL ? ladder->GetLength () : fromArea->GetCenter ().GetDistance (area->GetCenter ()));
			float totalCost (fromArea->GetCostSoFar () + distance);

			if (cvar_bot_zombie.value > 0.0f)
				return totalCost;

			if (!area->IsConnected (fromArea))
			{
				const float damage (CCSBot::GetApproximateFallDamage (-fromArea->ComputeHeightChange (area)));

				if (damage > 0.0f)
				{
					if (damage + 10.0f >= m_bot->pev->health)
						return -1.0f;

					if (m_routeType == NavigationMesh::SAFEST_ROUTE || m_bot->m_profile->aggression * 15.0f + 10.0f < damage)
						totalCost += MATH_GET_SQUARED (damage) * 100.0f;
				}
			}

			if (area->HasAttributes (NAV_MESH_CROUCH))
			{
				float penalty (m_routeType == NavigationMesh::SAFEST_ROUTE ? 5.0f : 20.0f);

				if (m_hostageEscortCount > 0u)
					penalty *= 3.0f;

				totalCost += penalty * distance;
			}

			if (area->HasAttributes (NAV_MESH_JUMP))
				totalCost += distance;

			if (m_routeType == NavigationMesh::SAFEST_ROUTE)
				totalCost += area->GetDanger (m_bot->m_team - 1u) * distance * cost;

			if (m_bot->IsAttacking ())
				return totalCost;

			const float areaHalfSize (Math::GetMidPoint (area->GetExtent ().GetSizeX (), area->GetExtent ().GetSizeY ()));

			if (areaHalfSize < 1.0f)
				return totalCost;

			return area->GetPlayerCount (m_bot->m_team) * 50000.0f / areaHalfSize + totalCost;
		}

	private:
		inline PathCost &operator = (const PathCost &/*right*/);	// Avoid "warning C4512: 'PathCost' : assignment operator could not be generated".
};

const bool IsSpotOccupied (const BaseEntity *const entity, const Math::Vector3D &spotPosition)
{
	/// @todo CONTINUE DECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	float closestDistance;
	const BaseEntity *closestEntity (UTIL_GetClosestPlayer (spotPosition, &closestDistance));

	if (closestEntity != NULL && closestEntity != entity && closestDistance < 75.0f)
		return true;

	if (g_pHostages == NULL)
		return false;

	closestEntity = NULL;

	closestDistance = 100000000.0f;

	for (unsigned int hostageIndex (0u); hostageIndex < *(_DWORD *)(g_pHostages + 48); ++hostageIndex)
	{
		const Hostage *const hostage (*(_DWORD *)(4 * hostageIndex + g_pHostages));
		const float distance (spotPosition.GetDistance (hostage->GetOrigin ()));

		if (distance < closestDistance)
		{
			closestEntity = hostage;
			closestDistance = distance;
		}
	}

	return closestEntity != NULL && closestEntity != entity && closestDistance < 75.0f;
}

class CollectHidingSpotsFunctor
{
	private:
		enum { MaximumHidingSpots = 256u };

/*!off=0(0)*/		const BaseEntity *const     m_entity;
/*!off=4(1)*/		const Math::Vector3D *const	m_origin;	/// @warning I'M NOT SHURE ABOUT THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*!off=8(2)*/		const float                 m_radius;
/*!off=12(3)*/		HidingSpot                 *m_hidingSpots[MaximumHidingSpots];
/*!off=1036(259)*/		unsigned int                m_hidingSpotsNumber;
/*!off=1040(260)*/		const HidingSpot::Flag_t    m_flags;
/*!off=1044(261)*/		const Place_t               m_place;
/*!off=1048(262)*/		const bool                  m_includeCrouch;

	public:
		inline CollectHidingSpotsFunctor (const BaseEntity *const entity, const Math::Vector3D *const origin = NULL, const float radius = -1.0f, const HidingSpot::Flag_t flags = HidingSpot::IN_COVER, const Place_t place = UNDEFINED_PLACE, const bool includeCrouch = false) :
			m_entity (entity),
			m_origin (origin),
			m_radius (radius),
			m_hidingSpotsNumber (0u),
			m_flags (flags),
			m_place (place),
			m_includeCrouch (includeCrouch)
		{ /* VOID */ }

	private:
		inline CollectHidingSpotsFunctor &operator = (const CollectHidingSpotsFunctor &/*right*/);	// Avoid "warning C4512: 'CollectHidingSpotsFunctor' : assignment operator could not be generated".

	public:
		inline const bool operator () (const NavigationArea *const area) const
		{
			if (m_place > UNDEFINED_PLACE && area->GetPlace () != m_place)
				return true;

			const HidingSpotList &areaHidingSpots (area->GetHidingSpots ());

			for (HidingSpotList::IndexType_t index (0u); index < areaHidingSpots.GetElementNumber (); ++index)
			{
				HidingSpot *const hidingSpot (areaHidingSpots[index]);
				const Math::Vector3D &hidingSpotPosition (hidingSpot->GetPosition ());

				if (!m_includeCrouch)
				{
					const NavigationArea *const hidingSpotArea (TheNavMesh->GetNavArea (hidingSpotPosition));

					if (hidingSpotArea != NULL && hidingSpotArea->HasAttributes (NAV_MESH_CROUCH))
						continue;
				}

				if (m_radius > 0.0f && m_origin->GetDistanceSquared (hidingSpotPosition) > MATH_GET_SQUARED (m_radius))
					continue;

				if (!IsSpotOccupied (m_entity, hidingSpotPosition) && (hidingSpotArea->GetFlags () & m_flags))
				{
					m_hidingSpots[m_hidingSpotsNumber] = hidingSpot;

					if (++m_hidingSpotsNumber >= MaximumHidingSpots)
						return false;
				}
			}

			return true;
		}

	public:
		inline       HidingSpot *const GetHidingSpots       (void) const { return m_hidingSpots; }
		inline const unsigned int      GetHidingSpotsNumber (void) const { return m_hidingSpotsNumber; }
};
}
}
}