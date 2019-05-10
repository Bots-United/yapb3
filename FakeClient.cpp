//
// Copyright © 2003-2012, by YaPB Development Team. All rights reserved.
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
// FakeClient.cpp
//
// Class: FakeClient
//
// Description: Contains the 'FakeClient' class Function Prototypes.
//
// Version: $ID:$
//

#include <Core.hpp>

/*virtual */void FakeClient::Think (void)
{
	// if fake client bit were removed by some evil action..., so we will set it back :)
	m_edict->variables.flags |= HalfLifeEngine::SDK::Constants::FL_FAKECLIENT;	// restore it
/*
	GetEdict ()->privateData->GetBasePlayerPointer ()->random_seed = g_randomNumberGenerator.GetValueBetween (0u, 0x7FFFFFFFu);	// full range

	R_AnimateLight ();
	UpdateAngles ();	// update the view and model angles....
*/
	// Call original function in base class....
	Client::Think ();
}

/*virtual */void FakeClient::AliveThink (void)
{
	GetEdict ()->privateData->GetBasePlayerPointer ()->random_seed = g_randomNumberGenerator.GetValueBetween (0u, 0x7FFFFFFFu);	// full range
	GetEdict ()->variables.lightLevel = HalfLifeEngine::Utilities::Light::R_LightPoint (g_server->GetFakeClientManager ()->GetLightStyleValue (), g_server->GetServer ().worldmodel, GetOrigin ());

//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot \"%s\" lightLevel=%u, host lightLevel=%u.\n", GetName ().GetData (), GetIllumination (), g_server->GetHumanManager ()->GetHostClient ()->GetIllumination ());

	// update the view and model angles....
	UpdateAngles ();

	// Call original function in base class....
	Client::AliveThink ();
}