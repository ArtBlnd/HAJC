#ifndef HAJC_CORE_IR_IR_H_
#define HAJC_CORE_IR_IR_H_

#include <HAJC-Core/Interface/IBase.h>
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
        // have cpu registers, with mem2reg store, load.
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

    // IR object attribute
    // This is a IR object base, abstract instructions for generating machine-code
    // all IR should be lowered into LEVEL_3 normalization level
    class IR : public IBase {
        friend class Module;

    protected:
        std::vector<IBase*> m_operands;
        IRNormalizeLevel m_level;

        explicit IR(IBKind type, IBase* parents, const std::string& tag);

    public:
        IBase* GetOperand(unsigned int i = 0);
        unsigned int GetNumOperand() const;
        IRNormalizeLevel SetNormalizeLevel(IRNormalizeLevel level);
        IRNormalizeLevel GetNormalizeLevel() const;
        
        IR() = delete;
        virtual ~IR() = default;

        // Create IR objects will be stored itself
        // make sure that IR not be distroyed itself using delete or free

                
        static IR* CreateType(IBase* parents, unsigned int typeSize, std::string tag = "");
        static IR* CreateScalarType(IBase* parents, unsigned int typeSize, unsigned int scalarSize, 
                std::string tag = "");
        static IR* CreateValue(IBase* parents, IR* valueType, std::string tag = "");
        static IR* CreateValue(IBase* parents, IR* valueType, IBase* initV, std::string tag = "");
        static IR* CreateArray(IBase* parents, IR* type, unsigned int size);
        static IR* CreateArray(IBase* parents, std::vector<IBase*> array);
        static IR* CreateLoad(IBase* parents, IR* v1, IR* v2, std::string tag = "");
        static IR* CreateLoad(IBase* parents, IR* v1, std::vector<IR*> v2, std::string tag = "");
        static IR* CreateStore(IBase* parents, IR* v1, IR* v2, std::string atg = "");
        static IR* CreateStore(IBase* parents, IR* v1, std::vector<IR*> v2, std::string tag = "");
        static IR* CreateMove(IBase* parents, IR* v1, IR* v2, std::string tag = "");
    };
    

    /// IRType
    ///     - Attribute
    ///         define type that fixed size.
    ///
    ///     - Usage
    ///         [name] = type [size];
    /// 
    class IRType : public IR {
        friend class IR;

        unsigned int m_typeSize;

    protected:
        explicit IRType();
        explicit IRType(IBase* parents);

    public:
        unsigned int GetTypeSize() const;
        unsigned int SetTypeSize(unsigned int typeSize);
    };

    /// IRScalarType
    ///     - Attribute
    ///         define type that has scalar size
    ///         its array-like, but it can be optimized by SIMD
    ///         this is something that generally called most SIMD types like _mm256
    ///
    ///     - Usage
    ///         [name] = scalar_type [size] [scalar_size];
    ///         [name] = scalar_type [size * scalar_size];
    ///
    class IRScalarType : public IR {
        friend class IR;

        unsigned int m_typeScalarSize;
        unsigned int m_typeSize;

    protected:
        explicit IRScalarType(IBase* parents);

    public:
        unsigned int GetTypeSize() const;
        unsigned int SetTypeSize(unsigned int typeSize);
        unsigned int GetScalarTypeSize() const;
        unsigned int SetScalarTypeSize(unsigned int typeSize);
    };

    /// IRValue
    ///     - Attribute
    ///         defines new value based on pre-defined type
    ///         it will allocate memory from stack, heap, register, etc that should be
    ///         
    ///     - Usage
    ///         [name] = value [type];
    ///         [name] = value [type] [initV];
    ///
    class IRValue : public IR {
        friend class IR;

    protected:
        explicit IRValue(IBase* parents, IRType* type);
        explicit IRValue(IBase* parents, IRType* type, IBase* initV);

    public:
        IRType* SetValueType(IRType* type);
        IRType* GetValueType();
        IBase* SetInitValue(IBase* value);
        IBase* GetInitValue();
    };

    class IRArray : public IR {
        friend class IR;

        bool m_isTypedArray;

    protected:
        explicit IRArray(IBase* parents, IRType* type, unsigned int size);
        explicit IRArray(IBase* parents, std::vector<IBase*> arrays);

    public:
        bool GetIsTypedArray() const;
    };

    class IRLoad : public IR {
        friend class IR;

        bool m_isZeroExtend;

    protected:
        explicit IRLoad(IBase* parents, IBase* to, IBase* from);
        bool SetZeroExtend(bool ze);
        bool GetZeroExtend();
    };

    class IRStore : public IR {
        friend class IR;

        bool m_isZeroExtend;

    protected:
        explicit IRStore(IBase* parents, IBase* to, IBase* from);
        bool SetZeroExtend(bool ze);
        bool GetZeroExtend();
    };

    class IRMove : public IR {
        friend class IR;

        bool m_isZeroExtend;
    
    protected:
        explicit IRMove(IBase* parents, IBase* to, IBase* from);
        bool SetZeroExtend(bool ze);
        bool GetZeroExtend();
    };
}

#endif