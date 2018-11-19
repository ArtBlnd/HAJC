#include <language/AST.h>
#include <language/Parser.h>

#include <tools/Argument.h>
#include <tools/Debug.h>
#include <tools/FileContext.h>

#include <vector>
#include <string>
#include <fstream>

using namespace HAJC;


// Option table descriptions
//      Those will be passed with program arguments by --key=value
//      You can use it by using option variable.
namespace OptionTable
{
    //===========================================================================================================================
    //               | option variable | option key       | description                                                         |
    DEFINE_ARGUMENT_S(LogFilename      , log-file         , "Set log filename as option");
    DEFINE_ARGUMENT_B(EmitAstOnly      , emit-ast-only    , "Do not compile as a binary, only emit AST only.");
    DEFINE_ARGUMENT_S(EmitAstFilename  , emit-ast-filename, "Target AST output filename. (this only work with --emit-ast-only)");
    DEFINE_ARGUMENT_B(EmitTokenOnly    , emit-token-only  , "Do not compile as a binary, only emit tokens only.");
}


inline bool InitExec(unsigned int args, char** argv)
{
    noway_assert(argv == nullptr, "program argument cannot be nullptr!");

    {
        // This will check that has more than one argument.
        // We have to pass one program argument at least for output file.

        std::vector<std::string> arguments;
        for(unsigned int i = 1; i < args; ++i)
        {
            arguments.push_back(std::string(argv[i]));
        }

        if(args <= 1 || !Argument::Init(arguments))
        {
            printf("Arguments :\n");
            Argument::Show();
            printf("\n\n");
            return false;
        }
    }

    return true;
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
        dbgLogOutput("no source file targeted or wrong argument found! \n");
        dbgLogOutput("usage : %s filename arguments...\n", argv[0]);
        return -1;
    }

    std::fstream logFile;
    if(!OptionTable::LogFilename->empty())
    {
        logFile = std::fstream(OptionTable::LogFilename);
        if(logFile.is_open())
        {
            dbgSetOutput(logFile);
        }
    }

    return MainExec(argv[1]) ? 0 : -1;
} 