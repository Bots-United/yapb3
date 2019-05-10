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
// BMPFileCreator.hpp
//
// Class: BMPFileCreator
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_BMP_FILE_CREATOR_INCLUDED
#define CORE_TOOL_LIBRARY_BMP_FILE_CREATOR_INCLUDED

class BMPFileCreator
{
	//
	// Group: Constants.
	//
	public:
		enum Color_t
		{
			Color_Black,		// black
			Color_White,		// white
			Color_WhiteGrey,	// white - grey
			Color_Grey,			// grey
			Color_DarkGrey,		// dark grey
			Color_Silver,		// silver
			Color_DarkRed,		// dark red
			Color_Red,			// red
			Color_DarkYellow,	// dark yellow
			Color_Yellow,		// yellow
			Color_DarkGreen,	// dark green
			Color_Green,		// green
			Color_DarkBlue,		// dark blue
			Color_Blue,			// blue
			Color_DarkPurple,	// dark purple
			Color_Purple,		// purple
			Color_Pink,			// pink

			Color_Total
		};

	//
	// Group: Private members.
	//
	private:
		// width and height of the debug bitmap image
		const unsigned short m_width;
		const unsigned short m_height;

		unsigned char *const m_buffer;

		const Math::Extent2D m_extent;

		const Math::Vector2D m_scale;
		const float          m_scaleOffset;

		Color_t              m_backgroundColor;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline BMPFileCreator (const Math::Extent2D &extent, const Color_t backgroundColor = Color_Black/* set all to all black (and allow for lighter palette) */, const unsigned short width = 2000u, const unsigned short height = 2000u) :
			m_width (width),
			m_height (height),

			m_buffer (new unsigned char[width * height]),	// allocate memory

			m_extent (extent),

			// compute the X and Y divider scale, and take the greatest of both
			m_scale (Math::Vector2D (extent.GetSizeX () / width, extent.GetSizeY () / height)),
//			m_scale (Math::Vector2D ((1.0f + extent.GetSizeX ()) / width, (1.0f + extent.GetSizeY ()) / height)),

			// add a little offset (margin) for safety
			m_scaleOffset (m_scale.x > m_scale.y ? m_scale.x / 100.0f + m_scale.x : m_scale.y / 100.0f + m_scale.y),

			m_backgroundColor (backgroundColor)
		{
			// Reliability check.
			if (!IsValid ())
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes for BMP buffer!", width * height * sizeof (unsigned char) / 1024.0f);

				return;
			}

			// zero all the crap out
			Clear ();
		}

		inline ~BMPFileCreator (void) { delete [] m_buffer; }

	//
	// Group: Private operators.
	//
	private:
		inline BMPFileCreator &operator = (const BMPFileCreator &/*right*/);	// Avoid "warning C4512: 'BMPFileCreator' : assignment operator could not be generated".

	//
	// Group: Functions.
	//
	public:
		inline const bool                  IsValid        (void) const { return m_buffer != NULL; }
		inline const unsigned short        GetWidth       (void) const { return m_width; }
		inline const unsigned short        GetHeight      (void) const { return m_height; }
		inline const unsigned char *const  GetBuffer      (void) const { return m_buffer; }
		inline const Math::Extent2D       &GetExtent      (void) const { return m_extent; }
		inline const Math::Vector2D       &GetScale       (void) const { return m_scale; }
		inline const float                 GetScaleOffset (void) const { return m_scaleOffset; }

		inline void Clear (void)
		{
			// zero all the crap out
			memset (m_buffer, m_backgroundColor, m_width * m_height * sizeof (unsigned char));
		}
		inline void Clear (const Color_t backgroundColor)
		{
			m_backgroundColor = backgroundColor;

			// zero all the crap out
			Clear ();
		}

		inline void DrawLine (const Math::Vector2D &source, const Math::Vector2D &destination, const Color_t color = Color_White/* set color for the point (white) */)
		{
			// This function prints a vector line into a bitmap dot matrix. The dot matrix (m_buffer) is a global array.
			// The size of the bitmap is always assumed to be m_width * m_height pixels
			// (currently m_width * m_height to fit with the size of the universe, with an adaptative unit scale, up to 1 pixel = 10 vector units).

			// Reliability check: cancel if BMP buffer unallocated.
			if (!IsValid ())
			{
				AddLogEntry (true, LogLevel_Error, false, "BMPFileCreator::Write(): Function called with NULL BMP buffer!");

				return;
			}

			// translate the world coordinates in image pixel coordinates
			int x0 (static_cast <int> ((source.x - m_extent.mins.x) / m_scaleOffset));
			int x1 (static_cast <int> ((destination.x - m_extent.mins.x) / m_scaleOffset));
			int y0 (static_cast <int> ((source.y - m_extent.mins.y) / m_scaleOffset));
			int y1 (static_cast <int> ((destination.y - m_extent.mins.y) / m_scaleOffset));

			int dx ((x1 - x0) * 2);
			int dy ((y1 - y0) * 2);

			int offset, fraction;
			char stepX, stepY;

			if (dx < 0)
			{
				dx = -dx;
				stepX = -1;
			}
			else
				stepX = 1;

			if (dy < 0)
			{
				dy = -dy;
				stepY = -1;
			}
			else
				stepY = 1;

			offset = y0 * m_width + x0;

			if (offset < 0 || offset >= m_width * m_height)
			{
				AddLogEntry (true, LogLevel_Error, false, "BMPFileCreator::DrawLine(): Bad BMP buffer index %i (range 0 - %u).", offset, m_width * m_height);

				return;
			}

			// draw the first point of the line
			m_buffer[offset] = static_cast <const unsigned char> (color);

			// is the line rather horizontal than vertical? We need to know this to determine the step advance in the Bresenham grid, either we draw y = f(x), or x = f(y)
			if (dx > dy)
			{
				// the line is rather horizontal, we can draw it safely for incremental values of x

				// fraction of height in x0 pixel's 'square' where y0 should be
				fraction = 2 * dy - dx;

				// while we've not reached the end of the segment...
				while (x0 != x1)
				{
					// if y0 should rather be drawn on a different height than its previous height...
					if (fraction >= 0)
					{
						y0 += stepY;	// draw it one pixel aside, then (depending on line orientation)
						fraction -= 2 * dx;	// and reset its fraction (Bresenham, not sure I get the math)
					}

					x0 += stepX;	// in either case, draw x0 one pixel aside its previous position
					fraction += 2 * dy;	// and update y0's fraction (not sure I get the math - but whatever)

					// compute the offset in the BMP buffer corresponding to this point
					offset = y0 * m_width + x0;

					if (offset < 0 || offset >= m_width * m_height)
					{
						AddLogEntry (true, LogLevel_Error, false, "BMPFileCreator::DrawLine(): Bad BMP buffer index %i (range 0 - %u).", offset, m_width * m_height);

						return;
					}

					// set this point to have the specified color
					m_buffer[offset] = static_cast <const unsigned char> (color);
				}
			}
			else	// else the line is rather vertical, we NEED to draw it for incremental values of y (if we did it for incremental values of x instead, we would drop half the pixels)
			{
				// fraction of width in y0 pixel's 'square' where x0 should be
				fraction = 2 * dx - dy;

				// while we've not reached the end of the segment...
				while (y0 != y1)
				{
					// if x0 should rather be drawn on a different width than its previous width...
					if (fraction >= 0)
					{
						x0 += stepX;	// draw it one pixel aside, then (depending on line orientation)
						fraction -= 2 * dy;	// and reset its fraction (Bresenham, not sure I get the math)
					}

					y0 += stepY;	// in either case, draw y0 one pixel aside its previous position
					fraction += 2 * dx;	// and update x0's fraction (not sure I get the math - but whatever)

					// compute the offset in the BMP buffer corresponding to this point
					offset = y0 * m_width + x0;

					if (offset < 0 || offset >= m_width * m_height)
					{
						AddLogEntry (true, LogLevel_Error, false, "BMPFileCreator::DrawLine(): Bad BMP buffer index %i (range 0 - %u).", offset, m_width * m_height);

						return;
					}

					// set this point to have the specified color
					m_buffer[offset] = static_cast <const unsigned char> (color);
				}
			}

			// finished, segment has been printed into the BMP dot matrix
		}
		inline void DrawPoint (const Math::Vector2D &point, const Color_t color = Color_White/* set color for the point (white) */)
		{
			// Draw a small cross

			// Reliability check: cancel if BMP buffer unallocated.
			if (!IsValid ())
			{
				AddLogEntry (true, LogLevel_Error, false, "BMPFileCreator::DrawPoint(): Function called with NULL BMP buffer!");

				return;
			}

			// translate the world coordinates in image pixel coordinates
			const int x0 (static_cast <int> ((point.x - m_extent.mins.x) / m_scaleOffset));
			const int y0 (static_cast <int> ((point.y - m_extent.mins.y) / m_scaleOffset));

			const int offset (y0 * m_width + x0);

			if (offset < 0 || offset >= m_width * m_height)
			{
				AddLogEntry (true, LogLevel_Error, false, "BMPFileCreator::DrawPoint(): Bad BMP buffer index %i (range 0 - %i).", offset, m_width * m_height);

				return;
			}

			m_buffer[offset] = static_cast <const unsigned char> (color);  // draw the point itself

			if (offset + 1 < m_width * m_height)
				m_buffer[offset + 1] = static_cast <const unsigned char> (color);	// make a small star on the right

			if (offset - 1 >= 0)
				m_buffer[offset - 1] = static_cast <const unsigned char> (color);	// make a small star on the left

			if (offset + m_width < m_width * m_height)
				m_buffer[offset + m_width] = static_cast <const unsigned char> (color);	  // make a small star below

			if (offset - m_width >= 0)
				m_buffer[offset - m_width] = static_cast <const unsigned char> (color);	  // make a small star above
		}
		inline void MarkAxis (void)
		{
			// Mark meridians as slighly darker in alternance
			// Palette is defined such that increasing the palette index
			// Makes a slightly darker dark

			unsigned short x, y;
			int x0, y0;

			x0 = static_cast <int> ((0.0f - m_extent.mins.x) / m_scaleOffset);
			y0 = static_cast <int> ((0.0f - m_extent.mins.y) / m_scaleOffset);

			// Mark X axis by keeping X to 0 and varying Y
			if (m_extent.mins.x < 0.0f && m_extent.maxs.x > 0.0f)
				for (y = 0u; y < m_height; ++y)
					m_buffer[y * m_width + x0] += 2u;

			// Mark Y axis by keeping Y to 0 and varying X
			if (m_extent.mins.y < 0.0f && m_extent.maxs.y > 0.0f)
				for (x = 0u; x < m_width; ++x)
					m_buffer[y0 * m_width + x] += 2u;
		}
		inline void MarkMeredians (const unsigned short meredianSize = 256u)
		{
			// Mark each meredians (default 256 units) of alternating color

			unsigned short x, y;
			bool isMeredian;

			// Mark some meredians
			for (x = 0u; x < m_width; ++x)
			{
				isMeredian = static_cast <int> (static_cast <int> (x * m_scaleOffset + m_extent.mins.x + 8192.0f) / static_cast <float> (meredianSize)) & 0x01;

				if (isMeredian)
					for (y = 0u; y < m_height; ++y)
						++m_buffer[y * m_width + x];
			}

			// Mark some meredians
			for (y = 0u; y < m_height; ++y)
			{
				isMeredian = static_cast <int> (static_cast <int> (y * m_scaleOffset + m_extent.mins.y + 8192.0f) / static_cast <float> (meredianSize)) & 0x01;

				if (isMeredian)
					for (x = 0u; x < m_height; ++x)
						++m_buffer[y * m_width + x];
			}
		}

		inline const bool Write (const DynamicString &filename) const
		{
			// This function writes the debug bitmap image buffer in a .BMP file to disk.
			// The format is 256 color and m_width * m_height pixels.
			// The center of the world being roughly the center of the bitg_map.
			// The bitmap is stored in the file specified by 'filename' (which can be a relative path from the YaPB base directory).

			// Reliability check: cancel if BMP buffer unallocated.
			if (!IsValid ())
			{
				AddLogEntry (true, LogLevel_Error, false, "BMPFileCreator::Write(): Function called with NULL BMP buffer!");

				return false;
			}

			// open/create the BMP file for writing in binary mode...
			STDIOFile file (filename, "wb");

			// Reliability check.
			if (!file.IsValid ())
			{
				AddLogEntry (true, LogLevel_Error, false, "BMPFileCreator::Write(): Unable to open/create BMP file '%s'!", filename.GetData ());

				// Cancel if error creating file....
				return false;
			}

			return WriteInternal (file);
		}
		inline const bool Write (BaseFile &file) const
		{
			// This function writes the debug bitmap image buffer in a .BMP file to disk.
			// The format is 256 color and m_width * m_height pixels.
			// The center of the world being roughly the center of the bitg_map.
			// The bitmap is stored in the file specified by 'file'.

			// Reliability check: cancel if BMP buffer unallocated.
			if (!IsValid ())
			{
				AddLogEntry (true, LogLevel_Error, false, "BMPFileCreator::Write(): Function called with NULL BMP buffer!");

				return false;
			}

			// Reliability check.
			if (!file.IsValid ())
			{
				AddLogEntry (true, LogLevel_Error, false, "BMPFileCreator::Write(): Unable to open/create BMP file '%s'!", file.GetPathName ().GetData ());

				// Cancel if invalid file....
				return false;
			}

			return WriteInternal (file);
		}

	private:
		inline const bool WriteInternal (BaseFile &file) const
		{
			const BITMAPINFOHEADER BMPInfoHeader =
			{
				sizeof (BITMAPINFOHEADER),	// Size (write the info header size (does 40 bytes))
				m_width,					// Width (write the image width (2000 pixels))
				m_height,					// Height (write the image height (2000 pixels))
				1u,							// Planes (write the # of planes (1))
				8u,							// Bit count (write the bit count (8))
				BI_RGB,						// Compression (write the compression id (no compression))
				m_width * m_height,			// Size image (write the image size (m_width * m_height))
				0u,							// X pels per meter (write the X pixels per meter (not specified))
				0u,							// Y pels per meter (write the Y pixels per meter (not specified))
				256u,						// Color used (write the # of colors used (all))
				256u						// Color important (write the # of important colors (wtf?))
			};
			const RGBQUAD palette[256u] =
			{
				// write the color palette (R, G, B, reserved byte)
				{0u,     0u,   0u, 0u},	// Color_Black
				{255u, 255u, 255u, 0u},	// Color_White
				{223u, 223u, 223u, 0u},	// Color_WhiteGrey
				{175u, 175u, 175u, 0u},	// Color_Grey
				{128u, 128u, 128u, 0u},	// Color_DarkGrey
				{192u, 192u, 192u, 0u},	// Color_Silver
				{128u,   0u,   0u, 0u},	// Color_DarkRed
				{255u,   0u,   0u, 0u},	// Color_Red
				{128u, 128u,   0u, 0u},	// Color_DarkYellow
				{255u, 255u,   0u, 0u},	// Color_Yellow
				{0u,   128u,   0u, 0u},	// Color_DarkGreen
				{0u,   255u,   0u, 0u},	// Color_Green
				{0u,     0u, 128u, 0u},	// Color_DarkBlue
				{0u,     0u, 128u, 0u},	// Color_Blue
				{128u,   0u, 128u, 0u},	// Color_DarkPurple
				{128u,   0u, 128u, 0u},	// Color_Purple
				{255u,   0u, 255u, 0u},	// Color_Pink

				// Unused.... (fill out the rest of the palette with zeros)
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u},
				{0u,     0u,   0u, 0u}
			};
			const BITMAPFILEHEADER BMPFileHeader =
			{
				'MB',	// Type (write the BMP header tag "BM")
				sizeof (BMPFileHeader) + sizeof (BMPInfoHeader) + sizeof (palette) + sizeof (unsigned char) * m_width * m_height,	// Size (file size field)
				0u,	// Reserved 1 (dump zeros in the first reserved field (unused))
				0u,	// Reserved 2 (dump zeros in the second reserved field (unused))
				sizeof (BMPFileHeader) + sizeof (BMPInfoHeader) + sizeof (palette)	// Offset bits (data start field)
			};

			// and tell us we're done
			return file.Write (BMPFileHeader) &&			// write the BMP file header
				file.Write (BMPInfoHeader) &&				// write the BMP info header
				file.Write (palette) &&						// write the BMP palette headers
				file.Write (m_buffer, m_width * m_height);	// write the image itself
		}
};

#endif	// ifndef CORE_TOOL_LIBRARY_BMP_FILE_CREATOR_INCLUDED