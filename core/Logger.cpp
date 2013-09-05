#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

#include "Logger.h"


//Logger* Logger::sLogger = NULL;
std::auto_ptr<Logger> Logger::sLogger( NULL );


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
    mLogDir(logDir),
    mFileNamePrefix(fileNamePrefix),
    mFileNameExt(fileNameExt)
{
    // Create log file

    std::string logFileName = mLogDir + mFileNamePrefix 
                            + GetCurrentDateTimeStr("h", "m", "s") 
                            + mFileNameExt;

    mLogFile.open(logFileName.c_str());
    if( !mLogFile.is_open() ) 
        throw std::runtime_error(std::string("Error opening log file ") + logFileName);
}


Logger::~Logger()
{
    // Close log file

    LOG("Closing log file.");

    mLogFile.close();
}


void Logger::InitLog( const std::string& logDir, 
                      const std::string& fileNamePrefix,
                      const std::string& fileNameExt )
{
    sLogger.reset( new Logger(logDir, fileNamePrefix, fileNameExt) );
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
    std::ofstream& logFile = Logger::GetLogFile();

    logFile << text << std::endl;
}
