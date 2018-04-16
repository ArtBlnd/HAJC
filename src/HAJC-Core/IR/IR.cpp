#include <HAJC-Core/IR/IR.h>
#include <HAJC-Core/Utils/HAJCLog.h>

#include <string>
#include <cmath>

namespace HAJC {
    IR::IR(IBKind type, IBase* parents, const std::string& tag) :
            IBase(type, parents, CreateTagWithPrefix("ir", tag)) {
        if(type == IBKind::IB_UNKNOWN)
            NodifyLog("created IR type is unknwon! is it working correct?", NodifyLevel::WARNING);
        if(tag.empty())
            NodifyLog("created IR tag is empty! is it working correct?", NodifyLevel::WARNING);
    }

    IBase* IR::GetOperand(unsigned int i) {
        if(i >= GetNumOperand())
            NodifyLog("accessing on wrong operand position! are you sure is operand exists?", NodifyLevel::FATAL_ERROR);

        return m_operands[i];
    }

    unsigned int IR::GetNumOperand() const {
        return m_operands.size(); 
    }

    IRNormalizeLevel IR::SetNormalizeLevel(IRNormalizeLevel level) {
        if(std::abs((int)(level) - (int)(m_level)) > 1)
            NodifyLog("modifying normalize level by 2 step is dangerous! are you sure is it normalized as right way?", NodifyLevel::WARNING);
        IRNormalizeLevel oldLevel = m_level;
        m_level = level;
        return oldLevel;
    }

    IRNormalizeLevel IR::GetNormalizeLevel() const {
        return m_level;
    }

    IRType::IRType(IBase* parents) :
            IR(IBKind::IB_IR_TYPE, parents, "type") { }

    unsigned int IRType::GetTypeSize() const {
        return m_typeSize;
    }
    
    unsigned int IRType::SetTypeSize(unsigned int typeSize) {
        unsigned int oldSize = m_typeSize;
        m_typeSize = typeSize;

        return oldSize;
    }

    IRScalarType::IRScalarType(IBase* parents) :
            IR(IBKind::IB_IR_SCALAR_TYPE, parents, "scalar_type") { }

    unsigned int IRScalarType::GetTypeSize() const {
        return m_typeSize;
    }
    
    unsigned int IRScalarType::SetTypeSize(unsigned int typeSize) {
        unsigned int oldSize = m_typeSize;
        m_typeSize = typeSize;

        return oldSize;
    }

    unsigned int IRScalarType::GetScalarTypeSize() const {
        return m_typeScalarSize;
    }

    unsigned int IRScalarType::SetScalarTypeSize(unsigned int typeSize) {
        unsigned int oldSize = m_typeScalarSize;
        m_typeScalarSize = typeSize;

        return oldSize;
    }

    IRValue::IRValue(IBase* parents, IRType* type) :
            IR(IBKind::IB_IR_VALUE, parents, "value") {
        m_operands.push_back(type);
    }

    IRValue::IRValue(IBase* parents, IRType* type, IBase* initV) :
            IR(IBKind::IB_IR_VALUE, parents, "value") {
        m_operands.push_back(type);
        m_operands.push_back(initV);
    }

    IRType* IRValue::SetValueType(IRType* type) {
        IBase* oldType = m_operands[0];
        m_operands[0] = type;
        return (IRType*)oldType;
    }

    IRType* IRValue::GetValueType() {
        return (IRType*)GetOperand(0);
    }

    IBase* IRValue::SetInitValue(IBase* value) {
        IBase* oldV = m_operands[1];
        m_operands[1] = value;
        return (IRType*)oldV;
    }

    IBase* IRValue::GetInitValue() {
        return GetOperand(1);
    }

    IRArray::IRArray(IBase* parents, IRType* type, unsigned int size) :
            IR(IBKind::IB_IR_ARRAY, parents, "array") {
        m_isTypedArray = true;
    }

    IRArray::IRArray(IBase* parents, std::vector<IBase*> arrays) :
            IR(IBKind::IB_IR_ARRAY, parents, "array") {
        m_isTypedArray = false;
    }

    bool IRArray::GetIsTypedArray() const {
        return m_isTypedArray;
    }
}