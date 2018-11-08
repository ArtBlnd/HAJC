#ifndef __HAJC_TOOLS_MEMORY_H__
#define __HAJC_TOOLS_MEMORY_H__

#include <type_traits>
#include <vector>
#include <tools/Host.h>
#include <tools/Utils.h>
#include <tools/Debug.h>

namespace HAJC
{

#if __HAJC_ENABLE_AVX2__
#include <intrin.h>
    
    template <class Ty>
    void FastZeroFill(Ty* buffer, size_t Len)
    {
        size_t size = Len * sizeof(Ty);
    
        unsigned int index = 0;
        for(; (size - index) >= 32; index += 32)
        {
            _mm_storeu_ps((float*)Offset(buffer, index), _mm_setzero_ps());
        }
    
        memset(Offset(buffer, index), 0, size % 32);
    }
#else
    template <class Ty>
    void FastZeroFill(Ty* buffer, size_t Len)
    {
        memset(buffer, 0, sizeof(Ty) * Len);
    }
#endif

    struct PageTable
    {
        unsigned int ptPageNum = 0;
        HostPage*    ptPageBeg = nullptr;
        HostPage*    ptPageEnd = nullptr;
    }; // struct PageTable

    struct IMemPool : public PageTable
    {
        uint64_t mpiUsed;
        uint64_t mpiFree;

        virtual void* allocate(size_t) = 0;
        virtual void deallocate(void* const) = 0;

        virtual ~IMemPool() = default;
    }; // struct IMemPool

    class MemPoolInternal : public IMemPool
    {
        void ReservePage() noexcept
        {
            const unsigned int newPages = 1;

            HostPage* newPage = new HostPage();
            if(HostPage::Create(newPage, nullptr, newPages, HPF_COMMIT | HPF_WRITE | HPF_READ))
            {
                ptPageEnd->nodeNext = newPage;
                ptPageEnd = newPage;
                ptPageNum += newPages;
            }
            else
            {
                noway_assert(true, "failed to create page from host-os!");
            }
        }

    public:
        void* allocate(size_t size) noexcept override
        {
            if(ptPageEnd->pageUsed + size > ptPageEnd->pageSize)
            {
                ReservePage();
            }

            mpiUsed++;
            return Offset(ptPageEnd->page, ptPageEnd->pageUsed += size);
        }

        void deallocate(void* const object) override
        {
            mpiFree++;
        }

        virtual ~MemPoolInternal() = default;
    }; // class MemPoolInternal
    
    template <class Ty>
    class MemPoolReusable : public IMemPool
    {
        std::vector<Ty*> mprPool;

        Ty* PopAndGet()
        {
            Ty* object = mprPool.back(); mprPool.pop_back();
            return object;
        }

        void ReservePage() noexcept
        {
            const unsigned int newIndex = HostPage::GetPageSize() / sizeof(Ty);
            const unsigned int newPages = 1;

            // seems pre-allocate object pools are empty.
            // we have to fill-up with page memory.

            // TODO : currently, this MemPoolReusable does not have base page allocate implements
            //        needs to implements base page allocations.
            HostPage* newPage = new HostPage();
            if(HostPage::Create(newPage, nullptr, newPages, HPF_COMMIT | HPF_WRITE | HPF_READ))
            {
                ptPageEnd->nodeNext = newPage;
                ptPageEnd = newPage;
                ptPageNum += newPages;
            }
            else
            {
                noway_assert(true, "failed to create page from host-os!");
            }

            mprPool.reserve(mprPool.capacity() + newIndex);
            mprPool.resize(newIndex);
            for(unsigned int i = 0; i < newIndex; ++i)
            {
                // TODO : vectorization based optimizations to improve fill-up speed.
                mprPool[i] = (Ty*)Offset(newPage->page, i * sizeof(Ty));
            }
        }

    public:
        void* allocate(size_t size) noexcept override
        {
            if(mprPool.empty())
            {
                // reserve another page if its empty.
                ReservePage();
            }

            // get pre-allocated object from mprPool.
            noway_assert(mprPool.empty(), "pool is empty. something seems wrong!");
            mpiUsed++;
            return PopAndGet();
        }

        void deallocate(void* const object) override
        {
            // deconstruct object.
            ((Ty*)object)->~Ty();
            mpiFree++;
            mprPool.push_back((Ty*)object);
        }

        MemPoolReusable() { ReservePage(); }
        virtual ~MemPoolReusable()
        {
            for(HostPage* pageIdx = ptPageBeg; pageIdx != nullptr; pageIdx = pageIdx->nodeNext)
            {
                HostPage::Delete(pageIdx);
            }
        }
    }; // class MemPoolReusable


    struct IPooled
    {
    protected:
        IMemPool* mpBase = nullptr;

    public:
        void* operator new(size_t size, IMemPool* pool) noexcept
        {
            noway_assert(pool == nullptr, "memory pool does not available!");
            return pool->allocate(size);
        }

        void operator delete(void* ptr, IMemPool* pool) noexcept
        {
            pool->deallocate(ptr);
        }

        void operator delete(void* ptr) noexcept
        {
            IPooled* thisPtr = (IPooled*)ptr;
            thisPtr->mpBase->deallocate(ptr);
        }
    }; // struct IPooled

} // namespace HAJC

#endif