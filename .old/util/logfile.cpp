/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: logfile.cpp                                                   $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/logfile.cpp 6     8/10/04 12:36p Jamie             $ *
* $NoKeywords: $                                                              *
******************************************************************************/

/** \file logfile.cpp
 */

#include "logfile.hpp"

namespace util {

//-----------------------------------------------------------------------------

LogFile::LogFile()
  : m_loggerName("No Logging"), m_logFolder(), m_logFileName(),
    m_maxLogLevel(noLogging), m_useStdOut(false), m_ofs(), m_os(m_ofs)
{
}

LogFile::LogFile(std::string const& loggerName,
                 LogLevel maxLogLevel)
  : m_loggerName(loggerName), m_logFolder(), m_logFileName(),
    m_maxLogLevel(maxLogLevel), m_useStdOut(true), m_ofs(), m_os(std::cout)
{
    if(m_maxLogLevel==noLogging)
    {
        return;
    }

    std::cout << "The " << m_loggerName
            << " log output on standard out has been Started at ";

    _strdate(m_sysDate);
    _strtime(m_sysTime);

    std::cout << m_sysTime << " (24hr) on " << m_sysDate << " (mm/dd/yy)\n\n" << std::flush;
}

LogFile::LogFile(std::string const& loggerName,
                 std::string const& logFolder, std::string const& logFileName,
                 LogLevel maxLogLevel)
  : m_loggerName(loggerName), m_logFolder(logFolder), m_logFileName(logFileName),
    m_maxLogLevel(maxLogLevel), m_useStdOut(false), m_ofs(), m_os(m_ofs)
{
    if(m_maxLogLevel==noLogging)
    {
        return;
    }

    std::string fileName = logFolder + logFileName;

    if(fileName.empty())
    {
        THROW_EXC2(INVALID_PARAM, "LogFile::LogFile() empty file name.")
    }

    m_ofs.open(fileName.c_str());
    if(!m_ofs.good())
    {
        THROW_EXC2(INVALID_PARAM, "LogFile::LogFile() can't open file at " + fileName)
    }

    _strdate(m_sysDate);
    _strtime(m_sysTime);
    m_ofs << "The " << m_loggerName << " log file " << fileName
          << " has been Started at " << m_sysTime << " (24hr) on "
          << m_sysDate << " (mm/dd/yy)\n\n" << std::flush;
}

//-----------------------------------------------------------------------------

LogFile::~LogFile()
{
    if(m_useStdOut)
    {
        Header();
        m_os << "The Log on standard out has been Closed\n" << std::flush;
    }
    else if(m_ofs && m_ofs.is_open() && m_ofs.good())
    {
        Header();
        m_os << "The Log File has been Closed\n" << std::flush;
        m_ofs.close();
    }
}

//-----------------------------------------------------------------------------

void LogFile::LogMsg(LogLevel logLevel, std::string const& msg, bool writeHeader)
{
    if(ContinueCheck(logLevel, writeHeader))
    {
        m_os << msg << std::flush;
    }
}

//-----------------------------------------------------------------------------

bool LogFile::ContinueCheck(LogLevel logLevel, bool writeHeader)
{
    if(m_maxLogLevel==noLogging || logLevel>m_maxLogLevel)
    {
        return false;
    }

    if(!m_useStdOut && m_ofs.bad())
    {
        THROW_EXC("LogFile::LogMsg() can't access log file")
    }

    if(writeHeader)
    {
        LogFile::Header();
    }

    return true;
}

void LogFile::Header()
{
    _strdate(m_sysDate);
    _strtime(m_sysTime);
    m_os << "\n\n[" << m_sysDate << ' ' << m_sysTime << "] ";
}

//-----------------------------------------------------------------------------

} // namespace

//** eof **********************************************************************
