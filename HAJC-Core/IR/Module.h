#ifndef HAJC_IR_MODULE_H_
#define HAJC_IR_MODULE_H_

#include <vector>
#include <HAJC/Interface/IBase.h>
#include <HAJC-Core/IR/IR.h>

namespace HAJC {
    class Module : public IBase {
        std::vector<IBase*> m_subModules;
    };
}

#endif