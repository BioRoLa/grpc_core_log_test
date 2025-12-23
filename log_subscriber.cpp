#include "NodeHandler.h"
#include "Log.pb.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

std::mutex log_mutex;

// Track last sequence number to avoid duplicate outputs
struct LastMessage {
    uint32_t seq = 0;
} last_message;

// Convert log level to string
std::string logLevelToString(log_msg::LogLevel level) {
    switch(level) {
        case log_msg::LogLevel::DEBUG: return "DEBUG";
        case log_msg::LogLevel::INFO:  return "INFO";
        case log_msg::LogLevel::WARN:  return "WARN";
        case log_msg::LogLevel::ERROR: return "ERROR";
        case log_msg::LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

// Get color for log level
const char* getColorForLevel(log_msg::LogLevel level) {
    switch(level) {
        case log_msg::LogLevel::DEBUG: return BLUE;
        case log_msg::LogLevel::INFO:  return GREEN;
        case log_msg::LogLevel::WARN:  return YELLOW;
        case log_msg::LogLevel::ERROR: return RED;
        case log_msg::LogLevel::FATAL: return BOLD RED;
        default: return WHITE;
    }
}

// Format timestamp
std::string formatTimestamp(int64_t sec, int32_t usec) {
    std::time_t time = static_cast<std::time_t>(sec);
    std::tm* tm_info = std::localtime(&time);
    
    std::stringstream ss;
    ss << std::put_time(tm_info, "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(6) << usec;
    return ss.str();
}

// Callback function for log messages
void logCallback(log_msg::LogEntry log_entry) {
    std::lock_guard<std::mutex> lock(log_mutex);
    
    // Get current timestamp and sequence number
    int64_t current_sec = log_entry.header().stamp().sec();
    int32_t current_usec = log_entry.header().stamp().usec();
    uint32_t current_seq = log_entry.header().seq();
    
    // Skip if sequence is the same or older (duplicate message)
    if (current_seq <= last_message.seq && current_seq != 0) {
        return;
    }
    
    // Update last sequence number
    last_message.seq = current_seq;
    
    // Get color based on log level
    const char* color = getColorForLevel(log_entry.level());
    
    // Format timestamp
    std::string timestamp = formatTimestamp(current_sec, current_usec);
    
    // Format log level with fixed width
    std::string level_str = logLevelToString(log_entry.level());
    
    // Print formatted log message with color
    std::cout << color 
              << "[" << timestamp << "] "
              << "[" << std::setw(5) << std::left << level_str << "] "
              << "[" << log_entry.node_name() << "] "
              << log_entry.message()
              << RESET << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << GREEN << "==================================================" << RESET << std::endl;
    std::cout << GREEN << "  Log Test Subscriber - Listening to /log topic" << RESET << std::endl;
    std::cout << GREEN << "==================================================" << RESET << std::endl;
    std::cout << std::endl;
    
    // Initialize node handler
    core::NodeHandler nh;
    
    // Subscribe to log topic with larger queue size to avoid missing messages
    // Increased from 1000 to 10000 for high-frequency log updates
    core::Subscriber<log_msg::LogEntry>& log_sub = 
        nh.subscribe<log_msg::LogEntry>("/log", 10000, logCallback, 100);
    
    std::cout << CYAN << "Subscribed to /log topic. Waiting for messages..." << RESET << std::endl;
    std::cout << std::endl;
    
    // Print color legend
    std::cout << "Log Level Colors:" << std::endl;
    std::cout << BLUE << "  DEBUG" << RESET << " - Debug information" << std::endl;
    std::cout << GREEN << "  INFO " << RESET << " - Informational messages" << std::endl;
    std::cout << YELLOW << "  WARN " << RESET << " - Warning messages" << std::endl;
    std::cout << RED << "  ERROR" << RESET << " - Error messages" << std::endl;
    std::cout << BOLD RED << "  FATAL" << RESET << " - Fatal error messages" << std::endl;
    std::cout << std::endl;
    std::cout << "Press Ctrl+C to exit" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << std::endl;
    
    // Spin and process incoming messages at high frequency
    // Increased from 100 Hz to 1000 Hz to handle fast log updates
    core::Rate rate(1000);
    while (true) {
        core::spinOnce();
        rate.sleep();
    }
    
    return 0;
}
