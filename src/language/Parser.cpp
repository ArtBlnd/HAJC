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

        context->sourceLength = (size_t)file.tellg();
        context->sourceBuffer = new char[context->sourceLength];
        context->filename     = filename;
        context->mpiLarge     = new MemPoolInternal();
        context->mpiSmall     = new MemPoolInternal();

        file.seekg(std::ios::beg);
        file.read(context->sourceBuffer, context->sourceLength);
        return true;
    }

    inline void RecStringAndClean(AStringHolder &token, ParseContext* context)
    {
        if(!token.empty())
        {
            TokenContext::CreateTokenString(context, token);
            token.clear();
        }
    }

    bool IsAlphabet(const char c)
    {
        if(c >= 'a' && c <= 'z')
        {
            return true;
        }

        if(c >= 'A' && c <= 'Z')
        {
            return true;
        }

        return false;
    }

    void psTokenlizeContext(ParseContext* context)
    {
        CONTRACT_CALL_TRACE;

        AString<256> recordedToken;
        for(size_t index = 0; index < context->sourceLength; ++index)
        {
            char c = context->sourceBuffer[index];

            if (c == '\0')
            {
                // EOF reached.
                break;
            }

            switch(c)
            {
            case '$': // Define identifier.
                RecStringAndClean(recordedToken, context);
                TokenContext::CreateTokenDefine(context);
                break;

            case '=': // Assign identifier.
                RecStringAndClean(recordedToken, context);
                TokenContext::CreateTokenAssign(context);
                break;

            case '[': case ']':
            case '(': case ')':
            case '{': case '}':
                RecStringAndClean(recordedToken, context);
                TokenContext::CreateTokenBracket(context, AString<1>(&c));
                break;

            case ' ': // White space identifier.
                RecStringAndClean(recordedToken, context);
                break;

            default:
                if (IsAlphabet(c))
                {
                    recordedToken += c;
                    break;
                }
                    
                RecStringAndClean(recordedToken, context);
                TokenContext::CreateTokenUnknown(context, AString<1>(&c));
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

    AString<1>   globAssignToken = "=";
    TokenContext globAssignTokenContext(globAssignToken);
    TokenContext* TokenContext::CreateTokenAssign(ParseContext* context)
    {
        context->sourceTokens.push_back(&globAssignTokenContext);
        return &globAssignTokenContext; // Statically defined.
    }

    AString<1>   globDefineToken = "$";
    TokenContext globDefineTokenContext(globDefineToken);
    TokenContext* TokenContext::CreateTokenDefine(ParseContext* context)
    {
        context->sourceTokens.push_back(&globDefineTokenContext);
        return &globDefineTokenContext; // Statically defined.
    }

    TokenContext* TokenContext::CreateTokenSpecal(ParseContext* context, const AStringHolder& token)
    {
        TokenContext* tokenContext = new(context->mpiSmall) TokenSpecal(token);
        context->sourceTokens.push_back(tokenContext);
        return tokenContext;
    }

    TokenContext* TokenContext::CreateTokenString(ParseContext* context, const AStringHolder& token)
    {
        TokenContext* tokenContext = new(context->mpiLarge) TokenString(token);
        context->sourceTokens.push_back(tokenContext);
        return tokenContext;
    }

    TokenContext* TokenContext::CreateTokenBracket(ParseContext * context, const AStringHolder& token)
    {
        TokenContext* tokenContext = new(context->mpiSmall) TokenBracket(token);
        context->sourceTokens.push_back(tokenContext);
        return tokenContext;
    }

    TokenContext* TokenContext::CreateTokenUnknown(ParseContext * context, const AStringHolder& token)
    {
        TokenContext* tokenContext = new(context->mpiSmall) TokenBracket(token);
        context->sourceTokens.push_back(tokenContext);
        return tokenContext;
    }
}