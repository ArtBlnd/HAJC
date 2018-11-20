#ifndef __HAJC_TOOLS_DEBUG_H__
#define __HAJC_TOOLS_DEBUG_H__

#include <iostream>

#ifdef __HAJC_DEBUG__
#define CONTRACT_CALL_TRACE     \
    HAJC::TraceContext __call_trace_context_def(__FILE__, __PRETTY_FUNCTION__, __LINE__)
#define noway_assert(cond, message) \
    if(cond) do {                   \
        AssertMessage(message);     \
    } while(false)
#else
#define CONTRACT_CALL_TRACE
#define noway_assert(cond, message)
#endif

namespace HAJC
{
    void SetLogOutput(std::ostream& stream);
    void logf(const char* message);

    template <class... ArgsTy>
    void logf(const char* message, ArgsTy... args)
    {
        char messageBuffer[512] = {0, };
        snprintf(messageBuffer, 512, message, args...);
        logf(messageBuffer);
    }

    struct TraceContext
    {
        std::string file;
        std::string func;
        unsigned int line;

        TraceContext(char* file, char* func, unsigned int line) noexcept;
        ~TraceContext();
    };

    void DumpTrace();
    void AssertMessage(const std::string message);

} // namespace HAJC


#endif