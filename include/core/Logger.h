#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <sstream>
#include <ctime>
#include <iomanip>

namespace ExperimentRedbear {

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger {
public:
    static Logger& getInstance();

    void setLogFile(const std::string& filename);
    void setLogLevel(LogLevel level);
    void setConsoleOutput(bool enabled);

    void log(LogLevel level, const char* file, int line, const char* function, const std::string& message);

    static std::string levelToString(LogLevel level);
    static std::string getCurrentTimestamp();

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream m_logFile;
    LogLevel m_minLevel = LogLevel::TRACE;
    bool m_consoleOutput = true;
    std::mutex m_mutex;
};

// Macros for easy logging
#define LOG_TRACE(msg) ExperimentRedbear::Logger::getInstance().log(ExperimentRedbear::LogLevel::TRACE, __FILE__, __LINE__, __FUNCTION__, msg)
#define LOG_DEBUG(msg) ExperimentRedbear::Logger::getInstance().log(ExperimentRedbear::LogLevel::DEBUG, __FILE__, __LINE__, __FUNCTION__, msg)
#define LOG_INFO(msg) ExperimentRedbear::Logger::getInstance().log(ExperimentRedbear::LogLevel::INFO, __FILE__, __LINE__, __FUNCTION__, msg)
#define LOG_WARNING(msg) ExperimentRedbear::Logger::getInstance().log(ExperimentRedbear::LogLevel::WARNING, __FILE__, __LINE__, __FUNCTION__, msg)
#define LOG_ERROR(msg) ExperimentRedbear::Logger::getInstance().log(ExperimentRedbear::LogLevel::ERROR, __FILE__, __LINE__, __FUNCTION__, msg)
#define LOG_FATAL(msg) ExperimentRedbear::Logger::getInstance().log(ExperimentRedbear::LogLevel::FATAL, __FILE__, __LINE__, __FUNCTION__, msg)

#define LOG_TRACE_S(ss) ExperimentRedbear::Logger::getInstance().log(ExperimentRedbear::LogLevel::TRACE, __FILE__, __LINE__, __FUNCTION__, ss.str())
#define LOG_DEBUG_S(ss) ExperimentRedbear::Logger::getInstance().log(ExperimentRedbear::LogLevel::DEBUG, __FILE__, __LINE__, __FUNCTION__, ss.str())
#define LOG_INFO_S(ss) ExperimentRedbear::Logger::getInstance().log(ExperimentRedbear::LogLevel::INFO, __FILE__, __LINE__, __FUNCTION__, ss.str())
#define LOG_WARNING_S(ss) ExperimentRedbear::Logger::getInstance().log(ExperimentRedbear::LogLevel::WARNING, __FILE__, __LINE__, __FUNCTION__, ss.str())
#define LOG_ERROR_S(ss) ExperimentRedbear::Logger::getInstance().log(ExperimentRedbear::LogLevel::ERROR, __FILE__, __LINE__, __FUNCTION__, ss.str())
#define LOG_FATAL_S(ss) ExperimentRedbear::Logger::getInstance().log(ExperimentRedbear::LogLevel::FATAL, __FILE__, __LINE__, __FUNCTION__, ss.str())

} // namespace ExperimentRedbear
