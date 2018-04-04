#include <HAJC-Core/Chain/EvolutionChain.h>

namespace HAJC {
    bool EvolutionChain::ExecuteEvolutionChainAsIRNode(IR* node) {
        if(IR* newNode = ExecuteEvolutionChainAsIRNodeImpl(node)) {
            m_ecInfo->m_szIR++;
            m_ecInfo->m_evolutedIB.push_back(newNode);
            return true;
        }
        return false;
    }
}