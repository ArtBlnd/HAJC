#ifndef HAJC_CORE_EVOLUTION_CHAIN_EVOLUTION_CHAIN
#define HAJC_CORE_EVOLUTION_CHAIN_EVOLUTION_CHAIN

#include <vector>

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
    }

    class EvolutionChainInfo {
        friend class EvolutionChain;

        unsigned int m_szLocked;

    protected:
        std::vector<IValue*> m_evolutedIV;

        unsigned int m_szIV;

        unsigned int m_szIR;
        unsigned int m_szStruct;
        unsigned int m_szArray;
        unsigned int m_szConst;

        unsigned int m_szIRLoad;
        unsigned int m_szIRCasting;
        unsigned int m_szIRMath;
        unsigned int m_szIRRegRef;
        unsigned int m_szIRStore;
        unsigned int m_szIRTypeDef;
        unsigned int m_szIRValue;
    };

    class EvolutionChain {
        const ECLevel m_level;
        const ECKind m_kind;

        EvolutionChain* m_privNode;
        EvolutionChain* m_nextNode;
        EvolutionChainInfo* m_ecInfo;

    public:
        EovlutionChain(ECKind kind = ECKind::ECUnknown, ECLevel level = ECLevel::UNKNOWN);
        virtual ~EvolutionChain() = default;

        const ECLevel GetLevel();
        const ECKind GetKind();

        EvolutionChain* GetPrivNode();
        EvolutionChain* GetNextNode();

        virtual bool ExecuteEvolutionChainAsIRNodeImpl(IR* node) = 0;
        virtual bool ExecuteEvolutionChainAsIVImpl(IValue* iv) = 0;

        bool ExecuteEvolutionChainAsIRNode(IR* node, bool evoluteSingleNode = false);
        bool ExecuteEvolutionChainAsIV(IValue* iv, bool evoluteSingleIV = false);
        
    };
}

#endif