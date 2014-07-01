/*
 * MATLAB Compiler: 4.14 (R2010b)
 * Date: Fri Feb 11 15:46:01 2011
 * Arguments: "-B" "macro_default" "-W" "lib:fit_sine_series" "-T" "link:lib"
 * "-d" "C:\Documents and
 * Settings\Administrator\×ÀÃæ\VisBubbles\MainApp\code.Hanyu\visBubble\winQt_Tes
 * t\fit_sine_series" "-w" "enable:specified_file_mismatch" "-w"
 * "enable:repeated_file" "-w" "enable:switch_ignored" "-w"
 * "enable:missing_lib_sentinel" "-w" "enable:demo_license" "-v" "C:\Documents
 * and
 * Settings\Administrator\×ÀÃæ\VisBubbles\MainApp\code.Hanyu\visBubble\winQt_Tes
 * t\fit_sine_series\fit_sine_series.m" 
 */

#ifndef __fit_sine_series_h
#define __fit_sine_series_h 1

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

#ifdef EXPORTING_fit_sine_series
#define PUBLIC_fit_sine_series_C_API __global
#else
#define PUBLIC_fit_sine_series_C_API /* No import statement needed. */
#endif

#define LIB_fit_sine_series_C_API PUBLIC_fit_sine_series_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_fit_sine_series
#define PUBLIC_fit_sine_series_C_API __declspec(dllexport)
#else
#define PUBLIC_fit_sine_series_C_API __declspec(dllimport)
#endif

#define LIB_fit_sine_series_C_API PUBLIC_fit_sine_series_C_API


#else

#define LIB_fit_sine_series_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_fit_sine_series_C_API 
#define LIB_fit_sine_series_C_API /* No special import/export declaration */
#endif

extern LIB_fit_sine_series_C_API 
bool MW_CALL_CONV fit_sine_seriesInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_fit_sine_series_C_API 
bool MW_CALL_CONV fit_sine_seriesInitialize(void);

extern LIB_fit_sine_series_C_API 
void MW_CALL_CONV fit_sine_seriesTerminate(void);



extern LIB_fit_sine_series_C_API 
void MW_CALL_CONV fit_sine_seriesPrintStackTrace(void);

extern LIB_fit_sine_series_C_API 
bool MW_CALL_CONV mlxFit_sine_series(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                     *prhs[]);

extern LIB_fit_sine_series_C_API 
long MW_CALL_CONV fit_sine_seriesGetMcrID();



extern LIB_fit_sine_series_C_API bool MW_CALL_CONV mlfFit_sine_series(int nargout, mxArray** c0, mxArray** c1, mxArray** A, mxArray** err, mxArray* x, mxArray* y, mxArray* w, mxArray* N);

#ifdef __cplusplus
}
#endif
#endif
