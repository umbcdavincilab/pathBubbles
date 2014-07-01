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

#include <stdio.h>
#define EXPORTING_fit_sine_series 1
#include "fit_sine_series.h"

static HMCRINSTANCE _mcr_inst = NULL;


#if defined( _MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__) || defined(__LCC__)
#ifdef __LCC__
#undef EXTERN_C
#endif
#include <windows.h>

static char path_to_dll[_MAX_PATH];

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, void *pv)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        if (GetModuleFileName(hInstance, path_to_dll, _MAX_PATH) == 0)
            return FALSE;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
    }
    return TRUE;
}
#endif
#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultPrintHandler(const char *s)
{
  return mclWrite(1 /* stdout */, s, sizeof(char)*strlen(s));
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultErrorHandler(const char *s)
{
  int written = 0;
  size_t len = 0;
  len = strlen(s);
  written = mclWrite(2 /* stderr */, s, sizeof(char)*len);
  if (len > 0 && s[ len-1 ] != '\n')
    written += mclWrite(2 /* stderr */, "\n", sizeof(char));
  return written;
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_fit_sine_series_C_API
#define LIB_fit_sine_series_C_API /* No special import/export declaration */
#endif

LIB_fit_sine_series_C_API 
bool MW_CALL_CONV fit_sine_seriesInitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler)
{
    int bResult = 0;
  if (_mcr_inst != NULL)
    return true;
  if (!mclmcrInitialize())
    return false;
  if (!GetModuleFileName(GetModuleHandle("fit_sine_series"), path_to_dll, _MAX_PATH))
    return false;
    {
        mclCtfStream ctfStream = 
            mclGetEmbeddedCtfStream(path_to_dll, 
                                    87204);
        if (ctfStream) {
            bResult = mclInitializeComponentInstanceEmbedded(   &_mcr_inst,
                                                                error_handler, 
                                                                print_handler,
                                                                ctfStream, 
                                                                87204);
            mclDestroyStream(ctfStream);
        } else {
            bResult = 0;
        }
    }  
    if (!bResult)
    return false;
  return true;
}

LIB_fit_sine_series_C_API 
bool MW_CALL_CONV fit_sine_seriesInitialize(void)
{
  return fit_sine_seriesInitializeWithHandlers(mclDefaultErrorHandler, 
                                               mclDefaultPrintHandler);
}

LIB_fit_sine_series_C_API 
void MW_CALL_CONV fit_sine_seriesTerminate(void)
{
  if (_mcr_inst != NULL)
    mclTerminateInstance(&_mcr_inst);
}

LIB_fit_sine_series_C_API 
long MW_CALL_CONV fit_sine_seriesGetMcrID() 
{
  return mclGetID(_mcr_inst);
}

LIB_fit_sine_series_C_API 
void MW_CALL_CONV fit_sine_seriesPrintStackTrace(void) 
{
  char** stackTrace;
  int stackDepth = mclGetStackTrace(&stackTrace);
  int i;
  for(i=0; i<stackDepth; i++)
  {
    mclWrite(2 /* stderr */, stackTrace[i], sizeof(char)*strlen(stackTrace[i]));
    mclWrite(2 /* stderr */, "\n", sizeof(char)*strlen("\n"));
  }
  mclFreeStackTrace(&stackTrace, stackDepth);
}


LIB_fit_sine_series_C_API 
bool MW_CALL_CONV mlxFit_sine_series(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "fit_sine_series", nlhs, plhs, nrhs, prhs);
}

LIB_fit_sine_series_C_API 
bool MW_CALL_CONV mlfFit_sine_series(int nargout, mxArray** c0, mxArray** c1, mxArray** 
                                     A, mxArray** err, mxArray* x, mxArray* y, mxArray* 
                                     w, mxArray* N)
{
  return mclMlfFeval(_mcr_inst, "fit_sine_series", nargout, 4, 4, c0, c1, A, err, x, y, w, N);
}
