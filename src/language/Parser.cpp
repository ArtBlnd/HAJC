#include <language/Parser.h>

#include <fstream>

namespace HAJC
{
    bool psCreateContext(ParseContext* context, const std::string& filename)
    {
        CONTRACT_CALL_TRACE;
        
        std::fstream file(filename, std::fstream::in);

        if(!file.is_open())
        {
            return false;
        }

        context->sourceLength = file.tellg();
        context->sourceBuffer = new char[context->sourceLength];
        context->filename     = filename;
        context->mpiLarge     = new MemPoolInternal();
        context->mpiSmall     = new MemPoolInternal();

        file.seekg(std::ios::beg);
        file.read(context->sourceBuffer, context->sourceLength);
        return true;
    }

    void psTokenlizeContext(ParseContext* context)
    {
        CONTRACT_CALL_TRACE;

        AString<256> recordedToken;
        for(size_t index = 0; index < context->sourceLength; ++index)
        {
            char c = context->sourceBuffer[index];

            if(c == '\0')
            {
                // EOF reached.
                break;
            }

            switch(c)
            {
            case '$': // Define identifier.
                if(!recordedToken.empty())
                {
                    TokenContext::CreateTokenString(context, recordedToken);
                    recordedToken.clear();
                }

                TokenContext::CreateTokenDefine(context);
                break;

            case '=': // Assign identifier.
                if(!recordedToken.empty())
                {
                    TokenContext::CreateTokenString(context, recordedToken);
                    recordedToken.clear();
                }

                TokenContext::CreateTokenAssign(context);
                break;

            case ' ': // White space identifier.
                TokenContext::CreateTokenString(context, recordedToken);
                recordedToken.clear();
                break;

            default:

                break;
            }
        }
    }

    void psDeleteContext(ParseContext* context)
    {
        delete[] context->sourceBuffer;

        delete context->mpiLarge;
        delete context->mpiSmall;

        context->mpiLarge     = nullptr;
        context->mpiSmall     = nullptr;
        context->sourceBuffer = nullptr;
        context->sourceLength = 0;
        context->filename.clear();
    }

    AString<1>  globAssignToken = "=";
    TokenContext globAssignTokenContext(globAssignToken);
    TokenContext* TokenContext::CreateTokenAssign(ParseContext* context)
    {
        context->sourceTokens.push_back(&globAssignTokenContext);
        return &globAssignTokenContext; // Statically defined.
    }

    AString<1>  globDefineToken = "$";
    TokenContext globDefineTokenContext(globDefineToken);
    TokenContext* TokenContext::CreateTokenDefine(ParseContext* context)
    {
        context->sourceTokens.push_back(&globDefineTokenContext);
        return &globDefineTokenContext; // Statically defined.
    }

    TokenContext* TokenContext::CreateTokenSpecal(ParseContext* context, const AStringHolder& token)
    {
        TokenContext* tokenContext = (TokenSpecal*) new(context->mpiSmall) TokenSpecal(token);
        context->sourceTokens.push_back(tokenContext);
        return tokenContext;
    }

    TokenContext* TokenContext::CreateTokenString(ParseContext* context, const AStringHolder& token)
    {
        TokenContext* tokenContext = (TokenString*) new(context->mpiLarge) TokenString(token);
        context->sourceTokens.push_back(tokenContext);
        return tokenContext;
    }
}