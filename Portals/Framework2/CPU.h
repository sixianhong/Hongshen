/***********      .---.         .-"-.      *******************\
* -------- *     /   ._.       / � ` \     * ---------------- *
* Author's *     \_  (__\      \_�v�_/     * humus@rogers.com *
*   note   *     //   \\       //   \\     * ICQ #47010716    *
* -------- *    ((     ))     ((     ))    * ---------------- *
*          ****--""---""-------""---""--****                  ********\
* This file is a part of the work done by Humus. You are free to use  *
* the code in any way you like, modified, unmodified or copy'n'pasted *
* into your own work. However, I expect you to respect these points:  *
*  @ If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  @ For use in anything commercial, please request my approval.      *
*  @ Share your work and ideas too as much as you can.                *
\*********************************************************************/

#ifndef _CPU_H_
#define _CPU_H_

#include "Platform.h"

extern int cpuFamily;
extern char cpuVendor[13];
extern char cpuBrandName[49];
extern bool cpuCMOV, cpu3DNow, cpu3DNowExt, cpuMMX, cpuMMXExt, cpuSSE, cpuSSE2, cpuSSE3;


#if defined(WIN32)
#define CPUID __asm _emit 0x0F __asm _emit 0xA2
#define RDTSC __asm _emit 0x0F __asm _emit 0x31

void cpuidAsm(uint32 func, uint32 *a, uint32 *b, uint32 *c, uint32 *d);
#define cpuid(func, a, b, c, d) cpuidAsm(func, &a, &b, &c, &d)


#pragma warning(disable: 4035)
inline uint64 getCycleNumber(){
	__asm {
		RDTSC
	}
}

#elif defined(LINUX)
#define cpuid(in,a,b,c,d)\
  asm volatile ("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));

#define rdtsc(a,d)\
  asm volatile ("rdtsc" : "=a" (a), "=d" (d));

inline uint64 getCycleNumber(){
	union {
		struct {
			unsigned int a,d;
		};
		long long ad;
	} ads;

	rdtsc(ads.a,ads.d);
	return ads.ad;
}
#endif

uint64 getHz();
void initCPU();

#endif // _CPU_H_
