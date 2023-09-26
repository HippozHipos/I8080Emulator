#pragma once
#pragma warning (disable : 4996)


#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <mutex>
#include <fstream>

//Logger Class

class Logger final
{
public:

	//For Setting defualt log type, or an indiviudal log with custom format
	enum TimeFormat
	{
		Date,
		Time,
		DateTime,
		None
	};

	//For specifying the severity of the logging infomation
	enum LogType
	{
		Info,
		Error,
		Warning,
	};

public:

	//Default Constructer creates logger at current file path of the exe
	Logger(TimeFormat format = TimeFormat::DateTime);

	//Specified Path for log file, with default date time logging
	Logger(std::string fpath, TimeFormat format = TimeFormat::DateTime);
	Logger(const char* fpath, TimeFormat format = TimeFormat::DateTime);

	virtual ~Logger();



public:

	/*
	 * For setting a new deafult logging date time format
	 */
	void SetDefaultTimeFormat(TimeFormat newFormat);


	/*
	 * A logtype and or timetype overide can be specified from the deafult set. Otherwise logs are logged using the default infomation.
	 * Returns a bool of whether it sucseeded or not
	 */

	 // Equates to passing in LogType::Info
	bool Write(const std::string& Message);
	// Allows for passing custom Severity Level for log
	bool Write(const std::string& Message, LogType Severity);
	// Log with fully custom parameters (does not make use of the default)
	bool CustomWrite(const std::string& Message, LogType Severity, TimeFormat Format);



private:
	std::mutex m_FileLock;
	TimeFormat m_DeafultFormat;
	std::ofstream m_LogFile;

public:
	std::string GetDateTimeFormatted(TimeFormat dt);

	static std::string GetLogTypePrefix(const LogType type);

	static std::string GetCurrentWorkingDirectory();
};


#endif