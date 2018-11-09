#include <compiler/Compiler.h>

namespace HAJC
{
    IMemPool* CompilerPool::GetBlockPool()
    {
        noway_assert(cpBlock == nullptr, "block memory pool not have been initialized!");
        return cpBlock;
    }

    IMemPool* CompilerPool::GetInstrPool(unsigned int cntOps)
    {
        if(MAX_INSTR_POOL_COUNT > cntOps)
        {
            // The first index of cpInstr has no operand slots
            // It will need to allocate operands dynamically if its needed.
            cntOps = 0;
        }

        return cpInstr[cntOps];
    }

    // This will initialize instruction memory pools recursively by template arguments.
    template <unsigned int opIndex = MAX_INSTR_POOL_COUNT>
    inline void InitInstrPool(IMemPool** pool)
    {
        if constexpr(opIndex != 0)
        {
            pool[opIndex - 1] = new MemPoolReusable<InstTreeOp<opIndex - 1>>();
            InitInstrPool<opIndex - 1>(pool);
        }
    }

    inline void InitBlockPool(IMemPool* (&pool))
    {
        pool = new MemPoolReusable<Block>();
    }

    Compiler::Compiler() noexcept :
        compPool(),
        compInfo()
    {
        InitInstrPool(compPool.cpInstr);
        InitBlockPool(compPool.cpBlock);
    }

    inline void ReleasePools(CompilerPool* pool)
    {
        for(unsigned int i = 0; i < MAX_INSTR_POOL_COUNT; ++i)
        {
            delete pool->cpInstr[i];
        }

        delete pool->cpBlock;
    }

    Compiler::~Compiler() noexcept
    {
        ReleasePools(&compPool);
    }

#define DEF_INST(name, namec, num, flag, maxop)                                         \
    Inst##name##* Compiler::Create##name##Inst()                                        \
    {                                                                                   \
        compInfo.ciCntInstr++;                                                          \
        return new (compPool.GetInstrPool(maxop)) Inst##name##();                       \
    }
#include <compiler/CompInstructionDefs.h>
}