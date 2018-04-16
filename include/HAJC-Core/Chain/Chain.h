#ifndef HAJC_CORE_EVOLUTION_CHAIN_EVOLUTION_CHAIN
#define HAJC_CORE_EVOLUTION_CHAIN_EVOLUTION_CHAIN

#include <vector>

#include <HAJC-Core/Interface/IBase.h>
#include <HAJC-Core/IR/IR.h>
#include <HAJC-Core/ADT/CommonTable.h>

namespace HAJC {
    enum class ChainLevel {
        VERY_LOW                                                        = 0x0000,
        LOW                                                             = 0x0001,
        MIDDLE                                                          = 0x0002,
        HIGH                                                            = 0x0003,
        VERY_HIGH                                                       = 0x0004,
        UNKNOWN                                                         = 0xFFFF
    };

    enum class ChainKind { 
        ECAlinger                                                       = 0x0001,
        ECUnknown                                                       = 0xFFFF
    };

    class ChainInfo {
        friend class ChainImpl;

        unsigned int m_szLocked;

    protected:
        std::vector<IBase*> m_passedIB;

        unsigned int m_szIR                 = 0;
        unsigned int m_szStruct             = 0;
        unsigned int m_szArray              = 0;
        unsigned int m_szConst              = 0;

        unsigned int m_szIRLoad             = 0;
        unsigned int m_szIRStore            = 0;
        unsigned int m_szIRCast             = 0;
        unsigned int m_szIRMath             = 0;
        unsigned int m_szIRRegDef           = 0;
        unsigned int m_szIRType             = 0;
        unsigned int m_szIRValue            = 0;
    };

    class ChainImpl {
        const ChainLevel m_level;
        const ChainKind m_kind;

        // ir that true on this table will not be passed into chain
        // it means it will just be ignored
        IRTypeTable<bool> m_tableIgnoreEvolute;

        // this will be collect all the evoluted IRs
        // to debug or check, or something else to check that is evoluted.
        // note that this can be shared
        ChainInfo* m_chainInfo;

        // basically the m_ecInfo isn't shared
        // set as m_isInfoShared true if its shared, so it will not call delete operator 
        // when it is distructing
        bool m_isInfoShared = false;

    public:
        ChainImpl(ChainKind kind = ChainKind::ECUnknown, ChainLevel level = ChainLevel::UNKNOWN);
        virtual ~ChainImpl() = default;

        const ChainLevel GetLevel();
        const ChainKind GetKind();
    };

    class ChainAlingerImpl : public ChainImpl {
        unsigned int m_szAlign;

        // force to modify the target node
        // some nodes are locked, it cannot be evoluted
        // this will force target to be modified
        bool m_isForced;

    public:
        explicit ChainAlingerImpl(unsigned int szAlign, ChainLevel level = ChainLevel::HIGH);
        ~ChainAlingerImpl();
    };
}

#endif