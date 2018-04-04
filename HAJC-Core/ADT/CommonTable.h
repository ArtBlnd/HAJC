#ifndef HAJC_CORE_ADT_COMMON_TABLE_H_
#define HAJC_CORE_ADT_COMMON_TABLE_H_

#include <exception>
#include <type_traits>

namespace HAJC {
    template <class Ty, size_t TableSize>
    class StaticTableImpl {
        Ty m_table[TableSize];

    public:
        Ty& LookUp(size_t index) {
            if(TableSize < index)
                std::exception("accessing on wrong table index!");
            return m_table[index];
        }

        Ty& operator[](size_t index) {
            return LookUp(index);
        }
    };

    template <class Ty, class EnumTy, size_t EnumRange>
    class EnumTable : private StaticTableImpl<typename Ty, EnumRange> {
        static_assert(!std::is_enum_v<EnumTy>);

    public:
        Ty& LookUp(EnumTy index) {
            return StaticTableImpl<typename Ty, EnumRange>::LookUp((size_t)index);
        }

        Ty& operator[](EnumTy index) {
            return LookUp(index);
        }
    };

    template <class Ty, size_t BaseSize>
    class DynamicTable {
    };
}

#endif