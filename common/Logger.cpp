#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

#include "Logger.h"


//Logger* Logger::sLogger = NULL;
#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L
std::recursive_mutex Logger::sLogLock;
std::unique_ptr<Logger> Logger::sLogger{ nullptr };
#else
std::auto_ptr<Logger> Logger::sLogger( NULL );
#endif  // C++11


std::string Logger::GetCurrentDateTimeStr( const char* hour, const char* min, const char* sec )
{
    time_t t = ::time(NULL);  // get time now
    struct tm* now = localtime( &t );

    std::stringstream timeSStr; 

    timeSStr << (now->tm_year + 1900) << '-' 
             << std::setw(2) << std::setfill('0')
             << (now->tm_mon + 1) << '-'
             << std::setw(2) << std::setfill('0')
             <<  now->tm_mday << '_'
             << std::setw(2) << std::setfill('0')
             <<  now->tm_hour << hour
             << std::setw(2) << std::setfill('0')
             <<  now->tm_min << min
             << std::setw(2) << std::setfill('0')
             <<  now->tm_sec << sec;

    return timeSStr.str();
}


Logger::Logger(const std::string& logDir, const std::string& fileNamePrefix, 
               const std::string& fileNameExt) :
    mLogFileName(logDir + fileNamePrefix + GetCurrentDateTimeStr("h", "m", "s")
               + fileNameExt),
    mLogFile(mLogFileName.c_str())  // Create log file. Or do it in InitLog()?
{
    if( !mLogFile.is_open() ) 
        throw std::runtime_error(std::string("Error opening log file ") + mLogFileName);
}


Logger::~Logger()
{

    LOG("Closing log file '" << mLogFileName << "'");

    // Close log file
#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L
    std::lock_guard<std::recursive_mutex> guard(sLogLock);  // May recurse
#endif  // C++11
    if( mLogFile.is_open() )
        mLogFile.close();
}


void Logger::InitLog( const std::string& logDir, 
                      const std::string& fileNamePrefix,
                      const std::string& fileNameExt )
{
#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L
    std::lock_guard<std::recursive_mutex> guard(sLogLock);
#endif  // C++11
    sLogger.reset( new Logger(logDir, fileNamePrefix, fileNameExt) );  // May call the destructor
}


std::ofstream& Logger::GetLogFile()
{
#if 0
    static Logger logger;

    return logger.mLogFile;
#else
//  if ( sLogger == NULL )
//      sLogger = new Logger();

    if ( sLogger.get() == NULL )
        sLogger.reset(new Logger());

    return sLogger->mLogFile;
#endif /* 0 */
}


void Logger::Log( const std::string& text )
{
#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L
    std::lock_guard<std::recursive_mutex> guard(sLogLock);
#endif  // C++11
    std::ofstream& logFile = Logger::GetLogFile();

    logFile << text << std::endl;
}
