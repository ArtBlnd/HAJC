#ifndef HAJC_INTERFACE_IBASE_H_
#define HAJC_INTERFACE_IBASE_H_

#include <string>

namespace HAJC {
    enum IBKind {
        IB_IR_TYPE                                                              = 0x0110,
        IB_IR_SCALAR_TYPE                                                       = 0x0111,
        IB_IR_VALUE                                                             = 0x0121,
        IB_IR_MATH                                                              = 0x0122,

        IB_UNKNOWN                                                              = 0xFFFF
    };

    class IBase {
        IBase* const m_parents;
        std::string m_tag;

        const IBKind m_kind;
    public:
        explicit IBase(IBKind kind, IBase* parents, const std::string& tag) 
                : m_kind(kind), m_parents(parents), m_tag(tag) { }

        IBKind GetKindOf() const { 
            return m_kind; 
        }

        const std::string& GetTag() const { 
            return m_tag; 
        }

        IBase* GetParents() const {
            return m_parents;
        }

        IBase* GetObject() const {
            return (IBase*)(this);
        }

        static bool IsIR(const IBase* ib) {
            return !!(ib->GetKindOf() & 0x0100);
        }
        static bool IsUnknown(const IBase* ib) {
            return !!(ib->GetKindOf() & 0xFFFF);
        }
    };

}

#endif