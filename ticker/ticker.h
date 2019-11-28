#pragma once

#include "windows.h"

typedef __int64				tick;
typedef unsigned __int64	utick;
typedef LONGLONG			tickc;

typedef double				sec_t;

class Ticker
{
public:
	static const double	freqSystem;		// convert system time to second
	static LONGLONG		freqCounter;	// frequency of counter 
	static inline int GetTickSystemPrecision()
	{

	}
	static inline int GetTickCounterPrecision()
	{

	}
	static inline int GetTickCyclePrecision()
	{

	}
public:
	/* prepare the Timer environment */
	static bool Prepare()
	{
		LARGE_INTEGER freq;
		if (FALSE == QueryPerformanceFrequency(&freq))
		{
			return false;
		}
		freqCounter = freq.QuadPart;
		return true;
	}
	/* get system time */
	static inline void GetSystemTime(SYSTEMTIME &st)
	{
		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);

		FileTimeToSystemTime(&ft, &st);
	}

	//
	// Cycle Timer
	//
	// TODO input param
	// TODO slow in release
	// TODO tick too fast
	static inline utick GetTickCycle()
	{
		__asm
		{
			_emit 0x0F;
			_emit 0x31;
		}
	}
	/*static inline utick rdtsc()
	{
	unsigned __int32 lo, hi;
	_asm
	{
	"rdtsc":"=a"(lo),"=d"(hi)
	}
	return (utick)hi << 32 | lo;
	}*/
	/* get cycle tick interval */
	static inline utick GetTickCycleInterval(const utick &start)
	{
		return GetTickCycle() - start;
	}
	/* get cycle tick interval ( Debug free ) */
	static inline void GetTickCycleInterval(const utick &start, utick &itv)
	{
		itv = GetTickCycle() - start;
	}
	/* get cycle tick interval second */
	static inline double GetTickCycleSecond(const utick &start)
	{
		return (double)GetTickCycleInterval(start) / Ticker::freqCounter / 1000.0;
	}
	/* get cycle tick interval second ( Debug free ) */
	static inline void GetTickCycleSecond(const utick &start, double &sec)
	{
		utick itv = 0;
		GetTickCycleInterval(start, itv);
		sec = (double)itv / Ticker::freqCounter / 1000.0;
	}

	//
	// System Timer
	//
	/* get system tick */
	static inline utick GetTickSystem()
	{
		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);

		return (
			ft.dwLowDateTime |
			((utick)ft.dwHighDateTime << 32));
	}
	/* get system tick */
	static inline utick GetTickSystem2()
	{
		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);

		utick t = 0;
		((DWORD*)&t)[0] = ft.dwLowDateTime;
		((DWORD*)&t)[1] = ft.dwHighDateTime;

		return t;
	}
	/* get system tick ( Debug free ) */
	static inline void GetTickSystem(utick &ts)
	{
		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);

		ts = (
			ft.dwLowDateTime |
			((utick)ft.dwHighDateTime << 32));
	}
	/* get system tick interval */
	static inline utick GetTickSystemInterval(const utick &start)
	{
		return GetTickSystem() - start;
	}
	/* get system tick interval ( Debug free ) */
	static inline void GetTickSystemInterval(const utick &start, utick &itv)
	{
		utick end = 0;
		GetTickSystem(end);
		itv = end - start;
	}
	/* get system tick interval second */
	static inline double GetTickSystemSecond(const utick &start)
	{
		return (double)GetTickSystemInterval(start) / Ticker::freqSystem;
	}
	/* get system tick interval second ( Debug free ) */
	static inline void GetTickSystemSecond(const utick &start, double &sec)
	{
		utick itv = 0;
		GetTickSystemInterval(start, itv);
		sec = (double)itv / Ticker::freqSystem;// TODO double, long long
	}

	//
	// Counter Timer
	//
	/* get counter tick */
	static inline tickc GetTickCounter()
	{
		LARGE_INTEGER c;
		QueryPerformanceCounter(&c);
		return c.QuadPart;
	}
	/* get counter tick ( Debug free ) */
	static inline void GetTickCounter(tickc &tc)
	{
		LARGE_INTEGER counter = { 0 };
		QueryPerformanceCounter(&counter);
		tc = counter.QuadPart;
	}
	/* get counter tick interval */
	static inline tickc GetTickCounterInterval(const tickc &start)
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		return end.QuadPart - start;
	}
	/* get counter tick interval (Debug free ) */
	static inline void GetTickCounterInterval(const tickc &start, tickc &itv)
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		itv = end.QuadPart - start;
	}
	/* get counter tick interval second */
	static inline double GetTickCounterSecond(const tickc &start)
	{
		return (double)GetTickCounterInterval(start) / Ticker::freqCounter;
	}
	/* get counter tick interval second ( Debug free ) */
	static inline void GetTickCounterSecond(const tickc &start, double &sec)
	{
		tickc itv = 0;
		GetTickCounterInterval(start, itv);
		sec = (double)itv / Ticker::freqCounter;
	}

	static void tickcompare00();
	static void tickcompare0();
	static void ticktime();
	static void tickcompare();
};
