#include <HAJC-Core/IR/IR.h>
#include <HAJC-Core/Utils/HAJCLog.h>

#include <string>

inline std::string CreateTagWithPrefix(const std::string& prefix, const std::string& tag) {
    std::string newTag;
    newTag.resize(prefix.size() + 1 + tag.size());

    newTag += prefix;
    newTag += "#";
    newTag += tag;

    return newTag;
}

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

    
}