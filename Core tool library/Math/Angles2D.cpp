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
// Angles2D.hpp
//
// Class: Angles2D
//
// Version: $ID:$
//

#include <Core.hpp>

namespace CoreToolLibrary
{
	namespace Math
	{
		CLASS_GLOBAL const Angles2D Angles2D::ZeroValue (0.0f, 0.0f);	// Initialize zero vector value.

		/*inline */void Angles2D::MakeVectors (void) const
		{
			BuildVectors (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward, HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right, HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->upward);
		}

		/*inline */void Angles2D::MakeForwardVector (void) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward = BuildForwardVector ();
		}
		/*inline */void Angles2D::MakeRightVector (void) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right = BuildRightVector ();
		}
		/*inline */void Angles2D::MakeUpwardVector (void) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->upward = BuildUpwardVector ();
		}
	}
}