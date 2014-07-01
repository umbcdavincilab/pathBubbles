
#include "LCSVParser.h"
#include <fstream>
#include <iostream>
#include <assert.h>

#define NPOS	-1


LCSVParser::LCSVParser(const char *csvFile)
{
	// Try to open up the requested file.
	m_inDataFile.open(csvFile);

	// Assert that the file exists.
	assert(m_inDataFile.fail() != 1);

	// If it's a valid file, and we're able to open it,
	if(m_inDataFile.fail() != 1 || m_inDataFile.is_open())
	{
		// Make a first pass to determine the dimensions of our data.
		GetDimensions();

		m_inDataFile.clear();
		m_inDataFile.seekg(0, std::ios::beg);

		// Send it off to be parsed.
		Parse();

		m_inDataFile.close();
	}
}

LCSVParser::~LCSVParser()
{
}

// Returns the total number of fields (columns) in the .csv file.
int LCSVParser::GetNumFields()
{
	return m_numCols;
}

// Returns the total number of line entries (rows) in the .csv file.
int LCSVParser::GetNumEntries()
{
	return m_numRows;
}

int LCSVParser::GetFieldIndexByName(const std::string &fieldName)
{
	// Loop through the parsed data to find the appropriately
	// named column
	for(int i = 0; i < GetNumFields(); i++)
	{
		if(m_parsedData[0][i].compare(fieldName) == 0)
		{
			return i;
		}
	}

	// If we're here, we couldn't find the given field, so spit
	// back a nasty index.
	return -1;
}


// Returns a particular data entry as a string.
bool LCSVParser::GetDataEntryInField(const std::string &fieldName, int entryIndex, std::string &entryData)
{
	// Get the field's index, given its name.
	int fieldIndex = GetFieldIndexByName(fieldName);
	// Using the field and entry indices, grab the appropriate data.
	return GetDataEntryInField(entryIndex, fieldIndex, entryData);
}

bool LCSVParser::GetDataEntryInField(int entryIndex, int fieldIndex, std::string &entryData)
{
	// Make sure the indices we've been given are valid.
	if(entryIndex >= 0 && entryIndex < GetNumEntries()
		&& fieldIndex >= 0 && fieldIndex < GetNumFields())
	{
		// If the indices are valid, return the requested data entry.
		entryData = m_parsedData[entryIndex][fieldIndex];
		// We were successful, return true.
		return true;
	}
	else
	{
		// If one or both of our indices are bad, return false, unsuccessful.
		return false;
	}
}


void LCSVParser::Parse()
{
	if(!m_inDataFile.is_open())
		return;

	int row = 0;
	m_parsedData = new std::string*[GetNumEntries()];
	// If so, let's start parsing the file line by line. -tmob
	std::string lineToParse;
	while( getline(m_inDataFile, lineToParse) )
	{
		// After we grab each line, send it off to be split into
		// the appropriate fields. -tmob
		ParseLine(lineToParse, row);
		row++;
	}
}


void LCSVParser::GetDimensions()
{
	m_numRows = 0;
	m_numCols = 0;

	if(!m_inDataFile.is_open())
		return;

	// If so, let's start parsing the file line by line. -tmob
	std::string lineToCount;
	while( getline(m_inDataFile, lineToCount) )
	{
		// Take the "field" dimension (cols) to be the largest number of fields
		// in any of our lines.
		int numCols = GetNumFieldsInLine(lineToCount);
		if(numCols > m_numCols)
			m_numCols = numCols;

		// Increment our row dimension for every line we obtain.
		m_numRows++;
	}

}

int LCSVParser::GetNumFieldsInLine(std::string &countMe)
{
	// Note: # of fields = # of commas in the line + 1.
	//---------------------------------------------------------

	int numCols = 0;

	// Let's try to get the position of the first comma in our line. -tmob
	int position = countMe.find(',');

	// If position is non-negative, we managed to find a comma.
	while (position >= 0)
	{
		// Tally a column for the comma we found.
		numCols++;
		// Reset our line.
		countMe = countMe.substr(position + 1);
		// And go ahead and find the next comma. -tmob
		position = countMe.find(',');
	}
	// We've accounted for all the commas, so now account for the final column.
	numCols++;

	// Spit back the number of columns in the line we were given.
	return numCols;
}


void LCSVParser::ParseLine(std::string &parseMe, int row)
{

	// Declare the necessary variables to hold our parsed data. -tmob
	std::string entry;
	m_parsedData[row] = new std::string[GetNumFields()];
	int numEntry = 0;

	// Let's try to get the position of the first comma in our line. -tmob
	int position = parseMe.find(',');
	int nonWhiteSpace = 0;

	// If position is non-negative, we managed to find a comma separator, so proceed
	// accordingly. -tmob
	while (position >= 0)
	{
		assert(numEntry <= GetNumFields());
		assert(row <= GetNumEntries());

		entry = parseMe.substr(0, position);

		// Take the substring of our line that ends at the location of our comma. -tmob
		nonWhiteSpace = entry.find_first_not_of(' ');

		// Check to see if we've got a blank entry.
		if(nonWhiteSpace == NPOS)
		{
			// If we have, push an empty string into our parsed data in
			// order to preserve proper ordering. -tmob
			m_parsedData[row][numEntry] = " ";
			numEntry++;
		}
		else
		{
			entry = entry.substr(nonWhiteSpace, position);
			// If we've got a non-empty entry, store it. -tmob
			m_parsedData[row][numEntry] = entry;
			// Update our entry index.
			numEntry++;
		}

		// Now reset parseMe to be the rest of our line to be parsed. -tmob
		parseMe = parseMe.substr(position + 1);
		// And go ahead and find the next comma. -tmob
		position = parseMe.find(',');
	}

	// Eat up any whitespace.
	nonWhiteSpace = parseMe.find_first_not_of(' ');
	// Now that we've eaten up all of the commas in our line, we are left to
	// parse the final column. -tmob

	// If all we have is whitespace, we've got a blank entry.
	if(nonWhiteSpace == NPOS)
	{
		m_parsedData[row][numEntry] = " ";
	}
	else
	{
		// Otherwise, we've got a non-empty field, so add it accordingly.
		parseMe = parseMe.substr(nonWhiteSpace, position);
		m_parsedData[row][numEntry] = parseMe;
	}
}
