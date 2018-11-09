#ifndef __HAJC_COMPILER_INSTRUCTIONS_H__
#define __HAJC_COMPILER_INSTRUCTIONS_H__

#include <utility>

#include <tools/Utils.h>
#include <tools/Debug.h>
#include <tools/Memory.h>

#include <datatypes/NodeBase.h>

namespace HAJC
{

    enum class IT_FLAG : uint32_t
    {
#define IT_FLAG_DEF(flgName, flgBits) ENUM_FLAG_DEF(flgName, flgBits)

        IT_FLAG_DEF(ITF_NONE   , 0x00),
        IT_FLAG_DEF(ITF_OP_1   , 0x01),
        IT_FLAG_DEF(ITF_OP_2   , 0x02),
        IT_FLAG_DEF(ITF_OP_3   , 0x03),
        IT_FLAG_DEF(ITF_OP_N   , 0x04),
#undef IT_FLAG_DEF
    };

    enum class IT_TYPE : uint32_t
    {
        ITT_NONE = 0,
#define DEF_INST(name, namec, num, flag, maxop) ITT_##namec## = num,
#include <compiler/CompInstructionDefs.h>
    };

    // instruction class pre-defines.
#define DEF_INST(name, namec, num, flag, maxop) struct Inst##name##;
#include <compiler/CompInstructionDefs.h>

    struct InstTree : public DLRawNode<InstTree>, public IFlag<IT_FLAG>, public IType<IT_TYPE>, public IPooled
    {
        // Get operand from instruction.
        // this can be non-InstTree object. before using this method. take care that this can be non-InstTree.
        template <class RetTy = InstTree*>
        RetTy GetOp(const unsigned int opIdx);

#define DEF_INST(name, namec, num, flag, maxop)                                         \
        Inst##name##* As##name##Node() const                                            \
        {                                                                               \
            if(Is##name##Node())                                                        \
            {                                                                           \
                return (Inst##name##*)this;                                             \
            }                                                                           \
            return nullptr;                                                             \
        }
#include <compiler/CompInstructionDefs.h>

#define DEF_INST(name, namec, num, flag, maxop)                                         \
        bool Is##name##Node() const                                                     \
        {                                                                               \
            return GetType() == (IT_TYPE)num;                                           \
        }
#include <compiler/CompInstructionDefs.h>

    protected:
        explicit InstTree() = default;
        virtual ~InstTree() = default;
    }; // class InstTree


    template <unsigned int opNum>
    class InstTreeOp : public InstTree
    {
        unsigned int itoOpLen        = opNum;
        bool         itoOpLenDynamic = false;

    public:
     
        union {
            void*  OpAlloc[opNum];
            void** Op;
        }; /* UNNAMED UNION */

        // This checks that operands of this instruction has been dynamically allocated.
        // or its allocated based on SSO optimization, which is statically defined with opNum template argument.
        bool IsDynamicOp() const;

        // Get current operand length.
        unsigned int GetOpLength() const;

        // Set new operand length.
        // NOTE : this can reallocate operand dynamically. if SSO slots are not available.
        unsigned int SetOpLength(unsigned int newLen);

        InstTreeOp() = default;
        virtual ~InstTreeOp()
        {
            if(itoOpLenDynamic)
            {
                delete[] Op;
            }
        }
    }; // class InstTreeOp

#define DEF_INST(name, namec, num, flag, maxop)                                         \
    struct Inst##name## : public InstTreeOp<maxop>                                      \
    {                                                                                   \
        explicit Inst##name##()                                                         \
        {                                                                               \
            SetFlag((IT_FLAG)flag);                                                     \
            SetType((IT_TYPE)num);                                                      \
        }                                                                               \
    };
#include <compiler/CompInstructionDefs.h>

    template<class RetTy>
    inline RetTy InstTree::GetOp(const unsigned int opIdx)
    {
        InstTreeOp<0>* itOp = (InstTreeOp<0>*)this;

        noway_assert(opIdx >= itOp->GetOpLength(), "operand index is over range.");
        return (RetTy)(itOp->Op[opIdx]);
    }

    template<unsigned int opNum>
    inline bool InstTreeOp<opNum>::IsDynamicOp() const
    {
        return itoOpLenDynamic;
    }

    template<unsigned int opNum>
    inline unsigned int InstTreeOp<opNum>::GetOpLength() const
    {
        return opNum;
    }

    template<unsigned int opNum>
    inline unsigned int InstTreeOp<opNum>::SetOpLength(unsigned int newLen)
    {
        if (itoOpLen < newLen)
        {
            if (IsDynamicOp())
            {
                delete[] Op;
            }
            else
            {
                itoOpLenDynamic = true;
            }
            Op = new void*[newLen];
        }

        std::swap(itoOpLen, newLen);
        return newLen;
    }

} // namespace HAJC

#endif