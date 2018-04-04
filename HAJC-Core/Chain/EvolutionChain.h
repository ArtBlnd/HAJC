#ifndef HAJC_CORE_EVOLUTION_CHAIN_EVOLUTION_CHAIN
#define HAJC_CORE_EVOLUTION_CHAIN_EVOLUTION_CHAIN

#include <vector>

#include <HAJC-Core/Interface/IBase.h>
#include <HAJC-Core/IR/IR.h>

namespace HAJC {
    enum class ECLevel {
        VERY_LOW                                                        = 0x0000,
        LOW                                                             = 0x0001,
        MIDDLE                                                          = 0x0002,
        HIGH                                                            = 0x0003,
        VERY_HIGH                                                       = 0x0004,
        UNKNOWN                                                         = 0xFFFF
    };

    enum class ECKind { 
        ECAlinger                                                       = 0x0001,
        ECUnknown                                                       = 0xFFFF
    };

    class EvolutionChainInfo {
        friend class EvolutionChain;

        unsigned int m_szLocked;

    protected:
        std::vector<IBase*> m_evolutedIB;

        unsigned int m_szIR;
        unsigned int m_szStruct;
        unsigned int m_szArray;
        unsigned int m_szConst;

        unsigned int m_szIRLoad;
        unsigned int m_szIRStore;
        unsigned int m_szIRCast;
        unsigned int m_szIRMath;
        unsigned int m_szIRRegDef;
        unsigned int m_szIRType;
        unsigned int m_szIRValue;
    };

    class EvolutionChain {
        const ECLevel m_level;
        const ECKind m_kind;

        EvolutionChain* m_privNode;
        EvolutionChain* m_nextNode;
        EvolutionChainInfo* m_ecInfo;

    public:
        EvolutionChain(ECKind kind = ECKind::ECUnknown, ECLevel level = ECLevel::UNKNOWN);
        virtual ~EvolutionChain() = default;

        const ECLevel GetLevel();
        const ECKind GetKind();

        EvolutionChain* GetPrivNode();
        EvolutionChain* GetNextNode();

        virtual IR* ExecuteEvolutionChainAsIRNodeImpl(IR* node) = 0;
        bool ExecuteEvolutionChainAsIRNode(IR* node);  
    };
}

#endif