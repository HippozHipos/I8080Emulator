#include "Util.h"
#include "Logger.h"

#include <windows.h>
#include <algorithm>


Logger::Logger(TimeFormat format)
{
    m_DeafultFormat = format;
    std::string default_path = GetCurrentWorkingDirectory();
    std::string time_stamp = GetDateTimeFormatted(TimeFormat::DateTime);
    std::replace(time_stamp.begin(), time_stamp.end(), ':', '-');

    default_path += "\\" + time_stamp + ".txt";
    m_LogFile.open(default_path);

    if (!m_LogFile.is_open())
        throw std::runtime_error("Logging Failed: Filepath Not Found");

    Write("Debug Log Initialized", LogType::Info);
}


Logger::Logger(std::string fpath, TimeFormat format)
{
    m_LogFile.open(fpath);

    if (!m_LogFile.is_open())
        throw std::runtime_error("Logging Failed: Filepath Not Found");

    Write("Debug Log Initialized", LogType::Info);
}

Logger::Logger(const char* fpath, TimeFormat format)
{
    m_LogFile.open(fpath);

    if (!m_LogFile.is_open())
        throw std::runtime_error("Logging Failed: Filepath Not Found");

    Write("Debug Log Initialized", LogType::Info);
}

Logger::~Logger()
{
    if (m_LogFile.is_open())
    {
        m_LogFile.close();
    }

    return;
}

void Logger::SetDefaultTimeFormat(TimeFormat newFormat)
{
    m_DeafultFormat = newFormat;

    if (m_LogFile.is_open())
    {
        Write("Deafult Time Formatting Updated", LogType::Info);
    }

    return;
}

bool Logger::Write(const std::string& Message)
{
    if (!m_LogFile.is_open()) return false;

    const std::string TypePrefix = GetLogTypePrefix(LogType::Info);
    const std::string TimePrefix = GetDateTimeFormatted(m_DeafultFormat);

    std::lock_guard<std::mutex> guard(m_FileLock);

    m_LogFile << TypePrefix + TimePrefix + ": " + Message << std::endl;

    return true;
}

bool Logger::Write(const std::string& Message, LogType Severity)
{
    if (!m_LogFile.is_open()) return false;

    const std::string TypePrefix = GetLogTypePrefix(Severity);
    const std::string TimePrefix = GetDateTimeFormatted(m_DeafultFormat);

    std::lock_guard<std::mutex> guard(m_FileLock);

    m_LogFile << TypePrefix + TimePrefix + ": " + Message << std::endl;

    return true;
}

bool Logger::CustomWrite(const std::string& Message, LogType Severity, TimeFormat Format)
{
    if (!m_LogFile.is_open()) return false;

    const std::string TypePrefix = GetLogTypePrefix(Severity);
    const std::string TimePrefix = GetDateTimeFormatted(Format);

    std::lock_guard<std::mutex> guard(m_FileLock);

    m_LogFile << TypePrefix + TimePrefix + ": " + Message << std::endl;

    return true;
}


std::string Logger::GetDateTimeFormatted(TimeFormat dt)
{
    time_t curTime = time(NULL);
    struct tm tstruct;
    char buffer[80];
    tstruct = *(localtime(&curTime));

    switch (dt) {
    case TimeFormat::Time:
        strftime(buffer, sizeof(buffer), "[%X]", &tstruct);
        break;

    case TimeFormat::Date:
        strftime(buffer, sizeof(buffer), "[%Y-%m-%d]", &tstruct);
        break;

    case TimeFormat::DateTime:
        strftime(buffer, sizeof(buffer), "[%Y-%m-%d %X]", &tstruct);
        break;

    case TimeFormat::None:
        return "[*]";

    default:
        return "[TIME FORMAT ERROR]";
        break;
    }
    return std::string(buffer);
}

std::string Logger::GetLogTypePrefix(const LogType type)
{
    switch (type)
    {
    case LogType::Info:
        return "[INFO]";
    case LogType::Warning:
        return "[WARNING]";
    case LogType::Error:
        return "[ERROR]";
    default:
        return "";
    }
}


std::string Logger::GetCurrentWorkingDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}