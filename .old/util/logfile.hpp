/******************************************************************************
* Copyright © 2004 Acceleware Inc.                                            *
*                                                                             *
* $Workfile:: logfile.hpp                                                   $ *
*-----------------------------------------------------------------------------*
* $Header:: /common/util/logfile.hpp 10    8/10/04 7:22p Jamie              $ *
* $NoKeywords: $                                                              *
******************************************************************************/

#ifndef UTIL_LOGFILE_HPP_INCLUDED
#define UTIL_LOGFILE_HPP_INCLUDED

#include "../types/types.hpp"
#include "array.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

namespace util {
     
/** \class LogFile
 *  \brief Log File Writer.
 *
 *  This log file class will write to a text file specified in the constructor.
 *  The LogLevel enumerated type controls the operation of this class. If the
 *  maxLogLevel passed into the constructor is noLogging, then this object will
 *  not open a file, and will "ignore" all calls to LogMsg. Any other value of
 *  maxLogLevel will cause a file to be opened (or an exception thrown if the
 *  filename is invalid or write-protected). Subsequent LogMsg calls will be
 *  appended to the open file iff the logLevel of the message is <= to
 *  maxLogLevel.
 *
 *  For example, a compile-time call to LogMsg(debugAlways, "My Message") will
 *  be ignored if at run-time the constructor is passed a maxLogLevel of
 *  userVerbose.
 *
 *  Future implementation:
 *  maxLogFileSize will determine at what size (in kB) the file will be closed,
 *  and a new file opened. maxLogFiles will define how many files will be
 *  created, before the oldest files start getting deleted.
 */
class LogFile
{

    NO_GEN(LogFile)

public:

    enum LogLevel
    {
        noLogging,
        userNormal,
        userVerbose,
        debugAlways,
        debugVerbose
    };

    ///PrintFmt is used with logArray, to determine which axis orientation
    ///  will be used to print the values.
    enum PrintFmt
    {
        defaultFmt,
        sliceX,
        sliceY,
        sliceZ,
        allIncrease
    };

    #define EXC_IDS INVALID_PARAM
    DECLARE_EXC_ROOT
    #undef EXC_IDS

    ///Default constructor that will ignore all logging commands.
    LogFile();

    ///Standard Out Constructor (uses cout).
    LogFile(std::string const& loggerName,
            LogLevel maxLogLevel=debugAlways);

    ///Main Constructor, to output to a text file.
    LogFile(std::string const& loggerName,
            std::string const& logFolder, std::string const& logFileName,
            LogLevel maxLogLevel=debugAlways);

    virtual ~LogFile();

    ///Send a message to the log file, if the logLevel of the message
    /// is <= to maxLogLevel. By default, a header containing the
    /// current date and time will output before the message string.
    void LogMsg(LogLevel logLevel, std::string const& msg,
                bool writeHeader=true);

    ///Similar to LogMsg, these templates for 1D, 2D and 3D arrays will
    /// work for any such array of objects that know how to stream
    /// themselves; The built-in types know how to stream themselves,
    /// but you will need to implement operator<< for any custom types.
    template<class T>
    void LogArray(LogLevel logLevel, util::array1d<T> const& array,
                  bool writeHeader=true, uint colWidth=0)
    {
        if(ContinueCheck(logLevel, writeHeader))
        {
            m_os << std::endl << array << std::flush;
        }
    }

    ///See LogArray 1D
    template<class T>
    void LogArray(LogLevel logLevel, util::array2d<T> const& array,
                  bool writeHeader=true, uint colWidth=0,
                  PrintFmt printFmt=defaultFmt)
    {
        if(ContinueCheck(logLevel, writeHeader))
        {
            m_os << array << std::flush;
        }
    }

    ///Similar to LogMsg, this template for 3D arrays will
    /// work for any array of objects that know how to stream
    /// themselves; The built-in types know how to stream themselves,
    /// but you will need to implement operator<< for any custom types.
    ///If the colWidth parameter is zero, commas will separate the values.
    ///If sliceNum is true, the index number will be printed before each slice.
    ///If rowNum is true, the index number will be printed at the start of each line.
    template<class T>
    void LogArray(LogLevel logLevel, util::array3d<T> const& array,
                  bool writeHeader=true, uint colWidth=0,
                  PrintFmt printFmt=defaultFmt,
                  bool sliceNum=true, bool rowNum=true)
    {
        if(ContinueCheck(logLevel, writeHeader))
        {
            switch (printFmt)
            {
                case sliceX:
                    SliceX(array, writeHeader, colWidth, sliceNum, rowNum);
                    break;
                case sliceY:
                    SliceY(array, writeHeader, colWidth, sliceNum, rowNum);
                    break;
                case sliceZ:
                    SliceZ(array, writeHeader, colWidth, sliceNum, rowNum);
                    break;
                case allIncrease:
                    AllIncrease(array, writeHeader, colWidth, sliceNum, rowNum);
                    break;
                default:
                    m_os << array;
            }//switch
            m_os << std::flush;
        }//if_ContinueCheck
    }

private:
    std::string const m_loggerName;
    std::string const m_logFolder;
    std::string const m_logFileName;
    LogLevel const m_maxLogLevel;
    bool const m_useStdOut;

    ///Output File Stream - the workhorse of this class.
    std::ofstream m_ofs;
    ///Reference to the output stream being used; either m_ofs or std::cout
    std::ostream& m_os;

    ///Temporary storage for Date-Time info, to avoid repeated new-delete cycles.
    char m_sysDate[9];
    char m_sysTime[9];

    ///Private f'n to eliminate code duplication for routine checks.
    bool ContinueCheck(LogLevel logLevel, bool writeHeader);

    ///Private f'n to prepend Date-Time info to each log entry.
    void Header();


    template<class T>
    void SliceX(util::array3d<T> const& array, bool writeHeader, uint colWidth,
                bool sliceNum, bool rowNum)
    {
        if(writeHeader)
            m_os << "3D array sliced in X (decreasing X blocks, decreasing Z rows, increasing Y columns):";

        //This calculation is slow, so do it before the loop.
        uint rowNumWidth;
        if(rowNum)
        {
            std::string sizeString = sformat("%u",array.size3());
            rowNumWidth = sizeString.size();
        }

        for(uint x=array.size1(); x>0; x--)
        {
            m_os << std::endl;
            if(sliceNum)
                m_os << std::endl << "--- X=" << x-1 << " ---";
            for(uint z=array.size3(); z>0; z--)
            {
                m_os << std::endl;
                if(rowNum)
                {
                    m_os << "Z=";
                    m_os.width(rowNumWidth);
                    m_os << z-1 << ":";
                }
                for(uint y=0; y<array.size2(); y++)
                {
                    if(colWidth>0)
                        m_os.width(colWidth);
                    else
                        if(y>0)
                            m_os << ',';
                    //The decrementing indexes need to be one higher, otherwise
                    // the for-loop will decrement the uint below zero.
                    m_os << array(x-1,y,z-1);
                }
            }
        }
    }

    template<class T>
    void SliceY(util::array3d<T> const& array, bool writeHeader, uint colWidth,
                bool sliceNum, bool rowNum)
    {
        if(writeHeader)
            m_os << "3D array sliced in Y (decreasing Y blocks, decreasing X rows, increasing Z columns):";

        //This calculation is slow, so do it before the loop.
        uint rowNumWidth;
        if(rowNum)
        {
            std::string sizeString = sformat("%u",array.size1());
            rowNumWidth = sizeString.size();
        }

        for(uint y=array.size2(); y>0; y--)
        {
            m_os << std::endl;
            if(sliceNum)
                m_os << std::endl << "--- Y=" << y-1 << " ---";
            for(uint x=array.size1(); x>0; x--)
            {
                m_os << std::endl;
                if(rowNum)
                {
                    m_os << "X=";
                    m_os.width(rowNumWidth);
                    m_os << x-1 << ":";
                }
                for(uint z=0; z<array.size3(); z++)
                {
                    if(colWidth>0)
                        m_os.width(colWidth);
                    else
                        if(z>0)
                            m_os << ',';
                    //The decrementing indexes need to be one higher, otherwise
                    // the for-loop will decrement the uint below zero.
                    m_os << array(x-1,y-1,z);
                }
            }
        }
    }

    template<class T>
    void SliceZ(util::array3d<T> const& array, bool writeHeader, uint colWidth,
                bool sliceNum, bool rowNum)
    {
        if(writeHeader)
            m_os << "3D array sliced in Z (decreasing Z blocks, decreasing Y rows, increasing X columns):";

        //This calculation is slow, so do it before the loop.
        uint rowNumWidth;
        if(rowNum)
        {
            std::string sizeString = sformat("%u",array.size2());
            rowNumWidth = sizeString.size();
        }

        for(uint z=array.size3(); z>0; z--)
        {
            m_os << std::endl;
            if(sliceNum)
                m_os << std::endl << "--- Z=" << z-1 << " ---";
            for(uint y=array.size2(); y>0; y--)
            {
                m_os << std::endl;
                if(rowNum)
                {
                    m_os << "Y=";
                    m_os.width(rowNumWidth);
                    m_os << y-1 << ":";
                }
                for(uint x=0; x<array.size1(); x++)
                {
                    if(colWidth>0)
                        m_os.width(colWidth);
                    else
                        if(x>0)
                            m_os << ',';
                    //The decrementing indexes need to be one higher, otherwise
                    // the for-loop will decrement the uint below zero.
                    m_os << array(x,y-1,z-1);
                }
            }
        }
    }

    template<class T>
    void AllIncrease(util::array3d<T> const& array, bool writeHeader, uint colWidth,
                bool sliceNum, bool rowNum)
    {
        if(writeHeader)
            m_os << "3D array All Increasing (increasing X blocks, increasing Y rows, increasing Z columns):";

        //This calculation is slow, so do it before the loop.
        uint rowNumWidth;
        if(rowNum)
        {
            std::string sizeString = sformat("%u",array.size2());
            rowNumWidth = sizeString.size();
        }

        for(uint x=0; x<array.size1(); x++)
        {
            m_os << std::endl;
            if(sliceNum)
                m_os << std::endl << "--- X=" << x << " ---";
            for(uint y=0; y<array.size2(); y++)
            {
                m_os << std::endl;
                if(rowNum)
                {
                    m_os << "Y=";
                    m_os.width(rowNumWidth);
                    m_os << y << ":";
                }
                for(uint z=0; z<array.size3(); z++)
                {
                    if(colWidth>0)
                        m_os.width(colWidth);
                    else
                        if(z>0)
                            m_os << ',';
                    m_os << array(x,y,z);
                }
            }
        }
    }
};

//-----------------------------------------------------------------------------

} // namespace

#endif

//** eof **********************************************************************
