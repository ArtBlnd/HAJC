#ifndef __HAJC_LANGUAGE_AST_H__
#define __HAJC_LANGUAGE_AST_H__

#include <tools/Memory.h>
#include <tools/Utils.h>

namespace HAJC
{
    enum class ASTKind 
    {
        ASTK_ASSIGN             = 0x01,
        ASTK_DEFINE             = 0x02,
        ASTK_SIZE_ATTRIBUTE     = 0x03,
        ASTK_PLATFORM_ATTRIBUTE = 0x04,
        ASTK_OPERATOR           = 0x05,
        ASTK_OPERANDS           = 0x06,
        ASTK_OPERAND_BEGIN      = 0x07,
        ASTK_OPERAND_END        = 0x08,
        ASTK_PREPROCESSOR       = 0x09
    };

    class ASTNode : public IType<ASTKind>, public IPooled
    {
        
    };
}

#endif