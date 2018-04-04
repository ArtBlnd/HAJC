#ifndef HAJC_CORE_IR_IR_MATH_H_
#define HAJC_CORE_IR_IR_MATH_H_

#include <HAJC-Core/IR/IR.h>

namespace HAJC {
    enum class IRMathType {
        RIGHT_SHIFT, LEFT_SHIFT, SIGNED_ADD, UNSIGNED_ADD, SIGNED_SUB, UNSIGNED_SUB,
        SIGNED_MUL, UNSIGNED_MUL, SIGNED_DIV, UNSIGNED_DIV
    };

    class IRMath : public IR {
        friend class IR;

        IRMathType m_typeMath;

        // the operator will be performed with carry flag
        // if carry flag doesn't exist on target platform (or native ADC(add with cary) instruction)
        // dosn't exists, it can be lowered to if-else statements.
        bool m_isWithCarry;

        // if its true, the target objects are abstract-sized type
        // such as i256 (big integer) type.
        bool m_isVirtualType;
        
    protected:
        explicit IRMath(IBase* parents, IBase* v1, IBase* v2, IRMathType type);

        bool SetIsWithCarry(bool isWithCarry);
        bool GetIsWithCarry() const;
        bool GetIsVirtualType() const;
    };
}

#endif