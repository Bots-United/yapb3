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
// CPU.hpp
//
// Purpose: win32 dependant ASM code for CPU capability detection
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_CPU_INCLUDED
#define CORE_TOOL_LIBRARY_CPU_INCLUDED

long long g_ClockSpeed;	// Clocks/sec
unsigned long g_dwClockSpeed;

double g_ClockSpeedMicrosecondsMultiplier;
double g_ClockSpeedMillisecondsMultiplier;
double g_ClockSpeedSecondsMultiplier;

class CCycleCount
{
public:
					CCycleCount();

	void			Sample();	// Sample the clock. This takes about 34 clocks to execute (or 26,000 calls per millisecond on a P900).

	void			Init();		// Set to zero.
	void			Init( float initTimeMsec );
	bool			IsLessThan( CCycleCount const &other ) const;					// Compare two counts.

	// Convert to other time representations. These functions are slow, so it's preferable to call them
	// during display rather than inside a timing block.
	unsigned long	GetCycles()  const;

	unsigned long	GetMicroseconds() const; 	
	double			GetMicrosecondsF() const; 	

	unsigned long	GetMilliseconds() const;
	double			GetMillisecondsF() const;

	double			GetSeconds() const;

	CCycleCount&	operator+=( CCycleCount const &other );

	// dest = rSrc1 + rSrc2
	static void		Add( CCycleCount const &rSrc1, CCycleCount const &rSrc2, CCycleCount &dest );	// Add two samples together.
	
	// dest = rSrc1 - rSrc2
	static void		Sub( CCycleCount const &rSrc1, CCycleCount const &rSrc2, CCycleCount &dest );	// Add two samples together.


	long long	m_Int64;
};

// -------------------------------------------------------------------------- // 
// CCycleCount inlines.
// -------------------------------------------------------------------------- // 

inline CCycleCount::CCycleCount()
{
	m_Int64 = 0;
}

inline void CCycleCount::Init()
{
	m_Int64 = 0;
}

inline void CCycleCount::Init( float initTimeMsec )
{
	if ( g_ClockSpeedMillisecondsMultiplier > 0 )
		m_Int64 = static_cast <long long> (initTimeMsec / g_ClockSpeedMillisecondsMultiplier);
	else
		m_Int64 = 0;
}

inline void CCycleCount::Sample()
{
	unsigned long* pSample = (unsigned long *)&m_Int64;
	__asm
	{
		// force the cpu to synchronize the instruction queue
		// NJS: CPUID can really impact performance in tight loops.
		//cpuid
		//cpuid
		//cpuid
		mov		ecx, pSample
		rdtsc
		mov		[ecx],     eax
		mov		[ecx+4],   edx
	}
}


inline CCycleCount& CCycleCount::operator+=( CCycleCount const &other )
{
	m_Int64 += other.m_Int64;
	return *this;
}

inline void CCycleCount::Add( CCycleCount const &rSrc1, CCycleCount const &rSrc2, CCycleCount &dest )
{
	dest.m_Int64 = rSrc1.m_Int64 + rSrc2.m_Int64;
}

inline void CCycleCount::Sub( CCycleCount const &rSrc1, CCycleCount const &rSrc2, CCycleCount &dest )
{
	dest.m_Int64 = rSrc1.m_Int64 - rSrc2.m_Int64;
}

inline bool CCycleCount::IsLessThan(CCycleCount const &other) const
{
	return m_Int64 < other.m_Int64;
}

inline unsigned long CCycleCount::GetCycles() const
{
	return (unsigned long)m_Int64;
}

inline unsigned long CCycleCount::GetMicroseconds() const
{
	return (unsigned long)((m_Int64 * 1000000) / g_ClockSpeed);
}

inline double CCycleCount::GetMicrosecondsF() const
{
	return (double)( m_Int64 * g_ClockSpeedMicrosecondsMultiplier );
}

inline unsigned long CCycleCount::GetMilliseconds() const
{
	return (unsigned long)((m_Int64 * 1000) / g_ClockSpeed);
}

inline double CCycleCount::GetMillisecondsF() const
{
	return (double)( m_Int64 * g_ClockSpeedMillisecondsMultiplier );
}

inline double CCycleCount::GetSeconds() const
{
	return (double)( m_Int64 * g_ClockSpeedSecondsMultiplier );
}

static bool cpuid(unsigned long function, unsigned long& out_eax, unsigned long& out_ebx, unsigned long& out_ecx, unsigned long& out_edx)
{
	bool retval = true;
	unsigned long local_eax, local_ebx, local_ecx, local_edx;
	__asm pushad;

	__try
	{
        __asm
		{
			xor edx, edx		// Clue the compiler that EDX is about to be used.
            mov eax, function   // set up CPUID to return processor version and features
								//      0 = vendor string, 1 = version info, 2 = cache info
            cpuid				// code bytes = 0fh,  0a2h
            mov local_eax, eax	// features returned in eax
            mov local_ebx, ebx	// features returned in ebx
            mov local_ecx, ecx	// features returned in ecx
            mov local_edx, edx	// features returned in edx
		}
    } 
	__except(EXCEPTION_EXECUTE_HANDLER) 
	{ 
		retval = false; 
	}

	out_eax = local_eax;
	out_ebx = local_ebx;
	out_ecx = local_ecx;
	out_edx = local_edx;

	__asm popad

	return retval;
}

static inline bool CheckMMXTechnology(void)
{
    unsigned long eax,ebx,edx,unused;
    if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & 0x800000 ) != 0;
}

static inline bool CheckSSETechnology(void)
{
    unsigned long eax,ebx,edx,unused;
    if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & 0x2000000L ) != 0;
}

static inline bool CheckSSE2Technology(void)
{
    unsigned long eax,ebx,edx,unused;
    if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & 0x04000000 ) != 0;
}

static inline bool Check3DNowTechnology(void)
{
    unsigned long eax, unused;
    if( !cpuid(0x80000000,eax,unused,unused,unused) )
		return false;

    if ( eax > 0x80000000L )
    {
     	if( !cpuid(0x80000001,unused,unused,unused,eax) )
			return false;

		return ( eax & 1<<31 ) != 0;
    }
    return false;
}

static inline bool CheckCMOVTechnology()
{
    unsigned long eax,ebx,edx,unused;
    if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & (1<<15) ) != 0;
}

static inline bool CheckFCMOVTechnology(void)
{
    unsigned long eax,ebx,edx,unused;
    if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & (1<<16) ) != 0;
}

static inline bool CheckRDTSCTechnology(void)
{
    unsigned long eax,ebx,edx,unused;
    if( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & 0x10 ) != 0;
}

// Return the Processor's vendor identification string, or "Generic_x86" if it doesn't exist on this CPU
static inline const char* GetProcessorVendorId()
{
	unsigned long unused, VendorIDRegisters[3];

	static char VendorID[13];
	
	memset( VendorID, 0, sizeof(VendorID) );
	if( !cpuid(0,unused, VendorIDRegisters[0], VendorIDRegisters[2], VendorIDRegisters[1] ) )
	{
		strcpy( VendorID, "Generic_x86" ); 
	}
	else
	{
		memcpy( VendorID+0, &(VendorIDRegisters[0]), sizeof( VendorIDRegisters[0] ) );
		memcpy( VendorID+4, &(VendorIDRegisters[1]), sizeof( VendorIDRegisters[1] ) );
		memcpy( VendorID+8, &(VendorIDRegisters[2]), sizeof( VendorIDRegisters[2] ) );
	}

	return VendorID;
}

// Returns non-zero if Hyper-Threading Technology is supported on the processors and zero if not. This does not mean that 
// Hyper-Threading Technology is necessarily enabled.
static bool HTSupported(void)
{
	const unsigned int HT_BIT		 = 0x10000000;  // EDX[28] - Bit 28 set indicates Hyper-Threading Technology is supported in hardware.
	const unsigned int FAMILY_ID     = 0x0f00;      // EAX[11:8] - Bit 11 thru 8 contains family processor id
	const unsigned int EXT_FAMILY_ID = 0x0f00000;	// EAX[23:20] - Bit 23 thru 20 contains extended family  processor id
	const unsigned int PENTIUM4_ID   = 0x0f00;		// Pentium 4 family processor id

	unsigned long unused,
				  reg_eax = 0, 
				  reg_edx = 0,
				  vendor_id[3] = {0, 0, 0};

	// verify cpuid instruction is supported
	if( !cpuid(0,unused, vendor_id[0],vendor_id[2],vendor_id[1]) 
	 || !cpuid(1,reg_eax,unused,unused,reg_edx) )
	 return false;

	//  Check to see if this is a Pentium 4 or later processor
	if (((reg_eax & FAMILY_ID) ==  PENTIUM4_ID) || (reg_eax & EXT_FAMILY_ID))
		if (vendor_id[0] == 'uneG' && vendor_id[1] == 'Ieni' && vendor_id[2] == 'letn')
			return (reg_edx & HT_BIT) != 0;	// Genuine Intel Processor with Hyper-Threading Technology

	return false;  // This is not a genuine Intel processor.
}

// Returns the number of logical processors per physical processors.
static unsigned char LogicalProcessorsPerPackage(void)
{
	const unsigned NUM_LOGICAL_BITS = 0x00FF0000; // EBX[23:16] indicate number of logical processors per package

    unsigned long unused,
		          reg_ebx = 0;
    if (!HTSupported()) 
		return 1; 

	if( !cpuid(1,unused,reg_ebx,unused,unused) )
		return 1;

	return (unsigned char) ((reg_ebx & NUM_LOGICAL_BITS) >> 16);
}

// Measure the processor clock speed by sampling the cycle count, waiting
// for some fraction of a second, then measuring the elapsed number of cycles.
static long long CalculateClockSpeed()
{
	LARGE_INTEGER waitTime, startCount, curCount;
	QueryPerformanceCounter(&startCount);
	QueryPerformanceFrequency(&waitTime);
	int scale = 5;		// Take 1/32 of a second for the measurement.
	waitTime.QuadPart >>= scale;

	CCycleCount start, end;
	start.Sample();
	do
	{
		QueryPerformanceCounter(&curCount);
	}
	while(curCount.QuadPart - startCount.QuadPart < waitTime.QuadPart);
	end.Sample();

	return (end.m_Int64 - start.m_Int64) << scale;
}

// Processor Information:
struct CPUInformation
{
	int	 m_Size;		// Size of this structure, for forward compatability.

	bool m_bRDTSC : 1,	// Is RDTSC supported?
		 m_bCMOV  : 1,  // Is CMOV supported?
		 m_bFCMOV : 1,  // Is FCMOV supported?
		 m_bSSE	  : 1,	// Is SSE supported?
		 m_bSSE2  : 1,	// Is SSE2 Supported?
		 m_b3DNow : 1,	// Is 3DNow! Supported?
		 m_bMMX   : 1,	// Is MMX supported?
		 m_bHT	  : 1;	// Is HyperThreading supported?

	unsigned char m_nLogicalProcessors,		// Number op logical processors.
		          m_nPhysicalProcessors;	// Number of physical processors

	long long m_Speed;						// In cycles per second.

	char* m_szProcessorID;				// Processor vendor Identification.
};

inline const CPUInformation& GetCPUInformation()
{
	static CPUInformation pi;

	// Has the structure already been initialized and filled out?
	if( pi.m_Size == sizeof(pi) )
		return pi;

	// Redundant, but just in case the user somehow messes with the size.
	ZeroMemory(&pi, sizeof(pi));

	// Fill out the structure, and return it:
	pi.m_Size = sizeof(pi);

	// Grab the processor frequency:
	pi.m_Speed = CalculateClockSpeed();

	
	// Get the logical and physical processor counts:
	pi.m_nLogicalProcessors = LogicalProcessorsPerPackage();

	SYSTEM_INFO si;
	ZeroMemory( &si, sizeof(si) );

	GetSystemInfo( &si );

	pi.m_nPhysicalProcessors = static_cast <unsigned char> (si.dwNumberOfProcessors / pi.m_nLogicalProcessors);
	pi.m_nLogicalProcessors *= pi.m_nPhysicalProcessors;

	// Make sure I always report at least one, when running WinXP with the /ONECPU switch, 
	// it likes to report 0 processors for some reason.
	if( pi.m_nPhysicalProcessors == 0 && pi.m_nLogicalProcessors == 0 )
	{
		pi.m_nPhysicalProcessors = 1;
		pi.m_nLogicalProcessors = 1;
	}

	// Determine Processor Features:
	pi.m_bRDTSC = CheckRDTSCTechnology();
	pi.m_bCMOV  = CheckCMOVTechnology();
	pi.m_bFCMOV = CheckFCMOVTechnology();
	pi.m_bMMX   = CheckMMXTechnology();
	pi.m_bSSE   = CheckSSETechnology();
	pi.m_bSSE2  = CheckSSE2Technology();
	pi.m_b3DNow = Check3DNowTechnology();

	pi.m_szProcessorID = (char*)GetProcessorVendorId();
	return pi;
}

inline void ClockSpeedInit()
{
	const CPUInformation& pi = GetCPUInformation();

	g_ClockSpeed = pi.m_Speed;
	g_dwClockSpeed = (unsigned long)g_ClockSpeed;

	g_ClockSpeedMicrosecondsMultiplier = 1000000.0 / (double)g_ClockSpeed;
	g_ClockSpeedMillisecondsMultiplier = 1000.0 / (double)g_ClockSpeed;
	g_ClockSpeedSecondsMultiplier = 1.0f / (double)g_ClockSpeed;
}

#endif	// ifndef CORE_TOOL_LIBRARY_CPU_INCLUDED