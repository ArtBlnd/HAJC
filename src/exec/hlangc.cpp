#include <language/AST.h>
#include <language/Parser.h>

#include <tools/Argument.h>
#include <tools/Debug.h>

#include <vector>
#include <string>

using namespace HAJC;

inline bool InitExec(unsigned int args, char** argv)
{
    noway_assert(argv == nullptr, "program argument cannot be nullptr!");

    if(args > 1)
    {
        std::vector<std::string> arguments;
        for(unsigned int i = 2; i < args; ++i)
        {
            arguments.push_back(std::string(argv[i]));
        }

        ArgumentParseCallback(arguments);
        return true;
    }

    return false;
}

bool MainExec(std::string filename)
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