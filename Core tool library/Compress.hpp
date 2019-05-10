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
// Compress.hpp
//
// Class: Compressor
//
// POD Data Compress Routines based on LZSS
//
// Actually I ripped this out of the AceBot Source and modified it to suit my needs.
// This is what the original (?) Author writes:
//
// Original file is Copyright ©, Steve Yeager 1998
//
// Not sure where I got this code, but thanks go to the author.
// I just rewote it to allow the use of buffers instead of files.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef COMPRESS_INCLUDED
#define COMPRESS_INCLUDED

class Compressor
{
	//
	// Group: Private members.
	//
	private:
		enum Constant_t
		{
			THRESHOLD = 2u,		// encode string into position and length if m_matchLength is greater than this index for root of binary search trees
			F         = 18u,	// upper limit for m_matchLength
			N         = 4096u,	// size of ring buffer
			NIL       = N
		};

		unsigned int  m_textSize;	// text size counter
		unsigned int  m_codeSize;	// code size counter

		unsigned char m_textBuffer[N + F - 1u];	// ring buffer of size N, with extra F-1 bytes to facilitate string comparison of longest match. Set by the InsertNode() procedure
		int           m_matchPosition;
		int           m_matchLength;

		// left & right children & parents -- these constitute binary search trees
		int           m_left[N + 1u];
		int           m_right[N + 257u];
		int           m_parent[N + 1u];

	//
	// Group: Private functions.
	//
	private:
		void InsertNode (const int node)
		{
			// Inserts string of length F, m_textBuffer[node..node + F - 1], into one of the trees (m_textBuffer[node]'th tree)
			// and returns the longest-match position and length via the global variables 'm_matchPosition' and 'm_matchLength'.
			// If m_matchLength = F, then removes the old node in favor of the new one, because the old one will be deleted sooner.
			// Note 'node' plays double role, as tree node and position in buffer.

			const unsigned char *const key = m_textBuffer + node;
			int index, compare = 1, temp = N + 1 + key[0];

			m_left[node] = m_right[node] = NIL;
			m_matchLength = 0;

			for (/* Empty */; /* Empty */; /* Empty */)
			{
				if (compare >= 0)
				{
					if (m_right[temp] != NIL)
						temp = m_right[temp];
					else
					{
						m_right[temp] = node;
						m_parent[node] = temp;

						return;
					}
				}
				else
				{
					if (m_left[temp] != NIL)
						temp = m_left[temp];
					else
					{
						m_left[temp] = node;
						m_parent[node] = temp;

						return;
					}
				}

				for (index = 1; index < F; ++index)
				{
					compare = key[index] - m_textBuffer[temp + index];

					if (compare != 0)
						break;
				}

				if (index > m_matchLength)
				{
					m_matchPosition = temp;
					m_matchLength = index;

					if (m_matchLength >= F)
						break;
				}
			}

			m_parent[node] = m_parent[temp];
			m_left[node] = m_left[temp];
			m_right[node] = m_right[temp];
			m_parent[m_left[temp]] = m_parent[m_right[temp]] = node;

			if (m_right[m_parent[temp]] == temp)
				m_right[m_parent[temp]] = node;
			else
				m_left[m_parent[temp]] = node;

			m_parent[temp] = NIL;
		}

		void DeleteNode (const int node)
		{
			// deletes node 'node' from tree

			if (m_parent[node] == NIL)
				return;	// not in tree

			int temp;

			if (m_right[node] == NIL)
				temp = m_left[node];
			else if (m_left[node] == NIL)
				temp = m_right[node];
			else
			{
				temp = m_left[node];

				if (m_right[temp] != NIL)
				{
					do
						temp = m_right[temp];
					while (m_right[temp] != NIL);

					m_parent[m_left[temp]] = m_parent[temp];
					m_left[temp] = m_left[node];
					m_right[m_parent[temp]] = m_left[temp];
					m_parent[m_left[node]] = temp;
				}

				m_right[temp] = m_right[node];
				m_parent[m_right[node]] = temp;
			}

			m_parent[temp] = m_parent[node];

			if (m_right[m_parent[node]] == node)
				m_right[m_parent[node]] = temp;
			else
				m_left[m_parent[node]] = temp;

			m_parent[node] = NIL;
		}

		const int InternalEncode (const DynamicString &filename, const unsigned char *const header, const unsigned char headerSize, const unsigned char *const buffer, const unsigned int bufferSize)
		{
			STDIOFile file (filename, "wb");

			if (!file.IsValid ())
				return -1;	// bail

			int index, length, node, stringPointer, lastMatchLength, codeBufferPointer;
			unsigned int bufferPointer (0u);
			unsigned char codeBuffer[17u], bit, mask;

			// write header first
			file.Write (header, headerSize);

			// initialize trees

			// For index = 0 to N - 1, m_right[index] and m_left[index] will be the right and left children of node 'index'.
			// These nodes need not be initialized. Also, m_parent[index] is the parent of node 'index'.
			// These are initialized to NIL  (= N), which stands for 'not used'.
			// For index = 0 to 255, m_right[N + index + 1] is the root of the tree for strings that begin with character 'index'.
			// These are initialized to NIL. Note there are 256 trees.

			for (index = N + 1; index <= N + 256u; ++index)
				m_right[index] = NIL;

			for (index = 0; index < N; ++index)
				m_parent[index] = NIL;

			// ...end of the initialize trees

			// codeBuffer[1..16] saves eight units of code, and codeBuffer[0] works as eight flags, "1" representing that the unit is an unencoded letter (1 byte), "0" a position-and-length pair (2 bytes).
			// Thus, eight units require at most 16 bytes of code.
			codeBuffer[0] = 0;
			codeBufferPointer = mask = 1;
			stringPointer = 0;
			node = N - F;

			for (index = stringPointer; index < node; ++index)
				m_textBuffer[index] = ' ';	// clear the buffer with any character that will appear often

			for (length = 0; length < F && bufferPointer < bufferSize; ++length)
			{
				bit = buffer[bufferPointer++];

				m_textBuffer[node + length] = bit;	// read F bytes into the last F bytes of the buffer
			}

			m_textSize = length;

			if (length == 0)
				return -1;	// text of size zero

			// Insert the F strings, each of which begins with one or more 'space' characters.
			// Note the order in which these strings are inserted.
			// This way, degenerate trees will be less likely to occur.
			for (index = 1; index <= F; ++index)
				InsertNode (node - index);

			// Finally, insert the whole string just read. The global variables m_matchLength and 'm_matchPosition' are set.
			InsertNode (node);

			// do until length of string to be processed is zero
			do
			{
				if (m_matchLength > length)
					m_matchLength = length;	// m_matchLength may be spuriously long near the end of text

				if (m_matchLength <= THRESHOLD)
				{
					m_matchLength = 1;	// not long enough match. Send one byte.
					codeBuffer[0] |= mask;	// 'send one byte' flag
					codeBuffer[codeBufferPointer++] = m_textBuffer[node];	// send uncoded
				}
				else
				{
					// send position and length pair. Note m_matchLength > THRESHOLD.
					codeBuffer[codeBufferPointer++] = static_cast <unsigned char> (m_matchPosition);
					codeBuffer[codeBufferPointer++] = static_cast <unsigned char> (((m_matchPosition >> 4) & 0xf0) | (m_matchLength - (THRESHOLD + 1)));
				}

				if ((mask <<= 1) == 0)
				{
					// shift mask left one bit
					for (index = 0; index < codeBufferPointer; ++index)
						file.PutCharacter (codeBuffer[index]);	// send at most 8 units of code together

					m_codeSize += codeBufferPointer;
					codeBuffer[0] = 0;
					codeBufferPointer = mask = 1;
				}

				lastMatchLength = m_matchLength;

				for (index = 0; index < lastMatchLength && bufferPointer < bufferSize; ++index)
				{
					bit = buffer[bufferPointer++];

					// delete old strings and
					DeleteNode (stringPointer);

					// ...read new bytes
					m_textBuffer[stringPointer] = bit;

					// if the position is near the end of buffer, extend the buffer to make string comparison easier
					if (stringPointer < F - 1)
						m_textBuffer[stringPointer + N] = bit;

					// since this is a ring buffer, increment the position modulo N
					stringPointer = (stringPointer + 1) & (N - 1);
					node = (node + 1) & (N - 1);

					// register the string in m_textBuffer[node..node + F - 1]
					InsertNode (node);
				}

				while (index++ < lastMatchLength)
				{
					// after the end of text, no need to read, but buffer may not be empty
					DeleteNode (stringPointer);

					stringPointer = (stringPointer + 1) & (N - 1);
					node = (node + 1) & (N - 1);

					if (length-- > 0)
						InsertNode (node);
				}
			} while (length > 0);

			if (codeBufferPointer > 1)
			{
				// send remaining code
				for (index = 0; index < codeBufferPointer; ++index)
					file.PutCharacter (codeBuffer[index]);

				m_codeSize += codeBufferPointer;
			}

			return m_codeSize;
		}

		const int InternalDecode (const DynamicString &filename, const unsigned char headerSize, unsigned char *const buffer, const unsigned int bufferSize)
		{
			// be careful with your bufferSize, will return an exit of -1 if failure

			STDIOFile file (filename, "rb");

			if (!file.IsValid ())
				return -1;	// bail

			int index, j, k, node;
			unsigned char bit;
			unsigned int flags = 0u, bufferPointer = 0u;

			// skip header
			file.Seek (headerSize, BaseFile::SeekType_Start);

			node = N - F;

			for (index = 0; index < node; ++index)
				m_textBuffer[index] = ' ';

			for (/* Empty */; /* Empty */; /* Empty */)
			{
				if (((flags >>= 1) & 256) == 0)
				{
					bit = static_cast <const unsigned char> (file.GetCharacter ());

					if (bit == EOF)
						break;

					// uses higher byte cleverly
					flags = bit | 0xff00;
				}

				// to count eight
				if (flags & 1)
				{
					bit = static_cast <const unsigned char> (file.GetCharacter ());

					if (bit == EOF)
						break;

					buffer[bufferPointer++] = bit;

					// check for overflow
					if (bufferPointer > bufferSize)
						return -1;

					m_textBuffer[node++] = bit;

					node &= (N - 1);
				}
				else
				{
					index = file.GetCharacter ();
					j = file.GetCharacter ();

					if (index == EOF || j == EOF)
						break;

					index |= ((j & 0xf0) << 4);
					j = (j & 0x0f) + THRESHOLD;

					for (k = 0; k <= j; ++k)
					{
						bit = m_textBuffer[(index + k) & (N - 1)];
						buffer[bufferPointer++] = bit;

						// check for overflow
						if (bufferPointer > bufferSize)
							return -1;

						m_textBuffer[node++] = bit;
						node &= (N - 1);
					}
				}
			}

			// return uncompressed size
			return bufferPointer;
		}

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Compressor (void) : m_textSize (0ul), m_codeSize (0ul) { /*VOID*/ }

	//
	// Group: Functions.
	//
	public:
		static inline const int Compress (const DynamicString &filename, const unsigned char *const header, const unsigned char headerSize, const unsigned char *const buffer, const unsigned int bufferSize)
		{
			// External encoder.

			Compressor compress;

			return compress.InternalEncode (filename, header, headerSize, buffer, bufferSize);
		}

		static inline const int Uncompress (const DynamicString &filename, const unsigned char headerSize, unsigned char *const buffer, const unsigned int bufferSize)
		{
			// External decoder.

			Compressor decompress;

			return decompress.InternalDecode (filename, headerSize, buffer, bufferSize);
		}
};

#endif	// ifndef COMPRESS_INCLUDED