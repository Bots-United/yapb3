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
// CoreToolLibrary.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_INCLUDED
#define CORE_TOOL_LIBRARY_INCLUDED

namespace CoreToolLibrary
{
	#include <Core tool library/Definitions.hpp>
	#include <Core tool library/CPU.hpp>
	#include <Core tool library/LinkedNode.hpp>
	#include <Core tool library/DoubleLinkedNode.hpp>
	#include <Core tool library/RandomNumberGenerator.hpp>
	#include <Core tool library/Math/Math.hpp>
	#include <Core tool library/Array.hpp>
	#include <Core tool library/Deque.hpp>
	#include <Core tool library/Stack.hpp>
	#include <Core tool library/String.hpp>
	#include <Core tool library/Color.hpp>
	#include <Core tool library/Exception.hpp>
	#include <Core tool library/File.hpp>
	#include <Core tool library/Stream.hpp>
	#include <Core tool library/Compress.hpp>
	#include <Core tool library/BMPFileCreator.hpp>
	#include <Core tool library/HashTable.hpp>
	#include <Core tool library/LinkedList.hpp>
	#include <Core tool library/Queue.hpp>
	#include <Core tool library/Map.hpp>
	#include <Core tool library/PriorityQueue.hpp>
	#include <Core tool library/Singleton.hpp>
	#include <Core tool library/Functions.hpp>
	#include <Core tool library/Library.hpp>
	#include <Core tool library/Log.hpp>
	#include <Core tool library/Timer.hpp>
	#include <Core tool library/CountdownTimer.hpp>
	#include <Core tool library/IntervalTimer.hpp>

	namespace Detours
	{
		#include <Core tool library/Detours/Detours definitions.hpp>
	}

	inline const float CommonTimer__GetSecondsWrapper (void) { return g_commonTimer.GetSeconds <float> (); }

	typedef CountdownTimer <CommonTimer__GetSecondsWrapper> SecondsCountdownTimer;
	typedef IntervalTimer <CommonTimer__GetSecondsWrapper> SecondsIntervalTimer;
}

#endif	// ifndef CORE_TOOL_LIBRARY_INCLUDED