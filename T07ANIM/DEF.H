/* FILE NAME: DEF.H
 * PROGRAMMER: VG4
 * DATE: 11.06.2016
 * PURPOSE: Base definitions.
 */

#ifndef __DEF_H_
#define __DEF_H_

#include <windows.h>

#pragma warning(disable: 4244 4305)

/* Debug memory allocation support */
#ifdef _DEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((VOID)0)
#endif /* _DEBUG */

#include <stdlib.h>

#include "vec.h"

/* 64-bit integer data types */
typedef __int64 INT64;
typedef unsigned __int64 UINT64;

#endif /* __DEF_H_ */

/* END OF 'DEF.H' FILE */
