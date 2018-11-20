#include <tools/Debug.h>

#include <vector>
#include <cstdio>
#include <cstdlib>

namespace HAJC
{
    std::ostream*              dbgOutput     = &std::cout;
    std::vector<TraceContext*> dbgTraceTable;

    void SetLogOutput(std::ostream& stream)
    {
        dbgOutput = &stream;
    }

    void logf(const char* message)
    {
        std::ostream& outstream = *dbgOutput;
        outstream << message;
    }

    TraceContext::TraceContext(char* file, char* func, unsigned int line) noexcept
    {
        this->file = file;
        this->func = func;
        this->line = line;

        dbgTraceTable.push_back(this);
    }

    TraceContext::~TraceContext()
    {
        dbgTraceTable.pop_back();
    }

    void DumpTrace()
    {
        logf("======TRACE DUMP====== (Most recent call last)\n");
        for(TraceContext* context : dbgTraceTable)
        {
            printf("%s : %s\n", context->file.c_str(), context->func.c_str());
        }
        logf("======================\n");
    }

    void AssertMessage(const std::string message)
    {
        logf("Assertion Error : %s\n\n", message);
        DumpTrace();

        exit(EXIT_FAILURE);
    }
} // namespace HAJC

