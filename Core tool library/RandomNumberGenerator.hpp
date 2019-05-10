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
// RandomNumberGenerator.hpp
//
// Class: RandomNumberGenerator
//
// Description: Random number generator used by the bot code.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef RANDOM_NUMBER_GENERATOR_INCLUDED
#define RANDOM_NUMBER_GENERATOR_INCLUDED

class RandomNumberGenerator	// Encapsulate random number generator.
{
	private:
		enum Constants_t
		{
			KK = 17u,
			JJ = 10u,
			R1 = 19u,
			R2 = 27u,

			MaximumBitsNumber = sizeof (unsigned int) * 8u - 1u,

			BufferSize = KK * 2u * sizeof (unsigned int)
		};	// define parameters

		int          m_bufferIndex[2u];	// indexes into buffer

		union
		{
			long double  m_randomPointer;
			unsigned int m_randomBits[3u];
		};	// used for conversion to float

		unsigned int m_historyBuffer[KK][2u];		// history buffer

		#if defined _DEBUG
			unsigned int m_selfTestBuffer[KK * 2u][2u];	// used for self-test
		#endif	// if defined _DEBUG
#if 0
		enum Architecture_t
		{
			LITTLE_ENDIAN,
			BIG_ENDIAN,
			NON_IEEE,
			EXTENDED_PRECISION_LITTLE_ENDIAN
		}            m_architecture;	// conversion to float depends on computer architecture

	public:
		inline RandomNumberGenerator (const unsigned int seed)
		{
			Initialize (seed);

			// detect computer architecture
			m_randomBits[2u] = 0ul;
			m_randomPointer = 1.0;

			m_architecture =
				m_randomBits[2u] == 0x3FFF ? EXTENDED_PRECISION_LITTLE_ENDIAN :
				m_randomBits[1u] == 0x3FF00000 ? LITTLE_ENDIAN :
				m_randomBits[0u] == 0x3FF00000 ? BIG_ENDIAN :
				NON_IEEE;
		}
#endif	// if 0
	private:
		//
		// Function: Random
		//
		// Description: Generates random number.
		//
		inline const long double Random (void)
		{
			// returns a random number between 0.0 and 1.0.

			const unsigned int z (GetRandomBits ());	// generate 64 random bits
#if 0
			switch (m_architecture)
			{
				case EXTENDED_PRECISION_LITTLE_ENDIAN:
					// 80 bits floats = 63 bits resolution
					m_randomBits[1u] = z | 0x80000000;

					break;

				case LITTLE_ENDIAN:
					// 64 bits floats = 52 bits resolution
					m_randomBits[1u] = (z & 0x000FFFFF) | 0x3FF00000;

					break;

				case BIG_ENDIAN:
					// 64 bits floats = 52 bits resolution
					m_randomBits[0u] = (m_randomBits[0u] & 0x000FFFFF) | 0x3FF00000;

					break;

				case NON_IEEE:
				default:
#endif	// if 0
					// not a recognized floating point format. 32 bits resolution
					return z * (1.0 / (static_cast <double> (static_cast <unsigned int> (-1L)) + 1.0));
#if 0
			}

//			return m_randomPointer - 1.0;
#endif	// if 0
		}

		//
		// Function: GetRandomBits
		//
		// Description: Generates random bits for random number generator.
		//
		const unsigned int GetRandomBits (void)
		{
			// generate next number
			const unsigned int z (_lrotl (m_historyBuffer[m_bufferIndex[0u]][0u], R1) + m_historyBuffer[m_bufferIndex[1u]][0u]);
			const unsigned int y (_lrotl (m_historyBuffer[m_bufferIndex[0u]][1u], R2) + m_historyBuffer[m_bufferIndex[1u]][1u]);

			m_historyBuffer[m_bufferIndex[0u]][0u] = y;
			m_historyBuffer[m_bufferIndex[0u]][1u] = z;

			// rotate list pointers
			if (--m_bufferIndex[0u] < 0)
				m_bufferIndex[0u] = KK - 1;

			if (--m_bufferIndex[1u] < 0)
				m_bufferIndex[1u] = KK - 1;

			#if defined _DEBUG
				// perform self-test
				if (m_historyBuffer[m_bufferIndex[0u]][0u] == m_selfTestBuffer[0u][0u] && memcmp (m_historyBuffer, m_selfTestBuffer[KK - m_bufferIndex[0u]], BufferSize) == 0)
				{
					// self-test failed
					AddLogEntry (false, LogLevel_Critical, false, "Random number generator %s!", (m_bufferIndex[1u] + KK - m_bufferIndex[0u]) % KK != JJ ? "not initialized" : "returned to initial state");
				}
			#endif	// if defined _DEBUG

			m_randomBits[0u] = y;
			m_randomBits[1u] = z;

			return y;
		}

	public:
		//
		// Function: Initialize
		//
		// Description: Initializes random number generator using specified seed.
		//
		// Parameters:
		//	seed - Seed for the random generator.
		//
		inline void Initialize (unsigned int seed)
		{
			// this function initializes the random number generator.

			unsigned char index;

			// make random numbers and put them into the buffer
			for (index = 0u; index < KK; ++index)
				for (unsigned char j (0u); j < 2u; ++j)
				{
					seed *= 2891336453ul;
					seed += 1ul;

					m_historyBuffer[index][j] = seed;
				}

			// set exponent of 'm_randomPointer'
			m_randomBits[2u] = 0ul;
			m_randomPointer = 1.0;

			// initialize pointers to circular buffer
			m_bufferIndex[0u] = 0;
			m_bufferIndex[1u] = JJ;

			#if defined _DEBUG
				// store state for self-test
				memcpy (m_selfTestBuffer, m_historyBuffer, BufferSize);
				memcpy (m_selfTestBuffer[KK], m_historyBuffer, BufferSize);
/*				for (index = 0u; index < BufferSize; ++index)
				{
					m_selfTestBuffer[index] = m_historyBuffer[index];
					m_selfTestBuffer[KK][index] = m_historyBuffer[index];
				}*/
			#endif	// if defined _DEBUG

			// randomize some more
			for (index = 0u; index < MaximumBitsNumber; ++index)
				GetRandomBits ();
		}

		//
		// Function: GetValueBetween <int>
		//
		// Description: Generates random 32bit long random number between specified bounds.
		//
		// Parameters:
		//	low - Lowest number.
		//	high - Higher number.
		//
		template <typename elementType> inline const elementType GetValueBetween (const elementType low, const elementType high)
		{
			// this function returns a random integer number between (and including) the starting and
			// ending values passed by parameters low and high.
/*
			const int interval (high - low + 1);

			if (interval <= 0)
				return low;	// Error.

			const int truncate = static_cast <const int> (interval * Random ());	// truncate

			return low + (truncate >= interval ? interval - 1 : truncate);*/
			return static_cast <const elementType> (low + static_cast <const elementType> (Random () * (static_cast <const float> (high - low) + 0.9f)));
		}
/*		template </* unsigned int *//*> inline const unsigned int GetValueBetween (const unsigned int low, const unsigned int high)
		{
			// this function returns a random unsigned integer number between (and including) the starting and
			// ending values passed by parameters low and high.
/*
			if (low >= high)
				return low;	// Error.

			const unsigned int interval (high - low + 1u);

			if (interval <= 0u || low >= high)
				return low;	// Error.

			const unsigned int truncate = static_cast <const unsigned int> (interval * Random ());	// truncate

			return low + (truncate >= interval ? interval - 1u : truncate);*//*
			return static_cast <const unsigned int> (low + static_cast <const unsigned int> (Random () * (static_cast <const float> (high - low) + 0.9f)));
		}
*/
		//
		// Function: GetValueBetween <float>
		//
		// Description: Generates random 32bit float random number between specified bounds.
		//
		// Parameters:
		//	low - Lowest number.
		//	high - Higher number.
		//
		template </* float */> inline const float GetValueBetween (const float low, const float high)
		{
			// this function returns a random floating-point number between (and including) the starting
			// and ending values passed by parameters low and high.
/*
			const float interval = high - low;

			if (interval <= 0.0f)
				return low;	// Error.

			const float truncate = static_cast <const float> (interval * Random ());	// truncate

			return low + (truncate >= interval ? interval : truncate);*/
			return static_cast <const float> (low + Random () * (high - low));
		}
		template <typename elementType> inline const elementType GetValueBetween (void);
		template </* char */> inline const char GetValueBetween (void)
		{
			return GetValueBetween <char> (static_cast <char> (static_cast <unsigned char> (-1) / 2u + 1u), static_cast <char> (static_cast <unsigned char> (-1) / 2u));
		}
		template </* unsigned char */> inline const unsigned char GetValueBetween (void)
		{
			return GetValueBetween <unsigned char> (0u, static_cast <unsigned char> (-1));
		}
		template </* short */> inline const short GetValueBetween (void)
		{
			return GetValueBetween <short> (static_cast <short> (static_cast <unsigned short> (-1) / 2u + 1u), static_cast <short> (static_cast <unsigned short> (-1) / 2u));
		}
		template </* unsigned short */> inline const unsigned short GetValueBetween (void)
		{
			return GetValueBetween <unsigned short> (0u, static_cast <unsigned short> (-1));
		}
		template </* int */> inline const int GetValueBetween (void)
		{
			return GetValueBetween <int> (static_cast <int> (static_cast <unsigned int> (-1) / 2u + 1u), static_cast <int> (static_cast <unsigned int> (-1) / 2u));
		}
		template </* unsigned int */> inline const unsigned int GetValueBetween (void)
		{
			return GetValueBetween <unsigned int> (0u, static_cast <unsigned int> (-1));
		}
		template </* bool */> inline const bool GetValueBetween (void)
		{
			// this function returns a random boolean value (false or true).

			return GetValueBetween <unsigned char> (false, true) == TRUE;
		}
};

// Mersenne Twister random number generator
class MTRand_int32
{
	private:
		static const int n = 624, m = 397;	// compile time constants
	// the variables below are static (no duplicates can exist)
		static unsigned int state[n];	// state vector array
		static int p;	// position in state array
		static bool init;	// true if init function is called

	private:
	// private functions used to generate the pseudo random numbers
		inline const unsigned int twiddle (const unsigned int u, const unsigned int v) const	// used by gen_state()
		{
			// inline for speed, must therefore reside in header file
			return (((u & 0x80000000UL) | (v & 0x7FFFFFFFUL)) >> 1) ^ ((v & 1UL) ? 0x9908B0DFUL : 0x0UL);
		}
		void gen_state (void)	// generate new state
		{	// generate new state vector
			for (int i = 0; i < n - m; ++i)
				state[i] = state[i + m] ^ twiddle (state[i], state[i + 1]);

			for (int i = n - m; i <= n; ++i)
				state[i] = state[i + m - n] ^ twiddle (state[i], state[i + 1]);

			state[n - 1] = state[m - 1] ^ twiddle (state[n - 1], state[0]);
			p = 0;	// reset position
		}
	// make copy constructor and assignment operator unavailable, they don't make sense
		MTRand_int32 (const MTRand_int32 &);	// copy constructor not defined
		void operator = (const MTRand_int32 &);	// assignment operator not defined

	public:
	// default constructor: uses default seed only if this is the first instance
		inline MTRand_int32 (void) { if (!init) seed (5489UL); init = true; }
	// constructor with 32 bit int as seed
		inline MTRand_int32 (unsigned int s) { seed (s); init = true; }
	// constructor with array of size 32 bit ints as seed
		inline MTRand_int32 (const unsigned int *seedArray, int size) { seed (seedArray, size); init = true; }
	// 2007-02-11: made the destructor virtual; thanks "double more" for pointing this out
		virtual inline ~MTRand_int32 (void) { /*VOID*/ }	// destructor

	public:
		// the two seed functions
		void seed (unsigned int s)	// seed with 32 bit integer
		{	// init by 32 bit seed
			state[0] = s & 0xFFFFFFFFUL;	// for > 32 bit machines

			for (int i = 1; i < n; ++i)
			{
				state[i] = 1812433253UL * (state[i - 1] ^ (state[i - 1] >> 30)) + i;
				// see Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier
				// in the previous versions, MSBs of the seed affect only MSBs of the array state
				// 2002/01/09 modified by Makoto Matsumoto
				state[i] &= 0xFFFFFFFFUL;	// for > 32 bit machines
			}
			p = n;	// force gen_state() to be called for next random number
		}
		void seed (const unsigned int *array, int size)	// seed with array
		{	// init by array
			int k, i = 1, j = 0;

			seed (19650218UL);

			for (k = n > size ? n : size; k > 0; --k)
			{
				state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> 30)) * 1664525UL)) + array[j] + j;	// non linear
				state[i] &= 0xFFFFFFFFUL;	// for > 32 bit machines
				++j;
				j %= size;

				if (++i == n)
				{
					state[0] = state[n - 1];
					i = 1;
				}
			}

			for (k = n - 1; k > 0; --k)
			{
				state[i] = (state[i] ^ ((state[i - 1] ^ (state[i - 1] >> 30)) * 1566083941UL)) - i;
				state[i] &= 0xFFFFFFFFUL;	// for > 32 bit machines

				if (++i == n)
				{
					state[0] = state[n - 1];
					i = 1;
				}
			}

			state[0] = 0x80000000UL;	// MSB is 1; assuring non-zero initial array
			p = n;	// force gen_state() to be called for next random number
		}
	// overload operator() to make this a generator (functor)
		inline const unsigned int operator () () { return rand_int32 (); }

	protected:	// used by derived classes, otherwise not accessible; use the ()-operator
		unsigned int rand_int32 (void)	// generate 32 bit random integer
		{	// generate 32 bit random int
			if (p == n)
				gen_state ();	// new state vector needed
		// gen_state() is split off to be non-inline, because it is only called once
		// in every 624 calls and otherwise irand() would become too big to get inlined
			unsigned int x = state[p++];

			x ^= (x >> 11);
			x ^= (x << 7) & 0x9D2C5680UL;
			x ^= (x << 15) & 0xEFC60000UL;

			return x ^ (x >> 18);
		}
};

// generates double floating point numbers in the half-open interval [0, 1)
class MTRand : public MTRand_int32
{
	public:
		inline MTRand (void) : MTRand_int32 () { /*VOID*/ }
		inline MTRand (unsigned int seed) : MTRand_int32 (seed) { /*VOID*/ }
		inline MTRand (const unsigned int *seedArray, int size) : MTRand_int32 (seedArray, size) { /*VOID*/ }
		inline ~MTRand (void) { /*VOID*/ }

	public:
		inline const double operator () ()
		{
			return static_cast <double> (rand_int32 ()) * (1. / 4294967296.);	// divided by 2^32
		}

	private:
		MTRand (const MTRand &);	// copy constructor not defined
		void operator = (const MTRand &);	// assignment operator not defined
};

// generates double floating point numbers in the closed interval [0, 1]
class MTRand_closed : public MTRand_int32
{
	public:
		inline MTRand_closed (void) : MTRand_int32 () { /*VOID*/ }
		inline MTRand_closed (unsigned int seed) : MTRand_int32 (seed) { /*VOID*/ }
		inline MTRand_closed (const unsigned int *seedArray, int size) : MTRand_int32 (seedArray, size) { /*VOID*/ }
		inline ~MTRand_closed (void) { /*VOID*/ }

	public:
		inline const double operator () ()
		{
			return static_cast <double> (rand_int32 ()) * (1. / 4294967295.);	// divided by 2^32 - 1
		}

	private:
		MTRand_closed (const MTRand_closed &);	// copy constructor not defined
		void operator = (const MTRand_closed &);	// assignment operator not defined
};

// generates double floating point numbers in the open interval (0, 1)
class MTRand_open : public MTRand_int32
{
	public:
		inline MTRand_open (void) : MTRand_int32 () { /*VOID*/ }
		inline MTRand_open (unsigned int seed) : MTRand_int32 (seed) { /*VOID*/ }
		inline MTRand_open (const unsigned int *seedArray, int size) : MTRand_int32 (seedArray, size) { /*VOID*/ }
		inline ~MTRand_open (void) { /*VOID*/ }

	public:
		inline const double operator () ()
		{
			return (static_cast <double> (rand_int32 ()) + .5) * (1. / 4294967296.);	// divided by 2^32
		}

	private:
		MTRand_open (const MTRand_open &);	// copy constructor not defined
		void operator = (const MTRand_open &);	// assignment operator not defined
};

// generates 53 bit resolution doubles in the half-open interval [0, 1)
class MTRand53 : public MTRand_int32
{
	public:
		inline MTRand53 (void) : MTRand_int32 () { /*VOID*/ }
		inline MTRand53 (unsigned int seed) : MTRand_int32 (seed) { /*VOID*/ }
		inline MTRand53 (const unsigned int *seedArray, int size) : MTRand_int32 (seedArray, size) { /*VOID*/ }
		inline ~MTRand53 (void) { /*VOID*/ }

	public:
		inline const double operator () ()
		{
			return (static_cast <double> (rand_int32 () >> 5) * 67108864. + static_cast <double> (rand_int32 () >> 6)) * (1. / 9007199254740992.);
		}

	private:
		MTRand53 (const MTRand53 &);	// copy constructor not defined
		void operator = (const MTRand53 &);	// assignment operator not defined
};

extern NAMESPACE_GLOBAL RandomNumberGenerator g_randomNumberGenerator;

#endif	// ifndef RANDOM_NUMBER_GENERATOR_INCLUDED