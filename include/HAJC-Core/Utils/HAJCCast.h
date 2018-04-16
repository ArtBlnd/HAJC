#ifndef HAJC_HAJC_CAST_H_
#define HAJC_HAJC_CAST_H_

namespace HAJC {
    template <class T1, class T2>
    T1* dyn_cast(T2* v1) {
        return dynamic_cast<T1>(v1);
    }
}

#endif