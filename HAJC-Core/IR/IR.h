#ifndef HAJC_CORE_IR_IR_H_
#define HAJC_CORE_IR_IR_H_

#include <HAJC/Interface/IBase.h>
#include <vector>

namespace HAJC {
    // NOTE : Level or normalization in same IR type can be different, make sure when 
    // you are using a same level of IR.
    enum class IRNormalizeLevel {
        // LEVEL_0
        // level 0 IR node. can transform into another platform assemblies direclty
        // such as mem2mem move, basic mathmetic opcodes, etcs
        LEVEL_0                                                         = 0x0000,

        // LEVEL_1
        // level 1 IR node. can transform into another common platform assemblies that 
        // have cpu registers, with meKm2reg store, load.
        LEVEL_1                                                         = 0x0001,

        // LEVEL_2
        // level 2 IR node. will be available for same architectures. for example x86_64 
        // LEVEL_2 IR node will support for all x86_64 cpus
        LEVEL_2                                                         = 0x0002,

        // LEVEL_3
        // level 3 IR node. available for specalized cpus. such as AVX, SSE, VMX
        // instructions
        LEVEL_3                                                         = 0x0003
    };

    class IRMetadata { 
        IRNormalizeLevel m_level;

    public:
        IRNormalizeLevel GetNormalizeLevel() const;
    };

    class IR : public IBase {
        friend class Module;

    protected:
        std::vector<IBase*> m_operands;

        explicit IR(IBKind type, IBase* parents, const std::string& tag);

    public:
        IBase* GetOperand(unsigned int i = 0);
        unsigned int GetNumOperand() const;

        IR() = delete;
        virtual ~IR() = default;

        static IR* CreateType(IBase* parents, unsigned int typeSize, std::string tag = "");
        static IR* CreateScalarType(IBase* parents, unsigned int typeSize, unsigned int scalarSize, 
                std::string tag = "");
        static IR* CreateValue(IBase* parents, IR* valueType, std::string tag = "");
    };
    
    class IRType : public IR {
        unsigned int m_typeSize;

    protected:
        explicit IRType();
        explicit IRType(IBase* parents, const std::string& tag);

    public:
        unsigned int GetTypeSize() const;
        unsigned int SetTypeSize(unsigned int typeSize);
    };

    class IRScalarType : public IR {
        unsigned int m_typeScalarSize;
        unsigned int m_typeSize;

    protected:
        explicit IRScalarType(IBase* parents, const std::string& tag);

    public:
        unsigned int GetTypeSize() const;
        unsigned int SetTypeSize(unsigned int typeSize);
        unsigned int GetScalarTypeSize() const;
        unsigned int SetScalarTypeSize(unsigned int typeSize);
    };

    class IRValue : public IR { 
        IRType* m_valueType;

    public:
        IRType* GetValueType();
    };
}

#endif