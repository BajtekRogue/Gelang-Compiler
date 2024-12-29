#ifndef NAME_SPACES_HPP
#define NAME_SPACES_HPP

#include <string>
#include <cinttypes>

/**
 * @brief Special memory addresses
 * 
 */
namespace Memory {
    inline const int64_t start = 1000;
    inline const int64_t arrayVariableAssign = 144;
    inline const int64_t constantsStart = 300;
    inline const int64_t one = 69;
    inline const int64_t returnMultiplication = 256;
    inline const int64_t returnDivision = 161;
    inline const int64_t multiplicationLeft = 11;
    inline const int64_t multiplicationRight = 12;
    inline const int64_t divisionLeft = 72;
    inline const int64_t divisionRight = 73;
    inline const int64_t divisionFlag = 75;
}

/**
 * @brief Label counters for loops and procedures
 */
namespace LabelCounters {
    inline int64_t ifCounter = 0;
    inline int64_t whileCounter = 0;
    inline int64_t repeatCounter = 0;
    inline int64_t forCounter = 0;
    inline int64_t procedureCounter = 0;
};

/**
 * @brief Flags for arithmetic operations
 */
namespace Arithmetic {
    inline bool multiplication = false;
    inline bool division = false;
    inline bool isOneNeeded = false;
    inline const std::string multiplicationProcedureName = "*$*";
    inline const std::string divisionProcedureName = "/@\\";
}

/**
 * @brief Colors for terminal output
 */
namespace Colors {
    inline const std::string reset   = "\033[1;0m";
    inline const std::string black   = "\033[1;30m";
    inline const std::string red     = "\033[1;31m";
    inline const std::string green   = "\033[1;32m";
    inline const std::string yellow  = "\033[1;33m";
    inline const std::string blue    = "\033[1;34m";
    inline const std::string magenta = "\033[1;35m";
    inline const std::string cyan    = "\033[1;36m";
    inline const std::string white   = "\033[1;37m";
}

#endif // NAME_SPACES_HPP

