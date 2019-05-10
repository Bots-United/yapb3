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
// BotNavigator.hpp
//
// Class: BaseNavigator
//
// Version: $ID:$
//

#include <Core.hpp>

/***********************************
Taken from Craig W. Reynolds's paper
entitled "Steering Behaviors for
Autonomous Characters".
	cwr@red.com
	www.red.com/cwr
**********************************/
const float YaPB::BaseNavigator::HISTORY_SAMPLING_RATE (0.05f);

/** Toward a static target. */
Math::Vector3D seek (Math::Vector3D position, Math::Vector3D target, float max_speed) {
	return (target - position).Normalize () * max_speed;
}
Math::Vector3D seek (Math::Vector3D position, HalfLifeEngine::SDK::Classes::Edict *target, float max_speed) {
	return seek (position, target->variables.origin, max_speed);
}

/** Away from a static target. */
Math::Vector3D flee (Math::Vector3D position, Math::Vector3D target, float max_speed) {
	return (position - target).Normalize () * max_speed;
}
Math::Vector3D flee (Math::Vector3D position, HalfLifeEngine::SDK::Classes::Edict *target, float max_speed) {
	return flee (position, target->variables.origin, max_speed);
}

/**
 * Where the target will be if they maintain
 * current velocity.
 */
Math::Vector3D predict (Math::Vector3D position, HalfLifeEngine::SDK::Classes::Edict *target) {
	return (target->variables.velocity * position.GetDistance (target->variables.origin)) + target->variables.origin;
}

/** Toward a moving target. */
Math::Vector3D pursue (Math::Vector3D position, HalfLifeEngine::SDK::Classes::Edict *target, float max_speed, Math::Vector3D offset) {
	return seek (position, predict (position, target) + offset, max_speed);
}

Math::Vector3D pursue (Math::Vector3D position, HalfLifeEngine::SDK::Classes::Edict *target, float max_speed) {
	return pursue (position, target, max_speed, Math::Vector3D::ZeroValue);
}

/** Away from a moving target. */
Math::Vector3D evade (Math::Vector3D position, HalfLifeEngine::SDK::Classes::Edict *target, float max_speed) {
	return flee (position, predict (position, target), max_speed);
}

/** Like seek, except slows down as approaches target. */
/* TODO: this is broken, not sure why
Math::Vector3D arrive (Math::Vector3D position, Math::Vector3D target, float max_speed, float slowing_distance) {
	Math::Vector3D target_offset = target - position;
	float distance = target_offset.GetLength ();
	float ramped_speed = max_speed * (distance / slowing_distance);
	float clipped_speed = Math::GetMinimumValueBetween (ramped_speed, max_speed);
	return (clipped_speed / distance) * target_offset.Normalize ();
}
*/

/** Align with local clients. */
Math::Vector3D align (YaPB *steered, float distance_threshold)
{
	int num_locals = 1;
	Math::Vector3D vel_sum = steered->GetVelocity ();	// include self

	// search the world for players...
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);	// quick access to player

		if (client == steered || !client->IsAlive ())
			continue;	// skip invalid players and skip self (i.e. this bot)
/*
		if (!sameSquad (steered->GetYaPBPointer (), client))
			continue; // don't align with non-squad members
*/
		if (steered->IsPositionVisible (client->GetOrigin ())) {
			if (steered->GetOrigin ().GetDistance (client->GetOrigin ()) < distance_threshold) {
				++num_locals;
				vel_sum += client->GetVelocity ();
			}
		}
	}

	return vel_sum / static_cast <float> (num_locals); // avg velocity
}

/** Toward local clients. */
Math::Vector3D cohese (YaPB *steered, float distance_threshold, float max_speed, Math::Vector3D prev_velocity) {
	int num_locals = 1;
	Math::Vector3D loc_sum = steered->GetOrigin (); // include self

	// search the world for players...
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);	// quick access to player

		if (client == steered || !client->IsAlive ())
			continue;	// skip invalid players and skip self (i.e. this bot)
/*
		if (!sameSquad (steered->GetYaPBPointer (), client))
			continue; // don't cohese with non-squad members
*/
		if (steered->IsPositionVisible (client->GetOrigin ())) {
			if (steered->GetOrigin ().GetDistance (client->GetOrigin ()) < distance_threshold) {
				++num_locals;
				loc_sum += client->GetOrigin ();
			}
		}
	}

	if (num_locals == 1) // only self
		return prev_velocity;

	Math::Vector3D avg_location = loc_sum / static_cast <float> (num_locals);
	return seek (steered->GetOrigin (), avg_location, max_speed);
}

/** Away from local clients. */
Math::Vector3D separate (YaPB *steered, float distance_threshold, float max_speed, Math::Vector3D prev_velocity) {
//	YaPB *steered_bot = steered->GetYaPBPointer ();

	int num_locals = 1;
	Math::Vector3D destination = steered->GetOrigin ();

	// search the world for players...
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);	// quick access to player

		if (client == steered || !client->IsAlive ())
			continue;	// skip invalid players and skip self (i.e. this bot)
/*
		YaPB *client_bot = client->GetYaPBPointer ();
		bool clientNonBot = (client_bot == NULL);
*/
		if (steered->IsPositionVisible (client->GetOrigin ())
//			&& (clientNonBot || steered_bot->move_priority <= client_bot->move_priority)
			)
		{
			float d = steered->GetOrigin ().GetDistance (client->GetOrigin ());
			d -= HalfLifeEngine::SDK::Constants::HumanWidth;
			Math::ClampValueInRange (d, 1.0f, distance_threshold);
			if (d < distance_threshold) {
				++num_locals;
				Math::Vector3D repulse = (steered->GetOrigin () - client->GetOrigin ()).Normalize ();
				repulse *= distance_threshold;
				destination += repulse / MATH_GET_SQUARED (d);
			}
		}
	}

	if (num_locals == 1) // only self
		return prev_velocity;

	return seek (steered->GetOrigin (), destination, max_speed);
}

/** As member of flock. */
Math::Vector3D flock (YaPB *steered, float max_speed, Math::Vector3D prev_velocity, float a, float c, float s) {
	float sum = a + c + s; // dominance ratios

	Math::Vector3D ali = align (steered, 64.0);
	Math::Vector3D coh = cohese (steered, 256.0, max_speed, prev_velocity);
	Math::Vector3D sep = separate (steered, 32.0, max_speed, prev_velocity);

	return ((ali * a) + (coh * c) + (sep * s)) / sum;
}

/** As member of flock. */
Math::Vector3D flock (YaPB *steered, float max_speed, Math::Vector3D prev_velocity) {
	return flock (steered, max_speed, prev_velocity, 0.0, 1.0, 10.0);
}

/** Toward next waypoint. */
//Math::Vector3D follow_wp (YaPB *pBot, float max_speed) {
//	return seek (pBot->GetOrigin (), pBot->wp_origin, max_speed);
//}

/**
 * Corrects the current desired
 * velocity to accomodate obstacles.
 */
/*Math::Vector3D avoid_obstacles (YaPB *pBot, float max_speed)
{
/*	if (pBot->m_speed.move > 0.0) {
		if (obstacleJump (pBot)) { // TODO: what about SHOULD jump?
			speak (pBot, "OBSTACLE JUMPING");
			//pBot->GetEdict ()->variables.button |= IN_JUMP;
		}

		if (obstacleDuck (pBot)) {
			speak (pBot, "OBSTACLE DUCKING");
			//pBot->GetEdict ()->variables.button |= IN_DUCK;
		}

		if (obstacleFront (pBot)) {
			speak (pBot, "OBSTACLE FRONT");
			//pBot->desiredVelocity = 
		}
		
		if (obstacleLeft (pBot)) {
			speak (pBot, "OBSTACLE LEFT");
			//pBot->desiredVelocity = 
		}

		if (obstacleRight (pBot)) {
			speak (pBot, "OBSTACLE RIGHT");
			//pBot->desiredVelocity = 
		}
	}
*
	return pBot->GetNavigator ()->desiredVelocity;
}
*/
/**
 * Produces a smoother version of its input values.
 *
 * @param c  The constant is bound to [0, 1] and
 *     determines how quickly the smoothing responds
 *     to newer values. A constant of zero returns
 *     "x", while a constant of one returns "old".
 */
Math::Vector3D mavg (Math::Vector3D old, Math::Vector3D x, float c) {
	if (c > 1.0)
		c = 1.0;
	else if (c < 0.0)
		c = 0.0;

	return ((c * old) + ((1 - c) * x));
}

/**
 * Produces a smoother version of its input values.
 *
 * @param all  Array values are expected to be in
 *     chronological order (i.e. the newest item
 *     is at the end of the array).
 */
Math::Vector3D mavg (Math::Vector3D all[], int size, float c) {
	if (size < 1)
		return Math::Vector3D::ZeroValue; // TODO: throw exception
	if (size < 2)
		return all[0];

	Math::Vector3D temp = all[0];
	for (int i = 1; i < size; i++)
		temp = mavg (temp, all[i], c);
	return temp;
}
	
/**
 * Smooths the given looking vector by
 * taking into account the past.
 */
Math::Angles2D YaPB::BaseNavigator::smoothLook (void)
{
	float c = 0.5f; // [0,1] -- zero = instant response

	Math::Vector3D coarse[VIEW_HISTORY_SIZE + 1];

	for (int i = 0; i < VIEW_HISTORY_SIZE; i++)
		coarse[i] = prev_v_angle[i].BuildForwardVector (); // X inverted

	Math::Angles2D new_angle = m_destinationOrigin.ToAngles2D ();
	new_angle = new_angle.Clamp (); // X prime
	new_angle.pitch = -new_angle.pitch; // X inverted

	coarse[VIEW_HISTORY_SIZE] = new_angle.BuildForwardVector ();

	Math::Vector3D smooth = mavg (coarse, VIEW_HISTORY_SIZE + 1, c);

	return smooth.ToAngles2D ();
}

/**
 * Smooths the given movement vector
 * by taking into account the past.
 */
Math::Angles2D YaPB::BaseNavigator::smoothVelocity (void)
{
	float c = 0.5f; // [0,1] -- zero = instant response

	float speed = desiredVelocity.GetLength ();

	Math::Vector3D coarse[VEL_HISTORY_SIZE + 1];

	for (int i = 0; i < VEL_HISTORY_SIZE; i++) {
		Math::Vector3D p = prev_velocity[i];
		p.z = 0.0f; // horizontal plane only
		coarse[i] = p.Normalize ();
		coarse[i] = coarse[i] * speed;
	}

	coarse[VEL_HISTORY_SIZE] = desiredVelocity;
	coarse[VEL_HISTORY_SIZE].z = 0; // horizontal plane only
	coarse[VEL_HISTORY_SIZE] = (coarse[VEL_HISTORY_SIZE]).Normalize ();
	coarse[VEL_HISTORY_SIZE] * speed;

	Math::Vector3D smooth = mavg (coarse, VEL_HISTORY_SIZE + 1, c);

	return smooth.ToAngles2D ();
}

template <typename portalFunctor, typename pointFunctor> /*static */const unsigned short YaPB::Navigator_NavigationMesh::stringPull (const portalFunctor &portals, const unsigned short nportals, pointFunctor &pts, const unsigned short maxPts)
{
	// Funnel/String pulling algorithm....

	// Find straight path.
	unsigned short npts (0u);
	// Init scan state
	Math::Vector3D portalApex, portalLeft, portalRight;
	unsigned short apexIndex (0u), leftIndex (0u), rightIndex (0u);
	portalApex = portalLeft = portals.GetPortal (0u).left;
	portalRight = portals.GetPortal (0u).right;

	// Add start point.
	pts.SetPoint (portalApex, npts, 1u);

	if (++npts >= maxPts)
		return npts;

	for (unsigned short i (1u); i < nportals; ++i)
	{
		const NavigationMesh::BaseNavigationArea::Connection::Portal3D_t &portal (portals.GetPortal (i));

		// Update right vertex.
		if (Math::IsPointLeftOfSegment2D (portalApex, portalRight, portal.right) <= 0.0f)
		{
			if (portalApex.AreEqual2D (portalRight) || Math::IsPointLeftOfSegment2D (portalApex, portalLeft, portal.right) > 0.0f)
			{
				// Tighten the funnel.
				portalRight = portal.right;
				rightIndex = i;
			}
			else
			{
				// Right over left, insert left to path and restart scan from portal left point.
				if (!pts.GetPoint (npts - 1u).AreEqual2D (portalLeft))
				{
					pts.SetPoint (portalLeft, npts, leftIndex == 0u ? 1u : leftIndex);

					// If reached end of path or there is no space to append more vertices, return.
					if (++npts >= maxPts)
						return npts;
				}

				// Make current left the new apex.
				portalApex = portalLeft;
				apexIndex = leftIndex;
				// Reset portal
				portalLeft = portalRight = portalApex;
				leftIndex = rightIndex = apexIndex;
				// Restart scan
				i = apexIndex;
				continue;
			}
		}

		// Update left vertex.
		if (Math::IsPointLeftOfSegment2D (portalApex, portalLeft, portal.left) >= 0.0f)
		{
			if (portalApex.AreEqual2D (portalLeft) || Math::IsPointLeftOfSegment2D (portalApex, portalRight, portal.left) < 0.0f)
			{
				// Tighten the funnel.
				portalLeft = portal.left;
				leftIndex = i;
			}
			else
			{
				// Left over right, insert right to path and restart scan from portal right point.
				if (!pts.GetPoint (npts - 1u).AreEqual2D (portalRight))
				{
					pts.SetPoint (portalRight, npts, rightIndex == 0u ? 1u : rightIndex);

					// If reached end of path or there is no space to append more vertices, return.
					if (++npts >= maxPts)
						return npts;
				}

				// Make current right the new apex.
				portalApex = portalRight;
				apexIndex = rightIndex;
				// Reset portal
				portalLeft = portalRight = portalApex;
				leftIndex = rightIndex = apexIndex;
				// Restart scan
				i = apexIndex;
				continue;
			}
		}
	}
/*
	// If the point already exists, remove it and add reappend the actual end location.
	if (pts.GetPoint (npts - 1u).AreEqual2D (portals.GetPortal (nportals - 1u).left))
		--npts;
*/
	// Append last point to path.
	if (npts < maxPts && !pts.GetPoint (npts - 1u).AreEqual2D (portals.GetPortal (nportals - 1u).left)/*! ADDED BY EDWARD !!!*/)
	{
		pts.SetPoint (portals.GetPortal (nportals - 1u).left, npts, nportals - 1u);
		++npts;
	}

	return npts;
}

void YaPB::BaseNavigator::CheckStuck (void)
{
	// this function checks if the bot doesn't move as fast as it would like to. It is called
	// every frame to do a periodic check; then, by comparing the bot's current position and
	// the previously recorded one, and regarding to the contents of the BotLegs structure,
	// this function can tell if the bot is stuck or not.

	const float minimumWalkVelocity (20.0f/*2.0f*/);
	const float minimumLadderVelocity (10.0f/*2.0f*/);
	const float idealSpeed (GetMaximumSpeed ());

	// if bot is supposed to move, record if bot is stuck
	if (idealSpeed <= 10.0f || !(m_owner->m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::ButtonBits_InMove))
	{
		// bot is supposed not to move
		m_previousOrigin = m_owner->GetOrigin ();//Math::Vector3D::ZeroValue;
		m_prevCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 2.0f;//0.0f;
		m_stuckAmount = 0.0f;
		m_stuckTimestamp = 0.0f;

		return;
	}
/*
/// Original YaPB method:
	{
	m_stuckAmount = 0.0f;
	m_movedDistanceSquared = MATH_GET_SQUARED (2.0f);

	if (m_prevCheckTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
		// see how far bot has moved since the previous position...
		m_movedDistanceSquared = m_owner->GetOrigin ().GetDistanceSquared (m_previousOrigin);

		// save current position as previous
		m_previousOrigin = m_owner->GetOrigin ();

		// and update timer to 200 milliseconds
		m_prevCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.2f;
	}

	if (Math::fabsf (m_speed.move) > 1.0f || Math::fabsf (m_speed.strafe) > 1.0f)
	{
		// didn't we move enough previously?
		if (m_movedDistanceSquared < MATH_GET_SQUARED (2.0f) && m_prevSpeed.move >= 1.0f)
		{
			// then consider being stuck
			m_prevCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
			m_stuckAmount = 1.0f;	// set stuck flag and assume bot is stuck
			m_stuckTimestamp = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
			m_stuckOrigin = m_owner->GetOrigin ();

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot \"%s\" IS STUCK!!!", m_owner->GetName ().GetData ());
		}
	}

	// save the previous speed (for checking if stuck)
	m_prevSpeed.move = Math::fabsf (m_speed.move);
	m_prevSpeed.strafe = Math::fabsf (m_speed.strafe);
	}
*/
#if 0
/// Whistler's method:
	{
		// is it the time to check if bot is stuck?
/*		if (m_prevCheckTime == 0.0f)
		{
			// first time, just record bot's current location
			m_prevCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 2.0f;
			m_previousOrigin = m_owner->GetOrigin ();
			m_stuckAmount = 0.0f;
		}
		else */if (m_prevCheckTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		{
			m_prevCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.5f;	// set next stuck checking time

			const bool isOnFloor (m_owner->IsOnFloor ());

			// if bot is on ground, check 2D Vector only; else check 3D Vector
			if (isOnFloor)
				m_movedDistanceSquared = m_owner->GetOrigin ().GetDistanceSquared2D (m_previousOrigin);
			else
				m_movedDistanceSquared = m_owner->GetOrigin ().GetDistanceSquared (m_previousOrigin);

			{
			const float dist (Math::sqrtf (m_movedDistanceSquared));
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::BaseNavigator::CheckStuck(): movedDistance=%.2f, +frame=%.2f, stuckAmt=%.2f.", m_owner->GetName ().GetData (), dist, dist * m_owner->GetFrameIntervalTime (), m_stuckAmount);
			}

//			m_movedDistanceSquared *= m_owner->GetFrameIntervalTime ();

			// Did we NOT move enough previously?
			if (m_movedDistanceSquared < MATH_GET_SQUARED (minimumWalkVelocity))
			{
				m_stuckAmount += 0.3f;	// consider being stuck if so

				if (m_stuckAmount > 1.0f)
					m_stuckAmount = 1.0f;
			}
			else
			{
				const float percentMoved ((isOnFloor ? m_owner->GetEdict ()->GetSpeed2D () : m_owner->GetEdict ()->GetSpeed ()) / idealSpeed);

				m_stuckAmount -= 0.2f * percentMoved;

				if (m_stuckAmount < 0.0f)
					m_stuckAmount = 0.0f;
			}

			m_previousOrigin = m_owner->GetOrigin ();	// record bot's current location as previous
/*
			if (m_fPrevIsStuck && !IsStuck ())
				m_owner->DebugMessage (DEBUG_BOTNAV, "Bot is UNSTUCK");
			else if (!m_fPrevIsStuck && IsStuck ())
				m_owner->DebugMessage (DEBUG_BOTNAV, "Bot is STUCK!!!");

			m_fPrevIsStuck = IsStuck ();*/
		}
	}
#endif	// if 0
#if 0
/// PMB method:
	{
	m_averageVelocity += m_owner->IsOnFloor () ? m_owner->GetEdict ()->GetSpeed2D () : m_owner->GetEdict ()->GetSpeed ();
	++m_averageVelocityFramesCount;
/*
	// if bot is controlled by the player...
	if (is_controlled)
	{
		m_isStucked = false;	// then it can't be stuck

		return;
	}
*/
	if (m_prevCheckTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		return;	// cancel if can't tell yet

	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::BaseNavigator::CheckStuck(): avgVel=%.2f, avgVelFramesCount=%u, avgVel/=%.2f.", m_owner->GetName ().GetData (), m_averageVelocity, m_averageVelocityFramesCount, m_averageVelocity / m_averageVelocityFramesCount);

	// now compute the real average
	m_averageVelocity /= m_averageVelocityFramesCount;

	// if bot definitely doesn't move as fast as it would like to...
	if (m_averageVelocity < minimumWalkVelocity)
	{
		m_isStucked = true;	// set stuck flag and assume bot is stuck

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "ALERT, %s IS STUCK!!!!", m_owner->GetName ().GetData ());
	}
	else
		m_isStucked = false;	// else consider bot is not stuck

	m_averageVelocity = 0.0f;	// reset average velocity
	m_averageVelocityFramesCount = 0u;	// reset frames count
	m_prevCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.5f;	// next check in 0.5 second
	}
#endif	// if 0
//#if 0
/// ZBot method:
	{
	if (m_isStucked)
	{
		if (m_owner->GetOrigin ().GetDistanceSquared (m_stuckOrigin) > MATH_GET_SQUARED (75.0f))
		{
			#if defined _DEBUG
//			PrintIfWatched ("UN-STUCK\n");

			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <bool> () && m_owner->IsDebuggingBot ())
				HalfLifeEngine::Globals::g_halfLifeEngine->EmitSound (*m_owner, HalfLifeEngine::SDK::Constants::SoundChannel_Item, "buttons/bell1.wav");
			#endif	// if defined _DEBUG

			m_isStucked = false;
			m_stuckTimestamp = 0.0f;
//			m_wiggleJumpTimestamp = 0.0f;
			m_distancesToPreviousOriginSinceStuckCurrentIndex0 = 0u;
			m_distancesToPreviousOriginSinceStuckCurrentIndex1 = 0u;
			m_currentPathIndexSetTimestamp = 0.0f;
		}
	}
	else
	{
		const float distance (m_owner->IsOnFloor () ? m_owner->GetOrigin ().GetDistance2D (m_previousOrigin) : m_owner->GetOrigin ().GetDistance (m_previousOrigin));

//		InternalAssert (m_owner->GetFrameIntervalTime () > 0.0f);	// OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (m_owner->GetFrameIntervalTime () > 0.0f)
		{
		m_distancesToPreviousOriginSinceStuck[m_distancesToPreviousOriginSinceStuckCurrentIndex0] = distance / m_owner->GetFrameIntervalTime ();

		if (++m_distancesToPreviousOriginSinceStuckCurrentIndex0 == MaximumStuckTable)
			m_distancesToPreviousOriginSinceStuckCurrentIndex0 = 0u;

		if (m_distancesToPreviousOriginSinceStuckCurrentIndex1 >= MaximumStuckTable)
		{
			float totalDistanceMoved (0.0f);
			const float distanceMinimum (/*m_currentLadder != NULL ? minimumLadderVelocity : */minimumWalkVelocity);

			for (unsigned char index (0u); index < m_distancesToPreviousOriginSinceStuckCurrentIndex1; ++index)
				totalDistanceMoved += m_distancesToPreviousOriginSinceStuck[index];

			if (totalDistanceMoved / m_distancesToPreviousOriginSinceStuckCurrentIndex1 < distanceMinimum)
			{
				m_stuckTimestamp = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
				m_stuckOrigin = m_owner->GetOrigin ();
//				m_wiggleJumpTimestamp = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_engfuncs.pfnRandomFloat (0.0f, 0.5f);

				#if defined _DEBUG
//				PrintIfWatched ("STUCK\n");

				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <bool> () && m_owner->IsDebuggingBot ())
					HalfLifeEngine::Globals::g_halfLifeEngine->EmitSound (*m_owner, HalfLifeEngine::SDK::Constants::SoundChannel_Item, "buttons/button11.wav");

				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::BaseNavigator::CheckStuck(): STUCK!", m_owner->GetName ().GetData ());
				#endif	// if defined _DEBUG

				m_isStucked = true;
			}
		}
		else
			++m_distancesToPreviousOriginSinceStuckCurrentIndex1;
		}
	}

	m_previousOrigin = m_owner->GetOrigin ();
	}
//#endif	// if 0
	m_stuckAmount = m_isStucked;	// REMOVE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

/*virtual */void YaPB::BaseNavigator::Think (void)
{
	if (m_owner->GetEdict ()->variables.flags & HalfLifeEngine::SDK::Constants::FL_FROZEN)
		return;

	m_maximumSpeed = m_owner->GetEdict ()->variables.maximumSpeed;
/*
	/// @warning REMOVE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
	if (!Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_BotBuy)->GetValue <bool> ())
		m_maximumSpeed = 0.0f;
	else if (!(g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.buttons & HalfLifeEngine::SDK::Constants::IN_FORWARD))
		m_maximumSpeed *= 0.25f;	// Reduse his speed to move like a turtle.........
	}
*/
	m_speed.move = (m_owner->m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_FORWARD) ? m_maximumSpeed : (m_owner->m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_BACK) ? -m_maximumSpeed : 0.0f;
	m_speed.strafe = (m_owner->m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_MOVELEFT) ? -m_maximumSpeed : (m_owner->m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_MOVERIGHT) ? m_maximumSpeed : 0.0f;

	// calculate 2 direction vectors, 1 without the up/down component
	m_moveDirection = (m_destinationOrigin - (m_owner->GetOrigin ()/* + m_owner->GetVelocity () * m_owner->GetFrameIntervalTime ()*/));
//	const Math::Vector3D directionNormal (m_moveDirection.GetNormalized ());

	m_moveAngles = m_moveDirection.ToAngles2D ();

	m_moveAngles.pitch = -m_moveAngles.pitch;	// invert for engine
/*
	// Fastrun TEST:
	{
		if (m_owner->m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_FORWARD)
		{
			m_moveAngles.yaw += 45.0f;

			static float nextTime (0.0f);

			if (nextTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			{
				nextTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_TimerGrenade)->GetValue <float> ();

				m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_MOVELEFT;
				m_speed.strafe = -250.0f;
			}
		}
	}
*/
	if ((m_jumpTime + 0.85f >= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () && !m_owner->IsOnFloor () && !m_owner->GetEdict ()->IsInWater ()) || m_duckTime >= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		m_owner->GetEdict ()->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_DUCK;

	CheckStuck ();

	if (IsStuck ())
		Jump ();
#if 0
	{
	// movement history

	// TODO: smooth speed too

	if (movement_time <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ()) {
		movement_time = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + HISTORY_SAMPLING_RATE;

		unsigned char i;

		for (i = 0; i < VIEW_HISTORY_SIZE - 1; i++)
			prev_v_angle[i] = prev_v_angle[i + 1];
		prev_v_angle[VIEW_HISTORY_SIZE - 1] = m_moveAngles;

		for (i = 0; i < VEL_HISTORY_SIZE - 1; i++)
			prev_velocity[i] = prev_velocity[i + 1];
		prev_velocity[VEL_HISTORY_SIZE - 1] = desiredVelocity;
	}

	desiredVelocity = seek (m_owner->GetOrigin (), m_destinationOrigin, m_maximumSpeed);//follow_wp (m_owner, m_maximumSpeed);

	/* unaligned collision avoidance */

	desiredVelocity = separate (m_owner, 32.0f, m_maximumSpeed, desiredVelocity);
	//desiredVelocity = flock (m_owner, m_maximumSpeed, desiredVelocity);

	/* avoid obstacles */

//	desiredVelocity = avoid_obstacles (m_owner, m_maximumSpeed);

	/* execute movement */

	m_moveAngles = smoothLook ();

	m_moveAngles.Clamp ();

	m_moveAngles.pitch = -m_moveAngles.pitch; // invert for engine

	Math::Angles2D velAngle = smoothVelocity ();
	float yaw_diff = m_moveAngles.yaw - velAngle.yaw;

	while (yaw_diff < 0.0f) {
		yaw_diff += 360.0f;
		// [0,oo)
	}

	while (yaw_diff >= 360.0f) {
		yaw_diff -= 360.0f;
		// [0,360)
	}

	float forwardPercent = yaw_diff;
	if (forwardPercent >= 180.0f) {
		forwardPercent -= 180.0f;
		// [0,180) -- fold to half-circle
	}

	if (forwardPercent > 90.0f) {
		forwardPercent -= (forwardPercent - 90.0f) * 2.0f;
		// [0,90] w/ 90=>90 & 180=>0 -- fold to quarter circle
	}

	// goal: 90=>0 & 0=>1
	forwardPercent *= -1.0f; // rotate
	forwardPercent += 90.0f; // shift
	forwardPercent /= 90.0f; // scale

	// reduces stuttering when moving very little
	if (m_maximumSpeed < 5.0f)
		m_maximumSpeed = 0.0f;

	m_speed.move = forwardPercent * m_maximumSpeed;
	if (90.0f < yaw_diff && yaw_diff < 270.0f) // backpedal
		m_speed.move *= -1.0f;
	m_speed.strafe = m_maximumSpeed - Math::fabsf (m_speed.move);
	if (yaw_diff > 180.0f) // left
		m_speed.strafe *= -1.0f;
//	float move_vertical = 0.0f; // never used

	desiredVelocity.z = 0.0f;

	#if defined MULTIPLE_DRAW_METHODS
	// yellow = desiredVelocity
	g_drawInterface->DrawLine (m_owner->GetOrigin (), m_owner->GetOrigin () * Math::Vector2D (m_speed.move, m_speed.strafe), Color <> (255u, 255u, 0u), 255u, 2u, false);
	#endif	// if defined MULTIPLE_DRAW_METHODS
	}
#endif	// if 0
}

/*virtual */void YaPB::BaseNavigator::AliveSpawnPost (void)
{
	// reset move and strafe speed...
	m_speed.Reset ();

	m_destinationOrigin = Math::Vector3D::ZeroValue;
	m_moveAngles = Math::Angles2D::ZeroValue;	// reset move angles
	m_previousDistanceToClosestPathElementSquared2D = 0.0f;
	m_distanceToClosestPathElementCheckTime = 0.0f;
	m_stuckCheckTime = 0.0f;
	m_duckTime = 0.0f;
	m_jumpTime = 0.0f;
	m_prevCheckTime = g_server->GetRoundInformation ().freezeEndTime + 1.5f;	// fake "paused" since bot is NOT stuck
	m_previousOrigin = m_owner->GetOrigin ();
	m_movedDistanceSquared = 0.0f;
	m_stuckAmount = 0.0f;
	m_doMovementToGoal = true;

	ResetStuckMonitor ();
}

const bool YaPB::Navigator_NavigationMesh::BuildRoute (NavigationMesh::NavigationArea *const source, NavigationMesh::NavigationArea *const destination, const Math::Vector3D *const destinationVector, PathNodeArray_t &route)
{
	InternalAssert (destination != NULL || destinationVector != NULL);

	route.RemoveAll ();

	NavigationMesh::NavigationArea *closestArea;

	if (!NavigationMesh::NavAreaBuildPath (source, destination, destinationVector, NavigationMesh::ShortestPathCost/*/PathCost*/ (), &closestArea))
		return false;

	// Reliability check.
	InternalAssert (closestArea != NULL);

	// Store some path information....
	{
		PathNodeArray_t::IndexType_t currentPathNodesNumber (0u);
		NavigationMesh::NavigationArea *area (closestArea);

	for (/* Empty */; area != NULL; area = area->GetParent ())
		++currentPathNodesNumber;

	// Reliability check.
	InternalAssert (currentPathNodesNumber > 0u);

	if (currentPathNodesNumber >= MaximumPathNodesNumber)
		currentPathNodesNumber = MaximumPathNodesNumber - 1u;

	route.SetElementNumber (currentPathNodesNumber);

	// Fill route....
	for (area = closestArea; currentPathNodesNumber > 0u && area != NULL; area = area->GetParent ())
	{
		--currentPathNodesNumber;

		route[currentPathNodesNumber].area = area;
		route[currentPathNodesNumber].traverseType = area->GetParentHow ();
		route[currentPathNodesNumber].point = area->GetCenter ();	/// This will be initialized in ComputePathPositions()....

		// Compute portal.
		if (area->GetParent () != NULL)
			area->GetParent ()->ComputePortal3D (area, static_cast <NavigationMesh::NavDirType_t> (area->GetParentHow ()), route[currentPathNodesNumber].portal);
	}

	// Post compute portal.
	{
		route.GetFirstElement ().portal.left = route.GetFirstElement ().portal.right = /*source->GetCenter*/m_owner->GetOrigin ();
		route.GetLastElement ().portal.left = route.GetLastElement ().portal.right = destination != NULL ? destination->GetCenter () : *destinationVector;
	}
	}

	if (route.GetElementNumber () == 1u)
	{
		BuildTrivialPath (destination != NULL ? destination->GetCenter () : *destinationVector, route);

		return true;
	}

	if (ComputePathPositions (route))
	{
		/// @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return true;
	}

	return false;
}
const bool YaPB::Navigator_NavigationMesh::BuildRoute (NavigationMesh::NavigationArea *const source, NavigationMesh::NavigationArea *const destination, PathNodeArray_t &route)
{
	return BuildRoute (source, destination, NULL, route);
}
const bool YaPB::Navigator_NavigationMesh::BuildRoute (NavigationMesh::NavigationArea *const source, const Math::Vector3D &destination, PathNodeArray_t &route)
{
	return BuildRoute (source, NULL, &destination, route);
}
const bool YaPB::Navigator_NavigationMesh::BuildRoute (BaseNavigationElement *const destination)
{
	m_pathPortalsNumber = 0u;
	m_pathPointsNumber = 0u;
	m_pathPoints2.RemoveAll ();

	const bool result (BuildRoute (m_owner->m_lastNavigationArea, destination->GetAreaPointer <NavigationMesh::NavigationArea> (), m_path));

	if (result)
	{
		m_currentPathIndex = 1u;

		m_previousDistanceToClosestPathElementSquared2D = m_owner->GetOrigin ().GetDistanceSquared2D (destination->GetAreaPointer <NavigationMesh::NavigationArea> ()->GetCenter ());
		m_distanceToClosestPathElementCheckTime = 0.0f;

		// Post compute portal.
		{
			m_path.GetFirstElement ().portal.left = m_path.GetFirstElement ().portal.right = m_owner->GetOrigin ();
			m_path.GetLastElement ().portal.left = m_path.GetLastElement ().portal.right = destination->GetAreaPointer <NavigationMesh::NavigationArea> ()->GetCenter ();
		}
	}

	return result;
}
const bool YaPB::Navigator_NavigationMesh::BuildRoute (const Math::Vector3D &destination)
{
	m_pathPortalsNumber = 0u;
	m_pathPointsNumber = 0u;
	m_pathPoints2.RemoveAll ();

	const bool result (BuildRoute (m_owner->m_lastNavigationArea, destination, m_path));

	if (result)
	{
		m_currentPathIndex = 1u;

		m_previousDistanceToClosestPathElementSquared2D = m_owner->GetOrigin ().GetDistanceSquared2D (destination);
		m_distanceToClosestPathElementCheckTime = 0.0f;

		// Post compute portal.
		{
			m_path.GetFirstElement ().portal.left = m_path.GetFirstElement ().portal.right = m_owner->GetOrigin ();
			m_path.GetLastElement ().portal.left = m_path.GetLastElement ().portal.right = destination;
		}
	}

	return result;
}

void YaPB::Navigator_NavigationMesh::SetPathIndex (const PathNodeArray_t::IndexType_t newPathIndex)
{
	InternalAssert (HasRoute ());
	InternalAssert (newPathIndex > 0u);

	const PathNodeArray_t::IndexType_t lastPathIndex (m_path.GetLastElementIndex ());

	m_currentPathIndex = newPathIndex > lastPathIndex ? lastPathIndex : newPathIndex;
	m_currentPathIndexSetTimestamp = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
	m_previousDistanceToClosestPathElementSquared2D = m_owner->GetOrigin ().GetDistanceSquared2D (m_path[m_currentPathIndex - 1u].point);
	m_distanceToClosestPathElementCheckTime = 0.0f;
/*
	if (m_path[m_currentPathIndex].area->IsNavigationLadder ())
	{
		SetupLadderMovement ();

		return;
	}

	m_currentLadder = NULL;
	m_currentSpotEncounter = m_currentPathIndex >= m_path.GetElementNumber ()/*! @note DO WE NEED THIS CHECK?!?!??!?!?!*//* || m_currentPathIndex < 2u ? NULL : m_path[m_currentPathIndex - 1u].area->GetSpotEncounter (m_path[m_currentPathIndex - 2u].area, m_path[m_currentPathIndex].area);
*/}
void YaPB::Navigator_NavigationMesh::BuildTrivialPath (const Math::Vector3D &destination, PathNodeArray_t &route)
{
	route.SetElementNumber (2u);

	route[0u].area = m_owner->m_lastNavigationArea;
	route[0u].point = m_owner->GetOrigin ();
	route[0u].point.z = m_owner->m_lastNavigationArea->GetZ (route[0u].point);
	route[0u].traverseType = NavigationMesh::NUM_TRAVERSE_TYPES;

	route[1u].area = m_owner->m_lastNavigationArea;
	route[1u].point = destination;
	route[1u].point.z = m_owner->m_lastNavigationArea->GetZ (destination);
	route[1u].traverseType = NavigationMesh::NUM_TRAVERSE_TYPES;
}
void YaPB::Navigator_NavigationMesh::BuildTrivialPath (const Math::Vector3D &destination)
{
	m_currentPathIndex = 1u;

	BuildTrivialPath (destination, m_path);

//	m_currentSpotEncounter = NULL;
	m_currentPathIndexSetTimestamp = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
	m_previousDistanceToClosestPathElementSquared2D = m_owner->GetOrigin ().GetDistanceSquared2D (destination);
	m_distanceToClosestPathElementCheckTime = 0.0f;
//	m_currentLadder = NULL;
	m_destinationOrigin = destination;
}

const bool YaPB::Navigator_NavigationMesh::ComputePathPositions (PathNodeArray_t &route)
{
	m_pathPortalsNumber = 0u;
	m_pathPoints2.RemoveAll ();

	{
		const NavigationMesh::NavigationArea *areas[MaximumPathNodesNumber] = {route.GetFirstElement ().area->GetAreaPointer <NavigationMesh::NavigationArea> (), route.GetFirstElement ().area->GetAreaPointer <NavigationMesh::NavigationArea> ()};
		unsigned int areasNumber (2u);

		{
		// Start portal
		m_pathPortals[m_pathPortalsNumber].left = m_pathPortals[m_pathPortalsNumber].right = /*route.GetFirstElement ().area->GetAreaPointer <NavigationMesh::NavigationArea> ()->GetCenter ()*/m_owner->GetOrigin ();
		++m_pathPortalsNumber;
		}

	// Portal between navmesh polygons
	route.GetLastElement ().area->GetAreaPointer <NavigationMesh::NavigationArea> ()->ComputePathPortals3D (m_pathPortals, m_pathPortalsNumber, areas, areasNumber);

		{
		// End portal
		m_pathPortals[m_pathPortalsNumber].left = m_pathPortals[m_pathPortalsNumber].right = route.GetLastElement ().area->GetAreaPointer <NavigationMesh::NavigationArea> ()->GetCenter ();
		++m_pathPortalsNumber;
		}

		areas[areasNumber] = route.GetLastElement ().area->GetAreaPointer <NavigationMesh::NavigationArea> ();
		areas[++areasNumber] = route.GetLastElement ().area->GetAreaPointer <NavigationMesh::NavigationArea> ();
		++areasNumber;
	}

//	const SimplePortalWrapper portalFunctor (m_pathPortals);
//	SimplePointWrapper pointFunctor (m_pathPoints);
	const PortalWrapper portalFunctor (route);
	PointWrapper pointFunctor (m_pathPoints2);
	m_pathPointsNumber = stringPull (portalFunctor/*m_pathPortals*/, route.GetElementNumber ()/*m_pathPortalsNumber*/, pointFunctor/*m_pathPoints*/, MaximumPathNodesNumber);
	m_pathPoints2.SetElementNumber (m_pathPointsNumber);
	for (unsigned short index (0u); index < m_pathPoints2.GetElementNumber (); ++index)
	{
		InternalAssert (m_pathPoints2[index].prevNodeIndex < route.GetElementNumber ());
//		if (m_pathPoints2[index].prevNodeIndex >= route.GetElementNumber ())
//			AddLogEntry (true, LogLevel_Error, false, "m_pathPoints2[%u].prevNodeIndex(%u) >= (%u)route.GetElementNumber()!!!", index, m_pathPoints2[index].prevNodeIndex, route.GetElementNumber ());
		InternalAssert (m_pathPoints2[index].nextNodeIndex < route.GetElementNumber ());
//		if (m_pathPoints2[index].nextNodeIndex >= route.GetElementNumber ())
//			AddLogEntry (true, LogLevel_Error, false, "m_pathPoints2[%u].nextNodeIndex(%u) >= (%u)route.GetElementNumber()!!!", index, m_pathPoints2[index].nextNodeIndex, route.GetElementNumber ());
	}

	/// @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	return m_pathPointsNumber > 0u;
}

void YaPB::Navigator_NavigationMesh::MoveTowardsPath (void)
{
	if (m_currentPathIndex == 0u)
		return;

	InternalAssert (m_currentPathIndex < m_path.GetElementNumber ());

	#if defined MULTIPLE_DRAW_METHODS
	// Draw debug stuff....
	{
		// Draw path portals....
		for (unsigned short index (0u); index < m_pathPortalsNumber; ++index)
		{
			const Color <> color (0u, 0u, 255u);

//			g_drawInterface->DrawLine (m_pathPortals[index].left, ColorWithAlpha <> (color, 255u), m_pathPortals[index].right, ColorWithAlpha <> (color, 100u), 255u, 5u, false);
			g_drawInterface->DrawLine (m_pathPortals[index].left, m_pathPortals[index].right, color, 255u, 2u, false);
		}

		// Draw path corridor....
		for (unsigned short index (1u); index < m_pathPortalsNumber; ++index)
		{
			const Color <> colorLeft (255u, 0u, 255u);	// pink
			const Color <> colorRight (255u, 255u, 255u);	// white

			g_drawInterface->DrawLine (m_pathPortals[index - 1u].left, m_pathPortals[index].left, colorLeft, 255u, 2u, false);
			g_drawInterface->DrawLine (m_pathPortals[index - 1u].right, m_pathPortals[index].right, colorRight, 255u, 2u, false);
		}
/*/
		// Draw path portals....
		for (PathNodeArray_t::IndexType_t index (0u); index < m_path.GetElementNumber (); ++index)
		{
			const Color <> color (0u, 0u, 255u);

//			g_drawInterface->DrawLine (m_path[index].portal.left, ColorWithAlpha <> (color, 255u), m_path[index].portal.right, ColorWithAlpha <> (color, 100u), 255u, 5u, false);
			g_drawInterface->DrawLine (m_path[index].portal.left, m_path[index].portal.right, color, 255u, 2u, false);
		}

		// Draw path corridor....
		for (PathNodeArray_t::IndexType_t index (1u); index < m_path.GetElementNumber (); ++index)
		{
			const Color <> colorLeft (255u, 0u, 255u);	// pink
			const Color <> colorRight (255u, 255u, 255u);	// white

			g_drawInterface->DrawLine (m_path[index - 1u].portal.left, m_path[index].portal.left, colorLeft, 255u, 2u, false);
			g_drawInterface->DrawLine (m_path[index - 1u].portal.right, m_path[index].portal.right, colorRight, 255u, 2u, false);
		}
*/
		// Draw path points calculated by funnel algorithm....
//		for (unsigned short index (1u); index < m_pathPointsNumber; ++index)
		for (unsigned short index (1u); index < m_pathPoints2.GetElementNumber (); ++index)
		{
			const float fraction (static_cast <float> (index) / m_pathPointsNumber);
			const float fractionInv (1.0f - fraction);
			const Color <> color (static_cast <unsigned char> (255u * fractionInv), static_cast <unsigned char> (255u * fraction), 0u);

//			g_drawInterface->DrawLine (m_pathPoints[index - 1u], m_pathPoints[index], color, 255u, 2u, false);
			g_drawInterface->DrawLine (m_pathPoints2[index - 1u].point, m_pathPoints2[index].point, color, 255u, 2u, false);

			m_path[m_pathPoints2[index - 1u].prevNodeIndex].area->Draw ();
			m_path[m_pathPoints2[index - 1u].nextNodeIndex].area->Draw ();
			m_path[m_pathPoints2[index].prevNodeIndex].area->Draw ();
			m_path[m_pathPoints2[index].nextNodeIndex].area->Draw ();
		}
	}
	#endif	// if defined MULTIPLE_DRAW_METHODS

	const PathNode_t *nextPathNode (&m_path[m_currentPathIndex - 1u]);

	if (m_owner->m_lastNavigationArea == nextPathNode->area)
	{
		if (m_owner->m_lastNavigationArea->HasAttributes (NavigationMesh::NAV_MESH_JUMP))
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot \"%s\" should jump because of area attributes!", m_owner->GetName ().GetData ());

			Jump ();
		}

		if (m_owner->m_lastNavigationArea->HasAttributes (NavigationMesh::NAV_MESH_CROUCH))
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot \"%s\" should duck because of area attributes!", m_owner->GetName ().GetData ());

			Duck ();
		}

		if (m_currentPathIndex >= m_path.GetLastElementIndex ())
		{
			/// @todo BOT SHOULD REACH DESTINATION POINT, NOT JUST TOUCH GOAL AREA!!!!

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot \"%s\" is finished his path!", m_owner->GetName ().GetData ());

			m_path.RemoveAll ();
			m_currentPathIndex = 0u;

			return;
		}

		SetPathIndex (m_currentPathIndex + 1u);

		++nextPathNode;
	}

	for (PathNodeArray_t::IndexType_t index (m_currentPathIndex); index < m_path.GetElementNumber (); ++index)
	{
		const PathNode_t &pathNode (m_path[index]);

		if (m_owner->m_lastNavigationArea != pathNode.area)
			continue;

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot \"%s\" is skipped some path points! Changin to current one....", m_owner->GetName ().GetData ());

		SetPathIndex (index + 1u);

		nextPathNode = &m_path[index];

		goto Label_LookAtPath;
	}

	// If we get here, check maybe bot is lost his path....
	if (m_distanceToClosestPathElementCheckTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
		m_distanceToClosestPathElementCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 1.0f;

		const float currentDistance (m_owner->GetOrigin ().GetDistanceSquared2D (nextPathNode->point));

		if (currentDistance - m_previousDistanceToClosestPathElementSquared2D >= 10.0f)
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot \"%s\" is lost his path! Rebuilding....", m_owner->GetName ().GetData ());

			if (!RebuildCurrentRoute ())
			{
				m_path.RemoveAll ();
				m_currentPathIndex = 0u;

				return;
			}

			nextPathNode = &m_path[m_currentPathIndex - 1u];
		}
		else
			m_previousDistanceToClosestPathElementSquared2D = currentDistance;
	}

	Label_LookAtPath:
	{
		const Math::Vector3D pointLookAt (nextPathNode->point + Math::Vector3D (0.0f, 0.0f, (m_owner->IsDucking () ? HalfLifeEngine::SDK::Constants::HalfDuckHumanHeight : HalfLifeEngine::SDK::Constants::HalfHumanHeight) + m_owner->GetEdict ()->variables.viewOffset.z));

		m_owner->MoveTowardsPosition (pointLookAt);

		m_destinationOrigin = pointLookAt;
	}
}

void YaPB::Navigator_NavigationMesh::Think (void)
{
	if (HasRoute ())
		MoveTowardsPath ();

	BaseNavigator::Think ();

	/// @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}