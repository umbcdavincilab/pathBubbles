/*
 * MATLAB Compiler: 4.8 (R2008a)
 * Date: Thu Sep 16 01:39:00 2010
 * Arguments: "-B" "macro_default" "-t" "-W" "lib:jcCamberLib" "-L" "C" "-T"
 * "link:lib" "-h" "libmwsglm.mlib" "libmmfile.mlib" "jcCamber.m" 
 */

#ifndef __jcCamberLib_h
#define __jcCamberLib_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_jcCamberLib
#define PUBLIC_jcCamberLib_C_API __global
#else
#define PUBLIC_jcCamberLib_C_API /* No import statement needed. */
#endif

#define LIB_jcCamberLib_C_API PUBLIC_jcCamberLib_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_jcCamberLib
#define PUBLIC_jcCamberLib_C_API __declspec(dllexport)
#else
#define PUBLIC_jcCamberLib_C_API __declspec(dllimport)
#endif

#define LIB_jcCamberLib_C_API PUBLIC_jcCamberLib_C_API


#else

#define LIB_jcCamberLib_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_jcCamberLib_C_API 
#define LIB_jcCamberLib_C_API /* No special import/export declaration */
#endif

extern LIB_jcCamberLib_C_API 
bool MW_CALL_CONV jcCamberLibInitializeWithHandlers(mclOutputHandlerFcn error_handler,
                                                    mclOutputHandlerFcn print_handler);

extern LIB_jcCamberLib_C_API 
bool MW_CALL_CONV jcCamberLibInitialize(void);

extern LIB_jcCamberLib_C_API 
void MW_CALL_CONV jcCamberLibTerminate(void);



extern LIB_jcCamberLib_C_API 
void MW_CALL_CONV jcCamberLibPrintStackTrace(void);


extern LIB_jcCamberLib_C_API 
bool MW_CALL_CONV mlxJcCamber(int nlhs, mxArray *plhs[],
                              int nrhs, mxArray *prhs[]);


extern LIB_jcCamberLib_C_API bool MW_CALL_CONV mlfJcCamber(int nargout
                                                           , mxArray** max_min
                                                           , mxArray* IntersectionPoints);

#ifdef __cplusplus
}
#endif

#endif
