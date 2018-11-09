#ifndef __HAJC_LANGUAGE_PARSER_H__
#define __HAJC_LANGUAGE_PARSER_H__

#include <datatypes/String.h>

#include <tools/Utils.h>
#include <tools/Memory.h>

#include <vector>

namespace HAJC
{
    struct TokenContext;

    struct ParseContext
    {
        // note that default max path length is 480.
        AString<480>               filename;
        char*                      sourceBuffer;
        size_t                     sourceLength;
        std::vector<TokenContext*> sourceTokens;

        MemPoolInternal*           mpiSmall;
        MemPoolInternal*           mpiLarge;
    };

    enum class TokenKind
    {
        TK_STRING  = 0x01,
        TK_SPECAL  = 0x02
    };
   
    struct TokenContext : public IType<TokenKind>, public IPooled
    {
        const AStringHolder& tcToken;

        virtual ~TokenContext() = default;
        TokenContext(const AStringHolder& holder) : tcToken(holder) { }

        static TokenContext* CreateTokenAssign(ParseContext* context);
        static TokenContext* CreateTokenDefine(ParseContext* context);
        static TokenContext* CreateTokenSpecal(ParseContext* context, const AStringHolder& token);
        static TokenContext* CreateTokenString(ParseContext* context, const AStringHolder& token);
    };

    struct TokenString : public TokenContext
    {
        // reserve 64 bytes.
        AString<1 << 6> tsString; 
        virtual ~TokenString() = default;
    
        explicit TokenString(const AStringHolder& token) : TokenContext(tsString) 
        {
            tsString = token;
        }
    };

    struct TokenSpecal : public TokenContext
    {
        // reserve 4 bytes.
        AString<1 << 2> tsString; 
        virtual ~TokenSpecal() = default;

        explicit TokenSpecal(const AStringHolder& token) : TokenContext(tsString) 
        {
            tsString = token;
        }
    };

    bool psCreateContext(ParseContext* context, const std::string& filename);
    void psTokenlizeContext(ParseContext* context);
    void psDeleteContext(ParseContext* context);
}

#endif