#include <HAJC-Core/EvolutionChain/EvolutionChain.h>



bool EvolutionChain::ExecuteEvolutionChainAsIRNode(IR* node, bool evoluteSingleNode) {
        if(evoluteSingleNode)
            return ExecuteEvolutionChainAsIRNodeImpl(node);

        for(IR* currentNode = node; !currentNode->IsNodeEnd(); 
                currentNode = currentNode->GetNextNode())
            if(!ExecuteEvolutionChainAsIRNodeImpl(node))
                return false;

        return true;
}