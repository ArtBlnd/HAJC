#include <language/AST.h>
#include <language/Parser.h>

#include <tools/Argument.h>
#include <tools/Debug.h>
#include <tools/FileContext.h>

#include <vector>
#include <string>
#include <fstream>

using namespace HAJC;

inline bool InitExec(unsigned int args, char** argv)
{
    noway_assert(argv == nullptr, "program argument cannot be nullptr!");

    {
        // This will check that has more than one argument.
        // We have to pass one program argument at least for output file.

        std::vector<std::string> arguments;
        for(unsigned int i = 2; i < args; ++i)
        {
            arguments.push_back(std::string(argv[i]));
        }

        if(args <= 1 && !Argument::Init(arguments))
        {
            Argument::Show();
            return false;
        }
    }

    return true;
}


// Option table descriptions
//      Those will be passed with program arguments by --key=value
//      You can use it by using option variable.
namespace Options
{
    //==============================================================================
    //               | option variable | option key | description                  |
    DEFINE_ARGUMENT_S(logFile          , log-file   , "Set log filename as option");
    DEFINE_ARGUMENT_S(outFile          , out-file   , "Set out filename as option");
}


bool MainExec(const std::string& filename)
{
    


    ParseContext context;
    if(!psCreateContext(&context, filename))
    {
        return false;
    }

    psTokenlizeContext(&context);
    psDeleteContext(&context);

    return true;
}

int main(int args, char** argv, char** env)
{
    CONTRACT_CALL_TRACE;
    
    if(!InitExec(args, argv))
    {
        dbgLogOutput("no source file targeted!\n");
        dbgLogOutput("usage : %s filename arguments...\n", argv[0]);
        return -1;
    }

    return MainExec(argv[1]);
}