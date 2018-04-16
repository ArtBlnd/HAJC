#include <HAJC/HAJCLog.h>
#include <vector>

std::vector<HAJC::Visitable*> g_LogCallbacks;

namespace HAJC {
    std::string ErrLevelToString(NodifyLevel level) {
        switch(level) {
        case NodifyLevel::DEBUG: 
            return "DEBUG";
        case NodifyLevel::NOTICE: 
            return "NOTICE";
        case NodifyLevel::WARNING:
            return "WARNING";
        case NodifyLevel::ERROR:
            return "ERROR";
        case NodifyLevel::FATAL_ERROR:
            return "FATAL_ERROR";
        }
    }

    void NodifyLog(const std::string& Output, NodifyLevel Level) {
        std::string Log;

        Log += "[";
        Log += ErrLevelToString(Level);
        Log += "] ";
        Log += Output;

        
    }
}