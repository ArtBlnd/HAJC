#ifndef HAJC_HAJC_LOG_H_
#define HAJC_HAJC_LOG_H_

#include <string>
#include <cstdint>

namespace HAJC {
    enum class NodifyLevel {
        DEBUG, NOTICE, WARNING, ERROR, FATAL_ERROR
    };

    void NodifyLog(const std::string& Output, NodifyLevel Level);
}

#endif