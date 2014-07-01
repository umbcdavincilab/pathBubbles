#ifndef LCSVPARSER_H
#define LCSVPARSER_H

#include <string>
#include <fstream>

/*
read the information in the .csv file
*/

class LCSVParser
{
public:

	LCSVParser(const char *csvFile);
	virtual ~LCSVParser();

	void Parse();

	// Returns the total number of fields (columns) in the .csv file.
	int GetNumFields();

	// Returns the total number of line entries (rows) in the .csv file.
	int GetNumEntries();

	// Given the name of a field, returns the field's column index.
	int GetFieldIndexByName(const std::string &fieldName);

	// Stuffs the requested data into string &entryData.
	// Return false if there is no data to be had at the given field or index.
	bool GetDataEntryInField(const std::string &fieldName, int entryIndex, std::string &entryData);
	bool GetDataEntryInField(int entryIndex, int fieldIndex, std::string &entryData);

private:
	void ParseLine(std::string &parseMe, int row);
	void GetDimensions();
	int GetNumFieldsInLine(std::string &countMe);

	std::ifstream m_inDataFile;
	std::string** m_parsedData;
	int m_numRows;
	int m_numCols;
};

#endif // LCSVPARSER_H
