#ifndef __HAJC_TOOLS_FLAG_TOOL_DEF_H__
#define __HAJC_TOOLS_FLAG_TOOL_DEF_H__

#include <cstdint>
#include <type_traits>

namespace HAJC
{

typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

template <class TyA, class TyB>
constexpr TyA AlignUp(const TyA value, const TyB align)
{
    return (value + align - 1) / align * align;
}

template <class TyA, class TyB>
constexpr TyA Offset(const TyA value, const TyB offset)
{
    return (TyA)(((size_t)value) + offset);
}

template <class TyA, class TyB>
constexpr bool HasFlag(TyA flagA, TyB flagB)
{
    return ((uint64_t)flagA & (uint64_t)flagB) == (uint64_t)flagB;
}

template <class TyA, class TyB>
constexpr void SetFlag(TyA& flagA, TyB flagB)
{
    (uint64_t)(flagA) |= (uint64_t)flagB;
}

template <class TyA, class TyB>
constexpr void DelFlag(TyA& flagA, TyB flagB)
{
    (uint64_t)(flagA) ^= (uint64_t)flagB;
}

#define ENUM_FLAG_DEF(flagName, flagBits)               \
    flagName = flagBits << flagBits


template <class Ty>
class IFlag
{
    Ty m_flag;

public:
    Ty GetFlag() const
    {
        return m_flag;
    }

    Ty SetFlag(Ty flag)
    {
        std::swap(m_flag, flag);
        return flag;
    }

    Ty AddFlag(const Ty flag)
    {
        m_flag |= flag;
        return m_flag;
    }

    Ty DelFlag(const Ty flag)
    {
        m_flag ^= flag;
        return m_flag;
    }
};

template <class Ty>
class IType
{
    Ty m_type;

public:
    Ty GetType() const
    {
        return m_type;
    }

    Ty SetType(Ty type)
    {
        std::swap(m_type, type);
        return type;
    }
};

} // namespace HAJC
#endif