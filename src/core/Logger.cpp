#include "core/Logger.h"
#include <iostream>
#include <sstream>
#include <iomanip>

namespace ExperimentRedbear {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : m_minLevel(LogLevel::TRACE), m_consoleOutput(true) {}

Logger::~Logger() {
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
    m_logFile.open(filename, std::ios::out | std::ios::app);
}

void Logger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_minLevel = level;
}

void Logger::setConsoleOutput(bool enabled) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_consoleOutput = enabled;
}

void Logger::log(LogLevel level, const char* file, int line, const char* function, const std::string& message) {
    if (level < m_minLevel) return;

    std::lock_guard<std::mutex> lock(m_mutex);

    std::ostringstream oss;
    oss << "[" << getCurrentTimestamp() << "] ";
    oss << "[" << levelToString(level) << "] ";
    
    // Extract filename from path
    std::string filename = file;
    size_t pos = filename.find_last_of("/\\");
    if (pos != std::string::npos) {
        filename = filename.substr(pos + 1);
    }
    
    oss << "[" << filename << ":" << line << "] ";
    oss << "[" << function << "] ";
    oss << message << "\n";

    std::string logLine = oss.str();

    if (m_consoleOutput) {
        // Color codes for console
        const char* colorCode = "";
        switch (level) {
            case LogLevel::TRACE:   colorCode = "\033[37m"; break;     // White
            case LogLevel::DEBUG:   colorCode = "\033[36m"; break;     // Cyan
            case LogLevel::INFO:    colorCode = "\033[32m"; break;     // Green
            case LogLevel::WARNING: colorCode = "\033[33m"; break;     // Yellow
            case LogLevel::ERROR:   colorCode = "\033[31m"; break;     // Red
            case LogLevel::FATAL:   colorCode = "\033[35m"; break;     // Magenta
        }
        std::cout << colorCode << logLine << "\033[0m";
    }

    if (m_logFile.is_open()) {
        m_logFile << logLine;
        m_logFile.flush();
    }
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE:   return "TRACE";
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO ";
        case LogLevel::WARNING: return "WARN ";
        case LogLevel::ERROR:   return "ERROR";
        case LogLevel::FATAL:   return "FATAL";
        default: return "UNKNOWN";
    }
}

std::string Logger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

} // namespace ExperimentRedbear
