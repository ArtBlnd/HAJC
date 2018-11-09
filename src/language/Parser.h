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
        static TokenContext* CreateTokenBracket(ParseContext* context, const AStringHolder& token);
        static TokenContext* CreateTokenUnknown(ParseContext* context, const AStringHolder& token);
    };

#define DEFINE_TOKEN(name, reserves)                                                    \
    struct Token##name## : public TokenContext                                          \
    {                                                                                   \
        AString <1 << reserves> tsString;                                               \
        virtual ~Token##name##() = default;                                             \
                                                                                        \
        explicit Token##name##(const AStringHolder& token) : TokenContext(tsString)     \
        {                                                                               \
            tsString = token;                                                           \
        }                                                                               \
    }

    DEFINE_TOKEN(Unknown, 0x01);
    DEFINE_TOKEN(String , 0x08);
    DEFINE_TOKEN(Specal , 0x01);
    DEFINE_TOKEN(Bracket, 0x01);
#undef DEFINE_TOKEN

    bool psCreateContext(ParseContext* context, const std::string& filename);
    void psTokenlizeContext(ParseContext* context);
    void psDeleteContext(ParseContext* context);
}

#endif