#ifndef __HAJC_TOOLS_SINGLETON_H__
#define __HAJC_TOOLS_SINGLETON_H__

#include <atomic>
#include <mutex>

namespace HAJC
{
    template <class Ty>
    class Singleton
    {
        std::atomic<Ty*> instance;

        Ty* GetInst()
        {
            if (instance.load(std::memory_order_acquire) == nullptr)
            {
                instance.store(new Ty(), std::memory_order_release);
            }

            return instance;
        }

    public:
        Ty* operator->() { return  GetInst(); }
        Ty& operator* () { return *GetInst(); }

        ~Singleton()
        {
            if (Ty* instPtr = GetInst())
            {
                delete instPtr;
            }
        }
    };
}

#endif