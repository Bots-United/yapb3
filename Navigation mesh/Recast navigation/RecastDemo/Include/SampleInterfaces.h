//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#ifndef SAMPLEINTERFACES_H
#define SAMPLEINTERFACES_H

// These are example implementations of various interfaces used in Recast and Detour.

/// Recast build context.
class BuildContext : public rcContext
{
	TimeVal m_startTime[RC_MAX_TIMERS];
	int m_accTime[RC_MAX_TIMERS];

	static const int MAX_MESSAGES = 1000;
	const char* m_messages[MAX_MESSAGES];
	int m_messageCount;
	static const int TEXT_POOL_SIZE = 8000;
	char m_textPool[TEXT_POOL_SIZE];
	int m_textPoolSize;
	
public:
	BuildContext();
	virtual ~BuildContext();
	
	/// Dumps the log to stdout.
	void dumpLog(const char* format, ...);
	/// Returns number of log messages.
	int getLogCount() const;
	/// Returns log message text.
	const char* getLogText(const int i) const;
	
protected:	
	/// Virtual functions for custom implementations.
	///@{
	virtual void doResetLog();
	virtual void doLog(const rcLogCategory /*category*/, const char* /*msg*/, const int /*len*/);
	virtual void doResetTimers();
	virtual void doStartTimer(const rcTimerLabel /*label*/);
	virtual void doStopTimer(const rcTimerLabel /*label*/);
	virtual int doGetAccumulatedTime(const rcTimerLabel /*label*/) const;
	///@}
};

inline void vec3ToRecast (const Math::Vector3D &v, float *const p)
{
	// convert vertex to HL engine coordinates: swap y and z
	p[0] = v.x;
	p[1] = v.z;
	p[2] = v.y;
}
inline Math::Vector3D recastToVec3 (const float *const p)
{
	return Math::Vector3D (p[0], p[2], p[1]);	// convert vertex to HL engine coordinates: swap y and z
//	return Math::Vector3D (p).GetPermutated (Math::Vector3D::AxisID_Y);	// convert vertex to HL engine coordinates: swap y and z
}

/// OpenGL debug draw implementation.
class DebugDrawGL : public duDebugDraw
{
public:
	virtual void depthMask(bool state)
	{
		#if defined MULTIPLE_DRAW_METHODS
			g_drawInterface->DepthMask (state);
		#endif	// if defined MULTIPLE_DRAW_METHODS
	}
	virtual void texture(bool state)
	{
		#if defined MULTIPLE_DRAW_METHODS
			g_drawInterface->TextureInternally (state);
		#endif	// if defined MULTIPLE_DRAW_METHODS
	}
	virtual void begin(duDebugDrawPrimitives prim, float size = 1.0f)
	{
		#if defined MULTIPLE_DRAW_METHODS
			g_drawInterface->Begin (static_cast <DrawInterface::Type_t> (prim), static_cast <const unsigned char> (size));
		#endif	// if defined MULTIPLE_DRAW_METHODS
	}
	virtual void vertex(const float* pos, unsigned int color)
	{
		#if defined MULTIPLE_DRAW_METHODS
			g_drawInterface->SubmitVertex (recastToVec3 (pos), ColorWithAlpha <>::GetFromRaw (color));
		#endif	// if defined MULTIPLE_DRAW_METHODS
	}
	virtual void vertex(const float x, const float y, const float z, unsigned int color)
	{
		#if defined MULTIPLE_DRAW_METHODS
			g_drawInterface->SubmitVertex (recastToVec3 (Math::Vector3D (x, y, z)), ColorWithAlpha <>::GetFromRaw (color));
		#endif	// if defined MULTIPLE_DRAW_METHODS
	}
	virtual void vertex(const float* pos, unsigned int color, const float* uv)
	{
		#if defined MULTIPLE_DRAW_METHODS
			g_drawInterface->SubmitVertex (recastToVec3 (pos), ColorWithAlpha <>::GetFromRaw (color), uv);
		#endif	// if defined MULTIPLE_DRAW_METHODS
	}
	virtual void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v)
	{
		#if defined MULTIPLE_DRAW_METHODS
			g_drawInterface->SubmitVertex (recastToVec3 (Math::Vector3D (x, y, z)), ColorWithAlpha <>::GetFromRaw (color), Math::Vector2D (u, v));
		#endif	// if defined MULTIPLE_DRAW_METHODS
	}
	virtual void end()
	{
		#if defined MULTIPLE_DRAW_METHODS
			g_drawInterface->End ();
		#endif	// if defined MULTIPLE_DRAW_METHODS
	}
};

/// stdio file implementation.
class FileIO : public duFileIO
{
	FILE* m_fp;
	int m_mode;
public:
	FileIO();
	virtual ~FileIO();
	bool openForWrite(const char* path);
	bool openForRead(const char* path);
	virtual bool isWriting() const;
	virtual bool isReading() const;
	virtual bool write(const void* ptr, const size_t size);
	virtual bool read(void* ptr, const size_t size);
};

#endif // SAMPLEINTERFACES_H

