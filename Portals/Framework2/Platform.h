/***********      .---.         .-"-.      *******************\
* -------- *     /   ._.       / ´ ` \     * ---------------- *
* Author's *     \_  (__\      \_°v°_/     * humus@rogers.com *
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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

// For VC 2005 to be happy about "unsecure" CRT functions
#if _MSC_VER >= 1400
#pragma warning (disable: 4996)
#endif

typedef unsigned short uint16;
typedef   signed short  int16;

typedef unsigned int uint32;
typedef   signed int  int32;


#define MCHAR2(a, b) (a | (b << 8))
#define MCHAR4(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))

#if defined(_WIN32)

typedef   signed __int64  int64;
typedef unsigned __int64 uint64;

#elif defined(LINUX)

typedef unsigned int uint;
typedef   signed long long  int64;
typedef unsigned long long uint64;

#define stricmp(a, b) strcasecmp(a, b)

#endif // LINUX

#endif // _PLATFORM_H_
