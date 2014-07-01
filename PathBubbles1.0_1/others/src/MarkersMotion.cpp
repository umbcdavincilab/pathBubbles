#include <stdlib.h>
#include "MarkersMotion.h"
#include "LCSVParser.h"

#define X_COORD            0
#define Y_COORD            1
#define Z_COORD            2
#define NUM_MARKER_COORDS  3

#define NO_DATA			   -32768

MarkersMotion::MarkersMotion(const char *motionDataFile, const int framesPerSec)
{
	// Start by parsing our motion (raw data) file (.csv)
	m_pDataParser = new LCSVParser(motionDataFile);
	// And set the frequency that the data in the above file was collected.
	m_framesPerSec = framesPerSec;

	// Initialize number of frames and number of markers.
	m_numFrames = m_pDataParser->GetNumEntries() - 1;
	m_numMarkers = m_pDataParser->GetNumFields()/NUM_MARKER_COORDS;

	// Now initialize our 3D "array" of motion data from the parsed data file.
	m_motionData = new double**[GetNumFrames()];

	for(int frame = 0; frame < GetNumFrames(); frame++)
	{
		m_motionData[frame] = new double*[GetNumMarkers()];

		for(int markerIndex = 0; markerIndex < GetNumMarkers(); markerIndex++)
		{
			m_motionData[frame][markerIndex] = new double[NUM_MARKER_COORDS];

			for(int coordinate = 0; coordinate < NUM_MARKER_COORDS; coordinate++)
			{
				// Calculate the proper coordinate index to look up the appropriate data
				// for the given marker.
				int coordinateColumn = (markerIndex * NUM_MARKER_COORDS) + coordinate;

				// Find and store the appropriate data for each coordinate of our marker.
				// Note:  We obtain data from entry (frame + 1) in order to avoid
				//        the first row, which consists of field headers.
				//----------------------------------------------------------------------
				std::string rawEntry;
				m_pDataParser->GetDataEntryInField(frame + 1, coordinateColumn, rawEntry);
				// Check if we've got no data in this entry.
				if(rawEntry.compare("NaN") == 0)
				{
					// If we have no data, add in our NO_DATA constant placeholder.
					m_motionData[frame][markerIndex][coordinate] = NO_DATA;
				}
				else
				{
					// Otherwise, our data is good, so add it appropriately.
					m_motionData[frame][markerIndex][coordinate] = atof(rawEntry.c_str());
				}
			}
		}
	}
}


MarkersMotion::MarkersMotion(double*** inMotionData, int numFrames, int numMarkers, const int framesPerSec)
{
	m_motionData = inMotionData;
	m_numFrames = numFrames;
	m_numMarkers = numMarkers;
	m_framesPerSec = framesPerSec;
}


MarkersMotion::~MarkersMotion()
{
	// Clear out all of the motion data.
	for(int i = 0; i < GetNumFrames(); i++)
	{
		for(int j = 0; j < GetNumMarkers(); j++)
		{
			delete m_motionData[i][j];
		}
		delete m_motionData[i];
	}
	delete m_motionData;
	m_motionData = NULL;

	delete m_pDataParser;
	m_pDataParser = NULL;
}


int MarkersMotion::GetNumMarkers()
{
	return m_numMarkers;
}

int MarkersMotion::GetNumFrames()
{
	return m_numFrames;
}


int MarkersMotion::GetFramesPerSecond()
{
	// Get the number of frames per second the data was recorded at.
	return m_framesPerSec;
}


double MarkersMotion::GetTimeStep()
{
	// By inverting our frames per second, we obtain the time interval between
	// frames (in fractions of a second)
	return 1.0f/GetFramesPerSecond();
}


double MarkersMotion::GetTotalTime()
{
	// The total time of our recorded data is precisely the number of frames
	// recorded multiplied by the interval between frames.
	return GetNumFrames() * GetTimeStep();
}


bool MarkersMotion::GetMarkerPositionAtFrame(const int &frame, const int &markerIndex,
											 double &x, double &y, double &z)
{
	// Make sure we have been given a valid frame #.
	if(frame < 0 || frame > GetNumFrames())
		return false;

	// Make sure we have been given a valid marker index.
	if(markerIndex < 0 || markerIndex > GetNumMarkers())
		return false;

	// Now get the x, y, z coordinates for the requested marker based on the frame
	// and marker index we've been given.  If any of our coordinates contain no data,
	// return false and jump out of here.
	if(m_motionData[frame][markerIndex][X_COORD] == NO_DATA)
		return false;

	if(m_motionData[frame][markerIndex][Y_COORD] == NO_DATA)
		return false;

	if(m_motionData[frame][markerIndex][Z_COORD] == NO_DATA)
		return false;

	// If all of our data is good to go, set x, y, z accordingly and return successfully.
	x = m_motionData[frame][markerIndex][X_COORD];
	y = m_motionData[frame][markerIndex][Y_COORD];
	z = m_motionData[frame][markerIndex][Z_COORD];

	return true;
}


// Returns position interpolated linearly between prevFrame and prevFrame + 1, time
// ranges from 0 to 1.
bool MarkersMotion::GetLinearInterpolatedMarkerPositionAtTime(const int &prevFrame, const double &time,
															  const int &markerIndex, double &x, double &y, double &z)
{
	// Make sure we've been given a valid time.
	if(time < 0.0f || time > 1.0f)
		return false;

	// Make sure we've been given a valid marker index.
	if(markerIndex < 1 || markerIndex > GetNumMarkers())
		return false;

	// Now get the marker's coordinates at prevFrame and prevFrame + 1 in order to
	// get the linearly interpolated data for the requested time.
	double x0, y0, z0;
	double x1, y1, z1;

	// If we can't get valid data at the previous frame, return false, unsuccessful.
	if(!GetMarkerPositionAtFrame(prevFrame, markerIndex, x0, y0, z0))
		return false;

	// If we can't get valid data at the next frame, return false, unsuccessful.
	if(!GetMarkerPositionAtFrame(prevFrame + 1, markerIndex, x1, y1, z1))
		return false;

	// OK, here, we've got good data.  It's time to interpolate.
	// We parametrize the line between (x0, y0, z0) and (x1, y1, z1) using the
	// parametrization:
	//
	//           (xt, yt, zt) = (1 - t)(x0, y0, z0) + (t)(x1, y1, z1)
	//----------------------------------------------------------------------------

	x = ((1 - time) * x0) + (time * x1);
	y = ((1 - time) * y0) + (time * y1);
	z = ((1 - time) * z0) + (time * z1);

	// We were successful, so let everyone know.
	return true;
}