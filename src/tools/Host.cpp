#include <tools/Host.h>
#include <tools/Debug.h>
#include <tools/Argument.h>

#include <vector>

#ifdef __HAJC_WINDOWS__
#include <Windows.h>
#endif

#define SET_IF_HAS(v, f, s) (((v & f) != 0) ? s : 0)

namespace HAJC
{

#ifdef __HAJC_WINDOWS__
    size_t hostPageSize = 0;
    size_t HostPage::GetPageSize()
    {
        if(hostPageSize == 0)
        {
            SYSTEM_INFO info;
            GetSystemInfo(&info);

            hostPageSize = info.dwPageSize;
        }

        return hostPageSize;
    }

    int MemTranslateFlags(uint64_t flags)
    {
        int memFlags = 0;
        memFlags |= SET_IF_HAS(flags, HPF_COMMIT , MEM_COMMIT  );
        memFlags |= SET_IF_HAS(flags, HPF_REERVED, MEM_RESERVE );

        return memFlags;
    }

    int PgeTranslateFlags(uint64_t flags)
    {
        int pgeFlags = 0;

        if(SET_IF_HAS(flags, HPF_EXECUTE, true))
        {
            if(SET_IF_HAS(flags, HPF_READ, true) && SET_IF_HAS(flags, HPF_WRITE, true))
            {
                pgeFlags |= PAGE_EXECUTE_READWRITE;
            }
            else
            {
                pgeFlags |= SET_IF_HAS(flags, HPF_READ , PAGE_EXECUTE_READ);
                pgeFlags |= SET_IF_HAS(flags, HPF_WRITE, PAGE_EXECUTE_WRITECOPY);
            }
        }
        else
        {
            if(SET_IF_HAS(flags, HPF_READ, true) && SET_IF_HAS(flags, HPF_WRITE, true))
            {
                pgeFlags |= PAGE_READWRITE;
            }
            else
            {
                pgeFlags |= SET_IF_HAS(flags, HPF_READ , PAGE_READONLY);
                pgeFlags |= SET_IF_HAS(flags, HPF_WRITE, PAGE_WRITECOPY);
            }
        }

        pgeFlags |= SET_IF_HAS(flags, HPF_NO_ACCESS, PAGE_NOACCESS);

        return pgeFlags;
    }

    bool HostPage::Create(HostPage* page, HostPage* base, unsigned int pages, uint64_t flags)
    {
        size_t memSize = pages * GetPageSize();
        void*  memPage;
        void*  memBase;

        if(base != nullptr)
        {
            if((base->pageSize - base->pageUsed) < memSize)
            {
                return false;
            }

            base->pageUsed += memSize;
            memBase = Offset(base->page, base->pageUsed);
        }
        else
        {
            memBase = nullptr;
        }

        const int memFlags = MemTranslateFlags(flags);
        const int pgeFlags = PgeTranslateFlags(flags);
        memPage = VirtualAlloc(memBase, memSize, memFlags, pgeFlags);
       
        if(memPage == nullptr)
        {
            return false;
        }

        page->base     = base;
        page->page     = memPage;
        page->pageSize = memSize;
        page->pageUsed = 0;

        return true;
    }

    bool HostPage::Delete(HostPage* page)
    {
        if(page->base != nullptr)
        {
            page->base->pageUsed -= page->pageSize;
            return !!VirtualFree(page->page, page->pageSize, MEM_DECOMMIT);
        }
        return !!VirtualFree(page->page, page->pageSize, MEM_RELEASE);
    }


    bool HostPage::Modify(HostPage* page, uint64_t flags)
    {
        return !!VirtualProtect(page->page, page->pageSize, PgeTranslateFlags(flags), nullptr);;
    }

#else

#endif
} // namespace HAJC