#ifndef __HAJC_COMPILER_COMPILER_H__
#define __HAJC_COMPILER_COMPILER_H__

#include <tools/Memory.h>

#include <compiler/CompInstruction.h>
#include <compiler/CompBlock.h>
#include <compiler/CompTypes.h>
#include <compiler/CompilerDefs.h>

namespace HAJC
{
    struct CompilerPool
    {
        IMemPool* cpInstr[MAX_INSTR_POOL_COUNT];
        IMemPool* cpBlock;

        IMemPool* GetBlockPool();
        IMemPool* GetInstrPool(unsigned int cntOps);
    };

    struct CompilerInfo
    {
        unsigned int ciCntBlock;
        unsigned int ciCntInstr;
    };

    struct Compiler
    {
        Compiler() noexcept;
        ~Compiler() noexcept;

        CompilerPool compPool;
        CompilerInfo compInfo;

#define DEF_INST(name, namec, num, flag, maxop)                                         \
        Inst##name##* Create##name##Inst();
#include <compiler/CompInstructionDefs.h>

    }; // class Compiler

} // namespace HAJC

#endif