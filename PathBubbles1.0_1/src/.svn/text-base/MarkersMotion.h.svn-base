#ifndef MARKERSMOTION_H
#define MARKERSMOTION_H

#include <stdlib.h>
#include <stdio.h>
#include <string>

class LCSVParser;

class MarkersMotion
{
public:

	// Constructor for creating markers motion data from file.
	MarkersMotion(const char *motionDataFile, const int framesPerSec);

	// Constructor for creating markers motion data from a pre-processed
	// 3D "array" of data.
	// Note: Dimensions are [Frames] [Markers] [x, y, z coordinates]
	MarkersMotion(double*** inMotionData, int numFrames, int numMarkers, const int framesPerSec);
	virtual ~MarkersMotion();

	// Get the total number of markers being visualized.
	int GetNumMarkers();

	// Get the number of frames of data recorded in the data file.
	int GetNumFrames();

	// Get the number of frames recorded per second.
	int GetFramesPerSecond();

	// Get the amount of time elapsed between each frame (in fractions of a second)
	double GetTimeStep();

	// Get the total time of the recorded data in seconds.
	double GetTotalTime();

	// Get a specific marker's coordinates at a specific frame.
	// Returns false if there is no valid data for the given marker at the given frame.
	bool GetMarkerPositionAtFrame(const int &frame, const int &markerIndex,
		double &x, double &y, double &z);

	// Get the linearly interpolated position of a specific marker at a specific time.
	// Note:  Time ranges between 0.0 and numFrames()*timeStep().
	// Returns false if there is no valid data for the from which the given marker's location
	// can be interpolated.
	bool GetLinearInterpolatedMarkerPositionAtTime(const int &prevFrame, const double &time,
		const int &markerIndex, double &x, double &y, double &z);

private:
	int m_framesPerSec;
	int m_numMarkers;
	int m_numFrames;
	double*** m_motionData;
	LCSVParser *m_pDataParser;
};

#endif // MARKERSMOTION_H
