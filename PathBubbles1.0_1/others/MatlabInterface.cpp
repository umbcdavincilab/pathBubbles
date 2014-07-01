// #include "MatlabInterface.h"
// 
// void MatlabInterface::fit_sine_series()
// {
// 	mxArray* dataIn;
// 	mxArray* dataOut;
// 	double *y;
// 	double ret;
// 	//********************************************
// 	if( !mclInitializeApplication( NULL, 0) )
// 	{
// 		return;
// 	}
// 	if( !fit_sine_seriesInitialize() )
// 	{
// 		return;
// 	}
// 	//*******************************************
// 	//
// 	mlfFit_sine_series( 4, &dataOut, &dataOut, &dataOut, &dataOut, dataIn, dataIn, dataIn, dataIn );
// 	//
// 	y = mxGetPr( dataOut );
// 	//
// 	ret = *y;
// 	//
// 	fit_sine_seriesTerminate();
// 	//
// 	mxDestroyArray( dataIn );
// 	mxDestroyArray( dataOut );
// 	mclTerminateApplication();
// 	return;
// }